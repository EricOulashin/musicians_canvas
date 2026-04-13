cmake_minimum_required(VERSION 3.16)
project(musicians_canvas VERSION 0.1.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Qt6 COMPONENTS Multimedia QUIET)
find_package(Qt6 COMPONENTS LinguistTools QUIET)
set(QT_MULTIMEDIA_FOUND ${Qt6Multimedia_FOUND})

find_library(FLUIDSYNTH_LIBRARY NAMES fluidsynth libfluidsynth fluidsynth-3 libfluidsynth-3 fluidsynth.so.3)
find_path(FLUIDSYNTH_INCLUDE_DIR fluidsynth.h)
find_library(RTMIDI_LIBRARY NAMES rtmidi librtmidi)
find_path(RTMIDI_INCLUDE_DIR RtMidi.h PATH_SUFFIXES "" rtmidi)
find_library(FLAC_LIBRARY NAMES FLAC libFLAC)
find_path(FLAC_INCLUDE_DIR FLAC/stream_decoder.h)

if(NOT FLUIDSYNTH_LIBRARY OR NOT FLUIDSYNTH_INCLUDE_DIR)
    message(FATAL_ERROR "FluidSynth not found. Install libfluidsynth-dev")
endif()
if(NOT RTMIDI_LIBRARY OR NOT RTMIDI_INCLUDE_DIR)
    message(FATAL_ERROR "RtMidi not found. Install librtmidi-dev")
endif()
if(NOT FLAC_LIBRARY)
    message(FATAL_ERROR "libFLAC not found. Install libflac-dev")
endif()

# Path to the shared audio_mixer_cpp library sources (relative to this project)
set(AUDIO_MIXER_CPP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../audio_mixer_cpp/code/common)

set(AUDIO_MIXER_SOURCES
    ${AUDIO_MIXER_CPP_DIR}/AudioFile.cpp
    ${AUDIO_MIXER_CPP_DIR}/AudioFileInfo.cpp
    ${AUDIO_MIXER_CPP_DIR}/AudioFileResultType.cpp
    ${AUDIO_MIXER_CPP_DIR}/AudioFileTools.cpp
    ${AUDIO_MIXER_CPP_DIR}/EOUtils.cpp
    ${AUDIO_MIXER_CPP_DIR}/FLACFile.cpp
    ${AUDIO_MIXER_CPP_DIR}/FLACFileInfo.cpp
    ${AUDIO_MIXER_CPP_DIR}/MetadataAudioFile.cpp
    ${AUDIO_MIXER_CPP_DIR}/WAVFile.cpp
    ${AUDIO_MIXER_CPP_DIR}/WAVFileInfo.cpp
    ${AUDIO_MIXER_CPP_DIR}/utilFunctions.cpp
)

