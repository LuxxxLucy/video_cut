#include "VideoCut.h"
#include "VideoDecoder.h"
#include <QFileDialog>
#include <QFile>
#include <QByteArray>
#include <QColorDialog>
#include <QFileInfo>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include <dirent.h>
#include <boost/algorithm/string.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <algorithm>
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

void optimize_3_frame(int k,std::vector<cv::Mat> images,std::vector<cv::Mat> &labels)
{
	labels[k+1]=labels[k];
	cv::Mat temp_label=labels[k+1].clone();
	int row = temp_label.rows;
	int col = temp_label.cols;
	int max_value=0;
	//用膨胀的方法，将上一张的foreground进行膨胀
	for(int i=0 ; i<row ; i++){
		for(int j=0 ; j<col ; j++){
			max_value=0;
			for(int x=0 ; x<11&&x+i<row ; x++){
				for(int y=0 ; y<11&&y+j<col ; y++){
					max_value=(max_value<labels[k+1].at<uchar>(i+x,j+y) ? labels[k+1].at<uchar>(i+x,j+y):max_value);
				}
			}
			temp_label.at<uchar>(i,j)=(max_value==0 ? 2 : 3);
		}
	}
	labels[k+1]=temp_label.clone();
	cv::Rect rect(0,0,images[k+1].cols-1,images[k+1].rows-1);
	cv::Mat image_copy = images[k+1].clone();
	cv::Mat bgdModel,fgdModel;
	cv::grabCut(image_copy,labels[k+1],rect,bgdModel,fgdModel,20,cv::GC_INIT_WITH_MASK);
	std::cout << k << std::endl;
	for(int i=0;i<row;i++){
		for(int j=0;j<col ;j++){
			labels[k+1].at<uchar>(i,j)&=0x1;
		}
	}
	return;
}

std::vector<cv::Mat> read_video2Mat(std::string video_path)
{
	std::vector<cv::Mat> images_temp;
	std::vector<cv::Mat> images;
	std::string cmd="ffmpeg -i "+video_path+" -q:v 2  data/source/Video/%d.png";
	// system(cmd.c_str());

	std::string path = "data/source/Video/";
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
		boost::split( fields, dirp->d_name, boost::is_any_of( "." ) );
		int i=(int)std::atof(fields[0].c_str() );
		index.push_back(i);
		cv::Mat img = cv::imread(fname,CV_LOAD_IMAGE_COLOR);
		if (img.empty()) {
			continue;
		}
		images_temp.push_back(img.clone());
	}
	closedir(dp);

	std::cout<<" video decoding okay! frame number "<<images_temp.size()<<index.size()<<std::endl;

	images.resize(images_temp.size());
	for(int i =0;i<index.size();i++)
	{
		images[index[i]-1] = images_temp[i].clone();
	}

	return images;

}

std::vector<int> read_keyframes(std::string dirname,std::vector<cv::Mat>& labels)
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

		boost::split( fields, dirp->d_name, boost::is_any_of( "." ) );
		index.push_back( (int)std::atof(fields[0].c_str() ) );
		cv::Mat img = cv::imread(fname,CV_LOAD_IMAGE_GRAYSCALE);
		std::cout << img.channels() << '\n';

		if (img.empty()) {
			continue;
		}
		labels.push_back(img.clone());
	}
	closedir(dp);

	std::cout << "total key frame number" << labels.size()<< '\n';

	return index;
}

void merge_and_save(std::vector<cv::Mat> images,std::vector<cv::Mat> labels)
{
	std::cout << (images[0].type()==CV_32FC3?0:1) << 	'\n';
	std::cout << labels[0].type() << '\n';
	std::cout << "now start saving!"  << '\n';
	for (int s=0;s<labels.size();s++)
	{
		for(int i = 0; i < images[s].rows; i++)
		{
    		for(int j = 0; j < images[s].cols; j++)
			{
        		if(labels[s].at<uchar>(i,j)==0)
				{
					images[s].at<cv::Vec3b>(i,j)[0]=0;
					images[s].at<cv::Vec3b>(i,j)[1]=0;
					images[s].at<cv::Vec3b>(i,j)[2]=0;
					// images[s].at<int>(i,j,0)=0;
					// images[s].at<int>(i,j,1)=0;
					// images[s].at<int>(i,j,2)=0;
				}
			}
		}
		imwrite( "data/source/backVideo/"+std::to_string(s)+".png", images[s]);
	}
	std::cout << "image saving okay saving!"  << '\n';
	std::string cmd= "ffmpeg -start_number 0 -i data/source/backVideo/%d.png -vcodec mpeg4 test.avi";

	// system(cmd.c_str());

}

