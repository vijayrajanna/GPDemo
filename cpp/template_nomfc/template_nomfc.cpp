//////////////////////////////////////////////////////////////////////////////////////
// template_nomfc.cpp - Example template without using MFC
// Written in 2013 by Gazepoint www.gazept.com
//
// To the extent possible under law, the author(s) have dedicated all copyright 
// and related and neighboring rights to this software to the public domain worldwide. 
// This software is distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this 
// software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <iostream>
#include <string>

#include "Winsock2.h"

#pragma comment(lib, "ws2_32.lib")

int _tmain(int argc, _TCHAR* argv[])
{
  SOCKADDR_IN addr;
  WSADATA wsadata; 
  SOCKET ipsocket;
  u_long poll = TRUE;
	char rxbuffer[4096];
	unsigned int result;
	std::string str;

  if (WSAStartup(0x0101, &wsadata)) 
  {
    return 0;
  }
  
  ipsocket = socket(AF_INET, SOCK_STREAM, 0);
  if (ipsocket == INVALID_SOCKET) 
  {
    return 0;
  }
  
  if (ioctlsocket(ipsocket, FIONBIO, &poll) == SOCKET_ERROR) 
  {
    return 0;
  }
  
  addr.sin_family = AF_INET;
  addr.sin_port = htons(4242);
  addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

  connect(ipsocket, (struct sockaddr*)&addr, sizeof (addr));
  Sleep(250);

	str = "<SET ID=\"ENABLE_SEND_TIME\" STATE=\"1\" />\r\n";
  send(ipsocket, str.c_str(), str.length(), 0);
	str = "<SET ID=\"ENABLE_SEND_POG_FIX\" STATE=\"1\" />\r\n";
  send(ipsocket, str.c_str(), str.length(), 0);
	str = "<SET ID=\"ENABLE_SEND_DATA\" STATE=\"1\" />\r\n";
  send(ipsocket, str.c_str(), str.length(), 0);

  do
  {
		Sleep(50);
    
		result = recv(ipsocket, rxbuffer, 4096, 0);

    if (result == SOCKET_ERROR)
    {
    }
    else if (result > 0)
    {
			if (result < 4096)
      {
				rxbuffer[result]=0;
				printf("\n%s", rxbuffer);
      }      
		}
  }
  while (1);

  closesocket(ipsocket);

  WSACleanup();

	return 0;
}

