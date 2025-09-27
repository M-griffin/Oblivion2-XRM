pacman -Syu

# Install MinGW-w64 toolchain and build tools: 64 BIT

pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-make
pacman -S mingw-w64-x86_64-cmake
pacman -S mingw-w64-x86_64-ninja

pacman -Rs mingw-w64-x86_64-gcc
pacman -Rs mingw-w64-x86_64-make
pacman -Rs mingw-w64-x86_64-cmake
pacman -Rs mingw-w64-x86_64-ninja

# 32 bit

pacman -S mingw-w64-i686-gcc
pacman -S mingw-w64-i686-make
pacman -S mingw-w64-i686-cmake
pacman -S mingw-w64-i686-ninja

pacman -Rs mingw-w64-i686-gcc
pacman -Rs mingw-w64-i686-make
pacman -Rs mingw-w64-i686-cmake
pacman -Rs mingw-w64-i686-ninja

# Install Conan

pacman -S mingw-w64-x86_64-python mingw-w64-x86_64-python-pip

# 32 Bit

pacman -S mingw-w64-i686-python mingw-w64-i686-python-pip

pip install conan

pip show conan

Name: conan
Version: 2.20.1
Summary: Conan C/C++ package manager
Home-page: https://conan.io
Author: JFrog LTD
Author-email: luism@jfrog.com
License: MIT
Location: C:/msys64/mingw64/lib/python3.12/site-packages
Requires: colorama, fasteners, Jinja2, patch-ng, python-dateutil, PyYAML, requests, urllib3
Required-by:

# Program Path for Msys2

C:\Program Files\JetBrains\CLion 2025.1.4\bin