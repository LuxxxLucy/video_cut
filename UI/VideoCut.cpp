#include "VideoCut.h"
#include "VideoDecoder.h"
#include <QFileDialog>
#include <QFile>
#include <QByteArray>
#include <QColorDialog>
#include <QFileInfo>
#include <opencv2/core/core.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include <dirent.h>
#include <boost/algorithm/string.hpp>

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

	//"�����ؼ�֡"����
	SaveBitMapAction = new QAction(QIcon("Resources/save_bitmap.png"), tr("�����ؼ�֡"), this);
	SaveBitMapAction->setShortcut(tr("Ctrl+S"));
	SaveBitMapAction->setStatusTip(tr("�����ؼ�֡"));
	connect(SaveBitMapAction, SIGNAL(triggered()), this, SLOT(SaveBitMap()));

	//"������ȡ����"����
	openCutVideoFileAction = new QAction(QIcon("Resources/open_cut_video.png"), tr("������ȡ����"), this);
	openCutVideoFileAction->setShortcut(tr("Ctrl+W"));
	openCutVideoFileAction->setStatusTip(tr("������ȡ����"));
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
	widthLabel = new QLabel(tr("�߿���"));    		//�����߿�ѡ���ؼ�
	widthSpinBox = new QSpinBox;
	widthSpinBox->setValue(2);
	connect(widthSpinBox, SIGNAL(valueChanged(int)), centerWidget, SLOT(setWidth(int)));

	colorBtn = new QToolButton;                  //������ɫѡ���ؼ�
	QPixmap pixmap(20, 20);
	pixmap.fill(Qt::blue);
	colorBtn->setIcon(QIcon(pixmap));
	connect(colorBtn, SIGNAL(clicked()), this, SLOT(ShowColor()));

	clearBtn = new QToolButton();               	//����������ť
	clearBtn->setText(tr("����"));
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
	QFileDialog* fd = new QFileDialog(this, "ѡ���ؼ�֡�ļ���", "file");
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
	QColor color = QColorDialog::getColor(static_cast<int>(Qt::blue), this);	//ʹ�ñ�׼��ɫ�Ի���QColorDialog����һ����ɫֵ
	if (color.isValid())
	{
		centerWidget->setColor(color);
		QPixmap p(20, 20);
		p.fill(color);
		colorBtn->setIcon(QIcon(p));
	}
}

void optimize_3_frame(int i,std::vector<cv::Mat> images,std::vector<cv::Mat> labels)
{
	labels[i+1]=labels[i];
	return;
}

std::vector<cv::Mat> read_video2Mat(std::string video_path)
{
	std::vector<cv::Mat> images;

	std::cout<<"reading!"<<std::endl;

	//打开视频文件：其实就是建立一个VideoCapture结构
    cv::VideoCapture capture(video_path);

	std::cout<<"reading!"<<std::endl;
    //检测是否正常打开:成功打开时，isOpened返回ture
    if(!capture.isOpened())
        std::cout<<"fail to open!"<<std::endl;

	std::cout<<"reading!"<<std::endl;
    int totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);
	std::cout<<"reading!"<<std::endl;


    int frameToStart = 0;
    capture.set( CV_CAP_PROP_POS_FRAMES,frameToStart);
    std::cout<<" total "<<totalFrameNumber<< " start from "<< frameToStart <<std::endl;

    int frameToStop = totalFrameNumber;

    if(frameToStop < frameToStart)
    {
        std::cout<<" end frame number smaller than start"<<std::endl;
        return images;
    }
    else
    {
        std::cout<<"end frame as "<<frameToStop<<std::endl;
    }
    //获取帧率
    double rate = capture.get(CV_CAP_PROP_FPS);
    std::cout<<"frame rate"<<rate<<std::endl;

    //定义一个用来控制读取视频循环结束的变量
    bool stop = false;
    //承载每一帧的图像
    cv::Mat frame;
    //显示每一帧的窗口
    //两帧间的间隔时间:
    //int delay = 1000/rate;
    int delay = 1000/rate;

    //利用while循环读取帧
    //currentFrame是在循环体中控制读取到指定的帧后循环结束的变量
    long currentFrame = frameToStart;

    //滤波器的核
    int kernel_size = 3;
    cv::Mat kernel = cv::Mat::ones(kernel_size,kernel_size,CV_32F)/(float)(kernel_size*kernel_size);

    while(!stop)
    {
        //读取下一帧
        if(!capture.read(frame))
        {
            std::cout<<"read video failed"<<std::endl;
            return images;
        }

        //这里加滤波程序
        filter2D(frame,frame,-1,kernel);
		std::cout<<frame.channels()<<std::cout;
		images.push_back(frame.clone());

        currentFrame++;
		if(currentFrame>=frameToStop)
			stop=true;

    }
    //关闭视频文件
    capture.release();
    return images;
}

