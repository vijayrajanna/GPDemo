//////////////////////////////////////////////////////////////////////////////////////
// apiclient.cpp - Example class for communication over the Open Gaze API
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
#include <conio.h>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>

#include <string>
#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <deque>

#include "..\include\GPClient.h"

void print_inputs()
{
	cout << "\n'c' to connect";
	cout << "\n'd' to disconnect";
	cout << "\n't' to setup tx data";
	cout << "\n'q' to query vars";
	cout << "\n'x' to exit";
	cout << "\nInput?: ";
}

int _tmain(int argc, _TCHAR* argv[])
{		
	char ch;
	deque<string> data_set;
	vector<string> cmd[2];
	GPClient client;

	cmd[0].push_back("ENABLE_SEND_DATA");
	cmd[0].push_back("ENABLE_SEND_COUNTER");
	cmd[0].push_back("ENABLE_SEND_TIME");
	cmd[0].push_back("ENABLE_SEND_TIME_TICK");    
	cmd[0].push_back("ENABLE_SEND_POG_FIX");
	cmd[0].push_back("ENABLE_SEND_POG_LEFT");
	cmd[0].push_back("ENABLE_SEND_POG_RIGHT");
	cmd[0].push_back("ENABLE_SEND_POG_BEST");
	cmd[0].push_back("ENABLE_SEND_PUPIL_LEFT");
	cmd[0].push_back("ENABLE_SEND_PUPIL_RIGHT");
	cmd[0].push_back("ENABLE_SEND_EYE_LEFT");
	cmd[0].push_back("ENABLE_SEND_EYE_RIGHT");
	cmd[0].push_back("ENABLE_SEND_CURSOR");
	cmd[0].push_back("ENABLE_SEND_BLINK");
	cmd[0].push_back("ENABLE_SEND_USER_DATA");

	cmd[1].push_back("CALIBRATE_START");
	cmd[1].push_back("CALIBRATE_SHOW");
	cmd[1].push_back("CALIBRATE_TIMEOUT");
	cmd[1].push_back("CALIBRATE_DELAY");
	cmd[1].push_back("CALIBRATE_RESULT_SUMMARY");
	cmd[1].push_back("CALIBRATE_CLEAR");
	cmd[1].push_back("CALIBRATE_RESET");
	cmd[1].push_back("CALIBRATE_ADDPOINT");
	cmd[1].push_back("USER_DATA");
	cmd[1].push_back("TRACKER_DISPLAY");    
	cmd[1].push_back("TIME_TICK_FREQUENCY");    
	cmd[1].push_back("SCREEN_SIZE");
	cmd[1].push_back("CAMERA_SIZE");
	cmd[1].push_back("PRODUCT_ID");
	cmd[1].push_back("SERIAL_ID");
	cmd[1].push_back("COMPANY_ID");
	cmd[1].push_back("API_ID");
	cmd[1].push_back("TRACKER_ID");

	client.client_connect();

	print_inputs();

	do
	{
		ch = NULL;

		// check for quick key
		if (_kbhit() != 0) { ch = _getch(); }

		// 'enter' so show possible inputs
		if (ch == 13)
		{
			print_inputs();
		}

		// connector or disconnect
		if (ch == 'c')
		{	
			cout << "\nAttempt connect";
			client.client_connect();
			cout << "\nInput?: ";
		}
		else if (ch == 'd')
		{
			cout << "\nAttempt disconnect";
			client.client_disconnect();
			cout << "\nInput?: ";
		}

		// configure via API
		if (ch == 't' || ch == 'q')
		{
			unsigned int val, sel;
			int cmd_type = 0;
			char setorget;

			string setorgetstr;
			ostringstream data;
			ostringstream str_strm;

			cout << "\n's'et or 'g'et?: ";
			setorget = _getch();

			if (ch == 'q') { cmd_type = 1; }

			if (setorget == 'g' || setorget == 's')
			{
				for (unsigned int i = 0; i < cmd[cmd_type].size(); i++)
				{
					cout << "\n" << i << " " << cmd[cmd_type].at(i);
				}
				cout << "\nInput?: ";
				cin >> sel;

				if (setorget == 's')
				{
					setorgetstr = "SET";
					if (cmd[cmd_type].at(sel) == "CALIBRATE_TIMEOUT" || cmd[cmd_type].at(sel) == "CALIBRATE_DELAY")
					{
						float timeval;
						cout << "\nVALUE? (float): ";
						cin >> timeval;
						data << "VALUE=\"" << timeval << "\" ";
					}
					else if (cmd[cmd_type].at(sel) == "SCREEN_SIZE")
					{
						int x,y,w,h;
						cout << "\nX? (int): ";
						cin >> x;
						data << "X=\"" << x << "\" ";
						cout << "\nY? (int): ";
						cin >> y;
						data << "Y=\"" << y << "\" ";
						cout << "\nWIDTH? (int): ";
						cin >> w;
						data << "WIDTH=\"" << w << "\" ";
						cout << "\nHEIGHT? (int): ";
						cin >> h;
						data << "HEIGHT=\"" << h << "\" ";
					}					
					else if (cmd[cmd_type].at(sel) == "CALIBRATE_CLEAR")
					{
					}
					else if (cmd[cmd_type].at(sel) == "CALIBRATE_RESET")
					{
					}
					else if (cmd[cmd_type].at(sel) == "CALIBRATE_ADDPOINT")
					{
						float x,y;
						cout << "\nX? (float): ";
						cin >> x;
						data << "X=\"" << x << "\" ";
						cout << "\nY? (float): ";
						cin >> y;
						data << "Y=\"" << y << "\" ";
					}
					else if (cmd[cmd_type].at(sel) == "USER_DATA")
					{
						string str;
						cout << "\nVALUE? (string): ";
						cin >> str;
						data << "VALUE=\"" << str << "\" ";
					}
					else if (cmd[cmd_type].at(sel) == "TRACKER_ID")
					{
						string active;
            string search;
						cout << "\nACTIVE_ID?: ";
						cin >> active;
						cout << "\nSEARCH? (NONE, SIMPLE, CURSOR, GAZE): ";
						cin >> search;
						data << "ACTIVE_ID=\"" << active << "\" SEARCH=\"" << search << "\" ";
					}
					else
					{
						cout << "\nSTATE?: ";
						cin >> val;
						data << "STATE=\"" << val << "\" ";
					}
				}
				else
				{
					setorgetstr = "GET";
				}

				// create XML string and send
				if (sel >= 0 && sel < cmd[cmd_type].size())
				{
					str_strm << "<" << setorgetstr << " ID=\"" << cmd[cmd_type].at(sel) << "\" " << data.str() << "/>";

					cout << str_strm.str().c_str() << "\n";
					client.send_cmd(str_strm.str());
				}
			}      
		}


		// display response on screen
		//////////////////////////////////////////////////////////////////////////////////////
		// Use this section if you want to see EVERY record (will lag, can't display at 60Hz!)
		/*
		client.get_rx(data_set);
		for (int i = 0; i < data_set.size(); i++)
		{
			cout << data_set.at(i).c_str() << "\n";
		}*/

		//////////////////////////////////////////////////////////////////////////////////////
		// Use this section if you just want the latest data record
		string rxstr = client.get_rx_latest();
		if (rxstr != "")
		{
			cout << rxstr.c_str() << "\n";
		}

		Sleep (50);
	}
	while (ch != 'x');

	return 0;
}


