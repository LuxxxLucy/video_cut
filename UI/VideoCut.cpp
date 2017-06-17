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
	//"导入源文件"动作
	openVideoFileAction = new QAction(QIcon("Resources/open_orivideo.png"), tr("导入源视频"), this);
	openVideoFileAction->setShortcut(tr("Ctrl+O"));
	openVideoFileAction->setStatusTip(tr("导入源视频"));
	connect(openVideoFileAction, SIGNAL(triggered()), this, SLOT(ShowOriginalVideoFile()));

	//"导入已有关键帧"动作
	openBitMapAction = new QAction(QIcon("Resources/open_bitmap.png"), tr("导入已有关键帧"), this);
	openBitMapAction->setShortcut(tr("Ctrl+M"));
	openBitMapAction->setStatusTip(tr("导入已有关键帧"));
	connect(openBitMapAction, SIGNAL(triggered()), this, SLOT(ShowBitMap()));

	//"保存关键帧"动作
	SaveBitMapAction = new QAction(QIcon("Resources/save_bitmap.png"), tr("保存关键帧"), this);
	SaveBitMapAction->setShortcut(tr("Ctrl+S"));
	SaveBitMapAction->setStatusTip(tr("保存关键帧"));
	connect(SaveBitMapAction, SIGNAL(triggered()), this, SLOT(SaveBitMap()));

	//"导入截取结果"动作
	openCutVideoFileAction = new QAction(QIcon("Resources/open_cut_video.png"), tr("导入截取结果"), this);
	openCutVideoFileAction->setShortcut(tr("Ctrl+W"));
	openCutVideoFileAction->setStatusTip(tr("导入截取结果"));
	connect(openCutVideoFileAction, SIGNAL(triggered()), this, SLOT(ShowVideoCutFile()));

	//"导入背景视频"动作
	openBgVideoFileAction = new QAction(QIcon("Resources/open_bg_video.png"), tr("导入背景视频"), this);
	openBgVideoFileAction->setShortcut(tr("Ctrl+B"));
	openBgVideoFileAction->setStatusTip(tr("导入背景视频"));
	connect(openBgVideoFileAction, SIGNAL(triggered()), this, SLOT(ShowBgVideoFile()));

	//"videocut"动作
	videoCutOutAction = new QAction(QIcon("Resources/cut.png"), tr("提取视频对象"), this);
	videoCutOutAction->setShortcut(tr("Ctrl+X"));
	videoCutOutAction->setStatusTip(tr("提取视频对象"));
	connect(videoCutOutAction, SIGNAL(triggered()), this, SLOT(doVideoCut()));

	//"videopaste"动作
	videoPasteAction = new QAction(QIcon("Resources/paste.png"), tr("粘贴到目标视频"), this);
	videoPasteAction->setShortcut(tr("Ctrl+V"));
	videoPasteAction->setStatusTip(tr("粘贴到目标视频"));
	connect(videoPasteAction, SIGNAL(triggered()), this, SLOT(doVideoPaste()));

	//"退出"动作
	exitAction = new QAction(QIcon("Resources/exit.png"),tr("退出"), this);
	exitAction->setShortcut(tr("Ctrl+Q"));
	exitAction->setStatusTip(tr("退出程序"));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

	//实现撤销和重做的动作（Action）
	//撤销和重做
	undoAction = new QAction(QIcon("Resources/undo.png"), "撤销", this);
	//connect(undoAction, SIGNAL(triggered()), showWidget->text, SLOT(undo()));
	redoAction = new QAction(QIcon("Resources/redo.png"), "重做", this);
	//connect(redoAction, SIGNAL(triggered()), showWidget->text, SLOT(redo()));
}

void VideoCut::createMenus()
{
	//文件菜单
	fileMenu = menuBar()->addMenu(tr("文件"));
	fileMenu->addAction(openVideoFileAction);
	fileMenu->addAction(openBitMapAction);
	fileMenu->addAction(SaveBitMapAction);
	fileMenu->addAction(openCutVideoFileAction);
	fileMenu->addAction(openBgVideoFileAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);

	editMenu = menuBar()->addMenu(tr("编辑"));
	editMenu->addAction(undoAction);
	editMenu->addAction(redoAction);

	execMenu = menuBar()->addMenu(tr("运行"));
	execMenu->addAction(videoCutOutAction);
	execMenu->addAction(videoPasteAction);

}

void VideoCut::createToolBars()
{
	//文件工具条
	fileTool = addToolBar("File");
	fileTool->addAction(openVideoFileAction);
	fileTool->addAction(openBitMapAction);
	fileTool->addAction(SaveBitMapAction);
	fileTool->addAction(openCutVideoFileAction);
	fileTool->addAction(openBgVideoFileAction);

	//撤销和重做工具条
	doToolBar = addToolBar("doEdit");
	doToolBar->addAction(undoAction);
	doToolBar->addAction(redoAction);

	execToolBar = addToolBar("run");
	execToolBar->addAction(videoCutOutAction);
	execToolBar->addAction(videoPasteAction);

	paintToolBar = addToolBar("paint");
	widthLabel = new QLabel(tr("线宽："));    		//创建线宽选择控件
	widthSpinBox = new QSpinBox;
	widthSpinBox->setValue(2);
	connect(widthSpinBox, SIGNAL(valueChanged(int)), centerWidget, SLOT(setWidth(int)));

	colorBtn = new QToolButton;                  //创建颜色选择控件
	QPixmap pixmap(20, 20);
	pixmap.fill(Qt::blue);
	colorBtn->setIcon(QIcon(pixmap));
	connect(colorBtn, SIGNAL(clicked()), this, SLOT(ShowColor()));

	clearBtn = new QToolButton();               	//创建清除按钮
	clearBtn->setText(tr("清除"));
	connect(clearBtn, SIGNAL(clicked()), centerWidget, SLOT(clear()));

	paintToolBar->addWidget(widthLabel);
	paintToolBar->addWidget(widthSpinBox);
	paintToolBar->addWidget(colorBtn);
	paintToolBar->addWidget(clearBtn);
}

void VideoCut::ShowOriginalVideoFile()
{
	fileFull = QFileDialog::getOpenFileName(this, "导入源视频", "/", "Video files(*.avi;*.mp4;*.mkv;*.rmvb;*.mov;*.wmv)");
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
	QFileDialog* fd = new QFileDialog(this, "选择关键帧文件夹", "file");
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
	QColor color = QColorDialog::getColor(static_cast<int>(Qt::blue), this);	//使用标准颜色对话框QColorDialog获得一个颜色值
	if (color.isValid())
	{
		centerWidget->setColor(color);
		QPixmap p(20, 20);
		p.fill(color);
		colorBtn->setIcon(QIcon(p));
	}
}

void VideoCut::doVideoCut() {
	//源视频的文件名用 this->fileFull获取  轮廓图文件夹用this->centerWidget->getSavePath()获取 目标文件夹用户没有指定，你就放在按照之前的设计放在data那里面的某个目录下面吧，你自己定
}
void VideoCut::doVideoPaste() {

}
