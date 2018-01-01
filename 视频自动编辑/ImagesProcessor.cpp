#include "ImagesProcessor.h"

#define FN 300.0
#define pointPframe 5

std::vector<cv::Mat> result;

cv::Mat image_blur(cv::Mat img, int k) {
	cv::Mat result_img;
	int r;
	r = 32 - k;
	cv::blur(img, result_img, cv::Size(r, r), cv::Point(-1, -1));
	return result_img;
}

cv::Mat image_gray(cv::Mat img, int k) {
	cv::Mat result_img(img.rows, img.cols, CV_8UC3, cv::Scalar(0, 0, 0));
	unsigned char *imgarray = new unsigned char[img.rows*img.rows * 3];
	imgarray = img.data;
	uchar *p = result_img.data;
	for (int i = 0; i < img.rows*img.cols * 3; i += 3) {
		double tem;
		tem = (imgarray[i] + imgarray[i + 1] + imgarray[i + 2]) / 3.0;
		*p = imgarray[i] + (tem - imgarray[i]) / 32.0 * (31.0 - k);
		p++;
		*p = imgarray[i + 1] + (tem - imgarray[i + 1]) / 32.0 * (31.0 - k);
		p++;
		*p = imgarray[i + 2] + (tem - imgarray[i + 2]) / 32.0 * (31.0 - k);
		p++;
	}
	return result_img;
}

cv::Mat image_blue(cv::Mat img, double k) {
	cv::Mat result_img(img.rows, img.cols, CV_8UC3, cv::Scalar(0, 0, 0));
	unsigned char *imgarray = new unsigned char[img.rows*img.rows * 3];
	imgarray = img.data;
	uchar *p = result_img.data;
	for (int i = 0; i < img.rows*img.cols * 3; i += 3) {
		if (k < 16) {
			*p = imgarray[i] / (32.0 / (32.0 - k));
			p++;
			*p = imgarray[i + 1] / (32.0 / (1.0 + 0.5 * k));
			p++;
			*p = imgarray[i + 2] / (32.0 / (16.0 - 0.5 * k));
			p++;
		}
		else {
			*p = imgarray[i] / (32.0 / (k));
			p++;
			*p = imgarray[i + 1] / (32.0 / (1.0 + 0.5 * k));
			p++;
			*p = imgarray[i + 2] / (32.0 / (16.0 - 0.5 * k));
			p++;
		}
	}
	return result_img;
}

cv::Mat image_fade(cv::Mat img_fadeout, cv::Mat img_fadein, double k) {
	cv::Mat result_img;
	cv::addWeighted(img_fadeout, 1 - k, img_fadein, k, 0.0, result_img);
	return result_img;
}


void move_zoom(cv::Mat image) {
	if (image.rows > 300 && image.cols > 300)				//enlarge
	{

	}
	int linex = 0;
	int liney = 400 - image.rows / 2;
	int x = image.cols;
	int y = image.rows;
	int link = 0;
	int framenumber = FN;
	int times1 = image.cols / pointPframe;
	int times2 = (600 + image.cols / 2) / pointPframe;
	std::cout << "t1==" << times1 << std::endl << "t2==" << times2 << std::endl;
	int j1 = 0, k1, flag = 0;
	cv::Mat img;
	for (int i = 0; i < framenumber; i++) {
		cv::Mat dst;
		dst.create(800, 1200, image.type());
		int rx, ry;
		j1 = 0;
		if (i <= times1) {
			for (int j = liney; j < liney + image.rows; j++) {

				for (int k = 0; k < i * pointPframe; k++) {
					k1 = k;
					dst.at

						<cv::Vec3b>(j, k) = image.at

						<cv::Vec3b>(j1, k1);
				}
				j1++;
			}
		}
		else if (i >= times2 && flag == 0) {

			if (y*1.1 >= 800 || x*1.1 >= 1200) {
				flag = 1;
				continue;
			}
			x = x*1.1;
			y = y*1.1;
			cv::resize(image, img, cv::Size(x, y), 0, 0, cv::INTER_LINEAR);
			cv::Mat roi2 = img;
			//cv::imshow("resize", roi2);
			//cv::waitKey(1);

			cv::Rect roi_rect = cv::Rect(dst.cols / 2 - roi2.cols / 2, dst.rows / 2 - roi2.rows / 2, roi2.cols, roi2.rows);
			roi2.copyTo(dst(roi_rect));
			/*cv::Rect roi = cv::Rect(dst.rows / 2 - img.rows / 2, dst.cols / 2 - img.cols / 2, img.rows, img.cols);
			img.copyTo(dst(roi));*/
		}
		else if (i >= times2 && flag == 1) {
			cv::Mat roi2 = img;

			for (int i1 = 0; i1 < img.rows; i1++) {
				for (int j1 = 0; j1 < img.cols; j1++) {
					for (int k1 = 0; k1 < 3; k1++) {
						img.at

							<cv::Vec3b>(i1, j1)[k1] *= 0.95;
					}
					if (img.at

						<cv::Vec3b>(i1, j1)[0] <= 0.1 && img.at

						<cv::Vec3b>(i1, j1)[1] <= 0.1 && img.at

						<cv::Vec3b>(i1, j1)[2] <= 0.1) {
						link = 1;
						break;
					}
				}
			}
			if (link == 1) {
				break;
			}
			cv::Rect roi_rect = cv::Rect(dst.cols / 2 - roi2.cols / 2, dst.rows / 2 - roi2.rows / 2, roi2.cols, roi2.rows);
			img.copyTo(dst(roi_rect));
		}
		else {
			int i2 = liney;
			for (int i1 = 0; i1 < image.rows; i1++) {
				int j2 = (i - times1)*pointPframe;
				for (int j1 = 0; j1 < image.cols; j1++) {
					dst.at

						<cv::Vec3b>(i2, j2) = image.at

						<cv::Vec3b>(i1, j1);
					j2++;
				}
				i2++;
			}

		}
		//cv::imshow("test", dst);
		//cv::waitKey(1);
		dst = image_blur(dst, i / 10);
		result.push_back(dst);
	}
}

void move_rotate(cv::Mat image) {

}

void rotate_enlarge(cv::Mat image) {

}

std::vector<cv::Mat> imagesProcess(std::vector<cv::Mat>& srcImages) {

	for (int i = 0; i < srcImages.size(); i++) {
		move_zoom(srcImages[i]);
	}
	return result;
}