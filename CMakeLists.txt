cmake_minimum_required(VERSION 3.20)
project(VisibleSim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-long-long -Wpedantic")
include(FetchContent)

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


# Include CPM.cmake for dependency management
set(CPM_DOWNLOAD_VERSION 0.40.8) 
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake")
    file(DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif()

include(${CPM_DOWNLOAD_LOCATION})


# Fetch dependencies
# Fetch dependencies
#CPMAddPackage("gh:glfw/glfw#3.3.8" )
# CPMAddPackage("https://sourceforge.net/projects/glew/files/glew/2.2.0/glew-2.2.0.zip" NAME GLEW)
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
CPMAddPackage("gh:beltoforion/muparser#v2.3.5" )

find_package(OpenMP REQUIRED)

if(OpenMP_C_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Xpreprocessor -fopenmp -I/opt/homebrew/include")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/opt/homebrew/lib -lomp")
endif()




if (UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif ()

if (WIN32)
    message(STATUS ">>> Win32")
    add_compile_definitions(USE_FREEGLUT)
    add_compile_definitions("ROOT_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\"")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "../simulatorCore/lib")
    
    if (MSVC)
        message(STATUS "+Visual Studio")
        include_directories(${glfw_SOURCE_DIR}/include ${glew_SOURCE_DIR}/include ${muparser_SOURCE_DIR}/include)
        target_link_libraries(${PROJECT_NAME} glfw glew muparser)
    endif ()

    if (MINGW)
        message(STATUS "+MinGW")
        include_directories(${glfw_SOURCE_DIR}/include ${glew_SOURCE_DIR}/include ${muparser_SOURCE_DIR}/include)
        target_link_libraries(${PROJECT_NAME} glfw glew muparser)
    endif ()
endif ()

if (LINUX)
    message(STATUS ">>> Linux")
    include_directories(${glfw_SOURCE_DIR}/include ${glew_SOURCE_DIR}/include ${muparser_SOURCE_DIR}/include)
    target_link_libraries(${PROJECT_NAME} glfw glew muparser pthread)
endif ()

IF (APPLE)
    message(STATUS ">>> APPLE SILICON")
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    message(STATUS ">>> GLEW_INCLUDE_DIRS: ${glew_SOURCE_DIR}")

    set(CMAKE_OSX_ARCHITECTURES "arm64")
    # Using directories installed by CPM
    set(GLEW_INCLUDE_DIRS ${glew_SOURCE_DIR}/include)
    set(GLEW_LIBRARY_DIRS ${glew_SOURCE_DIR}/lib)
    set(FREEGLUT_INCLUDE_DIRS ${freeglut_SOURCE_DIR}/include)
    set(FREEGLUT_LIBRARY_DIRS ${freeglut_SOURCE_DIR}/lib)
    # include_directories(${freeglut_SOURCE_DIR}/include/gl)
    link_directories( ${GLEW_LIBRARY_DIRS} ${FREEGLUT_LIBRARY_DIRS} )
    include_directories(${GLEW_INCLUDE_DIRS} ${FREEGLUT_INCLUDE_DIRS})
ENDIF (APPLE)

include_directories(simulatorCore/src)
ADD_SUBDIRECTORY(simulatorCore)

cpm_export_variables("-DGLEW_STATIC")
add_definitions(-DGLEW_STATIC)
set(GLEW_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glew/include")

# IF (APPLE)
#     message(STATUS ">>> APPLE SILICON")
#     find_package(freeglut REQUIRED)
#     target_link_libraries(src  muparser  glew  freeglut OpenGL::GL)
# ENDIF (APPLE)

add_executable(simpleColorBB applicationsSrc/simpleColorBB/simpleColorBB.cpp applicationsSrc/simpleColorBB/simpleColorCodeBB.cpp)
target_link_libraries(simpleColorBB BlinkyBlocks)
set_target_properties(simpleColorBB PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/applicationsBin/simpleColorBB)

# add_executable(slidingCubesExample applicationsSrc/slidingCubesExample/slidingCubesExample.cpp applicationsSrc/slidingCubesExample/slidingCubesExampleBlockCode.cpp)
# target_link_libraries(slidingCubesExample SlidingCubes)
# set_target_properties(slidingCubesExample PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/applicationsBin/slidingCubesExample)
