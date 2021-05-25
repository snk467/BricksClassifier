#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <iostream>
#include <io.h>
#include <fcntl.h>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "Args.h"
#include "FileSystemHelper.h"



int main(int, char* []) 
{
    (void)_setmode(_fileno(stdout), _O_U16TEXT);
    std::wcout << L"BlocksDetector, POBR project, Sławomir Nikiel" << std::endl << std::endl;

    std::vector<cv::Mat> images = FileSystemHelper::loadImages(Args::dataDir);

    for (int i = 0; i < images.size(); i++)
    {
        std::vector<cv::Mat> image = images[i];
    }

    return 0;
}