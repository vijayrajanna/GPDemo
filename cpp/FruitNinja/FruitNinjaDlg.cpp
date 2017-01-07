//////////////////////////////////////////////////////////////////////////////////////
// FruintNinjaDlg.cpp - Example class for using API to get gaze data and move the mouse cursor in various patterns
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
#include "FruitNinja.h"
#include "FruitNinjaDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Type of blade motions
enum {BLADE_NONE=0,BLADE_CIRCLE,BLADE_SLASH,BLADE_ROSE};

CFruitNinjaDlg::CFruitNinjaDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFruitNinjaDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	_width = 2;
	_height = 2;
}

void CFruitNinjaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFruitNinjaDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CROSS, &CFruitNinjaDlg::OnBnClickedButtonSlash)
	ON_BN_CLICKED(IDC_BUTTON_CIRCLE, &CFruitNinjaDlg::OnBnClickedButtonCircle)
	ON_BN_CLICKED(IDC_BUTTON_INF, &CFruitNinjaDlg::OnBnClickedButtonRose)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CFruitNinjaDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CFruitNinjaDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, &CFruitNinjaDlg::OnBnClickedButtonDisconnect)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CFruitNinjaDlg::OnBnClickedButtonExit)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CFruitNinjaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		
	
	// Default blade and connect to server
	_blade = BLADE_NONE;
	OnBnClickedButtonConnect();

	SetTimer(100,30,NULL);

	return TRUE; 
}

void CFruitNinjaDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CFruitNinjaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFruitNinjaDlg::OnBnClickedButtonSlash()
{
	_blade = BLADE_SLASH;
}

void CFruitNinjaDlg::OnBnClickedButtonCircle()
{
	_blade = BLADE_CIRCLE;
}

void CFruitNinjaDlg::OnBnClickedButtonRose()
{
	_blade = BLADE_ROSE;
}

// Lift left mouse button
void CFruitNinjaDlg::OnBnClickedButtonStop()
{
	_blade = BLADE_NONE;

	INPUT Input={0};
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1,&Input,sizeof(INPUT));
}

// Connect and also get screen size (so we can multiply the 0..1 gaze data to screen pixels (convert from percent to pixels)
void CFruitNinjaDlg::OnBnClickedButtonConnect()
{
	if (_client.is_connected() == FALSE)
	{
		_client.client_connect();
		_client.send_cmd("<GET ID=\"SCREEN_SIZE\" />");
	}
}

void CFruitNinjaDlg::OnBnClickedButtonDisconnect()
{
	_client.client_disconnect();
}

void CFruitNinjaDlg::OnBnClickedButtonExit()
{
  OnOK();
}

void CFruitNinjaDlg::OnTimer(UINT_PTR nIDEvent)
{
	static double angle = 0;
	static double t = 0;
  static int lost_count = 0;

	INPUT Input={0};
	Input.type = INPUT_MOUSE;

	if (_client.is_connected() == TRUE)
	{
		int count, valid, tmpi;
		float time;
		float x,y,tmp;
		string str;
		
		str = _client.get_rx_latest();

		if (str.length() > 0)
		{
			_timeout = GetTickCount();

			// API response to screen size query, extract data then start the data sending
			if (str.find ("<ACK ID=\"SCREEN_SIZE\"") != string::npos)
			{
				sscanf_s(str.c_str(), "<ACK ID=\"SCREEN_SIZE\" X=\"%i\" Y=\"%i\" WIDTH=\"%i\" HEIGHT=\"%i\" />", &_x,&_y,&_width,&_height);
				_client.send_cmd("<SET ID=\"ENABLE_SEND_COUNTER\" STATE=\"1\" />");
				_client.send_cmd("<SET ID=\"ENABLE_SEND_TIME\" STATE=\"1\" />");
				_client.send_cmd("<SET ID=\"ENABLE_SEND_POG_FIX\" STATE=\"1\" />");
				_client.send_cmd("<SET ID=\"ENABLE_SEND_CURSOR\" STATE=\"1\" />");
				_client.send_cmd("<SET ID=\"ENABLE_SEND_DATA\" STATE=\"1\" />");
			}
			// API data record, extract useful info
			else if (str.find ("<REC ") != string::npos)
			{
				sscanf_s(str.c_str(), "<REC CNT=\"%d\" TIME=\"%f\" FPOGX=\"%f\" FPOGY=\"%f\" FPOGS=\"%f\" FPOGD=\"%f\" FPOGID=\"%d\" FPOGV=\"%d\" CX=\"%f\" CY=\"%f\" CS=\"%d\" />", 
					&count, &time, &x, &y, &tmp, &tmp, &tmpi, &valid, &tmp, &tmp, &tmpi);

				// Scale to pixels and offset to handle multi-monitor possibility
				x = x * _width + _x;
				y = y * _height + _y;

				if (valid == TRUE)
				{
					// Computer different blade motions
					lost_count = 0;
					if (_blade == BLADE_SLASH)
					{
					}
					else if (_blade == BLADE_CIRCLE)
					{
						angle = angle + 0.6;
						if (angle > 2*3.141592) {angle = 0;}

						double radius = 80;
						x += radius*cos(angle);
						y += radius*sin(angle);
					}
					else if (_blade == BLADE_ROSE)
					{
						t = t + 0.4;

						double k = 2;
						double radius = 220;
						x += radius*cos(k*t)*sin(t);
						y += radius*cos(k*t)*cos(t);
					}

					// Send move command for mouse cursor
					if (_blade != BLADE_NONE)
					{
						double xscale = 65535 / ((float)_width - 1);
						double yscale = 65535 / ((float)_height - 1);

						// Convert from pixels to Input scale
						Input.mi.dx = (LONG)(x * xscale);
						Input.mi.dy = (LONG)(y * yscale);

						Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTDOWN;

						SendInput(1,&Input,sizeof(INPUT));  
					}
				}
				else
				{
					// Lift left mouse button if no good data for a while
					lost_count++;
					if (lost_count > 60)
					{
						lost_count = 0;
						Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
						SendInput(1,&Input,sizeof(INPUT));
					}
				}
			}
		}

		// If we didn't get data in a while, disconnect and reconnect
		if (GetTickCount() - _timeout > 5000)
		{
			_timeout = GetTickCount();
			OnBnClickedButtonDisconnect();
			OnBnClickedButtonConnect();
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}
