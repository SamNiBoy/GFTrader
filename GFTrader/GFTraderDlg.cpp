
// GFTraderDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "GFTrader.h"
#include "GFTraderDlg.h"
#include "afxdialogex.h"
#include "winsock.h"    
#include "mysql.h"   
#include <vector>
#include <string>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CPoint TRADE_MENU(317, 14);
CPoint BUY_MENU(42, 70);
CPoint SELL_MENU(48, 105);
CPoint STOCK_FIELD(271, 114);
CPoint PRICE_FIELD(340, 146);
CPoint PRICE_OPT5(298, 278);
CPoint QTY_FIELD(210, 243);
CPoint BUY_SELL_BTN(268, 340);
CPoint YES_CONFIRM_PNT(1047, 634);
COLORREF YES_CONFIRM_BLUE_COLOR = RGB(63, 151, 240);
CPoint NO_CONFIRM(1090, 633);
CPoint MSG_CONFIRM(959, 583);

CPoint OK_BTN_BLUE_COLOR_PNT(979, 582);
COLORREF OK_BTN_BLUE_COLOR = RGB(63, 151, 240);


int ACTION_DELAY_200 = 200;
int ACTION_DELAY_300 = 300;
int ACTION_DELAY_500 = 500;
int ACTION_DELAY_1000 = 1000;
// CAboutDlg dialog used for App About

MYSQL m_sqlCon;
MYSQL_RES* m_res;         //查询的返回结果集
MYSQL_ROW m_row;          //获取每一条记录
boolean mysql_init_flg = false;

boolean stop_trade = true;

HACCEL  hAccTable;


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStop();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	
END_MESSAGE_MAP()


// CGFTraderDlg dialog



CGFTraderDlg::CGFTraderDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GFTRADER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGFTraderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGFTraderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CGFTraderDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDTRADE, &CGFTraderDlg::OnBnClickedTrade)
	ON_BN_CLICKED(IDBALANCE, &CGFTraderDlg::OnBnClickedBalance)
	ON_BN_CLICKED(IDSTOPTRADE, &CGFTraderDlg::OnBnClickedStoptrade)
	
	ON_BN_CLICKED(IDC_BTN_BUY, &CGFTraderDlg::OnBnClickedBtnBuy)
	ON_EN_KILLFOCUS(IDC_STOCK_ID, &CGFTraderDlg::OnEnKillfocusStockId)
	ON_EN_KILLFOCUS(IDC_TRADE_MNY, &CGFTraderDlg::OnEnKillfocusTradeMny)
	ON_BN_CLICKED(IDC_BTN_SELL, &CGFTraderDlg::OnBnClickedBtnSell)
	ON_EN_KILLFOCUS(IDC_TRADE_HANDS, &CGFTraderDlg::OnEnKillfocusTradeHands)
//	ON_WM_KEYUP()
//ON_WM_KEYDOWN()
ON_WM_CLOSE()
END_MESSAGE_MAP()


// CGFTraderDlg message handlers

BOOL CGFTraderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	hAccTable = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACC_BTN_BUY));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGFTraderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGFTraderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

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



		
		//int a = ::SendMessage(*pWnd, SW_HIDE, NULL, NULL);
		//        ::SendMessage(*pWnd, SW_SHOW, NULL, NULL);
		//CString s;
		//char buf[100];
		//sprintf_s(buf, "abc%d", a);
		//AfxTrace((CString)buf);
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGFTraderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HWND GF = NULL;
CWnd* GF_Wnd = NULL;


