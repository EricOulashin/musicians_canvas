#include "projectsettingsdialog.h"
#include <QTabWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QApplication>
#ifdef QT_MULTIMEDIA_AVAILABLE
#include <QMediaDevices>
#include <QAudioDevice>
#include <QAudioSource>
#include <QBuffer>
#include <QElapsedTimer>
#include <QEventLoop>
#include <QTimer>
#endif
#include <RtMidi.h>
#include <string>
#include <cmath>
#if defined(HAVE_PORTAUDIO)
#include "portaudiorecorder.h"
#endif

using std::string;

// Standard sample rates offered in the UI
static const int s_stdRates[] = {
    8000, 11025, 16000, 22050, 32000, 44100, 48000, 88200, 96000, 176400, 192000
};

ProjectSettingsDialog::ProjectSettingsDialog(const ProjectSettings& settings,
                                             QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Project Settings"));
    setMinimumSize(450, 300);
    resize(500, 340);

    auto* layout = new QVBoxLayout(this);

    m_tabWidget = new QTabWidget();
    setupMidiTab();
    setupAudioTab();
    layout->addWidget(m_tabWidget);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok |
                                         QDialogButtonBox::Cancel);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttons);

    loadSettings(settings);
}

ProjectSettings ProjectSettingsDialog::projectSettings() const
{
    ProjectSettings s;
    s.midiDeviceIndex     = m_midiDeviceCombo->currentData().toInt();
    if (m_midiInputCombo)
        s.midiInputPortName = m_midiInputCombo->currentData().toString();
    s.soundFontPath       = m_soundFontEdit->text().trimmed();
    s.audioInputDeviceId  = m_audioInputCombo->currentData().toByteArray();
    s.audioOutputDeviceId = m_audioOutputCombo->currentData().toByteArray();
    s.sampleRate          = m_sampleRateCombo->currentData().toInt();
    s.channelCount        = m_monoRadio->isChecked() ? 1 : 2;
#if defined(HAVE_PORTAUDIO)
    s.useQtAudioInput              = m_recordingUseQtRadio->isChecked();
    s.portAudioInputDeviceIndex    = m_portAudioDeviceCombo->currentData().toInt();
#else
    s.useQtAudioInput              = true;
    s.portAudioInputDeviceIndex    = -1;
#endif
    return s;
}

void ProjectSettingsDialog::setupMidiTab()
{
    auto* widget = new QWidget();
    auto* layout = new QVBoxLayout(widget);

    auto* midiGroup = new QGroupBox(tr("MIDI Output Device"));
    auto* midiLayout = new QVBoxLayout(midiGroup);
    m_midiDeviceCombo = new QComboBox();
    m_midiDeviceCombo->setMinimumWidth(300);
    midiLayout->addWidget(m_midiDeviceCombo);
    layout->addWidget(midiGroup);

    // MIDI input device — used to record MIDI tracks
    auto* midiInGroup = new QGroupBox(tr("MIDI Input Device (for recording MIDI tracks)"));
    auto* midiInLayout = new QVBoxLayout(midiInGroup);
    m_midiInputCombo = new QComboBox();
    m_midiInputCombo->setMinimumWidth(300);
    midiInLayout->addWidget(m_midiInputCombo);
    layout->addWidget(midiInGroup);

    auto* refreshBtn = new QPushButton(tr("Refresh"));
    connect(refreshBtn, &QPushButton::clicked,
            this, &ProjectSettingsDialog::refreshDevices);
    layout->addWidget(refreshBtn);

    auto* sfGroup = new QGroupBox(tr("SoundFont (for MIDI synthesis)"));
    auto* sfLayout = new QHBoxLayout(sfGroup);
    m_soundFontEdit = new QLineEdit();
    m_soundFontEdit->setPlaceholderText(tr("Path to .sf2 SoundFont file (empty = app default)"));
    sfLayout->addWidget(m_soundFontEdit);

    auto* browseBtn = new QPushButton(tr("Browse..."));
    connect(browseBtn, &QPushButton::clicked,
            this, &ProjectSettingsDialog::onBrowseSoundFont);
    sfLayout->addWidget(browseBtn);
    layout->addWidget(sfGroup);

    layout->addStretch();
    m_tabWidget->addTab(widget, tr("MIDI"));
}

