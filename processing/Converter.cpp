#include "Converter.h"

void Converter::BGRToGray(cv::Mat source, cv::Mat1b& destination)
{
	cv::Mat_<cv::Vec3b> image = source;
	cv::Mat_<uchar> grayImage = cv::Mat_<uchar>(image.rows, image.cols);

	for (int x = 0; x < image.cols; x++)
	{
		for (int y = 0; y < image.rows; y++)
		{
			cv::Vec3b pixel = image(y, x);

			uchar B = pixel[0];
			uchar G = pixel[1];
			uchar R = pixel[2];
			//uchar value = (R + G + B) / 3;
			uchar value = (uchar)(0.299f * R + 0.587f * G + 0.114f * B);

			grayImage(y, x) = value;
		}
	}

	destination = grayImage;
}

void Converter::BGRToHLS(cv::Mat3b source, cv::Mat3b& destination)
{
	
	cv::Mat3b result = cv::Mat3b(source.rows, source.cols);

	for (int x = 0; x < source.cols; x++)
	{
		for (int y = 0; y < source.rows; y++)
		{
			float R = source(y, x)[2] / 255.f;
			float G = source(y, x)[1] / 255.f;
			float B = source(y, x)[0] / 255.f;

			float vMax = std::max({ R,G,B });
			float vMin = std::min({ R,G,B });

			float L = (vMax + vMin) / 2;
			
			float S;
			if (L < 0.5)
			{
				S = (vMax - vMin) / (vMax + vMin);
			}
			else
			{
				S = (vMax - vMin) / (2.f - (vMax + vMin));
			}

			float H = 0.f;
			if ((R == G) && (G == B))
			{
				H = 0.f;
			}
			else if (vMax == R)
			{
				H = 60.f * (G - B) / (vMax - vMin);
			}
			else if (vMax = G)
			{
				H = 120.f + 60.f * (B - R) / (vMax - vMin);
			}
			else if (vMax = B)
			{
				H = 240.f + 60.f * (R - G) / (vMax - vMin);
			}

			if (H < 0)
			{
				H += 360.f;
			}
			
			H /= 2.f;
			L *= 255.f;
			S *= 255.f;

			result(y, x) = cv::Vec3b((uchar)H, (uchar)L, (uchar)S);
		}
	}

	destination = result;
}

void Converter::BGRToHSV(cv::Mat3b source, cv::Mat3b& destination)
{
	
	cv::Mat3b result = cv::Mat3b(source.rows, source.cols);

	for (int x = 0; x < source.cols; x++)
	{
		for (int y = 0; y < source.rows; y++)
		{
			float R = source(y, x)[2] / 255.f;
			float G = source(y, x)[1] / 255.f;
			float B = source(y, x)[0] / 255.f;

			float rgbMin = std::min({ R,G,B });

			float V = std::max({ R,G,B });

			float S;
			if (V != 0.f)
			{
				S = (V - rgbMin) / V;
			}
			else
			{
				S = 0.f;
			}

			float H = 0.f;
			if ((R == G) && (G == B))
			{
				H = 0.f;
			}
			else if (V == R)
			{
				H = 60.f * (G - B) / (V - rgbMin);
			}
			else if (V = G)
			{
				H = 120.f + 60.f * (B - R) / (V - rgbMin);
			}
			else if (V = B)
			{
				H = 240.f + 60.f * (R - G) / (V - rgbMin);
			}

			if (H < 0)
			{
				H += 360.f;
			}

			H /= 2.f;
			S *= 255.f;
			V *= 255.f;

			result(y, x) = cv::Vec3b((uchar)H, (uchar)S, (uchar)V);
		}
	}

	destination = result;
}