BOOL CALLBACK EnumChildProc(HWND hwndChild, LPARAM lParam)
{
	int idChild;
	idChild = GetWindowLong(hwndChild, GWL_ID);

	TCHAR buf[512];
	::GetClassName(hwndChild, (LPWSTR)buf, 512);
	CString str = buf;
	str.TrimRight();

	AfxTrace((CString)"\nChild Window ClassName:[" + str + "]");

	EnumChildWindows(hwndChild, EnumChildProc, NULL);

	//ShowWindow(hwndChild, SW_SHOW);

	return TRUE;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	TCHAR lpString[512];
	TCHAR caption[512];

	memset(lpString, 0, 512);
	memset(caption, 0, 512);
	GetClassName((HWND)hwnd, (LPWSTR)lpString, 512);
	::GetWindowText(hwnd, caption, 512);

	CString msg;
	msg.Format(_T("HWND:[%d], Caption:[%s], Class:[%s]\n"), hwnd, caption, lpString);
	//AfxTrace(msg);
	//if (strcmp("Chrome_WidgetWin_1", (char*)lpString) == 0)
	if ((CString)"广发操盘手" == caption)
	{
		CString msg;
		msg.Format(_T("HWND:%d, Caption:%s, Class:%s\n"), hwnd, caption, lpString);
		//AfxMessageBox(msg);
		GF = hwnd;

		//EnumChildWindows(GF, EnumChildProc, NULL);

		/*HWND hWndChild = ::GetWindow(GF, GW_CHILD);

		while (hWndChild)
		{
			TCHAR buf[512];
			::GetClassName(hWndChild, (LPWSTR)buf, 512);//得到枚举的控件是什么类型的，
			CString str = buf;
			str.TrimRight();
			//AfxMessageBox(str);
			CWnd* pedit;

			if (str == "ComboBoxEx32")
			{
				HWND tt = ::GetWindow(hWndChild, GW_CHILD);
				HWND yy = ::GetWindow(tt, GW_CHILD);
				pedit = CWnd::FromHandle(yy);
				pedit->SendMessage(WM_SETTEXT, 0, (LPARAM)"ABC");
			}
			if (str == "Button")      //枚举到按钮，有保存和取消按钮
			{
				CWnd* pwnd = CWnd::FromHandle(hWndChild);
				pwnd->SendMessage(WM_GETTEXT, sizeof(buf) / sizeof(TCHAR), (LPARAM)(void*)buf);
				//发消息得到按钮的标题
				CString str = buf;
				str.TrimRight();
			    //pedit->SendMessage(WM_SETTEXT, 0, (LPARAM)"DEF");
			}
			else
			{
				HWND tt = ::GetWindow(hWndChild, GW_CHILD);
				while (tt != NULL)
				{
					HWND yy = ::GetWindow(tt, GW_CHILD);
					::GetClassName(yy, (LPWSTR)buf, 512);
					AfxTrace((CString)"\nControl type:" + str);
					tt = yy;
				}

			}
			AfxTrace((CString)"\nControl type:" + str);
			hWndChild = ::GetWindow(hWndChild, GW_HWNDNEXT);
		}*/
		return false;
	}
	return true;
}




CString lst_stock = (CString)"";
CString lst_qty = (CString)"";
CString lst_price = (CString)"";
CString success_flg = (CString)"N";
CString lst_id = (CString)"";

