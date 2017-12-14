#!/bin/sh

if [ $# -ne 1 ]; then
  echo usage: $0 scriptname
  exit
fi
name=$1

g++ `root-config --cflags --libs` $name.C -o ${name}_C
if [ $? -ne 0 ]; then
  exit
fi
./${name}_C