std::vector<int> read_keyframes(std::string dirname,std::vector<cv::Mat> labels)
{
	std::string path = dirname + "/";
	std::vector<int> index;

	//load image
	DIR *dp;
	struct dirent *dirp;
	if((dp=opendir(path.c_str()))==NULL){
		perror("opendir error");
		free(dp);
		exit(1);
	}


	while((dirp=readdir(dp))!=NULL){
		if((strcmp(dirp->d_name,".")==0)||(strcmp(dirp->d_name,"..")==0))
			continue;
		std::string fname = path+dirp->d_name;
		std::vector<std::string> fields;

		boost::split( fields, fname, boost::is_any_of( "." ) );
		index.push_back( (int)std::atof(fields[0].c_str() ) );
		cv::Mat img = cv::imread(fname,CV_LOAD_IMAGE_GRAYSCALE);
		if (img.empty()) {
			continue;
		}
		labels.push_back(img);
	}
	closedir(dp);

	return index;
}

void merge_and_save(std::vector<cv::Mat> images,std::vector<cv::Mat> labels)
{
	for (int s=0;s<=labels.size();s++)
	{
		for(int i = 0; i < labels[s].rows; i++)
    		for(int j = 0; j < labels[s].cols; j++)
			{
        		if(labels[s].at<double>(i,j)!=0)
				{
					images[s].at<double>(i,j,0)=0;
					images[s].at<double>(i,j,1)=0;
					images[s].at<double>(i,j,2)=0;
					images[s].at<double>(i,j,3)=0;
				}

			}

	}
}

void VideoCut::doVideoCut() {
	//Դ��Ƶ���ļ����� this->fileFull��ȡ  ����ͼ�ļ�����this->centerWidget->getSavePath()��ȡ Ŀ���ļ����û�û��ָ�������ͷ��ڰ���֮ǰ�����Ʒ���data��������ĳ��Ŀ¼�����ɣ����Լ���

	std::string video_path="../data/news.avi";
	std::cout<< " video path is "<< video_path <<std::endl;
	std::string keyframes_path="../data/source/keyframe/news.avi";
	// read cut of keyframes
	std::vector<cv::Mat> labels;
	std::vector<cv::Mat> images=read_video2Mat(video_path);
	std::cout<< " key frames path is "<< keyframes_path <<std::endl;
	std::vector<int> keyframe_indexs=read_keyframes(keyframes_path,labels);

	// assign labels fo keyframes
	for(int i=0;i<keyframe_indexs.size();i++)
	{
		// assign values to labels
	}

	for(int i=0;i<keyframe_indexs.size()-1;i++)
	{
		int start_index=keyframe_indexs[i],end_index=keyframe_indexs[i+1];
		int interval=end_index-start_index;
		#define MAX_ITERATION_NUM 10
		for(int iter=0;iter<MAX_ITERATION_NUM;iter++)
		{
			for(int i=0;i<interval;i++)
			{
				optimize_3_frame(i+start_index,images,labels);
			}
		}
	}

	// read all frames
	merge_and_save(images,labels);
}
void VideoCut::doVideoPaste() {


}
