cmake_minimum_required (VERSION 3.10)

project(rtstft_ctl VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
if (APPLE)
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()

# set(CMAKE_UNITY_BUILD TRUE)
set(JUCE_ENABLE_MODULE_SOURCE_GROUPS ON)
add_subdirectory(JUCE)

if (DONT_COPY)
    set(COPY_STEP FALSE)
else()
    set(COPY_STEP TRUE)
endif()

juce_add_plugin(${PROJECT_NAME}
    VERSION 0.1.0                     
    # ICON_BIG ...                    
    # ICON_SMALL ...
    COMPANY_NAME sound_ctl            
    IS_SYNTH FALSE                    
    NEEDS_MIDI_INPUT FALSE            
    NEEDS_MIDI_OUTPUT FALSE           
    IS_MIDI_EFFECT FALSE              
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE  
    COPY_PLUGIN_AFTER_BUILD ${COPY_STEP}      
    PLUGIN_MANUFACTURER_CODE SCTL     
    PLUGIN_CODE Rtst                  
    FORMATS AU VST3 Standalone      
    PRODUCT_NAME ${PROJECT_NAME})        

juce_generate_juce_header(${PROJECT_NAME})


set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 90)

# add librtstft
file(GLOB_RECURSE RTSTFT ${CMAKE_CURRENT_LIST_DIR}/RTSTFT/*.c ${CMAKE_CURRENT_LIST_DIR}/RTSTFT/*.h)
list(REMOVE_ITEM RTSTFT ${CMAKE_CURRENT_LIST_DIR}/RTSTFT/src/tests.c ${CMAKE_CURRENT_LIST_DIR}/RTSTFT/src/wavfile.c)
target_sources(${PROJECT_NAME} PRIVATE ${RTSTFT})

# add C++ source
file(GLOB_RECURSE Source ${CMAKE_CURRENT_LIST_DIR}/Source/*.cpp ${CMAKE_CURRENT_LIST_DIR}/Source/*.h)
target_sources(${PROJECT_NAME} PRIVATE ${Source})


add_compile_definitions(_USE_MATH_DEFINES)

target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0
        )

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        # rtstft_ctl
        # ${CMAKE_SOURCE_DIR}/RTSTFT/build/lib/librtstft.a
        juce::juce_audio_utils
    PUBLIC
    #     juce::juce_recommended_config_flags
    #     juce::juce_recommended_lto_flags
    #     juce::juce_recommended_warning_flags
        )
