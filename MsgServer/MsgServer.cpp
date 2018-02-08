//////////////////////////////////////////////////////////////////////////////
// MsgServer.cpp  -  Server  one-way HTTP messaging and file transfer		//
//  Language:      Visual C++ 2015										    //
//  Platform:      HP Pavilion x-64, Windows 10							    //
//  Application:   Remote Code Publisher - CSE687 Project4					//
//  Author:        Yadav Murthy, Syracuse University					    //
//                 (315) 608-1734, ynarayan@syr.edu						    //
//																		    //
//////////////////////////////////////////////////////////////////////////////
/*
* This package implements a server that receives HTTP style messages and
* files from multiple concurrent clients and simply displays the messages
* and stores files.
*
*/
/*
* Required Files:
*   MsgClient.cpp, MsgServer.cpp
*   HttpMessage.h, HttpMessage.cpp
*   Cpp11-BlockingQueue.h
*   Sockets.h, Sockets.cpp
*   FileSystem.h, FileSystem.cpp
*   Logger.h, Logger.cpp
*   Utilities.h, Utilities.cpp
*/

#include "../MsgServer/MsgServer.h"
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

// -- Constructor 

ClientHandler::ClientHandler(BlockingQueue<HttpMessage>& msgQ) : msgQ_(msgQ) {}

//----< this defines processing to frame messages >------------------

HttpMessage ClientHandler::readMessage(Socket& socket) {
	connectionClosed_ = false;  HttpMessage msg;
	while (true) {  // read message attributes
		std::string attribString = socket.recvString('\n');
		if (attribString.size() > 1) {
			HttpMessage::Attribute attrib = HttpMessage::parseAttribute(attribString);      msg.addAttribute(attrib);
		}
		else
			break;
	}
	if (msg.attributes().size() == 0) {	  // If client is done, connection breaks and recvString returns empty string
		connectionClosed_ = true;    return msg;
	}
	if (msg.attributes()[0].first == "POST") {  // read body if POST - all messages in this demo are POSTs
		std::string filename = msg.findValue("file");   // is this a file message?
		if (filename != "") {
			size_t contentSize;      std::string sizeString = msg.findValue("content-length");
			if (sizeString != "")
				contentSize = Converter<size_t>::toValue(sizeString);
			else
				return msg;
			readFile(filename, contentSize, socket, msg.attributes()[2].second);
		}
		if (filename != "") {	  // construct message body
			msg.removeAttribute("content-length");      std::string bodyString = "<file>" + filename + "</file>";
			std::string sizeString = Converter<size_t>::toString(bodyString.size());
			msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));      msg.addBody(bodyString);
		}
		else {
			size_t numBytes = 0;
			size_t pos = msg.findAttribute("content-length");
			if (pos < msg.attributes().size()) {
				numBytes = Converter<size_t>::toValue(msg.attributes()[pos].second);
				Socket::byte* buffer = new Socket::byte[numBytes + 1];        socket.recv(numBytes, buffer);
				buffer[numBytes] = '\0';        std::string msgBody(buffer);        msg.addBody(msgBody);
				delete[] buffer;
			}
		}
	}
	if (msg.attributes()[0].first == "GET") {
		ServerSender s1;	  s1.execute(100, 1);
		Show::stop();	  CodeAnalysisExecutive exec;
		exec.executeMain(argc_ch, argv_ch);	  Show::start();
	}
	return msg;
}
//----< read a binary file from socket and save >--------------------
/*
 * This function expects the sender to have already send a file message,
 * and when this function is running, continuosly send bytes until
 * fileSize bytes have been sent.
 */
bool ClientHandler::readFile(const std::string& filename, size_t fileSize, Socket& socket, std::string cat)
{

	std::string fqname;
	if (cat == "Category1")
		fqname = "../ServerRepo/Category1/" + filename;
	if (cat == "Category2")
		fqname = "../ServerRepo/Category2/" + filename;
	//std::string fqname = "../TestFiles/" + filename + ".snt";  
   // std::string fqname = "../ServerRepo/" + filename;  
	FileSystem::File file(fqname);
	file.open(FileSystem::File::out, FileSystem::File::binary);
	if (!file.isGood())
	{
		/*
		 * This error handling is incomplete.  The client will continue
		 * to send bytes, but if the file can't be opened, then the server
		 * doesn't gracefully collect and dump them as it should.  That's
		 * an exercise left for students.
		 */
		Show::write("\n\n  can't open file " + fqname);
		return false;
	}

	const size_t BlockSize = 2048;
	Socket::byte buffer[BlockSize];

	size_t bytesToRead;
	while (true)
	{
		if (fileSize > BlockSize)
			bytesToRead = BlockSize;
		else
			bytesToRead = fileSize;

		socket.recv(bytesToRead, buffer);

		FileSystem::Block blk;
		for (size_t i = 0; i < bytesToRead; ++i)
			blk.push_back(buffer[i]);

		file.putBlock(blk);
		if (fileSize < BlockSize)
			break;
		fileSize -= BlockSize;
	}
	file.close();
	return true;
}
//----< receiver functionality is defined by this function >---------

void ClientHandler::operator()(Socket socket)
{
	while (true)
	{
		HttpMessage msg = readMessage(socket);
		if (connectionClosed_ || msg.bodyString() == "quit")
		{
			Show::write("\n\n  clienthandler thread is terminating");
			break;
		}
		msgQ_.enQ(msg);
	}
}

