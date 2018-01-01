#pragma once

#include <opencv2\opencv.hpp>
#include "Debug.h"

std::vector<cv::Mat> imagesProcess(std::vector<cv::Mat> &srcImages);
void move_zoom(cv::Mat image);
cv::Mat image_fade(cv::Mat img_fadeout, cv::Mat img_fadein, double k);
cv::Mat image_blue(cv::Mat img, double k);
cv::Mat image_gray(cv::Mat img, int k);
cv::Mat image_blur(cv::Mat img, int k);