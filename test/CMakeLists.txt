cmake_minimum_required(VERSION 2.8)
add_definitions("-O3 -g -std=c++11 -stdlib=libc++ -DDEBUG")

include_directories(
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/lib 
  ${CMAKE_CURRENT_LIST_DIR}/common
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
  lib/base64/base64.cc
  lib/crypt/crypt.cc
  lib/exception/base_exception.cc 
  lib/log/logger.cc 
  lib/random/random.cc
  common/network/websocket/client/handshake_helper.cc 
  common/network/websocket/client/websocket_async.cc 
  common/network/websocket/client/client_frame_data.cc 
  common/network/websocket/secret/secret_key.cc
  common/network/websocket/ws_packet_data_helper.cc
  lib/network/io/async_socket.cc
  lib/network/io/async_ssl_socket.cc
  lib/network/io/socket.cc
  lib/network/io/ssl_socket.cc
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
  ssl
  crypto
)

