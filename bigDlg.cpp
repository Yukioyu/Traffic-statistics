
// bigDlg.cpp: 实现文件

#include "pch.h"
#include "framework.h"
#include "big.h"
#include "bigDlg.h"
#include "afxdialogex.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <Windows.h>
using namespace std;
using namespace cv;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// 在外部声明一个用于存储每个矩形框轨迹的容器
std::map<int, std::vector<Point>> trajectories;

// 存储每个车辆的ID和中心点
std::map<int, cv::Point> vehicleCenters;
int vehicleID = 0;
// 定义一个阈值，用于判断两个点是否属于同一辆车
const double distanceThreshold = 250.0;



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
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















// CbigDlg 对话框



CbigDlg::CbigDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BIG_DIALOG, pParent)
	, count(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CbigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, count);
}

BEGIN_MESSAGE_MAP(CbigDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CbigDlg::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, &CbigDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CbigDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CbigDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CbigDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CbigDlg 消息处理程序

BOOL CbigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	// IDC_STATIC控件与video输出窗口连接
	CWnd* pWnd1 = GetDlgItem(IDC_STATIC);
	pWnd1->GetClientRect(&rect1);
	namedWindow("video", WINDOW_AUTOSIZE);
	HWND hWnd1 = (HWND)cvGetWindowHandle("video");
	HWND hParent1 = ::GetParent(hWnd1);
	::SetParent(hWnd1, GetDlgItem(IDC_STATIC)->m_hWnd);
	::ShowWindow(hParent1, SW_HIDE);

	// IDC_STATIC2控件与video2窗口连接
	CWnd* pWnd2 = GetDlgItem(IDC_STATIC2);
	pWnd2->GetClientRect(&rect2);
	namedWindow("video2", WINDOW_AUTOSIZE);
	HWND hWnd2 = (HWND)cvGetWindowHandle("video2");
	HWND hParent2 = ::GetParent(hWnd2);
	::SetParent(hWnd2, GetDlgItem(IDC_STATIC2)->m_hWnd);
	::ShowWindow(hParent2, SW_HIDE);

	// IDC_STATIC3控件与video3窗口连接
	CWnd* pWnd3 = GetDlgItem(IDC_STATIC3);
	pWnd3->GetClientRect(&rect3);
	namedWindow("video3", WINDOW_AUTOSIZE);
	HWND hWnd3 = (HWND)cvGetWindowHandle("video3");
	HWND hParent3 = ::GetParent(hWnd3);
	::SetParent(hWnd3, GetDlgItem(IDC_STATIC3)->m_hWnd);
	::ShowWindow(hParent3, SW_HIDE);

	// IDC_STATIC4控件与video4窗口连接
	CWnd* pWnd4 = GetDlgItem(IDC_STATIC4);
	pWnd4->GetClientRect(&rect4);
	namedWindow("video4", WINDOW_AUTOSIZE);
	HWND hWnd4 = (HWND)cvGetWindowHandle("video4");
	HWND hParent4 = ::GetParent(hWnd4);
	::SetParent(hWnd4, GetDlgItem(IDC_STATIC4)->m_hWnd);
	::ShowWindow(hParent4, SW_HIDE);

	// IDC_STATIC5控件与video5窗口连接
	CWnd* pWnd5 = GetDlgItem(IDC_STATIC5);
	pWnd5->GetClientRect(&rect5);
	namedWindow("video5", WINDOW_AUTOSIZE);
	HWND hWnd5 = (HWND)cvGetWindowHandle("video5");
	HWND hParent5 = ::GetParent(hWnd5);
	::SetParent(hWnd5, GetDlgItem(IDC_STATIC5)->m_hWnd);
	::ShowWindow(hParent5, SW_HIDE);

	// 用于动态目标检测
	pBgmodel = createBackgroundSubtractorMOG2();
	// 设置方差阈值参数，用于判断当前像素是前景还是背景
	pBgmodel->setVarThreshold(500);
	// 汽车数量初始化
	count = 0;
	// 文件地址初始化
	path = _T("");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CbigDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CbigDlg::OnPaint()
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
HCURSOR CbigDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void GetContours(vector<vector<Point>>& contours)
{
	// 设置最小轮廓长度与最大轮廓长度
	int cmin = 100;   
	int cmax = 1000;
	// 定义迭代器
	vector<vector<Point>>::const_iterator iterator = contours.begin();
	while (iterator != contours.end())
	{
		if ((iterator->size()) < cmin || (iterator->size()) > cmax)
		{
			// 若轮廓长度小于最小值或大于最大值，就删除这个轮廓
			iterator = contours.erase(iterator);
		}
		else
		{
			iterator++;
		}
	}
}

bool DescSort(vector<Point> p1, vector<Point> p2)
{
	// 用于sort函数对于轮廓按照轮廓面积降序排序
	return contourArea(p1) > contourArea(p2);
}


// 用于获取矩阵的中心点
Point GetCenterPoint(Rect rect)
{
	Point c_point;
	c_point.x = rect.x + cvRound(rect.width / 2.0);
	c_point.y = rect.y + cvRound(rect.height / 2.0);
	return c_point;
}

bool state = false;  // false代表原视频，true代表识别状态
bool is_open = false;  // 用来判断是否打开了文件


void CbigDlg::OnBnClickedButton1()
{
	// 打开文件
	string str;
	str = CT2A(path.GetString());
	LPCSTR p = str.data();
	OutputDebugStringA(p);
	//TRACE(str);

	// 车辆计数清零
	count = 0;
	UpdateData(FALSE);

;	state = false;
	capture.open(str);

	if (!capture.isOpened())
	{
		MessageBox(_T("打开视频失败！"), _T("提示"));
	}
	else
	{
		// 成功打开视频则is_open置true，打开定时器1
		is_open = true;
		SetTimer(1, 60, NULL);
	}
}


void CbigDlg::OnBnClickedButton2()
{
	// 若没有先打开文件，则发出警告
	if (is_open == false)
	{
		MessageBox(_T("请先打开视频文件！"), _T("提示"));
	}
	else
	{
		string str;
		str = CT2A(path.GetString());
		state = true;

		// 汽车数量计数器清零
		count = 0;
		UpdateData(FALSE);

		// 打开要识别的文件，打开定时器2进行识别
		capture.open(str);
		SetTimer(2, 60, NULL);
	}
}


void CbigDlg::OnBnClickedButton3()
{
	// str存当前按钮的文字
	CString str;
	GetDlgItem(IDC_BUTTON3)->GetWindowTextW(str);

	// 如果当前按钮为源视频播放暂停状态，则要显示继续并且暂停定时器1
	if (str == "暂停" && state == false)
	{
		GetDlgItem(IDC_BUTTON3)->SetWindowText(L"继续");
		KillTimer(1);
	}
	// 如果当前按钮为源视频识别暂停状态，则要显示继续并且暂停定时器2
	else if (str == "暂停" && state == true)
	{
		GetDlgItem(IDC_BUTTON3)->SetWindowText(L"继续");
		KillTimer(2);
	}
	// 如果当前按钮为源视频播放继续状态，则要显示暂停并且打开定时器1
	else if (str == "继续" && state == false)
	{
		GetDlgItem(IDC_BUTTON3)->SetWindowText(L"暂停");
		SetTimer(1, 60, NULL);
	}
	// 如果当前按钮为源视频识别继续状态，则要显示暂停并且打开定时器2
	else
	{
		GetDlgItem(IDC_BUTTON3)->SetWindowText(L"暂停");
		SetTimer(2, 60, NULL);
	}
}

void CbigDlg::OnTimer(UINT_PTR nIDEvent)
{
	// 定时器事件1，当打开视频时调用
	if (nIDEvent == 1)
	{
		// 读入视频一帧
		capture >> frame;
		if (state == true)
		{
			// 如果已经在识别状态，则停止定时器1
			KillTimer(1);
		}
		else if (frame.empty())
		{
			// 如果视频读取完毕，则停止定时器1，并输出提示
			KillTimer(1);
			MessageBox(_T("视频播放完毕！"), _T("提示"));
		}
		else
		{
			// 将视频缩放到和控件一样的大小
			resize(frame, frame, Size(rect1.Width(), rect1.Height()));
			// 显示视频
			imshow("video", frame);
		}
	}

	// 定时器事件2，当识别视频时使用
	else if (nIDEvent == 2) 
	{

		// 读入一帧
		capture >> frame;
		// 如果为播放原视频状态，则停止定时器2
		if (state == false) 
		{
			KillTimer(2);
		}
		else if (frame.empty())
		{
			// 视频识别完毕，则停止定时器2，并输出相应提示
			KillTimer(2);
			MessageBox(_T("视频识别完毕！"), _T("提示"));
		}
		else
		{
			// 用来存储剔除后的轮廓
			vector<vector<Point>> real_contours;

			// 返回大小为13*13的结构元素
			Mat element = getStructuringElement(MORPH_RECT, Size(13, 13));

			// 将当前帧缩小为原来四分之一，加快处理速度
			//resize(frame, image, Size(frame.cols / 2, frame.rows / 2), INTER_LINEAR);
			resize(frame, image, Size(frame.cols / 3, frame.rows / 3), INTER_LINEAR);

			// 得到前景掩码图像，存放在bin_image中（灰度图）		
			pBgmodel->apply(image, bin_image);

			// 下面是根据前景图的操作，和原图像融合得到有纹理的前景图
			// 高斯滤波，内核大小为5*5
			GaussianBlur(bin_image, bin_image, Size(5, 5), 0);

			// 进行闭运算，用来填充背景内细小空洞，平滑边界
			// 膨胀，使用element结构元素扩展前景图
		
		
			dilate(bin_image, bin_image, element);
			erode(bin_image, bin_image, element);
	
			// 二值化阈值处理，小于阈值10的像素，替换为0，大于10的像素，替换为255
			threshold(bin_image, bin_image, 10, 255, THRESH_BINARY);

			mov_target.create(image.size(), image.type());
			// 将mov_target所有像素置为0
			mov_target = Scalar::all(0);

			// 将image复制到mov_target中，并且image对应bin_image中像素值为0的像素点都不会复制到mov_target中
			// 相当于提取运动目标
			image.copyTo(mov_target, bin_image);

			// 得到背景图像
			pBgmodel->getBackgroundImage(background);

			// 使用findContours函数检测轮廓，contours中有多少元素，就有多少个轮廓
			// 第四个参数CV_RETR_EXTERNAL表示只检测外轮廓，第五个参数CV_CHAIN_APPROX_NONE保存物体边界所有连续的轮廓点，第六个参数为偏移量
			findContours(bin_image, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));

			// 对不符合要求的轮廓进行删除
			GetContours(contours);
			
			// 排序
			sort(contours.begin(), contours.end(), DescSort);
			for (int i = 0; i < contours.size(); i++)
			{
				// 去除伪目标，当第i个轮廓的面积小于最大轮廓面积的五分之一，就算做伪目标
				if (contourArea(contours[i]) < contourArea(contours[0]) / 5)
					break;

				// 使用boundingRect函数计算轮廓的最小外接矩形，并存入real_contours变量中
				rect = boundingRect(contours[i]);
				real_contours.push_back(contours[i]);

				// 对每辆检测出来的车辆绘制绿色矩形框
				rectangle(image, rect, SCALAR_GREEN, 2);
			}
			// 绘制一个固定区域对车辆进行计数
			int image_height = (int)round((double)image.rows * 0.4);
			Rect test_area(0, image_height, image.cols, 150);
			rectangle(image, test_area, SCALAR_GREEN, 2);

			for (int i = 0; i < real_contours.size(); i++) 
			{
				Rect real_rect = boundingRect(real_contours[i]);

				// 获得矩形中心点
				Point center;
				center = GetCenterPoint(boundingRect(real_contours[i]));
				// 如果中心点进入矩形框
				if (center.y <= image_height + 100 && center.y >= image_height)
				{
					double distance;
					// 使用is_caculate标记来记录车辆是否重复计数
					bool is_caculate = false;

					if (center_point_array.size() > 0) 
					{
						// 遍历所有之前的中心点
						for (auto i = center_point_array.begin(); i != center_point_array.end(); i++)
						{
							// 计算与每个之前中心点的距离，判断是不是同一辆车
							distance = sqrt(pow((center.x - i->x), 2) + pow((center.y - i->y), 2));
							// 如果距离小于某一个阈值，那么就说明是同一辆车
							if (distance <= 30)
							{
								// 更新数组中的中心点
								*i = center;
								// 当前汽车已被计算过
								is_caculate = true;
							}
						}
					}
					// 如果当前汽车没有被遍历过，就要计数
					if (is_caculate == false)
					{
						center_point_array.push_back(center);
						int currentID = ++vehicleID; // 分配新的ID
						count++;
						UpdateData(FALSE);
						// 初始化车辆轨迹
						trajectories[currentID] = std::vector<cv::Point>();
						// 在图像上显示车辆的ID
						std::string text = "ID " + std::to_string(currentID);
						cv::putText(image, text, center, cv::FONT_HERSHEY_SIMPLEX, 1, SCALAR_RED, 2);
					}

					// 更新车辆轨迹
					if (!trajectories.empty())
					{
						for (auto& pair : trajectories)
						{
							int currentID = pair.first;
							std::vector<cv::Point>& trajectory = pair.second;
							// 获取当前轨迹的最后一个点
							cv::Point lastPoint = trajectory.empty() ? cv::Point(-1, -1) : trajectory.back();
							// 计算当前中心点与最后一个点的距离
							double distance = cv::norm(center - lastPoint);
							// 如果距离小于阈值，则将新中心点加入轨迹向量中
							//if (distance < distanceThreshold)
							//{
							trajectory.push_back(center);

						}                          
					}
					// 绘制车辆轨迹
					cv::Scalar SCALAR_BLUE = cv::Scalar(255, 0, 0);
					// 创建一个克隆图像用于绘制轨迹
					cv::Mat CopyFrame = image.clone();

					// 绘制已经计算过的车辆轨迹
					for (const auto& pair : trajectories)
					{
						const std::vector<cv::Point>& trajectory = pair.second;
						if (trajectory.size() > 1)
						{
							for (int j = 1; j < trajectory.size(); j++)
							{
								cv::circle(CopyFrame, trajectory[j], 2, cv::Scalar(0, 0, 255), -1);
							}
						}
					}
					// 显示绘制好轨迹的帧
					cv::imshow("Trajectory", CopyFrame);
					// 进入检测矩形框就要标红
					rectangle(image, real_rect, SCALAR_RED, 2);
			}
				else 
				{
					// 没有进入检测矩形框但是检测到了就标为绿色
					rectangle(image, real_rect, SCALAR_GREEN, 2);
				}
			}

			// 使视频的画面大小适应窗口的大小，将各个界面输出到屏幕上
			resize(image, image, Size(rect1.Width(), rect1.Height()));
			resize(frame, frame, Size(rect2.Width(), rect2.Height()));
			resize(background, background, Size(rect3.Width(), rect3.Height()));
			resize(mov_target, mov_target, Size(rect4.Width(), rect4.Height()));
			resize(bin_image, bin_image, Size(rect5.Width(), rect5.Height()));
			imshow("video", image);
			imshow("video2", frame);
			imshow("video3", background);
			imshow("video4", mov_target);
			imshow("video5", bin_image);

		}
	}
	CDialogEx::OnTimer(nIDEvent);
}



void CbigDlg::OnBnClickedButton4()
{
	// 关闭按钮直接终止程序
	exit(0);
}


void CbigDlg::OnBnClickedButton5()
{
	// 构造打开文件对话框
	// CFileDialog构造函数：
	// 第一个参数为显示打开对话框还是保存对话框（TRUE打开）
	// 第二个参数为指定默认的文件扩展名
	// 第三个参数为指定默认文件名
	// 第四个参数为一些特定的风格
	// 第五个参数为可供选择的文件类型
	// 第六个参数为父窗口指针
	TCHAR szFilter[] = _T("视频文件(*.mp4)|*.mp4|所有文件(*.*)|*.*||");
	CFileDialog fileDlg(TRUE, NULL, NULL, 0, szFilter, this);

	// 显示打开文件对话框
	if (IDOK == fileDlg.DoModal())
	{
		// 如果点击了打开按钮，则将选择的文件路径显示到编辑框（path）里  
		path = fileDlg.GetPathName();
		SetDlgItemText(IDC_EDIT2, path);
	}
}
