cmake_minimum_required(VERSION 3.6)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


project(canvas-lib CXX C)


add_subdirectory(DiligentCore)
add_subdirectory(DiligentTools)
add_subdirectory(SampleBase)

    function(add_emscripten_app TARGET_NAME SOURCE INCLUDE ASSETS)
        add_executable(${TARGET_NAME} ${SOURCE} ${INCLUDE} ${ASSETS})   
    endfunction()

    function(add_target_platform_app TARGET_NAME SOURCE INCLUDE ASSETS)
        add_emscripten_app("${TARGET_NAME}" "${SOURCE}" "${INCLUDE}" "${ASSETS}")
    endfunction()

function(add_sample_app APP_NAME IDE_FOLDER SOURCE INCLUDE SHADERS ASSETS)

    set_source_files_properties(${SHADERS} PROPERTIES VS_TOOL_OVERRIDE "None")
    set(ALL_ASSETS ${ASSETS} ${SHADERS})
    add_target_platform_app(${APP_NAME} "${SOURCE}" "${INCLUDE}" "${ALL_ASSETS}")

    set_source_files_properties(${ALL_ASSETS} PROPERTIES 
        VS_DEPLOYMENT_LOCATION "."
        MACOSX_PACKAGE_LOCATION "Resources" 
    )

    target_include_directories(${APP_NAME}
    PRIVATE	
        src
    )

    target_link_libraries(${APP_NAME}
    PRIVATE
        # On Linux we must have Diligent-NativeAppBase go first, otherwise the linker 
        # will fail to resolve Diligent::CreateApplication() function.
        Diligent-NativeAppBase
        Diligent-BuildSettings
        Diligent-SampleBase
    )
    set_common_target_properties(${APP_NAME})

    if(MSVC)
        # Disable MSVC-specific warnings
        # - w4201: nonstandard extension used: nameless struct/union
        target_compile_options(${APP_NAME} PRIVATE /wd4201)
    endif()

    set_target_properties(${APP_NAME} PROPERTIES
        FOLDER ${IDE_FOLDER}
    )

    source_group("src" FILES ${SOURCE} ${INCLUDE})
    source_group("assets" FILES ${ALL_ASSETS})	

    # target_sources(${APP_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/readme.md")
    # set_source_files_properties(
    #     "${CMAKE_CURRENT_SOURCE_DIR}/readme.md" PROPERTIES HEADER_FILE_ONLY TRUE
    # )

    if(PLATFORM_MACOS AND VULKAN_LIB_PATH)
        # Configure rpath so that executables can find vulkan library
        set_target_properties(${APP_NAME} PROPERTIES
            BUILD_RPATH "${VULKAN_LIB_PATH}"
        )
    endif()

    if(PLATFORM_EMSCRIPTEN)
        set(RESOURCE_PATH "${PROJECT_SOURCE_DIR}/assets/")
        target_link_options(${APP_NAME} PRIVATE "SHELL: -s ALLOW_MEMORY_GROWTH=1 --preload-file '${RESOURCE_PATH}@' -sENVIRONMENT='web'  --no-entry -sUSE_ES6_IMPORT_META=0 -sEXPORT_NAME='WWasmModule' --std=c++20 --bind")
        # target_link_options(${APP_NAME} PRIVATE "SHELL: -s ALLOW_MEMORY_GROWTH=1 --preload-file '${RESOURCE_PATH}@' --no-entry -sENVIRONMENT='web' -sUSE_ES6_IMPORT_META=0 -sEXPORT_NAME='WWasmModule' --std=c++20 -sEXPORTED_FUNCTIONS='[\"_malloc\", \"_free\" ]' -sEXPORTED_RUNTIME_METHODS='[\"ccall\", \"cwrap\" ]' --bind")
        append_sample_base_emscripten_source(${APP_NAME})
        set_target_properties(${APP_NAME} PROPERTIES SUFFIX ".mjs")

    endif()

    if(DILIGENT_INSTALL_SAMPLES)
        # Install instructions
        file(RELATIVE_PATH TUTORIAL_REL_PATH "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
        install(TARGETS     ${APP_NAME}
                DESTINATION "${CMAKE_INSTALL_BINDIR}/${TUTORIAL_REL_PATH}/$<CONFIG>")

        if(PLATFORM_LINUX OR PLATFORM_WIN32)
            install(DIRECTORY   "${CMAKE_CURRENT_SOURCE_DIR}/assets/"
                    DESTINATION "${CMAKE_INSTALL_BINDIR}/${TUTORIAL_REL_PATH}/$<CONFIG>")
        endif()

        if(PLATFORM_WIN32)
            get_supported_backends(BACKEND_LIBRARIES)
            install(TARGETS  ${BACKEND_LIBRARIES}
                    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}/${TUTORIAL_REL_PATH}/$<CONFIG>"
                    LIBRARY DESTINATION "${CMAKE_INSTALL_BINDIR}/${TUTORIAL_REL_PATH}/$<CONFIG>"
                    ARCHIVE DESTINATION "${CMAKE_INSTALL_BINDIR}/${TUTORIAL_REL_PATH}/$<CONFIG>")
        endif()

        if(PLATFORM_LINUX)
            set_target_properties(${APP_NAME} PROPERTIES
                INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/${DILIGENT_CORE_DIR}/${CMAKE_BUILD_TYPE}"
            )
        endif()
    endif()

endfunction()

set(INCLUDE
  src/lib.hpp
)

set(SOURCE
  src/main.cpp
  src/lib.cpp
)

set(SHADERS)
set(ASSETS)

# add_executable(canvas-lib WIN32 src/main.cpp)
# # target_compile_options(canvas-lib PROPERTIES LINK_FLAGS " --bind")
# set_target_properties(canvas-lib PROPERTIES LINK_FLAGS "--no-entry -sENVIRONMENT='web' -sUSE_ES6_IMPORT_META=0 -sEXPORT_NAME='WWasmModule' --std=c++20 -sEXPORTED_FUNCTIONS='[\"_malloc\", \"_free\" ]' -sEXPORTED_RUNTIME_METHODS='[\"ccall\", \"cwrap\" ]' --bind")

add_sample_app("canvas-lib" "sus" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
# 
# target_link_libraries(HelloDiligent
# PRIVATE
# Diligent-GraphicsEngineD3D11-shared
# Diligent-GraphicsEngineOpenGL-shared
# Diligent-GraphicsEngineD3D12-shared
# Diligent-GraphicsEngineVk-shared
# )
# # copy_required_dlls(HelloDiligent)