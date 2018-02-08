////////////////////////////////////////////////////////////////////////////////
// Publisher.cpp - Implementation of PublishCode class						  //
//																		      //
//  Language:      Visual C++ 2015										      //
//  Platform:      HP Pavilion x-64, Windows 10							      //
//  Application:   Dependency-Based Code Publisher - CSE687 Project3	      //
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
* This package contains the test stub to test functions from PublishCode class.
*
* Required Files:
* ---------------
* Publisher.h
*
* Build Process:
* --------------
* devenv CodePublisher.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 5 April, 2017
* - first release
*
*/

#include "Publisher.h"

#ifdef TEST_PUBLISHCODE

int main()		// test stub
{
	std::unordered_map<std::string, std::set<std::string>> depTab;
	std::string cmdArg = "dummyArg";
	std::string trialEncode = "<std::cout>";
	PublishCode publisher;

	publisher.publishCode(depTab, cmdArg);
	publisher.encodeHTMLTags(trialEncode);
	return 0;
}

#endif // TEST_PUBLISHCODE