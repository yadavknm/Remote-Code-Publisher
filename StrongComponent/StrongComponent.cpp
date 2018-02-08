////////////////////////////////////////////////////////////////////////////////
// StrongComponent.cpp - Testing functions in StrongComponent.h				  //
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
* This package contains the test stub to test functions from StrongComponent class.
*
* Required Files:
* ---------------
* StrongComponent.h
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

#include "StrongComponent.h"
using namespace strong;

//----< main() function >---------------------------------
/*
* Test stub to test the functionality of StrongComponent class.
*/
#ifdef TEST_STRONGCOMPONENT

int main() {
	Graph grph;
	grph.setgraphsize(5);
	grph.addEdge(1, 0);
	grph.addEdge(0, 2);
	grph.addEdge(2, 1);
	grph.addEdge(0, 3);
	grph.addEdge(3, 4);
	grph.SCC();
}

#endif // TEST_STRONGCOMPONENT