void ProjectSettingsDialog::setupAudioTab()
{
    auto* widget = new QWidget();
    auto* layout = new QVBoxLayout(widget);

    // --- Device selection ---
    auto* inputGroup = new QGroupBox(tr("Audio Input Device"));
    auto* inputLayout = new QVBoxLayout(inputGroup);
    m_audioInputCombo = new QComboBox();
    m_audioInputCombo->setMinimumWidth(300);
    inputLayout->addWidget(m_audioInputCombo);
    layout->addWidget(inputGroup);

#if defined(HAVE_PORTAUDIO)
    {
        auto* capGroup  = new QGroupBox(tr("Recording capture"));
        auto* capLayout = new QVBoxLayout(capGroup);
        m_recordingUsePortAudioRadio =
            new QRadioButton(tr("PortAudio (native input, recommended when built-in)"));
        m_recordingUseQtRadio = new QRadioButton(tr("Qt Multimedia"));
        auto* capBackendGroup = new QButtonGroup(this);
        capBackendGroup->addButton(m_recordingUsePortAudioRadio);
        capBackendGroup->addButton(m_recordingUseQtRadio);
        m_recordingUsePortAudioRadio->setChecked(true);
        capLayout->addWidget(m_recordingUsePortAudioRadio);
        capLayout->addWidget(m_recordingUseQtRadio);
        capLayout->addWidget(new QLabel(tr("PortAudio input device:")));
        m_portAudioDeviceCombo = new QComboBox();
        m_portAudioDeviceCombo->setMinimumWidth(300);
        capLayout->addWidget(m_portAudioDeviceCombo);
        connect(m_recordingUsePortAudioRadio, &QRadioButton::toggled, this,
                &ProjectSettingsDialog::onRecordingBackendChanged);
        connect(m_recordingUseQtRadio, &QRadioButton::toggled, this,
                &ProjectSettingsDialog::onRecordingBackendChanged);
        layout->addWidget(capGroup);
    }
#endif

    // Re-populate format options when the input device changes
    connect(m_audioInputCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ProjectSettingsDialog::updateAudioFormatOptions);

    auto* outputGroup = new QGroupBox(tr("Audio Output Device"));
    auto* outputLayout = new QVBoxLayout(outputGroup);
    m_audioOutputCombo = new QComboBox();
    m_audioOutputCombo->setMinimumWidth(300);
    outputLayout->addWidget(m_audioOutputCombo);
    layout->addWidget(outputGroup);

    // --- Audio format (applies to all audio tracks in this project) ---
    auto* fmtGroup = new QGroupBox(tr("Audio Format (applies to all audio tracks)"));
    auto* fmtLayout = new QVBoxLayout(fmtGroup);

    // Sample rate — populated dynamically from the selected input device
    auto* srRow = new QHBoxLayout();
    srRow->addWidget(new QLabel(tr("Sample rate:")));
    m_sampleRateCombo = new QComboBox();
    srRow->addWidget(m_sampleRateCombo);
    srRow->addStretch();
    fmtLayout->addLayout(srRow);

    // Channels
    auto* chRow = new QHBoxLayout();
    chRow->addWidget(new QLabel(tr("Channels:")));
    m_monoRadio   = new QRadioButton(tr("Mono"));
    m_stereoRadio = new QRadioButton(tr("Stereo"));
    m_stereoRadio->setChecked(true);  // default stereo
    auto* chGroup = new QButtonGroup(this);
    chGroup->addButton(m_monoRadio,   1);
    chGroup->addButton(m_stereoRadio, 2);
    chRow->addWidget(m_monoRadio);
    chRow->addWidget(m_stereoRadio);
    chRow->addStretch();
    fmtLayout->addLayout(chRow);

    // When the user toggles mono/stereo, refresh the sample rate list
    // since the max rate may differ between mono and stereo.
    connect(m_monoRadio, &QRadioButton::toggled,
            this, &ProjectSettingsDialog::refreshSampleRateList);

    layout->addWidget(fmtGroup);
    layout->addStretch();
    m_tabWidget->addTab(widget, tr("Audio"));
}

