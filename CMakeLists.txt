cmake_minimum_required(VERSION 3.16)
project(musicians_canvas VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Qt6 COMPONENTS Multimedia QUIET)
set(QT_MULTIMEDIA_FOUND ${Qt6Multimedia_FOUND})

find_library(FLUIDSYNTH_LIBRARY NAMES fluidsynth libfluidsynth fluidsynth-3 libfluidsynth-3 fluidsynth.so.3)
find_path(FLUIDSYNTH_INCLUDE_DIR fluidsynth.h)
find_library(RTMIDI_LIBRARY NAMES rtmidi librtmidi)
find_path(RTMIDI_INCLUDE_DIR RtMidi.h PATH_SUFFIXES "" rtmidi)

if(NOT FLUIDSYNTH_LIBRARY OR NOT FLUIDSYNTH_INCLUDE_DIR)
    message(FATAL_ERROR "FluidSynth not found. Install libfluidsynth-dev")
endif()
if(NOT RTMIDI_LIBRARY OR NOT RTMIDI_INCLUDE_DIR)
    message(FATAL_ERROR "RtMidi not found. Install librtmidi-dev")
endif()

set(SOURCES
    src/musicians_canvas_main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/trackwidget.cpp
    src/trackwidget.h
    src/trackconfigdialog.cpp
    src/trackconfigdialog.h
    src/settingsdialog.cpp
    src/settingsdialog.h
    src/waveformwidget.cpp
    src/waveformwidget.h
    src/midivisualizationwidget.cpp
    src/midivisualizationwidget.h
    src/midisynth.cpp
    src/themeutils.cpp
    src/audioutils.cpp
    src/trackdata.cpp
    src/appsettings.cpp
)

add_executable(musicians_canvas ${SOURCES})
if(WIN32)
    set_target_properties(musicians_canvas PROPERTIES WIN32_EXECUTABLE TRUE)
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
)
add_executable(virtual_midi_keyboard ${VK_SOURCES})
if(WIN32)
    set_target_properties(virtual_midi_keyboard PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
target_include_directories(virtual_midi_keyboard PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${FLUIDSYNTH_INCLUDE_DIR}
    ${RTMIDI_INCLUDE_DIR}
)
target_link_libraries(virtual_midi_keyboard
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    ${FLUIDSYNTH_LIBRARY}
    ${RTMIDI_LIBRARY}
)
if(QT_MULTIMEDIA_FOUND)
    target_link_libraries(virtual_midi_keyboard Qt6::Multimedia)
    target_compile_definitions(virtual_midi_keyboard PRIVATE QT_MULTIMEDIA_AVAILABLE)
endif()

target_link_libraries(musicians_canvas
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    ${FLUIDSYNTH_LIBRARY}
    ${RTMIDI_LIBRARY}
)
if(QT_MULTIMEDIA_FOUND)
    target_link_libraries(musicians_canvas Qt6::Multimedia)
    target_compile_definitions(musicians_canvas PRIVATE QT_MULTIMEDIA_AVAILABLE)
endif()

target_include_directories(musicians_canvas PRIVATE
    ${FLUIDSYNTH_INCLUDE_DIR}
    ${RTMIDI_INCLUDE_DIR}
)
