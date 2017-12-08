#include "ImagesReader.h"


std::vector<cv::Mat> readAllImage(std::vector<std::string>& imageURL) {
	std::vector<cv::Mat> srcImages;
	for (int i = 0; i < imageURL.size(); i++) {
		auto URL = imageURL.at(i);
		cv::Mat image = cv::imread(URL);
		if (!image.data) {
			if(DE_BUG) 
				std::cerr << "Can't open " << i + 1 << " image!" << std::endl;
			exit(0);
		}
		if (DE_BUG) {
			cv::imshow("image", image);
			cv::waitKey(0);
		}
		srcImages.push_back(image);
	}
	return srcImages;
}
