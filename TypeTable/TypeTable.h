#pragma once
////////////////////////////////////////////////////////////////////////////
// TypeTable.h - Construction of typetable to support dependency analysis //
//																		  //
//  Language:      Visual C++ 2015										  //
//  Platform:      HP Pavilion x-64, Windows 10							  //
//  Application:   Type-based Dependency Analysis - CSE687 Project2		  //
//  Author:        Yadav Murthy, Syracuse University					  //
//                 (315) 608-1734, ynarayan@syr.edu						  //
//																		  //
////////////////////////////////////////////////////////////////////////////
/*
*
* Manual Information :-
* ----------------------
* Package Operations:
* -------------------
* This package contains a TypeTable class.
* Its purpose is to create a TypeTable to store information 
* regarding Types that support Dependency Analysis
* It is implemented using C++11 STL unordered_map
*
* Required Files:
* ---------------
* None
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
#include <unordered_map>
using firstItem = std::string;
using secondItem = std::pair<std::string, std::string>;
using typeTable = std::unordered_map<firstItem, secondItem>;


////////////////////////////////////////////////////////////////////////////
// TypeTable class represents an unordered map to store type information  //
// - The type information stored in this table is used for dependency     //
//   analysis                                                             //
////////////////////////////////////////////////////////////////////////////
class TypeTable {
public:
	typeTable& getTypeTable();
private:
	typeTable tt_;
};

//----< TypeTable::getTypeTable() function >---------------------------------
/*
* Getter Function to get the type table.
*/
inline typeTable& TypeTable::getTypeTable()
{
	return tt_;
}