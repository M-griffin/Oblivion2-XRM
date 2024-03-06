/*
 **	sqlite3test.cpp
 **
 **	Published / author: 2006-03-23 / grymse@alhem.net
 **/

/*
Copyright (C) 2006  Anders Hedstrom

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#include <stdio.h>
#include <sqlite3.h>
#include <libsqlitewrapped.h>


int main(int argc,char *argv[])
{
	Database::Mutex mutex; // not really necessary here at all
	StderrLog log;
	Database db(mutex, "test3.db", &log);
	Query q(db);

	// create a test3 table
	q.execute("create table test3 ( num integer, name string )");

	// fill test3 with some data
	q.execute("insert into test3 values(1, 'Anders')");
	q.execute("insert into test3 values(2, 'Grymse')");

	// retrieve data
	q.get_result("select * from test3");
	while (q.fetch_row())
	{
		long num = q.getval();
		std::string name = q.getstr();
		printf("#%ld: %s\n", num, name.c_str());
	}
	q.free_result();

	//
	return 0;
}