set(SOURCES
    src/musicians_canvas_main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/trackwidget.cpp
    src/trackwidget.h
    src/trackconfigdialog.cpp
    src/trackconfigdialog.h
    src/effectdsp.cpp
    src/effectdsp.h
    src/effectwidget.cpp
    src/effectwidget.h
    src/reverbwidget.cpp
    src/reverbwidget.h
    src/choruswidget.cpp
    src/choruswidget.h
    src/flangewidget.cpp
    src/flangewidget.h
    src/vibratoeffect.cpp
    src/vibratoeffect.h
    src/effectsdialog.cpp
    src/effectsdialog.h
    src/effectchaineditorwidget.cpp
    src/effectchaineditorwidget.h
    src/audiopcmlimits.h
    src/effectchaindragoverlay.cpp
    src/effectchaindragoverlay.h
    src/overdrivedistortionwidget.cpp
    src/overdrivedistortionwidget.h
    src/ampandcabinetmodelwidget.cpp
    src/ampandcabinetmodelwidget.h
    src/settingsdialog.cpp
    src/settingsdialog.h
    src/projectsettings.h
    src/projectsettings.cpp
    src/projectsettingsdialog.cpp
    src/projectsettingsdialog.h
    src/audioformats.h
    src/midirecorder.cpp
    src/midirecorder.h
    src/midiplayer.cpp
    src/midiplayer.h
    src/midifile.cpp
    src/midifile.h
    src/waveformwidget.cpp
    src/waveformwidget.h
    src/midivisualizationwidget.cpp
    src/midivisualizationwidget.h
    src/midisynth.cpp
    src/themeutils.cpp
    src/audioutils.cpp
    src/drumtrackgenerator.cpp
    src/drumtrackgenerator.h
    src/recordingpostprocess.cpp
    src/portaudiorecorder.cpp
    src/trackdata.cpp
    src/appsettings.cpp
    src/mixdialog.cpp
    src/mixdialog.h
    src/metronomedialog.cpp
    src/metronomedialog.h
    src/trackmixerundocommand.cpp
    src/trackmixerundocommand.h
    src/workflowdialogs.cpp
    src/workflowdialogs.h
    src/segmentdisplay.cpp
    src/segmentdisplay.h
    src/audiostartup.cpp
    src/audiostartup.h
    src/resources.qrc
    ${AUDIO_MIXER_SOURCES}
)

set(MC_MONITOR_SOURCES
    src/recordingwritedevice.cpp
    src/recordingwritedevice.h
    src/recordingaudiomonitor.cpp
    src/recordingaudiomonitor.h
    src/midirecordingmonitor.cpp
    src/midirecordingmonitor.h
)

# ── Translations (Qt Linguist) ────────────────────────────────────────
set(MC_LANGS de es fr ja pt_BR zh_TW zh_CN da el ga nb pl ru fi sv cy pirate)

set(MC_TS_FILES "")
set(VK_TS_FILES "")
foreach(lang ${MC_LANGS})
    list(APPEND MC_TS_FILES "${CMAKE_CURRENT_SOURCE_DIR}/translations/musicians_canvas_${lang}.ts")
    list(APPEND VK_TS_FILES "${CMAKE_CURRENT_SOURCE_DIR}/translations/virtual_midi_keyboard_${lang}.ts")
endforeach()

set(MC_QM_FILES "")
set(VK_QM_FILES "")

if(Qt6LinguistTools_FOUND)
    qt_add_translation(MC_QM_FILES ${MC_TS_FILES})
    qt_add_translation(VK_QM_FILES ${VK_TS_FILES})
else()
    # Fallback: invoke lrelease directly if available
    find_program(LRELEASE_EXECUTABLE NAMES lrelease-qt6 lrelease
                 HINTS "${Qt6Core_DIR}/../../../bin" "${Qt6Core_DIR}/../../libexec"
                       "/usr/lib/qt6/bin" "/usr/bin")
    if(LRELEASE_EXECUTABLE)
        message(STATUS "Using lrelease directly: ${LRELEASE_EXECUTABLE}")
        foreach(ts ${MC_TS_FILES} ${VK_TS_FILES})
            get_filename_component(ts_name "${ts}" NAME_WE)
            set(qm "${CMAKE_CURRENT_BINARY_DIR}/${ts_name}.qm")
            add_custom_command(OUTPUT "${qm}"
                COMMAND "${LRELEASE_EXECUTABLE}" "${ts}" -qm "${qm}"
                DEPENDS "${ts}" COMMENT "Compiling ${ts_name}.qm"
                VERBATIM)
            if(ts_name MATCHES "^musicians_canvas_")
                list(APPEND MC_QM_FILES "${qm}")
            else()
                list(APPEND VK_QM_FILES "${qm}")
            endif()
        endforeach()
    else()
        message(STATUS "lrelease not found — translations will not be compiled")
    endif()
endif()

