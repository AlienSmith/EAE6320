#pragma once
#include <iostream>
#include <External/Asio/asio.hpp>
#include "ThreadGuard.h"
//Shut down and close
void ClientSendData(asio::ip::tcp::socket& sock) {
	const char requrest_buf[] = { 0x48,0x65,0x6c,0x6c,0x6f };
	//sync write
	asio::write(sock, asio::buffer(requrest_buf));
	//Shut down the send part of the socket can no longer write anymore
	sock.shutdown(asio::socket_base::shutdown_send);
	asio::streambuf response_buf;
	asio::error_code ec;
	//this will be blocked until all info has been read
	asio::read(sock, response_buf, ec);
	if (ec == asio::error::eof) {
		//All response message has been recieved;
		//Handle it here
	}
	else {
		throw asio::system_error(ec);
	}
	return;
}
void ServerRecieveData(asio::ip::tcp::socket& sock) {
	asio::streambuf request_buf;
	asio::error_code ec;
	asio::read(sock, request_buf, ec);
	//eof means the client finished sending data
	if (ec != asio::error::eof) {
		throw asio::system_error(ec);
		const char response_buf[] = { 0x48,0x69,0x21 };
		asio::write(sock, asio::buffer(response_buf));
		sock.shutdown(asio::socket_base::shutdown_send);
	}
}
void Client() {
	std::string host = "127.0.0.1";
	unsigned short port_num = 3333;
	asio::io_service ios;
	try {
		asio::ip::tcp::endpoint ep(asio::ip::address::from_string(host), port_num);
		asio::ip::tcp::socket sock(ios, ep.protocol());
		sock.connect(ep);
		ClientSendData(sock);
	}
	catch (asio::system_error& e) {
		std::cout << "Error occured!"
			<< "Error code =" << e.code() << ".Message:" << e.what();
	}
	return;
}
void Server(){
	std::cout << "Server Start" << std::endl;
	unsigned short port_num = 3333;
	const unsigned short BACK_BUF_SIZE = 30;
	try {
		asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(), port_num);
		asio::io_service ios;
		//Create open and bind
		asio::ip::tcp::acceptor acceptor = asio::ip::tcp::acceptor(ios, ep);
		asio::ip::tcp::socket sock(ios);
		acceptor.listen(BACK_BUF_SIZE);
		acceptor.accept(sock);
		ServerRecieveData(sock);
	}
	catch (asio::system_error& e) {
			std::cout << "Error occured! Error code" << e.code() << ".Message" << e.what();
	}
}
void ShutdownAndCloseEntrance() {
	//Server();
	//std::thread server([] {Server(); });Server()
	//std::this_thread::sleep_for(std::chrono::seconds(2));
	Client();
	//std::thread cliant([] {Client(); });
	std::cout << "Thread started" << std::endl;
	return;
}
//Cancel Async calls
void Cancel_Async_Call() {
	//Client
	std::string host = "127.0.0.1";
	unsigned int port = 3333;
	try {
		asio::ip::tcp::endpoint ep(asio::ip::address::from_string(host), port);
		asio::io_service ios;
		std::shared_ptr<asio::ip::tcp::socket> sock(new asio::ip::tcp::socket(ios, ep.protocol()));
		//async connect with lambda callback function
		sock->async_connect(ep, [sock](const asio::error_code ec) {
				//if canceled or error occured this ec will have error code
			if (ec.value() != 0) {
				if (ec == asio::error::operation_aborted) {
					std::cout << "Operation canceled"<<std::endl;
				}
				else {
					std::cout << "Error occured!"
						<< "Error code =" << ec << ".Message:" << ec.message();
				}
				return;
			}
			else {
				std::cout << "Connect success"<<std::endl;
			}
			});
		std::thread worker_thread([&ios]() {try {
			ios.run();
		}
		catch (asio::system_error& e) {
			std::cout << "Error occured!"
				<< "Error code =" << e.code() << ".Message:" << e.what();
		}});
		//wait for connection for 2 seconds 
		std::this_thread::sleep_for(std::chrono::seconds(2));
		//Cancel whatever operation on the socket
#pragma warning(suppress : 4996)
		sock->cancel();
		//Since the function has been canceled it will return imidiately
		worker_thread.join();
	}
	catch (asio::system_error& e) {
		std::cout << "Error occured!"
			<< "Error code =" << e.code() << ".Message:" << e.what();
	}
	return;
}
// Read Async
struct ARead_Session {
	//Async read 
	std::shared_ptr<asio::ip::tcp::socket> sock;
	std::unique_ptr<char[]> buf;
	unsigned int buf_size;
};
void AReadCallBack(const asio::error_code& e,std::shared_ptr<ARead_Session> s){
	if (e == asio::error::eof) {
		//client has cut the connection
		std::string output(s->buf.get());
		std::cout << output << std::endl;
		return;
	}
	else if (e.value() != 0) {
		std::cout << "Error occured! Error Code =" << e << ".Message" << e.message();
		return;
	}
	else {
		//client has cut the connection
		std::string output(s->buf.get());
		std::cout << output << std::endl;
		return;
	}
}
void AshcReadTCP(std::shared_ptr<asio::ip::tcp::socket> sock) {
	const unsigned int BACK_BUFFER_SIZE = 7;
	std::shared_ptr<ARead_Session> s(new ARead_Session());
	s->buf.reset(new char[BACK_BUFFER_SIZE]);
	s->buf_size = BACK_BUFFER_SIZE;
	s->sock = sock;
}
struct Read_Session {
	std::shared_ptr<asio::ip::tcp::socket> sock;
	std::unique_ptr<char[]> buf;
	std::size_t total_bytes_read;
	unsigned int buf_size;
};
void ReadCallBack(const asio::error_code& e,
	std::size_t bytes_transfered, 
	std::shared_ptr<Read_Session> s){
	if (e == asio::error::eof) {
		//TODO This is success?
		std::string output(s->buf.get());
		std::cout << output << std::endl;
		return;
	}
	if (e.value() != 0) {
		std::cout << "Error occured! Error Code =" << e << ".Message" << e.message();
		return;
	}
	s->total_bytes_read += bytes_transfered;

	if (s->total_bytes_read == s->buf_size) {
		std::string output(s->buf.get());
		std::cout << output << std::endl;
		return;
	}

	s->sock->async_read_some(
		asio::buffer(s->buf.get() + s->total_bytes_read, s->buf_size - s->total_bytes_read),
		std::bind(ReadCallBack,
			std::placeholders::_1,
			std::placeholders::_2,
			s));
}
int ReadSomeAsyncTCP(std::shared_ptr<asio::ip::tcp::socket> sock) {
	std::shared_ptr<Read_Session> s(new Read_Session());
	const unsigned int MESSAGE_SIZE = 7;
	s->buf.reset(new char[MESSAGE_SIZE]);
	s->total_bytes_read = 0;
	s->sock = sock;
	s->buf_size = MESSAGE_SIZE;
	s->sock->async_read_some(
		asio::buffer(s->buf.get(), s->buf_size),
		std::bind(ReadCallBack, std::placeholders::_1, std::placeholders::_2, s));
	return 0;
}
int ReadSomeAsyncTCPPrintServer() {
	const unsigned char BACK_BLOCK_SIZE = 30;
	unsigned int port_num = 3333;
	asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(), port_num);
	asio::io_service ios;
	try {
		//Create and open
		asio::ip::tcp::acceptor acceptor(ios, ep.protocol());
		//Bind
		acceptor.bind(ep);
		std::cout << "Waiting to Connect" << std::endl;
		acceptor.listen(BACK_BLOCK_SIZE);
		//Do not open it
		std::shared_ptr<asio::ip::tcp::socket> sock(new asio::ip::tcp::socket(ios));
		acceptor.accept(*sock);
		std::cout << "Connected" << std::endl;
		ReadSomeAsyncTCP(sock);
		ios.run();
		/*std::thread t{ [&ios] {ios.run(); } };
		Networking::thread_guard g(t);*/
		std::cout << "Wait for socket thread to join" << std::endl;
	}
	catch (asio::system_error& e) {
		std::cout << "Error occured! Error code = " << e.code() << ".Message" << e.what();
	}
	return 0;
}
struct Session {
	//Used for async TCP write_some
	std::shared_ptr<asio::ip::tcp::socket> sock;
	std::string buf;
	std::size_t total_bytes_written;
};
struct Session_write {
	//Used for async TCP write;
	std::shared_ptr<asio::ip::tcp::socket> sock;
	std::string buf;
};
void async_write_CallBack(const asio::error_code& ec, std::size_t bytes_transfered,std::shared_ptr<Session_write> s) {
	std::cout << "Call Back func" << std::endl;
	if (ec.value() != 0) {
		//When error
		std::cout << "Failed write to tcp socket asynchronizely. Error Code = " << ec.value() << ".Message: " << ec.message();
		return;
	}
	//When all data in buf write and no error
}
void async_write_TCP() {
	std::string host = "127.0.0.1";
	unsigned int port = 3333;
	asio::ip::tcp::endpoint ep(asio::ip::address::from_string(host), port);
	asio::io_service ios;
	//create and open
	std::shared_ptr<asio::ip::tcp::socket> sock(new asio::ip::tcp::socket(ios, ep.protocol()));
	asio::error_code ec;
	// one client this connect func will bind the active socket to a ip and port_num chosen by the os before try to connect with the remote server
	sock->connect(ep, ec);
	if (ec.value() != 0) {
		std::cout << "Failed to Connect to Acceptor. Error Code = " << ec.value() << ".Message: " << ec.message();
	}
	std::shared_ptr<Session_write> session(new Session_write());
	session->sock = sock;
	session->buf = "Hello";
}
void async_write_some_CallBack(const asio::error_code& ec, std::size_t byte_transfered, std::shared_ptr<Session> s) {
	std::cout << "Call Back func" << std::endl;
	if (ec.value() != 0) {
		std::cout << "Failed write to tcp socket asynchronizely. Error Code = " << ec.value() << ".Message: " << ec.message();
		return;
	}
	s->total_bytes_written += byte_transfered;
	if (s->total_bytes_written == s->buf.length()) {
		std::cout << "Async write complete" << std::endl;
		return;
	}
	s->sock->async_write_some(asio::buffer(
		s->buf.c_str() + s->total_bytes_written,
		s->buf.length() - s->total_bytes_written),
		std::bind(async_write_some_CallBack, std::placeholders::_1, std::placeholders::_2, s));
}
void AsyncwriteToSocket(std::shared_ptr<asio::ip::tcp::socket> sock) {
	std::shared_ptr<Session> s(new Session);

	s->buf = std::string("Hello");
	s->total_bytes_written = 0;
	s->sock = sock;

	s->sock->async_write_some(
		asio::buffer(s->buf),
		std::bind(async_write_some_CallBack,std::placeholders::_1,std::placeholders::_2,s)
		);
}
int CreateOutputBuffer() {
	//allocate buffer
	const std::string buf = "Hello";
	//convert to constBufferSequence like item
	asio::const_buffers_1 output_buf = asio::buffer(buf);
	return 0;
}
int CreateInputBuffer() {
	const size_t BUF_SIZE_BYTES = 20;
	//allocate 
	std::unique_ptr<char[]> buf(new char[BUF_SIZE_BYTES]);
	//convert
	asio::mutable_buffers_1 input_buf = asio::buffer(static_cast<void*>(buf.get()), BUF_SIZE_BYTES);
	return 0;
}
int StreamBuf() {
	asio::streambuf buf;
	std::ostream output(&buf);
	output << "Message1\nMessage2";
	std::istream input(&buf);
	std::string message1;
	std::getline(input, message1);
	std::cout << message1.c_str()<<std::endl;
	return 0;
}

