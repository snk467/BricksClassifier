#include "Converter.h"

Converter::Converter(cv::Mat image)
{
	mImage = image.clone();
}

cv::Mat_<uchar> Converter::toGray()
{
	cv::Mat_<cv::Vec3b> image = mImage.clone();
	cv::Mat_<uchar> grayImage = cv::Mat_<uchar>(mImage.rows, mImage.cols);

	for (int x = 0; x < image.cols; x++)
	{
		for (int y = 0; y < image.rows; y++)
		{
			cv::Vec3b pixel = image(y, x);

			int B = pixel[0];
			int G = pixel[1];
			int R = pixel[2];
			int value = (R + G + B) / 3;

			grayImage(y, x) = value;
		}
	}

	return grayImage;
}
