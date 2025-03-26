cmake_minimum_required(VERSION 3.15)
project(VisibleSim)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-long-long -Wpedantic")


if (UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif ()

IF (WIN32)
    message(STATUS ">>> Win32")
    add_compile_definitions(USE_FREEGLUT)

    add_compile_definitions("ROOT_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\"")
    message(STATUS "Installation dir:" ${CMAKE_CURRENT_SOURCE_DIR})
    #[[add_compile_definitions(DEBUG_GRAPHICS)]]

    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "../simulatorCore/lib")

    IF (MSVC)
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
    ENDIF (MSVC)

    IF (MINGW)
        message(STATUS "+MinGW")

        ##########################################################
        ######Change Paths to match your library installation#####
        ##########################################################
        set(FREEGLUT_INCLUDE_DIRS C:/VisibleSim/freeglut-3.2.1/include)
        set(FREEGLUT_LIBRARY_DIRS C:/VisibleSim/freeglut-3.2.1/cmake-build-debug/lib)
        add_compile_definitions(GLEW_STATIC)
        set(GLEW_INCLUDE_DIRS C:/VisibleSim/glew-2.1.0/include)
        set(GLEW_LIBRARY_DIRS C:/VisibleSim/glew-2.1.0/build/cmake/cmake-build-debug/lib)
        set(muparser_INCLUDE_DIRS C:/VisibleSim/muparser-2.3.3-1/include)
        set(muparser_LIBRARY_DIRS C:/VisibleSim/muparser-2.3.3-1/cmake-build-debug)

        ##########################################################
        ##########################################################
        ##########################################################
        set(ROBOTS_LIBRARY_DIRS "./simulatorCore/lib")

        link_directories(${ROBOTS_LIBRARY_DIRS} ${FREEGLUT_LIBRARY_DIRS} ${GLEW_LIBRARY_DIRS} ${muparser_LIBRARY_DIRS})
        include_directories(${FREEGLUT_INCLUDE_DIRS} ${GLUT_LIBRARIES} ${GLEW_INCLUDE_DIRS} ${muparser_INCLUDE_DIRS})
    ENDIF (MINGW)
ENDIF (WIN32)

IF (LINUX)
    message(STATUS ">>> Linux")

    find_package(GLUT REQUIRED)
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(GLU REQUIRED)
    include_directories( ${OPENGL_INCLUDE_DIRS} ${GLU_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLU_LIBRARIES} -lmuparser -lpthread)
    if (NOT GLUT_FOUND)
        message(ERROR " GLUT not found!")
    endif (NOT GLUT_FOUND)

    if (NOT OpenGL_FOUND)
        message(ERROR " openGL not found!")
    endif (NOT OpenGL_FOUND)

    include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} )

ENDIF (LINUX)

IF (APPLE)
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
    ENDIF (APPLE)

include_directories(simulatorCore/src)
ADD_SUBDIRECTORY(simulatorCore)

add_executable(simpleColorBB applicationsSrc/simpleColorBB/simpleColorBB.cpp applicationsSrc/simpleColorBB/simpleColorCodeBB.cpp)
target_link_libraries(simpleColorBB BlinkyBlocks ) #
set_target_properties(simpleColorBB PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/applicationsBin/simpleColorBB)
