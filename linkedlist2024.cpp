// linkedlist2024.cpp : Defines the entry point for the application.
//

#include "linkedlist2024.h"
#include <iostream>
#include <string>
#include <assert.h>
#include <initializer_list>

using namespace std;
using namespace LinkedList2024;

// wasn't sure if 3rd party libraries are allowed, so I didn't want to use a testing library

template<class DataType>
void test_list(const LinkedList<DataType>& list, const initializer_list<DataType>& expected) {
	int i = 0;
	auto e = expected.begin();

	cout << endl << "testing list..." << endl;

	for (const auto& n : list) {
		assert(e != expected.end());
		cout << "#" << i << ": " << n << " vs " << *e << endl;
		assert(n == *e);
		++e;
		i++;
	}

	assert(e == expected.end());
}

// this class tests for memory leaks by keeping track of the number of constructions and destructions
class TestMemoryLeaks
{
public:
	static int contructs, destructs;

	TestMemoryLeaks() { contructs++; }
	TestMemoryLeaks(const TestMemoryLeaks& other) { contructs++; }
	~TestMemoryLeaks() { destructs++; }
};
int TestMemoryLeaks::contructs = 0;
int TestMemoryLeaks::destructs = 0;

int main()
{
	int i;

	// test ints
	LinkedList<int> list({ 0,1,2,3,4 });
	test_list(list, { 0,1,2,3,4 });

	// test crawling backwards
	i = 4;
	for (auto n = list.rend(); n != list.rbegin(); ++n) {
		assert(*n == i--);
	}

	// remove odd numbers
	for (auto n = list.begin(); n != list.end();) {
		if (*n % 2 == 1) {
			n = list.erase(n);
		}
		else {
			++n;
		}
	}

	test_list(list, { 0,2,4 });

	// test clear and push new ints
	list.clear();

	list.push_back(0);
	list.push_back(4);
	list.push_back(5);
	list.push_back(1);

	test_list(list, { 0,4,5,1 });

	// test find_first
	assert(*list.find_first(5) == 5);
	assert(*--list.find_first(5) == 4);
	assert(*++list.find_first(5) == 1);

	// test pop_front and pop_back
	list.pop_front();
	test_list(list, { 4,5,1 });
	list.pop_back();
	test_list(list, { 4,5 });

	// test implicit clear and re-init
	list = { 3,2,1 };
	test_list(list, { 3,2,1 });

	// test list of strings
	LinkedList<string> slist;

	slist.push_front("a");
	slist.push_front("b");
	slist.push_front("c");
	slist.push_front("d");
	test_list<string>(slist, { "d", "c", "b", "a"});

	// test erasing individual elements, and find_first
	slist.erase(slist.find_first("c"));
	test_list<string>(slist, { "d", "b", "a" });

	slist.erase(slist.find_first("b"));
	test_list<string>(slist, { "d", "a" });

	slist.erase(slist.find_first("d"));
	test_list<string>(slist, { "a" });

	slist.erase(slist.find_first("a"));
	test_list<string>(slist, { });

	// test for memory leaks
	LinkedList<TestMemoryLeaks> test_mem_leaks_list;
	test_mem_leaks_list.push_back(TestMemoryLeaks());
	test_mem_leaks_list.push_back(TestMemoryLeaks());
	test_mem_leaks_list.push_back(TestMemoryLeaks());
	test_mem_leaks_list.clear();
	assert(TestMemoryLeaks::contructs == 6);
	assert(TestMemoryLeaks::destructs == 6);

	cout << endl << "success!" << endl;
	return 0;
}
