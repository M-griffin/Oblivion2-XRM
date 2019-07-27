#!/bin/bash
#
# Oblivion/2 XRM [LINUX / ARM]
#
# Commandline Build Script 
# For users that want to checkout and build the system 
# Without installing Codelite IDE.
# Michael Griffin 8/12/2018
# Updated 7/14/2019 - Fixed Development folders and updated 
#                   - Packages to their own enviroment folder
#                   - Added Conversion and Unit Tests to Cmdline build
# Updated 7/26/2019 - Added Command line Parameter for CPU cores to speed up compiling

# Save Current Working Directory
cwd=$(pwd)

echo ''
echo -e "\033[1;31m"Build Oblivion2-XRM Demo System and Utils
echo -e "\033[1;31m"Clean build folder and starting compilation
echo -e "\033[0;31m"===========================================
echo -e "\033[0;m"
echo ''

if [ $# -lt 1 ]; then
  echo 1>&2 "$0: not enough arguments"
  echo 1>&2 "use -j1 or -j# [# ex.. number of cpu cores for faster compiling]"
  echo
  exit 2
elif [ $# -gt 1 ]; then
  echo 1>&2 "$0: too many arguments"
  echo
  exit 2
fi

# *** 1. Clean the Project or Make sure it's clean
cd ../linux
make -f SqliteWrapped.mk clean
cd $cwd

echo ''
echo -e "\033[1;31m"Build SqliteWrapped
echo -e "\033[0;31m"===================
echo -e "\033[0;m"
echo ''

# *** 2. Build SqliteWrapped Dependency first.
cd ../linux
make -f SqliteWrapped.mk $1
cat ../sqlitewrap/IError.h ../sqlitewrap/StderrLog.h ../sqlitewrap/SysLogs.h ../sqlitewrap/Database.h ../sqlitewrap/Query.h > ./Debug/libSqliteWrapped.h
cd $cwd

echo ''
echo -e "\033[1;31m"Build Legacy Menu Conversion Tool
echo -e "\033[0;31m"=================================
echo -e "\033[0;m"

# *** 3. Build the Legacy Menu Conversion Tool
cd ../linux
make -f xrm-menu-convert.mk $1
cd $cwd

echo ''
echo -e "\033[1;31m"Build Legacy Menu Prompt Conversion Tool
echo -e "\033[0;31m"========================================
echo -e "\033[0;m"

# *** 4. Build the Legacy Menu Prompt Conversion Tool
cd ../linux
make -f xrm-menu-prompt-convert.mk $1
cd $cwd

echo ''
echo -e "\033[1;31m"Build XRM-Server Unit Tests
echo -e "\033[0;31m"===========================
echo -e "\033[0;m"

# *** 5. Build the Unit Tests
cd ../linux
make -f xrm-unittest.mk $1
cd $cwd

echo ''
echo -e "\033[1;31m"Build XRM-Server [Main Program]
echo -e "\033[0;31m"===============================
echo -e "\033[0;m"

# *** 6. Build the Program
cd ../linux
make -f xrm-server.mk $1
cd $cwd

echo ''
echo -e "\033[1;31m"Coping Executables to Build Folder
echo -e "\033[0;31m"=================================
echo -e "\033[0;m"

# *** 7. Copy the Built Executable (Right now it's a Debug Executable to Build Folder)
rm -f xrm-server
rm -f xrm-menu-convert
rm -f xrm-menu-prompt-convert
rm -f xrm-unittest

cp ../linux/Debug/xrm-server .
cp ../linux/Debug/xrm-menu-convert .
cp ../linux/Debug/xrm-menu-prompt-convert .
cp ../linux/Debug/xrm-unittest .

echo ''
echo -e "\033[1;31m"Running Unit Tests
echo -e "\033[0;31m"==================
echo -e "\033[0;m"
./xrm-unittest

echo ''
echo -e "\033[1;31m"Build Completed - Check for errors!
echo -e "\033[1;31m"run ./xrm-server to start the program
echo -e "\033[0;31m"======================================
echo -e "\033[0m"

