#pragma once
//////////////////////////////////////////////////////////////////////////////
// MsgServer.h  -  Server  one-way HTTP messaging and file transfer			//
//  Language:      Visual C++ 2015										    //
//  Platform:      HP Pavilion x-64, Windows 10							    //
//  Application:   Remote Code Publisher - CSE687 Project4					//
//  Author:        Yadav Murthy, Syracuse University					    //
//                 (315) 608-1734, ynarayan@syr.edu						    //
//																		    //
//////////////////////////////////////////////////////////////////////////////
/*
* Manual Information :-
* ----------------------
* This package implements a server that receives HTTP style messages and
* files from multiple concurrent clients and simply displays the messages
* and stores files.
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
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include "../Analyzer/Executive.h"
#include <string>
#include <iostream>



using namespace CodeAnalysis;
using namespace Async;
using namespace Logging;
using Show = StaticLogger<1>;
using namespace Utilities;


/////////////////////////////////////////////////////////////////////
// ClientHandler class
/////////////////////////////////////////////////////////////////////
// - instances of this class are passed by reference to a SocketListener
// - when the listener returns from Accept with a socket it creates an
//   instance of this class to manage communication with the client.
// - You no longer need to be careful using data members of this class
//   because each client handler thread gets its own copy of this 
//   instance so you won't get unwanted sharing.
// - I changed the SocketListener semantics to pass
//   instances of this class by value for version 5.2.
// - that means that all ClientHandlers need copy semantics.
//
class ClientHandler
{
public:
	ClientHandler(BlockingQueue<HttpMessage>& msgQ); /*: msgQ_(msgQ) {}*/
	void operator()(Socket socket);
	void commandLineArgs(int argc, char* argv[]);
	int argc_ch;
	char* argv_ch[6];
private:
	bool connectionClosed_;
	HttpMessage readMessage(Socket& socket);
	bool readFile(const std::string& filename, size_t fileSize, Socket& socket, std::string cat);
	BlockingQueue<HttpMessage>& msgQ_;
};



class ServerSender
{
public:
	using EndPoint = std::string;
	void execute(const size_t TimeBetweenMessages, const size_t NumMessages);
private:
	HttpMessage makeMessage(size_t n, const std::string& msgBody, const EndPoint& ep);
	void sendMessage(HttpMessage& msg, Socket& socket);
	bool sendFile(const std::string& fqname, Socket& socket);
};
