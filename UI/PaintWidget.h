#pragma once

#include <QWidget>
#include <QtGui>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QColor>
#include <QPixmap>
#include <QPoint>
#include <QPainter>
#include <QPalette>

class PaintWidget : public QWidget
{
	Q_OBJECT

public:
	PaintWidget(QWidget *parent = 0);
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void paintEvent(QPaintEvent *);
	QPixmap *pix;

public slots:
	void setWidth(int);
	void setColor(QColor);
	void clear();

private:
	
	QPoint startPos;
	QPoint endPos;
	int style;
	int width;
	QColor color;
};