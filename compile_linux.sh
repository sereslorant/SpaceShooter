
mkdir build
cd build

mkdir linux
cd linux

cmake ../../old/

make

cp -r ../../old/Content/    ./Content/
cp -r ../../old/Map/        ./Map/
cp -r ../../old/Script/     ./Script/
