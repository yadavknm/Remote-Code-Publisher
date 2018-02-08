//////////////////////////////////////////////////////////////////
// NoSqlDb.cpp - Implementation of Key/Value in memory database //
// ver 1.0                                                      //
// Yadav Murthy, CSE687 - Object Oriented Design, Spring 2017   //
//////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package contains implementations of functions from Element<T> class and  NoSqlDb<T> class.
* Its purpose is to support add,delete and update elements into NoSqlDb.
* It is implemented using C++11 STL unordered_map
*
* Required Files:
* ---------------
* NoSqldb.h, Query.h
*
* Build Process:
* --------------
* devenv Project1.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 13 Feb 2017
* - first release
*
*/


#ifdef TEST_NoSqlDb



#include "NoSqlDb.h"	// including the necessary header files
#include "Query.h"
#include <iostream>
#include <set>

using StrData = std::string;
using intData = int;
using Key = NoSqlDb<StrData>::Key;
using Keys = NoSqlDb<StrData>::Keys;

int main()
{
  NoSqlDb<StrData> db;
  Element<StrData> elem1;
  elem1.name = "elem1";
  elem1.category = "test";
  elem1.description = "elem1's description";
  elem1.data = "elem1's StrData";
  elem1.timeDate = elem1.getTimeDate();
  db.save(elem1.name, elem1);   
  db.resave(elem2.name, elem2);  db.resave(elem1.name, elem1);  db.resave(elem3.name, elem3);
  db.deleteElem(elem3.name, elem3);							// deleting an element of a specified key
  elem2.deleteChild();										// deleyting the children from the Element
  db.resave(elem2.name,elem2);
  db.editText(elem1.name, elem1, "NEW TEXT DESCRIPTION");	//editing the text of an Element
  elem1 = db.value("elem1");
  db.replaceInstance(elem1.name.getValue(), elem1, elem2);	//replacing the data of an Element
  elem1 = db.value("elem1");
  elem2 = db.value("elem2");
  std::string xml = db.toXml();		//converting the database contents into XML format
  std::cout<<"\n"<<db.toXml();
  XmlParser parser("../xml1.xml"); //parsing the contents of an XML file into the database
  XmlDocument* pXmlDoc = parser.buildDocument();
  std::cout << pXmlDoc->toString();
  db.fromXml(pXmlDoc->toString());	// augmenting the data from XML file
  Query<StrData> query;
  Element<StrData> result1 = query.Query1(db, "elem1");	// queries the value of a specified key "elem1"
  std::cout << result1.show();							// displaying the result of the query
  Keys result2 = query.Query2(db, "elem1"); // querying the children of "elem1"
  query.showResult(result2);
  Keys result3 = query.Query3(db, "elem");	// returns the keys with a pattern "elem"
  query.showResult(result3);
  Keys result4 = query.Query4(db, "elem");	// returns the keys that contain the pattern "elem" in the item metadata of element
  query.showResult(result4);
  Keys result5 = query.Query5(db, "te");	// returns the keys that contain the pattern "te" in the category metadata of element
  query.showResult(result5);
  Keys result6 = query.Query6(db, "Data");	// returns the keys that contain the pattern "Data" in the data of element
  query.showResult(result6);
  Keys result8 = query.queryOnAQuery(db, "em2", result4); // supporting query on the result of query4
  query.showResult(result8);
  std::set<Key> set1(result2.begin(), result2.end()); // result of query2
  std::set<Key> set2(result3.begin(), result3.end()); // result of query3
  Keys unionOfQueryResults;
  std::set_union(set1.begin(), set1.end(), set2.begin(), set2.end(), std::back_inserter(unionOfQueryResults)); // union of result2 and result3
  Keys result9 = query.queryUnion(db, "lem", unionOfQueryResults); // performing a query on the union of results
  query.showResult(result9);
}
#endif //TEST_NoSqlDb