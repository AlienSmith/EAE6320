// Network.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Chapter2.h"
int TCPserver() {
	//IPv4
	//queue contain the pending connection request 
	const int BACKLOG_SIZE = 30;
	unsigned int port_num = 3333;
	asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(), port_num);
	asio::io_service ios;
	try {
		//Open acceptor
		asio::ip::tcp::acceptor acceptor(ios,ep.protocol());
		//Bind Acceptor
		acceptor.bind(ep);
		//StartListen
		acceptor.listen(BACKLOG_SIZE);
		//Create Active Socket
		asio::ip::tcp::socket sock(ios);
		std::cout << "Waiting for connection" << std::endl;
		acceptor.accept(sock);
		std::cout << "Connected" << std::endl;
	}catch(asio::system_error & e){
		std::cout << "Error occured! Error code = " << e.code() << ".Message" << e.what();
	}
	return 0;
}
int DNStcpClient() {
	std::string host = "localhost";
	std::string port_num = "3333";
	asio::ip::tcp::resolver::query resolve_query(host, port_num, asio::ip::tcp::resolver::query::numeric_service);
	asio::io_service ios;
	asio::ip::tcp::resolver resolver(ios);
	asio::error_code ec;
	asio::ip::tcp::resolver::iterator it(resolver.resolve(resolve_query,ec));
	if (ec.value() != 0) {
		std::cout << "Failed to resolve dns. Error Code = " << ec.value() << ".Message: " << ec.message();
		return ec.value();
	}
	asio::ip::tcp::socket sock(ios);
	asio::connect(sock, it,ec);
	if (ec.value() != 0) {
		std::cout << "Failed to connect to server. Error Code = " << ec.value() << ".Message: " << ec.message();
		return ec.value();
	}
	return ec.value();
}
int TCPClient() {
	//tcp v4 client
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
	return ec.value();
}
int CreateAndBindAcceptor() {
	// tcp v4 server
	unsigned short port_num = 3333;
	std::string host = "127.0.0.1";
	asio::ip::tcp::endpoint ep(asio::ip::address::from_string(host), port_num);
	asio::io_service ios;
	asio::ip::tcp::acceptor acceptor(ios, ep.protocol());
	asio::error_code ec;

	acceptor.bind(ep, ec);
	if (ec.value() != 0) {
		std::cout << "Failed to Bind Acceptor. Error Code = " << ec.value() << ".Message: " << ec.message();
		return ec.value();
	}
	return ec.value();
}
int CreateAndBindSocket() {
	//udp v4 server
	unsigned short port_num = 3333;
	asio::ip::udp::endpoint ep(asio::ip::address_v4::any(), port_num);
	asio::io_service ios;
	asio::ip::udp::socket sock(ios, ep.protocol());
	asio::error_code ec;
	sock.bind(ep, ec);
	if (ec.value() != 0) {
		std::cout << "Failed to Bind Socket. Error Code = " << ec.value() << ".Message: " << ec.message();
		return ec.value();
	}
	return ec.value();
}
int ResolveDNStoIP() {
	//TCP v6
	std::string host = "samplehost.com";
	std::string port_num = "3333";

	asio::io_service ios;
	asio::ip::tcp::resolver::query resolve_qury(host, port_num, asio::ip::tcp::resolver::query::numeric_service);
	asio::ip::tcp::resolver resolver(ios);
	asio::error_code ec;
	asio::ip::tcp::resolver::iterator it = resolver.resolve(resolve_qury, ec);
	if (ec.value() != 0) {
		std::cout << "Failed to Resolve the DNS name. Error Code = " << ec.value() << ".Message: " << ec.message();
		return ec.value();
	}
	return ec.value();
}
int ResolveDNStoIPUDP() {
	std::string host = "samplehost.com";
	std::string port_num = "3333";

	asio::io_service ios;
	asio::ip::udp::resolver::query resolve_qury(host, port_num, asio::ip::udp::resolver::query::numeric_service);

	asio::ip::udp::resolver resolver(ios);
	asio::error_code ec;

	asio::ip::udp::resolver::iterator it = resolver.resolve(resolve_qury, ec);
	if (ec.value() != 0) {
		std::cout << "Failed to Resolve the DNS name. Error Code = " << ec.value() << ".Message: " << ec.message();
		return ec.value();
	}
	return ec.value();
}
int CreatePassiveSocket() {
	//only tcp version exist
	asio::io_service ios;
	//v4 vs v6
	asio::ip::tcp protocol = asio::ip::tcp::v4();
	asio::ip::tcp::acceptor sock(ios);

	asio::error_code ec;
	sock.open(protocol, ec);
	if (ec.value() != 0) {
		std::cout << "Failed to Create Passive socket. Error Code = " << ec.value() << ".Message: " << ec.message();
		return ec.value();
	}
	return ec.value();
}
int CreateActiveSocket() {
	//Create service class
	asio::io_service ios;
	//transprot protocal instance (TCP IPv4)
	asio::ip::tcp protocol = asio::ip::tcp::v4();
	// Create active TCP socket
	asio::ip::tcp::socket sock(ios);
	//Openning the socket;
	asio::error_code ec;
	sock.open(protocol, ec);
	if (ec.value() != 0) {
		std::cout << "Failed to Create real socket. Error Code = " << ec.value() << ".Message: " << ec.message();
		return ec.value();
	}
	return ec.value();

}
int CreateActiveUDPSocket() {
	asio::io_service ios;
	//IPv4 or IPv6 no default constructor provided
	asio::ip::udp protocal = asio::ip::udp::v6();
	asio::ip::udp::socket sock(ios);
	asio::error_code ec;
	sock.open(protocal, ec);
	if (ec.value() != 0) {
		std::cout << "Failed to Create real socket. Error Code = " << ec.value() << ".Message: " << ec.message();
		return ec.value();
	}
	return ec.value();
}
int CreateEndPointOnClient() {
	//Create Endpoint on client
	std::string raw_ip_address = "127.0.0.1";
	unsigned short port_num = 3333;
	//store the return of error code
	asio::error_code ec;
	// make address instance 
	asio::ip::address ip_address = asio::ip::address::from_string(raw_ip_address, ec);

	if (ec.value() != 0) {
		std::cout << "Failed to prase the IP address. Error Code = " << ec.value() << ".Message: " << ec.message();
		return ec.value();
	}

	// specify endpoints on server we want to communicate with
	asio::ip::tcp::endpoint ep(ip_address, port_num);
	//asio::ip::udp::endpoint ep(ip_address, port_num);

	// the endpoint is ready to go.
	return ec.value();
}
int CreateEndPointOnServer() {
	unsigned short port_num = 3333;
	//create ipv6 ip address instance represents all ip obtained by the host this will not fail.
	//asio::ip::address do not have any() func 
	asio::ip::address ip_address = asio::ip::address_v6::any();
	//create endpoint
	asio::ip::tcp::endpoint ep(ip_address, port_num);
	//0 for success following asio::error_code
	return 0;
}
int main()
{
	//CreateEndPointOnClient();
	//CreateActiveSocket();
	//CreateActiveUDPSocket();
	//CreatePassiveSocket();
	//ResolveDNStoIP();
	//ResolveDNStoIPUDP();
	//CreateAndBindAcceptor();
	//CreateAndBindSocket();
	//TCPClient();
	//CreateAndBindAcceptor();
	//TCPserver();
	//TCPClient();
	//writesomeToTCPsocket();
	//ReadSomeTCPServer();
	//ReadTCPServer();
	//ReadSomeAsyncTCPPrintServer();
	Cancel_Async_Call();
	//writeToTCPsocketAsync();
	//multithreadwritetoTCP();
	//StreamBuf();
	// the endpoint is ready to go.
    std::cout << "Hello World!\n";
	char a = std::getchar();
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
