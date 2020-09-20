
// GraphIsomorphismDlg.h: 头文件
//

#pragma once
#include "graphMat.h"
#include <sstream>

// CGraphIsomorphismDlg 对话框
class CGraphIsomorphismDlg : public CDialogEx
{
// 构造
public:
	CGraphIsomorphismDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GRAPHISOMORPHISM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox comboDirectControl;
	CComboBox comboFuncContro;
	afx_msg void OnBnClickedJudge();
	CEdit editGraphF;
	CEdit editGraphS;
	CEdit editOut;
	CButton buttonJudge;
};
