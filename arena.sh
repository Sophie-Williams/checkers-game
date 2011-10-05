#!/bin/sh

set -ue

field=$1

mkdir -p arena
rm -f arena/*
cp $field arena/matrix.txt

cd arena

echo "Start:" 
cat matrix.txt 
echo

while [[ $(head -n 1 matrix.txt | cut -d\  -f3) == 'U' ]]; do
  ../bot
  cat matrix.txt
  echo
  sleep 1
done
