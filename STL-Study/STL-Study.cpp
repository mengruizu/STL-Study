// STL-Study.cpp: 定义控制台应用程序的入口点。
//
#include "stdafx.h"

// memory leak detection
#define _CRTDBG_MAP_ALLOC // memory leak info add file name and line number
#include <cstdlib>
#include <crtdbg.h>

#include <iostream>
#include "tiny_alloc.h"
#include "tiny_iterator.h"
#include "tiny_type_traits.h"
#include "tiny_numeric.h"
#include "tiny_vector.h"
#include <list>
#include <vector>
#include <numeric>
#include <iterator>

void tiny_alloc_unit_test();
void tiny_vector_unit_test();
void tiny_copy_unit_test();
using std::cout;
using std::endl;
using namespace tiny;

int main(int argc, char *argv[])
{
    // if detected memory leaks, before program quit will print info.
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    _CrtMemState s1, s2, s3;
    _CrtMemCheckpoint(&s1); 	// memory snapshot s1

#ifdef PASS_UNIT_TEST
	tiny_alloc_unit_test();
	tiny_vector_unit_test();
#endif // PASS_UNIT_TEST
	tiny_copy_unit_test();

    _CrtMemCheckpoint(&s2);		// memory snapshot s2

    if(_CrtMemDifference(&s3, &s1, &s2)) // diff snapshot
    {
        _RPTF0( _CRT_WARN ,"_CrtMemDumpStatistics: \n");
        _CrtMemDumpStatistics(&s3);
    }
	printf("press any key to continue...");
	getchar();
    return 0;
}

void tiny_alloc_unit_test()
{
    int* a = (int*)malloc_alloc::allocate(sizeof(int));
    a = (int*)malloc_alloc::reallocate(a, sizeof(int) * 2);
    a[0] = 10;
    a[1] = 11;
    malloc_alloc::deallocate(a);
}

void tiny_vector_unit_test()
{
	// vector() 
	vector<int> vec;
	cout << "vec: " << vec.size() << endl;
	for (int n : vec)
		cout << n << " ";
	cout << endl;
	
	// vector(size_t count, const T& value) 
	vector<int> vec1((size_t)20, 10);	/// = { 1, 2, 3 , 4, 5 };
	cout << "vec1: " << vec1.size() << endl;
	for (int n : vec1)
		cout << n << " ";
	cout << endl;

	// explicit vector(size_t count) 
	vector<int> vec2((size_t)20);	/// = { 1, 2, 3 , 4, 5 };
	cout << "vec2: " << vec.size() << endl;

	cout << "vec3: " << endl;
	// vector(InputIterator first, InputIterator last) 
	vector<int> vec3(10, 20);	/// = { 1, 2, 3 , 4, 5 };
	for (int n: vec3)
		cout << n << " ";
	cout << endl;

	cout << "vec4: " << endl;
	// vector(InputIterator first, InputIterator last) 
	int a[10] = { 1, 2, 3 };
	vector<int> vec4(a, a+3);
	for(int n: vec4)
		cout << n << " ";
	cout << endl;

	cout << "vec5: " << endl;
	// vector(const vector& other) 
	vector<int> vec5(vec4);
	for(int n: vec5)
		cout << n << " ";
	cout << endl;

	cout << "vec6: " << endl;
	vector<int> vec6 = { 1, 2, 3, 4, 5, 6 };
	for(int n: vec6)
		cout << n << " ";
	cout << endl;
}

class copy_class
{
public:
	copy_class() : _data(3)
	{}
	int _data;
};

class copy_class_def
{
public:
	copy_class_def() :_data(3)
	{}
	int _data;
};
namespace tiny {
	template<>
	struct type_traits<copy_class_def>
	{
		typedef true_type	has_trivial_default_constructor;
		typedef true_type	has_trivial_copy_constructor;
		typedef true_type	has_trivial_assignment_operator;
		typedef true_type	has_trivial_destructor;
		typedef false_type	is_POD_type; // POD: Plain Old Data
	};
}

void tiny_copy_unit_test()
{
	const char ccs[5] = { 'a', 'b', 'c', 'd', 'e' };
	char ccd[5];
	printf("1. ccs => ccd \n");
	tiny::copy(ccs, ccs + 5, ccd);
	for (char c : ccd)
		putchar(c);
	printf("\n");

	const wchar_t cwcs[5] = { 'a', 'b', 'c', 'd', 'e' };
	wchar_t cwcd[5];
	printf("\n 2. cwcs => cwcd \n");
	tiny::copy(cwcs, cwcs + 5, cwcd);
	for (wchar_t wc : cwcd)
		putwchar(wc);
	printf("\n");

	int ia[5] = { 0, 1, 2, 3, 4 };
	int _ia[5];
	printf("\n 3. int[] \n");
	copy(ia, ia + 5, _ia);
	for (int i : _ia)
		printf("%d ", i);
	putchar('\n');

#if 0
	std::list<int> ilists(ia, ia + 5);
	std::list<int> ilistd(5);
	printf("\n 4. list<int> \n");
	tiny::copy(ilists.begin(), ilists.end(), ilistd.begin());
	for (int i : ilistd)
		printf("%d ", i);
	putchar('\n');
#endif // 0

	vector<int> ivecs(ia, ia + 5);
	vector<int> ivecd(5);
	printf("\n 5. vector<int> \n");
	copy(ivecs.begin(), ivecs.end(), ivecd.begin());
	for(int i: ivecd)
		printf("%d ", i);
	putchar('\n');


	copy_class c[5];
	vector<copy_class> Cvs(c, c + 5);
	vector<copy_class> Cvd(5);
	printf("\n 6. vector<copy_class> \n");
	copy(Cvs.begin(), Cvs.end(), Cvd.begin());
	for (size_t i = 0; i < Cvd.size(); ++i)
		printf("%d ", Cvd[i]._data);
	putchar('\n');

	copy_class_def c1[5];
	vector<copy_class_def> Cvs1(c1, c1 + 5);
	vector<copy_class_def> Cvd1(5);
	printf("\n 6.1. vector<copy_class_def> \n");
	copy(Cvs1.begin(), Cvs1.end(), Cvd1.begin());
	for (size_t i = 0; i < Cvd1.size(); ++i)
		printf("%d ", Cvd1[i]._data);
	putchar('\n');
}
