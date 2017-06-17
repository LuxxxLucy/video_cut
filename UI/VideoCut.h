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

	void createActions();                        	//��������
	void createMenus();                           	//�����˵�
	void createToolBars();                      	//����������
	static void doVideoCut();
	static void doVideoPaste();

private:
	QMenu *fileMenu;                           		//�����˵���
	QMenu *execMenu;
	QMenu *editMenu;

	QString fileName;
	QString filePath;
	QString fileFull;
	CenterWidget *centerWidget;

	QAction *openVideoFileAction;                   //�ļ��˵���
	QAction *openBitMapAction;
	QAction *SaveBitMapAction;
	QAction *openCutVideoFileAction;
	QAction *openBgVideoFileAction;
	QAction *exitAction;

	QAction *videoCutOutAction;
	QAction *videoPasteAction;

	QAction *undoAction;
	QAction *redoAction;

	QToolBar *fileTool;                          	//������
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

};
