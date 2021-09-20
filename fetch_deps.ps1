$tmp_dir = "tmp"

echo "Removing old lib folder (if it exists)..."
(rm -r -force lib) 2>&1>$null

mkdir lib > $null
cd lib

echo "Fetching dependencies..."
git clone https://gitlab.cse.unsw.edu.au/COMP3421/chicken3421-package $tmp_dir
cd $tmp_dir || exit


if($args.count -eq 0) {
  echo "No generator given. Falling back to system default."
  $GENERATOR_ARG = ""
} else {
  $gen =  $args[0]
  echo "Using generator: $gen"
  $GENERATOR_ARG = "-G$gen"
}

cmake -S . -B $tmp_dir "$GENERATOR_ARG" && cmake --build $tmp_dir

cd ..
cmake --install $tmp_dir/$tmp_dir --prefix .

echo "Cleaning up..."
rm -r -force $tmp_dir


