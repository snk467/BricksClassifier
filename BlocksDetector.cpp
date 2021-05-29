#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <io.h>
#include <fcntl.h>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "Args.h"
#include "IOHelper.h"
#include "Converter.h"
#include "Filter.h"
#include "ImageProcessor.h"
#include <stack>
#include <filesystem>

int main(int, char* [])
{
	(void)_setmode(_fileno(stdout), _O_U16TEXT);
	std::wcout << L"BlocksDetector, POBR project, Sławomir Nikiel" << std::endl << std::endl;

	bool calibration = false;

	if (calibration)
	{
		std::vector<cv::Mat> masks = IOHelper::loadImages(Args::modelsDir);
		std::vector<cv::String> filesNames;
		cv::glob(Args::modelsDir + "\\*", filesNames, false);		

		for (int i = 0; i < masks.size(); i++)
		{
			cv::Mat1b m;
			Converter::BGRToGray(masks[i], m);
			Segment segment = Segment();
			for (int x = 0; x < m.cols; x++)
			{
				for (int y = 0; y < m.rows; y++)
				{
					cv::Point2i p = cv::Point2i(x, y);
					if (m(p) == 255)
					{
						segment.addPoint(p);
					}
				}
			}
			std::wcout << std::filesystem::path(filesNames[i]).filename() << std::endl;
			segment.calculateGeometricMoments(true);
		}
	}
	else
	{
		std::vector<cv::Mat> images = IOHelper::loadImages(Args::dataDir);

		for (int i = 0; i < images.size(); i++)
		{
			cv::Mat3b source = images[i];


			/*Filtracja rankingowa*/

			cv::Mat filteredImage;
			ImageProcessor::rankFilter(source, filteredImage, 3, 5);
			IOHelper::outputImage(filteredImage, "rank" + std::to_string(i));

			/*Progowanie*/

			cv::Mat1b mask;
			ImageProcessor::threshold(filteredImage, mask);
			IOHelper::outputImage(mask, "threshold" + std::to_string(i));

			/*Dylacja i erozja*/

			ImageProcessor::dilate(mask, mask);
			ImageProcessor::dilate(mask, mask);
			ImageProcessor::erode(mask, mask);
			IOHelper::outputImage(mask, "mask" + std::to_string(i));

			/*Generowanie mapy segmentów*/

			SegmentMap segmentMap;
			ImageProcessor::generateSegmentMap(filteredImage, segmentMap, mask);
			IOHelper::outputImage(segmentMap.getMap(), "segmentMap" + std::to_string(i));

			/*Rysowanie bounding boxów*/

			std::vector<Segment> segments = segmentMap.getSegments();

			for (auto& segment : segments)
			{
				segment.calculateGeometricMoments(false);
				if (segment.whoAmI() != Segment::Label::unknown)
				{
					segment.drawBox(source);
				}

			}

			IOHelper::outputImage(source, "out" + std::to_string(i), true);
		}
	}

	cv::waitKey(-1);
	return 0;
}