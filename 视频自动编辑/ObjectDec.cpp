#include "ObjectDec.h"

std::vector<cv::Mat> ObjectDec(std::vector<cv::Mat> &srcImages,int type) {
	//»ÀŒÔºÏ≤‚

	std::vector<cv::Mat> outImages;

	switch (type) {
	case 0:
		outImages = HumanDec(srcImages);
		break;
	case 1:
		outImages = UnkonwDec(srcImages);
	default:
		break;
	}
	return outImages;
}

std::vector<cv::Mat> HumanDec(std::vector<cv::Mat> &srcImages) {

	std::vector<cv::Mat> outImages;

	cv::HOGDescriptor peopleHOG;

	//cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();

	peopleHOG.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());

	std::vector<cv::Rect>found, found_filtered;

	for (size_t k = 0; k < srcImages.size(); k++) {
		found.clear();
		found_filtered.clear();
		cv::Mat srcImage = srcImages.at(k);
		peopleHOG.detectMultiScale(srcImage, found);
		for (size_t i = 0; i < found.size(); i++) {
			size_t j;
			cv::Rect r = found[i];
			for (j = 0; j < found.size(); j++)
				if (j != i && (r&found[j]) == r)
					break;
			if (j == found.size())
				found_filtered.push_back(r);
		}
		for (size_t i = 0; i < found_filtered.size(); i++) {
			cv::Mat outImage;
			cv::Rect r = found_filtered[i];
			outImage = srcImage(r);
			outImages.push_back(outImage);
		}
		if (found_filtered.size() == 0)
			outImages.push_back(srcImage);
	}
	if (DE_BUG) {
		for (size_t i = 0; i < outImages.size(); i++) {
			cv::imshow("out", outImages[i]);
			cv::waitKey(0);
		}
	}
	return outImages;
}

std::vector<cv::Mat> UnkonwDec(std::vector<cv::Mat> &srcImages) {
	return std::vector<cv::Mat>();
}
