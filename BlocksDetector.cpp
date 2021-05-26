#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <io.h>
#include <fcntl.h>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "Args.h"
#include "FileSystemHelper.h"
#include "Converter.h"
#include "Filter.h"


void showImage(cv::Mat image, std::string title)
{
    cv::resize(image, image, cv::Size(image.cols, image.rows));
    cv::imshow(title, image); 
}

cv::Mat hsvThreshold(cv::Mat source)
{
    cv::Mat_<cv::Vec3b> outImage = source.clone();
    cv::Mat_<cv::Vec3b> image;
    cv::cvtColor(source, image, cv::COLOR_BGR2HSV);

    // H - 0-179
    // S - 0-255

    int lowH = 0; //0
    int highH = 15; //15 -- hist 25/27
    int lowS = 106;//95 --hist 55 #new 107
    int highS = 230;//230
    int highV = 210;//190

    for (int x = 0; x < image.cols; x++)
    {
        for (int y = 0; y < image.rows; y++)
        {
            if (image(y,x)[1] >= lowS && image(y, x)[1] <= highS && image(y, x)[0] >= lowH && image(y, x)[0] <= highH /*&& image(y,x)[2] < highV*/)
            {
                outImage(y, x)[0] = outImage(y, x)[1] = outImage(y, x)[2] = 255;
            }
            else
            {
                outImage(y, x)[0] = outImage(y, x)[1] = outImage(y,x)[2] = 0;
            }
        }
    }

    return outImage;
}




int main(int, char* []) 
{
    (void)_setmode(_fileno(stdout), _O_U16TEXT);
    std::wcout << L"BlocksDetector, POBR project, Sławomir Nikiel" << std::endl << std::endl;

    std::vector<cv::Mat> images = FileSystemHelper::loadImages(Args::dataDir);

    for (int i = 0; i < images.size(); i++)
    {
        cv::Mat image = images[i];

        image = Filter::rankFilter(image, 3, 2);
        showImage(image, "rank" + std::to_string(i));
        cv::imwrite("rank" + std::to_string(i) + ".jpg", image);

        image = hsvThreshold(image);
        showImage(image, std::to_string(i));
        cv::imwrite("out" + std::to_string(i) + ".jpg", image);
    }
    

    cv::waitKey(-1);
    return 0;
}