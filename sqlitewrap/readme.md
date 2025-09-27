## linux

mkdir build && cd build
cmake ..
make
sudo make install

## Windows (with MinGW):

mkdir build && cd build
cmake -G "MinGW Makefiles" ..
mingw32-make
sudo mingw32-make install

## supports both Debug and Release builds

cmake -D CMAKE_BUILD_TYPE=Debug ..

## sqlite3

SQLite3 is handled via find_package(Sqlite3), and must be available on the system.

## install

2. Install SQLite3 for MinGW
   MSYS2: Run pacman -S mingw-w64-x86_64-sqlite3
   vcpkg: vcpkg install sqlite3:x64-mingw-static
   Manual: Copy sqlite3.h, sqlite3.lib, libsqlite3.a to project and adjust CMakeLists.txt to point to custom paths.

# CMAKE Example Windows Debug

C:\msys64\mingw64\bin\cmake.exe -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=C:/msys64/mingw64/bin/gcc.exe
-DCMAKE_CXX_COMPILER=C:/msys64/mingw64/bin/g++.exe -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=conan_provider.cmake -G "MinGW
Makefiles" -S C:\git\Oblivion2-XRM\sqlitewrap -B C:\git\Oblivion2-XRM\sqlitewrap\cmake-build-debug

# CMAKE Example Windows Release

C:\msys64\mingw64\bin\cmake.exe -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=C:/msys64/mingw64/bin/gcc.exe
-DCMAKE_CXX_COMPILER=C:/msys64/mingw64/bin/g++.exe -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=conan_provider.cmake -G "MinGW
Makefiles" -S C:\git\Oblivion2-XRM\sqlitewrap -B C:\git\Oblivion2-XRM\sqlitewrap\cmake-build-release

# Build Debug

C:\msys64\mingw64\bin\cmake.exe --build C:\git\Oblivion2-XRM\sqlitewrap\cmake-build-debug --target all -- -j 30

# Build Release

C:\msys64\mingw64\bin\cmake.exe --build C:\git\Oblivion2-XRM\sqlitewrap\cmake-build-release --target all -- -j 30