#
# Oblivion/2 XRM [OSX] (Tested on High Sierra)
#
# Commandline Build Script 
# For users that want to checkout and build the system 
# Without installing Codelite IDE.
# Michael Griffin 08/12/2018 - Initial Creation
# Michael Griffin 07/26/2019 - Update for new Project Layout and OS X Corrections.

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

# *** 1. Build SQL Wrapped Dependency
cd ../osx
make -f Makefile clean

echo ''
echo -e "\033[1;31m"Build SqliteWrapped
echo -e "\033[0;31m"===================
echo -e "\033[0;m"
echo ''

make -f SqliteWrapped.mk $1

# Setup Header File for Lib, the project has this, so we have to do it manually here.
cat ../sqlitewrap/IError.h ../sqlitewrap/StderrLog.h ../sqlitewrap/SysLogs.h ../sqlitewrap/Database.h ../sqlitewrap/Query.h > ./Debug/libSqliteWrapped.h

cd $cwd

echo ''
echo -e "\033[1;31m"Build Legacy Menu Conversion Tool
echo -e "\033[0;31m"=================================
echo -e "\033[0;m"

# *** 3. Build the Legacy Menu Conversion Tool
cd ../osx
make -f xrm-menu-convert.mk $1
cd $cwd

echo ''
echo -e "\033[1;31m"Build Legacy Menu Prompt Conversion Tool
echo -e "\033[0;31m"========================================
echo -e "\033[0;m"

# *** 4. Build the Legacy Menu Prompt Conversion Tool
cd ../osx
make -f xrm-menu-prompt-convert.mk $1
cd $cwd

echo ''
echo -e "\033[1;31m"Build XRM-Server Unit Tests
echo -e "\033[0;31m"===========================
echo -e "\033[0;m"

# *** 5. Build the Unit Tests
cd ../osx
make -f xrm-unittest.mk $1
cd $cwd


echo ''
echo -e "\033[1;31m"Build XRM-Server [Main Program]
echo -e "\033[0;31m"===============================
echo -e "\033[0;m"

# *** 6. Build the Program
cd ../osx
make -f xrm-server.mk $1
cd $cwd


# *** 7. Copy the Built Executable (Right now it's a Debug Executable to Build Folder)
rm -f xrm-server
rm -f xrm-menu-convert
rm -f xrm-menu-prompt-convert
rm -f xrm-unittest

cp ../osx/Debug/xrm-server .
cp ../osx/Debug/xrm-menu-convert .
cp ../osx/Debug/xrm-menu-prompt-convert .
cp ../osx/Debug/xrm-unittest .


# Setup Dynamic Library for SqliteWraped, Look for this in the Debug folder.
# Once the Release folder is used this will be updated for there.
# Specific to OSX since it doesn't like static libraries.

if [ -d "./Debug" ] 
then
    cd Debug
    echo "Directory ./Debug Exists."
    cp ../../osx/Debug/libSqliteWrapped.a .
else
    cd Debug
    echo "Creating ./Debug Folder for Dynamic Libraries."
    mkdir ./Debug 
    cp ../../osx/Debug/libSqliteWrapped.a .    
fi

cd $cwd

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

