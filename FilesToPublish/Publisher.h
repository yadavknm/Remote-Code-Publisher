#pragma once
////////////////////////////////////////////////////////////////////////////
// Publisher.h  -  Publishes the source code files						    
//  Language:      Visual C++ 2015										    
//  Platform:      HP Pavilion x-64, Windows 10							    
//  Application:   Dependency-Based Code Publisher - CSE687 Project3		
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
* This package contains a PublishCode class.
* Its purpose is to publish source code files (*.h and *.cpp) as .htm files
* This package also supports encoding of the source code files into web pages
*
* Required Files:
* ---------------
* FileSystem.h, DependencyAnalysis.h
*
* Build Process:
* --------------
* devenv CodePublisher.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 19 March, 2017
* - first release
*
*/
#include <iostream>
#include <fstream>
#include "../DependencyAnalysis/DependencyAnalysis.h"
#include "../FileSystem/FileSystem.h"

using depTable = std::unordered_map<std::string, std::set<std::string>>;
using namespace std;

////////////////////////////////////////////////////////////////////////////
// PublishCode class														
// - supports publishing of source code files								
// - encoding source code files into web readable format for publishing		
// - random ID generation for creating random IDs to support outlining		
////////////////////////////////////////////////////////////////////////////
class PublishCode {
public:
	 void publishCode(depTable depTable_, std::string cmdArg);		// function to publish code
	 void encodeHTMLTags(std::string& data);						// function to escape HTML tags in source code files
	 string randomIDGen();											// function to generate random ID used to assign for <div> tags
private:
	int count = 0;
};

//----< PublishCode::publishCode() function >---------------------------------
/*
* Publishes the source code files as web pages
*/
inline void PublishCode::publishCode(depTable depTable_, std::string cmdArg)
{
	string targetPath = "../PublishedFiles/";
	ofstream indexPage(targetPath + cmdArg);
	for (auto it = depTable_.begin(); it != depTable_.end(); ++it) {
		string fileName = FileSystem::Path::getName(it->first);
		string line;
		ifstream myfileIn(it->first);	
		string fileNameHTM = targetPath + fileName + ".htm";
		ofstream myfileOut(fileNameHTM);
		myfileOut << "<!DOCTYPE html>\n";
		myfileOut << "<html>\n<head>\n<link rel=\"stylesheet\" href=\"styles.css\">\n";
		myfileOut << "<script src = \"HideUnhide.js\">";
		myfileOut << "</script>";
		myfileOut << "</head>\n";
		myfileOut << "<body>\n";
		myfileOut << "<h3>";
		myfileOut << FileSystem::Path::getName(it->first);
		myfileOut << "</h3><hr />";
		myfileOut << "<div class=\"indent\"><h4>Dependencies:</h4>";
		for (auto dep : it->second) {
			string depHTM = dep + ".htm";
			myfileOut << "<a href=\"" << FileSystem::Path::getName(depHTM) << "\">"<< "<label>" << FileSystem::Path::getName(dep) << "</label>" << "</a>" << "\n";
		}
		myfileOut << "</div><hr />";
		myfileOut << "<pre>\n";
		indexPage << "<a href=\"" << fileNameHTM << "\">" << fileNameHTM << "</a>" << "<br />";
		if (myfileIn.is_open())
		{
			while (getline(myfileIn, line))
			{
				encodeHTMLTags(line);
				myfileOut << line << '\n';
			}
			myfileIn.close();
		}
		else cout << "Unable to open file";
		myfileOut << "</pre>\n";
		myfileOut << "</body>\n</html>\n";
		if (myfileOut.is_open())
		{
			myfileOut.close();
		}
	}
}

//----< PublishCode::encodeHTMLTags() function >---------------------------------
/*
* encodes the source code files with HTML tags to make it web readable
*/
inline void PublishCode::encodeHTMLTags(std::string& data) {
	std::string buffer;
	buffer.reserve(data.size());
	for (size_t pos = 0; pos != data.size(); ++pos) {
		switch (data[pos]) {
		case '&':  buffer.append("&amp;");       break;
		case '\"': buffer.append("&quot;");      break;
		case '\'': buffer.append("&apos;");      break;
		case '<':  buffer.append("&lt;");        break;
		case '>':  buffer.append("&gt;");        break;
		case '{': {
			string randomID = randomIDGen();
			buffer.append("<button id=\"Button\" onClick=\"hideUnhideScope('" + randomID + "')\">" + "-" + "</button>{");
			buffer.append("<div id=\"" + randomID + "\">");
		}										break;
		case '}': buffer.append("</div>}");		break;
		default:   buffer.append(&data[pos], 1); break;
		}
	}
	data.swap(buffer);
}

//----< PublishCode::randomIDGen() function >---------------------------------
/*
* Generates a random ID to be assigned to <div> tags to support outlining feature
*/
inline string PublishCode::randomIDGen() {
	count++;
	std::string s = std::to_string(count);
	return s;
}
