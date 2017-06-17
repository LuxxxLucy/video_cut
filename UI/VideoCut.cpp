#include "VideoCut.h"
#include "VideoDecoder.h"
#include <QFileDialog>
#include <QFile>
#include <QByteArray>
#include <QColorDialog>
#include <QFileInfo>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

VideoCut::VideoCut(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowTitle(tr("Video Cut Out"));
	centerWidget = new CenterWidget(this);
	setCentralWidget(centerWidget);

	createActions();
	createMenus();
	createToolBars();
}
VideoCut::~VideoCut() {

}
void VideoCut::createActions()
{
	//"����Դ�ļ�"����
	openVideoFileAction = new QAction(QIcon("Resources/open_orivideo.png"), tr("����Դ��Ƶ"), this);
	openVideoFileAction->setShortcut(tr("Ctrl+O"));
	openVideoFileAction->setStatusTip(tr("����Դ��Ƶ"));
	connect(openVideoFileAction, SIGNAL(triggered()), this, SLOT(ShowOriginalVideoFile()));

	//"�������йؼ�֡"����
	openBitMapAction = new QAction(QIcon("Resources/open_bitmap.png"), tr("�������йؼ�֡"), this);
	openBitMapAction->setShortcut(tr("Ctrl+M"));
	openBitMapAction->setStatusTip(tr("�������йؼ�֡"));
	connect(openBitMapAction, SIGNAL(triggered()), this, SLOT(ShowBitMap()));

	//"����ؼ�֡"����
	SaveBitMapAction = new QAction(QIcon("Resources/save_bitmap.png"), tr("����ؼ�֡"), this);
	SaveBitMapAction->setShortcut(tr("Ctrl+S"));
	SaveBitMapAction->setStatusTip(tr("����ؼ�֡"));
	connect(SaveBitMapAction, SIGNAL(triggered()), this, SLOT(SaveBitMap()));

	//"�����ȡ���"����
	openCutVideoFileAction = new QAction(QIcon("Resources/open_cut_video.png"), tr("�����ȡ���"), this);
	openCutVideoFileAction->setShortcut(tr("Ctrl+W"));
	openCutVideoFileAction->setStatusTip(tr("�����ȡ���"));
	connect(openCutVideoFileAction, SIGNAL(triggered()), this, SLOT(ShowVideoCutFile()));

	//"���뱳����Ƶ"����
	openBgVideoFileAction = new QAction(QIcon("Resources/open_bg_video.png"), tr("���뱳����Ƶ"), this);
	openBgVideoFileAction->setShortcut(tr("Ctrl+B"));
	openBgVideoFileAction->setStatusTip(tr("���뱳����Ƶ"));
	connect(openBgVideoFileAction, SIGNAL(triggered()), this, SLOT(ShowBgVideoFile()));

	//"videocut"����
	videoCutOutAction = new QAction(QIcon("Resources/cut.png"), tr("��ȡ��Ƶ����"), this);
	videoCutOutAction->setShortcut(tr("Ctrl+X"));
	videoCutOutAction->setStatusTip(tr("��ȡ��Ƶ����"));
	connect(videoCutOutAction, SIGNAL(triggered()), this, SLOT(doVideoCut()));

	//"videopaste"����
	videoPasteAction = new QAction(QIcon("Resources/paste.png"), tr("ճ����Ŀ����Ƶ"), this);
	videoPasteAction->setShortcut(tr("Ctrl+V"));
	videoPasteAction->setStatusTip(tr("ճ����Ŀ����Ƶ"));
	connect(videoPasteAction, SIGNAL(triggered()), this, SLOT(doVideoPaste()));

	//"�˳�"����
	exitAction = new QAction(QIcon("Resources/exit.png"),tr("�˳�"), this);
	exitAction->setShortcut(tr("Ctrl+Q"));
	exitAction->setStatusTip(tr("�˳�����"));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

	//ʵ�ֳ����������Ķ�����Action��
	//����������
	undoAction = new QAction(QIcon("Resources/undo.png"), "����", this);
	//connect(undoAction, SIGNAL(triggered()), showWidget->text, SLOT(undo()));
	redoAction = new QAction(QIcon("Resources/redo.png"), "����", this);
	//connect(redoAction, SIGNAL(triggered()), showWidget->text, SLOT(redo()));
}

