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

static cv::Vec3b randomColor()
{
    static cv::RNG rng(12345);
    return cv::Vec3b((uchar)rng.uniform(0, 255), (uchar)rng.uniform(0, 255), (uchar)rng.uniform(0, 255));
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

        ImageProcessor::rankFilter(image, image, 3, 5);
        IOHelper::outputImage(image, "rank" + std::to_string(i));

        /*Progowanie*/

        cv::Mat1b mask;
        ImageProcessor::threshold(image, mask);
        IOHelper::outputImage(mask, "threshold" + std::to_string(i));

        /*Dylacja i erozja*/

        ImageProcessor::dilate(mask, mask);
        ImageProcessor::dilate(mask, mask);
        ImageProcessor::erode(mask, mask);
        IOHelper::outputImage(mask, "mask" + std::to_string(i));

        /*Generowanie mapy segmentów*/

        SegmentMap segmentMap;
        ImageProcessor::generateSegmentMap(image, segmentMap, mask);
        IOHelper::outputImage(segmentMap.getMap(), "out" + std::to_string(i), true);
    }
    

    cv::waitKey(-1);
    return 0;
}