DWORD WINAPI ThreadTrade(LPVOID pParam)
{
	// TODO: Add your control notification handler code here
//CDialogEx::OnOK();

	GF = NULL;

    EnumWindows(EnumWindowsProc, 0);

	//CWnd* pWnd = CWnd::FromHandle(GF);

	if (GF)
	{
		GF_Wnd = CWnd::FromHandle(GF);

		while (!stop_trade)
		{
			CGFTraderDlg *dlg = (CGFTraderDlg*)(AfxGetApp()->m_pMainWnd);
			CString ts = dlg->getTradeString(lst_stock, lst_qty, lst_price, success_flg);

			if (ts.GetLength() > 0)
			{
				//GF_Wnd->ShowWindow(SW_HIDE);
				//::Sleep(200);
				//maximize window.

			Again:

				GF_Wnd->ShowWindow(SW_MAXIMIZE);
				::Sleep(ACTION_DELAY_200);

				//open trading panel.
				dlg->clickMouse(GF_Wnd, TRADE_MENU);
				
				::Sleep(ACTION_DELAY_200);

				CString action = ts.Left(1);
				CString code = ts.Mid(1, 6);
				CString qty = ts.Right(ts.GetLength() - 7);

				AfxTrace((CString)"Action:" + action);
				AfxTrace((CString)"code:" + code);
				AfxTrace((CString)"qty:" + qty);

				if (action == "B")
				{
					dlg->clickMouse(GF_Wnd, BUY_MENU);
					::Sleep(ACTION_DELAY_300);

					dlg->clickMouse(GF_Wnd, STOCK_FIELD);
					dlg->clearField(GF_Wnd);
					::Sleep(ACTION_DELAY_200);
					dlg->sendString(GF_Wnd, code);
					::Sleep(ACTION_DELAY_1000);

					dlg->clickMouse(GF_Wnd, PRICE_FIELD);
					::Sleep(ACTION_DELAY_200);
					dlg->clickMouse(GF_Wnd, PRICE_OPT5);
					::Sleep(ACTION_DELAY_200);

					dlg->clickMouse(GF_Wnd, QTY_FIELD);
					dlg->clearField(GF_Wnd);
					::Sleep(ACTION_DELAY_200);
					dlg->sendString(GF_Wnd, qty);
					::Sleep(ACTION_DELAY_200);

					dlg->clickMouse(GF_Wnd, BUY_SELL_BTN);
					::Sleep(ACTION_DELAY_500);

					if (dlg->checkPointHasColor(YES_CONFIRM_PNT, YES_CONFIRM_BLUE_COLOR))
					{
						dlg->clickMouse(GF_Wnd, YES_CONFIRM_PNT);
						::Sleep(ACTION_DELAY_300);

						if (dlg->checkPointHasColor(OK_BTN_BLUE_COLOR_PNT, OK_BTN_BLUE_COLOR))
						{
							success_flg = "N";
							dlg->clickMouse(GF_Wnd, OK_BTN_BLUE_COLOR_PNT);
						}
						else
						{
							success_flg = "Y";
						}
					}
					else if (dlg->checkPointHasColor(OK_BTN_BLUE_COLOR_PNT, OK_BTN_BLUE_COLOR))
					{
							success_flg = "N";
							dlg->clickMouse(GF_Wnd, OK_BTN_BLUE_COLOR_PNT);
					}
					else
					{
						goto Again;
					}

					lst_stock = code;
					lst_qty = qty;
					lst_price = "0.0";
				}
				else if (action == 'S')
				{
					dlg->clickMouse(GF_Wnd, SELL_MENU);
					::Sleep(ACTION_DELAY_300);

					dlg->clickMouse(GF_Wnd, STOCK_FIELD);
					dlg->clearField(GF_Wnd);
					::Sleep(ACTION_DELAY_200);
					dlg->sendString(GF_Wnd, code);
					::Sleep(ACTION_DELAY_500);

					dlg->clickMouse(GF_Wnd, PRICE_FIELD);
					::Sleep(ACTION_DELAY_200);
					dlg->clickMouse(GF_Wnd, PRICE_OPT5);
					::Sleep(ACTION_DELAY_200);

					dlg->clickMouse(GF_Wnd, QTY_FIELD);
					dlg->clearField(GF_Wnd);
					::Sleep(ACTION_DELAY_200);
					dlg->sendString(GF_Wnd, qty);
					::Sleep(ACTION_DELAY_200);
					dlg->clickMouse(GF_Wnd, BUY_SELL_BTN);
					::Sleep(ACTION_DELAY_500);

					if (dlg->checkPointHasColor(YES_CONFIRM_PNT, YES_CONFIRM_BLUE_COLOR))
					{
						dlg->clickMouse(GF_Wnd, YES_CONFIRM_PNT);
						::Sleep(ACTION_DELAY_300);

						if (dlg->checkPointHasColor(OK_BTN_BLUE_COLOR_PNT, OK_BTN_BLUE_COLOR))
						{
							success_flg = "N";
							dlg->clickMouse(GF_Wnd, OK_BTN_BLUE_COLOR_PNT);
						}
						else
						{
							success_flg = "Y";
						}
					}
					else if (dlg->checkPointHasColor(OK_BTN_BLUE_COLOR_PNT, OK_BTN_BLUE_COLOR))
					{
						success_flg = "N";
						dlg->clickMouse(GF_Wnd, OK_BTN_BLUE_COLOR_PNT);
					}
					else
					{
						goto Again;
					}

					lst_stock = code;
					lst_qty = qty;
					lst_price = "0.0";
				}
				::Sleep(ACTION_DELAY_300);
			}
			else {
				::Sleep(ACTION_DELAY_1000);
			}
		}
	}
	else {
		AfxMessageBox((CString)"请打开广发证券客户端并登录!");
	}

	return 0;
}

