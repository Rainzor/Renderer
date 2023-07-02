#---------------------------------------------------------------------------------------------------
# CMake Build Configuration for the Ray Tracing Weekend Series
#
# See README.md for guidance.
#---------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.25)
project(Render)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set to c++11
set ( CMAKE_CXX_STANDARD 17 )
#Change Qt Path
set(CMAKE_PREFIX_PATH "D:/Qt/5.15.2/mingw81_64")

find_package(OpenMP)
if (OPENMP_FOUND)
    message("OPENMP FOUND")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/common
        ${CMAKE_CURRENT_SOURCE_DIR}/include/shapes
        ${CMAKE_CURRENT_SOURCE_DIR}/include/external
)


file(GLOB_RECURSE SOURCE_FILES
        "src/*.cpp"
        )

add_executable(Renderer main.cpp mainwindow.cpp ${SOURCE_FILES})
add_executable(Example ./example/example.cpp ${SOURCE_FILES})
add_executable(TEST ./test.cpp ${SOURCE_FILES})

target_link_libraries(Renderer
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET Renderer POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:Renderer>/plugins/platforms/")
        add_custom_command(TARGET Renderer POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:Renderer>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET Renderer POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:Renderer>")
    endforeach (QT_LIB)
endif ()