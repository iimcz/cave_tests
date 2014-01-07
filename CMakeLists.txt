cmake_minimum_required(VERSION 2.8.10)

enable_language(CXX)
project (cave_tests)



SET(CAVELIB_DIR "/usr/local/CAVE/" CACHE PATH "Path to top dir of Cavelib installation")
OPTION (USE_CAVELIB "Build cavelib version." ON)

#IF (WIN32)
#SET(GLEW_LIB "" CACHE FILEPATH "Path to Glew32.lib")
#SET(GLEW_INCLUDE "" CACHE PATH "Path to Glew includes")
#SET(GLEW_LIBS ${GLEW_LIB} opengl32.lib glu32.lib )
#ENDIF()

include_directories( ${CMAKE_SOURCE_DIR}/src )

IF (UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++0x")
ENDIF ()


SET (EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
add_subdirectory(src)