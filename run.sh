#!/bin/bash

for app in ./test/*
do
  file=${app##*/}
  name=${file%%.*}
  echo $name
  ./build/$name
done
