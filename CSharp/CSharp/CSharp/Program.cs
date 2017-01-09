//////////////////////////////////////////////////////////////////////////////////////
// Program.cs - Example OpenGaze API client in C#
// Written in 2013 by Gazepoint www.gazept.com
//
// To the extent possible under law, the author(s) have dedicated all copyright 
// and related and neighboring rights to this software to the public domain worldwide. 
// This software is distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this 
// software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//////////////////////////////////////////////////////////////////////////////////////
// Tested this program works as expected
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace CSharp
{
	class Program
	{
		const int ServerPort = 4242;
		const string ServerAddr = "127.0.0.1";

		static void Main(string[] args)
		{
			bool exit_state = false;
			int startindex, endindex;
			TcpClient gp3_client;
			NetworkStream data_feed;
			StreamWriter data_write;
			String incoming_data = "";

			ConsoleKeyInfo keybinput;

			// Try to create client object, return if no server found
			try
			{
				gp3_client = new TcpClient(ServerAddr, ServerPort);
			}
			catch (Exception e)
			{
				Console.WriteLine("Failed to connect with error: {0}", e);
				return;
			}

			// Load the read and write streams
			data_feed = gp3_client.GetStream();
			data_write = new StreamWriter(data_feed);

			// Setup the data records
			data_write.Write("<SET ID=\"ENABLE_SEND_TIME\" STATE=\"1\" />\r\n");
			data_write.Write("<SET ID=\"ENABLE_SEND_POG_FIX\" STATE=\"1\" />\r\n");
			data_write.Write("<SET ID=\"ENABLE_SEND_CURSOR\" STATE=\"1\" />\r\n");
			data_write.Write("<SET ID=\"ENABLE_SEND_DATA\" STATE=\"1\" />\r\n");

			// Flush the buffer out the socket
			data_write.Flush();

			do
			{
				int ch = data_feed.ReadByte();
				if (ch != -1)
				{
					incoming_data += (char)ch;
					
					// find string terminator ("\r\n") 
					if (incoming_data.IndexOf("\r\n") != -1)					
					{
						// only process DATA RECORDS, ie <REC .... />
						if (incoming_data.IndexOf("<REC") != -1)
						{
							double time_val;
							double fpogx;
							double fpogy;
							int fpog_valid;

							// Process incoming_data string to extract FPOGX, FPOGY, etc...
							startindex = incoming_data.IndexOf("TIME=\"") + "TIME=\"".Length;
							endindex = incoming_data.IndexOf("\"", startindex);
							time_val = Double.Parse(incoming_data.Substring(startindex, endindex - startindex));

							startindex = incoming_data.IndexOf("FPOGX=\"") + "FPOGX=\"".Length;
							endindex = incoming_data.IndexOf("\"", startindex);
							fpogx = Double.Parse(incoming_data.Substring(startindex, endindex - startindex));

							startindex = incoming_data.IndexOf("FPOGY=\"") + "FPOGY=\"".Length;
							endindex = incoming_data.IndexOf("\"", startindex);
							fpogy = Double.Parse(incoming_data.Substring(startindex, endindex - startindex));

							startindex = incoming_data.IndexOf("FPOGV=\"") + "FPOGV=\"".Length;
							endindex = incoming_data.IndexOf("\"", startindex);
							fpog_valid = Int32.Parse(incoming_data.Substring(startindex, endindex - startindex));

							Console.WriteLine("Raw data: {0}", incoming_data);
							Console.WriteLine("Processed data: Time {0}, Gaze ({1},{2}) Valid={3}", time_val, fpogx, fpogy, fpog_valid);
						}

						incoming_data = "";
					}
				}

				if (Console.KeyAvailable == true)
				{
					keybinput = Console.ReadKey(true);
					if (keybinput.Key != ConsoleKey.Q)
					{
						exit_state = true;
					}
				}
			}
			while (exit_state == false);

			data_write.Close();
			data_feed.Close();
			gp3_client.Close();
		}
	}
}