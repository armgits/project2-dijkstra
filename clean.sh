#!/bin/bash

set -e

if [ -d build/ ]
then
  rm -r build/
fi

if [ -d results/ ]
then
  for filename in results/*; do
    file_extension="${filename##*.}"

    if [[ $file_extension == txt ]]; then
      rm ${filename}
    fi
  done
fi
