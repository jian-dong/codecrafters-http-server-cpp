cmake_minimum_required(VERSION 3.13)

project(http-server-starter-cpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)

set(HTTP_SERVER_SOURCES
    src/client_session.cpp
    src/file_storage.cpp
    src/gzip_compressor.cpp
    src/http_request.cpp
    src/http_request_parser.cpp
    src/http_response.cpp
    src/http_server.cpp
    src/request_handler.cpp
    src/socket_writer.cpp
    src/string_utils.cpp)

add_library(http_server_lib ${HTTP_SERVER_SOURCES})
target_include_directories(http_server_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(http_server_lib PUBLIC Threads::Threads ZLIB::ZLIB)

add_executable(http-server src/server_main.cpp)
target_link_libraries(http-server PRIVATE http_server_lib)

option(BUILD_TESTING "Build tests" ON)

if(BUILD_TESTING)
  include(CTest)
  enable_testing()

  if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/googletest/CMakeLists.txt)
    message(
        FATAL_ERROR
          "googletest submodule is missing. Run: git submodule update --init --recursive")
  endif()

  add_subdirectory(third_party/googletest)

  add_executable(
      server_tests
      tests/http_request_parser_test.cpp
      tests/http_response_test.cpp
      tests/request_handler_test.cpp)

  target_link_libraries(server_tests PRIVATE http_server_lib GTest::gtest_main)

  include(GoogleTest)
  gtest_discover_tests(server_tests)
endif()