void ProjectSettingsDialog::refreshDevices()
{
    m_midiDeviceCombo->clear();
    // Index -1 = use app default; index 0 = built-in FluidSynth; index >=1 = external port
    m_midiDeviceCombo->addItem(tr("(App default)"), -1);
    m_midiDeviceCombo->addItem(tr("Built-in FluidSynth synthesizer"), 0);
    try
    {
        RtMidiOut midiOut;
        const unsigned int portCount = midiOut.getPortCount();
        for (unsigned int i = 0; i < portCount; ++i)
        {
            const string name = midiOut.getPortName(i);
            // External ports are 1-based so index 0 stays reserved for FluidSynth
            m_midiDeviceCombo->addItem(QString::fromStdString(name), static_cast<int>(i + 1));
        }
    }
    catch (...)
    {
        // External MIDI output not available; FluidSynth entry above still works
    }

    // MIDI input devices — listed by stable port name (matched the same way
    // VkMidiIo matches them).  Empty string = no MIDI input configured.
    if (m_midiInputCombo)
    {
        m_midiInputCombo->clear();
        m_midiInputCombo->addItem(tr("(None)"), QString());
        try
        {
            RtMidiIn midiIn;
            const unsigned int n = midiIn.getPortCount();
            for (unsigned int i = 0; i < n; ++i)
            {
                const QString name = QString::fromStdString(midiIn.getPortName(i));
                m_midiInputCombo->addItem(name, name);
            }
        }
        catch (...)
        {
            // No MIDI input available; the (None) entry above is still valid
        }
    }

    m_audioInputCombo->clear();
    m_audioOutputCombo->clear();
#ifdef QT_MULTIMEDIA_AVAILABLE
    m_audioInputCombo->addItem(tr("(App default)"), QByteArray());
    const auto inputs = QMediaDevices::audioInputs();
    for (const auto& device : inputs)
        m_audioInputCombo->addItem(device.description(), device.id());

    m_audioOutputCombo->addItem(tr("(App default)"), QByteArray());
    const auto outputs = QMediaDevices::audioOutputs();
    for (const auto& device : outputs)
        m_audioOutputCombo->addItem(device.description(), device.id());
#else
    m_audioInputCombo->addItem(tr("(App default)"), QByteArray());
    m_audioOutputCombo->addItem(tr("(App default)"), QByteArray());
#endif

#if defined(HAVE_PORTAUDIO)
    refreshPortAudioInputDevices();
#endif
}

void ProjectSettingsDialog::loadSettings(const ProjectSettings& settings)
{
    refreshDevices();

    for (int i = 0; i < m_midiDeviceCombo->count(); ++i)
    {
        if (m_midiDeviceCombo->itemData(i).toInt() == settings.midiDeviceIndex)
        {
            m_midiDeviceCombo->setCurrentIndex(i);
            break;
        }
    }

    if (m_midiInputCombo)
    {
        for (int i = 0; i < m_midiInputCombo->count(); ++i)
        {
            if (m_midiInputCombo->itemData(i).toString() == settings.midiInputPortName)
            {
                m_midiInputCombo->setCurrentIndex(i);
                break;
            }
        }
    }

    m_soundFontEdit->setText(settings.soundFontPath);

    for (int i = 0; i < m_audioInputCombo->count(); ++i)
    {
        if (m_audioInputCombo->itemData(i).toByteArray() == settings.audioInputDeviceId)
        {
            m_audioInputCombo->setCurrentIndex(i);
            break;
        }
    }

    for (int i = 0; i < m_audioOutputCombo->count(); ++i)
    {
        if (m_audioOutputCombo->itemData(i).toByteArray() == settings.audioOutputDeviceId)
        {
            m_audioOutputCombo->setCurrentIndex(i);
            break;
        }
    }

    // Channels — set before sample rates so refreshSampleRateList uses
    // the correct mono/stereo selection.
    if (settings.channelCount == 1)
        m_monoRadio->setChecked(true);
    else
        m_stereoRadio->setChecked(true);

    // Sample rate — updateAudioFormatOptions() was already called via the
    // currentIndexChanged signal when we populated the input combo above.
    // Now select the correct rate from the project settings.
    int srIdx = m_sampleRateCombo->findData(settings.sampleRate);
    if (srIdx >= 0)
        m_sampleRateCombo->setCurrentIndex(srIdx);
    // If the saved rate isn't available for this device,
    // refreshSampleRateList already selected the best fallback.

#if defined(HAVE_PORTAUDIO)
    if (settings.useQtAudioInput)
        m_recordingUseQtRadio->setChecked(true);
    else
        m_recordingUsePortAudioRadio->setChecked(true);

    {
        const int savedPaDev = settings.portAudioInputDeviceIndex;
        int paIdx            = m_portAudioDeviceCombo->findData(savedPaDev);
        if (paIdx >= 0)
            m_portAudioDeviceCombo->setCurrentIndex(paIdx);
        else if (savedPaDev >= 0)
        {
            m_portAudioDeviceCombo->addItem(
                tr("Saved device %1 (not in current list)").arg(savedPaDev), savedPaDev);
            m_portAudioDeviceCombo->setCurrentIndex(m_portAudioDeviceCombo->count() - 1);
        }
    }
    onRecordingBackendChanged();
#endif
}

