#include "Qt_OpenCV.h"
#include<iostream>
#pragma execution_character_set("utf-8")//�������룬��Ȼ������Ϣ����
double generateGaussianNoise(double mu, double sigma);
Qt_OpenCV::Qt_OpenCV(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//Qt_OpenCV::connect(ui.menu_src->menuAction(), SIGNAL(triggered()), this, SLOT(Menu_Src()));
	ui.menuBar->addAction(QString::fromUtf8("�ҶȻ�"), this, SLOT(Menu_Action()));
	ui.menuBar->addAction(QString::fromUtf8("ԭͼ��"), this, SLOT(Menu_Action()));
	ui.menuBar->addAction(QString::fromUtf8("ֱ��ͼ"), this, SLOT(Menu_Action()));
	ui.menuBar->addAction(QString::fromUtf8("��������������"), this, SLOT(Menu_Action()));

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
bool bool_border(Mat Mark, Mat border, int x, int y)//�жϵ�(x,y)�Ƿ������ڱ߽�
{
	int row = Mark.rows, col = Mark.cols;
	if (Mark.at<uchar>(y, x) == 255)
		return false;//����õ��Ѿ�������Ϊ�߽�
	if (x > 0 && y > 0 && y < row - 1 && x < col - 1)//���ڱ�Ե
		if (
			(Mark.at<uchar>(y - 1, x) == 0 || Mark.at<uchar>(y + 1, x) == 0 || Mark.at<uchar>(y, x - 1) == 0 || Mark.at<uchar>(y, x + 1) == 0)//����������һ����Ϊ0
			&&
			(Mark.at<uchar>(y - 1, x) != 0 || Mark.at<uchar>(y + 1, x) != 0 || Mark.at<uchar>(y, x - 1) != 0 || Mark.at<uchar>(y, x + 1) != 0)//����������һ���㲻Ϊ0
			|| (Mark.at<uchar>(y - 1, x) != 0 && Mark.at<uchar>(y + 1, x) != 0 && Mark.at<uchar>(y, x - 1) != 0 && Mark.at<uchar>(y, x + 1) != 0)//�������ܶ�Ϊ255ʱҲ�߽�
			)
			return true;
		else
			return false;
	if (y == 0 && x > 0 && x < col - 1)//�ϱ�Ե--1
		if (
			(Mark.at<uchar>(y + 1, x) == 0 || Mark.at<uchar>(y, x - 1) == 0 || Mark.at<uchar>(y, x + 1) == 0)//����������һ����Ϊ0
			&&
			(Mark.at<uchar>(y + 1, x) != 0 || Mark.at<uchar>(y, x - 1) != 0 || Mark.at<uchar>(y, x + 1) != 0)//����������һ���㲻Ϊ0
			|| (  Mark.at<uchar>(y + 1, x) != 0 && Mark.at<uchar>(y, x - 1) != 0 && Mark.at<uchar>(y, x + 1) != 0)//�������ܶ�Ϊ255ʱҲ�߽�


			)
			return true;
		else
			return false;
	if (x > 0 && y == row - 1 && x < col - 1)//�ұ�Ե--2
		if (
			(Mark.at<uchar>(y - 1, x) == 0 || Mark.at<uchar>(y, x - 1) == 0 || Mark.at<uchar>(y, x + 1) == 0)//����������һ����Ϊ0
			&&
			(Mark.at<uchar>(y - 1, x) != 0 || Mark.at<uchar>(y, x - 1) != 0 || Mark.at<uchar>(y, x + 1) != 0)//����������һ���㲻Ϊ0
			|| (Mark.at<uchar>(y - 1, x) != 0  && Mark.at<uchar>(y, x - 1) != 0 && Mark.at<uchar>(y, x + 1) != 0)//�������ܶ�Ϊ255ʱҲ�߽�
			)
			return true;
		else
			return false;
	if (y > 0 && x == 0 && y < row - 1)//�ϱ�Ե--3
		if (
			(Mark.at<uchar>(y - 1, x) == 0 || Mark.at<uchar>(y + 1, x) == 0 || Mark.at<uchar>(y, x + 1) == 0)//����������һ����Ϊ0
			&&
			(Mark.at<uchar>(y - 1, x) != 0 || Mark.at<uchar>(y + 1, x) != 0 || Mark.at<uchar>(y, x + 1) != 0)//����������һ���㲻Ϊ0
			|| (Mark.at<uchar>(y - 1, x) != 0 && Mark.at<uchar>(y + 1, x) != 0  && Mark.at<uchar>(y, x + 1) != 0)//�������ܶ�Ϊ255ʱҲ�߽�
			)
			return true;
		else
			return false;
	if (y > 0 && y < row - 1 && x == col - 1)//�±�Ե--4
		if (
			(Mark.at<uchar>(y - 1, x) == 0 || Mark.at<uchar>(y + 1, x) == 0 || Mark.at<uchar>(y, x - 1) == 0)//����������һ����Ϊ0
			&&
			(Mark.at<uchar>(y - 1, x) != 0 || Mark.at<uchar>(y + 1, x) != 0 || Mark.at<uchar>(y, x - 1) != 0)//����������һ���㲻Ϊ0
			|| (Mark.at<uchar>(y - 1, x) != 0 && Mark.at<uchar>(y + 1, x) != 0 && Mark.at<uchar>(y, x - 1) != 0 )//�������ܶ�Ϊ255ʱҲ�߽�
			)
			return true;
		else
			return false;
	if (x == 0 && y == 0)//���Ͻ�--5
		if (
			(Mark.at<uchar>(y + 1, x) == 0 || Mark.at<uchar>(y, x + 1) == 0)//����������һ����Ϊ0
			&&
			(Mark.at<uchar>(y + 1, x) != 0 || Mark.at<uchar>(y, x + 1) != 0)//����������һ���㲻Ϊ0
			|| ( Mark.at<uchar>(y + 1, x) != 0  && Mark.at<uchar>(y, x + 1) != 0)//�������ܶ�Ϊ255ʱҲ�߽�
			)
			return true;
		else
			return false;
	if (x == 0 && y == row - 1)//���Ͻ�--6
		if (
			(Mark.at<uchar>(y - 1, x) == 0 || Mark.at<uchar>(y, x + 1) == 0)//����������һ����Ϊ0
			&&
			(Mark.at<uchar>(y - 1, x) != 0 || Mark.at<uchar>(y, x + 1) != 0)//����������һ���㲻Ϊ0
			|| (Mark.at<uchar>(y - 1, x) != 0  && Mark.at<uchar>(y, x + 1) != 0)//�������ܶ�Ϊ255ʱҲ�߽�
			)
			return true;
		else
			return false;
	if (y == 0 && x == col - 1)//���½�--7
		if (
			(Mark.at<uchar>(y + 1, x) == 0 || Mark.at<uchar>(y, x - 1) == 0)//����������һ����Ϊ0
			&&
			(Mark.at<uchar>(y + 1, x) != 0 || Mark.at<uchar>(y, x - 1) != 0)//����������һ���㲻Ϊ0
			|| (  Mark.at<uchar>(y + 1, x) != 0 && Mark.at<uchar>(y, x - 1) != 0 )//�������ܶ�Ϊ255ʱҲ�߽�
			)
			return true;
		else
			return false;
	if (y == row - 1 && x == col - 1)//���½�--8
		if (
			(Mark.at<uchar>(y - 1, x) == 0 || Mark.at<uchar>(y, x - 1) == 0)//����������һ����Ϊ0
			&&
			(Mark.at<uchar>(y - 1, x) != 0 || Mark.at<uchar>(y, x - 1) != 0)//����������һ���㲻Ϊ0
			|| (Mark.at<uchar>(y - 1, x) != 0  && Mark.at<uchar>(y, x - 1) != 0 )//�������ܶ�Ϊ255ʱҲ�߽�
			)
			return true;
		else
			return false;

};
void cal_border(Mat Mark, Mat border, int new_y, int new_x)
//���д˺���ʱ��Mark���Ѿ���ӵ�(new_x,new_y)ΪMark���µ�һ���㣬���޸���(new_x,new_y)��Ϊbrder
//�ú�������õ�ı仯������Χ�ĸ����border����Ӱ��
{
	int row = Mark.rows, col = Mark.cols;
	if (new_x != 0)//��Ϊ�����ĵ�
	{
		if (bool_border(Mark, border, new_x - 1, new_y))
			border.at<uchar>(new_y, new_x - 1) = 255;
		else
			border.at<uchar>(new_y, new_x - 1) = 0;
	}
	//����
	//std::cout << "1" << std::endl;
	if (new_x != row - 1)//��Ϊ���Ҳ�ĵ�
	{
		if (bool_border(Mark, border, new_x + 1, new_y))
			border.at<uchar>(new_y, new_x + 1) = 255;
		else
			border.at<uchar>(new_y, new_x + 1) = 0;
	}
	//�Ҳ��
	//std::cout << "2" << std::endl;
	if (new_y != 0)//��Ϊ���ϲ�ĵ�
	{
		if (bool_border(Mark, border, new_x, new_y - 1))
			border.at<uchar>(new_y - 1, new_x) = 255;
		else
			border.at<uchar>(new_y - 1, new_x) = 0;
	}
	//�ϲ��
	//std::cout << "3" << std::endl;
	if (new_y != col - 1)//��Ϊ���²�ĵ�
	{
		if (bool_border(Mark, border, new_x, new_y + 1))
			border.at<uchar>(new_y + 1, new_x) = 255;
		else
			border.at<uchar>(new_y + 1, new_x) = 0;
	}
	//�²��
	//std::cout << "4" << std::endl;
};
void Qt_OpenCV::Menu_Action(QAction* act) {
	if (act->text() == "��")
	{
		QString path = QFileDialog::getOpenFileName(this, "���ļ�", "/", "ALL(*.*);;BMP(*.bmp);;JPG(*.jpg)");
		if (!path.isEmpty())
		{
			qDebug() << path;
			frame_bgr = imread(path.toLocal8Bit().constData());//imread��֧��ASCII�룬��QT��ҪUTF8�������ı���
			if (frame_bgr.empty())
				return;

			cvtColor(frame_bgr, frame_rgb, COLOR_BGR2RGB);    //���QImage��ʽ��ͼƬ
			frame_rgb.copyTo(work);

			QImage img((const uchar*)frame_rgb.data, frame_rgb.cols, frame_rgb.rows, frame_rgb.cols*(frame_rgb.channels()), QImage::Format_RGB888);
			ui.scrollAreaWidgetContents->setMinimumSize(QSize(img.width(), img.height()));
			ui.label->resize(QSize(img.width(), img.height()));
			ui.label->setPixmap(QPixmap::fromImage(img));
			img.~QImage();
		}
	}
	else if (act->text() == "����")
	{
		imwrite("result.bmp", work);
	}
	else if (act->text() == "ԭͼ��")
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
	else if (act->text() == "�ҶȻ�")
	{
		if (work.empty())
			return;
		Mat temp;
		if (work.type() != 0)//�ж�ͼ������
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
	else if (act->text() == "����㷨")
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
		qDebug() << "����㷨";
	}
	else if (act->text() == "����ֵ")
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
		qDebug() << "����ֵ";
	}
	else if (act->text() == "��������")
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
	else if (act->text() == "������")
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
	else if (act->text() == "��˹����")
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
	else if (act->text() == "��ֵ�˲�3X3")
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
	else if (act->text() == "��ֵ�˲�5X5")
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
	else if (act->text() == "��ֵ�˲�3X3")
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
	else if (act->text() == "��ֵ�˲�5X5")
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
	else if (act->text() == "��˹�˲�3X3")
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
	else if (act->text() == "��˹�˲�5X5")
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
	else if (act->text() == "ֱ��ͼ")
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
		int max = 0;//�����洢���ȳ������Ĵ���
		for (int i = 0; i < 256; i++)
			if (gray[i] > max)
				max = gray[i];
		/*������Ϊ��ɫ*/
		for (int i = 0; i < 512; i++)
			for (int j = 0; j < 512; j++)
				temp.at < uchar >(i, j) = 0;
		/*��ͼ*/
		for (int i = 0; i < 256; i++)
			for (int j = 0; j < ((gray[i] * 1.0 / max) * 511); j++)
			{
				temp.at < uchar >(511 - j, i * 2) = 255;
				temp.at < uchar >(511 - j, i * 2 + 1) = 255;
			}
		imshow("Gray histogram ", temp);
		temp.release();
	}
	else if (act->text() == "ֱ��ͼ����")
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
		// �Ҷȱ任
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
		Mat temp;//��ǰͼ��
		work.copyTo(temp);
		Mat temp_src;//ԭͼ��
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
		//Scalar s= sum(sub);         // ������������֮��
		//double sse = s.val[0] + s.val[1] + s.val[2]; // ����ǲ�ɫͼ�����ͨ����

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
		Mat temp;//��ǰͼ��
		work.copyTo(temp);
		Mat temp_src;//ԭͼ��
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
	else if (act->text() == "��������������")
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
		//temp_avg���ڴ洢���򣬻Ҷ�ֵΪ��ƽ��ֵ
		//temp_border���ڴ洢�߽磬ֻ��0/255
		//mark���ڴ洢�Ѿ������ĵ�
		unsigned char  ***A;
		int num_A = 0, threshold = 40;//num_A��¼A�ļ��ϸ�������0��ʼ
		double num_mark = 0, theta;//���ڴ洢��ǵĵ�ĸ�����theta��ֵ

		A = new unsigned char **[work.rows*work.cols];//����һ����ά�������ڴ洢A0-AK
		A[num_A] = new unsigned char*[work.rows];
		for (int i = 0; i < work.rows; i++)
			A[num_A][i] = new unsigned char[work.cols];

		for (int i = 0; i < work.rows; i++)//��ʼ������A0
			for (int j = 0; j < work.cols; j++)
				A[num_A][i][j] = 0;

		double *ave_A, *pix_in_A, *sun_A;

		ave_A = new double[work.rows*work.cols];//����һ�� ��ά����洢ƽ��ֵ
		for (int i = 0; i < work.rows*work.cols; i++)//��ʼ��
			ave_A[i] = 0;

		pix_in_A = new double[work.rows*work.cols];//����һ�� ��ά����洢AK�е�ĸ���
		for (int i = 0; i < work.rows*work.cols; i++)//��ʼ��
			pix_in_A[i] = 0;

		sun_A = new double[work.rows*work.cols];//����һ�� ��ά����洢AK�е����ֵ
		for (int i = 0; i < work.rows*work.cols; i++)//��ʼ��
			sun_A[i] = 0;


		std::cout << work.cols / 2 + 1 << ' ' << work.rows / 2 << std::endl;

		A[num_A][work.cols / 2][work.rows / 2] = 255;//���м������Ϊ��ʼ�����ӵ�
		pix_in_A[num_A]++;//A0�У���ĸ�����1
		sun_A[num_A] += work.at<uchar>(work.cols / 2, work.rows / 2);//A0�У�������غͼ��ϳ�ʼ��
		ave_A[num_A] = sun_A[num_A] / pix_in_A[num_A];//����A0�У�����ƽ��ֵ
		mark.at<uchar>(work.cols / 2, work.rows / 2) = 255;//��mark�н���ʼ��mark,��ʾ�Ѿ�����
		cal_border(mark, temp_border, work.cols / 2, work.rows / 2);//�޸�border
		std::cout << "��һ�����" << std::endl;
		for (int i = 0; i < temp_border.cols; i++)
			for (int j = 0; j < temp_border.rows; j++)
				if (temp_border.at<uchar>(j, i) == 255)
					std::cout << j << ' ' << i << std::endl;


		A[num_A][work.cols / 2 + 1][work.rows / 2] = 255;//���м������Ϊ��ʼ�����ӵ�
		mark.at<uchar>(work.cols / 2 + 1, work.rows / 2) = 255;//��mark�н���ʼ��mark,��ʾ�Ѿ�����
		temp_border.at<uchar>(work.cols / 2 + 1, work.rows / 2) = 0;//��mark�н���ʼ��mark,��ʾ�Ѿ�����
		cal_border(mark, temp_border, work.cols / 2 + 1, work.rows / 2);//�޸�border
		std::cout << "�ڶ������" << std::endl;
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
		while (1)//��ͣѭ����֪���������еĵ�
		{

			num_mark++;//ѭ��һ���һ�������A
			double theta_min = 255;
			int x_min, y_min, temp_k;

			for (int i = 0; i < temp_border.cols; i++)
				for (int j = 0; j < temp_border.rows; j++)
					if (temp_border.at<uchar>(j, i) == 255)
					{
						for (int k = 0; k <= num_A; k++)//�������еļ���A
						{
							//�ҵ���T�ϵ���С��thetaֵ
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
				mark.at<uchar>(y_min, x_min) = 255;//�޸�����A�ļ���
				temp_border.at<uchar>(y_min, x_min) = 0;
				A[temp_k][y_min][x_min] = 255;//�޸�����AK
				pix_in_A[temp_k]++;//A0�У���ĸ�����1
				sun_A[temp_k] += work.at<uchar>(y_min, x_min);//A0�У�������غͼ��ϳ�ʼ��
				ave_A[temp_k] = sun_A[temp_k] / pix_in_A[temp_k];//����A0�У�����ƽ��ֵ	
				cal_border(mark, temp_border, y_min, x_min);//�޸�border
				//std::cout << temp_k << ' ' << x_min << ' ' << y_min << ' ' << pix_in_A[temp_k] << ' ' << sun_A[temp_k] << std::endl;
			}
			else
			{
				//����һ���µ�A����
				num_A++;
				A[num_A] = new unsigned char*[work.rows];
				for (int i = 0; i < work.rows; i++)
					A[num_A][i] = new unsigned char[work.cols];

				for (int i = 0; i < work.rows; i++)//��ʼ������A0
					for (int j = 0; j < work.cols; j++)
						A[num_A][i][j] = 0;

				mark.at<uchar>(y_min, x_min) = 255;//�޸�����A�ļ���
				temp_border.at<uchar>(y_min, x_min) = 0;
				A[num_A][y_min][x_min] = 255;//�޸�����AK
				pix_in_A[num_A]++;//A0�У���ĸ�����1
				sun_A[num_A] += work.at<uchar>(y_min, x_min);//A0�У�������غͼ��ϳ�ʼ��
				ave_A[num_A] = sun_A[num_A] / pix_in_A[num_A];//����A0�У�����ƽ��ֵ
				cal_border(mark, temp_border, y_min, x_min);//�޸�border

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
					for (int k = 0; k <= num_A; k++)//�������еļ���A
					{
						//�ҵ���T�ϵ���С��thetaֵ
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
				if (mark.at<uchar>(i, j) != 0)//����Ѿ����������
					continue;
				double num, sum = 0;//���ڴ洢��ǰ��������ص��������ܵĻҶȺ�
				num = 1;
				Mat A = Mat::zeros(Size(work.rows, work.cols), 0);//����һ���µļ���
				Mat border = Mat::zeros(Size(work.rows, work.cols), 0);//�������ϱ߽�ļ���
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
double generateGaussianNoise(double mu, double sigma)//�����˹����muΪ��ֵ��sigmaΪ����
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
