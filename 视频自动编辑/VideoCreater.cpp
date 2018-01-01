#include "VideoCreater.h"

void videoCreate(std::vector<cv::Mat> &outFrames) {
	cv::VideoWriter videoWriter;
	videoWriter.open("outVideo.avi", CV_FOURCC('D', 'I', 'V', 'X'), 16, cv::Size(outFrames.at(0).cols, outFrames.at(0).rows),true);
	if (!videoWriter.isOpened())
		exit(0);
	for (size_t i = 0; i < outFrames.size(); i++) 
		videoWriter << outFrames[i];
}
