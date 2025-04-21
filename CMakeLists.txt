cmake_minimum_required(VERSION 3.20)
project(VisibleSim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-long-long -Wpedantic")

# Set the following environment variables before running CMake:
# bash
# export CC=/opt/homebrew/opt/llvm/bin/clang
# export CXX=/opt/homebrew/opt/llvm/bin/clang++
# export LDFLAGS="-L/opt/homebrew/opt/libomp/lib"
# export CPPFLAGS="-I/opt/homebrew/opt/libomp/include"
# export PATH="/opt/homebrew/opt/llvm/bin:$PATH"
# Optionally, you can set OpenMP_ROOT to help CMake find libomp:
# bash
# export OpenMP_ROOT=/opt/homebrew/opt/libomp
# 4. Run CMake with the Correct Compiler
# Now, run CMake as usual:
# bash
# cmake -B build .

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

if(WIN32)
    message(STATUS ">>> Win32")
    add_compile_definitions(USE_FREEGLUT)
    add_compile_definitions("ROOT_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\"")

    # message(STATUS "Installation dir:" ${CMAKE_CURRENT_SOURCE_DIR})
    # #[[add_compile_definitions(DEBUG_GRAPHICS)]]

    # set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "../simulatorCore/lib")
    IF(MSVC)
        message(STATUS "+Visual studio")
        set(GLUT_DIR F:/freeglut-3.0.0)
        set(CMAKE_PREFIX_PATH FREEGLUT)

        set(FREEGLUT_INCLUDE_DIRS F:/freeglut-3.0.0/include)
        set(FREEGLUT_LIBRARY_DIRS F:/freeglut-3.0.0/cmake-build-debug/lib)
        set(FREEGLUT_LIBRARIES libfreeglut_static.a)

        find_package(OPENGL REQUIRED)

        include_directories(${FREEGLUT_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${GLUT_INTERFACE_DIRS})
        link_directories(${FREEGLUT_LIBRARY_DIRS} ${OPENGL_LIBRARY_DIRS} ${GLUT_INTERFACE_LIB_DIRS})
        add_definitions(${FREEGLUT_DEFINITIONS} ${OPENGL_DEFINITIONS})
        add_executable(${PROJECT_NAME} ${PROJECT_FILES})
        target_link_libraries(${PROJECT_NAME} ${FREEGLUT_LIBRARIES} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} glutInterface.lib)
    ENDIF(MSVC)

    IF(MINGW)
        message(STATUS "+MinGW")

        # Include CPM.cmake for dependency management
        set(CPM_DOWNLOAD_VERSION 0.40.8)
        set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

        if(NOT(EXISTS ${CPM_DOWNLOAD_LOCATION}))
            message(STATUS "Downloading CPM.cmake")
            file(DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
        endif()

        include(${CPM_DOWNLOAD_LOCATION})
        set(BUILD_SHARED_LIBS OFF)

        # Fetch dependencies
        CPMAddPackage(
            NAME glew
            GITHUB_REPOSITORY Perlmint/glew-cmake
            GIT_TAG glew-cmake-2.2.0
        )
        CPMAddPackage(
            NAME freeglut
            GITHUB_REPOSITORY freeglut/freeglut
            VERSION 3.6.0
        )
        CPMAddPackage(
            NAME muparser
            GITHUB_REPOSITORY beltoforion/muparser
            VERSION 2.3.5
            OPTIONS "BUILD_SHARED_LIBS=OFF"
        )

        # #########################################################
        # #####Change Paths to match your library installation#####
        # #########################################################
        set(FREEGLUT_INCLUDE_DIRS build/_deps/freeglut-src/include)
        set(FREEGLUT_LIBRARY_DIRS build/_deps/freeglut-build/lib)
        add_compile_definitions(GLEW_STATIC)
        set(GLEW_INCLUDE_DIRS build/_deps/glew-src/include)
        set(GLEW_LIBRARY_DIRS build/_deps/glew-build/lib)
        set(muparser_INCLUDE_DIRS build/_deps/muparser-src/include)
        set(muparser_LIBRARY_DIRS build/_deps/muparser-build)

        # #########################################################
        # #########################################################
        # #########################################################
        set(ROBOTS_LIBRARY_DIRS ${CMAKE_SOURCE_DIR}/lib)

        link_directories(${ROBOTS_LIBRARY_DIRS} ${FREEGLUT_LIBRARY_DIRS} ${GLEW_LIBRARY_DIRS} ${muparser_LIBRARY_DIRS})
        include_directories(${FREEGLUT_INCLUDE_DIRS} ${GLUT_LIBRARIES} ${GLEW_INCLUDE_DIRS} ${muparser_INCLUDE_DIRS})
    ENDIF(MINGW)
ENDIF(WIN32)

IF(LINUX)
    message(STATUS ">>> Linux")
    include_directories(${glfw_SOURCE_DIR}/include ${glew_SOURCE_DIR}/include ${muparser_SOURCE_DIR}/include)
    target_link_libraries(${PROJECT_NAME} glfw glew muparser pthread)
endif()

IF(APPLE)
    message(STATUS ">>> APPLE SILICON")
    find_package(OpenGL REQUIRED)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
    set(GLEW_INCLUDE_DIRS /opt/homebrew/include)
    set(GLEW_LIBRARY_DIRS /opt/homebrew/lib)
    set(GLUT_INCLUDE_DIRS /opt/homebrew/include)
    set(GLUT_LIBRARY_DIRS /opt/homebrew/lib)
    set(muparser_LIBRARY_DIRS /opt/homebrew/lib)
    set(muparser_INCLUDE_DIRS /opt/homebrew/include)
    link_directories(${ROBOTS_LIBRARY_DIRS} ${FREEGLUT_LIBRARY_DIRS} ${GLEW_LIBRARY_DIRS} ${muparser_LIBRARY_DIRS})
    include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${muparser_INCLUDE_DIRS})
ENDIF(APPLE)

include_directories(simulatorCore/src)
ADD_SUBDIRECTORY(simulatorCore)

# cpm_export_variables("-DGLEW_STATIC")
# add_definitions(-DGLEW_STATIC)
# set(GLEW_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glew/include")

add_executable(simpleColorBB applicationsSrc/simpleColorBB/simpleColorBB.cpp applicationsSrc/simpleColorBB/simpleColorCodeBB.cpp)
target_link_libraries(simpleColorBB BlinkyBlocks)
set_target_properties(simpleColorBB PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/applicationsBin/simpleColorBB)

# add_executable(slidingCubesExample applicationsSrc/slidingCubesExample/slidingCubesExample.cpp applicationsSrc/slidingCubesExample/slidingCubesExampleBlockCode.cpp)
# target_link_libraries(slidingCubesExample SlidingCubes)
# set_target_properties(slidingCubesExample PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/applicationsBin/slidingCubesExample)
