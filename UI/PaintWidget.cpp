#include "PaintWidget.h"
#include <QtGui>
#include <QPen>

PaintWidget::PaintWidget(QWidget *parent) :
	QWidget(parent)
{
	setAutoFillBackground(true);    //�Դ��屳��ɫ������
	pix = new QPixmap(size());      	//��QPixmap��������׼����ʱ���ջ��Ƶ�����
	pix->fill(Qt::transparent);
	this->style = static_cast<int>(Qt::SolidLine);
	this->width = 2;
	this->color = Qt::blue;
}

void PaintWidget::setWidth(int w)
{
	width = w;
}

void PaintWidget::setColor(QColor c)
{
	color = c;
}

void PaintWidget::mousePressEvent(QMouseEvent *e)
{
	startPos = e->pos();
}

void PaintWidget::mouseMoveEvent(QMouseEvent *e)
{
	QPainter *painter = new QPainter;

	QPen pen;
	pen.setStyle((Qt::PenStyle)style);
	pen.setWidth(width);
	pen.setColor(color);

	painter->begin(pix);
	painter->setPen(pen);
	painter->drawLine(startPos, e->pos());
	painter->end();
	startPos = e->pos();
	update();
}

void PaintWidget::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.drawPixmap(QPoint(0, 0), *pix);
}

void PaintWidget::clear()
{
	QPixmap *clearPix = new QPixmap(size());
	clearPix->fill(Qt::transparent);
	pix = clearPix;
	update();
}
