#include "VideoCreater.h"

void videoCreate(std::vector<cv::Mat> &outFrames) {
	cv::VideoWriter videoWriter;
	videoWriter.open("outVideo.avi", -1, 16, cv::Size(outFrames.at(0).cols, outFrames.at(0).rows));
	if (!videoWriter.isOpened())
		exit(0);
	for (int i = 0; i < outFrames.size(); i++) 
		videoWriter << outFrames.at(0);
}
