#include <iostream>
#include <WS2tcpip.h>
#include <thread>
#include <string>
#include <windows.h>
#include <vector>
#include <conio.h>

#pragma comment (lib, "ws2_32.lib") // This part will disable some warnings
using namespace std;
void Recieve();
void Send(std::string);
std::vector <char> Board =  { '1', '2', '3', '4', '5', '6', '7', '8', '9' };
void printGame();
std::string  IP = "10.164.1.140"; // Player 1 IP address 
bool flag = 0;
int wincon = -1;
int Boardvalue = 0;
int WinCondition();
void Message();
std::string GetRightInput();
void Message() {
	string Rpick;
	std::cout << "It is not your turn.. please wait for player 1 ..." << std::endl;

	do {
		if (flag == 1) {
			Rpick = GetRightInput();

			if (flag == 1) {
				Boardvalue = static_cast<int>(Rpick[0] - 49);
				Board[Boardvalue] = 'O';
				printGame();
				wincon = WinCondition();
				if (wincon == 0 || wincon == 1) {
					std::cout << "Player  >>>" << wincon + 1 << " <<< win ";

				}
				Send(Rpick);
				flag = !flag;
			}
			else {
				std::cout << "Opps!!! It's not your turn ... wait for player 1 to play ... !";
			}
		}
		} while (Rpick != "q");
	
}

std::string GetRightInput()
{
	std::string pick;
	while (true) 
	{
		std::cout << "whats your pick >>> ";
		std::cin >> pick;
		

		if (std::cin.fail()) 
		{
			std::cin.clear(); 
			std::cin.ignore(32767, '\n'); 
		}
		else // else our extraction succeeded
		{
			std::cin.ignore(32767, '\n'); // clear (up to 32767) characters out of the buffer until a '\n' character is removed
			return pick; // so return the value we extracted
		}
	}
}


void printGame() {
	system("CLS");
	for (int a = { 0 }; a < 3; a++)
	{
		std::cout << "  | ";
		for (int b = { 0 }; b < 3; b++)
		{
			std::cout << Board[b + (a * 3)] << " | ";
		}
		std::cout << "\n";
	}
}

/// Game win condition 

int playing = -1;
int WinCondition()
{
	if (((Board[0] == 'X') && (Board[1] == 'X') && (Board[2] == 'X')) || ((Board[0] == 'O') && (Board[1] == 'O') && (Board[2] == 'O')))
	{
		std::cout << "Player " << Board[0] << " is the winner on top row!!\n";
		playing = 0;
		return 0;
	}
	// cheking vertical line 2
	if (((Board[3] == 'X') && (Board[4] == 'X') && (Board[5] == 'X')) || ((Board[3] == 'O') && (Board[4] == 'O') && (Board[5] == 'O')))
	{
		std::cout << "Player " << Board[3] << " is the winner on second row!!\n";
		playing = 0;
		return 0;
	}
	// cheking vertical line 3
	if (((Board[6] == 'X') && (Board[7] == 'X') && (Board[8] == 'X')) || ((Board[6] == 'O') && (Board[7] == 'O') && (Board[8] == 'O')))
	{
		std::cout << "Player " << Board[6] << " is the winner on third row!!\n";
		playing = 0;
		return 0;
	}


	// cheking horizontal line 1
	if (((Board[0] == 'X') && (Board[3] == 'X') && (Board[6] == 'X')) || ((Board[0] == 'O') && (Board[3] == 'O') && (Board[6] == 'O')))
	{
		std::cout << "Player " << Board[0] << " is the winner on first column !!\n";
		playing = 0;
		return 0;
	}
	// cheking horizontal line 2
	if (((Board[1] == 'X') && (Board[4] == 'X') && (Board[7] == 'X')) || ((Board[1] == 'O') && (Board[4] == 'O') && (Board[7] == 'O')))
	{
		std::cout << "Player " << Board[1] << " is the winner on second column !!\n";
		playing = 0;
		return 0;
	}
	// cheking horizontal line 3
	if (((Board[2] == 'X') && (Board[5] == 'X') && (Board[8] == 'X')) || ((Board[2] == 'O') && (Board[5] == 'O') && (Board[8] == 'O')))
	{
		std::cout << "Player " << Board[2] << " is the winner on second column !!\n";
		playing = 0;
		return 0;
	}
	// cheking diagonal line 1 from top left to bottom right
	if (((Board[0] == 'X') && (Board[4] == 'X') && (Board[8] == 'X')) || ((Board[0] == 'O') && (Board[4] == 'O') && (Board[8] == 'O')))
	{
		std::cout << "Player " << Board[0] << " is the winner on the diagonal row1 !!\n";
		playing = 0;
		return 0;

	}
	// cheking diagonal line 2 from top right to bottom left
	if (((Board[2] == 'X') && (Board[4] == 'X') && (Board[6] == 'X')) || ((Board[2] == 'O') && (Board[4] == 'O') && (Board[6] == 2)))
	{
		std::cout << "Player " << Board[2] << " is the winner on the diagonal row2!!\n";
		playing = 0;
		return 0;

	}
	return -1;
}


