cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(qtmpv VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt5 REQUIRED COMPONENTS Widgets)

find_package(PkgConfig REQUIRED)
pkg_check_modules(libmpv REQUIRED mpv)

set(PROJECT_SOURCES
  src/main.cpp
  src/qtmpv.h
  src/qtmpv.cpp
  src/overlay/overlaywidget.cpp
  src/overlay/overlaywidget.h
  src/overlay/overlayline.cpp
  src/overlay/overlayline.h
  src/overlay/overlaytext.cpp
  src/overlay/overlaytext.h
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} Qt5::Widgets mpv)

set_target_properties(${PROJECT_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
