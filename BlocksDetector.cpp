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

	std::wcout << L"Display calibration data? [y/N]" << std::endl;
	std::string answer;

	do
	{
		std::getline(std::cin, answer);
	}while (answer != "" && answer != "y" && answer != "N");

	bool calibration = false;
	if (answer == "y")
	{
		calibration = true;
	}


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
			std::vector<Segment> ones;
			std::vector<Segment> zeros;

			for (auto& segment : segments)
			{
				segment.calculateGeometricMoments(false);
				Segment::Label label = segment.whoAmI();
				if (label != Segment::Label::unknown)
				{
					if (label != Segment::Label::one && label != Segment::Label::zero)
					{
						segment.drawBox(source, IOHelper::mapLabel(label));
					}
					else if(label == Segment::Label::one)
					{
						ones.push_back(segment);
					}
					else
					{
						zeros.push_back(segment);
					}
				}
			}

			std::vector<Segment>::iterator onesIt = ones.begin();
			std::vector<Segment>::iterator zerosIt = zeros.begin();

			Segment::Label label = Segment::Label::unknown;
			while (onesIt != ones.end()) 
			{
				while (zerosIt != zeros.end())
				{
					Segment potentialTen = Segment::merge(*onesIt, *zerosIt);
					std::wcout << "Merged one and zero of size: " << (*onesIt).area() << " " << (int)(*onesIt).whoAmI() << " " << (*zerosIt).area() << " " << (int)(*zerosIt).whoAmI()  << " " << potentialTen.area() << std::endl;
					label = potentialTen.whoAmI();
					if (label == Segment::Label::ten)
					{
						std::wcout << "Ten detected, size: " << potentialTen.area() << std::endl;
						potentialTen.drawBox(source, IOHelper::mapLabel(label));
						std::wcout << "Removed zero of size: " << (*zerosIt).area() << std::endl;
						zerosIt = zeros.erase(zerosIt);
						break;
					}
					else
						zerosIt++;
				}

				if (label == Segment::Label::ten)
				{
					std::wcout << "Removed one of size: " << (*onesIt).area() << std::endl;
					onesIt = ones.erase(onesIt);
					label = Segment::Label::unknown;
				}
				else
					onesIt++;
			}

			for (auto& s : ones)
			{
				Segment::Label label = s.whoAmI();
				std::wcout << "Retrived one of size: " << s.area() << " and label: " << (int)label << std::endl;
				s.drawBox(source, IOHelper::mapLabel(label));
			}

			for (auto& s : zeros)
			{
				Segment::Label label = s.whoAmI();
				std::wcout << "Retrived zero of size: " << s.area() << " and label: " << (int)label << std::endl;
				s.drawBox(source, IOHelper::mapLabel(label));
			}



			IOHelper::outputImage(source, "out" + std::to_string(i), true);
		}
	}

	cv::waitKey(-1);
	return 0;
}