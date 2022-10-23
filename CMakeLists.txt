# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PROJ_NAME tetrits )
set(CMAKE_BUILD_TYPE Debug)

project("${PROJ_NAME}" C CXX ASM)
#find_package(Box2D REQUIRED)

include_directories(./include
  ${YANCEY_BASE_INCLUDE}
  )

include("yancey-base.cmake")

# Add executable. Default name is the project name, version 0.1
add_executable("${PROJ_NAME}"
  "src/main.cpp"
  "src/Tetrits.cpp"
  )

# Add the standard library to the build
target_link_libraries(${PROJ_NAME} yancey-base )




