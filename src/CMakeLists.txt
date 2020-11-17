file(GLOB horizon_SOURCES main.cpp horizon.cpp app/*.cpp common/*.cpp gui/*.cpp gui/audio_effects/*.cpp network/*.cpp)


file(GLOB horizon_DARWIN_SOURCES common/darwin/*.m common/darwin/*.h common/darwin/*.cpp)

file(GLOB horizon_FORMS gui/*.ui gui/audio_effects/*.ui
    )
set(horizon_RESOURCES ../misc/assets/resources.qrc ../misc/assets/core.qrc
                      ../misc/assets/themes.qrc)

include_directories(${CMAKE_CURRENT_BINARY_DIR} .)
include_directories(${Qt5Widgets_INCLUDES})
include_directories("../lib/LabSound/include")
add_definitions(${Qt5Widgets_DEFINITIONS})

qt5_wrap_ui(horizon_FORMS_HEADERS ${horizon_FORMS})
qt5_add_resources(horizon_RESOURCES_RCC ${horizon_RESOURCES})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

set(horizon_COMPILE
    ${horizon_COMPILE} ${horizon_SOURCES} ${horizon_FORMS_HEADERS}
    ${horizon_RESOURCES_RCC} ${horizon_QM})

if(NOT APPLE)
  add_executable(${HORIZON_EXECUTABLE_NAME} ${horizon_COMPILE})
else()
  list(APPEND horizon_COMPILE ${horizon_DARWIN_SOURCES})
  # On Macs we must ensure the required frameworks are linked
  find_library(AUDIOUNITS_LIBRARY "AudioUnit")
  find_library(AUDIOTOOLBOX_LIBRARY "AudioToolbox")
  find_library(ACCELERATE_LIBRARY "Accelerate")
  find_library(COCOA_LIBRARY "Cocoa")
  find_library(COREAUDIO_LIBRARY "CoreAudio")
  find_library(CORESERVICES_LIBRARY "CoreServices")

  mark_as_advanced(AUDIOUNITS_LIBRARY AUDIOTOOLBOX_LIBRARY ACCELERATE_LIBRARY
                   COCOA_LIBRARY COREAUDIO_LIBRARY CORESERVICES_LIBRARY)
  list(
    APPEND
    horizon_LIBRARIES
    "${AUDIOUNITS_LIBRARY}"
    "${AUDIOTOOLBOX_LIBRARY}"
    "${ACCELERATE_LIBRARY}"
    "${COCOA_LIBRARY}"
    "${COREAUDIO_LIBRARY}"
    "${CORESERVICES_LIBRARY}")

  set(HORIZON_VERSION_REGEX "^v([0-9]+)\.([0-9]+)\.(.*)$")
  string(REGEX REPLACE "${HORIZON_VERSION_REGEX}" "\\1"
                       CPACK_PACKAGE_VERSION_MAJOR "${horizon_version}")
  string(REGEX REPLACE "${HORIZON_VERSION_REGEX}" "\\2"
                       CPACK_PACKAGE_VERSION_MINOR "${horizon_version}")
  string(REGEX REPLACE "${HORIZON_VERSION_REGEX}" "\\3"
                       CPACK_PACKAGE_VERSION_PATCH "${horizon_version}")

  set(MACOSX_BUNDLE_BUNDLE_NAME "${HORIZON_EXECUTABLE_NAME}")
  set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.horizon-daw.horizon")
  set(MACOSX_BUNDLE_INFO_STRING "Horizon Digital Audio Workstation")
  set(MACOSX_BUNDLE_ICON_FILE "icon.icns")
  set(MACOSX_BUNDLE_LONG_VERSION_STRING "${horizon_version}")
  set(MACOSX_BUNDLE_SHORT_VERSION_STRING
      "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
  set(MACOSX_BUNDLE_BUNDLE_VERSION "${horizon_version}")

  set_source_files_properties(
    "${CMAKE_CURRENT_SOURCE_DIR}/../misc/assets/app_icon/icon.icns"
    PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  set(horizon_COMPILE ${horizon_COMPILE}
                      "${CMAKE_CURRENT_SOURCE_DIR}/../misc/assets/app_icon/icon.icns")

  add_executable(${HORIZON_EXECUTABLE_NAME} MACOSX_BUNDLE ${horizon_COMPILE})
endif()

add_dependencies(${HORIZON_EXECUTABLE_NAME} generate_version_header)

# Qt modules
list(APPEND horizon_qt_modules "WebSockets" "WebEngine" "WebEngineWidgets")
foreach(horizon_qt_module ${horizon_qt_modules})
  message(STATUS "Finding QT Plugin ${horizon_qt_module}")
  find_package(Qt5${horizon_qt_module} REQUIRED)
  list(APPEND horizon_LIBRARIES Qt5::${horizon_qt_module})
endforeach()

# LabSound
list(APPEND horizon_LIBRARIES LabSound)

# libnyquist
list(APPEND horizon_LIBRARIES libnyquist)

# libwavpack
list(APPEND horizon_LIBRARIES libwavpack)

set_target_properties(${HORIZON_EXECUTABLE_NAME} PROPERTIES COMPILE_DEFINITIONS "${horizon_DEFINITIONS}")

set_target_properties(${HORIZON_EXECUTABLE_NAME} PROPERTIES LINK_FLAGS "${horizon_LINK_FLAGS}")

foreach(module ${horizon_LIBRARIES})
  message(STATUS "Target link module ${module}")
endforeach()

target_link_libraries(${HORIZON_EXECUTABLE_NAME} ${horizon_LIBRARIES})

install(TARGETS ${HORIZON_EXECUTABLE_NAME}
    BUNDLE DESTINATION . COMPONENT Runtime
    RUNTIME DESTINATION bin COMPONENT Runtime
    )

if (NOT APPLE)

else()
    set_target_properties(${HORIZON_EXECUTABLE_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/../shared/Info.plist")

    set(qtconf_dest_dir "${HORIZON_EXECUTABLE_NAME}.app/Contents/Resources")

    macro(install_qt5_plugin _qt_plugin_name _qt_plugins_var)
        get_target_property(_qt_plugin_path "${_qt_plugin_name}" LOCATION)
        if(EXISTS "${_qt_plugin_path}")
            get_filename_component(_qt_plugin_file "${_qt_plugin_path}" NAME)
            get_filename_component(_qt_plugin_type "${_qt_plugin_path}" PATH)
            get_filename_component(_qt_plugin_type "${_qt_plugin_type}" NAME)
            set(_qt_plugin_dest "${plugin_dest_dir}/${_qt_plugin_type}")
            install(FILES "${_qt_plugin_path}" DESTINATION "${_qt_plugin_dest}" COMPONENT Runtime)
            set(${_qt_plugins_var}
                "${${_qt_plugins_var}};\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${_qt_plugin_dest}/${_qt_plugin_file}")
        else()
            message(FATAL_ERROR "QT plugin ${_qt_plugin_name} not found")
        endif()
    endmacro()

    # Install needed Qt plugins

    foreach(_horizon_qt_module ${horizon_qt_modules} "Gui" "Widgets" "SVG" "WebSockets" "WebEngineCore" "WebEngineWidgets")
        set(_module_plugins "${Qt5${_horizon_qt_module}_PLUGINS}")
        foreach(_plugin ${_module_plugins})
            message(STATUS "Installing QT Plugin ${_plugin}")
            install_qt5_plugin("${_plugin}" FIXUP_BUNDLE_QT_PLUGINS)
        endforeach()
    endforeach()


    # install a qt.conf file
    # this inserts some cmake code into the install script to write the file
    install(CODE "
        file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${qtconf_dest_dir}/qt.conf\" \"[Paths]\nPlugins = PlugIns\nImports = Resources/qml\nQml2Imports = Resources/qml\n\")
        "
        COMPONENT Runtime
        )

    set(FIXUP_BUNDLE_APPS "\${CMAKE_INSTALL_PREFIX}/${HORIZON_EXECUTABLE_NAME}.app")

    get_property(_horizon_installed_plugins GLOBAL PROPERTY HORIZON_INSTALLED_PLUGINS)

    # Directories to look for dependencies
    set(FIXUP_BUNDLE_DEP_DIRS "${CMAKE_BINARY_DIR};${QT_LIBRARY_DIRS}")

    # Get reference to deployqt
    get_target_property(uic_location Qt5::uic IMPORTED_LOCATION)
    get_filename_component( _dir ${uic_location} DIRECTORY)
    set(deployqt "${_dir}/macdeployqt")
    if(NOT EXISTS ${deployqt})
      message(FATAL_ERROR "Failed to locate deployqt executable: [${deployqt}]")
    endif()

    # Execute deployqt during package creation
    # See https://doc.qt.io/qt-5/osx-deployment.html#macdeploy
    install(CODE "set(deployqt \"${deployqt}\")" COMPONENT Runtime)
    install(CODE [===[
    execute_process(COMMAND "${deployqt}" "${CMAKE_INSTALL_PREFIX}/Horizon.app")
    ]===] COMPONENT Runtime)

    install(CODE "
        include(BundleUtilities)
        set(BU_CHMOD_BUNDLE_ITEMS ON)
        fixup_bundle(\"${FIXUP_BUNDLE_APPS}\" \"${FIXUP_BUNDLE_QT_PLUGINS};${_horizon_installed_plugins}\" \"${FIXUP_BUNDLE_DEP_DIRS}\")
        verify_app(\"${FIXUP_BUNDLE_APPS}\")
        "
        COMPONENT Runtime
        )

    set(CPACK_GENERATOR "DragNDrop")
    include(CPack)
endif()
