#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <direct.h>

using namespace std;
using namespace cv;

void splitWithStrtok(const char* str, const char* delim, vector<string>& ret)
{
	char* strcopy = new char[strlen(str) + 1];
	strcpy(strcopy, str);
	char* word = strtok(strcopy, delim);
	ret.push_back(word);
	while (word = strtok(nullptr, delim))
		ret.push_back(word);
	delete[] strcopy;
}

int main()
{
	
	printf("usage:   视频地址 抽帧间隔\n");
	printf("example: F:/FrameExtract/test.mp4 5\n");
	
	char videopath[100];
	cin >> videopath;
	int interval;
	cin >> interval;

	string folderPath;
	vector<string> folderPaths;
	string fileName;
	vector<string> fileNames;
	splitWithStrtok(videopath, "/", folderPaths);
	for (int i = 0; i < folderPaths.size() - 1; i++) {
		folderPath += folderPaths[i];
		folderPath += "/";
	}
	splitWithStrtok(folderPaths.back().c_str(), ".", fileNames);
	fileName = fileNames[0];
	folderPath += fileName;
	folderPath += "/";

	cout << folderPath;
	//在视频同级目录下创建文件夹，文件夹名与视频名一致
	
	/*string command;
	command = "md " + folderPath;
	system(command.c_str());*/
	_mkdir(folderPath.c_str());
	

	VideoCapture capture(videopath);
	//检测是否正常打开:成功打开时，isOpened返回ture
	if (!capture.isOpened())
		cout << "fail to open!" << endl;

	//获取整个帧数
	long totalFrameNumber = capture.get(CAP_PROP_FRAME_COUNT);
	cout << "整个视频共" << totalFrameNumber << "帧" << endl;


	//设置开始帧()
	long frameToStart = 0;
	capture.set(CAP_PROP_POS_FRAMES, frameToStart);
	cout << "从第" << frameToStart << "帧开始读" << endl;

	//设置结束帧
	int frameToStop = totalFrameNumber - 1;

	if (frameToStop < frameToStart)
	{
		cout << "结束帧小于开始帧，程序错误，即将退出！" << endl;
		return -1;
	}
	else
	{
		cout << "结束帧为：第" << frameToStop << "帧" << endl;
	}

	//获取帧率
	double rate = capture.get(CAP_PROP_FPS);
	cout << "帧率为:" << rate << endl;


	//定义一个用来控制读取视频循环结束的变量
	bool stop = false;

	//承载每一帧的图像
	Mat frame;

	//显示每一帧的窗口
	//namedWindow( "Extractedframe" );

	//两帧间的间隔时间:
	//int delay = 1000/rate;
	double delay = 1000 / rate;


	//利用while循环读取帧
	//currentFrame是在循环体中控制读取到指定的帧后循环结束的变量
	long currentFrame = frameToStart;


	while (!stop)
	{
		//读取下一帧
		if (!capture.read(frame))
		{
			cout << "视频解帧结束" << endl;
			return -1;
		}

		//cout << "正在读取第" << currentFrame << "帧" << endl;
		/*namedWindow("Extractedframe", 0);
		imshow("Extractedframe", frame);*/

		//此处为跳帧操作
		if (currentFrame % interval == 0) //帧数间隔
		{
			cout << "正在写第" << currentFrame << "帧" << endl;
			stringstream str;
			str << folderPath << fileName << "_" << currentFrame << ".jpg";        /*图片存储位置*/

			cout << str.str() << endl;
			imwrite(str.str(), frame);
		}

		//waitKey(intdelay=0)当delay≤ 0时会永远等待；当delay>0时会等待delay毫秒
		//当时间结束前没有按键按下时，返回值为-1；否则返回按键
		int c = waitKey(delay);
		//按下ESC或者到达指定的结束帧后退出读取视频
		if ((char)c == 27 || currentFrame > frameToStop)
		{
			stop = true;
		}
		//按下按键后会停留在当前帧，等待下一次按键
		if (c >= 0)
		{
			waitKey(0);
		}
		currentFrame++;

	}

	//关闭视频文件
	capture.release();
	waitKey(0);
	return 0;
}