void VideoCut::createMenus()
{
	//�ļ��˵�
	fileMenu = menuBar()->addMenu(tr("�ļ�"));
	fileMenu->addAction(openVideoFileAction);
	fileMenu->addAction(openBitMapAction);
	fileMenu->addAction(SaveBitMapAction);
	fileMenu->addAction(openCutVideoFileAction);
	fileMenu->addAction(openBgVideoFileAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);

	editMenu = menuBar()->addMenu(tr("�༭"));
	editMenu->addAction(undoAction);
	editMenu->addAction(redoAction);

	execMenu = menuBar()->addMenu(tr("����"));
	execMenu->addAction(videoCutOutAction);
	execMenu->addAction(videoPasteAction);

}

void VideoCut::createToolBars()
{
	//�ļ�������
	fileTool = addToolBar("File");
	fileTool->addAction(openVideoFileAction);
	fileTool->addAction(openBitMapAction);
	fileTool->addAction(SaveBitMapAction);
	fileTool->addAction(openCutVideoFileAction);
	fileTool->addAction(openBgVideoFileAction);

	//����������������
	doToolBar = addToolBar("doEdit");
	doToolBar->addAction(undoAction);
	doToolBar->addAction(redoAction);

	execToolBar = addToolBar("run");
	execToolBar->addAction(videoCutOutAction);
	execToolBar->addAction(videoPasteAction);

	paintToolBar = addToolBar("paint");
	widthLabel = new QLabel(tr("�߿�"));    		//�����߿�ѡ��ؼ�
	widthSpinBox = new QSpinBox;
	widthSpinBox->setValue(2);
	connect(widthSpinBox, SIGNAL(valueChanged(int)), centerWidget, SLOT(setWidth(int)));

	colorBtn = new QToolButton;                  //������ɫѡ��ؼ�
	QPixmap pixmap(20, 20);
	pixmap.fill(Qt::blue);
	colorBtn->setIcon(QIcon(pixmap));
	connect(colorBtn, SIGNAL(clicked()), this, SLOT(ShowColor()));

	clearBtn = new QToolButton();               	//���������ť
	clearBtn->setText(tr("���"));
	connect(clearBtn, SIGNAL(clicked()), centerWidget, SLOT(clear()));

	paintToolBar->addWidget(widthLabel);
	paintToolBar->addWidget(widthSpinBox);
	paintToolBar->addWidget(colorBtn);
	paintToolBar->addWidget(clearBtn);
}

void VideoCut::ShowOriginalVideoFile()
{
	fileFull = QFileDialog::getOpenFileName(this, "����Դ��Ƶ", "/", "Video files(*.avi;*.mp4;*.mkv;*.rmvb;*.mov;*.wmv)");
	QFileInfo finfo = QFileInfo(fileFull);
	fileName = finfo.fileName();
	filePath = finfo.absolutePath();
	this->centerWidget->setSavePath("./data/source/keyframe/" + fileName + "/");
	if (!fileName.isEmpty())
	{
		QByteArray temp = fileFull.toLatin1();
		if (DecodeVideo(temp.data(),this->centerWidget) == -1) {
			printf("failed to decode video!");
		}
	}
}
void VideoCut::ShowBitMap() {
	QFileDialog* fd = new QFileDialog(this, "ѡ��ؼ�֡�ļ���", "file");
	fd->setFileMode(QFileDialog::Directory);
	if (fd->exec() == QDialog::Accepted)
	{
		QString path = fd->selectedFiles()[0];
		centerWidget->setSavePath(path);
		centerWidget->loadBitmap();
	}
}
void VideoCut::SaveBitMap() {
	centerWidget->saveBitmap();
}

void VideoCut::ShowVideoCutFile() {

}
void VideoCut::ShowBgVideoFile() {

}

void VideoCut::ShowColor()
{
	QColor color = QColorDialog::getColor(static_cast<int>(Qt::blue), this);	//ʹ�ñ�׼��ɫ�Ի���QColorDialog���һ����ɫֵ
	if (color.isValid())
	{
		centerWidget->setColor(color);
		QPixmap p(20, 20);
		p.fill(color);
		colorBtn->setIcon(QIcon(p));
	}
}

void VideoCut::doVideoCut() {
	//Դ��Ƶ���ļ����� this->fileFull��ȡ  ����ͼ�ļ�����this->centerWidget->getSavePath()��ȡ Ŀ���ļ����û�û��ָ������ͷ��ڰ���֮ǰ����Ʒ���data�������ĳ��Ŀ¼����ɣ����Լ���
}
void VideoCut::doVideoPaste() {

}
