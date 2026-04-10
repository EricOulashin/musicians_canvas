#include "portaudiorecorder.h"

#include <QObject>
#include <QtGlobal>
#include <algorithm>
#include <cmath>

#ifndef HAVE_PORTAUDIO

bool PortAudioRecorder::isCompiledWithPortAudio() { return false; }
bool PortAudioRecorder::initializeLibrary(QString*) { return false; }
void PortAudioRecorder::terminateLibrary() {}
bool PortAudioRecorder::hasInputDevice() { return false; }
int PortAudioRecorder::defaultInputDeviceIndex() { return -1; }
int PortAudioRecorder::deviceCount() { return 0; }
QString PortAudioRecorder::deviceName(int) { return {}; }
int PortAudioRecorder::maxInputChannels(int) { return 0; }
PortAudioRecorder::PortAudioRecorder() = default;
PortAudioRecorder::~PortAudioRecorder() { stop(); }
bool PortAudioRecorder::start(int, int, int, QString& errorMessage)
{
    errorMessage = QObject::tr("PortAudio was not enabled in this build.");
    return false;
}
void PortAudioRecorder::stop() {}
QByteArray PortAudioRecorder::takeRecordedPcm() { return {}; }
qint64 PortAudioRecorder::processedMicroseconds() const { return 0; }
float PortAudioRecorder::recentPeak() const { return 0.0f; }

void PortAudioRecorder::setMonitorTap(std::function<void(const char*, int)> /*fn*/) {}
void PortAudioRecorder::clearMonitorTap() {}

#else

#    include <portaudio.h>

#    include <mutex>

namespace
{
std::mutex s_paMutex;
int        s_paInitCount = 0;

static bool formatIsOkForInput(const PaStreamParameters& in, double sampleRate)
{
    const PaError err = Pa_IsFormatSupported(&in, nullptr, sampleRate);
    return err == paFormatIsSupported;
}
}  // namespace

bool PortAudioRecorder::isCompiledWithPortAudio()
{
    return true;
}

bool PortAudioRecorder::initializeLibrary(QString* errorMessage)
{
    std::lock_guard<std::mutex> lock(s_paMutex);
    if (s_paInitCount == 0)
    {
        const PaError err = Pa_Initialize();
        if (err != paNoError)
        {
            if (errorMessage)
                *errorMessage = QString::fromUtf8(Pa_GetErrorText(err));
            return false;
        }
    }
    ++s_paInitCount;
    return true;
}

void PortAudioRecorder::terminateLibrary()
{
    std::lock_guard<std::mutex> lock(s_paMutex);
    if (s_paInitCount > 0)
    {
        --s_paInitCount;
        if (s_paInitCount == 0)
            static_cast<void>(Pa_Terminate());
    }
}

bool PortAudioRecorder::hasInputDevice()
{
    if (Pa_GetDeviceCount() < 1)
        return false;
    return Pa_GetDefaultInputDevice() != paNoDevice;
}

int PortAudioRecorder::defaultInputDeviceIndex()
{
    return Pa_GetDefaultInputDevice();
}

int PortAudioRecorder::deviceCount()
{
    return Pa_GetDeviceCount();
}

QString PortAudioRecorder::deviceName(int paDeviceIndex)
{
    if (paDeviceIndex < 0 || paDeviceIndex >= Pa_GetDeviceCount())
        return {};
    const PaDeviceInfo* info = Pa_GetDeviceInfo(paDeviceIndex);
    if (info == nullptr || info->name == nullptr)
        return {};
    return QString::fromUtf8(info->name);
}

int PortAudioRecorder::maxInputChannels(int paDeviceIndex)
{
    if (paDeviceIndex < 0 || paDeviceIndex >= Pa_GetDeviceCount())
        return 0;
    const PaDeviceInfo* info = Pa_GetDeviceInfo(paDeviceIndex);
    return info ? info->maxInputChannels : 0;
}

int PortAudioRecorder::paCallback(const void* input, void* /*output*/, unsigned long frameCount,
                                  const PaStreamCallbackTimeInfo* /*timeInfo*/,
                                  PaStreamCallbackFlags /*statusFlags*/, void* userData)
{
    auto* self = static_cast<PortAudioRecorder*>(userData);
    if (input != nullptr && frameCount > 0 && self->m_bytesPerFrame > 0)
    {
        const int nBytes = static_cast<int>(frameCount) * self->m_bytesPerFrame;
        const char* pcm = static_cast<const char*>(input);
        std::function<void(const char*, int)> tap;
        {
            std::lock_guard<std::mutex> lock(self->m_mutex);
            self->m_buffer.append(pcm, nBytes);
            self->m_totalFrames += static_cast<qint64>(frameCount);
        }
        {
            std::lock_guard<std::mutex> lock(self->m_monitorMutex);
            tap = self->m_monitorTap;
        }
        if (tap)
            tap(pcm, nBytes);
    }
    return paContinue;
}

PortAudioRecorder::PortAudioRecorder() = default;

PortAudioRecorder::~PortAudioRecorder()
{
    stop();
}

