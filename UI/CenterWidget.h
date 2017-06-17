#pragma once

#include <QWidget>
#include <QListWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QColor>
#include <QDir>
#include "PaintWidget.h"
class CenterWidget : public QWidget
{
	Q_OBJECT

public:
	CenterWidget(QWidget *parent);
	~CenterWidget();
	QListWidget *list;
	QStackedWidget *stack;

public slots :
	void setWidth(int);
	void setColor(QColor);
	void currentChange(int);
	void clear();
	void saveBitmap();
	bool loadBitmap();
	void setSavePath(QString DirName);
	QString getSavePath();
private:
	int width;
	QColor color;
	QString savePath;

};