void VideoCut::initial(cv::Mat label,cv::Mat &image)
{
	int erosion_elem=2;
	int erosion_type;
	if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
	else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
	else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

	int erosion_size=10;
	cv::Mat element = cv::getStructuringElement( erosion_type, Size( 2*erosion_size + 1, 2*erosion_size+1 ), cv::Point( erosion_size, erosion_size ) );
	/// Apply the erosion operation
	cv::Mat temp;
	erode( label, temp, element );
	for
}

void VideoCut::doVideoCut() {
	//Դ��Ƶ���ļ����� this->fileFull��ȡ  ����ͼ�ļ�����this->centerWidget->getSavePath()��ȡ Ŀ���ļ����û�û��ָ�������ͷ��ڰ���֮ǰ�����Ʒ���data��������ĳ��Ŀ¼�����ɣ����Լ���

	std::string video_path="./data/news.avi";
	std::string keyframes_path="./data/source/keyframe/news.avi";
	// read cut of keyframes
	std::vector<cv::Mat> labels,temp_labels;
	std::cout<< " key frames path is "<< keyframes_path <<std::endl;
	std::vector<int> keyframe_indexs=read_keyframes(keyframes_path,temp_labels);
	std::sort(keyframe_indexs.begin(),keyframe_indexs.end());
	std::cout<< " preprocessing okay,labelled frames "<< temp_labels.size() <<std::endl;

	std::cout<< " video path is "<< video_path <<std::endl;
	std::vector<cv::Mat> images=read_video2Mat(video_path);
	std::cout<< " preprocessing okay,total frames "<< images.size() <<std::endl;

	std::cout << temp_labels[0].channels() << '\n';
	// assign labels fo keyframes
	for(int i=0;i<images.size();i++)
	{
		labels.push_back( cv::Mat::zeros(images[0].rows,images[0].cols,CV_8UC1));
	}
	//对于temp_labels进行改变
	for(int i=0;i<temp_labels.size();i++)
	{
		cv::Mat change_labels=cv::Mat::zeros(temp_labels[0].rows,temp_labels[0].cols,CV_8UC1);
		for(int x=0;x<temp_labels[i].rows;x++)
		{
			bool booll=false;
			int flag=2;
			for(int y=0;y<temp_labels[i].cols;y++)
			{
				change_labels.at<uchar>(x,y)=flag;
				if(temp_labels[i].at<uchar>(x,y)>0)
				{
					if(!booll){
						booll=true;
					}
				}else{
					if(booll)
					{
						if(flag==2)
							flag=3;
						else
							flag=2;
					}
					booll=false;
				}
			}
		}
		temp_labels[i]=change_labels.clone();
	}
	for(int i=0;i<keyframe_indexs.size();i++)
	{
		labels[keyframe_indexs[i]]=temp_labels[i].clone();
		cv::Rect rect(0,0,images[i+1].cols,images[i+1].rows);
		cv::Mat image_copy = images[keyframe_indexs[i]].clone();
		cv::Mat bgdModel,fgdModel;
		cv::grabCut(image_copy,labels[keyframe_indexs[i]],rect,bgdModel,fgdModel,20,cv::GC_INIT_WITH_MASK);
		for(int j=0;j<image_copy.rows;j++){
			for(int k=0;k<image_copy.cols ;k++){
				labels[keyframe_indexs[i]].at<uchar>(j,k)&=0x1;
			}
		}
	}
	std::cout<< " label initial okay " <<std::endl;
	for(int i=0;i<keyframe_indexs.size()-1;i++)
	{
		int start_index=keyframe_indexs[i],end_index=keyframe_indexs[i+1];
		// if(start_index>)
		// 	break;
		int interval=end_index-start_index;
		#define MAX_ITERATION_NUM 1
		for(int iter=0;iter<MAX_ITERATION_NUM;iter++)
		{
			for(int i=0;i<interval-1;i++)
			{
				std::cout << "do optimize_3_frame" << std::endl;
				optimize_3_frame(i+start_index,images,labels);
			}
		}
	}
	//keyframe的最后一针不是视频的最后一帧，所以增加这段代码
	// int start_index=keyframe_indexs[keyframe_indexs.size()-1],end_index=images.size();
	// for(int i=0;i<end_index-start_index-1;i++){
	// 	optimize_3_frame(i+start_index,images,labels);
	// }
	// read all frames
	std::cout << "cut okay!" << '\n';
	merge_and_save(images,labels);
}
void VideoCut::doVideoPaste() {


}
