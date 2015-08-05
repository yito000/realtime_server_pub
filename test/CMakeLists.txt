cmake_minimum_required(VERSION 2.8)
add_definitions("-Os -std=c++11 -stdlib=libc++ -DDEBUG")

include_directories(
  ${CMAKE_CURRENT_LIST_DIR}/lib 
  ${CMAKE_CURRENT_LIST_DIR}/libs/include 
  ${CMAKE_CURRENT_LIST_DIR}/external_src/flatbuffers
)

link_directories(
  ${CMAKE_CURRENT_LIST_DIR}/libs/lib
)

add_executable(
  test_app 
  main3.cc 
  jikken.pb.cc 
  uiid.pb.cc 
  lib/exception/base_exception.cc 
  lib/log/logger.cc 
  lib/network/websocket/client/handshake_helper.cc 
  lib/network/websocket/client/websocket.cc 
  lib/network/websocket/client/websocket_async.cc 
  lib/network/websocket/client/frame_data.cc 
  lib/network/io/socket.cc
  external_src/flatbuffers/idl_gen_text.cpp
  external_src/flatbuffers/idl_parser.cpp
  external_src/flatbuffers/reflection.cpp
)

target_link_libraries(
  test_app 
  boost_system-mt 
  boost_thread-mt 
  boost_regex-mt 
  protobuf
)

