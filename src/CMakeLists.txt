
find_package(X11)
find_package(OpenGL)
find_package(GLEW)

IF(USE_CAVELIB)
    add_definitions(-DCAVE_THREAD -DCAVE_VERSION)
    SET(LIBS ${LIBS} -lcave_ogl_mt)
ELSE()
    find_package(GLUT)
    SET(LIBS ${LIBS} ${GLUT_LIBRARIES})
ENDIF()


link_directories ( ${CAVELIB_DIR}/lib )
include_directories ( ${CAVELIB_DIR}/include )

add_executable(triangles triangles.cpp
                        Application.h Application.cpp
                        Particle.h Particle.cpp
                        Scene.h Scene.cpp
                        Shader.h Shader.cpp
                        )


SET(LIBS ${LIBS} -pthread ${X11_LIBRARIES} ${X11_Xi_LIB} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} )

target_link_libraries ( triangles  ${LIBS} )

install(TARGETS triangles RUNTIME DESTINATION bin)