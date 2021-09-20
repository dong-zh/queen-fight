#! /usr/bin/env sh

tmp_dir="tmp"

echo "Removing old lib folder (if it exists)..."
rm -rf lib

mkdir lib
cd lib || exit

echo "Fetching dependencies..."
git clone https://gitlab.cse.unsw.edu.au/COMP3421/chicken3421-package ${tmp_dir}
cd ${tmp_dir} || exit

if [ $# -eq 0 ];
then
  echo "No generator given. Falling back to system default."
  GENERATOR_ARG=""
else
  echo "Using generator: $1"
  GENERATOR_ARG="-G$1"
fi

cmake -S . -B ${tmp_dir} "$GENERATOR_ARG" \
&& cmake --build ${tmp_dir}                        \

cd ..
cmake --install ${tmp_dir}/${tmp_dir} --prefix .

echo "Cleaning up..."
rm -rf ${tmp_dir}
