#include "IOHelper.h"

using std::string;

std::vector<cv::Mat> IOHelper::loadImages(string path)
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

void IOHelper::outputImage(cv::Mat image, std::string title, bool show, bool save)
{
    if (save)
    {
        cv::imwrite(title + ".png", image);
    }
    if (show)
    {
        int cols = image.cols;
        int rows = image.rows;
        while (rows > 800)
        {
            rows /= 2;
            cols /= 2;
        }
        cv::resize(image, image, cv::Size(cols, rows));
        cv::imshow(title, image);
    }
}