# Generate a .qrc file embedding all .qm files so they are available via :/translations/
set(MC_TR_QRC "${CMAKE_CURRENT_BINARY_DIR}/musicians_canvas_translations.qrc")
file(WRITE "${MC_TR_QRC}" "<!DOCTYPE RCC>\n<RCC version=\"1.0\">\n  <qresource prefix=\"/translations\">\n")
foreach(qm ${MC_QM_FILES})
    get_filename_component(qm_name "${qm}" NAME)
    file(APPEND "${MC_TR_QRC}" "    <file alias=\"${qm_name}\">${qm}</file>\n")
endforeach()
file(APPEND "${MC_TR_QRC}" "  </qresource>\n</RCC>\n")

set(VK_TR_QRC "${CMAKE_CURRENT_BINARY_DIR}/virtual_midi_keyboard_translations.qrc")
file(WRITE "${VK_TR_QRC}" "<!DOCTYPE RCC>\n<RCC version=\"1.0\">\n  <qresource prefix=\"/translations\">\n")
foreach(qm ${VK_QM_FILES})
    get_filename_component(qm_name "${qm}" NAME)
    file(APPEND "${VK_TR_QRC}" "    <file alias=\"${qm_name}\">${qm}</file>\n")
endforeach()
file(APPEND "${VK_TR_QRC}" "  </qresource>\n</RCC>\n")

# Also copy .qm files to build/translations/ for external loading
if(MC_QM_FILES OR VK_QM_FILES)
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/translations")
    foreach(qm ${MC_QM_FILES} ${VK_QM_FILES})
        get_filename_component(qm_name "${qm}" NAME)
        add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/translations/${qm_name}"
            COMMAND ${CMAKE_COMMAND} -E copy "${qm}" "${CMAKE_CURRENT_BINARY_DIR}/translations/${qm_name}"
            DEPENDS "${qm}" COMMENT "Copying ${qm_name}")
        list(APPEND MC_QM_COPY_TARGETS "${CMAKE_CURRENT_BINARY_DIR}/translations/${qm_name}")
    endforeach()
    add_custom_target(copy_translations ALL DEPENDS ${MC_QM_COPY_TARGETS})
endif()

# Windows .rc file for application icon
if(WIN32)
    list(APPEND SOURCES src/appicon.rc)
endif()

