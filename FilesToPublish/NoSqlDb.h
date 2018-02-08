////////////////////////////////////////////////////////////////
 // NoSqlDb.h - Key/Value in memory database                   
 // ver 1.0                                                    
 // Yadav Murthy, CSE687 - Object Oriented Design, Spring 2017 
 //////////////////////////////////////////////////////////////
 /*
 * Package Operations:
 * -------------------
 * This package contains an Element<T> class and a NoSqlDb<T> class.
 * Its purpose is to support add,delete and update elements into NoSqlDb.
 * It is implemented using C++11 STL unordered_map
 *
 * Required Files:
 * ---------------
 * XmlDocument.h, XmlElement.h, StrHelper.h, Convert.h
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
#pragma once
#pragma warning(disable : 4996) 

#include <unordered_map>					// including the necessary inbuilt header files
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>
//#include "../CppProperties/CppProperties.h"	// including the necessary userdefined header files
#include"../CppProperties/CppProperties.h"
#include "../StrHelper.h"
#include "../Convert/Convert.h"


/////////////////////////////////////////////////////////////////////
// Element class represents a data record in our NoSql database
// - in our NoSql database that is just the value in a key/value pair
// - it also stores the child data as a vector
/////////////////////////////////////////////////////////////////////
template<typename Data>
class Element
{
public:
	using Name = std::string;
	using Category = std::string;
	using Descr = std::string;
	using TimeDate = std::string;

	Property<Name> name;            // metadata
	Property<Category> category;    // metadata
	Property<TimeDate> timeDate;    // metadata
	Property<Descr> description;	  // metadata
	Property<Data> data;            // data
	std::vector<std::string> children_; //metadata

	std::string show();					// function to display Element's contents
	std::string getTimeDate();			// function to get the current Date and Time
	void addChild(const std::string& key);//function to add children to an element
	void deleteChild();					//function to delete an element
	std::vector<std::string>& getChildren();//function to retrieve the children of an element
};

//----< Element<T>::show() function >---------------------------------------------
/*
* Function to display the contents of an element
*/
template<typename Data>
std::string Element<Data>::show()
{
	std::ostringstream out;
	out.setf(std::ios::adjustfield, std::ios::left);
	out << "\n    " << std::setw(8) << "name" << " : " << name; // displaying each metadata
	out << "\n    " << std::setw(8) << "category" << " : " << category;
	out << "\n    " << std::setw(8) << "description" << " : " << description;
	out << "\n    " << std::setw(8) << "timeDate" << " : " << timeDate;
	out << "\n    " << std::setw(8) << "data" << " : " << data;
	if (children_.size() >= 0)		 // displaying the children of the element
	{
		out << "\n    children : ";
		for (std::string thisKey : children_)
		{
			out << thisKey << ", ";
		}
	}
	out << "\n";
	return out.str();
}


//----< Element<T>::getTimeDate() function >---------------------------------------------
/*
* Function to obtain the current Time and Date
*/
template<typename Data>
std::string Element<Data>::getTimeDate()
{
	time_t now = time(0);
	char * dt = ctime(&now);
	std::string timeStr = dt;
	return trim(timeStr);
}

//----< Element<T>::addChild() function >---------------------------------------------
/*
* Function to addChildren to an Element of a specified Key
*/
template<typename Data>
void Element<Data>::addChild(const std::string& key)
{
	children_.push_back(key);
}

//----< Element<T>::deleteChild() function >---------------------------------------------
/*
* Function to delete the children from an Element of a specified Key
*/
template<typename Data>
void Element<Data>::deleteChild()
{
	children_.clear();
}

//----< Element<T>::getChildren() function >---------------------------------------------
/*
* Function to obtain the children of an Element 
*/
template<typename Data>
std::vector<std::string>& Element<Data>::getChildren()
{
	return children_;
}

/////////////////////////////////////////////////////////////////////
// NoSqlDb class is a key/value pair in-memory database
// - stores and retrieves elements
// - implements toXml and fromXml functions
/////////////////////////////////////////////////////////////////////
template<typename Data>
class NoSqlDb
{
public:
	using Key = std::string;
	using Keys = std::vector<Key>;

	const int saveAfterCounts = 5;			// counters for autosave, after a certain number of writes
	int writeCounter = 0;
	Keys keys();								//function that stores vector of keys	
	bool save(Key key, Element<Data> elem);	//function to add the element to the database and save it
	void resave(Key key, Element<Data> elem);	// funvoction to resave the database after a modification to an element
	void deleteElem(Key key, Element<Data> elem);									// function to delete an element from the database
	void editText(Key key, Element<Data> elem, std::string newText);				//function to edit the description of an element
	void replaceInstance(Key key, Element<Data> elem, Element<Data> newElem);		// function to replace an existing value's instance
	Element<Data> value(Key key);				// function to return the value of a specified key
	size_t count();							// returns the size of the database
//	void fromXmlFile(NoSqlDb<Data>& db, std::string file);
private:
	using Item = std::pair<Key, Element<Data>>;
	std::unordered_map<Key, Element<Data>> store;
};

//----< NoSqlDb<T>::keys() function >---------------------------------------------
template<typename Data>
typename NoSqlDb<Data>::Keys NoSqlDb<Data>::keys()
{
	Keys keys;
	for (Item item : store)
	{
		keys.push_back(item.first); // storing the keys in a vector 
	}
	return keys;
}

//----< NoSqlDb<T>::save() function >---------------------------------------------
template<typename Data>
bool NoSqlDb<Data>::save(Key key, Element<Data> elem)
{
	if (store.find(key) != store.end())
		return false;
	store[key] = elem;
	writeCounter++;
	// if writeCounter > saveAfterCounts, persist the database contents
	return true;
}

//----< NoSqlDb<T>::resave() function >---------------------------------------------
template<typename Data>
void NoSqlDb<Data>::resave(Key key, Element<Data> elem)
{
	store[key] = elem;
	writeCounter++;
	// if writeCounter > saveAfterCounts, persist
}

//----< NoSqlDb<T>::deleteElem() function >---------------------------------------------
template<typename Data>
void NoSqlDb<Data>::deleteElem(Key key, Element<Data> elem)
{
	// delete logic
	store.erase(key);
}

//----< NoSqlDb<T>::editText() function >---------------------------------------------
template<typename Data>
void NoSqlDb<Data>::editText(Key key, Element<Data> elem, std::string newText)
{
	elem.description = newText;
	store[key] = elem;
}

//----< NoSqlDb<T>::replaceInstance() function >---------------------------------------------
template<typename Data>
void NoSqlDb<Data>::replaceInstance(Key key, Element<Data> elem, Element<Data> newElem)
{

	elem.data = newElem.data;
	store[key] = elem;
}

//----< NoSqlDb<T>::value() function >---------------------------------------------
template<typename Data>
Element<Data> NoSqlDb<Data>::value(Key key)
{
	if (store.find(key) != store.end())
		return store[key];
	return Element<Data>();
}

//----< NoSqlDb<T>::count() function >---------------------------------------------
template<typename Data>
size_t NoSqlDb<Data>::count()
{
	return store.size();
}


