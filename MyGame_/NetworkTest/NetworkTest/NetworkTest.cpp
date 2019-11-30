// NetworkTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
//#include <Engine/Network/Client.h>
#include <Engine/Network/Server.h>
//#include <Engine/Network/IO_Struct.h>
int main()
{
	std::string local_host = "127.0.0.1";
	std::string port_num = "3333";
	//server();
	//Server
	{
		Network::ServerLogic server_logic;
		Network::TCP::Server server;
		server.SetServerLogic(&server_logic);
		Network::network_error_code error_code;
		if (!server.Run(port_num, error_code)) {
			printf(error_code.code.c_str());
		}
	}
	//Client
	/*{
		Network::ClientLogic clientlogic;
		std::shared_ptr<Network::TCP::Client> client = Network::TCP::Client::Create_and_Run(local_host, port_num);
		while (true) {
			clientlogic.GenerateRandomInputs();
			client->SubmitInputStruct(*clientlogic.GetInputStructure());
			*(clientlogic.GetUpdateStructure()) = client->GetUpdateStruct();
			clientlogic.Draw();
			Sleep(1000);
		}
	}*/
	{
		/*Network::ClientLogic clientlogic;
		Network::TCP::Client client;
		client.SetLogicClas(&clientlogic);
		Network::network_error_code error_code;
		if (!client.run(local_host, port_num, error_code)) {
			printf("Error \n");
			printf(error_code.code.c_str());
		}*/
	}
	std::cout << "End\n";
	int a;
	std::cin >> a;
	return a;
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