add_executable(musicians_canvas ${SOURCES} ${MC_TR_QRC} ${MC_QM_FILES})
target_compile_definitions(musicians_canvas PRIVATE APP_VERSION="${PROJECT_VERSION}")
if(WIN32)
    set_target_properties(musicians_canvas PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
if(APPLE)
    set_target_properties(musicians_canvas PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME "Musician's Canvas"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.ericoulashin.musicianscanvas"
        MACOSX_BUNDLE_ICON_FILE "MusiciansCanvas.icns"
    )
endif()

find_package(PkgConfig QUIET)

# Prefer PkgConfig IMPORTED_TARGET so -L from the .pc file reaches the linker (required on
# macOS/Homebrew where libportaudio is not on the default library path).
set(MC_PORTAUDIO_LINKED FALSE)
if(PkgConfig_FOUND)
    pkg_check_modules(PORTAUDIO_PC QUIET IMPORTED_TARGET portaudio-2.0)
endif()
if(PORTAUDIO_PC_FOUND AND TARGET PkgConfig::PORTAUDIO_PC)
    target_link_libraries(musicians_canvas PRIVATE PkgConfig::PORTAUDIO_PC)
    target_compile_definitions(musicians_canvas PRIVATE HAVE_PORTAUDIO)
    message(STATUS "PortAudio found — recording can use native capture (HAVE_PORTAUDIO)")
    set(MC_PORTAUDIO_LINKED TRUE)
endif()
if(NOT MC_PORTAUDIO_LINKED)
    find_path(PORTAUDIO_INCLUDE_DIR portaudio.h PATH_SUFFIXES include)
    find_library(PORTAUDIO_LIBRARY NAMES portaudio libportaudio)
    if(PORTAUDIO_INCLUDE_DIR AND PORTAUDIO_LIBRARY)
        target_include_directories(musicians_canvas PRIVATE ${PORTAUDIO_INCLUDE_DIR})
        target_link_libraries(musicians_canvas PRIVATE ${PORTAUDIO_LIBRARY})
        target_compile_definitions(musicians_canvas PRIVATE HAVE_PORTAUDIO)
        message(STATUS "PortAudio found — recording can use native capture (HAVE_PORTAUDIO)")
    else()
        message(STATUS "PortAudio not found — build continues; PortAudio capture disabled (stubs)")
    endif()
endif()

option(BUILD_TESTING "Build headless musicians_canvas_tests executable (unit tests)" ON)
if(BUILD_TESTING)
    add_executable(musicians_canvas_tests
        tests/musicians_canvas_tests.cpp
        src/recordingpostprocess.cpp
        src/trackdata.cpp
        src/audioutils.cpp
        src/effectdsp.cpp
        src/midisynth.cpp
        src/appsettings.cpp
        ${AUDIO_MIXER_SOURCES}
    )
    target_link_libraries(musicians_canvas_tests PRIVATE
        Qt6::Core
        ${FLAC_LIBRARY}
        ${FLUIDSYNTH_LIBRARY}
    )
    target_include_directories(musicians_canvas_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${AUDIO_MIXER_CPP_DIR}
        ${FLUIDSYNTH_INCLUDE_DIR}
    )
    enable_testing()
    add_test(NAME musicians_canvas_tests COMMAND musicians_canvas_tests)
endif()

set(VK_SOURCES
    src/virtual_midi_keyboard/virtual_midi_keyboard_main.cpp
    src/virtual_midi_keyboard/vk_mainwindow.cpp
    src/virtual_midi_keyboard/vk_mainwindow.h
    src/virtual_midi_keyboard/vk_configdialog.cpp
    src/virtual_midi_keyboard/vk_configdialog.h
    src/virtual_midi_keyboard/vk_helpdialog.cpp
    src/virtual_midi_keyboard/vk_helpdialog.h
    src/virtual_midi_keyboard/vk_knob.cpp
    src/virtual_midi_keyboard/vk_knob.h
    src/virtual_midi_keyboard/vk_midiio.cpp
    src/virtual_midi_keyboard/vk_midiio.h
    src/virtual_midi_keyboard/vk_pianokeyboard.cpp
    src/virtual_midi_keyboard/vk_pianokeyboard.h
    src/virtual_midi_keyboard/vk_settings.cpp
    src/virtual_midi_keyboard/vk_settings.h
    src/virtual_midi_keyboard/vk_resources.qrc
)
if(WIN32)
    list(APPEND VK_SOURCES src/virtual_midi_keyboard/appicon.rc)
endif()
add_executable(virtual_midi_keyboard ${VK_SOURCES} ${VK_TR_QRC} ${VK_QM_FILES})
target_compile_definitions(virtual_midi_keyboard PRIVATE APP_VERSION="${PROJECT_VERSION}")
if(WIN32)
    set_target_properties(virtual_midi_keyboard PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
# On macOS, bundle virtual_midi_keyboard inside musicians_canvas.app so there
# is a single .app the user can distribute.  The VK executable is NOT built as
# its own MACOSX_BUNDLE; instead it is copied into the MC bundle post-build.
if(APPLE)
    # Ensure the main app bundle exists before we copy VK into it (avoids races with -j).
    add_dependencies(virtual_midi_keyboard musicians_canvas)
    add_custom_command(TARGET virtual_midi_keyboard POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            "$<TARGET_FILE:virtual_midi_keyboard>"
            "$<TARGET_BUNDLE_DIR:musicians_canvas>/Contents/MacOS/virtual_midi_keyboard"
        COMMENT "Copying virtual_midi_keyboard into musicians_canvas.app bundle"
    )
endif()
target_include_directories(virtual_midi_keyboard PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${FLUIDSYNTH_INCLUDE_DIR}
    ${RTMIDI_INCLUDE_DIR}
)
target_link_libraries(virtual_midi_keyboard PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    ${FLUIDSYNTH_LIBRARY}
    ${RTMIDI_LIBRARY}
)
if(QT_MULTIMEDIA_FOUND)
    target_link_libraries(virtual_midi_keyboard PRIVATE Qt6::Multimedia)
    target_compile_definitions(virtual_midi_keyboard PRIVATE QT_MULTIMEDIA_AVAILABLE)
endif()

target_link_libraries(musicians_canvas PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    ${FLUIDSYNTH_LIBRARY}
    ${RTMIDI_LIBRARY}
    ${FLAC_LIBRARY}
)
if(QT_MULTIMEDIA_FOUND)
    target_sources(musicians_canvas PRIVATE ${MC_MONITOR_SOURCES})
    target_link_libraries(musicians_canvas PRIVATE Qt6::Multimedia)
    target_compile_definitions(musicians_canvas PRIVATE QT_MULTIMEDIA_AVAILABLE)
endif()

target_include_directories(musicians_canvas PRIVATE
    ${FLUIDSYNTH_INCLUDE_DIR}
    ${RTMIDI_INCLUDE_DIR}
    ${AUDIO_MIXER_CPP_DIR}
)

# The audio_mixer_cpp headers use int16_t / int32_t / int64_t without
# including <cstdint>.  On Linux/macOS the type is pulled in transitively,
# but MinGW on Windows does not.  Force-include <cstdint> so the types are
# always visible.
if(MSVC)
    target_compile_options(musicians_canvas PRIVATE /FIcstdint)
    if(BUILD_TESTING AND TARGET musicians_canvas_tests)
        target_compile_options(musicians_canvas_tests PRIVATE /FIcstdint)
    endif()
else()
    target_compile_options(musicians_canvas PRIVATE -include cstdint)
    if(BUILD_TESTING AND TARGET musicians_canvas_tests)
        target_compile_options(musicians_canvas_tests PRIVATE -include cstdint)
    endif()
endif()

# Platform-specific libraries for low-latency audio initialisation
if(WIN32)
    # advapi32: Windows registry API (RegOpenKeyExW, RegEnumKeyExW) for ASIO detection
    # winmm:    Windows Multimedia API used by the MIDI and audio subsystems
    target_link_libraries(musicians_canvas PRIVATE advapi32 winmm)
else()
    # Threads (pthreads on Linux/macOS) – used by Qt Multimedia's audio backend
    find_package(Threads REQUIRED)
    target_link_libraries(musicians_canvas PRIVATE Threads::Threads)

    # PipeWire exists only on Linux in practice. Skip pkg-config on macOS so CI does not
    # run "Checking for module 'libpipewire-0.3'" (and avoid any toolchain edge cases).
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND PkgConfig_FOUND)
        pkg_check_modules(PIPEWIRE QUIET libpipewire-0.3)
        if(PIPEWIRE_FOUND)
            # FluidSynth's PipeWire driver: link libpipewire so pw_init() is available.
            target_include_directories(virtual_midi_keyboard PRIVATE ${PIPEWIRE_INCLUDE_DIRS})
            target_link_libraries(virtual_midi_keyboard PRIVATE ${PIPEWIRE_LIBRARIES})
            target_compile_definitions(virtual_midi_keyboard PRIVATE HAVE_PIPEWIRE)

            target_include_directories(musicians_canvas PRIVATE ${PIPEWIRE_INCLUDE_DIRS})
            target_link_libraries(musicians_canvas PRIVATE ${PIPEWIRE_LIBRARIES})
            target_compile_definitions(musicians_canvas PRIVATE HAVE_PIPEWIRE)
        endif()
    endif()
endif()
