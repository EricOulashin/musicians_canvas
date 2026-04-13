#include "midifile.h"
#include <QFile>
#include <QByteArray>
#include <QDebug>
#include <algorithm>
#include <cmath>
#include <cstdint>

namespace MidiFile {

// ─── Write ────────────────────────────────────────────────────────────────

namespace {

// Variable-length quantity encoding used throughout the SMF event stream.
// Encodes a non-negative integer as 7-bit groups, MSB set on every byte
// except the last.  This is the format used for delta-times and meta-event
// length fields.
QByteArray encodeVlq(quint32 value)
{
    QByteArray out;
    out.prepend(static_cast<char>(value & 0x7F));
    value >>= 7;
    while (value > 0)
    {
        out.prepend(static_cast<char>((value & 0x7F) | 0x80));
        value >>= 7;
    }
    return out;
}

// Big-endian writers
void writeBE16(QByteArray& dst, quint16 v)
{
    dst.append(static_cast<char>((v >> 8) & 0xFF));
    dst.append(static_cast<char>(v & 0xFF));
}
void writeBE32(QByteArray& dst, quint32 v)
{
    dst.append(static_cast<char>((v >> 24) & 0xFF));
    dst.append(static_cast<char>((v >> 16) & 0xFF));
    dst.append(static_cast<char>((v >>  8) & 0xFF));
    dst.append(static_cast<char>(v & 0xFF));
}

constexpr quint16 kPpq        = 1000;          // 1000 ticks per quarter note
constexpr quint32 kTempoUsPq  = 1'000'000;     // 60 BPM => 1 tick = 1 ms

quint32 secondsToTicks(double seconds)
{
    if (seconds < 0) seconds = 0;
    return static_cast<quint32>(std::llround(seconds * 1000.0));
}

} // anonymous namespace

bool write(const QString& path,
           const QVector<MidiNote>& notes,
           double lengthSeconds,
           const QVector<MidiControlChange>& controlChanges)
{
    // Build a sorted event list (note-on, note-off, CC) by absolute tick
    struct Event
    {
        quint32 tick;
        quint8  status;
        quint8  data1;
        quint8  data2;
    };
    QVector<Event> events;
    events.reserve(notes.size() * 2 + controlChanges.size() + 1);

    for (const auto& n : notes)
    {
        const int ch = (n.channel >= 0 && n.channel <= 15) ? n.channel : 0;
        const quint32 onTick  = secondsToTicks(n.startTime);
        const quint32 offTick = secondsToTicks(n.startTime + n.duration);
        events.push_back({ onTick,
                           static_cast<quint8>(0x90 | ch),
                           static_cast<quint8>(n.note & 0x7F),
                           static_cast<quint8>(n.velocity & 0x7F) });
        events.push_back({ offTick,
                           static_cast<quint8>(0x80 | ch),
                           static_cast<quint8>(n.note & 0x7F),
                           static_cast<quint8>(0) });
    }
    for (const auto& cc : controlChanges)
    {
        const int ch = (cc.channel >= 0 && cc.channel <= 15) ? cc.channel : 0;
        events.push_back({ secondsToTicks(cc.timeSec),
                           static_cast<quint8>(0xB0 | ch),
                           static_cast<quint8>(cc.controller & 0x7F),
                           static_cast<quint8>(cc.value & 0x7F) });
    }

    // Stable sort by tick — when two events have the same tick we keep the
    // original ordering (typically note-off before next note-on for the
    // same note number).
    std::stable_sort(events.begin(), events.end(),
        [](const Event& a, const Event& b) { return a.tick < b.tick; });

    // ── MTrk body ────────────────────────────────────────────────────
    QByteArray track;

    // Tempo meta event at tick 0: FF 51 03 tt tt tt
    track.append(encodeVlq(0));            // delta time = 0
    track.append(static_cast<char>(0xFF));
    track.append(static_cast<char>(0x51));
    track.append(encodeVlq(3));            // length = 3
    track.append(static_cast<char>((kTempoUsPq >> 16) & 0xFF));
    track.append(static_cast<char>((kTempoUsPq >>  8) & 0xFF));
    track.append(static_cast<char>(kTempoUsPq        & 0xFF));

    // Note events with delta times
    quint32 lastTick = 0;
    for (const auto& e : events)
    {
        const quint32 dt = e.tick - lastTick;
        track.append(encodeVlq(dt));
        track.append(static_cast<char>(e.status));
        track.append(static_cast<char>(e.data1));
        track.append(static_cast<char>(e.data2));
        lastTick = e.tick;
    }

    // End of track at lengthSeconds (or at the last event tick, whichever
    // is later).  This preserves the recorded length even when the last
    // note ends earlier than the recording stop point.
    quint32 eotTick = secondsToTicks(lengthSeconds);
    if (eotTick < lastTick) eotTick = lastTick;
    const quint32 eotDelta = eotTick - lastTick;
    track.append(encodeVlq(eotDelta));
    track.append(static_cast<char>(0xFF));
    track.append(static_cast<char>(0x2F));
    track.append(static_cast<char>(0x00));

    // ── MThd header ──────────────────────────────────────────────────
    QByteArray header;
    header.append("MThd", 4);
    writeBE32(header, 6);          // header length
    writeBE16(header, 0);          // format 0 (single track)
    writeBE16(header, 1);          // 1 track
    writeBE16(header, kPpq);       // ticks per quarter note

    // ── MTrk header ──────────────────────────────────────────────────
    QByteArray trackHeader;
    trackHeader.append("MTrk", 4);
    writeBE32(trackHeader, static_cast<quint32>(track.size()));

    // ── Write the file ───────────────────────────────────────────────
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly))
    {
        qWarning() << "MidiFile::write: could not open" << path;
        return false;
    }
    f.write(header);
    f.write(trackHeader);
    f.write(track);
    f.close();
    return true;
}

