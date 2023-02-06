#include "Qt_OpenCV.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Qt_OpenCV w;
	w.showMaximized();
//	w.showFullScreen();
	w.setStyleSheet(QString("QWidget{padding:0px;margin:0px;border:0px;}"));
	w.show();
	return a.exec();
}