#ifdef QT_MULTIMEDIA_AVAILABLE
QAudioDevice ProjectSettingsDialog::selectedInputDevice() const
{
    const QByteArray selectedId = m_audioInputCombo->currentData().toByteArray();
    if (selectedId.isEmpty())
        return QMediaDevices::defaultAudioInput();

    const auto inputs = QMediaDevices::audioInputs();
    for (const auto& dev : inputs)
    {
        if (dev.id() == selectedId)
            return dev;
    }
    return {};
}

void ProjectSettingsDialog::probeDeviceRates()
{
    m_probedMonoMaxRate   = 0;
    m_probedStereoMaxRate = 0;

    const QAudioDevice device = selectedInputDevice();
    if (device.isNull())
        return;

    // Use the device's preferred format for the probe (typically stereo).
    const QAudioFormat fmt = device.preferredFormat();
    const int reportedRate = fmt.sampleRate();
    const int reportedCh   = fmt.channelCount();
    const int bps          = fmt.bytesPerSample();  // bytes per single sample
    if (reportedRate <= 0 || reportedCh <= 0 || bps <= 0)
        return;

    // Brief capture — 750 ms is enough to measure the byte rate reliably.
    QBuffer buffer;
    buffer.open(QBuffer::WriteOnly);

    QAudioSource source(device, fmt);
    source.start(&buffer);

    QElapsedTimer timer;
    timer.start();

    QEventLoop loop;
    QTimer::singleShot(750, &loop, &QEventLoop::quit);
    loop.exec();

    source.stop();
    // Process any remaining events so the buffer receives final data.
    QApplication::processEvents();

    const double elapsed = timer.elapsed() / 1000.0;
    const int    bytes   = buffer.data().size();

    if (elapsed < 0.3 || bytes <= 0)
        return;  // probe too short, keep defaults

    // Compute the measured byte rate.
    const double byteRate = bytes / elapsed;

    // Expected byte rate if the reported format is accurate:
    const double expectedByteRate = reportedRate * reportedCh * bps;

    // Compute the actual frame rate at the reported channel count.
    const int bytesPerFrame   = reportedCh * bps;
    const double actualRate   = byteRate / bytesPerFrame;

    // Snap to nearest standard rate.
    auto snapRate = [](double measured) -> int
    {
        int best = 0;
        double bestDist = 1e9;
        for (int rate : s_stdRates)
        {
            const double dist = std::abs(measured - rate);
            if (dist < bestDist)
            {
                bestDist = dist;
                best = rate;
            }
        }
        return best;
    };

    const int stereoRate = snapRate(actualRate);

    // The device's true byte rate is constant regardless of what channel
    // count we request (the hardware can only send what it has).
    // For mono, the same byte stream gives twice as many frames per second
    // because each frame is half as wide.
    const double monoFrameRate = byteRate / bps;  // 1 channel
    const int monoRate = snapRate(monoFrameRate);

    // If the probed rates match the reported rate, the device is truthful.
    // If they differ, we've discovered the actual hardware behavior.
    m_probedStereoMaxRate = stereoRate;
    m_probedMonoMaxRate   = monoRate;
}
#endif // QT_MULTIMEDIA_AVAILABLE

void ProjectSettingsDialog::updateAudioFormatOptions()
{
#ifdef QT_MULTIMEDIA_AVAILABLE
    // Probe the device to discover its true data rate.
    // This is a brief (~750 ms) capture that measures the actual byte rate
    // delivered by QAudioSource, which may differ from what the API reports
    // (e.g. on PipeWire, a 32 kHz mono webcam is presented as stereo but
    // delivers data at half the advertised frame rate).
    QApplication::setOverrideCursor(Qt::WaitCursor);
    probeDeviceRates();
    QApplication::restoreOverrideCursor();

    // ── Channel options ───────────────────────────────────────────
    const QAudioDevice device = selectedInputDevice();
    if (!device.isNull())
    {
        QAudioFormat chTest = device.preferredFormat();

        chTest.setChannelCount(1);
        const bool monoSupported = device.isFormatSupported(chTest);

        chTest.setChannelCount(2);
        const bool stereoSupported = device.isFormatSupported(chTest);

        // If the probed stereo rate equals the mono rate, the device is
        // truly stereo.  If the stereo rate is lower (e.g. half), the
        // device is physically mono and stereo wastes bandwidth.
        const bool stereoIsReduced =
            (m_probedStereoMaxRate > 0 && m_probedMonoMaxRate > 0 &&
             m_probedStereoMaxRate < m_probedMonoMaxRate);

        if (monoSupported && (stereoSupported || !stereoIsReduced))
        {
            m_monoRadio->setEnabled(true);
            m_stereoRadio->setEnabled(stereoSupported);
        }
        else if (monoSupported)
        {
            m_monoRadio->setEnabled(true);
            m_stereoRadio->setEnabled(false);
            m_monoRadio->setChecked(true);
        }
        else
        {
            m_monoRadio->setEnabled(true);
            m_stereoRadio->setEnabled(true);
        }
    }
    else
    {
        m_monoRadio->setEnabled(true);
        m_stereoRadio->setEnabled(true);
    }
#endif

    // Now populate the sample rate list based on the current channel selection.
    refreshSampleRateList();
}

