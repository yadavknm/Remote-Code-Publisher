#pragma once
//////////////////////////////////////////////////////////////////////////////
// XmlPersist.h - Persisting dependency data in NoSqlDb into XML			//
//  Language:      Visual C++ 2015										    //
//  Platform:      HP Pavilion x-64, Windows 10							    //
//  Application:   Type-based Dependency Analysis - CSE687 Project2		    //
//  Author:        Yadav Murthy, Syracuse University					    //
//                 (315) 608-1734, ynarayan@syr.edu						    //
//																		    //
//////////////////////////////////////////////////////////////////////////////
/*
*
* Manual Information :-
* ----------------------
* Package Operations:
* -------------------
* This package contains a class to persist data into XML
* Its purpose is to persist the dependency information stored in NoSQLDb
*
* Required Files:
* ---------------
* NoSqlDb.h, XmlElement.h
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

//#include"../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"
#include <iostream>
#include "../NoSqlDb/NoSqlDb.h"
using namespace XmlProcessing;

//////////////////////////////////////////////////////////////////////////////
// XmlPersist class															//
// - supports XML persistance												//
//////////////////////////////////////////////////////////////////////////////
class XmlPersist {


public:
	std::string toXmlFromDb(NoSqlDb<std::string> db);

};


//----< XmlPersist::toXmlFromDb() function >---------------------------------
/*
* Extract information from NoSQLDb to persist into XML
*/
inline std::string XmlPersist::toXmlFromDb(NoSqlDb<std::string> db) {

	using sPtr = std::shared_ptr < AbstractXmlElement >;

	sPtr root = makeTaggedElement("DEPENDENCY_TABLE");	// root element

	for (unsigned int i = 0; i < db.keys().size(); ++i)
	{

		sPtr pKeyElem = makeTaggedElement("FILE");		// file name element
		root->addChild(pKeyElem);
		sPtr pTextKeyElem = makeTextElement(db.keys().at(i));
		pKeyElem->addChild(pTextKeyElem);
		sPtr pChildren = makeTaggedElement("CHILD_DEPENDENCIES");	// dependencies element
		pKeyElem->addChild(pChildren);
		for (unsigned int m = 0; m < db.value(db.keys().at(i)).children_.size(); ++m)	// dislaying all the dependent files
		{
			sPtr pElemChildrenVect = makeTaggedElement("CHILD");
			pChildren->addChild(pElemChildrenVect);
			sPtr pTextElemChildrenVect = makeTextElement(db.value(db.keys().at(i)).children_.at(m));
			pElemChildrenVect->addChild(pTextElemChildrenVect);
		}
	}
	sPtr docEl = makeDocElement(root);
	std::cout << "  " << docEl->toString();	// xml to string conversion
	std::cout << "\n\n";
	return docEl->toString();
}
