#pragma once
////////////////////////////////////////////////////////////////////////////
// DependencyAnalysis.h - Construction of dependency table and storing into 
//						  databse.										    
//  Language:      Visual C++ 2015										    
//  Platform:      HP Pavilion x-64, Windows 10							    
//  Application:   Type-based Dependency Analysis - CSE687 Project2		    
//  Author:        Yadav Murthy, Syracuse University					    
//                 (315) 608-1734, ynarayan@syr.edu						    
//																		    
////////////////////////////////////////////////////////////////////////////
/*
*
* Manual Information :-
* ----------------------
* Package Operations:
* -------------------
* This package contains a DependencyAnalysis class.
* Its purpose is to create a dependency table and store the dependencies
* in the NoSQL Database created in Project 1. It also supports XML persisting
* of the stored dependencies.
*
* Required Files:
* ---------------
* TypeTable.h, NoSqlDb.h, ActionsAndRules.h, Tokenizer.h, XMLPersist.h
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
#include <set>
#include "../Parser/ActionsAndRules.h"
#include "../TypeTable/TypeTable.h"
#include "../NoSqlDb/NoSqlDb.h"
#include "../Tokenizer/Tokenizer.h"
#include "../XmlPersist/XmlPersist.h"
#include "../Publisher/Publisher.h"

using namespace Scanner;
using namespace CodeAnalysis;
using StrData = std::string;
using firstItem = std::string;
using secondItem = std::pair<std::string, std::string>;
using typeTable = std::unordered_map<firstItem, secondItem>;
using depTable = std::unordered_map<std::string, std::set<std::string>>;

////////////////////////////////////////////////////////////////////////////
// DependencyAnalysis class													
// - supports creation of a dependency analysis table						
// - storing it into the database											
// - persist the contents into XML format									
////////////////////////////////////////////////////////////////////////////
class DependencyAnalysis {
public:
	DependencyAnalysis();						// construction
	depTable& getDepTable();					// getters and setters for dependency table
	void setDepTable(depTable& depT);
	int saveToDb(std::string  filespec);		// function to save and show in database
	int toXmlFile(std::string xml);				// function to write results in an XML file
	NoSqlDb<std::string>* makeDepAnalTable(TypeTable& x, std::string filespec, std::string cmdArg); // constructing Dependency analysis table
	void showDepAnalTable(std::string filespec);// displaying Dependency analysis table
private:
	depTable depTable_;							// dependencyTable unorderedMap object
	NoSqlDb<std::string> DB;					// NoSqlDb object
	Element<StrData> elementObj;				// Element object of NoSqlDb
	AbstrSynTree& ASTref_;						// Reference of Abstract Syntax tree
	
};