boolean CGFTraderDlg::checkPointHasColor(CPoint pt, COLORREF clr)
{
	CWindowDC dc(CWnd::FromHandle(GF));

	COLORREF color = dc.GetPixel(pt);

	int Red = GetRValue(color);
	int Green = GetGValue(color);
	int Blue = GetBValue(color);


	CString msg;
	msg.Format(_T("OK_BTN_BLUECOLOR CHECK:%ld vs %ld"), color, clr);
	AfxTrace(msg);
	//AfxMessageBox(msg);
	if (color == clr)
	{
		return true;
	}
	return false;
}

CWinThread *bck_thread = NULL;






boolean CGFTraderDlg::getMYSQLConnection()
{
	if (!mysql_init_flg || mysql_ping(&m_sqlCon))
	{
		mysql_init(&m_sqlCon);
		char value = 1;
		mysql_options(&m_sqlCon, MYSQL_OPT_RECONNECT, &value);
		// localhost:服务器 root/123456为账号密码 managesystemdb为数据库名 3306为端口    
		//if (!mysql_real_connect(&m_sqlCon, "111.229.27.150", "root", "mysql,16", "StockApp", 3306, NULL, 0))
		if (!mysql_real_connect(&m_sqlCon, "127.0.0.1", "root", "mysql,16", "StockApp", 3306, NULL, 0))
		{
			AfxMessageBox(_T("访问数据库失败!"));
			CString e = (CString)mysql_error(&m_sqlCon);//需要将项目属性中字符集修改为“使用多字节字符集”或“未设置”  
			MessageBox(e);
			return false;
		}
		mysql_init_flg = true;
	}
	return true;
}

CString CGFTraderDlg::getTradeString(CString &lst_stock, CString &lst_qty, CString &lst_price, CString success_flg)
{
	/*
	int r = rand() % 100;
	if (r < 50)
	{
		return (CString)"B002349500";
	}
	else
	{
		return (CString)"S6016071400";
	}*/

	CString rtv = (CString)"";


	if (getMYSQLConnection())
	{
		if (lst_stock.GetLength() > 0)
		{
			//sprintf(sql,  "update pendingTrade set status = 'S', success_qty = %s, success_price = %s where stock = '%s' and status = 'N'", lst_qty, lst_price, lst_stock);

			CString status = (CString)"C";
			if (success_flg == "Y")
			{
				status = "S";
			}
			else if (success_flg == "N")
			{
				status = "C";
				lst_qty = " success_qty";
				lst_price = " success_price";
			}

			mysql_query(&m_sqlCon, (CString)"update pendingTrade set status = '" + status  + "', success_qty = " + lst_qty + ", success_price = " + lst_price + " where stock = '" + lst_stock + "' and id = " + lst_id + " and status = 'N'");
			
			int count = mysql_affected_rows(&m_sqlCon);

			if (count == 1)
			{
				mysql_commit(&m_sqlCon);
			}
			else
			{
				mysql_rollback(&m_sqlCon);
			}

			CString msg;
			msg.Format(_T("pendingTrade table updated with %d rows."), count);
			AfxTrace(msg);
			lst_stock = "";
			lst_qty = "";
			lst_price = "";
			lst_id = "";
		}
		
		//mysql_query(&m_sqlCon, "SET NAMES 'UTF-8'");//解决从数据库中读取数据后汉字乱码显示的问题  

			//查询数据
			if (mysql_query(&m_sqlCon, "select stock, id, qty, price, is_buy_flg from pendingTrade where status = 'N' order by stock"))
				return (CString)"";

			//获取结果集
			m_res = mysql_store_result(&m_sqlCon);

			//获取记录
			std::vector<std::string> m_data[100];  //存放数据库记录，最大为100条
			int i = 0;
			if (m_row = mysql_fetch_row(m_res))
			{

				CString stock_id = (CString)m_row[0];
				lst_id = (CString)m_row[1];
				CString qty = (CString)m_row[2];
				CString price = (CString)m_row[3];
				CString is_buy_flg = (CString)m_row[4];

				CString onerecord;
				onerecord.Format((CString)"\nStock:%s - Qty:%s - Price:%s, isBuy:%s\n", stock_id, qty, price, is_buy_flg);
				//OutputDebugString(onerecord);
				AfxTrace(onerecord);

				if (is_buy_flg == "1")
				{
					rtv = (CString)"B" + stock_id + qty;
				}
				else {
					rtv = (CString)"S" + stock_id + qty;
				}
			}
			mysql_free_result(m_res);
			//mysql_close(&m_sqlCon);
		return rtv;
	}
}

