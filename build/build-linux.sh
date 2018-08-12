# Oblivion/2 XRM [LINUX / ARM]
# Commandline Build Script 
# For users that want to checkout and build the system 
# Without installing Codelite IDE.
# Michael Griffin 8/12/2018

# Save Current Working Directory
cwd=$(pwd)

# *** 1. Build SQL Wrapped Dependency
cd ../sqlitewrap
make -f Makefile clean
make -f Makefile
cd $cwd

# *** 2. Build XRM-Server (Main BBS System)
cd ../linux
# Replace Home Folders used in Develop with Current Working Paths.
sed -i 's+/home/blue/code/Oblivion2-XRM/src/+../src/+' xrm-server.mk
sed -i 's+/home/merc/code/Oblivion2-XRM/src/+../src/+' xrm-server.mk
# Build the Program
make -f xrm-server.mk clean
make -f xrm-server.mk
cd $cwd

# Copy the Built Executable (Right now it's a Debug Executable to Build Folder)
rm -f xrm-server
cp ../linux/Debug/xrm-server .

echo .
echo 'Build Completed.'
