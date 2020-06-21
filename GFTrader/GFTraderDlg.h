
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
	BOOLEAN SetFiledValues();
	void OnBnClickedCancel();
	void OnBnClickedTrade();
	afx_msg boolean checkPointHasColor(CPoint pt, COLORREF clr);
	boolean getMYSQLConnection();
	CString getTradeString(CString& lst_stock, CString& lst_qty, CString& lst_price, CString success_flg);
	void clickMouse(CWnd* pwnd, CPoint pt);
	void pressKey(CWnd* pwnd, CHAR c);
	afx_msg void OnBnClickedBalance();
	afx_msg void OnBnClickedStoptrade();
	afx_msg void OnEnChangeStock();
	afx_msg void OnBnClickedTrade4();
	afx_msg void OnBnClickedBtnBuy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnSellDef();
	afx_msg void OnEnChangeStockId();
	afx_msg void OnEnKillfocusStockId();
	afx_msg void OnEnKillfocusTradeMny();
	afx_msg void OnBnClickedBtnSell();
	afx_msg void OnEnKillfocusTradeHands();
//	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnSetWinPrice();
	afx_msg void OnBnClickedBtnSetLostPrice();
};
