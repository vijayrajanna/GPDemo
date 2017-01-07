#pragma once

#include "..\include\GPClient.h"

// CFruitNinjaDlg dialog
class CFruitNinjaDlg : public CDialogEx
{
// Construction
public:
	CFruitNinjaDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FRUITNINJA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	int _x,_y,_width,_height;
	double _timeout;
	int _blade;
	GPClient _client;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSlash();
	afx_msg void OnBnClickedButtonCircle();
	afx_msg void OnBnClickedButtonRose();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonDisconnect();
	afx_msg void OnBnClickedButtonExit();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
