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
#include <math.h>
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
	//"????????"????
	openVideoFileAction = new QAction(QIcon("Resources/open_orivideo.png"), tr("导入原视频"), this);
	openVideoFileAction->setShortcut(tr("Ctrl+O"));
	openVideoFileAction->setStatusTip(tr("导入原视频"));
	connect(openVideoFileAction, SIGNAL(triggered()), this, SLOT(ShowOriginalVideoFile()));

	//"???????��???"????
	openBitMapAction = new QAction(QIcon("Resources/open_bitmap.png"), tr("导入关键帧֡"), this);
	openBitMapAction->setShortcut(tr("Ctrl+M"));
	openBitMapAction->setStatusTip(tr("导入关键帧"));
	connect(openBitMapAction, SIGNAL(triggered()), this, SLOT(ShowBitMap()));

	//"????????"????
	SaveBitMapAction = new QAction(QIcon("Resources/save_bitmap.png"), tr("保存关键帧֡"), this);
	SaveBitMapAction->setShortcut(tr("Ctrl+S"));
	SaveBitMapAction->setStatusTip(tr("保存关键帧֡"));
	connect(SaveBitMapAction, SIGNAL(triggered()), this, SLOT(SaveBitMap()));

	//"???????????"????
	openCutVideoFileAction = new QAction(QIcon("Resources/open_cut_video.png"), tr("导入截取结果"), this);
	openCutVideoFileAction->setShortcut(tr("Ctrl+W"));
	openCutVideoFileAction->setStatusTip(tr("导入截取结果"));
	connect(openCutVideoFileAction, SIGNAL(triggered()), this, SLOT(ShowVideoCutFile()));

	//"?????????"????
	openBgVideoFileAction = new QAction(QIcon("Resources/open_bg_video.png"), tr("导入背景视频"), this);
	openBgVideoFileAction->setShortcut(tr("Ctrl+B"));
	openBgVideoFileAction->setStatusTip(tr("导入背景视频"));
	connect(openBgVideoFileAction, SIGNAL(triggered()), this, SLOT(ShowBgVideoFile()));

	//"videocut"????
	videoCutOutAction = new QAction(QIcon("Resources/cut.png"), tr("do video cut"), this);
	videoCutOutAction->setShortcut(tr("Ctrl+X"));
	videoCutOutAction->setStatusTip(tr("do video cut"));
	connect(videoCutOutAction, SIGNAL(triggered()), this, SLOT(doVideoCut()));

	//"videopaste"????
	videoPasteAction = new QAction(QIcon("Resources/paste.png"), tr("paste"), this);
	videoPasteAction->setShortcut(tr("Ctrl+V"));
	videoPasteAction->setStatusTip(tr("paste"));
	connect(videoPasteAction, SIGNAL(triggered()), this, SLOT(doVideoPaste()));

	//"???"????
	exitAction = new QAction(QIcon("Resources/exit.png"),tr("exit"), this);
	exitAction->setShortcut(tr("Ctrl+Q"));
	exitAction->setStatusTip(tr("exit"));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

	//???????????????????Action??
	//??????????
	undoAction = new QAction(QIcon("Resources/undo.png"), "undo", this);
	//connect(undoAction, SIGNAL(triggered()), showWidget->text, SLOT(undo()));
	redoAction = new QAction(QIcon("Resources/redo.png"), "redo", this);
	//connect(redoAction, SIGNAL(triggered()), showWidget->text, SLOT(redo()));
}

void VideoCut::createMenus()
{
	//??????
	fileMenu = menuBar()->addMenu(tr("file"));
	fileMenu->addAction(openVideoFileAction);
	fileMenu->addAction(openBitMapAction);
	fileMenu->addAction(SaveBitMapAction);
	fileMenu->addAction(openCutVideoFileAction);
	fileMenu->addAction(openBgVideoFileAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);

	editMenu = menuBar()->addMenu(tr("edit"));
	editMenu->addAction(undoAction);
	editMenu->addAction(redoAction);

	execMenu = menuBar()->addMenu(tr("execute"));
	execMenu->addAction(videoCutOutAction);
	execMenu->addAction(videoPasteAction);

}

