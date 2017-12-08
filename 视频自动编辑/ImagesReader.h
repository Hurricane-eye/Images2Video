#pragma once

//#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>
#include <opencv2\core.hpp>
#include <opencv2\imgcodecs.hpp>
#include <opencv2\highgui.hpp>
#include "Debug.h"

std::vector<cv::Mat> readAllImage(std::vector<std::string>&imageURL);