void ProjectSettingsDialog::refreshSampleRateList()
{
    const int previousRate = m_sampleRateCombo->currentData().toInt();

    m_sampleRateCombo->clear();

    // Always offer the common recording sample rates.  Like Audacity,
    // rates above the device's native rate are supported via high-quality
    // windowed sinc resampling after recording.
    static const int s_offeredRates[] = {
        8000, 11025, 16000, 22050, 32000, 44100, 48000, 96000
    };

#ifdef QT_MULTIMEDIA_AVAILABLE
    const bool isMono = m_monoRadio->isChecked();
    const int nativeRate = isMono ? m_probedMonoMaxRate : m_probedStereoMaxRate;

    if (nativeRate > 0)
    {
        for (int rate : s_offeredRates)
        {
            QString label = QStringLiteral("%1 Hz").arg(rate);
            if (rate == nativeRate)
                label += tr(" (native)");
            else if (rate > nativeRate)
                label += tr(" (resampled)");
            m_sampleRateCombo->addItem(label, rate);
        }
        // If the native rate isn't in the offered list, add it
        bool nativeInList = false;
        for (int rate : s_offeredRates)
        {
            if (rate == nativeRate)
            {
                nativeInList = true;
                break;
            }
        }
        if (!nativeInList)
        {
            const QString label = QStringLiteral("%1 Hz").arg(nativeRate) + tr(" (native)");
            // Insert in sorted position
            int insertIdx = 0;
            for (int i = 0; i < m_sampleRateCombo->count(); ++i)
            {
                if (m_sampleRateCombo->itemData(i).toInt() > nativeRate)
                    break;
                insertIdx = i + 1;
            }
            m_sampleRateCombo->insertItem(insertIdx, label, nativeRate);
        }
    }
    else
#endif
    {
        // No probe data — show the standard list without annotations
        for (int rate : s_offeredRates)
            m_sampleRateCombo->addItem(QStringLiteral("%1 Hz").arg(rate), rate);
    }

    // Restore the previous selection, or fall back to 44100
    int bestIdx = -1;
    if (previousRate > 0)
        bestIdx = m_sampleRateCombo->findData(previousRate);
    if (bestIdx < 0)
        bestIdx = m_sampleRateCombo->findData(44100);
    if (bestIdx < 0)
        bestIdx = m_sampleRateCombo->count() - 1;
    m_sampleRateCombo->setCurrentIndex(bestIdx);
}

void ProjectSettingsDialog::onBrowseSoundFont()
{
    const QString path = QFileDialog::getOpenFileName(
        this, tr("Select SoundFont"),
        m_soundFontEdit->text(),
        tr("SoundFont files (*.sf2 *.SF2);;All files (*)"));
    if (!path.isEmpty())
        m_soundFontEdit->setText(path);
}

#if defined(HAVE_PORTAUDIO)
void ProjectSettingsDialog::refreshPortAudioInputDevices()
{
    m_portAudioDeviceCombo->clear();
    m_portAudioDeviceCombo->addItem(tr("(Default audio input)"), -1);

    if (!PortAudioRecorder::isCompiledWithPortAudio())
    {
        onRecordingBackendChanged();
        return;
    }

    const int n = PortAudioRecorder::deviceCount();
    for (int i = 0; i < n; ++i)
    {
        if (PortAudioRecorder::maxInputChannels(i) <= 0)
            continue;
        const QString name = PortAudioRecorder::deviceName(i);
        m_portAudioDeviceCombo->addItem(name.isEmpty() ? QString::number(i)
                                                       : QStringLiteral("%1: %2").arg(i).arg(name),
                                       i);
    }
    onRecordingBackendChanged();
}

void ProjectSettingsDialog::onRecordingBackendChanged()
{
    const bool usePa = m_recordingUsePortAudioRadio && m_recordingUsePortAudioRadio->isChecked();
    m_portAudioDeviceCombo->setEnabled(usePa && PortAudioRecorder::isCompiledWithPortAudio());
}
#endif
