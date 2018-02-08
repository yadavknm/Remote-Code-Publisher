#pragma once
//////////////////////////////////////////////////////////////////////////////
// MsgClient.h  -  Demonstrates Client one-way HTTP messaging and			//
//					file transfer											//
//  Language:      Visual C++ 2015										    //
//  Platform:      HP Pavilion x-64, Windows 10							    //
//  Application:   Remote Code Publisher - CSE687 Project4					//
//  Author:        Yadav Murthy, Syracuse University					    //
//                 (315) 608-1734, ynarayan@syr.edu						    //
//																		    //
//////////////////////////////////////////////////////////////////////////////
/*
* This package implements a client that sends HTTP style messages and
* files to a server that simply displays messages and stores files.
*
* Required Files:
* ---------------
* FileSystem.h, Logger.h, Sockets.h, HttpMessage.h, Utilities.h, Cpp11-BlockingQueue.h
*	
*
* Build Process:
* --------------
* devenv CodePublisher.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 5 May, 2017
* - first release
*
*/
#include "../HttpMessage/HttpMessage.h"
#include "../Sockets/Sockets.h"
#include "../FileSystem/FileSystem.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include <string>
#include <iostream>
#include <thread>
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#pragma warning(disable: 4267)

using namespace Async;
using namespace Logging;
using Show = StaticLogger<1>;
using namespace Utilities;
using Utils = StringHelper;

/////////////////////////////////////////////////////////////////////
// ClientCounter creates a sequential number for each client
//
class ClientCounter
{
public:
	ClientCounter() { ++clientCount; }
	size_t count() { return clientCount; }
private:
	static size_t clientCount;
};

size_t ClientCounter::clientCount = 0;

/////////////////////////////////////////////////////////////////////
// MsgClient class
// - was created as a class so more than one instance could be 
//   run on child thread
//
class ClientSender
{
public:
	using EndPoint = std::string;
	void execute(const size_t TimeBetweenMessages, const size_t NumMessages, int category);
private:
	HttpMessage makeMessage(size_t n, const std::string& msgBody, const EndPoint& ep);
	void sendMessage(HttpMessage& msg, Socket& socket);
	bool sendFile(const std::string& fqname, Socket& socket, int cat);
};


class ServerHandler
{
public:
	ServerHandler(BlockingQueue<HttpMessage>& msgQ) : msgQ_(msgQ) {}
	void operator()(Socket socket);
private:
	bool connectionClosed_;
	HttpMessage readMessage(Socket& socket);
	bool readFile(const std::string& filename, size_t fileSize, Socket& socket);
	BlockingQueue<HttpMessage>& msgQ_;
};