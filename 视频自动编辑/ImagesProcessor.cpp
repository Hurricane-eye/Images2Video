#include "ImagesProcessor.h"

#define FN 500.0
#define pointPframe 5
#define framex 800
#define framey 1200
#define anglePframe 2.5

std::vector<cv::Mat> result;

cv::Mat image_blur(cv::Mat img, int k)
{
	if (k > 31)k = 31;
	cv::Mat result_img;
	int r;
	r = 32 - k;
	cv::blur(img, result_img, cv::Size(r, r), cv::Point(-1, -1));
	return result_img;
}

cv::Mat image_gray(cv::Mat img, int k)
{
	if (k > 31)k = 31;
	cv::Mat result_img(img.rows, img.cols, CV_8UC3, cv::Scalar(0, 0, 0));
	unsigned char *imgarray = new unsigned char[img.rows*img.rows * 3];
	imgarray = img.data;
	uchar *p = result_img.data;
	for (int i = 0; i < img.rows*img.cols * 3; i += 3)
	{
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

cv::Mat image_blue(cv::Mat img, double k)
{
	if (k > 31.0)k = 31.0;
	cv::Mat result_img(img.rows, img.cols, CV_8UC3, cv::Scalar(0, 0, 0));
	unsigned char *imgarray = new unsigned char[img.rows*img.rows * 3];
	imgarray = img.data;
	uchar *p = result_img.data;
	for (int i = 0; i < img.rows*img.cols * 3; i += 3)
	{
		if (k < 16)
		{
			*p = imgarray[i] / (32.0 / (32.0 - k));
			p++;
			*p = imgarray[i + 1] / (32.0 / (1.0 + 0.5 * k));
			p++;
			*p = imgarray[i + 2] / (32.0 / (16.0 - 0.5 * k));
			p++;
		}
		else
		{
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

cv::Mat image_fade(cv::Mat img_fadeout, cv::Mat img_fadein, double k)
{
	cv::Mat result_img;
	cv::addWeighted(img_fadeout, 1 - k, img_fadein, k, 0.0, result_img);
	return result_img;
}

int imageRotate(cv::InputArray src, cv::OutputArray dst, double angle)
{
	cv::Mat input = src.getMat();
	if (input.empty()) {
		return -1;
	}

	//得到图像大小  
	int width = input.cols;
	int height = input.rows;

	//计算图像中心点  
	cv::Point2f center;
	center.x = width / 2.0;
	center.y = height / 2.0;

	//获得旋转变换矩阵  
	double scale = 1.0;
	cv::Mat trans_mat = cv::getRotationMatrix2D(center, -angle, scale);

	//计算新图像大小  
	double angle1 = angle * CV_PI / 180.;
	double a = sin(angle1) * scale;
	double b = cos(angle1) * scale;
	double out_width = height * fabs(a) + width * fabs(b);
	double out_height = width * fabs(a) + height * fabs(b);

	//在旋转变换矩阵中加入平移量  
	trans_mat.at<double>(0, 2) += cvRound((out_width - width) / 2);
	trans_mat.at<double>(1, 2) += cvRound((out_height - height) / 2);


	//仿射变换  
	warpAffine(input, dst, trans_mat, cv::Size(out_width, out_height));

	return 0;
}

void A(cv::Mat image)
{
	int seed = rand() % 2;
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
	for (int i = 0; i < framenumber; i++)
	{
		cv::Mat dst;
		dst.create(800, 1200, image.type());
		int rx, ry;
		j1 = 0;
		if (i <= times1)
		{
			for (int j = liney; j < liney + image.rows; j++)
			{

				for (int k = 0; k < i * pointPframe; k++)
				{
					k1 = k;
					dst.at

						<cv::Vec3b>(j, k) = image.at

						<cv::Vec3b>(j1, k1);
				}
				j1++;
			}
		}
		else if (i >= times2 && flag == 0)
		{

			if (y*1.1 >= 800 || x*1.1 >= 1200)
			{
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
		else if (i >= times2 && flag == 1)
		{
			cv::Mat roi2 = img;

			for (int i1 = 0; i1 < img.rows; i1++)
			{
				for (int j1 = 0; j1 < img.cols; j1++)
				{
					for (int k1 = 0; k1 < 3; k1++)
					{
						img.at

							<cv::Vec3b>(i1, j1)[k1] *= 0.95;
					}
					if (img.at

						<cv::Vec3b>(i1, j1)[0] <= 0.1 && img.at

						<cv::Vec3b>(i1, j1)[1] <= 0.1 && img.at

						<cv::Vec3b>(i1, j1)[2] <= 0.1)
					{
						link = 1;
						break;
					}
				}
			}
			if (link == 1)
			{
				break;
			}
			cv::Rect roi_rect = cv::Rect(dst.cols / 2 - roi2.cols / 2, dst.rows / 2 - roi2.rows / 2, roi2.cols, roi2.rows);
			img.copyTo(dst(roi_rect));
		}
		else
		{
			int i2 = liney;
			for (int i1 = 0; i1 < image.rows; i1++)
			{
				int j2 = (i - times1)*pointPframe;
				for (int j1 = 0; j1 < image.cols; j1++)
				{
					dst.at

						<cv::Vec3b>(i2, j2) = image.at

						<cv::Vec3b>(i1, j1);
					j2++;
				}
				i2++;
			}

		}
		
		if (seed == 0)
		{
			dst = image_blur(dst, i / (FN / 100));
		}
		else if (seed == 1)
		{
			dst = image_gray(dst, i / (FN / 50));
		}
		cv::imshow("test", dst);
		cv::waitKey(1);
		result.push_back(dst);
	}
}

void B(cv::Mat image)
{
	int seed = rand() % 2;
	int x = image.cols;
	int y = image.rows;
	double angle = 0;
	cv::Mat img;
	for (int i = 0; i < 1440 / anglePframe; i++)
	{
		cv::Mat img1, img2;
		cv::Mat dst(framex, framey, image.type(), cv::Scalar::all(0));
		if (i <= 720 / anglePframe)
		{
			if (y * 1.3 <= framex && x * 1.3 <= framey && y * 1.3 <= framey && x * 1.3 <= framex)
			{
				x = x * 1.01;
				y = y * 1.01;
			}
			if (i*anglePframe <= 360)
			{
				angle = i * 2 * anglePframe;
			}
			else
			{
				angle = i * anglePframe;
			}

			cv::resize(image, img1, cv::Size(x, y), 0, 0, cv::INTER_LINEAR);
			imageRotate(img1, img2, angle);

			int x2 = img2.cols;
			int y2 = img2.rows;

			cv::Mat roi = img2;
			cv::Rect roi_rect = cv::Rect(dst.cols / 2 - roi.cols / 2, dst.rows / 2 - roi.rows / 2, roi.cols, roi.rows);
			roi.copyTo(dst(roi_rect));
			std::cout << dst.at<cv::Vec3b>(0, 0) << std::endl;
			img = dst;
		}
		else
		{
			cv::Mat roi2 = img;
			for (int i1 = 0; i1 < img.rows; i1++)
			{
				for (int j1 = 0; j1 < img.cols; j1++)
				{
					for (int k1 = 0; k1 < 3; k1++)
					{
						img.at<cv::Vec3b>(i1, j1)[k1] *= 0.90;
					}

				}
			}
			if (img.at<cv::Vec3b>(img.rows / 2, img.cols / 2)[0] <= 0.1 && img.at<cv::Vec3b>(img.rows / 2, img.cols / 2)[1] <= 0.1 && img.at<cv::Vec3b>(img.rows / 2, img.cols / 2)[2] <= 0.1)
				break;

			cv::Rect roi_rect = cv::Rect(dst.cols / 2 - roi2.cols / 2, dst.rows / 2 - roi2.rows / 2, roi2.cols, roi2.rows);
			img.copyTo(dst(roi_rect));

		}
		
		if (seed == 0)
		{
			dst = image_blur(dst, i / (FN / 100));
		}
		else if (seed == 1)
		{
			dst = image_gray(dst, i / (FN / 50));
		}
		cv::imshow("test", dst);
		cv::waitKey(1);
		result.push_back(dst);
	}

}

void C(cv::Mat image)
{
	int seed = rand() % 2;
	int x = image.cols;
	int y = image.rows;
	double angle = 0;
	double xy = sqrt(pow(x, 2) + pow(y, 2));
	int time1 = (framey - xy) / 2;
	cv::Mat img = image;
	for (int i = 0; i < FN; i++)
	{
		cv::Mat img1, img2;
		cv::Mat dst(framex, framey, image.type(), cv::Scalar::all(0));
		if (i * pointPframe < time1)
		{
			angle = i * anglePframe;
			imageRotate(image, img2, angle);

			int x2 = img2.cols;
			int y2 = img2.rows;
			int t = 0;
			if (i * pointPframe - (x2 - x) / 2 < 0)
			{
				t = 0 - i * pointPframe - (x2 - x) / 2;
			}
			cv::Mat roi = img2;
			cv::Rect roi_rect = cv::Rect(i * pointPframe + (xy - x2) / 2, dst.rows / 2 - roi.rows / 2, roi.cols, roi.rows);
			roi.copyTo(dst(roi_rect));
			std::cout << dst.at<cv::Vec3b>(0, 0) << std::endl;
		}
		else if (i * pointPframe > time1 && (int)angle % 360 != 0)
		{
			angle = i * anglePframe;
			imageRotate(image, img2, angle);
			cv::Mat roi2 = img2;


			cv::Rect roi_rect = cv::Rect(dst.cols / 2 - roi2.cols / 2, dst.rows / 2 - roi2.rows / 2, roi2.cols, roi2.rows);
			roi2.copyTo(dst(roi_rect));

		}
		else
		{
			cv::Mat roi2 = image;
			for (int i1 = 0; i1 < img.rows; i1++)
			{
				for (int j1 = 0; j1 < img.cols; j1++)
				{
					for (int k1 = 0; k1 < 3; k1++)
					{
						img.at<cv::Vec3b>(i1, j1)[k1] *= 0.90;
					}

				}
			}
			if (img.at<cv::Vec3b>(img.rows / 2, img.cols / 2)[0] <= 0.1 && img.at<cv::Vec3b>(img.rows / 2, img.cols / 2)[1] <= 0.1 && img.at<cv::Vec3b>(img.rows / 2, img.cols / 2)[2] <= 0.1)
				break;

			cv::Rect roi_rect = cv::Rect(dst.cols / 2 - roi2.cols / 2, dst.rows / 2 - roi2.rows / 2, roi2.cols, roi2.rows);
			img.copyTo(dst(roi_rect));
		}
		
		if (seed == 0)
		{
			dst = image_blur(dst, i / (FN / 100));
		}
		else if(seed == 1)
		{
			dst = image_gray(dst, i / (FN / 50));
		}
        cv::imshow("test", dst);
		cv::waitKey(1);
		result.push_back(dst);
	}
}

std::vector<cv::Mat> imagesProcess(std::vector<cv::Mat>& srcImages) {

	for (int i = 0; i < srcImages.size(); i++)
	{

		int k = rand() % 3;
		if (k == 0)
		{
			A(srcImages[i]);
		}
		else if (k == 1)
		{
			B(srcImages[i]);
		}
		else if (k == 2)
		{
			C(srcImages[i]);
		}
	}
	return result;
}
