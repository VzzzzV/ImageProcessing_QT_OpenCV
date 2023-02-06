#include "Qt_OpenCV.h"
#include<iostream>
#pragma execution_character_set("utf-8")//设置字码，不然中文消息报错
double generateGaussianNoise(double mu, double sigma);
Qt_OpenCV::Qt_OpenCV(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//Qt_OpenCV::connect(ui.menu_src->menuAction(), SIGNAL(triggered()), this, SLOT(Menu_Src()));
	ui.menuBar->addAction(QString::fromUtf8("灰度化"), this, SLOT(Menu_Action()));
	ui.menuBar->addAction(QString::fromUtf8("原图像"), this, SLOT(Menu_Action()));
	ui.menuBar->addAction(QString::fromUtf8("直方图"), this, SLOT(Menu_Action()));
	ui.menuBar->addAction(QString::fromUtf8("无种子区域生长"), this, SLOT(Menu_Action()));

	ui.centralWidget->setWindowFlag(Qt::FramelessWindowHint);
	ui.scrollAreaWidgetContents->setWindowFlag(Qt::FramelessWindowHint);
	ui.scrollArea->setWindowFlag(Qt::FramelessWindowHint);

	ui.menuBar->setStyleSheet(QString("QMenuBar{padding:0px;margin:0px;border:0px;}"));
	this->setStyleSheet(QString("QWidget{padding:0px;margin:0px;border:0px;background-color:lightgrey;}"));
	ui.centralWidget->setStyleSheet(QString("QWidget{padding:0px;margin:0px;border:0px;background-color:lightgrey;}"));
	ui.label->setStyleSheet(QString("QLabel{padding:0px;margin:0px;border:0px;background-color:lightgrey;}"));
	ui.scrollArea->setStyleSheet(QString("QScrollArea{padding:0px;margin:0px;border:0px;background-color:lightgrey;}"));
	ui.scrollAreaWidgetContents->setStyleSheet(QString("QWidget{padding:0px;margin:0px;border:0px;background-color:lightgrey;}"));

	ui.scrollAreaWidgetContents->move(0, 0);
	ui.scrollArea->move(0, 0);
	ui.label->move(0, 0);

}
bool bool_border(Mat Mark, Mat border, int x, int y)//判断点(x,y)是否是属于边界
{
	int row = Mark.rows, col = Mark.cols;
	if (Mark.at<uchar>(y, x) == 255)
		return false;//如果该点已经被标则不为边界
	if (x > 0 && y > 0 && y < row - 1 && x < col - 1)//不在边缘
		if (
			(Mark.at<uchar>(y - 1, x) == 0 || Mark.at<uchar>(y + 1, x) == 0 || Mark.at<uchar>(y, x - 1) == 0 || Mark.at<uchar>(y, x + 1) == 0)//四周至少有一个点为0
			&&
			(Mark.at<uchar>(y - 1, x) != 0 || Mark.at<uchar>(y + 1, x) != 0 || Mark.at<uchar>(y, x - 1) != 0 || Mark.at<uchar>(y, x + 1) != 0)//四周至少有一个点不为0
			|| (Mark.at<uchar>(y - 1, x) != 0 && Mark.at<uchar>(y + 1, x) != 0 && Mark.at<uchar>(y, x - 1) != 0 && Mark.at<uchar>(y, x + 1) != 0)//或者四周都为255时也边界
			)
			return true;
		else
			return false;
	if (y == 0 && x > 0 && x < col - 1)//上边缘--1
		if (
			(Mark.at<uchar>(y + 1, x) == 0 || Mark.at<uchar>(y, x - 1) == 0 || Mark.at<uchar>(y, x + 1) == 0)//四周至少有一个点为0
			&&
			(Mark.at<uchar>(y + 1, x) != 0 || Mark.at<uchar>(y, x - 1) != 0 || Mark.at<uchar>(y, x + 1) != 0)//四周至少有一个点不为0
			|| (  Mark.at<uchar>(y + 1, x) != 0 && Mark.at<uchar>(y, x - 1) != 0 && Mark.at<uchar>(y, x + 1) != 0)//或者四周都为255时也边界


			)
			return true;
		else
			return false;
	if (x > 0 && y == row - 1 && x < col - 1)//右边缘--2
		if (
			(Mark.at<uchar>(y - 1, x) == 0 || Mark.at<uchar>(y, x - 1) == 0 || Mark.at<uchar>(y, x + 1) == 0)//四周至少有一个点为0
			&&
			(Mark.at<uchar>(y - 1, x) != 0 || Mark.at<uchar>(y, x - 1) != 0 || Mark.at<uchar>(y, x + 1) != 0)//四周至少有一个点不为0
			|| (Mark.at<uchar>(y - 1, x) != 0  && Mark.at<uchar>(y, x - 1) != 0 && Mark.at<uchar>(y, x + 1) != 0)//或者四周都为255时也边界
			)
			return true;
		else
			return false;
	if (y > 0 && x == 0 && y < row - 1)//上边缘--3
		if (
			(Mark.at<uchar>(y - 1, x) == 0 || Mark.at<uchar>(y + 1, x) == 0 || Mark.at<uchar>(y, x + 1) == 0)//四周至少有一个点为0
			&&
			(Mark.at<uchar>(y - 1, x) != 0 || Mark.at<uchar>(y + 1, x) != 0 || Mark.at<uchar>(y, x + 1) != 0)//四周至少有一个点不为0
			|| (Mark.at<uchar>(y - 1, x) != 0 && Mark.at<uchar>(y + 1, x) != 0  && Mark.at<uchar>(y, x + 1) != 0)//或者四周都为255时也边界
			)
			return true;
		else
			return false;
	if (y > 0 && y < row - 1 && x == col - 1)//下边缘--4
		if (
			(Mark.at<uchar>(y - 1, x) == 0 || Mark.at<uchar>(y + 1, x) == 0 || Mark.at<uchar>(y, x - 1) == 0)//四周至少有一个点为0
			&&
			(Mark.at<uchar>(y - 1, x) != 0 || Mark.at<uchar>(y + 1, x) != 0 || Mark.at<uchar>(y, x - 1) != 0)//四周至少有一个点不为0
			|| (Mark.at<uchar>(y - 1, x) != 0 && Mark.at<uchar>(y + 1, x) != 0 && Mark.at<uchar>(y, x - 1) != 0 )//或者四周都为255时也边界
			)
			return true;
		else
			return false;
	if (x == 0 && y == 0)//左上角--5
		if (
			(Mark.at<uchar>(y + 1, x) == 0 || Mark.at<uchar>(y, x + 1) == 0)//四周至少有一个点为0
			&&
			(Mark.at<uchar>(y + 1, x) != 0 || Mark.at<uchar>(y, x + 1) != 0)//四周至少有一个点不为0
			|| ( Mark.at<uchar>(y + 1, x) != 0  && Mark.at<uchar>(y, x + 1) != 0)//或者四周都为255时也边界
			)
			return true;
		else
			return false;
	if (x == 0 && y == row - 1)//右上角--6
		if (
			(Mark.at<uchar>(y - 1, x) == 0 || Mark.at<uchar>(y, x + 1) == 0)//四周至少有一个点为0
			&&
			(Mark.at<uchar>(y - 1, x) != 0 || Mark.at<uchar>(y, x + 1) != 0)//四周至少有一个点不为0
			|| (Mark.at<uchar>(y - 1, x) != 0  && Mark.at<uchar>(y, x + 1) != 0)//或者四周都为255时也边界
			)
			return true;
		else
			return false;
	if (y == 0 && x == col - 1)//左下角--7
		if (
			(Mark.at<uchar>(y + 1, x) == 0 || Mark.at<uchar>(y, x - 1) == 0)//四周至少有一个点为0
			&&
			(Mark.at<uchar>(y + 1, x) != 0 || Mark.at<uchar>(y, x - 1) != 0)//四周至少有一个点不为0
			|| (  Mark.at<uchar>(y + 1, x) != 0 && Mark.at<uchar>(y, x - 1) != 0 )//或者四周都为255时也边界
			)
			return true;
		else
			return false;
	if (y == row - 1 && x == col - 1)//右下角--8
		if (
			(Mark.at<uchar>(y - 1, x) == 0 || Mark.at<uchar>(y, x - 1) == 0)//四周至少有一个点为0
			&&
			(Mark.at<uchar>(y - 1, x) != 0 || Mark.at<uchar>(y, x - 1) != 0)//四周至少有一个点不为0
			|| (Mark.at<uchar>(y - 1, x) != 0  && Mark.at<uchar>(y, x - 1) != 0 )//或者四周都为255时也边界
			)
			return true;
		else
			return false;

};
void cal_border(Mat Mark, Mat border, int new_y, int new_x)
//运行此函数时，Mark中已经添加点(new_x,new_y)为Mark中新的一个点，且修改了(new_x,new_y)不为brder
//该函数计算该点的变化对其周围四个点的border属性影响
{
	int row = Mark.rows, col = Mark.cols;
	if (new_x != 0)//不为最左侧的点
	{
		if (bool_border(Mark, border, new_x - 1, new_y))
			border.at<uchar>(new_y, new_x - 1) = 255;
		else
			border.at<uchar>(new_y, new_x - 1) = 0;
	}
	//左侧点
	//std::cout << "1" << std::endl;
	if (new_x != row - 1)//不为最右侧的点
	{
		if (bool_border(Mark, border, new_x + 1, new_y))
			border.at<uchar>(new_y, new_x + 1) = 255;
		else
			border.at<uchar>(new_y, new_x + 1) = 0;
	}
	//右侧点
	//std::cout << "2" << std::endl;
	if (new_y != 0)//不为最上侧的点
	{
		if (bool_border(Mark, border, new_x, new_y - 1))
			border.at<uchar>(new_y - 1, new_x) = 255;
		else
			border.at<uchar>(new_y - 1, new_x) = 0;
	}
	//上侧点
	//std::cout << "3" << std::endl;
	if (new_y != col - 1)//不为最下侧的点
	{
		if (bool_border(Mark, border, new_x, new_y + 1))
			border.at<uchar>(new_y + 1, new_x) = 255;
		else
			border.at<uchar>(new_y + 1, new_x) = 0;
	}
	//下侧点
	//std::cout << "4" << std::endl;
};
void Qt_OpenCV::Menu_Action(QAction* act) {
	if (act->text() == "打开")
	{
		QString path = QFileDialog::getOpenFileName(this, "打开文件", "/", "ALL(*.*);;BMP(*.bmp);;JPG(*.jpg)");
		if (!path.isEmpty())
		{
			qDebug() << path;
			frame_bgr = imread(path.toLocal8Bit().constData());//imread仅支持ASCII码，而QT需要UTF8否则中文报错
			if (frame_bgr.empty())
				return;

			cvtColor(frame_bgr, frame_rgb, COLOR_BGR2RGB);    //获得QImage格式的图片
			frame_rgb.copyTo(work);

			QImage img((const uchar*)frame_rgb.data, frame_rgb.cols, frame_rgb.rows, frame_rgb.cols*(frame_rgb.channels()), QImage::Format_RGB888);
			ui.scrollAreaWidgetContents->setMinimumSize(QSize(img.width(), img.height()));
			ui.label->resize(QSize(img.width(), img.height()));
			ui.label->setPixmap(QPixmap::fromImage(img));
			img.~QImage();
		}
	}
	else if (act->text() == "保存")
	{
		imwrite("result.bmp", work);
	}
	else if (act->text() == "原图像")
	{
		if (work.empty())
		{
			return;
		}
		frame_rgb.copyTo(work);
		QImage img((const uchar*)work.data, work.cols, work.rows, work.cols*(work.channels()), QImage::Format_RGB888);
		ui.scrollAreaWidgetContents->setMinimumSize(QSize(img.width(), img.height()));
		ui.label->setGeometry(QRect(0, 0, img.width(), img.height()));
		ui.label->setPixmap(QPixmap::fromImage(img));
		img.~QImage();
	}
	else if (act->text() == "灰度化")
	{
		if (work.empty())
			return;
		Mat temp;
		if (work.type() != 0)//判断图像类型
		{
			cvtColor(work, temp, CV_RGB2GRAY);
			temp.copyTo(work);
		}
		temp.release();
		QImage img((const uchar*)work.data, work.cols, work.rows, work.cols*(work.channels()), QImage::Format_Indexed8);
		ui.scrollAreaWidgetContents->setMinimumSize(QSize(img.width(), img.height()));
		ui.label->resize(QSize(img.width(), img.height()));
		ui.label->setPixmap(QPixmap::fromImage(img));
		img.~QImage();

	}
	else if (act->text() == "大津算法")
	{
		if (work.empty())
		{
			return;
		}
		if (work.type() != 0)
			cvtColor(work, work, CV_RGB2GRAY);
		Mat temp(Size(work.cols, work.rows), 0);
		threshold(work, temp, 0, 255, CV_THRESH_OTSU);
		/*	IplImage tmp = IplImage(work);
		CvArr* arr = (CvArr*)&tmp;
		CvArr* arr1 = arr;
		cvConvertScale(arr, arr1, 1.0, 0);
		save = cv::cvarrToMat(arr1);

		int T, t, n0, n1;
		double u, u0, u1, w0, w1, sum0, sum1, G = 0, g;
		for (t = 0; t<256; t++)
		{

		n0 = 0;
		n1 = 0;
		sum0 = 0;
		sum1 = 0;

		for (int i = 0; i<work.rows; i++)
		for (int j = 0; j<work.cols; j++)
		if (work.at<uchar>(i, j) <= t)
		{
		n0 = n0++;
		sum0 = sum0 + work.at<uchar>(i, j);
		}
		else
		{
		n1 = n1++;
		sum1 = sum1 + work.at<uchar>(i, j);
		}
		if (n0 == 0 || n1 == 0)
		continue;
		w0 = n0 * 1.0 / (n0 + n1);
		w1 = n1 * 1.0 / (n0 + n1);
		u0 = sum0 / n0;
		u1 = sum1 / n1;
		u = w0 * u0 + w1 * u1;
		g = w0 * (u0 - u)*(u0 - u) + w1 * (u1 - u)*(u1 - u);
		//g=w0*w1*(u0-u1)*(u0-u1)
		if (G<g)
		{
		G = g;
		T = t;
		}
		}
		for (int i = 0; i<work.rows; i++)
		for (int j = 0; j<work.cols; j++)
		if (work.at<uchar>(i, j) <= T)
		save.at<uchar>(i, j) = 0;
		else
		save.at<uchar>(i, j) = 255;
		*/
		temp.copyTo(work);
		temp.release();
		QImage img((const uchar*)work.data, work.cols, work.rows, work.cols*(work.channels()), QImage::Format_Indexed8);
		ui.scrollAreaWidgetContents->setMinimumSize(QSize(img.width(), img.height()));
		ui.label->resize(QSize(img.width(), img.height()));
		ui.label->setPixmap(QPixmap::fromImage(img));
		img.~QImage();
		qDebug() << "大津算法";
	}
	else if (act->text() == "门阈值")
	{
		if (work.empty())
		{
			return;
		}
		if (work.type() != 0)
			cvtColor(work, work, CV_RGB2GRAY);
		Mat temp(Size(work.cols, work.rows), 0);


		int T = 100;


		threshold(work, temp, T, 255, 0);
		//threshold(src,dst,threshold,max,type)
		/*
		for (int i = 0; i<work.rows; i++)
			for (int j = 0; j<work.cols; j++)
				if (work.at<uchar>(i, j) <= T)
					save.at<uchar>(i, j) = 0;
				else
					save.at<uchar>(i, j) = 255;
		*/

		temp.copyTo(work);
		temp.release();
		QImage img((const uchar*)work.data, work.cols, work.rows, work.cols*(work.channels()), QImage::Format_Indexed8);
		ui.scrollAreaWidgetContents->setMinimumSize(QSize(img.width(), img.height()));
		ui.label->resize(QSize(img.width(), img.height()));
		//ui.label->setGeometry(QRect(20,20, img.width(), img.height()));
		ui.label->setPixmap(QPixmap::fromImage(img));
		img.~QImage();
		qDebug() << "门阈值";
	}
	else if (act->text() == "椒盐噪声")
	{
		if (work.empty())
		{
			return;
		}
		if (work.type() != 0)
			cvtColor(work, work, CV_RGB2GRAY);
		Mat temp;
		work.copyTo(temp);
		for (int i = 0; i < work.rows*work.cols/10; i++)
		{

			if (i % 2 == 0)
				temp.at<uchar>(rand() % work.rows, rand() % work.cols) = 255;
			else
				temp.at<uchar>(rand() % work.rows, rand() % work.cols) = 0;
		}
		temp.copyTo(work);
		temp.release();
		QImage img((const uchar*)work.data, work.cols, work.rows, work.cols*(work.channels()), QImage::Format_Indexed8);
		ui.scrollAreaWidgetContents->setMinimumSize(QSize(img.width(), img.height()));
		ui.label->resize(QSize(img.width(), img.height()));
		ui.label->setPixmap(QPixmap::fromImage(img));
		img.~QImage();
	}
	else if (act->text() == "白噪声")
	{
		if (work.empty())
		{
			return;
		}
		if (work.type() != 0)
			cvtColor(work, work, CV_RGB2GRAY);
		Mat temp;
		work.copyTo(temp);
		for (int i = 0; i < work.rows*work.cols / 10; i++)
			temp.at<uchar>(rand() % work.rows, rand() % work.cols) = 255;

		temp.copyTo(work);
		temp.release();
		QImage img((const uchar*)work.data, work.cols, work.rows, work.cols*(work.channels()), QImage::Format_Indexed8);
		ui.scrollAreaWidgetContents->setMinimumSize(QSize(img.width(), img.height()));
		ui.label->resize(QSize(img.width(), img.height()));
		ui.label->setPixmap(QPixmap::fromImage(img));
		img.~QImage();
	}
	else if (act->text() == "高斯噪声")
	{
		if (work.empty())
		{
			return;
		}
		if (work.type() != 0)
			cvtColor(work, work, CV_RGB2GRAY);
		Mat temp;
		work.copyTo(temp);
		for (int i = 0; i < work.rows*work.cols / 5; i++)
		{
			int noise = generateGaussianNoise(0, 30);
			int x = rand() % work.rows;
			int y = rand() % work.cols;
			if (temp.at<uchar>(x, y) + noise > 255)
				temp.at<uchar>(x, y) = 255;
			else if (temp.at<uchar>(x, y) + noise > 255 < 0)
				temp.at<uchar>(x, y) = 0;
			else
				temp.at<uchar>(x, y) += noise;
		}
		temp.copyTo(work);
		temp.release();
		QImage img((const uchar*)work.data, work.cols, work.rows, work.cols*(work.channels()), QImage::Format_Indexed8);
		ui.scrollAreaWidgetContents->setMinimumSize(QSize(img.width(), img.height()));
		ui.label->resize(QSize(img.width(), img.height()));
		ui.label->setPixmap(QPixmap::fromImage(img));
		img.~QImage();
	}
	else if (act->text() == "均值滤波3X3")
	{
		if (work.empty())
		{
			return;
		}
		if (work.type() != 0)
			cvtColor(work, work, CV_RGB2GRAY);
		Mat temp;
		work.copyTo(temp);

		for (int i = 1; i < work.rows - 1; i++)
			for (int j = 1; j < work.cols - 1; j++)
				work.at<uchar>(i, j) =
				(int)((temp.at<uchar>(i - 1, j - 1) + temp.at<uchar>(i - 1, j) + temp.at<uchar>(i - 1, j + 1)
					+ temp.at<uchar>(i, j - 1) + temp.at<uchar>(i, j) + temp.at<uchar>(i, j + 1)
					+ temp.at<uchar>(i + 1, j - 1) + temp.at<uchar>(i, j) + temp.at<uchar>(i + 1, j + 1)) / 9);

		//temp.copyTo(work);
		temp.release();
		QImage img((const uchar*)work.data, work.cols, work.rows, work.cols*(work.channels()), QImage::Format_Indexed8);
		ui.scrollAreaWidgetContents->setMinimumSize(QSize(img.width(), img.height()));
		ui.label->resize(QSize(img.width(), img.height()));
		ui.label->setPixmap(QPixmap::fromImage(img));
		img.~QImage();
	}
	else if (act->text() == "均值滤波5X5")
	{
		if (work.empty())
		{
			return;
		}
		if (work.type() != 0)
			cvtColor(work, work, CV_RGB2GRAY);
		Mat temp;
		work.copyTo(temp);

		for (int i = 2; i < work.rows - 2; i++)
			for (int j = 2; j < work.cols - 2; j++)
				work.at<uchar>(i, j) =
				(int)((
					temp.at<uchar>(i - 2, j - 2) + temp.at<uchar>(i - 2, j - 1) + temp.at<uchar>(i - 2, j) + temp.at<uchar>(i - 2, j + 1) + temp.at<uchar>(i - 2, j + 2) +
					temp.at<uchar>(i - 1, j - 2) + temp.at<uchar>(i - 1, j - 1) + temp.at<uchar>(i - 1, j) + temp.at<uchar>(i - 1, j + 1) + temp.at<uchar>(i - 1, j + 2) +
					temp.at<uchar>(i, j - 2) + temp.at<uchar>(i, j - 1) + temp.at<uchar>(i, j) + temp.at<uchar>(i, j + 1) + temp.at<uchar>(i, j + 2) +
					temp.at<uchar>(i + 1, j - 2) + temp.at<uchar>(i + 1, j - 1) + temp.at<uchar>(i + 1, j) + temp.at<uchar>(i + 1, j + 1) + temp.at<uchar>(i + 1, j + 2) +
					temp.at<uchar>(i + 2, j - 2) + temp.at<uchar>(i + 2, j - 1) + temp.at<uchar>(i + 2, j) + temp.at<uchar>(i + 2, j + 1) + temp.at<uchar>(i + 2, j + 2))
					/ 25);

		temp.release();
		QImage img((const uchar*)work.data, work.cols, work.rows, work.cols*(work.channels()), QImage::Format_Indexed8);
		ui.scrollAreaWidgetContents->setMinimumSize(QSize(img.width(), img.height()));
		ui.label->resize(QSize(img.width(), img.height()));
		ui.label->setPixmap(QPixmap::fromImage(img));
		img.~QImage();
	}
	else if (act->text() == "中值滤波3X3")
	{
		if (work.empty())
		{
			return;
		}
		if (work.type() != 0)
			cvtColor(work, work, CV_RGB2GRAY);
		Mat temp;
		work.copyTo(temp);

		for (int i = 1; i < work.rows - 1; i++)
			for (int j = 1; j < work.cols - 1; j++)
			{
				int pix[8];
				pix[0] = temp.at<uchar>(i - 1, j - 1);
				pix[1] = temp.at<uchar>(i - 1, j);
				pix[2] = temp.at<uchar>(i - 1, j + 1);
				pix[3] = temp.at<uchar>(i, j - 1);
				pix[4] = temp.at<uchar>(i, j);
				pix[5] = temp.at<uchar>(i, j + 1);
				pix[6] = temp.at<uchar>(i + 1, j - 1);
				pix[7] = temp.at<uchar>(i + 1, j);
				pix[8] = temp.at<uchar>(i + 1, j + 1);
				for (int k = 0; k < 9; k++)
				{
					for (int l = 0; l < 8 - k; l++)
					{
						if (pix[l] > pix[l + 1])
						{
							int temp = pix[l];
							pix[l] = pix[l + 1];
							pix[l + 1] = temp;
						}
					}
				}
				work.at<uchar>(i, j) = int(pix[4]);
			}

		temp.release();
		QImage img((const uchar*)work.data, work.cols, work.rows, work.cols*(work.channels()), QImage::Format_Indexed8);
		ui.scrollAreaWidgetContents->setMinimumSize(QSize(img.width(), img.height()));
		ui.label->resize(QSize(img.width(), img.height()));
		ui.label->setPixmap(QPixmap::fromImage(img));
		img.~QImage();
	}
	else if (act->text() == "中值滤波5X5")
	{
		if (work.empty())
		{
			return;
		}
		if (work.type() != 0)
			cvtColor(work, work, CV_RGB2GRAY);
		Mat temp;
		work.copyTo(temp);

		for (int i = 2; i < work.rows - 2; i++)
			for (int j = 2; j < work.cols - 2; j++)
			{
				int pix[25];
				pix[0] = temp.at<uchar>(i - 2, j - 2);
				pix[1] = temp.at<uchar>(i - 2, j - 1);
				pix[2] = temp.at<uchar>(i - 2, j);
				pix[3] = temp.at<uchar>(i - 2, j + 1);
				pix[4] = temp.at<uchar>(i - 2, j + 2);
				pix[5] = temp.at<uchar>(i - 1, j - 2);
				pix[6] = temp.at<uchar>(i - 1, j - 1);
				pix[7] = temp.at<uchar>(i - 1, j);
				pix[8] = temp.at<uchar>(i - 1, j + 1);
				pix[9] = temp.at<uchar>(i - 1, j + 2);
				pix[10] = temp.at<uchar>(i, j - 2);
				pix[11] = temp.at<uchar>(i, j - 1);
				pix[12] = temp.at<uchar>(i, j);
				pix[13] = temp.at<uchar>(i, j + 1);
				pix[14] = temp.at<uchar>(i, j + 2);
				pix[15] = temp.at<uchar>(i + 1, j - 2);
				pix[16] = temp.at<uchar>(i + 1, j - 1);
				pix[17] = temp.at<uchar>(i + 1, j);
				pix[18] = temp.at<uchar>(i + 1, j + 1);
				pix[19] = temp.at<uchar>(i + 1, j + 2);
				pix[20] = temp.at<uchar>(i + 2, j - 2);
				pix[21] = temp.at<uchar>(i + 2, j - 1);
				pix[22] = temp.at<uchar>(i + 2, j);
				pix[23] = temp.at<uchar>(i + 2, j + 1);
				pix[24] = temp.at<uchar>(i + 2, j + 2);
				for (int k = 0; k < 25; k++)
				{
					for (int l = 0; l < 24 - k; l++)
					{
						if (pix[l] > pix[l + 1])
						{
							int temp = pix[l];
							pix[l] = pix[l + 1];
							pix[l + 1] = temp;
						}
					}
				}
				work.at<uchar>(i, j) = int(pix[12]);
			}

		//temp.copyTo(work);
		temp.release();
		QImage img((const uchar*)work.data, work.cols, work.rows, work.cols*(work.channels()), QImage::Format_Indexed8);
		ui.scrollAreaWidgetContents->setMinimumSize(QSize(img.width(), img.height()));
		ui.label->resize(QSize(img.width(), img.height()));
		ui.label->setPixmap(QPixmap::fromImage(img));
		img.~QImage();
	}
	else if (act->text() == "高斯滤波3X3")
	{
		if (work.empty())
		{
			return;
		}
		if (work.type() != 0)
			cvtColor(work, work, CV_RGB2GRAY);
		Mat temp;
		work.copyTo(temp);

		for (int i = 1; i < work.rows - 1; i++)
			for (int j = 1; j < work.cols - 1; j++)
				work.at<uchar>(i, j) =
				(int)((temp.at<uchar>(i - 1, j - 1) + temp.at<uchar>(i - 1, j) * 2 + temp.at<uchar>(i - 1, j + 1)
					+ temp.at<uchar>(i, j - 1) * 2 + temp.at<uchar>(i, j) * 4 + temp.at<uchar>(i, j + 1) * 2
					+ temp.at<uchar>(i + 1, j - 1) + temp.at<uchar>(i, j) * 2 + temp.at<uchar>(i + 1, j + 1)) / 16);

		//temp.copyTo(work);
		temp.release();
		QImage img((const uchar*)work.data, work.cols, work.rows, work.cols*(work.channels()), QImage::Format_Indexed8);
		ui.scrollAreaWidgetContents->setMinimumSize(QSize(img.width(), img.height()));
		ui.label->resize(QSize(img.width(), img.height()));
		ui.label->setPixmap(QPixmap::fromImage(img));
		img.~QImage();
	}
	else if (act->text() == "高斯滤波5X5")
	{
		if (work.empty())
		{
			return;
		}
		if (work.type() != 0)
			cvtColor(work, work, CV_RGB2GRAY);
		Mat temp;
		work.copyTo(temp);

		for (int i = 2; i < work.rows - 2; i++)
			for (int j = 2; j < work.cols - 2; j++)
				work.at<uchar>(i, j) =
				(int)((
					temp.at<uchar>(i - 2, j - 2) + temp.at<uchar>(i - 2, j - 1) * 4 + temp.at<uchar>(i - 2, j) * 7 + temp.at<uchar>(i - 2, j + 1) * 4 + temp.at<uchar>(i - 2, j + 2) +
					temp.at<uchar>(i - 1, j - 2) * 4 + temp.at<uchar>(i - 1, j - 1) * 16 + temp.at<uchar>(i - 1, j) * 26 + temp.at<uchar>(i - 1, j + 1) * 16 + temp.at<uchar>(i - 1, j + 2) * 4 +
					temp.at<uchar>(i, j - 2) * 7 + temp.at<uchar>(i, j - 1) * 26 + temp.at<uchar>(i, j) * 41 + temp.at<uchar>(i, j + 1) * 26 + temp.at<uchar>(i, j + 2) * 7 +
					temp.at<uchar>(i + 1, j - 2) * 4 + temp.at<uchar>(i + 1, j - 1) * 16 + temp.at<uchar>(i + 1, j) * 26 + temp.at<uchar>(i + 1, j + 1) * 16 + temp.at<uchar>(i + 1, j + 2) * 4 +
					temp.at<uchar>(i + 2, j - 2) + temp.at<uchar>(i + 2, j - 1) * 4 + temp.at<uchar>(i + 2, j) * 7 + temp.at<uchar>(i + 2, j + 1) * 4 + temp.at<uchar>(i + 2, j + 2))
					/ 273);

		temp.release();
		QImage img((const uchar*)work.data, work.cols, work.rows, work.cols*(work.channels()), QImage::Format_Indexed8);
		ui.scrollAreaWidgetContents->setMinimumSize(QSize(img.width(), img.height()));
		ui.label->resize(QSize(img.width(), img.height()));
		ui.label->setPixmap(QPixmap::fromImage(img));
		img.~QImage();
	}
	else if (act->text() == "直方图")
	{
		if (work.empty())
		{
			return;
		}
		if (work.type() != 0)
			cvtColor(work, work, CV_RGB2GRAY);

		Mat temp(Size(512, 512), 0);
		int gray[256] = { 0 };
		for (int i = 0; i < work.rows; i++)
			for (int j = 0; j < work.cols; j++)
				gray[int(work.at<uchar>(i, j))] ++;
		int max = 0;//用来存储量度出现最多的次数
		for (int i = 0; i < 256; i++)
			if (gray[i] > max)
				max = gray[i];
		/*背景设为黑色*/
		for (int i = 0; i < 512; i++)
			for (int j = 0; j < 512; j++)
				temp.at < uchar >(i, j) = 0;
		/*画图*/
		for (int i = 0; i < 256; i++)
			for (int j = 0; j < ((gray[i] * 1.0 / max) * 511); j++)
			{
				temp.at < uchar >(511 - j, i * 2) = 255;
				temp.at < uchar >(511 - j, i * 2 + 1) = 255;
			}
		imshow("Gray histogram ", temp);
		temp.release();
	}
	else if (act->text() == "直方图均衡")
	{
		if (work.empty())
		{
			return;
		}
		if (work.type() != 0)
			cvtColor(work, work, CV_RGB2GRAY);
		Mat temp;
		work.copyTo(temp);
		//equalizeHist(work, temp);

		int histogram[256] = { 0 };
		for (int i = 0; i < work.rows; i++)
			for (int j = 0; j < work.cols; j++)
				histogram[int(work.at<uchar>(i, j))] ++;

		int numberOfPixel = work.rows*work.cols;
		int LUT[256];
		LUT[0] = 1.0*histogram[0] / numberOfPixel * 255;
		int sum = histogram[0];
		for (int i = 1; i <= 255; ++i)
		{
			sum += histogram[i];
			LUT[i] = 1.0*sum / numberOfPixel * 255;
		}
		// 灰度变换
		uchar *dataOfSrc = work.data;
		uchar *dataOfDst = temp.data;
		for (int i = 0; i <= numberOfPixel - 1; ++i)
			dataOfDst[i] = LUT[dataOfSrc[i]];

		temp.copyTo(work);
		temp.release();
		QImage img((const uchar*)work.data, work.cols, work.rows, work.cols*(work.channels()), QImage::Format_Indexed8);
		ui.scrollAreaWidgetContents->setMinimumSize(QSize(img.width(), img.height()));
		ui.label->resize(QSize(img.width(), img.height()));
		ui.label->setPixmap(QPixmap::fromImage(img));
		img.~QImage();
	}
	else if (act->text() == "SNR")
	{
		if (work.empty())
		{
			return;
		}
		if (work.type() != 0)
			cvtColor(work, work, CV_RGB2GRAY);
		double  SNR = 0;
		Mat temp;//当前图像
		work.copyTo(temp);
		Mat temp_src;//原图像
		frame_rgb.copyTo(temp_src);
		if (temp_src.type() != 0)
			cvtColor(temp_src, temp_src, CV_RGB2GRAY);

		int *sub, *sq;
		double  s_PSNR = 0, s_SNR = 0;
		sub = new int[work.cols*work.rows];
		sq = new int[work.cols*work.rows];
		for (int i = 0; i < work.rows*work.cols; i++)
		{
			sub[i] = (int(temp.data[i]) - int(temp_src.data[i]))*(int(temp.data[i]) - int(temp_src.data[i]));
			s_PSNR += sub[i];
			sq[i] = (int)temp_src.data[i] * (int)temp_src.data[i];
			s_SNR += sq[i];
		}

		SNR = 10.0*log10(s_SNR / s_PSNR);
		temp_src.release();
		temp.release();
		//Scalar s= sum(sub);         // 计算所有像素之和
		//double sse = s.val[0] + s.val[1] + s.val[2]; // 如果是彩色图像计算通道和

		QString dlgTitle = "SNR";
		QString strInfo = QString::number(SNR, 10, 4);
		QMessageBox::about(this, dlgTitle, strInfo);

	}
	else if (act->text() == "PSNR")
	{
		if (work.empty())
		{
			return;
		}
		if (work.type() != 0)
			cvtColor(work, work, CV_RGB2GRAY);
		double PSNR = 0;
		Mat temp;//当前图像
		work.copyTo(temp);
		Mat temp_src;//原图像
		frame_rgb.copyTo(temp_src);
		if (temp_src.type() != 0)
			cvtColor(temp_src, temp_src, CV_RGB2GRAY);

		int *sub;
		double  s_PSNR = 0;
		sub = new int[work.cols*work.rows];
		for (int i = 0; i < work.rows*work.cols; i++)
		{
			sub[i] = (int(temp.data[i]) - int(temp_src.data[i]))*(int(temp.data[i]) - int(temp_src.data[i]));
			s_PSNR += sub[i];
		}
		s_PSNR = s_PSNR / (temp.cols*temp.rows);
		PSNR = 10.0*log10((255 * 255) / s_PSNR);
		temp_src.release();
		temp.release();

		QString dlgTitle = "PSNR";
		QString strInfo = QString::number(PSNR, 10, 4);
		QMessageBox::about(this, dlgTitle, strInfo);

	}
	else if (act->text() == "NAFSM")
	{
		if (work.empty())
		{
			return;
		}
		if (work.type() != 0)
			cvtColor(work, work, CV_RGB2GRAY);
		Mat temp, med;
		work.copyTo(temp);
		work.copyTo(med);
		int count, max;
		double fraction;
		for (int i = 1; i < work.rows - 1; i++)
			for (int j = 1; j < work.cols - 1; j++)
			{
				if (work.at<uchar>(i, j) != 0 && work.at<uchar>(i, j) != 255)
					continue;
				max = 0;
				count = 0;
				int self = temp.at<uchar>(i, j);
				int pix[8], pix_temp[8];
				pix[0] = temp.at<uchar>(i - 1, j - 1);
				pix[1] = temp.at<uchar>(i - 1, j);
				pix[2] = temp.at<uchar>(i - 1, j + 1);
				pix[3] = temp.at<uchar>(i, j - 1);
				pix[4] = temp.at<uchar>(i, j + 1);
				pix[5] = temp.at<uchar>(i + 1, j - 1);
				pix[6] = temp.at<uchar>(i + 1, j);
				pix[7] = temp.at<uchar>(i + 1, j + 1);

				for (int k = 0; k < 8; k++)
					if (pix[k] != 0 && pix[k] != 255)
					{
						pix_temp[count] = pix[k];
						count++;
						if (abs(pix[k] - self) > max)
							max = abs(pix[k] - self);
					}
				if (max < 10)
					fraction = 0;
				else if (max < 30)
					fraction = (max - 10)*1.0 / 20;
				else
					fraction = 1;

				if (count != 0 || i<3 || j<3 || i>work.rows - 3 || j>work.cols - 3)
				{
					for (int k = 0; k < count; k++)
					{
						for (int l = 0; l < count - k; l++)
						{
							if (pix_temp[l] > pix_temp[l + 1])
							{
								int temp = pix_temp[l];
								pix_temp[l] = pix_temp[l + 1];
								pix_temp[l + 1] = temp;
							}
						}
					}
					if (count == 0)
						med.at<uchar>(i, j) = temp.at<uchar>(i, j);
					else if (count % 2 == 0)
						med.at<uchar>(i, j) = int((int(pix_temp[count / 2]) + int(pix_temp[count / 2 + 1])) / 2);
					else
						med.at<uchar>(i, j) = int(pix_temp[count / 2 + 1]);
					work.at<uchar>(i, j) = int((1 - fraction)*temp.at<uchar>(i, j) + fraction * med.at<uchar>(i, j));
					continue;
				}

				int pix_1[16], pix_1_temp[16] = { 0 };
				pix_1[0] = temp.at<uchar>(i - 2, j - 2);
				pix_1[1] = temp.at<uchar>(i - 2, j - 1);
				pix_1[2] = temp.at<uchar>(i - 2, j);
				pix_1[3] = temp.at<uchar>(i - 2, j + 1);
				pix_1[4] = temp.at<uchar>(i - 2, j + 2);
				pix_1[5] = temp.at<uchar>(i - 1, j - 2);
				pix_1[6] = temp.at<uchar>(i - 1, j + 1);
				pix_1[7] = temp.at<uchar>(i, j - 2);
				pix_1[8] = temp.at<uchar>(i, j + 2);
				pix_1[9] = temp.at<uchar>(i + 1, j - 2);
				pix_1[10] = temp.at<uchar>(i + 1, j + 2);
				pix_1[11] = temp.at<uchar>(i + 2, j - 2);
				pix_1[12] = temp.at<uchar>(i + 2, j - 1);
				pix_1[13] = temp.at<uchar>(i + 2, j);
				pix_1[14] = temp.at<uchar>(i + 2, j + 1);
				pix_1[15] = temp.at<uchar>(i + 2, j + 2);
				for (int k = 0; k < 16; k++)
					if (pix_1[k] != 0 || pix_1[k] != 255)
					{
						pix_1_temp[count] = pix_1[k];
						count++;
					}
				if (count != 0 || i<4 || j<4 || i>work.rows - 4 || j>work.cols - 4)
				{
					for (int k = 0; k < count; k++)
					{
						for (int l = 0; l < count - k; l++)
						{
							if (pix_1_temp[l] > pix_1_temp[l + 1])
							{
								int temp = pix_1_temp[l];
								pix_1_temp[l] = pix_1_temp[l + 1];
								pix_1_temp[l + 1] = temp;
							}
						}
					}
					if (count == 0)
						med.at<uchar>(i, j) = temp.at<uchar>(i, j);
					else if (count % 2 == 0)
						med.at<uchar>(i, j) = int((int(pix_1_temp[count / 2]) + int(pix_1_temp[count / 2 + 1])) / 2);
					else
						med.at<uchar>(i, j) = int(pix_1_temp[count / 2 + 1]);
					work.at<uchar>(i, j) = int((1 - fraction)*temp.at<uchar>(i, j) + fraction * med.at<uchar>(i, j));
					continue;
				}

				int pix_2[24], pix_2_temp[24] = { 0 };
				pix_2[0] = temp.at<uchar>(i - 3, j - 3);
				pix_2[1] = temp.at<uchar>(i - 3, j - 2);
				pix_2[2] = temp.at<uchar>(i - 3, j - 1);
				pix_2[3] = temp.at<uchar>(i - 3, j);
				pix_2[4] = temp.at<uchar>(i - 3, j + 1);
				pix_2[5] = temp.at<uchar>(i - 3, j + 2);
				pix_2[6] = temp.at<uchar>(i - 3, j + 3);
				pix_2[7] = temp.at<uchar>(i - 2, j - 3);
				pix_2[8] = temp.at<uchar>(i - 2, j + 3);
				pix_2[9] = temp.at<uchar>(i - 1, j - 3);
				pix_2[10] = temp.at<uchar>(i - 1, j + 3);
				pix_2[11] = temp.at<uchar>(i, j - 3);
				pix_2[12] = temp.at<uchar>(i, j + 3);
				pix_2[13] = temp.at<uchar>(i + 1, j - 3);
				pix_2[14] = temp.at<uchar>(i + 1, j + 3);
				pix_2[15] = temp.at<uchar>(i + 2, j - 3);
				pix_2[16] = temp.at<uchar>(i + 2, j + 3);
				pix_2[17] = temp.at<uchar>(i + 3, j - 3);
				pix_2[18] = temp.at<uchar>(i + 3, j - 2);
				pix_2[19] = temp.at<uchar>(i + 3, j - 1);
				pix_2[20] = temp.at<uchar>(i + 3, j);
				pix_2[21] = temp.at<uchar>(i + 3, j + 1);
				pix_2[22] = temp.at<uchar>(i + 3, j + 2);
				pix_2[23] = temp.at<uchar>(i + 3, j + 3);
				for (int k = 0; k < 24; k++)
					if (pix_2[k] != 0 || pix_2[k] != 255)
					{
						pix_2_temp[count] = pix_2[k];
						count++;
					}

				for (int k = 0; k < count; k++)
				{
					for (int l = 0; l < count - k; l++)
					{
						if (pix_2_temp[l] > pix_2_temp[l + 1])
						{
							int temp = pix_2_temp[l];
							pix_2_temp[l] = pix_2_temp[l + 1];
							pix_2_temp[l + 1] = temp;
						}
					}
				}
				if (count == 0)
					med.at<uchar>(i, j) = temp.at<uchar>(i, j);
				else if (count % 2 == 0)
					med.at<uchar>(i, j) = int((int(pix_2_temp[count / 2]) + int(pix_2_temp[count / 2 + 1])) / 2);
				else
					med.at<uchar>(i, j) = int(pix_2_temp[count / 2 + 1]);

				work.at<uchar>(i, j) = int((1 - fraction)*temp.at<uchar>(i, j) + fraction * med.at<uchar>(i, j));

				//work.at<uchar>(i, j) = temp.at<uchar>(i, j);
			}

		temp.release();
		QImage img((const uchar*)work.data, work.cols, work.rows, work.cols*(work.channels()), QImage::Format_Indexed8);
		ui.scrollAreaWidgetContents->setMinimumSize(QSize(img.width(), img.height()));
		ui.label->resize(QSize(img.width(), img.height()));
		ui.label->setPixmap(QPixmap::fromImage(img));
		img.~QImage();
	}
	else if (act->text() == "Prewitt")
	{
		if (work.empty())
		{
			return;
		}
		if (work.type() != 0)
			cvtColor(work, work, CV_RGB2GRAY);
		Mat temp;
		work.copyTo(temp);

		for (int i = 1; i < work.rows - 1; i++)
			for (int j = 1; j < work.cols - 1; j++)
			{
				int L, V, T;
				L = (int)((-temp.at<uchar>(i - 1, j - 1) - temp.at<uchar>(i - 1, j) - temp.at<uchar>(i - 1, j + 1)
					+ temp.at<uchar>(i + 1, j - 1) + temp.at<uchar>(i, j) + temp.at<uchar>(i + 1, j + 1)));
				V = (int)((-temp.at<uchar>(i - 1, j - 1) + temp.at<uchar>(i - 1, j + 1)
					- temp.at<uchar>(i, j - 1) + temp.at<uchar>(i, j + 1)
					- temp.at<uchar>(i + 1, j - 1) + temp.at<uchar>(i + 1, j + 1)));
				T = sqrt(L*L + V * V);
				if (T > 255)
					work.at<uchar>(i, j) = 255;
				else if (T < 0)
					work.at<uchar>(i, j) = 0;
				else
					work.at<uchar>(i, j) = T;
			}

		//temp.copyTo(work);
		temp.release();
		QImage img((const uchar*)work.data, work.cols, work.rows, work.cols*(work.channels()), QImage::Format_Indexed8);
		ui.scrollAreaWidgetContents->setMinimumSize(QSize(img.width(), img.height()));
		ui.label->resize(QSize(img.width(), img.height()));
		ui.label->setPixmap(QPixmap::fromImage(img));
		img.~QImage();
	}
	else if (act->text() == "Sobel")
	{
		if (work.empty())
		{
			return;
		}
		if (work.type() != 0)
			cvtColor(work, work, CV_RGB2GRAY);
		Mat temp;
		work.copyTo(temp);

		for (int i = 1; i < work.rows - 1; i++)
			for (int j = 1; j < work.cols - 1; j++)
			{
				int L, V, T;
				L = (int)((-temp.at<uchar>(i - 1, j - 1) - 2 * temp.at<uchar>(i - 1, j) - temp.at<uchar>(i - 1, j + 1)
					+ temp.at<uchar>(i + 1, j - 1) + 2 * temp.at<uchar>(i, j) + temp.at<uchar>(i + 1, j + 1)));
				V = (int)((-temp.at<uchar>(i - 1, j - 1) + temp.at<uchar>(i - 1, j + 1)
					- 2 * temp.at<uchar>(i, j - 1) + 2 * temp.at<uchar>(i, j + 1)
					- temp.at<uchar>(i + 1, j - 1) + temp.at<uchar>(i + 1, j + 1)));
				T = sqrt(L*L + V * V);
				if (T > 255)
					work.at<uchar>(i, j) = 255;
				else if (T < 0)
					work.at<uchar>(i, j) = 0;
				else
					work.at<uchar>(i, j) = T;
			}

		//temp.copyTo(work);
		temp.release();
		QImage img((const uchar*)work.data, work.cols, work.rows, work.cols*(work.channels()), QImage::Format_Indexed8);
		ui.scrollAreaWidgetContents->setMinimumSize(QSize(img.width(), img.height()));
		ui.label->resize(QSize(img.width(), img.height()));
		ui.label->setPixmap(QPixmap::fromImage(img));
		img.~QImage();
	}
	else if (act->text() == "Robert")
	{
		if (work.empty())
		{
			return;
		}
		if (work.type() != 0)
			cvtColor(work, work, CV_RGB2GRAY);
		Mat temp;
		work.copyTo(temp);

		for (int i = 1; i < work.rows - 1; i++)
			for (int j = 1; j < work.cols - 1; j++)
			{
				int L, V, T;
				L = (int)((temp.at<uchar>(i, j) - temp.at<uchar>(i + 1, j + 1)));
				V = (int)((temp.at<uchar>(i, j + 1) - temp.at<uchar>(i + 1, j)));
				T = sqrt(L*L + V * V);
				if (T > 255)
					work.at<uchar>(i, j) = 255;
				else if (T < 0)
					work.at<uchar>(i, j) = 0;
				else
					work.at<uchar>(i, j) = T;
			}

		//temp.copyTo(work);
		temp.release();
		QImage img((const uchar*)work.data, work.cols, work.rows, work.cols*(work.channels()), QImage::Format_Indexed8);
		ui.scrollAreaWidgetContents->setMinimumSize(QSize(img.width(), img.height()));
		ui.label->resize(QSize(img.width(), img.height()));
		ui.label->setPixmap(QPixmap::fromImage(img));
		img.~QImage();
	}
	else if (act->text() == "LOG(4)")
	{
		if (work.empty())
		{
			return;
		}
		if (work.type() != 0)
			cvtColor(work, work, CV_RGB2GRAY);
		Mat temp;
		work.copyTo(temp);

		for (int i = 1; i < work.rows - 1; i++)
			for (int j = 1; j < work.cols - 1; j++)
			{
				int T;
				T = (int)((-temp.at<uchar>(i - 1, j)
					- temp.at<uchar>(i, j - 1) + 4 * temp.at<uchar>(i, j) - temp.at<uchar>(i, j + 1)
					- temp.at<uchar>(i, j)));
				if (T > 255)
					work.at<uchar>(i, j) = 255;
				else if (T < 0)
					work.at<uchar>(i, j) = 0;
				else
					work.at<uchar>(i, j) = T;

			}
		//temp.copyTo(work);
		temp.release();
		QImage img((const uchar*)work.data, work.cols, work.rows, work.cols*(work.channels()), QImage::Format_Indexed8);
		ui.scrollAreaWidgetContents->setMinimumSize(QSize(img.width(), img.height()));
		ui.label->resize(QSize(img.width(), img.height()));
		ui.label->setPixmap(QPixmap::fromImage(img));
		img.~QImage();
	}
	else if (act->text() == "LOG(8)")
	{
		if (work.empty())
		{
			return;
		}
		if (work.type() != 0)
			cvtColor(work, work, CV_RGB2GRAY);
		Mat temp;
		work.copyTo(temp);

		for (int i = 1; i < work.rows - 1; i++)
			for (int j = 1; j < work.cols - 1; j++)
			{
				int T;
				T = (int)((-temp.at<uchar>(i - 1, j - 1) - temp.at<uchar>(i - 1, j) - temp.at<uchar>(i - 1, j + 1)
					- temp.at<uchar>(i, j - 1) + 8 * temp.at<uchar>(i, j) - temp.at<uchar>(i, j + 1)
					- temp.at<uchar>(i + 1, j - 1) - temp.at<uchar>(i, j) - temp.at<uchar>(i + 1, j + 1)));
				if (T > 255)
					work.at<uchar>(i, j) = 255;
				else if (T < 0)
					work.at<uchar>(i, j) = 0;
				else
					work.at<uchar>(i, j) = T;

			}
		//temp.copyTo(work);
		temp.release();
		QImage img((const uchar*)work.data, work.cols, work.rows, work.cols*(work.channels()), QImage::Format_Indexed8);
		ui.scrollAreaWidgetContents->setMinimumSize(QSize(img.width(), img.height()));
		ui.label->resize(QSize(img.width(), img.height()));
		ui.label->setPixmap(QPixmap::fromImage(img));
		img.~QImage();
	}
	else if (act->text() == "Canny")
	{
		if (work.empty())
		{
			return;
		}
		if (work.type() != 0)
			cvtColor(work, work, CV_RGB2GRAY);
		Mat temp;
		work.copyTo(temp);

		Canny(temp, temp, 60, 180, 3, 1);
		temp.copyTo(work);
		temp.release();
		QImage img((const uchar*)work.data, work.cols, work.rows, work.cols*(work.channels()), QImage::Format_Indexed8);
		ui.scrollAreaWidgetContents->setMinimumSize(QSize(img.width(), img.height()));
		ui.label->resize(QSize(img.width(), img.height()));
		ui.label->setPixmap(QPixmap::fromImage(img));
		img.~QImage();
	}
	else if (act->text() == "无种子区域生长")
	{
		Mat test(Size(256, 256), 0);
		for (int i = 0; i < 256; i++)
			for (int j = 0; j < 256; j++)
				test.at<uchar>(j, i) = i;
		imwrite("test1.bmp", test);
		for (int i = 0; i < 256; i++)
			for (int j = 0; j < 256; j++)
				test.at<uchar>(j, i) = sqrt((128-i)*(128-i)+ (128 - j)*(128 - j));
		imwrite("test2.bmp", test);
		if (work.empty())
		{
			return;
		}
		if (work.type() != 0)
			cvtColor(work, work, CV_RGB2GRAY);
		//Mat temp_avg = Mat::zeros(Size(work.rows, work.cols), 0);
		Mat	temp = Mat::zeros(Size(work.cols, work.rows), 0);
		Mat	temp_border = Mat::zeros(Size(work.cols, work.rows), 0);
		Mat	mark = Mat::zeros(Size(work.cols, work.rows), 0);;
		//temp_avg用于存储区域，灰度值为其平均值
		//temp_border用于存储边界，只有0/255
		//mark用于存储已经遍历的点
		unsigned char  ***A;
		int num_A = 0, threshold = 40;//num_A记录A的集合个数，从0开始
		double num_mark = 0, theta;//用于存储标记的点的个数和theta的值

		A = new unsigned char **[work.rows*work.cols];//创建一个三维数组用于存储A0-AK
		A[num_A] = new unsigned char*[work.rows];
		for (int i = 0; i < work.rows; i++)
			A[num_A][i] = new unsigned char[work.cols];

		for (int i = 0; i < work.rows; i++)//初始化集合A0
			for (int j = 0; j < work.cols; j++)
				A[num_A][i][j] = 0;

		double *ave_A, *pix_in_A, *sun_A;

		ave_A = new double[work.rows*work.cols];//创建一个 二维数组存储平均值
		for (int i = 0; i < work.rows*work.cols; i++)//初始化
			ave_A[i] = 0;

		pix_in_A = new double[work.rows*work.cols];//创建一个 二维数组存储AK中点的个数
		for (int i = 0; i < work.rows*work.cols; i++)//初始化
			pix_in_A[i] = 0;

		sun_A = new double[work.rows*work.cols];//创建一个 二维数组存储AK中点的总值
		for (int i = 0; i < work.rows*work.cols; i++)//初始化
			sun_A[i] = 0;


		std::cout << work.cols / 2 + 1 << ' ' << work.rows / 2 << std::endl;

		A[num_A][work.cols / 2][work.rows / 2] = 255;//将中间点设置为初始的种子点
		pix_in_A[num_A]++;//A0中，点的个数加1
		sun_A[num_A] += work.at<uchar>(work.cols / 2, work.rows / 2);//A0中，点的像素和加上初始点
		ave_A[num_A] = sun_A[num_A] / pix_in_A[num_A];//计算A0中，像素平均值
		mark.at<uchar>(work.cols / 2, work.rows / 2) = 255;//在mark中将初始点mark,表示已经遍历
		cal_border(mark, temp_border, work.cols / 2, work.rows / 2);//修改border
		std::cout << "第一个点后" << std::endl;
		for (int i = 0; i < temp_border.cols; i++)
			for (int j = 0; j < temp_border.rows; j++)
				if (temp_border.at<uchar>(j, i) == 255)
					std::cout << j << ' ' << i << std::endl;


		A[num_A][work.cols / 2 + 1][work.rows / 2] = 255;//将中间点设置为初始的种子点
		mark.at<uchar>(work.cols / 2 + 1, work.rows / 2) = 255;//在mark中将初始点mark,表示已经遍历
		temp_border.at<uchar>(work.cols / 2 + 1, work.rows / 2) = 0;//在mark中将初始点mark,表示已经遍历
		cal_border(mark, temp_border, work.cols / 2 + 1, work.rows / 2);//修改border
		std::cout << "第二个点后" << std::endl;
		for (int i = 0; i < temp_border.cols; i++)
			for (int j = 0; j < temp_border.rows; j++)
				if (temp_border.at<uchar>(j, i) == 255)
					std::cout << j << ' ' << i << std::endl;
		/*

		std::cout << (int)temp_border.at<uchar>(work.cols / 2+1, work.rows / 2) << ' '
			<< (int)temp_border.at<uchar>(work.cols / 2-1, work.rows / 2) << ' '
			<< (int)temp_border.at<uchar>(work.cols / 2, work.rows / 2+1) << ' '
			<< (int)temp_border.at<uchar>(work.cols / 2, work.rows / 2-1) <<' '
			<< (int)temp_border.at<uchar>(work.cols / 2, work.rows / 2) << std::endl;
*/
		while (1)//不停循环，知道历遍所有的点
		{

			num_mark++;//循环一遍多一个点进入A
			double theta_min = 255;
			int x_min, y_min, temp_k;

			for (int i = 0; i < temp_border.cols; i++)
				for (int j = 0; j < temp_border.rows; j++)
					if (temp_border.at<uchar>(j, i) == 255)
					{
						for (int k = 0; k <= num_A; k++)//遍历所有的集合A
						{
							//找到在T上的最小的theta值
							theta = abs(work.at<uchar>(j, i) - ave_A[k]);
							if (theta < theta_min)
							{
								theta_min = theta;
								x_min = i;
								y_min = j;
								temp_k = k;
							}
						}
					}
			/*
			std::cout << (int)temp_border.at<uchar>(y_min + 1, x_min) << ' '
				<< (int)temp_border.at<uchar>(y_min - 1, x_min) << ' '
				<< (int)temp_border.at<uchar>(y_min, x_min + 1) << ' '
				<< (int)temp_border.at<uchar>(y_min, x_min - 1) << ' '
				<< (int)temp_border.at<uchar>(y_min, x_min)<< std::endl;*/


			if (theta_min < threshold)
			{
				mark.at<uchar>(y_min, x_min) = 255;//修改区域A的集合
				temp_border.at<uchar>(y_min, x_min) = 0;
				A[temp_k][y_min][x_min] = 255;//修改区域AK
				pix_in_A[temp_k]++;//A0中，点的个数加1
				sun_A[temp_k] += work.at<uchar>(y_min, x_min);//A0中，点的像素和加上初始点
				ave_A[temp_k] = sun_A[temp_k] / pix_in_A[temp_k];//计算A0中，像素平均值	
				cal_border(mark, temp_border, y_min, x_min);//修改border
				//std::cout << temp_k << ' ' << x_min << ' ' << y_min << ' ' << pix_in_A[temp_k] << ' ' << sun_A[temp_k] << std::endl;
			}
			else
			{
				//创建一个新的A集合
				num_A++;
				A[num_A] = new unsigned char*[work.rows];
				for (int i = 0; i < work.rows; i++)
					A[num_A][i] = new unsigned char[work.cols];

				for (int i = 0; i < work.rows; i++)//初始化集合A0
					for (int j = 0; j < work.cols; j++)
						A[num_A][i][j] = 0;

				mark.at<uchar>(y_min, x_min) = 255;//修改区域A的集合
				temp_border.at<uchar>(y_min, x_min) = 0;
				A[num_A][y_min][x_min] = 255;//修改区域AK
				pix_in_A[num_A]++;//A0中，点的个数加1
				sun_A[num_A] += work.at<uchar>(y_min, x_min);//A0中，点的像素和加上初始点
				ave_A[num_A] = sun_A[num_A] / pix_in_A[num_A];//计算A0中，像素平均值
				cal_border(mark, temp_border, y_min, x_min);//修改border

			//	std::cout << temp_k << ' ' << x_min << ' ' << y_min << ' ' << pix_in_A[temp_k] << ' ' << sun_A[temp_k] << std::endl;
			}
			std::cout << num_mark << ' ' << mark.rows*mark.cols - 1 << " " << num_A << std::endl;
			/*
						std::cout <<  std::endl;
						for (int i = 0; i < temp_border.cols; i++)
							for (int j = 0; j < temp_border.rows; j++)
								if (temp_border.at<uchar>(j, i) == 255)
									std::cout << j << ' ' << i << std::endl;*/
			if (num_mark == mark.rows*mark.cols - 1)
				break;
		}
		/*
		int count = 0;
		for (int i = 0; i < temp.cols; i++)
			for (int j = 0; j < temp.rows; j++)
			{
				if (mark.at<uchar>(j, i) != 255)
				{
					double min=255;
					int t;
					for (int k = 0; k <= num_A; k++)//遍历所有的集合A
					{
						//找到在T上的最小的theta值
						theta = abs(work.at<uchar>(j, i) - ave_A[k]);
						if (theta < min)
							t = k;
						//std::cout << i << ' ' << j << std::endl;
						count++;
					}
					mark.at<uchar>(j, i) = ave_A[t];
				}
			}
		std::cout << count;*/
		int count = 0;
		for (int k = 0; k <= num_A; k++)
			for (int i = 0; i < temp.cols; i++)
				for (int j = 0; j < temp.rows; j++)
				{
					if (A[k][j][i] == 255)
					{
						temp.at<uchar>(j, i) = ave_A[k];
						//	std::cout << k << ' ' << i << ' ' << j << ' ' << ave_A[k] << std::endl;
						count++;
					}
				}
		std::cout << count;
		/*
		for (int i = 0; i < work.rows; i++)
			for (int j = 0; j < work.cols; j++)
			{
				if (mark.at<uchar>(i, j) != 0)//如果已经标记则跳过
					continue;
				double num, sum = 0;//用于存储当前区域的像素点数量和总的灰度和
				num = 1;
				Mat A = Mat::zeros(Size(work.rows, work.cols), 0);//创建一个新的集合
				Mat border = Mat::zeros(Size(work.rows, work.cols), 0);//创建集合边界的集合
				A.at<uchar>(i, j) = work.at<uchar>(i, j);
				mark.at<uchar>(i, j) = 1;
				sum += work.at<uchar>(i, j);
				cal_border(A, border);
			}*/
		temp.copyTo(work);
		temp.release();
		QImage img((const uchar*)work.data, work.cols, work.rows, work.cols*(work.channels()), QImage::Format_Indexed8);
		ui.scrollAreaWidgetContents->setMinimumSize(QSize(img.width(), img.height()));
		ui.label->resize(QSize(img.width(), img.height()));
		ui.label->setPixmap(QPixmap::fromImage(img));
		img.~QImage();
	}
	this->update();
}
double generateGaussianNoise(double mu, double sigma)//计算高斯噪声mu为均值，sigma为方差
{
	static double V1, V2, S;
	static int phase = 0;
	double X;
	double U1, U2;
	if (phase == 0) {
		do {
			U1 = (double)rand() / RAND_MAX;
			U2 = (double)rand() / RAND_MAX;

			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
		} while (S >= 1 || S == 0);

		X = V1 * sqrt(-2 * log(S) / S);
	}
	else {
		X = V2 * sqrt(-2 * log(S) / S);
	}
	phase = 1 - phase;
	return mu + sigma * X;
}
