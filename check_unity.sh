#!/bin/bash

if [ -d 'unity' ]; then
  echo 'unity directory exists with:'
  ls 'unity'
  cd 'unity'
  if [ ! -d 'src' ]; then
    git 'clone' 'https://github.com/ThrowTheSwitch/Unity.git' '.'
  fi
  if [ -d 'src' ]; then
    echo 'src directory exists with:'
    ls 'src'
  fi
  cd ..
fi

if [ ! -d 'unity' ]; then
  # Control will enter here if $DIRECTORY doesn't exist.
  ls
  git 'clone' 'https://github.com/ThrowTheSwitch/Unity.git' 'unity'
  sh ./check_unity.sh
fi
exit 0
