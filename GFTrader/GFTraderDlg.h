
// GFTraderDlg.h : header file
//

#pragma once
#pragma comment(lib, "libmysql.lib")


// CGFTraderDlg dialog
class CGFTraderDlg : public CDialogEx
{
// Construction
public:
	CGFTraderDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GFTRADER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	bool OnWindowEnum(int hwnd, int lparam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg
		void sendString(CWnd* pWnd, CString str);
	void clearField(CWnd* pWnd);
	void OnBnClickedCancel();
	afx_msg
		boolean checkTradeSuccess();
	void OnBnClickedTrade();
	boolean getMYSQLConnection();
	CString getTradeString(CString& lst_stock, CString& lst_qty, CString& lst_price, CString success_flg);
	void clickMouse(CWnd* pwnd, CPoint pt);
	void pressKey(CWnd* pwnd, CHAR c);
	afx_msg void OnBnClickedStop();
};