void VideoCut::createToolBars()
{
	//?????????
	fileTool = addToolBar("File");
	fileTool->addAction(openVideoFileAction);
	fileTool->addAction(openBitMapAction);
	fileTool->addAction(SaveBitMapAction);
	fileTool->addAction(openCutVideoFileAction);
	fileTool->addAction(openBgVideoFileAction);

	//????????????????
	doToolBar = addToolBar("doEdit");
	doToolBar->addAction(undoAction);
	doToolBar->addAction(redoAction);

	execToolBar = addToolBar("run");
	execToolBar->addAction(videoCutOutAction);
	execToolBar->addAction(videoPasteAction);

	paintToolBar = addToolBar("paint");
	widthLabel = new QLabel(tr("线宽"));    		//?????????????
	widthSpinBox = new QSpinBox;
	widthSpinBox->setValue(2);
	connect(widthSpinBox, SIGNAL(valueChanged(int)), centerWidget, SLOT(setWidth(int)));

	colorBtn = new QToolButton;                  //?????????????
	QPixmap pixmap(20, 20);
	pixmap.fill(Qt::blue);
	colorBtn->setIcon(QIcon(pixmap));
	connect(colorBtn, SIGNAL(clicked()), this, SLOT(ShowColor()));

	clearBtn = new QToolButton();               	//???????????
	clearBtn->setText(tr("清除"));
	connect(clearBtn, SIGNAL(clicked()), centerWidget, SLOT(clear()));

	paintToolBar->addWidget(widthLabel);
	paintToolBar->addWidget(widthSpinBox);
	paintToolBar->addWidget(colorBtn);
	paintToolBar->addWidget(clearBtn);
}

void VideoCut::ShowOriginalVideoFile()
{
	fileFull = QFileDialog::getOpenFileName(this, "导入原视频", "/", "Video files(*.avi;*.mp4;*.mkv;*.rmvb;*.mov;*.wmv)");
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
	QColor color = QColorDialog::getColor(static_cast<int>(Qt::blue), this);	//?????????????QColorDialog???????????
	if (color.isValid())
	{
		centerWidget->setColor(color);
		QPixmap p(20, 20);
		p.fill(color);
		colorBtn->setIcon(QIcon(p));
	}
}

void save_label_image(cv::Mat label,std::string path)
{
	cv::Mat new_label= label.clone();
	for(int i = 0; i < new_label.rows; i++)
	{
		for(int j = 0; j < new_label.cols; j++)
		{
    		if(label.at<uchar>(i,j)>0)
			{
				new_label.at<uchar>(i,j)=255;
				// new_label.at<uchar>(i,j)[1]=2;
				// new_label.at<uchar>(i,j)[2]=2;
			}
		}
	}
	cv::imwrite(path,new_label);
	return;
}

void save_label_image_1(cv::Mat label,std::string path)
{
	cv::Mat new_label= label.clone();
	for(int i = 0; i < new_label.rows; i++)
	{
		for(int j = 0; j < new_label.cols; j++)
		{
    		if(label.at<uchar>(i,j)==0)
			{
				new_label.at<uchar>(i,j)=0;
				// new_label.at<uchar>(i,j)[1]=2;
				// new_label.at<uchar>(i,j)[2]=2;
			}
		}
	}
	cv::imwrite(path,new_label);
	return;
}
void erosion(cv::Mat &label_o)
{
	cv::Mat label=label_o.clone();
	int erosion_elem=2;
	int erosion_type;
	if( erosion_elem == 0 ){ erosion_type = cv::MORPH_RECT; }
	else if( erosion_elem == 1 ){ erosion_type = cv::MORPH_CROSS; }
	else if( erosion_elem == 2) { erosion_type = cv::MORPH_ELLIPSE; }
	int erosion_size=5;
	cv::Mat element = cv::getStructuringElement( erosion_type, cv::Size( 2*erosion_size + 1, 2*erosion_size+1 ), cv::Point( erosion_size, erosion_size ) );
	/// Apply the erosion operation
	cv::Mat temp=label.clone();
	// dilate( label, label_o, element );
	erode( label, temp, element );

	return;
}

void dilation(cv::Mat &label_o)
{
	cv::Mat label=label_o.clone();
	int elem=2;
	int type;
	if( elem == 0 ){ type = cv::MORPH_RECT; }
	else if( elem == 1 ){ type = cv::MORPH_CROSS; }
	else if( elem == 2) { type = cv::MORPH_ELLIPSE; }
	int size=10;
	cv::Mat element = cv::getStructuringElement( type, cv::Size( 2*size + 1, 2*size+1 ), cv::Point( size, size ) );
	/// Apply the erosion operation
	cv::Mat temp=label.clone();
	// dilate( label, label_o, element );
	dilate( label, temp, element );
	return;
}

