////////////////////////////////////////////////////////////////////////////////
// XmlPersist.cpp - Testing functions in XmlPersist.h						  //
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
* This package contains the test stub to test functions from XmlPersist class.
*
* Required Files:
* ---------------
* XmlPersist.h
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
#include "XmlPersist.h"

#ifdef TEST_XMLPERSIST

//----< main() function >---------------------------------
/*
* Test stub to test the functionality of XmlPersist class.
*/
int main() {	
		XmlPersist x;
		NoSqlDb<std::string> db;
		Element<std::string> elem1;
		elem1.name = "elem1";
		elem1.category = "test";
		elem1.description = "elem1's description";
		elem1.data = "elem1's StrData";
		elem1.timeDate = elem1.getTimeDate();
		db.save(elem1.name, elem1);

		x.toXmlFromDb(db);
}

#endif // TEST_XMLPERSIST
