
// bigDlg.h: 头文件
//

#pragma once

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>

using namespace cv;
using namespace std;

const Scalar SCALAR_GREEN = Scalar(0.0, 200.0, 0.0);
const Scalar SCALAR_RED = Scalar(0.0, 0.0, 255.0);

// CbigDlg 对话框
class CbigDlg : public CDialogEx
{
// 构造
public:
	CbigDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BIG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// rect1-5用来将视频输出窗口绑定到控件
	CRect rect1, rect2, rect3, rect4, rect5;
	// 用来读取视频
	VideoCapture capture;
	// 存源视频每一帧
	Mat frame;
	// 用来保存原视频
	Mat image;
	// 提取运动目标
	Mat mov_target;
	// 提取背景
	Mat background;
	// 提取运动目标二值图
	Mat bin_image;
	// 运算过程中间变量
	Rect rect;
	// 存提取出的轮廓
	vector<vector<Point>> contours;

	vector<Vec4i> hierarchy;

	// 存储中心点的数组
	vector<Point> center_point_array;
	
	Ptr<BackgroundSubtractorMOG2> pBgmodel;

	int count;
	CString path;

	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
};
