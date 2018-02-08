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
 */
 /*
 * Required Files:
 *   MsgClient.h
 *   MsgClient.cpp, MsgServer.cpp
 *   HttpMessage.h, HttpMessage.cpp
 *   Cpp11-BlockingQueue.h
 *   Sockets.h, Sockets.cpp
 *   FileSystem.h, FileSystem.cpp
 *   Logger.h, Logger.cpp
 *   Utilities.h, Utilities.cpp
 */

#include "../MsgClient/MsgClient.h"
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


//----< factory for creating messages >------------------------------
/*
 * This function only creates one type of message for this demo.
 * - To do that the first argument is 1, e.g., index for the type of message to create.
 * - The body may be an empty string.
 * - EndPoints are strings of the form ip:port, e.g., localhost:8081. This argument
 *   expects the receiver EndPoint for the toAddr attribute.
 */
HttpMessage ClientSender::makeMessage(size_t n, const std::string& body, const EndPoint& ep)
{
	HttpMessage msg;  HttpMessage::Attribute attrib;  EndPoint myEndPoint = "localhost:8081";
	switch (n) {
	case 1:
		msg.clear();
		msg.addAttribute(HttpMessage::attribute("POST", "Message"));
		msg.addAttribute(HttpMessage::Attribute("mode", "oneway"));
		msg.addAttribute(HttpMessage::attribute("Category", "Category1"));
		msg.addAttribute(HttpMessage::parseAttribute("toAddr:" + ep));
		msg.addAttribute(HttpMessage::parseAttribute("fromAddr:" + myEndPoint));
		msg.addBody(body);
		if (body.size() > 0) {
			attrib = HttpMessage::attribute("content-length", Converter<size_t>::toString(body.size()));
			msg.addAttribute(attrib);
		}
		break;
	case 2:
		msg.clear();
		msg.addAttribute(HttpMessage::attribute("POST", "Message"));
		msg.addAttribute(HttpMessage::Attribute("mode", "oneway"));
		msg.addAttribute(HttpMessage::attribute("Category", "Category2"));
		msg.addAttribute(HttpMessage::parseAttribute("toAddr:" + ep));
		msg.addAttribute(HttpMessage::parseAttribute("fromAddr:" + myEndPoint));
		msg.addBody(body);
		if (body.size() > 0) {
			attrib = HttpMessage::attribute("content-length", Converter<size_t>::toString(body.size()));
			msg.addAttribute(attrib);
		}
		break;
	case 3:
		msg.clear();
		msg.addAttribute(HttpMessage::attribute("GET", "Message"));
		msg.addAttribute(HttpMessage::Attribute("mode", "oneway"));
		msg.addAttribute(HttpMessage::parseAttribute("toAddr:" + ep));
		msg.addAttribute(HttpMessage::parseAttribute("fromAddr:" + myEndPoint));
		msg.addBody(body);
		if (body.size() > 0) {
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

//----< send message using socket >----------------------------------

void ClientSender::sendMessage(HttpMessage& msg, Socket& socket)
{
	std::string msgString = msg.toString();
	socket.send(msgString.size(), (Socket::byte*)msgString.c_str());
}
//----< send file using socket >-------------------------------------
/*
 * - Sends a message to tell receiver a file is coming.
 * - Then sends a stream of bytes until the entire file
 *   has been sent.
 * - Sends in binary mode which works for either text or binary.
 */
bool ClientSender::sendFile(const std::string& filename, Socket& socket, int cat)
{	// assumes that socket is connected
	
	std::string fqname;
	std::string fqname1 = "../ClientRepo/Category1/" + filename;
	std::string fqname2 = "../ClientRepo/Category2/" + filename;
	if (cat == 1)
	{
		fqname = fqname1;
	}
	if (cat == 2)
	{
		fqname = fqname2;
	}
	FileSystem::FileInfo fi(fqname);
	size_t fileSize = fi.size();
	std::string sizeString = Converter<size_t>::toString(fileSize);
	FileSystem::File file(fqname);
	file.open(FileSystem::File::in, FileSystem::File::binary);
	if (!file.isGood())
		return false;

	HttpMessage msg = makeMessage(1, "", "localhost::8080");
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
//----< this defines the behavior of the client >--------------------

void ClientSender::execute(const size_t TimeBetweenMessages, const size_t NumMessages, int category)
{
	ClientCounter counter;  size_t myCount = counter.count();
	std::string myCountString = Utilities::Converter<size_t>::toString(myCount);
	Show::title(
		"Starting HttpMessage client" + myCountString +
		" on thread " + Utilities::Converter<std::thread::id>::toString(std::this_thread::get_id())
	);
	try {
		SocketSystem ss;    SocketConnecter si;
		while (!si.connect("localhost", 8080)) {
			Show::write("\n client waiting to connect");
			::Sleep(100);
		}		HttpMessage msg; 
		std::cout << "\n\n REQUIREMENT 7: The communication system shall provide support for passing HTTP style messages using either synchronous request/response or asynchronous one-way messaging. \n";
		for (size_t i = 0; i < NumMessages; ++i) {
			std::string msgBody =
				"<msg>Message #" + Converter<size_t>::toString(i + 1) +
				" from client #" + myCountString + "</msg>";
			msg = makeMessage(1, msgBody, "localhost:8080");
			sendMessage(msg, si);
			Show::write("\n\n  client" + myCountString + " sent\n" + msg.toIndentedString());
			::Sleep(TimeBetweenMessages);
		}	std::cout << "\n\n REQUIREMENT 5: Shall provide a Client program that can upload files \n";
		std::cout << "\n\n REQUIREMENT 8: The communication system shall also support sending and receiving streams of bytes6. Streams will be established with an initial exchange of messages. \n";
		std::vector<std::string> Category1_files = FileSystem::Directory::getFiles("../ClientRepo/Category1", "*.*");
		for (size_t i = 0; i < Category1_files.size(); ++i) {
			Show::write("\n\n  sending file from Category 1\n" + Category1_files[i]);			sendFile(Category1_files[i], si, 1);
		}
		std::vector<std::string> Category2_files = FileSystem::Directory::getFiles("../ClientRepo/Category2", "*.*");
		for (size_t i = 0; i < Category2_files.size(); ++i) {
			Show::write("\n\n  sending file from Category 2\n" + Category2_files[i]);			sendFile(Category2_files[i], si, 2);
		}
		msg = makeMessage(3, "GET", "localhost:8080");	// trying to receieve files
		sendMessage(msg, si);		std::cout << "\n\n REQUIREMENT 5: Provide a Client program that can view Repository contents";
		Show::write("\n\n  client" + myCountString + " sent\n" + msg.toIndentedString());
		msg = makeMessage(1, "quit", "toAddr:localhost:8080");  // shut down server's client handler
		sendMessage(msg, si);
		Show::write("\n\n  client" + myCountString + " sent\n" + msg.toIndentedString());
		Show::write("\n");    Show::write("\n  All done folks"); std::cout << "\n\n REQUIREMENT 9: Shall include an automated unit test suite that demonstrates you meet all the requirements of this project4 including the transmission of files. \n The requirements have been demonstrated on two consoles.";
	}
	catch (std::exception& exc) {
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";    Show::write(exMsg);
	}
}

//----<  ServerHandler::readMessage() function >---------------------------------
/*
* Reads messages sent from client
*/
HttpMessage ServerHandler::readMessage(Socket& socket) {
	connectionClosed_ = false;	HttpMessage msg;
	while (true) {	// read message attributes
		std::string attribString = socket.recvString('\n');
		if (attribString.size() > 1) {
			HttpMessage::Attribute attrib = HttpMessage::parseAttribute(attribString);
			msg.addAttribute(attrib);
		}
		else
			break;
	}
	if (msg.attributes().size() == 0) {	// If client is done, connection breaks and recvString returns empty string
		connectionClosed_ = true;
		return msg;
	}
	if (msg.attributes()[0].first == "POST") {	// read body if POST - all messages in this demo are POSTs
		std::string filename = msg.findValue("file");	// is this a file message?
		if (filename != "") {
			size_t contentSize;			std::string sizeString = msg.findValue("content-length");
			if (sizeString != "")
				contentSize = Converter<size_t>::toValue(sizeString);
			else
				return msg;
			readFile(filename, contentSize, socket);
		}
		if (filename != "") {	// construct message body
			msg.removeAttribute("content-length");			std::string bodyString = "<file>" + filename + "</file>";
			std::string sizeString = Converter<size_t>::toString(bodyString.size());
			msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));			msg.addBody(bodyString);
		}
		else {
			size_t numBytes = 0;			size_t pos = msg.findAttribute("content-length");
			if (pos < msg.attributes().size()) {
				numBytes = Converter<size_t>::toValue(msg.attributes()[pos].second);
				Socket::byte* buffer = new Socket::byte[numBytes + 1];
				socket.recv(numBytes, buffer);
				buffer[numBytes] = '\0';	std::string msgBody(buffer);	msg.addBody(msgBody);
				delete[] buffer;
			}
		}
	}
	return msg;
}

//----<  ServerHandler::readFile() function >---------------------------------
/*
* Reads files sent from client
*/
bool ServerHandler::readFile(const std::string& filename, size_t fileSize, Socket& socket)
{
	//std::string fqname = "../TestFiles/" + filename + ".snt";     --> ProfCode
	std::string fqname = "../ClientRepo/" + filename;  //  --> MyCode
	FileSystem::File file(fqname);
	file.open(FileSystem::File::out, FileSystem::File::binary);
	if (!file.isGood())
	{
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

//----<  ServerHandler::operator() function >---------------------------------

void ServerHandler::operator()(Socket socket)
{
	while (true)
	{
		HttpMessage msg = readMessage(socket);
		if (connectionClosed_ || msg.bodyString() == "quit")
		{
			Show::write("\n\n  serverhandler thread is terminating");
			break;
		}
		msgQ_.enQ(msg);
	}
}

//----< entry point - runs clients each on its own thread >------

int main()
{
	::SetConsoleTitle(L"Clients Running on Threads");
	std::cout << "\n\n REQUIREMENT 1: Shall use Visual Studio 2015 and its C++ Windows console projects, as provided in the ECS computer labs \n";
	std::cout << "\n\n REQUIREMENT 2: Shall use the C++ standard library's streams for all console I/O and new and delete for all heap-based memory management. \n";
	std::cout << "\n\n REQUIREMENT 3: Shall provide a Repository program that provides functionality to publish, as linked web pages, the contents of a set of C++ source code files. \n An index page will be shown with the links";
	std::cout << "\n\n REQUIREMENT 4: Shall, for the publishing process, satisfy the requirements of CodePublisher developed in Project #3 \n ";
	std::cout << "\n\n REQUIREMENT 6: Shall provide a message-passing communication system, based on Sockets, used to access the Repository's functionality from another process or machine\n ";
	Show::title("Demonstrating two HttpMessage Clients each running on a child thread");
	Show::attach(&std::cout);
	Show::start();
	Show::title("\n  HttpMessage Client started");
	BlockingQueue<HttpMessage> msgQ;
	try
	{
		SocketSystem ss;
		SocketListener sl(8081, Socket::IP6);
		ServerHandler cp(msgQ);
		sl.start(cp);

		
		ClientSender c1;
		std::thread t1(
			[&]() { c1.execute(100, 1, 1); } 
		);
		t1.join();
		/*
		* Since this is a server the loop below never terminates.
		* We could easily change that by sending a distinguished
		* message for shutdown.
		*/

		while (true)
		{
			HttpMessage msg = msgQ.deQ();
			Show::write("\n\n  client recvd message with body contents:\n" + msg.bodyString());
		}
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
	
}