void CGFTraderDlg::clickMouse(CWnd * pWnd, CPoint pt)
{
	::SendMessage(*pWnd, WM_LBUTTONDOWN, 0, (pt.y << 16) + pt.x);
	::SendMessage(*pWnd, WM_LBUTTONUP, 0, (pt.y << 16) + pt.x);
}

void CGFTraderDlg::pressKey(CWnd * pWnd, CHAR c)
{
	pWnd->SendMessage(WM_KEYDOWN, c);
	pWnd->SendMessage(WM_CHAR, c);
	pWnd->SendMessage(WM_KEYUP, c);
}

void CGFTraderDlg::sendString(CWnd* pWnd, CString str)
{
	for (int i = 0; i < str.GetLength(); i++)
	{
		pressKey(pWnd, str[i]);
	}
}

void CGFTraderDlg::clearField(CWnd* pWnd)
{
	for (int i = 0; i < 10; i++)
	{
		pWnd->SendMessage(WM_KEYDOWN, VK_BACK);
		pWnd->SendMessage(WM_CHAR, VK_BACK);
		pWnd->SendMessage(WM_KEYUP, VK_BACK);
		//::Sleep(300);
	}
	for (int i = 0; i < 10; i++)
	{
		pWnd->SendMessage(WM_KEYDOWN, VK_DELETE);
		pWnd->SendMessage(WM_CHAR, VK_DELETE);
		pWnd->SendMessage(WM_KEYUP, VK_DELETE);
		//::Sleep(300);
	}
}
void CGFTraderDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnClose();
	CDialogEx::OnOK();
}


void CGFTraderDlg::OnBnClickedTrade()
{
	if (bck_thread == NULL)
	{
		stop_trade = false;
		bck_thread = AfxBeginThread((AFX_THREADPROC)ThreadTrade, NULL, THREAD_PRIORITY_IDLE);
	}
	else
	{
		stop_trade = false;
	}

	CButton* pBtn1 = (CButton*)this->GetDlgItem(IDTRADE);
	pBtn1->EnableWindow(stop_trade);
	CButton* pBtn2 = (CButton*)this->GetDlgItem(IDSTOPTRADE);
	pBtn2->EnableWindow(!stop_trade);

}


void CGFTraderDlg::OnBnClickedBalance()
{
	// TODO: 在此添加控件通知处理程序代码
	if (getMYSQLConnection())
	{
		UINT rst = AfxMessageBox(L"你确定要一键平仓吗？", MB_YESNO);
		if (rst == IDYES)
		{
			mysql_query(&m_sqlCon, "update usrstk set stop_trade_mode_flg = 1 where gz_flg = 1 and stop_trade_mode_flg = 0 and suggested_by <> 'SYSTEM_SUGGESTER'");

			int count = mysql_affected_rows(&m_sqlCon);

			if (count >= 1)
			{
				mysql_commit(&m_sqlCon);
			}
			else
			{
				mysql_rollback(&m_sqlCon);
			}

			CString msg;
			msg.Format(_T("一共有 %d 支股票设置平仓."), count);
			AfxMessageBox(msg);
		}
	}
}


void CGFTraderDlg::OnBnClickedStoptrade()
{
	// TODO: 在此添加控件通知处理程序代码
	stop_trade = true;
	bck_thread = NULL;
	CButton* pBtn1 = (CButton*)this->GetDlgItem(IDTRADE);
	pBtn1->EnableWindow(stop_trade);
	CButton* pBtn2 = (CButton*)this->GetDlgItem(IDSTOPTRADE);
	pBtn2->EnableWindow(!stop_trade);
}