bool PortAudioRecorder::start(int paDeviceIndex, int channelCount, int preferredSampleRate,
                              QString& errorMessage)
{
    stop();

    if (Pa_GetDeviceCount() < 1)
    {
        errorMessage = QObject::tr("No PortAudio devices available.");
        return false;
    }

    int dev = paDeviceIndex;
    if (dev < 0)
        dev = Pa_GetDefaultInputDevice();
    if (dev == paNoDevice)
    {
        errorMessage = QObject::tr("No default PortAudio input device.");
        return false;
    }

    const PaDeviceInfo* devInfo = Pa_GetDeviceInfo(dev);
    if (devInfo == nullptr)
    {
        errorMessage = QObject::tr("Invalid PortAudio device index.");
        return false;
    }

    const int ch = std::clamp(channelCount, 1, std::max(1, devInfo->maxInputChannels));
    if (ch > devInfo->maxInputChannels)
    {
        errorMessage = QObject::tr("Device does not support %1 input channel(s).").arg(channelCount);
        return false;
    }

    PaStreamParameters in{};
    in.device                    = dev;
    in.channelCount              = ch;
    in.sampleFormat              = paInt16;
    in.suggestedLatency          = devInfo->defaultLowInputLatency;
    in.hostApiSpecificStreamInfo = nullptr;

    m_streamChannels = ch;
    m_bytesPerFrame  = ch * static_cast<int>(sizeof(qint16));

    const double native = devInfo->defaultSampleRate > 0 ? devInfo->defaultSampleRate : 44100.0;
    const double want   = preferredSampleRate > 0 ? static_cast<double>(preferredSampleRate) : native;

    static const int kFallbackRates[] = {
        48000, 44100, 96000, 88200, 64000, 48000, 32000, 24000, 22050, 192000, 176400, 16000,
        12000, 11025, 8000,
    };

    PaStream* stream   = nullptr;
    double    openedRate = 0;

    auto tryOpen = [&](double rate) -> bool
    {
        if (rate <= 0.0)
            return false;
        if (!formatIsOkForInput(in, rate))
            return false;
        PaStream* tryStream = nullptr;
        const PaError openErr =
            Pa_OpenStream(&tryStream, &in, nullptr, rate, paFramesPerBufferUnspecified, paClipOff,
                          &PortAudioRecorder::paCallback, this);
        if (openErr != paNoError || tryStream == nullptr)
        {
            if (tryStream)
                static_cast<void>(Pa_CloseStream(tryStream));
            return false;
        }
        stream     = tryStream;
        openedRate = rate;
        return true;
    };

    if (!tryOpen(want) && std::abs(want - native) > 1.0)
        static_cast<void>(tryOpen(native));
    if (stream == nullptr)
    {
        for (int r : kFallbackRates)
        {
            if (tryOpen(static_cast<double>(r)))
                break;
        }
    }

    if (stream == nullptr)
    {
        errorMessage =
            QObject::tr("Could not open PortAudio input at a supported sample rate for this device.");
        return false;
    }

    m_stream = stream;
    Q_UNUSED(openedRate);

    const PaStreamInfo* sInfo = Pa_GetStreamInfo(stream);
    m_streamSampleRate =
        sInfo ? static_cast<int>(std::lround(sInfo->sampleRate))
              : static_cast<int>(std::lround(native));

    m_totalFrames = 0;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_buffer.clear();
    }

    const PaError startErr = Pa_StartStream(stream);
    if (startErr != paNoError)
    {
        static_cast<void>(Pa_CloseStream(stream));
        m_stream = nullptr;
        errorMessage = QString::fromUtf8(Pa_GetErrorText(startErr));
        return false;
    }

    return true;
}

void PortAudioRecorder::stop()
{
    if (m_stream == nullptr)
        return;
    auto* s = static_cast<PaStream*>(m_stream);
    if (Pa_IsStreamActive(s) == 1)
        static_cast<void>(Pa_StopStream(s));
    static_cast<void>(Pa_CloseStream(s));
    m_stream = nullptr;
    clearMonitorTap();
}

QByteArray PortAudioRecorder::takeRecordedPcm()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return std::move(m_buffer);
}

qint64 PortAudioRecorder::processedMicroseconds() const
{
    if (m_streamSampleRate <= 0)
        return 0;
    return static_cast<qint64>(std::llround(1e6 * static_cast<double>(m_totalFrames)
                                            / static_cast<double>(m_streamSampleRate)));
}

float PortAudioRecorder::recentPeak() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_buffer.isEmpty() || m_streamChannels <= 0)
        return 0.0f;
    const int bytesPerFrame = m_streamChannels * static_cast<int>(sizeof(qint16));
    const int windowBytes   = qMin(m_buffer.size(), bytesPerFrame * 2048);
    const auto* s =
        reinterpret_cast<const qint16*>(m_buffer.constData() + m_buffer.size() - windowBytes);
    const int n = windowBytes / static_cast<int>(sizeof(qint16));
    float     peak = 0.0f;
    for (int i = 0; i < n; ++i)
        peak = std::max(peak, std::abs(s[i] / 32768.0f));
    return std::min(peak, 1.0f);
}

void PortAudioRecorder::setMonitorTap(std::function<void(const char*, int)> fn)
{
    std::lock_guard<std::mutex> lock(m_monitorMutex);
    m_monitorTap = std::move(fn);
}

void PortAudioRecorder::clearMonitorTap()
{
    std::lock_guard<std::mutex> lock(m_monitorMutex);
    m_monitorTap = {};
}

#endif  // HAVE_PORTAUDIO
