#include "VideoCut.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	VideoCut w;
	w.show();
	return a.exec();
}
