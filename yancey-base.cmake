cmake_minimum_required (VERSION 2.8.11)

set( YANCEY_BASE_DIR "${CMAKE_CURRENT_LIST_DIR}/../yancey-base")
#include("my_paths.cmake")
set( LIB_NAME "yancey-base" )

set(YANCEY_BASE_INCLUDE "${YANCEY_BASE_DIR}/include")

set(SDL2_DIR "/usr/lib/aarch64-linux-gnu/cmake/SDL2")
find_package(SDL2 REQUIRED)
#find_package(SDL2TTF REQUIRED)
find_package(OpenGL REQUIRED)
#find_package(Box2D REQUIRED)

add_library (${LIB_NAME} STATIC
${YANCEY_BASE_DIR}/src/YanceyBase.cpp
${YANCEY_BASE_DIR}/src/SDLGame.cpp
${YANCEY_BASE_DIR}/src/YanceyShape.cpp
${YANCEY_BASE_DIR}/src/YanceyDigit.cpp
${YANCEY_BASE_DIR}/src/YanceyVector.cpp
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

target_link_libraries(${LIB_NAME} ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${SDL2_TTF_LIBRARIES}  )
