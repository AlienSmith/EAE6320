#pragma once
#include <iostream>
#include <External/Asio/asio.hpp>
#include "ThreadGuard.h"
#include <string>
namespace Networking {
	namespace OO {
		class SyncUDPClient {
		public:
			SyncUDPClient() {}
		};
		class SyncTCPServer {
		public:
			static void Run_TEST() {
				SyncTCPServer server(3333);
				int i = 0;
				while (i < 3)
				{
					server.HandleClient();
				}
			}
			static std::string EMULATE_LONG_COMP_OP;
			static unsigned int BACK_BUF_SIZE;
			SyncTCPServer(unsigned int port_num) :
				m_ep(asio::ip::address_v4::any(), port_num),
				m_ios(asio::io_service()),
				m_acceptor(asio::ip::tcp::acceptor(m_ios, m_ep)) {
				m_acceptor.listen(BACK_BUF_SIZE);
			}
			bool InterpretRequest(std::string& request, int& o_time) {
				bool flag = true;
				for (unsigned int i = 0; i < EMULATE_LONG_COMP_OP.size(); i++) {
					if (request.c_str()[i] != EMULATE_LONG_COMP_OP.c_str()[i]) {
						flag = false;
						break;
					}
				}
				if (flag) {
					o_time = request.c_str()[EMULATE_LONG_COMP_OP.size()] - 48;
				}
				return flag;
			}
			void HandleClient() {
				//Create and unopen
				asio::ip::tcp::socket sock(m_ios);
				std::cout << "waiting for connection" << std::endl;
				m_acceptor.accept(sock);
				try {
					asio::streambuf request;
					asio::error_code ec;
					asio::read_until(sock, request, '\n', ec);
					if (ec == asio::error::eof || ec.value() == 0) {
						std::cout << "processing the request" << std::endl;
						std::istream input(&request);
						std::string temp_request;
						std::getline(input, temp_request);
						int delaytime = 0;
						if (InterpretRequest(temp_request, delaytime)) {
							std::cout << "Delay for" << delaytime << "seconds" << std::endl;
							std::this_thread::sleep_for(std::chrono::seconds(delaytime));
							std::string response = "OK\n";
							asio::write(sock, asio::buffer(response));
						}
						else {
							throw(asio::error::invalid_argument);
						}
					}
				}
				catch (asio::system_error& e) {
					std::cout << "Error" << e.code() << "seconds" << e.what() << std::endl;
				}
			}
		private:
			asio::ip::tcp::endpoint m_ep;
			asio::io_service m_ios;
			asio::ip::tcp::acceptor m_acceptor;
		};
		class SyncTCPClient {
		public:
			static void RUN_TEST() {
				const std::string host = "127.0.0.1";
				const unsigned int port_num = 3333;
				try {
					SyncTCPClient client(host, port_num);
					client.connect();
					std::cout << "Send request to the server..." << std::endl;
					std::string response = client.emulateLongComputationOP(10);
					std::cout << "Response recieved:" << response << std::endl;
					client.close();
				}
				catch (asio::system_error& e) {
					std::cout << "Error" << e.code() << ".Message" << e.what();
				}
			}
			SyncTCPClient(const std::string& host, unsigned int port_num) :
				m_ep(asio::ip::address::from_string(host), port_num),
				m_ios(asio::io_service()),
				m_sock(m_ios) {
				m_sock.open(m_ep.protocol());
			}
			std::string emulateLongComputationOP(unsigned int duration_sec) {
				std::string request = "EMULATE_LONG_COMP_OP" + std::to_string(duration_sec) + "\n";
				SendRequest(request);
				return recieveRespond();
			}
			void connect() {
				m_sock.connect(m_ep);
			}
			void close() {
				//Shut down both send and recieve and close
				m_sock.shutdown(asio::ip::tcp::socket::shutdown_both);
				m_sock.close();
			}
		private:
			void SendRequest(const std::string& request) {
				asio::write(m_sock, asio::buffer(request));
			}
			std::string recieveRespond() {
				std::string response;
				asio::streambuf buf;
				asio::error_code ec;
				asio::read_until(m_sock, buf, "\n",ec);
				if (ec == asio::error::eof || ec.value() == 0) {
					std::istream input(&buf);
					std::getline(input, response);
				}
				return response;
			}
			asio::ip::tcp::endpoint m_ep;
			asio::io_service m_ios;
			asio::ip::tcp::socket m_sock;
		};
	}
}