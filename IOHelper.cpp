#include "IOHelper.h"


using std::string;

std::vector<cv::Mat> IOHelper::loadImages(string path)
{
    std::wcout << "Loading images..." << std::endl;
    std::vector<cv::String> jpgFilesNames;
    std::vector<cv::String> pngFilesNames;
    cv::glob(path + "\\*.jpg", jpgFilesNames, false);
    cv::glob(path + "\\*.png", pngFilesNames, false);

    std::vector<cv::String> filesNames = std::vector<cv::String>(jpgFilesNames);
    filesNames.insert(filesNames.end(), pngFilesNames.begin(), pngFilesNames.end());

    std::vector<cv::Mat> images = std::vector<cv::Mat>();
    for (size_t i = 0; i < filesNames.size(); i++)
    {
        images.push_back(cv::imread(filesNames[i]));
    }
    std::wcout << "> " << images.size() << " images loaded" << std::endl;

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

std::string IOHelper::mapLabel(Segment::Label label)
{
    std::string labelString = "";
    switch (label)
    {
    case Segment::Label::zero:
        labelString = "0";
        break;
    case Segment::Label::one:
        labelString = "1";
        break;
    case Segment::Label::three:
        labelString = "3";
        break;
    case Segment::Label::four:
        labelString = "4";
        break;
    case Segment::Label::five:
        labelString = "5";
        break;
    case Segment::Label::eight:
        labelString = "8";
        break;
    case Segment::Label::ten:
        labelString = "10";
        break;
    case Segment::Label::unknown:
        break;
    default:
        break;
    }

    return labelString;
}
