// STL-Study.cpp: 定义控制台应用程序的入口点。
//
#include "stdafx.h"

// memory leak detection
#define _CRTDBG_MAP_ALLOC // memory leak info add file name and line number
#include <cstdlib>
#include <crtdbg.h>

#include "tiny_unit_test.h"
#include "tiny_vector.h"
#include <vector>

int main(int argc, char *argv[])
{
    // if detected memory leaks, before program quit will print info.
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    _CrtMemState s1, s2, s3;
    _CrtMemCheckpoint(&s1); 	// memory snapshot s1

#ifdef PASS_UNIT_TEST
	tiny_copy_unit_test();
	tiny_copy_backward_unit_test();
	tiny_vector_constructor_unit_test();
	tiny_STL_print_unit_test();
#endif // PASS_UNIT_TEST
	tiny_vector_member_unit_test();

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

