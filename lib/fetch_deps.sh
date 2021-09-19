#! /usr/bin/env sh

if [ $# -eq 0 ];
then
  echo "No generator given. Falling back to system default."
  GENERATOR_ARG=""
else
  echo "Using generator: $1"
  GENERATOR_ARG="-G$1"
fi

echo "Fetching dependencies..."

cmake -S . -B tmp "$GENERATOR_ARG"
cmake --build tmp
cmake --install tmp
rm -rf tmp
