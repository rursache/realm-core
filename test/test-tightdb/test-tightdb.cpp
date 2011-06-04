#include <stdio.h>
#include <tightdb.h>
#include <UnitTest++.h>
#include <assert.h>
#include "../Support/mem.h"
#include "../Support/number_names.h"


enum Days {
	Mon,
	Tue,
	Wed,
	Thu,
	Fri,
	Sat,
	Sun
};

TDB_TABLE_4(TestTable,
			Int,        first,
			String,     second,
			Int,        third,
			Enum<Days>, fourth)

int main() {
	TestTable table;

	// Build large table
	for (size_t i = 0; i < 250000; ++i) {
		// create random string
		const size_t n = rand() % 1000;// * 10 + rand();
		const string s = number_name(n);

		table.Add(n, s.c_str(), 100, Wed);
	}
	table.Add(0, "abcde", 100, Wed);

	printf("Memory usage: %d bytes\n", GetMemUsage());

	UnitTest::Timer timer;

	// Search small integer column
	{
		timer.Start();

		// Do a search over entire column (value not found)
		for (size_t i = 0; i < 100; ++i) {
			const size_t res = table.fourth.Find(Tue);
			if (res != -1) {	
				printf("error");
			}
		}

		const int search_time = timer.GetTimeInMs();
		printf("Search (small integer): %dms\n", search_time);
	}

	// Search byte-size integer column
	{
		timer.Start();

		// Do a search over entire column (value not found)
		for (size_t i = 0; i < 100; ++i) {
			const size_t res = table.third.Find(50);
			if (res != -1) {	
				printf("error");
			}
		}

		const int search_time = timer.GetTimeInMs();
		printf("Search (byte-size integer): %dms\n", search_time);
	}

	// Search string column
	{
		timer.Start();

		// Do a search over entire column (value not found)
		for (size_t i = 0; i < 100; ++i) {
			const size_t res = table.second.Find("abcde");
			if (res != 250000) {	
				printf("error");
			}
		}

		const int search_time = timer.GetTimeInMs();
		printf("Search (string): %dms\n", search_time);
	}

	// Add index
	{
		timer.Start();

		table.SetIndex(0);

		const int search_time = timer.GetTimeInMs();
		printf("Add index: %dms\n", search_time);
	}

	printf("Memory usage2: %d bytes\n", GetMemUsage());

	// Search with index
	{
		timer.Start();

		for (size_t i = 0; i < 100000; ++i) {
			const size_t n = rand() % 1000;
			const size_t res = table.first.Find(n);
			if (res == 2500002) { // to avoid above find being optimized away
				printf("error");
			}
		}

		const int search_time = timer.GetTimeInMs();
		printf("Search index: %dms\n", search_time);
	}

	//getchar(); // wait for key
	//return 1;
}
