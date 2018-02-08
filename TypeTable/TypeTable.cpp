////////////////////////////////////////////////////////////////////////////////
// TypeTable.cpp - Implementation of typetable to support dependency analysis //
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
* This package contains the test stub to test functions from TypeTable class.
*
* Required Files:
* ---------------
* TypeTable.h
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

#include "TypeTable.h"

#ifdef TEST_TYPETABLE // test stub

int main()
{

	TypeTable typeTab;
	typeTable objTypeTable = typeTab.getTypeTable();

}

#endif // TEST_TYPETABLE
