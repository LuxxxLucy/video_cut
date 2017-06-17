#pragma once

#include <QMainWindow>
#include <QImage>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QComboBox>
#include <QSpinBox>
#include <QToolBar>
#include <QFontComboBox>
#include <QToolButton>
#include <QTextCharFormat>
#include "CenterWidget.h"

class VideoCut : public QMainWindow
{
	Q_OBJECT

public:
	VideoCut(QWidget *parent = 0);
	~VideoCut();

	void createActions();                        	//创建动作
	void createMenus();                           	//创建菜单
	void createToolBars();                      	//创建工具栏

private:
	QMenu *fileMenu;                           		//各项菜单栏
	QMenu *execMenu;
	QMenu *editMenu;

	QString fileName;
	QString filePath;
	QString fileFull;
	CenterWidget *centerWidget;

	QAction *openVideoFileAction;                   //文件菜单项
	QAction *openBitMapAction;
	QAction *SaveBitMapAction;
	QAction *openCutVideoFileAction;
	QAction *openBgVideoFileAction;
	QAction *exitAction;

	QAction *videoCutOutAction;
	QAction *videoPasteAction;

	QAction *undoAction;
	QAction *redoAction;

	QToolBar *fileTool;                          	//工具栏
	QToolBar *doToolBar;
	QToolBar *execToolBar;
	QToolBar *paintToolBar;

	QLabel *widthLabel;
	QSpinBox *widthSpinBox;
	QToolButton *colorBtn;
	QToolButton *clearBtn;

protected slots:
	void ShowOriginalVideoFile();
	void ShowBitMap();
	void SaveBitMap();
	void ShowVideoCutFile();
	void ShowBgVideoFile();
	void ShowColor();

	void doVideoCut();
	void doVideoPaste();
};

