#include "FileSystemHelper.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>


using std::string;

std::vector<cv::Mat> FileSystemHelper::loadImages(string path)
{
    std::wcout << "Loading images..." << std::endl;
    std::vector<cv::String> filesNames;
    cv::glob(path + "\\*", filesNames, false);
    std::vector<cv::Mat> images = std::vector<cv::Mat>();
    for (size_t i = 0; i < filesNames.size(); i++)
    {
        images.push_back(cv::imread(filesNames[i]));
    }
    std::wcout << images.size() << " images loaded" << std::endl;

    return images;
}