void CGFTraderDlg::OnEnChangeStock()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CGFTraderDlg::OnBnClickedBtnBuy()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	CEdit* pEdit;
	CString stkid;
	pEdit = (CEdit*)GetDlgItem(IDC_STOCK_ID);
	pEdit->GetWindowText(str);

	stkid = str;

	if (stkid.GetLength() <= 0) {
		AfxMessageBox(_T("请输入股票代码!"));
		return;
	}

	pEdit = (CEdit*)GetDlgItem(IDC_TRADE_HANDS);
	pEdit->GetWindowText(str);

	int hands = _wtof(str);

	if (hands <= 0)
	{
		AfxMessageBox(_T("购买数量为零!"));
		return;
	}


	if (getMYSQLConnection())
	{
		
		CString sql;

		//mysql_query(&m_sqlCon, "SET NAMES 'UTF-8'");

		sql.Format(_T("insert into pendingTrade select '" + stkid + "', case when max(id) is null then 0 else max(id) + 1 end, %d , 0, 0, 0.0, 'N', null, 1, sysdate(), sysdate() from pendingTrade where stock = '" + stkid + "'"), hands);
		mysql_query(&m_sqlCon, (CStringA)sql);

		m_res = mysql_store_result(&m_sqlCon);

		int count = mysql_affected_rows(&m_sqlCon);

		if (count >= 1)
		{
			mysql_commit(&m_sqlCon);

			str.Format(_T("下单购买:" + stkid + " %d 手成功!"), hands / 100);
			AfxMessageBox(str);
		}
		else
		{
			mysql_rollback(&m_sqlCon);
		}
		//mysql_close(&m_sqlCon);
	}
}