void mat_linear_interploate(cv::Mat m1,cv::Mat m2,cv::Mat m3)
{
	return;
}

void bidirection_tracking_3_frame(int k,std::vector<cv::Mat> images,std::vector<cv::Mat> &labels)
{
	cv::Mat temp_label=labels[k+1].clone();
	int row = temp_label.rows;
	int col = temp_label.cols;
	for(int i=0;i<row;i++)
	{
		for(int j=0;j<col;j++)
		{
				labels[k+1].at<uchar>(i,j)=(uchar) ((labels[k].at<uchar>(i,j)+ labels[k+2].at<uchar>(i,j)) / 2.0);
		}
	}

	auto anchor = cv::Point( -1, -1 );
	int delta = 0;
	int ddepth = -1;
	int ind = 1;

	cv::Mat temp_mat1,temp_mat2;
	/// Update kernel size for a normalized box filter
	int kernel_size = 3 + 2*( ind%5 );
	cv::Mat kernel = cv::Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);
	/// Apply filter
	// cv::filter2D(labels[k+1], temp_mat1, ddepth , kernel, anchor, delta, cv::BORDER_DEFAULT );
	// labels[k+1]=temp_mat1.clone();
	// dilation(labels[k+1]);

	// cv::filter2D(labels[k+1], temp_mat1, ddepth , kernel, anchor, delta, cv::BORDER_DEFAULT );
	// cv::filter2D(labels[k+2], temp_mat2, ddepth , kernel, anchor, delta, cv::BORDER_DEFAULT );




	return;
}

