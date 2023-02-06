#pragma once

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include<QtWidgets/QScrollArea.h>
#include<windows.h>
#include <QDir>

#include <opencv2/core.hpp>  
#include<qdebug.h>

#include <opencv2\opencv.hpp> 
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#include "ui_Qt_OpenCV.h"

using namespace cv;

class Qt_OpenCV : public QMainWindow
{
	Q_OBJECT
	Mat frame_bgr, frame_rgb,work;

public:
	Qt_OpenCV(QWidget *parent = Q_NULLPTR);

	
private slots:
	void Menu_Action(QAction*);

private:
	Ui::Qt_OpenCVClass ui;

};
