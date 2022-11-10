cmake_minimum_required (VERSION 2.8.11)

set( BASE_DIR "/home/roseba/Projects/SOFT/CPP/yancey-base")
set( LIB_NAME "yancey-base" )

set(YANCEY_BASE_INCLUDE "${BASE_DIR}/include")

set(SDL2_DIR "/usr/lib/aarch64-linux-gnu/cmake/SDL2")
find_package(SDL2 REQUIRED)
#find_package(SDL2TTF REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Box2D REQUIRED)

add_library (${LIB_NAME} STATIC
${BASE_DIR}/src/YanceyBase.cpp
${BASE_DIR}/src/SDLGame.cpp
${BASE_DIR}/src/YanceyShape.cpp
${BASE_DIR}/src/YanceyDigit.cpp
)
target_compile_definitions(${LIB_NAME} PUBLIC DEBUG)

set_target_properties(${LIB_NAME} PROPERTIES PREFIX "")
#target_compile_definitions(${LIB_NAME} PUBLIC DEBUG)

target_include_directories (${LIB_NAME} PUBLIC
  ${YANCEY_BASE_INCLUDE}
  ${OPENGL_INCLUDE_DIRS} 
  ${SDL2_INCLUDE_DIRS}
  )

#message()

target_link_libraries(${LIB_NAME} ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${SDL2_TTF_LIBRARIES} -lSDL2_ttf )