int writesomeToTCPsocket() {
	//tcp v4 connect to TCPServer
	std::string raw_ip_address = "127.0.0.1";
	unsigned int port_num = 3333;
	asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
	asio::io_service ios;
	asio::ip::tcp::socket sock(ios, ep.protocol());
	asio::error_code ec;
	// one client this connect func will bind the active socket to a ip and port_num chosen by the os before try to connect with the remote server
	sock.connect(ep, ec);
	if (ec.value() != 0) {
		std::cout << "Failed to Connect to Acceptor. Error Code = " << ec.value() << ".Message: " << ec.message();
		return ec.value();
	}
	std::string buf = "Hello";

	std::size_t total_bytes_written = 0;
	//the write_some function takes constbuffersequence
	// do not guarantee how many bytes will be write
	// return howmany bytes have been writen
	// the start of unuploaded buffer shifts by prvious total_bytes_written
	while (total_bytes_written != buf.length()) {
		total_bytes_written += sock.write_some(
			asio::buffer(buf.c_str() + total_bytes_written, 
				buf.length() - total_bytes_written));
	}
	return 0;

}
int ReadAndPrint(asio::ip::tcp::socket& sock) {
	const unsigned char MESSAGE_SIZE = 10;
	char buf[MESSAGE_SIZE];
	try {
		asio::read(sock, asio::buffer(buf, MESSAGE_SIZE));
	}
	catch (asio::system_error& e) {
		if (e.code() == asio::error::eof) {
			//Connection closed cleanly by peer.
		}
		else{
			std::cout << "Error occured! Error code = " << e.code() << ".Message" << e.what();
		}
	}
	std::string message(buf, MESSAGE_SIZE);
	std::cout << message.c_str() << std::endl;
	return 0;
}
int ReadFromSocketDelim(asio::ip::tcp::socket& sock) {
	asio::streambuf buf;
	//This might contain info after "\n"
	asio::read_until(sock, buf, '\n');

	std::string message;

	std::istream input_stream(&buf);
	//This will make sure info after "\n" will not get into the message
	std::getline(input_stream, message);
	return 0;
}
//This one reports error which I do not know why
int ReadSomeAndPrint(asio::ip::tcp::socket& sock) {
	//allocate mutable buffer
	const unsigned char MESSAGE_SIZE = 10;
	char buf[MESSAGE_SIZE];
	std::size_t total_bytes_read = 0;
	try {
		while (total_bytes_read != MESSAGE_SIZE) {
			total_bytes_read += sock.read_some(
				asio::buffer(buf + total_bytes_read,
					MESSAGE_SIZE - total_bytes_read)
			);
		}
	}
	catch (asio::system_error& e) {
		if (e.code() == asio::error::eof) {
			//Connection closed cleanly by peer.
		}
		else {
			std::cout << "Error occured! Error code = " << e.code() << ".Message" << e.what();
		}
	}
	std::string message(buf, total_bytes_read);
	std::cout << message.c_str() << std::endl;
	return 0;
}
void function() { return; }
int ReadTCPServer() {
//IPv4
//queue contain the pending connection request 
	int accepted_applications_left = 5;
	const int BACKLOG_SIZE = 30;
	unsigned int port_num = 3333;
	asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(), port_num);
	asio::io_service ios;
	try {
		std::vector<std::thread*> thread_pool;
		//Open acceptor
		asio::ip::tcp::acceptor acceptor(ios, ep.protocol());
		//Bind Acceptor
		acceptor.bind(ep);
		//StartListen
		acceptor.listen(BACKLOG_SIZE);
		//Create Active Socket
		while (accepted_applications_left >= 0) {
			std::cout << "Waiting for connection" << std::endl;
			asio::ip::tcp::socket* temp_socket(new asio::ip::tcp::socket(ios));
			acceptor.accept(*temp_socket);
			std::thread* t(new std::thread([temp_socket] {
				std::cout << "Connected" << std::endl;
				ReadAndPrint(*temp_socket);
				delete temp_socket;
				}));
			thread_pool.push_back(t);
			accepted_applications_left--;
		}
		//Acts like a barrier everythread terminate here before the mainthread moves on
		std::for_each(thread_pool.begin(), thread_pool.end(),[](std::thread* t) {
			t->join();
			delete t;
		});
	}
	catch (asio::system_error& e) {
		std::cout << "Error occured! Error code = " << e.code() << ".Message" << e.what();
	}
	return 0;
}
int ReadSomeTCPServer() {
	//IPv4
//queue contain the pending connection request 
	int accepted_applications_left = 5;
	const int BACKLOG_SIZE = 30;
	unsigned int port_num = 3333;
	asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(), port_num);
	asio::io_service ios;
	try {
		std::vector<std::thread> thread_pool;
		//Open acceptor
		asio::ip::tcp::acceptor acceptor(ios, ep.protocol());
		//Bind Acceptor
		acceptor.bind(ep);
		//StartListen
		acceptor.listen(BACKLOG_SIZE);
		std::cout << "Waiting for connection" << std::endl;
		asio::ip::tcp::socket sock(ios);
		acceptor.accept(sock);
		ReadAndPrint(sock);
	}
	catch (asio::system_error& e) {
		std::cout << "Error occured! Error code = " << e.code() << ".Message" << e.what();
	}
	return 0;
}
int writeToTCPsocket() {
	//tcp v4 connect to TCPServer
	std::string raw_ip_address = "127.0.0.1";
	unsigned int port_num = 3333;
	asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
	asio::io_service ios;
	//Create and open
	asio::ip::tcp::socket sock(ios, ep.protocol());
	asio::error_code ec;
	// one client this connect func will bind the active socket to a ip and port_num chosen by the os before try to connect with the remote server
	sock.connect(ep, ec);
	if (ec.value() != 0) {
		std::cout << "Failed to Connect to Acceptor. Error Code = " << ec.value() << ".Message: " << ec.message();
		return ec.value();
	}
	std::string buf = "Hello";
	//no while loop
	//no tracking of amont being write
	//no recreate buffer object again and again
	//underling it it is the same implementation as the while loop
	asio::write(sock, asio::buffer(buf));
	return 0;
}
int multithreadwritetoTCP() {
	std::string raw_ip_address = "127.0.0.1";
	unsigned int port_num = 3333;
	asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
	asio::io_service ios;
	std::shared_ptr<asio::ip::tcp::socket> sock(new asio::ip::tcp::socket(ios, ep.protocol()));
	asio::error_code ec;
	// one client this connect func will bind the active socket to a ip and port_num chosen by the os before try to connect with the remote server
	sock->connect(ep, ec);
	if (ec.value() != 0) {
		std::cout << "Failed to Connect to Acceptor. Error Code = " << ec.value() << ".Message: " << ec.message();
		return ec.value();
	}
	AsyncwriteToSocket(sock);
	//lambda function
	std::thread t{ [&ios] {ios.run(); } };
	//functor
	//std::thread t( asio::io_service::run,std::ref(ios) );
	Networking::thread_guard g(t);
	std::cout << "End here" << std::endl;
	return 0;
}
int writeToTCPsocketAsync() {
	//Notice this is still synchronized cause the main thread run ios.run() which submit the 
	//main thread to asio to run callback function When call back function of the last pending 
	//asynchronous operation complete this function return
	//tcp v4 connect to TCPServer
	std::string raw_ip_address = "127.0.0.1";
	unsigned int port_num = 3333;
	asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
	asio::io_service ios;
	std::shared_ptr<asio::ip::tcp::socket> sock (new asio::ip::tcp::socket(ios,ep.protocol()));
	asio::error_code ec;
	// one client this connect func will bind the active socket to a ip and port_num chosen by the os before try to connect with the remote server
	sock->connect(ep, ec);
	if (ec.value() != 0) {
		std::cout << "Failed to Connect to Acceptor. Error Code = " << ec.value() << ".Message: " << ec.message();
		return ec.value();
	}
	//Asyncwrite
	// which thread call those callbackfunc?
	// TODO why callback function not being called
	AsyncwriteToSocket(sock);
	ios.run();
	std::cout << "End here"<<std::endl;
	return 0;
}