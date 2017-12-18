#pragma once

#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>
#include "Debug.h"

std::vector<cv::Mat> ObjectDec(std::vector<cv::Mat> &srcImage,int typeOfDec);

std::vector<cv::Mat> HumanDec(std::vector<cv::Mat> &srcImage);

std::vector<cv::Mat> UnkonwDec(std::vector<cv::Mat> &srcImage);