// ─── Read ─────────────────────────────────────────────────────────────────

namespace {

// Helper to consume a variable-length quantity from a buffer position
quint32 readVlq(const QByteArray& buf, int& pos)
{
    quint32 value = 0;
    while (pos < buf.size())
    {
        const quint8 byte = static_cast<quint8>(buf[pos++]);
        value = (value << 7) | (byte & 0x7F);
        if ((byte & 0x80) == 0) break;
    }
    return value;
}

quint16 readBE16(const QByteArray& buf, int pos)
{
    return (static_cast<quint16>(static_cast<quint8>(buf[pos])) << 8)
         |  static_cast<quint16>(static_cast<quint8>(buf[pos + 1]));
}
quint32 readBE32(const QByteArray& buf, int pos)
{
    return (static_cast<quint32>(static_cast<quint8>(buf[pos    ])) << 24)
         | (static_cast<quint32>(static_cast<quint8>(buf[pos + 1])) << 16)
         | (static_cast<quint32>(static_cast<quint8>(buf[pos + 2])) <<  8)
         |  static_cast<quint32>(static_cast<quint8>(buf[pos + 3]));
}

} // anonymous namespace

QVector<MidiNote> read(const QString& path, double* outLengthSeconds,
                       QVector<MidiControlChange>* outControlChanges)
{
    QVector<MidiNote> result;
    if (outLengthSeconds) *outLengthSeconds = 0.0;
    if (outControlChanges) outControlChanges->clear();

    QFile f(path);
    if (!f.open(QIODevice::ReadOnly))
    {
        qWarning() << "MidiFile::read: could not open" << path;
        return result;
    }
    const QByteArray buf = f.readAll();
    f.close();

    if (buf.size() < 14 || buf.left(4) != QByteArray("MThd", 4))
    {
        qWarning() << "MidiFile::read: not a SMF file:" << path;
        return result;
    }

    // Header: format (2), ntracks (2), division (2)
    const quint16 ntracks  = readBE16(buf, 10);
    const quint16 division = readBE16(buf, 12);

    // Determine ticks-per-quarter (we don't support SMPTE timing for read).
    quint16 ppq = (division & 0x8000) ? 480 : division;
    if (ppq == 0) ppq = 480;

    // Default tempo: 500,000 µs per quarter note (= 120 BPM) until a tempo
    // meta event is found.  Tempo can change mid-track in standard SMF.
    double secPerTick = (500000.0 / 1'000'000.0) / ppq;

    // For matching note-on/off pairs across all tracks: keyed by
    // (channel * 128 + note) → (start time in seconds, velocity).
    struct Active { double start; int velocity; };
    QVector<Active> active(16 * 128, { -1.0, 0 });

    double maxEndTime = 0.0;
    double trackEndSeconds = 0.0;

    int pos = 14;
    for (int trackIdx = 0; trackIdx < ntracks; ++trackIdx)
    {
        if (pos + 8 > buf.size()) break;
        if (buf.mid(pos, 4) != QByteArray("MTrk", 4)) break;
        const quint32 tlen = readBE32(buf, pos + 4);
        const int trackStart = pos + 8;
        const int trackEnd   = trackStart + static_cast<int>(tlen);
        pos = trackEnd;

        int p = trackStart;
        quint32 absTick = 0;
        quint8  runningStatus = 0;
        double  trackTimeSec = 0.0;

        while (p < trackEnd)
        {
            const quint32 dt = readVlq(buf, p);
            absTick += dt;
            trackTimeSec += dt * secPerTick;
            if (p >= trackEnd) break;

            quint8 status = static_cast<quint8>(buf[p]);
            if (status & 0x80) { p++; runningStatus = status; }
            else               { status = runningStatus; }

            if (status == 0xFF)
            {
                // Meta event
                const quint8 metaType = static_cast<quint8>(buf[p++]);
                const quint32 mlen = readVlq(buf, p);
                if (metaType == 0x51 && mlen == 3 && p + 3 <= trackEnd)
                {
                    // Set tempo: 24-bit big-endian µs per quarter note
                    const quint32 us =
                        (static_cast<quint32>(static_cast<quint8>(buf[p    ])) << 16) |
                        (static_cast<quint32>(static_cast<quint8>(buf[p + 1])) <<  8) |
                         static_cast<quint32>(static_cast<quint8>(buf[p + 2]));
                    secPerTick = (us / 1'000'000.0) / ppq;
                }
                else if (metaType == 0x2F)
                {
                    // End of track
                    if (trackTimeSec > trackEndSeconds)
                        trackEndSeconds = trackTimeSec;
                }
                p += static_cast<int>(mlen);
            }
            else if (status == 0xF0 || status == 0xF7)
            {
                // System exclusive — skip
                const quint32 slen = readVlq(buf, p);
                p += static_cast<int>(slen);
            }
            else
            {
                const quint8 kind = status & 0xF0;
                const quint8 ch   = status & 0x0F;
                if (kind == 0x90 || kind == 0x80
                    || kind == 0xA0 || kind == 0xB0 || kind == 0xE0)
                {
                    // 2-byte channel messages: noteoff, noteon, polyphonic
                    // aftertouch, control change, pitch bend
                    if (p + 2 > trackEnd) break;
                    const quint8 d1 = static_cast<quint8>(buf[p++]);
                    const quint8 d2 = static_cast<quint8>(buf[p++]);
                    if (kind == 0x90 && d2 > 0)
                    {
                        // Note on
                        const int slot = ch * 128 + d1;
                        if (active[slot].start >= 0.0)
                        {
                            // Implicit note off (legato re-trigger)
                            MidiNote n;
                            n.note      = d1;
                            n.velocity  = active[slot].velocity;
                            n.channel   = ch;
                            n.startTime = active[slot].start;
                            n.duration  = trackTimeSec - active[slot].start;
                            if (n.duration < 0.001) n.duration = 0.001;
                            result.append(n);
                        }
                        active[slot] = { trackTimeSec, d2 };
                    }
                    else if (kind == 0x80 || (kind == 0x90 && d2 == 0))
                    {
                        // Note off (or note-on velocity 0)
                        const int slot = ch * 128 + d1;
                        if (active[slot].start >= 0.0)
                        {
                            MidiNote n;
                            n.note      = d1;
                            n.velocity  = active[slot].velocity;
                            n.channel   = ch;
                            n.startTime = active[slot].start;
                            n.duration  = trackTimeSec - active[slot].start;
                            if (n.duration < 0.001) n.duration = 0.001;
                            result.append(n);
                            const double end = n.startTime + n.duration;
                            if (end > maxEndTime) maxEndTime = end;
                            active[slot] = { -1.0, 0 };
                        }
                    }
                    else if (kind == 0xB0 && outControlChanges)
                    {
                        MidiControlChange cc;
                        cc.timeSec = trackTimeSec;
                        cc.channel = ch;
                        cc.controller = d1;
                        cc.value = d2;
                        outControlChanges->append(cc);
                    }
                }
                else if (kind == 0xC0 || kind == 0xD0)
                {
                    // 1-byte channel messages: program change, channel pressure
                    if (p + 1 > trackEnd) break;
                    p++;
                }
                else
                {
                    // Unknown — best-effort skip
                    p++;
                }
            }
        }

        // Close any still-open notes at end of track
        for (int slot = 0; slot < 16 * 128; ++slot)
        {
            if (active[slot].start >= 0.0)
            {
                MidiNote n;
                n.note      = slot % 128;
                n.channel   = slot / 128;
                n.velocity  = active[slot].velocity;
                n.startTime = active[slot].start;
                n.duration  = trackTimeSec - active[slot].start;
                if (n.duration < 0.001) n.duration = 0.001;
                result.append(n);
                const double end = n.startTime + n.duration;
                if (end > maxEndTime) maxEndTime = end;
                active[slot] = { -1.0, 0 };
            }
        }
    }

    // Sort the merged note list by start time so playback order matches
    // the original recording order.
    std::sort(result.begin(), result.end(),
        [](const MidiNote& a, const MidiNote& b) { return a.startTime < b.startTime; });

    if (outLengthSeconds)
        *outLengthSeconds = std::max(maxEndTime, trackEndSeconds);
    return result;
}

} // namespace MidiFile
