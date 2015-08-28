cmake_minimum_required(VERSION 2.8)
add_definitions("-O3 -g -std=c++11 -stdlib=libc++")

include_directories(
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../src
  ${CMAKE_CURRENT_LIST_DIR}/../src/lib 
  ${CMAKE_CURRENT_LIST_DIR}/../src/common
  ${CMAKE_CURRENT_LIST_DIR}/../libs/include 
  ${CMAKE_CURRENT_LIST_DIR}/../src/external_src/flatbuffers
)

link_directories(
  ${CMAKE_CURRENT_LIST_DIR}/../libs/lib
)

add_executable(
  test_app 
  main3.cc 
#  jikken.pb.cc 
#  uiid.pb.cc 
  ../src/lib/base64/base64.cc
  ../src/lib/crypt/crypt.cc
  ../src/lib/exception/base_exception.cc 
  ../src/lib/log/logger.cc 
  ../src/lib/log/detail/platform/log_detail.cc
  ../src/lib/random/random.cc
  ../src/network/websocket/client/handshake_helper.cc 
  ../src/network/websocket/client/websocket_async.cc 
  ../src/network/websocket/client/client_frame_data.cc 
  ../src/network/websocket/secret/secret_key.cc
  ../src/network/websocket/ws_packet_data_helper.cc
  ../src/lib/network/io/async_socket.cc
  ../src/lib/network/io/async_ssl_socket.cc
  ../src/lib/network/io/socket.cc
  ../src/lib/network/io/ssl_socket.cc
  ../src/external_src/flatbuffers/idl_gen_text.cpp
  ../src/external_src/flatbuffers/idl_parser.cpp
  ../src/external_src/flatbuffers/reflection.cpp
)

target_link_libraries(
  test_app 
  boost_system-mt 
  boost_thread-mt 
  boost_regex-mt 
  ssl
  crypto
)

