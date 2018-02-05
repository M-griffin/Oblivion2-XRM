VERSION =	1.3.1
DIFF_VERSION =	1.3

INSTALL_PREFIX = /usr/devel
INSTALL_LIB =	$(INSTALL_PREFIX)/lib
INSTALL_INCLUDE = $(INSTALL_PREFIX)/include
INSTALL =	/usr/bin/install

INCLUDE =	-I/usr/devel/include -I.
CFLAGS =	-Wall -g -O2 -std=c++11 $(INCLUDE) -MD
# namespace
#CFLAGS +=	-DSQLITEW_NAMESPACE=sqlitew
CPPFLAGS =	$(CFLAGS)

LIBS =		-L/usr/lib -L/usr/local/lib -lSqliteWrapped -lsqlite3
LIBNAME =	libSqliteWrapped


PROGS =

all:		$(PROGS) $(LIBNAME).a $(LIBNAME).h

LIBM =		Database.o Query.o StderrLog.o SysLog.o
$(LIBNAME).a: 	$(LIBM)
		ar cr $(LIBNAME).a $(LIBM)
		ranlib $(LIBNAME).a

$(LIBNAME).h:	IError.h StderrLog.h SysLog.h Database.h Query.h
		cat IError.h StderrLog.h SysLog.h Database.h Query.h > $(LIBNAME).h

install:	all
		@mkdir -p $(INSTALL_LIB)
		@mkdir -p $(INSTALL_INCLUDE)
		$(INSTALL) $(LIBNAME).a $(INSTALL_LIB)
		$(INSTALL) $(LIBNAME).h $(INSTALL_INCLUDE)

clean:
		rm -f *~ *.o $(PROGS) *.d slask *.a $(LIBNAME).h

sqlite3test:	sqlite3test.o
		g++ -o $@ $^ $(LIBS)

-include	*.d

TARFILE =	sqlitewrapped-$(VERSION)
tar:		clean
		tar czf $(TARFILE).tar.gz \
			Query.* \
			Database.* \
			IError.h \
			StderrLog.* \
			SysLog.* \
			sqlite3test.cpp \
			Makefile gpl.txt \
			Project/*.dsp Project/*.dsw
		/usr/local/bin/tarfix.sh $(TARFILE)
		cp $(TARFILE).tar.gz /usr/local/apache/www.alhem.net/htdocs/project/sqlite
		cp $(TARFILE).zip /usr/local/apache/www.alhem.net/htdocs/project/sqlite

docs:
		./mkdot.sh

diff:
		diff -b -B -C 3 /usr/src/sqlitewrapped-$(DIFF_VERSION) . | \
			/usr/local/bin/d2html -title "sqlite3 wrapped" \
			-url "http://www.alhem.net/project/sqlite/" \
			> \
			/usr/local/apache/www.alhem.net/htdocs/project/sqlite/latest_diff.html

