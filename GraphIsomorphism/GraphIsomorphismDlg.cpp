
// GraphIsomorphismDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "GraphIsomorphism.h"
#include "GraphIsomorphismDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGraphIsomorphismDlg 对话框



CGraphIsomorphismDlg::CGraphIsomorphismDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GRAPHISOMORPHISM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGraphIsomorphismDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DIRECT, comboDirectControl);
	DDX_Control(pDX, IDC_COMBOF_FUNC, comboFuncContro);
	DDX_Control(pDX, IDC_EDIT_GRAPH_F, editGraphF);
	DDX_Control(pDX, IDC_EDIT_GRAPH_S, editGraphS);
	DDX_Control(pDX, IDC_EDIT_OUT, editOut);
	DDX_Control(pDX, IDC_JUDGE, buttonJudge);
}

BEGIN_MESSAGE_MAP(CGraphIsomorphismDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_JUDGE, &CGraphIsomorphismDlg::OnBnClickedJudge)
END_MESSAGE_MAP()


// CGraphIsomorphismDlg 消息处理程序

BOOL CGraphIsomorphismDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	comboDirectControl.AddString(TEXT("无向图"));//0
	comboDirectControl.AddString(TEXT("有向图"));//1
	comboDirectControl.SetCurSel(0);
	comboFuncContro.AddString(TEXT("递归"));//GI_DFS
	comboFuncContro.AddString(TEXT("递归(度优化)"));//GI_DFS_WITH_DEGREE
	comboFuncContro.AddString(TEXT("递归(HASH优化)"));//GI_HASH
	comboFuncContro.SetCurSel(2);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGraphIsomorphismDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGraphIsomorphismDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


UINT judgeThread(LPVOID ptr)
{
	CGraphIsomorphismDlg *dlg = (CGraphIsomorphismDlg*)ptr;
	dlg->buttonJudge.SetWindowTextW(TEXT("正在查找......"));
	dlg->buttonJudge.EnableWindow(false);
	//获取数据
	CString graphStr, graphStr2;
	dlg->editGraphF.GetWindowText(graphStr);
	dlg->editGraphS.GetWindowText(graphStr2);
	graphStr.Replace(TEXT("\t"), TEXT(" "));
	graphStr2.Replace(TEXT("\t"), TEXT(" "));
	std::stringstream ss;
	ss << CT2A(graphStr.GetString()) << std::endl << CT2A(graphStr2.GetString());
	bool ifWithDirection = dlg->comboDirectControl.GetCurSel();
	int func = dlg->comboFuncContro.GetCurSel();
	//操作
	graphMat g1, g2;
	int m, n;
	int u, v;
	//g1
	ss >> m >> n;
	g1.resize(m);
	for (int i = 0; i < n; ++i)
	{
		ss >> u >> v;
		if (ifWithDirection)
			g1.addEdge(u - 1, v - 1);
		else g1.addEdgeWithoutDirection(u - 1, v - 1);
	}
	//g2
	ss >> m >> n;
	g2.resize(m);
	for (int i = 0; i < n; ++i)
	{
		ss >> u >> v;
		if (ifWithDirection)
			g2.addEdge(u - 1, v - 1);
		else g2.addEdgeWithoutDirection(u - 1, v - 1);
	}
	g1.setMode(func);
	g1.isomorphism(g2);
	int *res = g1.getResult();
	std::stringstream sss;
	if (res)
	{
		sss << "是否同构:是\r\n映射关系:\r\n";
		for (int i = 0; i < m; i++)
		{
			sss << i + 1 << " -> " << res[i] + 1 << "\r\n";
		}
	}
	else sss << "是否同构:否\r\n";
	dlg->editOut.SetWindowTextW(CA2T(sss.str().c_str()));
	dlg->buttonJudge.EnableWindow(true);
	dlg->buttonJudge.SetWindowTextW(TEXT("判\n\n\n\n断"));
	return 0;
}

void CGraphIsomorphismDlg::OnBnClickedJudge()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxBeginThread(judgeThread,this);
}
