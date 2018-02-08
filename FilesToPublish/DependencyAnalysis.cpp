////////////////////////////////////////////////////////////////////////////
// DependencyAnalysis.cpp - Implementation of DependencyAnalysis.h          
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
* This package contains the definiotions of functions in DependencyAnalysis class.
* Its purpose is to create a dependency table and store the dependencies
* in the NoSQL Database created in Project 1. It also supports XML persisting
* of the stored dependencies.
*
* Required Files:
* ---------------
* DpendencyAnalysis.h
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

#include "DependencyAnalysis.h"

//----< DependencyAnalysis::DependencyAnalysis() function >---------------------------------
/*
* Constructing DependencyAnalysis object
*/
DependencyAnalysis::DependencyAnalysis() : 
	ASTref_(Repository::getInstance()->AST())
{

}

//----< DependencyAnalysis::getDepTable() function >---------------------------------
/*
* Getter function for Dependency Table
*/
depTable& DependencyAnalysis::getDepTable() {
	return depTable_;
}

//----< DependencyAnalysis::setDepTable() function >---------------------------------
/*
* Setter function for Dependency Table
*/
void DependencyAnalysis::setDepTable(depTable& depT) {
	depTable_ = depT;
}

//----< DependencyAnalysis::saveToDb() function >---------------------------------
/*
* Showing the dependencies stored in the NoSQL Database created in Project1.
*/
int DependencyAnalysis::saveToDb(std::string key) {
	if (key != "") {
		std::cout << "\n" << DB.value(key).show();
	}
	return 0;
}

//----< DependencyAnalysis::toXmlFile() function >---------------------------------
/*
* Persisting data to XML File from the database
*/
int DependencyAnalysis::toXmlFile(std::string xmlFileName) {
	std::ofstream myfile;

	myfile.open(xmlFileName);
	XmlPersist persist;
	myfile << persist.toXmlFromDb(DB);
	myfile.close();
	return 0;
}

//----< DependencyAnalysis::makeDepAnalTable() function >---------------------------------
/*
* Analyses the dependencies based on the Type information in the TypeTable and creates a dependency table
*/
NoSqlDb<std::string>* DependencyAnalysis::makeDepAnalTable(TypeTable& tt, std::string filespec, std::string cmdArg) {
	if (filespec != "") {
		static std::string path = "";
		try {
			std::ifstream in(filespec);
			if (!in.good())	{
				std::cout << "cant open";
				return &DB;
			}
			else{
				Toker toker;
				toker.returnComments();	toker.attach(&in);
				typeTable my = tt.getTypeTable(); std::set<std::string> temp_set;
				do	{
					std::string tok = toker.getTok();
					for (auto tt : my) {
						if (tok == tt.first) {
							temp_set.insert(tt.second.second);
						}}
				} while (in.good());
				elementObj.name = filespec;						// add the element data
				elementObj.timeDate = elementObj.getTimeDate();
				elementObj.category = "test file";
				elementObj.data = "dependency info";
				while (!elementObj.children_.empty())
					elementObj.children_.pop_back();
				for (auto iter = temp_set.begin(); iter != temp_set.end(); ++iter)
					elementObj.children_.push_back(*iter);
				DB.save(elementObj.name, elementObj);			// saving the element into DataBase
				depTable_.insert(std::make_pair(filespec, temp_set));
			}
			PublishCode pub;		
			pub.publishCode(depTable_, cmdArg);					// Publishing the dependencies
			std::cout << std::endl;
			showDepAnalTable(filespec);
			}
		catch (std::logic_error& ex)
		{
			std::cout << "\n  " << ex.what();
		}
		std::cout << "\n\n";	}
	return &DB;
}

//----< DependencyAnalysis::showDepAnalTable() function >---------------------------------
/*
* Displays the dependency table
*/
void DependencyAnalysis::showDepAnalTable(std::string filespec)
{
	std::cout << std::setw(60) << "-- FILE NAME --" << std::setw(60) << "-- DEPENDENCY --" << std::endl;
	std::cout << std::setw(60) << filespec;
	for (auto set_it = depTable_[filespec].begin(); set_it != depTable_[filespec].end(); ++set_it) {
		std::cout << std::endl;
		std::cout << std::setw(80) << " " << *set_it << "\n";
	}
}
