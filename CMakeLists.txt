cmake_minimum_required(VERSION 3.22)

project(KissOfShame VERSION 2.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)
FetchContent_Declare(JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        8.0.9
    GIT_SHALLOW    TRUE)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(KissOfShame
    PRODUCT_NAME                "The Kiss Of Shame"
    COMPANY_NAME                "Infernal Love"
    BUNDLE_ID                   com.InfernalLove.KissOfShame
    PLUGIN_MANUFACTURER_CODE    ILov
    PLUGIN_CODE                 Plug
    FORMATS                     AU VST3 Standalone
    IS_SYNTH                    FALSE
    NEEDS_MIDI_INPUT            FALSE
    NEEDS_MIDI_OUTPUT           FALSE
    IS_MIDI_EFFECT              FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD     FALSE
    VST3_CATEGORIES             Fx Distortion
    AU_MAIN_TYPE                kAudioUnitType_Effect)

juce_generate_juce_header(KissOfShame)

target_sources(KissOfShame PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
    Source/AudioProcessing/Granulate.cpp
    Source/GUIUtilities/CustomKnob.cpp
    Source/GUIUtilities/ImageInteractor.cpp)

juce_add_binary_data(KissOfShameAssets
    HEADER_NAME BinaryData.h
    NAMESPACE BinaryData
    SOURCES
        GUI_Resources/KOS_Graphics/00.png
        GUI_Resources/KOS_Graphics/01.png
        GUI_Resources/KOS_Graphics/03_alpha.png
        GUI_Resources/KOS_Graphics/04_alpha.png
        GUI_Resources/KOS_Graphics/05_alpha.png
        GUI_Resources/KOS_Graphics/06_alpha.png
        GUI_Resources/KOS_Graphics/07.png
        GUI_Resources/KOS_Graphics/08.png
        GUI_Resources/KOS_Graphics/09_alpha.png
        GUI_Resources/KOS_Graphics/09_v2.png
        GUI_Resources/KOS_Graphics/10.png
        GUI_Resources/KOS_Graphics/11.png
        GUI_Resources/KOS_Graphics/12_alpha.png
        GUI_Resources/KOS_Graphics/fond_alpha.png
        GUI_Resources/KOS_Graphics/fond_alone2.png
        GUI_Resources/KOS_Graphics/link.png
        GUI_Resources/KOS_Graphics/wheels.png
        Audio_Resources/Hiss.wav
        Audio_Resources/PinkNoise.wav
        Audio_Resources/LowLevelGrainNoise.wav)

target_compile_definitions(KissOfShame PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0)

target_link_libraries(KissOfShame
    PRIVATE
        KissOfShameAssets
        juce::juce_audio_utils
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags)

#===============================================================================
# Offline smoke test: boots the processor and proves audio passes through
# every environment without going silent or non-finite.
juce_add_console_app(KissOfShameSmokeTest PRODUCT_NAME "KissOfShameSmokeTest")

target_sources(KissOfShameSmokeTest PRIVATE Tests/SmokeTest.cpp)

target_include_directories(KissOfShameSmokeTest PRIVATE
    Source
    $<TARGET_PROPERTY:KissOfShame,JUCE_GENERATED_SOURCES_DIRECTORY>)

target_link_libraries(KissOfShameSmokeTest PRIVATE
    KissOfShame
    KissOfShameAssets
    juce::juce_audio_utils
    juce::juce_gui_extra)

enable_testing()
add_test(NAME SmokeTest COMMAND KissOfShameSmokeTest)
