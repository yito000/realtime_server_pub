#!/bin/bash

if [ ! -d "./cpp_out" ]; then
  mkdir cpp_out
fi

`pwd`/protoc --cpp_out=cpp_out *.proto

