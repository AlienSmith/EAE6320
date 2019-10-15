#pragma once
#include <iostream>
#include <External/Asio/asio.hpp>
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
	const unsigned char MESSAGE_SIZE = 100;
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
int ReadSomeTCPServer() {
	//IPv4
//queue contain the pending connection request 
	const int BACKLOG_SIZE = 30;
	unsigned int port_num = 3333;
	asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(), port_num);
	asio::io_service ios;
	try {
		//Open acceptor
		asio::ip::tcp::acceptor acceptor(ios, ep.protocol());
		//Bind Acceptor
		acceptor.bind(ep);
		//StartListen
		acceptor.listen(BACKLOG_SIZE);
		//Create Active Socket
		asio::ip::tcp::socket sock(ios);
		std::cout << "Waiting for connection" << std::endl;
		acceptor.accept(sock);
		std::cout << "Connected" << std::endl;
		//has EOF problem
		ReadSomeAndPrint(sock);
		//ReadAndPrint(sock);
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