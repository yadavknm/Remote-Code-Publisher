////////////////////////////////////////////////////////////////////////////////
// DepAnal.cpp - Testing functions in DepAnal.h								  //
//																		      //
//  Language:      Visual C++ 2015										      //
//  Platform:      HP Pavilion x-64, Windows 10							      //
//  Application:   Type-based Dependency Analysis - CSE687 Project2		      //
//  Author:        Yadav Murthy, Syracuse University					      //
//                 (315) 608-1734, ynarayan@syr.edu						      //
//																		      //
////////////////////////////////////////////////////////////////////////////////
/*
*
* Manual Information :-
* ----------------------
* Package Operations:
* -------------------
* This package contains the test stub to test functions from DepAnal class.
*
* Required Files:
* ---------------
* DepAnal.h
*
* Build Process:
* --------------
* devenv CodeAnalyzerEx.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 19 March, 2017
* - first release
*
*/

#include "DepAnal.h"


#ifdef TEST_DEPANAL		// test stub



int main()
{
	TypeAnal taObj;
	taObj.doTypeAnal();
	taObj.doDepAnal;
	taObj.GraphStrongComponent();
}

#endif // TEST_DEPANAL
