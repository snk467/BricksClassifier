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
#include <stack>


void showImage(cv::Mat image, std::string title)
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

static cv::Vec3b randomColor()
{
    cv::RNG rng(12345);
    return cv::Vec3b((uchar)rng.uniform(0, 255), (uchar)rng.uniform(0, 255), (uchar)rng.uniform(0, 255));
}

cv::Mat threshold(cv::Mat source)
{
    cv::Mat_<uchar> outImage = cv::Mat::zeros(source.size(), CV_8U);

    cv::Mat_<cv::Vec3b> image;
    cv::cvtColor(source, image, cv::COLOR_BGR2HLS);

    int lowH = 3; //0 -- HSV //3 -- HLS
    int highH = 16; //15 -- HSV //16 -- HLS
    int lowS = 65; //106 -- HSV //65 -- HLS
    int highS = 190; //230 -- HSV //170 -- HLS
    int lowL = 0; //0 -- HLS
    int highL = 145; //145 -- HLS

    for (int x = 0; x < image.cols; x++)
    {
        for (int y = 0; y < image.rows; y++)
        {
            if (image(y,x)[2] >= lowS && image(y, x)[2] <= highS && image(y, x)[0] >= lowH && image(y, x)[0] <= highH && image(y, x)[1] >= lowL && image(y, x)[1] <= highL)
            {
                outImage(y, x) = 255;
            }
        }
    }

    return outImage;
}

void floodFill(cv::Mat_<cv::Vec3b>& image, cv::Mat_<uchar>& mask, cv::Vec3b color, cv::Point2i seed, int threshold, bool eightConnected = true)
{
    uchar done = 0;
    std::stack<cv::Point2i> stack = std::stack<cv::Point2i>();

    if (mask(seed) == done)
    {
        return;
    }
    else
    {
        stack.push(seed);
    }    

    while (!stack.empty())
    {
        cv::Point2i currentPixel = stack.top();
        stack.pop();

        image(currentPixel) = color;
        mask(currentPixel) = done;

        if (true)
        {
            
        }


    }


}

int main(int, char* []) 
{
    (void)_setmode(_fileno(stdout), _O_U16TEXT);
    std::wcout << L"BlocksDetector, POBR project, Sławomir Nikiel" << std::endl << std::endl;

    std::vector<cv::Mat> images = FileSystemHelper::loadImages(Args::dataDir);

    for (int i = 0; i < images.size(); i++)
    {
        cv::Mat image = images[i];

        image = Filter::rankFilter(image, 3, 5);
        showImage(image, "rank" + std::to_string(i));
        cv::imwrite("rank" + std::to_string(i) + ".jpg", image);

        image = threshold(image);

        cv::dilate(image, image, cv::Mat());
        cv::dilate(image, image, cv::Mat());
        cv::erode(image, image, cv::Mat());

        showImage(image, "out" + std::to_string(i));
        cv::imwrite("out" + std::to_string(i) + ".jpg", image);
    }
    

    cv::waitKey(-1);
    return 0;
}