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
#include <stack>

static cv::Vec3b randomColor()
{
    static cv::RNG rng(12345);
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

bool isInRange(int value, int low, int high)
{
    return low <= value && value <= high;
}

void floodFill(cv::Mat3b& destination, cv::Mat3b& sourceHLS, cv::Mat1b& mask, cv::Vec3b color, cv::Point2i seed, int lowThreshold = 70, int highThreshold = 50, bool eightConnected = false) //high 50 jest ok, low 35 jest okej
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


    int channel = 2;
    int value = sourceHLS(seed)[channel];
    int low = value - lowThreshold;
    int high = value + highThreshold;

    while (!stack.empty())
    {
        cv::Point2i currentPixel = stack.top();
        stack.pop();

        destination(currentPixel) = color;
        mask(currentPixel) = done;

        int x = currentPixel.x;
        int y = currentPixel.y;


        cv::Point2i p = cv::Point2i(x, y - 1);
        if (p.y >= 0 && mask(p) != done && isInRange(sourceHLS(p)[channel], low, high))
        {
            stack.push(p);
        }
        p = cv::Point2i(x, y + 1);
        if (p.y < mask.rows && mask(p) != done && isInRange(sourceHLS(p)[channel], low, high))
        {
            stack.push(p);
        }
        p = cv::Point2i(x - 1, y);
        if (p.x >= 0 && mask(p) != done && isInRange(sourceHLS(p)[channel], low, high))
        {
            stack.push(p);
        }
        p = cv::Point2i(x + 1, y);
        if (p.x < mask.cols && mask(p) != done && isInRange(sourceHLS(p)[channel], low, high))
        {
            stack.push(p);
        }


        if (eightConnected)
        {
            
        }


    }


}

void hist()
{
    cv::Mat image;
    std::vector<cv::Mat> rgb;


    cv::split(image, rgb);

    cv::equalizeHist(rgb[0], rgb[0]);
    cv::equalizeHist(rgb[1], rgb[1]);
    cv::equalizeHist(rgb[2], rgb[2]);

    cv::merge(rgb, image);
}

void colorReduce(cv::Mat& image, int div = 64)
{
    int nl = image.rows;                    // number of lines
    int nc = image.cols * image.channels(); // number of elements per line

    for (int j = 0; j < nl; j++)
    {
        // get the address of row j
        uchar* data = image.ptr<uchar>(j);

        for (int i = 0; i < nc; i++)
        {
            // process each pixel
            data[i] = data[i] / div * div + div / 2;
        }
    }
}

int main(int, char* []) 
{
    (void)_setmode(_fileno(stdout), _O_U16TEXT);
    std::wcout << L"BlocksDetector, POBR project, Sławomir Nikiel" << std::endl << std::endl;

    std::vector<cv::Mat> images = IOHelper::loadImages(Args::dataDir);

    for (int i = 0; i < images.size(); i++)
    {
        cv::Mat3b image = images[i];

        /*Filtracja rankingowa*/

        image = Filter::rankFilter(image, 3, 5);
        IOHelper::outputImage(image, "rank" + std::to_string(i));

        /*Progowanie*/

        cv::Mat1b mask = threshold(image); 
        IOHelper::outputImage(mask, "threshold" + std::to_string(i));

        /*Dylacja i erozja*/

        cv::dilate(mask, mask, cv::Mat());
        cv::dilate(mask, mask, cv::Mat());
        cv::erode(mask, mask, cv::Mat());
        IOHelper::outputImage(mask, "mask" + std::to_string(i));

        /*Generowanie mapy segmentów*/

        cv::Mat1b tmpMask = mask.clone();
        cv::Mat3b destination = cv::Mat3b::zeros(image.rows, image.cols);
        colorReduce(image, 110);
        cv::cvtColor(image, image, cv::COLOR_BGR2HSV);
        for (int x = 0; x < image.cols; x++)
        {
            for (int y = 0; y < image.rows; y++)
            {
                floodFill(destination, image, tmpMask, randomColor(), cv::Point2i(x, y));
            }
        }

        IOHelper::outputImage(destination, "out" + std::to_string(i), true);
    }
    

    cv::waitKey(-1);
    return 0;
}