// FFMpeg + OpenCV demo
#include "VideoDecoder.h"
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QDebug>
#ifdef __cplusplus
extern "C" {
#endif

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/opt.h>  
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>

#ifdef __cplusplus
}
#endif

#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avformat.lib  ")
#pragma comment(lib, "avutil.lib    ")
#pragma comment(lib, "avdevice.lib  ")
#pragma comment(lib, "avfilter.lib  ")
#pragma comment(lib, "postproc.lib  ")
#pragma comment(lib, "swresample.lib")
#pragma comment(lib, "swscale.lib   ")

#include <iostream>

static void CopyDate(AVFrame *pFrame, int width, int height, int time);
static void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame);

using namespace cv;
using namespace std;

int DecodeVideo(char* filename, CenterWidget* widget)
{
	int result = 0;
	av_register_all(); // ע������FFmpeg����֧�ֵ��ļ���ʽ��codec

	AVFormatContext* pFormatCtx;
	pFormatCtx = avformat_alloc_context();

	// step1: ����ý���ļ�,����2������������ָ���ļ���ʽ��buffer��С�͸�ʽ���������ó�NULL�Ļ���libavformat�����Զ�ȥ̽������
	result = avformat_open_input(&pFormatCtx, filename, NULL, NULL);
	qDebug() << filename;
	if (result != 0)
	{
		cout << "open file fail" << endl;
		return -1;
	}
	cout << "open file succ" << endl;

	// step2: ������Ϣ������Ϣ
	result = avformat_find_stream_info(pFormatCtx, NULL);
	if (result != 0)
	{
		cout << "find stream fail" << endl;
		return -1;
	}
	cout << "find stream succ" << endl;

	// step3: ��ӡ��Ϣ
	//av_dump_format(pFormatCtx, 0, filename, 0);

	// step4���ҵ�video������
	int i = 0;
	int videoStream = -1;
	AVCodecContext* pCodecCtx = NULL;
	for (i = 0; i < pFormatCtx->nb_streams; i++)
	{
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoStream = i;
			break;
		}
	}

	if (videoStream == -1)
	{
		cout << "find stream video fail" << endl;
		return -1;
	}
	cout << "find stream video succ." << endl;

	// �õ�video������ʽ
	pCodecCtx = pFormatCtx->streams[videoStream]->codec;

	// step5: �õ�������
	AVCodec* pCodec = NULL;
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if (pCodec == NULL)
	{
		cout << "find decoder fail" << endl;
		return -1;
	}
	cout << "find decoder succ" << endl;

	result = avcodec_open2(pCodecCtx, pCodec, NULL);
	if (result != 0)
	{
		cout << "open codec fail" << endl;
		return -1;
	}
	cout << "open codec succ" << endl;

	// step6: ����ԭʼ����֡ �� RGB֡�ڴ�
	AVFrame* pFrame = NULL;
	AVFrame* pFrameRGB = NULL;
	pFrame = av_frame_alloc();
	pFrameRGB = av_frame_alloc();
	if (pFrame == NULL || pFrameRGB == NULL)
	{
		return -1;
	}

	int numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);
	uint8_t* buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));
	avpicture_fill((AVPicture*)pFrameRGB, buffer, AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);

	int frameFinishsed = 0;
	AVPacket packet;
	i = 0;

	// step7: ������ʽת���ı�
	SwsContext * pSwxCtx = sws_getContext(
		pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
		pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB24,
		SWS_BILINEAR, 0, 0, 0);

	cv::Mat image(pCodecCtx->height, pCodecCtx->width, CV_8UC3);
	int b = 0;
	int g = 1;
	int r = 2;
	int frameCount = 0;
	int listCount = 0;
	char name[64];
	while (true)
	{
		result = av_read_frame(pFormatCtx, &packet);
		if (result != 0)
		{
			break;
		}
		if (packet.stream_index == videoStream)
		{

			// ����
			avcodec_decode_video2(pCodecCtx, pFrame, &frameFinishsed, &packet);

			if (frameFinishsed && pFrame->key_frame == 1)
			{
				// ת��
				sws_scale(pSwxCtx, pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
					pFrameRGB->data, pFrameRGB->linesize);

				for (int m = 0; m < pCodecCtx->height; m++)
				{
					for (int n = 0; n < pCodecCtx->width; n++)
					{
						image.at<Vec3b>(m, n)[r] = pFrameRGB->data[0][3 * (m*pCodecCtx->width + n) + 0];
						image.at<Vec3b>(m, n)[g] = pFrameRGB->data[0][3 * (m*pCodecCtx->width + n) + 1];
						image.at<Vec3b>(m, n)[b] = pFrameRGB->data[0][3 * (m*pCodecCtx->width + n) + 2];
					}
				}
				cv::cvtColor(image, image, CV_BGR2RGB);
				QImage img = QImage((const unsigned char*)(image.data), image.cols, image.rows, QImage::Format_RGB888);
				QPalette pal;
				PaintWidget *tmp = new PaintWidget();
				tmp->setAutoFillBackground(true);
				tmp->setMinimumSize(image.cols, image.rows);
				pal.setBrush(tmp->backgroundRole(), QBrush(QPixmap::fromImage(img).scaled(tmp->size(), Qt::IgnoreAspectRatio,
					Qt::SmoothTransformation)));
				tmp->setPalette(pal);
				widget->stack->addWidget(tmp);
				sprintf(name, "%d.png", frameCount);
				widget->list->insertItem(listCount, QString(QLatin1String(name)));
				listCount++;
			}
			frameCount++;
		}

		av_free_packet(&packet);
	}

	avformat_close_input(&pFormatCtx);

	return 0;
}
