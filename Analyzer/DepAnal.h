#pragma once
 //////////////////////////////////////////////////////////////////////////////
 //	 DepAnal.h - Developing dependency analyzer			  					 //
 //  Language:      Visual C++ 2015										     //
 //  Platform:      HP Pavilion x-64, Windows 10							 //
 //  Application:   Type-based Dependency Analysis - CSE687 Project2		 //
 //  Author:        Yadav Murthy, Syracuse University					     //
 //                 (315) 608-1734, ynarayan@syr.edu						 //
 //																		     //
 //////////////////////////////////////////////////////////////////////////////
 /*
 *
 * Manual Information :-
 * ----------------------
 * Package Operations:
 * -------------------
 * This package contains a class TypeAnal which supports analyzing the dependencies
 *
 * Required Files:
 * ---------------
 * ActionsAndRules.h, TypeTable.h, DependencyAnalysis.h, StrongComponent.h
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

#include "../Parser/ActionsAndRules.h"
#include <iostream>
#include <functional>
#include "../TypeTable/TypeTable.h"
#include <set>
#include "../DependencyAnalysis/DependencyAnalysis.h"
#include "../StrongComponent/StrongComponent.h"


#pragma warning (disable : 4101)  // disable warning re unused variable x, below
using namespace strong;
namespace CodeAnalysis
{

//////////////////////////////////////////////////////////////////////////////
// TypeAnal class															//
// - supports dependency analysis using:									//
//		doTypeAnal()														//
//		doDepAnal()															//
//////////////////////////////////////////////////////////////////////////////
  class TypeAnal
  {
  public:
    using SPtr = std::shared_ptr<ASTNode*>;
	using firstItem = std::string;
	using secondItem = std::pair<std::string, std::string>;
	using typeTable = std::unordered_map<firstItem, secondItem>;

    TypeAnal();						// constructing TypeAnal object
    void doTypeAnal();				// function to perform Type Analysis
	void showTypeTable();			// function to display Type Table
	void doDepAnal(std::string cmdArg);				// function to perform Dependency Analysis
	void GraphStrongComponent();	// function to find strong components
	void XmlFileName(std::string x);// function to persist XML

  private:
    void DFS(ASTNode* pNode);
    AbstrSynTree& ASTref_;
    ScopeStack<ASTNode*> scopeStack_;
    Scanner::Toker& toker_;
	TypeTable myTt_;
	std::set<std::string> my_set;
	NoSqlDb<std::string>* StoreDB;
	DependencyAnalysis depTable;
  };

  //----< TypeAnal::TypeAnal() function >---------------------------------
  /*
  * Constructs TypeAnal object using scopeStack, Toker and ASTRef_
  */
  inline TypeAnal::TypeAnal() : 
    ASTref_(Repository::getInstance()->AST()),
    scopeStack_(Repository::getInstance()->scopeStack()),
    toker_(*(Repository::getInstance()->Toker()))
  {
    std::function<void()> test = [] { int x; };  // test detection of lambdas.
  }                                              

  inline bool doDisplay(ASTNode* pNode)
  {
    static std::string toDisplay[] = {
      "function", "lambda", "class", "struct", "enum", "alias", "typedef"
    };
    for (std::string type : toDisplay)
    {
      if (pNode->type_ == type)
        return true;
    }
    return false;
  }

  //----< TypeAnal::DFS() function >---------------------------------
  /*
  * Traversing the node to analyze Types
  */
  inline void TypeAnal::DFS(ASTNode* pNode)
  {
    static std::string path = "";
    if (pNode->path_ != path)
    {
      /*std::cout << "\n    -- " << pNode->path_ << "\\" << pNode->package_;*/
      path = pNode->path_;
    }
    if (doDisplay(pNode))
    {
      /*std::cout << "\n  " << pNode->name_;
      std::cout << ", " << pNode->type_;*/
    }
	if ((pNode->type_ == "class") || (pNode->type_ == "struct") || (pNode->type_ == "enum") || (pNode->type_ == "namespace")) {
		secondItem secondItem_;
		firstItem firstItem_ = pNode->name_;
		std::pair<std::string, std::string> temp;
		secondItem_.first = pNode->type_;
		secondItem_.second = pNode->path_;
		myTt_.getTypeTable().insert(std::make_pair(firstItem_, secondItem_));
	}
	my_set.insert(pNode->path_);
    for (auto pChild : pNode->children_)
      DFS(pChild);
  }

  //----< TypeAnal::doTypeAnal() function >---------------------------------
  /*
  * creates and displays type table
  */
  inline void TypeAnal::doTypeAnal()
  {
    ASTNode* pRoot = ASTref_.root();
    DFS(pRoot);
	std::cout << std::endl;
	std::cout << std::endl;
  }

  //----< TypeAnal::showTypeTable() function >---------------------------------
  /*
  *  displays type table
  */
  inline void TypeAnal::showTypeTable()
  {
	  title("TYPE TABLE");
	  std::cout << std::endl;
	  std::cout << std::setw(30) << "-- NAME --" << std::setw(30) << "-- TYPE --" << std::setw(30) << "-- FILE --\n";
	  for (auto it = myTt_.getTypeTable().begin(); it != myTt_.getTypeTable().end(); ++it) {
		  std::cout << std::setw(30) << it->first;
		  std::cout << std::setw(30) << it->second.first << std::setw(10) << " ";
		  std::cout << std::setw(50) << it->second.second << std::setw(30) << std::endl;
	  }
  }

  //----< TypeAnal::doDepAnal() function >---------------------------------
  /*
  * creates and displays dependency table
  */
  inline void TypeAnal::doDepAnal(std::string cmdArg) {
	  for (auto iter = my_set.begin(); iter != my_set.end(); ++iter) {
		  StoreDB = depTable.makeDepAnalTable(myTt_, *iter, cmdArg);
	  }
	  
	  /*title("Using NoSqlDb created in Project1 to save the dependency information");
	  for (auto iter = my_set.begin(); iter != my_set.end(); ++iter) {
		  depTable.saveToDb(*iter);
	  }*/
  }

  //----< TypeAnal::GraphStrongComponent() function >---------------------------------
  /*
  * Finding the strong components
  */
  inline void TypeAnal::GraphStrongComponent()
  {
	  Graph graphObj;
	  std::unordered_map<int, std::string> setVal;
	  std::unordered_map<std::string, int> graph_map;
	  graphObj.setgraphsize((int)StoreDB->keys().size());		// setting graph size based on the size of DataBase
	  int k = 0;
	  for (std::string key : StoreDB->keys())
	  {
		  graph_map[key] = k;
		  setVal[k] = key;
		  k++;
	  }
	  for (std::string key : StoreDB->keys())
	  {
		  Element<std::string> dependencies = StoreDB->value(key);
		  std::vector<std::string> myVec = dependencies.children_;		// dependent files stored as children
		  for (std::string child : myVec)
		  {
			  if (!(key == child))
			  {
				  graphObj.addEdge(graph_map[key], graph_map[child]);
			  }
		  }
	  }
	  graphObj.setFileGraphPath(setVal);
	  std::cout << std::endl;
	  graphObj.SCC();
  }

  //----< TypeAnal::XmlFileName() function >---------------------------------
  /*
  * Persist dependency information to XML File
  */
  inline void TypeAnal::XmlFileName(std::string x)
  {
	  if ((x == "") || 
		  (x == "*.cpp") || 
		  (x == ".*h") || 
		  (x == "/m") || 
		  (x == "/s") || 
		  (x == "/a") || 
		  (x == "/r") || 
		  (x == "/d") || 
		  (x == "/b") || 
		  (x == "/f")) {
		  x = "XML_DependencyAnalysis.txt";
	  }
	  else
	  {
		  x = x;
	  }
	  depTable.toXmlFile(x);
  }
}