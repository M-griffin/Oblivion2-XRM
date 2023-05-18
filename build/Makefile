# Simple Development Makefile for quickly rebuilding and running unittests
# When making changes, theres no need to recompile everything, so some shortcuts.
# Simply replace "linux" with "osx" to switch platforms - Change -j3 to # of cpu cores for faster compiles

default:
	./build-linux.sh -j3
clean:
	make -f ../linux/SqliteWrapped.mk clean
sql:
	make -f ../linux/SqliteWrapped.mk -j3
	cat ../sqlitewrap/IError.h ../sqlitewrap/StderrLog.h ../sqlitewrap/SysLogs.h ../sqlitewrap/Database.h ../sqlitewrap/Query.h> ./Debug/libSqliteWrapped.h
xrm:
	make -f ../linux/xrm-server.mk -j3
	cp -f ./Debug/xrm-server ../build/xrm-server
test:
	make -f ../linux/xrm-unittest.mk -j3
	cp -f ./Debug/xrm-unittest ../build/xrm-unittest
run:
	./xrm-server
mem:
	valgrind -s --track-origins=yes --tool=memcheck --leak-check=full --show-leak-kinds=all ./xrm-server
mem2:
	valgrind -s --track-origins=yes --tool=memcheck --leak-check=full --show-leak-kinds=all ../linux/Debug/xrm-server

run-test:
	./xrm-unittest
