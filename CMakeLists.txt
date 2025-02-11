cmake_minimum_required(VERSION 3.28)

project(c-testing-and-mocking C)

if(NOT CMAKE_C_COMPILER_ID STREQUAL "GNU")
  message(FATAL_ERROR "Only GCC is supported at this moment")
endif()

add_library(c-testing-and-mocking STATIC EXCLUDE_FROM_ALL
  framework.c
  mock.c
)

include(CheckCCompilerFlag)

check_c_compiler_flag("-std=c99" COMPILER_SUPPORTS_STD_C99)
if(COMPILER_SUPPORTS_STD_C99)
  target_compile_options(c-testing-and-mocking PRIVATE -std=c99)
else()
  message(WARNING "Complier flag '-std=c99' is not supported")
endif()

check_c_compiler_flag("-O3" COMPILER_SUPPORTS_O3)
if(COMPILER_SUPPORTS_O3)
  target_compile_options(c-testing-and-mocking PRIVATE -O3)
else()
  message(WARNING "Complier flag '-O3' is not supported")
endif()

target_include_directories(c-testing-and-mocking PUBLIC .)


if (C_TESTING_AND_MOCKING_TEST)
  set(CMAKE_EXPORT_COMPILE_COMMANDS on)

  project(c-testing-and-mocking-test C)

  add_executable(c-testing-and-mocking-test EXCLUDE_FROM_ALL
    test/main.c
  )

  target_compile_options(c-testing-and-mocking-test PUBLIC
    -std=c99
    -flto
    -fanalyzer
    -Wall
    -Wextra
    -pedantic
    -Werror
  )

  target_link_libraries(c-testing-and-mocking-test PUBLIC
    c-testing-and-mocking
  )

  add_custom_command(
    TARGET c-testing-and-mocking-test
    POST_BUILD
    COMMAND c-testing-and-mocking-test
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    VERBATIM
  )
endif()
