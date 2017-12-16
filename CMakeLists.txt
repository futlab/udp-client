cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(udp-client)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Eigen3 REQUIRED)
find_package(PCL REQUIRED COMPONENTS common io)
find_package(Qt3DPointcloudRenderer REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2 Widgets 3DCore 3DQuick 3DRender 3DQuickRender)

set(SOURCE
    src/main.cpp
	include/backend.h		src/backend.cpp
	include/task.h			src/task.cpp
	include/scan.h			src/scan.cpp
	include/connection.h	src/connection.cpp
	include/interfaces.h	src/interfaces.cpp
)

set(QML
    qml/main.qml
)

add_executable(udp-client ${SOURCE})

qt5_use_modules(udp-client Core Gui Qml Quick QuickControls2)

target_include_directories(${PROJECT_NAME} PRIVATE PRIVATE ${PCL_INCLUDE_DIRS})

include_directories(include ${Qt3DPointcloudRenderer_INCLUDE_DIRS} )

target_link_libraries(udp-client Qt3DPointcloudRenderer Eigen3::Eigen ${PCL_LIBRARIES})
