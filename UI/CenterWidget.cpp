#include "CenterWidget.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>
CenterWidget::CenterWidget(QWidget *parent)
	: QWidget(parent)
{
	list = new QListWidget(this);
	stack = new QStackedWidget(this);
	width = 2;
	color = Qt::blue;
	QHBoxLayout *mainLayout = new QHBoxLayout(this);
	mainLayout->setMargin(5);
	mainLayout->setSpacing(5);
	mainLayout->addWidget(list);
	mainLayout->addWidget(stack, 0, Qt::AlignHCenter);
	mainLayout->setStretchFactor(list, 1);
	mainLayout->setStretchFactor(stack, 3); //���ұ���Ϊ1:3
	connect(list, SIGNAL(currentRowChanged(int)), stack, SLOT(setCurrentIndex(int)));
	connect(list, SIGNAL(currentRowChanged(int)), this, SLOT(currentChange(int)));
}

CenterWidget::~CenterWidget()
{
}

void CenterWidget::setWidth(int w)
{
	width = w;
	PaintWidget* current = (PaintWidget*)stack->currentWidget();
	current->setWidth(w);
}

void CenterWidget::setColor(QColor c)
{
	color = c;
	PaintWidget* current = (PaintWidget*)stack->currentWidget();
	current->setColor(c);
}

void CenterWidget::clear()
{
	PaintWidget* current = (PaintWidget*)stack->currentWidget();
	current->clear();
}

void CenterWidget::currentChange(int index) {
	PaintWidget* current = (PaintWidget*)stack->currentWidget();
	current->setWidth(width);
	current->setColor(color);
}

void CenterWidget::setSavePath(QString path) {
	savePath = path;
}

QString CenterWidget::getSavePath() {
	return this->savePath;
}

void CenterWidget::saveBitmap() {
	PaintWidget* current = (PaintWidget*)stack->currentWidget();
	QString filename = list->currentItem()->text();
	list->currentItem()->setTextColor(Qt::green);
	QPixmap *pixmap = current->pix;
	QDir dir(savePath);
	if (!dir.exists()) {
		dir.mkpath(savePath);
	}
	pixmap->save(savePath + filename, "png");
}

bool CenterWidget::loadBitmap() {
	QDir dir(savePath);
	if (!dir.exists())
	{
		return false;
	}

	QStringList filters;
	//�ļ�����
	filters<<QString("*.bmp") << QString("*.png");
	//���������������ù�����
	qDebug() << savePath;
	QDirIterator dir_iterator(savePath, filters, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);

	while (dir_iterator.hasNext())
	{
		dir_iterator.next();
		QFileInfo file_info = dir_iterator.fileInfo();
		QString name = file_info.fileName();
		int index = 0;
		for (; index < list->count(); index++) {
			QString cmp = list->item(index)->text();
			if (QString::compare(cmp, name) == 0) {
				break;
			}
		}
		stack->setCurrentIndex(index);
		PaintWidget* current = (PaintWidget*)stack->currentWidget();
		QPixmap *tmp = new QPixmap(file_info.absoluteFilePath());
		current->pix = tmp;
		update();
	}
	return 0;
}