void optimize_3_frame(int k,std::vector<cv::Mat> images,std::vector<cv::Mat> &labels)
{
	labels[k+1]=labels[k];
	cv::Mat temp_label=labels[k+1].clone();
	int row = temp_label.rows;
	int col = temp_label.cols;
	int max_value=0;
	//�����͵ķ���������һ�ŵ�foreground��������
	for(int i=0 ; i<row ; i++){
		for(int j=0 ; j<col ; j++){
			max_value=0;
			for(int x=0 ; x<10&&x+i<row ; x++){
				for(int y=0 ; y<10&&y+j<col ; y++){
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
	cv::grabCut(image_copy,labels[k+1],rect,bgdModel,fgdModel,2,cv::GC_INIT_WITH_MASK);
	for(int i=0;i<row;i++){
		for(int j=0;j<col ;j++){
			labels[k+1].at<uchar>(i,j)&=0x1;
		}
	}
	cv::Mat l_d=labels[k].clone();
	dilation(l_d);
	for(int i=0;i<row;i++)
	{
		for(int j=0;j<col ;j++)
		{
			labels[k+1].at<uchar>(i,j)|=l_d.at<uchar>(i,j);
		}
	}
	cv::Mat l=labels[k].clone();
	erosion(l);
	for(int i=0;i<row;i++)
	{
		for(int j=0;j<col ;j++)
		{
			labels[k+1].at<uchar>(i,j)&=l.at<uchar>(i,j);
		}
	}
	return;
}

void bidirection_tracking(int start_index,int end_index,std::vector<cv::Mat> &images,std::vector<cv::Mat> &labels)
{
	std::cout << "start bidirection tracking" << std::endl;
	#define MAX_ITERATION_NUM 10
	int interval=end_index-start_index;
	int row = labels[start_index].rows;
	int col = labels[start_index].cols;
	for(int i=start_index;i<=end_index;i++)
	{
		for(int j=0 ; j<row ; j++)
		{
			for(int k=0 ; k<col ; k++)
			{
				if( labels[i].at<uchar>(j,k)==1)
					labels[i].at<uchar>(j,k)=255;
			}
		}
	}

	labels[end_index-1]=labels[end_index].clone();

	std::cout << "initialize tracking" << std::endl;
	for(int iter=0;iter<MAX_ITERATION_NUM;iter++)
	{
		for(int j=0;j<interval-1;j++)
		{
			// std::cout << "do bidirection trackingframe _3_frame" << std::endl;
			bidirection_tracking_3_frame(j+start_index,images,labels);
			bidirection_tracking_3_frame(interval-2-j+start_index,images,labels);
			// std::cout << "do bidirection reverse trackingframe _3_frame" << std::endl;
		}
	}
	save_label_image_1(labels[end_index-1],"data/testlabel11.png");
	save_label_image_1(labels[end_index-2],"data/testlabel10.png");
	for(int i=start_index;i<=end_index;i++)
	{
		for(int j=0 ; j<row ; j++)
		{
			for(int k=0 ; k<col ; k++)
			{
				if(labels[i].at<uchar>(j,k)>20)
					labels[i].at<uchar>(j,k)=1;
			}
		}
	}
	save_label_image(labels[end_index-1],"data/testlabel11_after.png");
	save_label_image(labels[end_index-2],"data/testlabel10_after.png");
}

std::vector<cv::Mat> read_video2Mat(std::string video_path)
{
	std::vector<cv::Mat> images_temp;
	std::vector<cv::Mat> images;
	std::string cmd="ffmpeg -i "+video_path+" -q:v 2  data/source/Video/%d.png";
	system(cmd.c_str());

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
		cv::Mat img = cv::imread(fname,CV_LOAD_IMAGE_COLOR);
		if (img.empty()) {
			continue;
		}
		index.push_back(i);
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
		cv::Mat img = cv::imread(fname,CV_LOAD_IMAGE_GRAYSCALE);

		if (img.empty()) {
			continue;
		}
		index.push_back( (int)std::atof(fields[0].c_str() ) );
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


	auto anchor = cv::Point( -1, -1 );
	int delta = 0;
	int ddepth = -1;
	int ind = 10;

	cv::Mat temp_mat,trans_label;
	/// Update kernel size for a normalized box filter
	int kernel_size = 3 + 2*( ind%5 );
	cv::Mat kernel = cv::Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);

	for (int s=0;s<labels.size();s++)
	{
		trans_label=labels[s].clone();
		for(int i = 0; i < images[s].rows; i++)
		{
    		for(int j = 0; j < images[s].cols; j++)
			{
				if(trans_label.at<uchar>(i,j)==1)
				{
					trans_label.at<uchar>(i,j)=255;
				}
			}
		}

		dilation(trans_label);
		cv::filter2D(trans_label, temp_mat, ddepth , kernel, anchor, delta, cv::BORDER_DEFAULT );
		trans_label=temp_mat.clone();
		// Read the images


		for(int i = 0; i < images[s].rows; i++)
		{
			/// Apply filter

    		for(int j = 0; j < images[s].cols; j++)
			{
        		if(trans_label.at<uchar>(i,j)==0)
				{
					images[s].at<cv::Vec3b>(i,j)[0]=0;
					images[s].at<cv::Vec3b>(i,j)[1]=0;
					images[s].at<cv::Vec3b>(i,j)[2]=0;
				}
				else
				{
					int cons=16;
					// double alpha= (pow( cons,trans_label.at<uchar>(i,j)/255.0)-1)/(cons-1);
					double alpha= (trans_label.at<uchar>(i,j)/255.0);
					images[s].at<cv::Vec3b>(i,j)[0]*= (int) alpha;
					images[s].at<cv::Vec3b>(i,j)[1]*= (int) alpha;
					images[s].at<cv::Vec3b>(i,j)[2]*= (int) alpha;
				}
				/// Apply filter
			}
		}
		cv::imwrite( "data/source/backVideo/"+std::to_string(s)+".png", images[s]);
	}
	std::cout << "image saving okay saving!"  << '\n';
	std::string cmd= "ffmpeg -start_number 0 -i data/source/backVideo/%d.png -vcodec mpeg4 test.avi";

	system(cmd.c_str());

}

void initial(cv::Mat &label_o,cv::Mat &new_label)
{
	cv::Mat label=label_o.clone();
	int erosion_elem=2;
	int erosion_type;
	if( erosion_elem == 0 ){ erosion_type = cv::MORPH_RECT; }
	else if( erosion_elem == 1 ){ erosion_type = cv::MORPH_CROSS; }
	else if( erosion_elem == 2) { erosion_type = cv::MORPH_ELLIPSE; }

	int erosion_size=5;
	cv::Mat element = cv::getStructuringElement( erosion_type, cv::Size( 2*erosion_size + 1, 2*erosion_size+1 ), cv::Point( erosion_size, erosion_size ) );
	/// Apply the erosion operation
	cv::Mat temp=label.clone();
	dilate( label, temp, element );
	// erode( label, temp, element );

	for(int i = 0; i < new_label.rows; i++)
	{
		for(int j = 0; j < new_label.cols; j++)
		{
    		if(temp.at<uchar>(i,j)==2)
			{
				new_label.at<uchar>(i,j)=2;
				// new_label.at<uchar>(i,j)[1]=2;
				// new_label.at<uchar>(i,j)[2]=2;
			}
		}
	}
	return;

}

void VideoCut::doVideoCut() {
	// std::string video_path="./data/news.avi";
	std::string video_path=std::string(this->fileFull.toLatin1().data());
	// std::string keyframes_path="./data/source/keyframe/news.avi";
	std::string keyframes_path=std::string(this->centerWidget->getSavePath().toLatin1().data());

	doVideoCut_real(video_path,keyframes_path);

}

void VideoCut::doVideoCut_real(std::string video_path,std::string keyframes_path) {

	// read cut of keyframes
	std::vector<cv::Mat> labels,temp_labels;
	std::cout<< " key frames path is "<< keyframes_path <<std::endl;
	std::vector<int> keyframe_indexs=read_keyframes(keyframes_path,temp_labels);
	std::sort(keyframe_indexs.begin(),keyframe_indexs.end());
	std::cout<< " preprocessing okay,labelled frames "<< temp_labels.size() <<std::endl;

	std::cout<< " video path is "<< video_path <<std::endl;
	std::vector<cv::Mat> images=read_video2Mat(video_path);
	std::cout<< " preprocessing okay,total frames "<< images.size() <<std::endl;


	// assign labels fo keyframes
	for(int i=0;i<images.size();i++)
	{
		labels.push_back( cv::Mat::zeros(images[0].rows,images[0].cols,CV_8UC1));
	}
	//����temp_labels���иı�
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
		int s=keyframe_indexs[i];
		// save_label_image(labels[keyframe_indexs[i]],"data/source/label_before_er.png");

		// initial(labels[keyframe_indexs[i]],labels[keyframe_indexs[i]]);
		// save_label_image(labels[keyframe_indexs[i]],"data/source/label.png");
		// imwrite( "data/source/testafter_"+std::to_string(s)+".png", image_copy);
		cv::Mat bgdModel,fgdModel;
		cv::Mat l_old = labels[keyframe_indexs[i]].clone();
		cv::grabCut(image_copy,labels[keyframe_indexs[i]],rect,bgdModel,fgdModel,20,cv::GC_INIT_WITH_MASK);
		cv::Mat image_copy_new=image_copy.clone();
		std::cout << "done" << '\n';
		for(int j=0;j<image_copy.rows;j++){
			for(int k=0;k<image_copy.cols ;k++){
				labels[keyframe_indexs[i]].at<uchar>(j,k)&=0x1;
			}
		}
		// imwrite( "data/source/testaftergraphcut_"+std::to_string(s)+".png", labels[keyframe_indexs[i]]);

		for(int j=0;j<l_old.rows;j++){
			for(int k=0;k<l_old.cols ;k++){
				labels[keyframe_indexs[i]].at<uchar>(j,k)&= (l_old.at<uchar>(j,k)-2);
			}
		}

		// save_label_image_1(labels[keyframe_indexs[i]],"data/source/label_aftercut.png");
	}

	std::cout<< " label initial okay " <<std::endl;
	for(int i=0;i<keyframe_indexs.size()-1;i++)
	{
		int start_index=keyframe_indexs[i],end_index=keyframe_indexs[i+1];
		// if(start_index>)
		// 	break;
		int interval=end_index-start_index;
		for(int j=0;j<interval-1;j++)
		{
			std::cout << "do optimize_3_frame" << std::endl;
			optimize_3_frame(j+start_index,images,labels);
		}
		bidirection_tracking(start_index,end_index,images,labels);
	}

	save_label_image(labels[11],"data/testlabel11_after_out.png");
	save_label_image(labels[10],"data/testlabel10_after_out.png");
	std::cout << labels[10] << '\n';
	//keyframe������һ�벻����Ƶ������һ֡�������������δ���
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