void ClientHandler::commandLineArgs(int argc, char* argv[]) {

	argc_ch = argc;
	for (int i = 0; i < argc; i++) {

		argv_ch[i] = argv[i];

	}
}


//----<  ServerSender::makeMessage() function >---------------------------------
/*
*Server makes the messages
*/
HttpMessage ServerSender::makeMessage(size_t n, const std::string& body, const EndPoint& ep)
{
	HttpMessage msg;
	HttpMessage::Attribute attrib;
	EndPoint myEndPoint = "localhost:8080";  // ToDo: make this a member of the sender
											 // given to its constructor.
	switch (n)
	{
	case 1:
		msg.clear();
		msg.addAttribute(HttpMessage::attribute("POST", "Message"));
		msg.addAttribute(HttpMessage::Attribute("mode", "oneway"));
		msg.addAttribute(HttpMessage::parseAttribute("toAddr:" + ep));
		msg.addAttribute(HttpMessage::parseAttribute("fromAddr:" + myEndPoint));

		msg.addBody(body);
		if (body.size() > 0)
		{
			attrib = HttpMessage::attribute("content-length", Converter<size_t>::toString(body.size()));
			msg.addAttribute(attrib);
		}
		break;
	default:
		msg.clear();
		msg.addAttribute(HttpMessage::attribute("Error", "unknown message type"));
	}
	return msg;
}

//----<  ServerSender::sendMessage() function >---------------------------------
/*
*Server sends the messages
*/
void ServerSender::sendMessage(HttpMessage& msg, Socket& socket)
{
	std::string msgString = msg.toString();
	socket.send(msgString.size(), (Socket::byte*)msgString.c_str());
}

//----<  ServerSender::sendFile() function >---------------------------------
/*
*Server sends the files
*/
bool ServerSender::sendFile(const std::string& filename, Socket& socket)
{
	// assumes that socket is connected

	std::string fqname = "../ServerRepo/" + filename;
	FileSystem::FileInfo fi(fqname);
	size_t fileSize = fi.size();
	std::string sizeString = Converter<size_t>::toString(fileSize);
	FileSystem::File file(fqname);
	file.open(FileSystem::File::in, FileSystem::File::binary);
	if (!file.isGood())
		return false;

	HttpMessage msg = makeMessage(1, "", "localhost::8081");
	msg.addAttribute(HttpMessage::Attribute("file", filename));
	msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
	sendMessage(msg, socket);
	const size_t BlockSize = 2048;
	Socket::byte buffer[BlockSize];
	while (true)
	{
		FileSystem::Block blk = file.getBlock(BlockSize);
		if (blk.size() == 0)
			break;
		for (size_t i = 0; i < blk.size(); ++i)
			buffer[i] = blk[i];
		socket.send(blk.size(), buffer);
		if (!file.isGood())
			break;
	}
	file.close();
	return true;
}

//----<  ServerSender::execute() function >---------------------------------
/*
*Execution of the server
*/
void ServerSender::execute(const size_t TimeBetweenMessages, const size_t NumMessages)
{
	Show::title(
		"Starting HttpMessage server on thread " + Utilities::Converter<std::thread::id>::toString(std::this_thread::get_id())
	);
	try
	{
		SocketSystem ss;
		SocketConnecter si;
		while (!si.connect("localhost", 8081))
		{
			Show::write("\n server waiting to connect");
			::Sleep(100);
		}
		//  send all *.htm files from ServerRepo folder

		//std::vector<std::string> files = FileSystem::Directory::getFiles("../PublishedFiles", "*.htm");
		std::vector<std::string> files = FileSystem::Directory::getFiles("../ServerRepo", "*.htm");
		for (size_t i = 0; i < files.size(); ++i)
		{
			Show::write("\n\n  sending file " + files[i]);
			sendFile(files[i], si);
		}
		// shut down client's server handler
		HttpMessage msg;
		msg = makeMessage(1, "quit", "toAddr:localhost:8081");
		sendMessage(msg, si);
		Show::write("\n\n  server sent\n" + msg.toIndentedString());

		Show::write("\n");
		Show::write("\n  All done folks");
		Show::write("\n  Server displaying the dependencies ");
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}



//----< main() >--------------------------------------------------

int main(int argc, char* argv[])
{
	//std::cout << argc;
	//for (int i = 0; i < argc; i++)
	//{
	//	std::cout << argv[i] << "\n";
	//}
	//ServerSender s1;
	//s1.argc_loc = argc;
	//for (int i = 0; i < argc; i++)
	//{
	//	s1.argv_loc[i] = argv[i];
	//}


	::SetConsoleTitle(L"HttpMessage Server - Runs Forever");

	Show::start();
	Show::attach(&std::cout);
	Show::title("\n  HttpMessage Server started");

	BlockingQueue<HttpMessage> msgQ;

	try
	{
		SocketSystem ss;
		SocketListener sl(8080, Socket::IP6);
		ClientHandler cp(msgQ);
		sl.start(cp);

		char* args[6];
		args[0] = "";
		args[1] = "../ServerRepo"; //making a fixed path for server repository
		args[2] = "*.h";
		args[3] = "*.cpp";
		args[4] = "/m";
		args[5] = "index.htm";

		cp.commandLineArgs(6, args);



		while (true)
		{
			HttpMessage msg = msgQ.deQ();
			Show::write("\n\n  server recvd message with body contents:\n" + msg.bodyString());
		}

	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}