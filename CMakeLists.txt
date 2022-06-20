cmake_minimum_required (VERSION 3.10)

project(rtstft_ctl VERSION 0.1.0)
    
add_subdirectory(JUCE)


juce_add_plugin(rtstft_ctl
    VERSION 0.1.0                     
    # ICON_BIG ...                    
    # ICON_SMALL ...
    COMPANY_NAME sound_ctl            
    IS_SYNTH FALSE                    
    NEEDS_MIDI_INPUT FALSE            
    NEEDS_MIDI_OUTPUT FALSE           
    IS_MIDI_EFFECT FALSE              
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE  
    COPY_PLUGIN_AFTER_BUILD TRUE      
    PLUGIN_MANUFACTURER_CODE SCTL     
    PLUGIN_CODE Rtst                  
    FORMATS AU VST3 Standalone      
    PRODUCT_NAME ${PROJECT_NAME})        

juce_generate_juce_header(${PROJECT_NAME})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

# add source
file(GLOB_RECURSE Source ${CMAKE_CURRENT_LIST_DIR}/Source/*.cpp ${CMAKE_CURRENT_LIST_DIR}/Source/*.h)
file(GLOB_RECURSE RTSTFT ${CMAKE_CURRENT_LIST_DIR}/RTSTFT/*.c ${CMAKE_CURRENT_LIST_DIR}/Source/*.h)
target_sources(${PROJECT_NAME} PRIVATE ${Source})
target_sources(${PROJECT_NAME} PRIVATE ${RTSTFT})

target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        # rtstft_ctl
        juce::juce_audio_utils
    # PUBLIC
    #     juce::juce_recommended_config_flags
    #     juce::juce_recommended_lto_flags
    #     juce::juce_recommended_warning_flags
        )
