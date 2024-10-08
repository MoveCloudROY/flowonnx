file(GLOB_RECURSE _src CONFIGURE_DEPENDS *.cpp)

foreach(TEST_FILE IN LISTS _src)
    message(STATUS "Find File: ${TEST_FILE}")
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE ABSOLUTE)
    message(STATUS "Configure Target: ${TEST_NAME}")
    add_executable(${TEST_NAME} ${TEST_FILE} )
    target_include_directories(${TEST_NAME} PRIVATE .)
    target_compile_options(${TEST_NAME}  PRIVATE -fno-omit-frame-pointer)
    target_compile_options(${TEST_NAME}  PRIVATE -fsanitize=address)
    target_link_options(${TEST_NAME}  PRIVATE -fsanitize=address)

    target_link_libraries(${TEST_NAME} PRIVATE flowonnx::flowonnx)

endforeach()