![image-20231225112058497](C:\Users\86178\AppData\Roaming\Typora\typora-user-images\image-20231225112058497.png)

1：MFC与OpenCv窗口连接

![image-20231225112512610](C:\Users\86178\AppData\Roaming\Typora\typora-user-images\image-20231225112512610.png)

2：获取边缘函数(将噪声或者非目标边缘删除)

![image-20231225112858107](C:\Users\86178\AppData\Roaming\Typora\typora-user-images\image-20231225112858107.png)

3：计算中心点（四舍五入方法）

![image-20231225113004422](C:\Users\86178\AppData\Roaming\Typora\typora-user-images\image-20231225113004422.png)

4：暂停原理（播放->定时器1； 识别->定时器2）

5：实现步骤

​	5.1  初始化13*13的矩形结构元素(形状，大小，锚点：默认（-1，-1）)

​			Mat element = getStructuringElement(MORPH_RECT, Size(13, 13));

​			线性缩小帧为原来的1/9

​			resize(frame, image, Size(frame.cols / 3, frame.rows / 3), INTER_LINEAR);

​	5.2  混合高斯模型的MOG2方法

![image-20231225122013073](C:\Users\86178\AppData\Roaming\Typora\typora-user-images\image-20231225122013073.png)

​			默认检测阴影，一次读500帧，方差阈值为500

​			![image-20231225122320525](C:\Users\86178\AppData\Roaming\Typora\typora-user-images\image-20231225122320525.png)

​			获取前景掩码图像（默认学习率-1 算法自动更新；0时不更新；1时逐帧更新）

​			使用BackgroundSubtractorMOG2改进方法，实现了自适应高斯混合模型参数的更新，增强了复杂场景背景检测的性能

​			pBgmodel->apply(image, bin_image);

​			高斯滤波（5*5内核，标准差为0，边界平推方式为不包含边界值倒序平推）

​			GaussianBlur(bin_image, bin_image, Size(5, 5), 0);

​			闭运算

​			阈值二值化处理(小于10 -> 0；大于10 -> 255)

​			threshold(bin_image, bin_image, 10, 255, THRESH_BINARY);

​			创建运动目标图像

```c++
	mov_target.create(image.size(), image.type());
	// 将mov_target所有像素置为0
	mov_target = Scalar::all(0);
	// 将原视频image复制到mov_target中，并且image对应bin_image中像素值为0的像素点都不会复制到mov_target中
	// 相当于提取运动目标
	image.copyTo(mov_target, bin_image);
```

​			MOG2方法获取背景图像

​			pBgmodel->getBackgroundImage(background);

​	5.3  轮廓检测

```c++
// 使用findContours函数检测轮廓，contours中有多少元素，就有多少个轮廓
// 第四个参数CV_RETR_EXTERNAL表示只检测外轮廓，第五个参数CV_CHAIN_APPROX_NONE保存物体边界所有连续的轮廓点，第六个参数为偏移量
	findContours(bin_image, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));
```

![image-20231225124206692](C:\Users\86178\AppData\Roaming\Typora\typora-user-images\image-20231225124206692.png)

​	不符合轮廓删除

​	轮廓面积降序排序

​	去除伪目标(轮廓面积小于最大的1/5)并计算最小外接矩形和绘制绿色

​	对固定区域进行计数（0.4*rows为上边缘 高度150px）

​	计数方法：

​		1、遍历所有轮廓获取中心点

​		2、判断是否进入感兴趣区域

​		3、判断车辆是否重复计数（2帧中心点小于10）

​		4、未计数车辆进行分配ID并初始化轨迹

​		![image-20231225125846186](C:\Users\86178\AppData\Roaming\Typora\typora-user-images\image-20231225125846186.png)

![image-20231225125926406](C:\Users\86178\AppData\Roaming\Typora\typora-user-images\image-20231225125926406.png)

![image-20231225130617124](C:\Users\86178\AppData\Roaming\Typora\typora-user-images\image-20231225130617124.png)

​	5、轨迹更新，中心点加入

​	6、绘制已经计算过的轨迹（遍历轨迹map，根据中心点数组绘制圆点）

![image-20231225130807231](C:\Users\86178\AppData\Roaming\Typora\typora-user-images\image-20231225130807231.png)

​		最后，画面自适应