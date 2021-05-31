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

void getAnswer(std::string& answer)
{
	do
	{
		std::getline(std::cin, answer);
	} while (answer != "" && answer != "y" && answer != "N" && answer != "Y" && answer != "n");
}

bool answerNegative(std::string& answer)
{
	return answer == "N" || answer == "n";
}

bool answerPositive(std::string& answer)
{
	return answer == "Y" || answer == "y";
}

int main(int, char* [])
{
	(void)_setmode(_fileno(stdout), _O_U16TEXT);


	std::wcout << L"BlocksDetector, POBR project, Sławomir Nikiel" << std::endl << std::endl;

	std::wcout << L"Calibration mode? [y/N]" << std::endl;
	std::string answer;

	getAnswer(answer);

	if (answerPositive(answer))
	{
		Args::calibration = true;
	}	

	if (Args::calibration)
	{
		std::vector<cv::Mat> masks = IOHelper::loadImages(Args::modelsDir);
		std::wcout << std::endl;
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
			std::wcout << std::endl;
		}
	}
	else
	{
		std::filesystem::remove_all(Args::outPath);

		std::wcout << L"Verbose? [y/N]" << std::endl;

		getAnswer(answer);

		if (answerPositive(answer))
		{
			Args::verbose = true;
		}

		std::wcout << L"Show output? [Y/n]" << std::endl;

		getAnswer(answer);

		if (answerNegative(answer))
		{
			Args::showOutput = false;
		}

		std::vector<cv::Mat> images = IOHelper::loadImages(Args::dataDir);

		for (int i = 0; i < images.size(); i++)
		{
			cv::Mat3b source = images[i];
			std::vector<Segment> recognised;
			recognised = ImageProcessor::process(source);			
		}
	}

	cv::waitKey(0);
	return 0;
}