////////////////////////////////////////////// Server Code


void Recieve() {
	
	WSADATA data;
	WORD version = MAKEWORD(3, 2); // we just make a version here, can be any number 

	int wsOk = WSAStartup(version, &data); // starting the socket 
	if (wsOk != 0) // if socket is not started , then quit 
	{
		cout << "Can't start Winsock! " << wsOk;
		return;
	}

	SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);  // Creating the socket // Sock_DGRAM is UDP socket
	sockaddr_in serverHint;
	serverHint.sin_addr.S_un.S_addr = ADDR_ANY; // any IP would be okay 
	serverHint.sin_family = AF_INET; // IPv4 addresses 
	serverHint.sin_port = htons(54002); // selecting a port to open for the Assigned ip 
	// Convert from little to big endian // study more about Little and big endian


	if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR) // biding to the socket 
	{
		cout << "Can't bind socket! " << WSAGetLastError() << endl;
		return;
	}


	sockaddr_in client; // instance object of the scokaddr // hold address information 
	int clientLength = sizeof(client); // The size of the client information

	char buf[1024]; // buffer size 

	// Enter a loop
	while (true)
	{
		ZeroMemory(&client, clientLength); 
		ZeroMemory(buf, 1024); // Clear the buffer
		// Waiting for a message
		int Msg = recvfrom(in, buf, 1024, 0, (sockaddr*)&client, &clientLength);
		if (Msg == SOCKET_ERROR)
		{
			perror("Error receiving from client ");
			WSAGetLastError();
			continue;
		}
		char clientIp[256]; 
		ZeroMemory(clientIp, 256); 

		// Converts from byte array to chars
		inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

		// Display the message / who sent it
		//cout << "Message recv from " << clientIp << " : " << buf << endl;
		std::cout << buf;
		Boardvalue = static_cast<int>(buf[0] - 49);
		std::cout << "-------------" << Boardvalue << std::endl;
		Board[Boardvalue] = 'X';
		printGame();
		WinCondition();

		flag =!flag;
		
	}
	closesocket(in);

	WSACleanup(); // shutdown and cleanup the socket
}

void Send(std::string s) {
	WSADATA data;

	// This server is going to use WinSock version 2
	WORD version = MAKEWORD(2, 2);
	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0)
	{
		cout << "Can't start Winsock! " << wsOk;
		return;
	}

	// CONNECT TO THE SERVER
	sockaddr_in server;
	server.sin_family = AF_INET; // AF_INET = IPv4 addresses
	server.sin_port = htons(54001); // Assigning the Port // htons = little to big endian
	std::string *ipAdd = const_cast<std::string*>(&IP);
	inet_pton(AF_INET, ipAdd->c_str(), &server.sin_addr); // Convert from string to byte array

	// Creating the Socket 
	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);
	
	// I am using a thread so I don't need this loop but if you want to terminate the connection you can have a exit condition here
	//do {

	int sendOk = sendto(out, s.c_str(), s.size() + 1, 0, (sockaddr*)&server, sizeof(server));

	if (sendOk == SOCKET_ERROR)
	{
		cout << "That didn't work! " << WSAGetLastError() << endl;
	}
	//	} while (s != "q");
		
	closesocket(out);

	WSACleanup();

}
void Print(string name) {

	std::cout << "        \t \t\t            Player 1  === > " << name << "\n";

}
int main()  
{
	string name;
//	std::cout << "Enter  your Name :";
	//cin >> name;
	//Print(name);
	std::cout << "Enter the player 1 IP address :";
	std::cin >> IP;
	std::string* ipAdd = const_cast<std::string*>(&IP);
	std::cout<< ipAdd->c_str();
	printGame();
	// creating threads to run send and recieve functions simultaneously 
	thread T1(Message);
	thread T2(Recieve);
	// joining the threads 
	T1.join();
	T2.join();
	return 0;
}