cmake_minimum_required (VERSION 3.11)

project ("star-keyword-core" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

aux_source_directory(src CORE_SOURCES)

add_executable(test-text test/test_text.cc test/my_assert.cc ${CORE_SOURCES})

enable_testing()
add_test(NAME test-text COMMAND test-text CONFIGURATIONS Debug)