BOOL CGFTraderDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (::TranslateAccelerator(GetSafeHwnd(), hAccTable, pMsg))
	{
		return true;
	}

	if (WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam)
	{
		if (GetFocus() == GetDlgItem(IDC_STOCK_ID))
		{
			CEdit* pEdit;
			pEdit = (CEdit*)GetDlgItem(IDC_TRADE_MNY);
			pEdit->SetFocus();
			return true;
		}
		else if (GetFocus() == GetDlgItem(IDC_TRADE_MNY))
		{
			CEdit* pEdit;
			pEdit = (CEdit*)GetDlgItem(IDC_TRADE_HANDS);
			pEdit->SetFocus();
			return true;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CGFTraderDlg::OnEnKillfocusStockId()
{
	// TODO: 在此添加控件通知处理程序代码

	CEdit* pBoxOne;
	pBoxOne = (CEdit*)GetDlgItem(IDC_STOCK_ID);
	CString str;
	pBoxOne->GetWindowText(str);

	if (str.GetLength() > 0) {
		pBoxOne = (CEdit*)GetDlgItem(IDC_TRADE_MNY);
		pBoxOne->SetWindowText(_T(""));
		pBoxOne = (CEdit*)GetDlgItem(IDC_TRADE_HANDS);
		pBoxOne->SetWindowText(_T(""));
		SetFiledValues();
		return;
	}
}

BOOLEAN CGFTraderDlg::SetFiledValues()
{
	BOOLEAN ready_to_go = false;
	CString str;
	CEdit* pEdit;
	pEdit = (CEdit*)GetDlgItem(IDC_STOCK_ID);

	pEdit->GetWindowText(str);

	if (str.GetLength() <= 0) {
		AfxMessageBox(_T("请输入股票代码!"));
		return false;
	}

	if (getMYSQLConnection())
	{
		CString stkid = str;
		CString sql;

		//mysql_query(&m_sqlCon, "SET NAMES 'UTF-8'");

		sql.Format(_T("select s.name, s3.cur_pri from stk s join (select id, max(ft_id) max_ft_id from stkdat2 where id = '%s' group by id) s2 on s.id = s2.id  join stkdat2 s3 on s2.max_ft_id = s3.ft_id where s.id = '%s'"), stkid, stkid);
		mysql_query(&m_sqlCon, (CStringA)sql);

		m_res = mysql_store_result(&m_sqlCon);

		std::vector<std::string> m_data[100];
		int i = 0;
		if (m_row = mysql_fetch_row(m_res))
		{
			CString name = (CString)m_row[0];
			CString price = (CString)m_row[1];

			pEdit = (CEdit*)GetDlgItem(IDC_STOCK_NAME);
			pEdit->SetWindowText(stkid + "/" + price);

			pEdit = (CEdit*)GetDlgItem(IDC_TRADE_MNY);
			pEdit->GetWindowText(str);

			double trade_tot_mny = 0;
			if (str.GetLength() <= 0) {

				pEdit = (CEdit*)GetDlgItem(IDC_TRADE_HANDS);
				pEdit->GetWindowText(str);

				if (str.GetLength() > 0)
				{
					pEdit = (CEdit*)GetDlgItem(IDC_TRADE_MNY);
					trade_tot_mny = _wtof(str) * _wtof(price);
					str.Format((_T("%.0f")), trade_tot_mny);
					pEdit->SetWindowText(str);
					return true;
				}

				pEdit = (CEdit*)GetDlgItem(IDC_TRADE_MNY_DEF);
				pEdit->GetWindowText(str);

				if (str.GetLength() <= 0) {

					str = "10000";
					pEdit->SetWindowText(_T("10000"));
				}
			}

			trade_tot_mny = _wtof(str);

			int hands = trade_tot_mny / _wtof(price);

			hands = hands / 100 * 100;

			if (hands <= 0)
			{
				AfxMessageBox(_T("购买数量为零!"));
				return false;
			}
			else
			{
				pEdit = (CEdit*)GetDlgItem(IDC_TRADE_HANDS);
				str.Format(_T("%d"), hands);
				pEdit->SetWindowText(str);

				pEdit = (CEdit*)GetDlgItem(IDC_TRADE_MNY);
				trade_tot_mny = hands * _wtof(price);
				str.Format((_T("%.0f")), trade_tot_mny);
				pEdit->SetWindowText(str);

				ready_to_go = true;
			}
			//str.Format(_T(" Are you sure to buy stock:%s with price %s for %d hands?"), stkid, price, hands);
			//AfxMessageBox(str);
		}
		mysql_free_result(m_res);
		//mysql_close(&m_sqlCon);
	}
	else 
	{
		AfxMessageBox(_T("数据库连接失败!"));
	}
	return ready_to_go;
}

void CGFTraderDlg::OnEnKillfocusTradeMny()
{
	// TODO: 在此添加控件通知处理程序代码
	CEdit* pEdit;
	CString str;

	pEdit = (CEdit*)GetDlgItem(IDC_STOCK_ID);

	pEdit->GetWindowText(str);

	if (str.GetLength() <= 0) {
		return;
	}

	pEdit = (CEdit*)GetDlgItem(IDC_TRADE_MNY);
	
	pEdit->GetWindowText(str);

	SetFiledValues();
}


void CGFTraderDlg::OnBnClickedBtnSell()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	CEdit* pEdit;
	CString stkid;
	pEdit = (CEdit*)GetDlgItem(IDC_STOCK_ID);
	pEdit->GetWindowText(str);

	stkid = str;

	if (stkid.GetLength() <= 0) {
		AfxMessageBox(_T("请输入股票代码!"));
		return;
	}

	pEdit = (CEdit*)GetDlgItem(IDC_TRADE_HANDS);
	pEdit->GetWindowText(str);

	int hands = _wtof(str);

	if (hands <= 0)
	{
		AfxMessageBox(_T("售出数量为零!"));
		return;
	}


	if (getMYSQLConnection())
	{

		CString sql;

		//mysql_query(&m_sqlCon, "SET NAMES 'UTF-8'");

		sql.Format(_T("insert into pendingTrade select '" + stkid + "', case when max(id) is null then 0 else max(id) + 1 end, %d , 0, 0, 0.0, 'N', null, 0, sysdate(), sysdate() from pendingTrade where stock = '" + stkid + "'"), hands);
		mysql_query(&m_sqlCon, (CStringA)sql);

		m_res = mysql_store_result(&m_sqlCon);

		int count = mysql_affected_rows(&m_sqlCon);

		if (count >= 1)
		{
			mysql_commit(&m_sqlCon);

			str.Format(_T("下单出售:" + stkid + " %d 手成功!"), hands / 100);
			AfxMessageBox(str);
		}
		else
		{
			mysql_rollback(&m_sqlCon);
		}
		
	}
}


void CGFTraderDlg::OnEnKillfocusTradeHands()
{
	// TODO: 在此添加控件通知处理程序代码
	CEdit* pEdit;
	pEdit = (CEdit*)GetDlgItem(IDC_STOCK_ID);
	CString str;
	pEdit->GetWindowText(str);

	if (str.GetLength() > 0) {

		pEdit = (CEdit*)GetDlgItem(IDC_TRADE_HANDS);
		pEdit->GetWindowText(str);

		if (str.GetLength() > 0)
		{
			pEdit = (CEdit*)GetDlgItem(IDC_TRADE_MNY);
			pEdit->GetWindowText(str);
			pEdit->SetWindowText(_T(""));
		}

		SetFiledValues();
		return;
	}
}


void CGFTraderDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//AfxMessageBox(_T("Close db connection and exit?"));
	mysql_close(&m_sqlCon);
	CDialogEx::OnClose();
}
