cmake_minimum_required (VERSION 2.8.11)

include("my_paths.cmake")
set(LIB_NAME "yancey-base" )

set(YANCEY_BASE_INCLUDE "${YANCEY_BASE_DIR}/include")

add_library (${LIB_NAME} STATIC
${YANCEY_BASE_DIR}/src/YanceyBase.cpp
${YANCEY_BASE_DIR}/src/PicoGame.cpp
${YANCEY_BASE_DIR}/src/SSD1306Game.cpp
${YANCEY_BASE_DIR}/src/YanceyShape.cpp
${SSD1306_BASE_DIR}/ssd1306.c
)
target_compile_definitions(${LIB_NAME} PUBLIC DEBUG)

set_target_properties(${LIB_NAME} PROPERTIES PREFIX "")
#target_compile_definitions(${LIB_NAME} PUBLIC DEBUG)


target_include_directories (${LIB_NAME} PUBLIC
  ${YANCEY_BASE_INCLUDE}
${SSD1306_BASE_DIR}
  )

target_link_libraries(${LIB_NAME} pico_stdlib hardware_i2c)

