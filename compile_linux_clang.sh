
mkdir build
cd build

mkdir linux_clang
cd linux_clang

cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ ../../old/

make

cp -r ../../old/Content/    ./Content/
cp -r ../../old/Map/        ./Map/
cp -r ../../old/Script/     ./Script/
