project(horizon)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.14")

# set(CMAKE_PREFIX_PATH "C:/Qt/5.14.2/msvc2017_6")

if (APPLE)
    cmake_minimum_required(VERSION 3.1)
    set(HORIZON_EXECUTABLE_NAME "Horizon")
else()
    cmake_minimum_required(VERSION 2.8.12)
    set(HORIZON_EXECUTABLE_NAME ${PROJECT_NAME})
endif()

# C++ 14 Support:
if (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 3.1)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(HORIZON_DEBUG ON)
    add_definitions( -DHORIZON_DEBUG  )
endif()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS webenginewidgets REQUIRED)
list(APPEND horizon_LIBRARIES Qt5::Widgets Qt5::WebEngine Qt5::WebEngineWidgets)

set(horizon_APP_ID      com.horizon-daw.horizon)
set(horizon_ICON_PREFIX misc/assets/app_icon/icon)
set(horizon_ICON_NORMAL misc/assets/app_icon/icon.svg)
set(horizon_DESKTOP     misc/shared/${horizon_APP_ID}.desktop)
set(horizon_APPDATA     misc/shared/${horizon_APP_ID}.appdata.xml)

# Compiling debug version
if(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-long-long")
    set(CMAKE_CXX_FLAGS_DEBUG
        "${CMAKE_CXX_FLAGS_DEBUG} -Wextra -Wall -pedantic -Wfloat-equal -Woverloaded-virtual -Wundef -Wno-inconsistent-missing-destructor-override")
endif()

add_custom_target(
    generate_version_header
    ALL
    COMMAND ${CMAKE_COMMAND}
        -D INPUT_FILE=${CMAKE_CURRENT_SOURCE_DIR}/src/common/version.h.in
        -D OUTPUT_FILE=${CMAKE_CURRENT_BINARY_DIR}/src/common/version.h
        -P ${CMAKE_CURRENT_SOURCE_DIR}/version.cmake
    BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/src/common/version.h
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
set_source_files_properties(
    ${CMAKE_CURRENT_BINARY_DIR}/src/common/version.h
    PROPERTIES GENERATED TRUE
)

add_subdirectory(lib/LabSound)
#add_subdirectory(lib/vst3sdk)
add_subdirectory(src)
