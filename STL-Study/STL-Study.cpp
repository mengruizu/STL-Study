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

using namespace tiny;

int main(int argc, char *argv[])
{
    // if detected memory leaks, before program quit will print info.
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    _CrtMemState s1, s2, s3;
    _CrtMemCheckpoint(&s1); 	// memory snapshot s1
    int* a = (int*)malloc_alloc::allocate(sizeof(int));
    a = (int*)malloc_alloc::reallocate(a, sizeof(int) * 2);
    a[0] = 10;
    a[1] = 11;
    malloc_alloc::deallocate(a);

    _CrtMemCheckpoint(&s2);		// memory snapshot s2
	
	int* b = new int[5];

	for (int i = 0; i < 5; i++)
	{
		b[i] = i;
	}

	std::cout << distance(b, b + 4) << std::endl;

	delete[] b;



    if(_CrtMemDifference(&s3, &s1, &s2)) // diff snapshot
    {
        _RPTF0( _CRT_WARN ,"_CrtMemDumpStatistics: \n");
        _CrtMemDumpStatistics(&s3);
    }
	printf("press any key to continue...");
	getchar();
    return 0;
}

