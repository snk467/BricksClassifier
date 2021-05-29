
#include "ImageProcessor.h"


void ImageProcessor::colorReduce(cv::Mat source, cv::Mat& destination, int div)
{
    if (!(&destination == &source))
    {
        destination = source.clone();        
    }

    int nl = destination.rows;
    int nc = destination.cols * destination.channels();

    for (int j = 0; j < nl; j++)
    {
        uchar* data = destination.ptr<uchar>(j);

        for (int i = 0; i < nc; i++)
        {
            data[i] = data[i] / div * div + div / 2;
        }
    }
}

void ImageProcessor::threshold(cv::Mat source, cv::Mat1b& destination)
{
    cv::Mat_<uchar> outImage = cv::Mat::zeros(source.size(), CV_8U);

    cv::Mat_<cv::Vec3b> image;
    Converter::BGRToHLS(source, image);
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
            if (image(y, x)[2] >= lowS && image(y, x)[2] <= highS && image(y, x)[0] >= lowH && image(y, x)[0] <= highH && image(y, x)[1] >= lowL && image(y, x)[1] <= highL)
            {
                outImage(y, x) = 255;
            }
        }
    }

    destination = outImage;
}

void ImageProcessor::floodFill(cv::Mat3b source, SegmentMap& destination, cv::Mat1b& mask, cv::Vec3b color, cv::Point2i seed, int lowThreshold, int highThreshold, bool eightConnected)
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
        mask(seed) = done;
    }


    int channel = 2;
    int value = source(seed)[channel];
    int low = value - lowThreshold;
    int high = value + highThreshold;
    destination.newSegment();

    while (!stack.empty())
    {
        cv::Point2i currentPixel = stack.top();
        stack.pop();

        destination.setMap(currentPixel, color);

        int x = currentPixel.x;
        int y = currentPixel.y;


        cv::Point2i p = cv::Point2i(x, y - 1);
        if (p.y >= 0 && mask(p) != done && isInRange(source(p)[channel], low, high))
        {
            stack.push(p);
            mask(p) = done;
        }
        p = cv::Point2i(x, y + 1);
        if (p.y < mask.rows && mask(p) != done && isInRange(source(p)[channel], low, high))
        {
            stack.push(p);
            mask(p) = done;
        }
        p = cv::Point2i(x - 1, y);
        if (p.x >= 0 && mask(p) != done && isInRange(source(p)[channel], low, high))
        {
            stack.push(p);
            mask(p) = done;
        }
        p = cv::Point2i(x + 1, y);
        if (p.x < mask.cols && mask(p) != done && isInRange(source(p)[channel], low, high))
        {
            stack.push(p);
            mask(p) = done;
        }


        if (eightConnected)
        {
            //TODO
        }
    }
}

void ImageProcessor::generateSegmentMap(cv::Mat3b source, SegmentMap& destination, cv::Mat1b mask)
{
    cv::Mat1b auxMask = mask.clone();
    destination = SegmentMap(source.cols, source.rows);
    ImageProcessor::colorReduce(source, source, 110);
    //cv::cvtColor(source, source, cv::COLOR_BGR2HSV);
    Converter::BGRToHSV(source, source);
    for (int x = 0; x < source.cols; x++)
    {
        for (int y = 0; y < source.rows; y++)
        {
            ImageProcessor::floodFill(source, destination, auxMask, destination.uniqueRandomColor(), cv::Point2i(x, y));
        }
    }
}

void ImageProcessor::rankFilter(cv::Mat source, cv::Mat& destination, int N, int index)
{
    // Sprawdzanie danych wej�ciowych 
    if (N % 2 == 0 || index < 0 || index >= N * N)
    {
        throw std::exception();
    }

    cv::Mat_<cv::Vec3b> image = source.clone();
    cv::Mat_<uchar> grayImage;
    Converter::BGRToGray(image, grayImage);
    cv::Mat_<cv::Vec3b> outImage = image.clone();
    for (int x = N / 2; x < image.cols - N / 2; x++)
    {
        for (int y = N / 2; y < image.rows - N / 2; y++)
        {
            // Lista do przechowywania przetwarzanych pikseli
            std::vector<std::pair<cv::Vec3b, uchar>> pixelsList = std::vector<std::pair<cv::Vec3b, uchar>>();
            // Gromadzenie przetwarzanych pikseli
            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    pixelsList.push_back(std::pair<cv::Vec3b, uchar>(image(y - N / 2 + j, x - N / 2 + i), grayImage(y - N / 2 + j, x - N / 2 + i)));
                }
            }
            std::sort(pixelsList.begin(), pixelsList.end(),
                [](const std::pair<cv::Vec3b, uchar>& a, const std::pair<cv::Vec3b, uchar>& b)
                {
                    return a.second < b.second;
                });

            // Ustawianie piksela wynikowego na odpowiedni� warto��
            outImage(y, x) = pixelsList[index].first;
        }
    }

    destination = outImage;
}

void ImageProcessor::morphologyFilter(cv::Mat1b source, cv::Mat1b& destination, bool dilate = true, cv::Mat1b kernel = cv::Mat())
{
    //cv::dilate(source, destination, cv::Mat());

    if (kernel.empty())
    {
        kernel = cv::Mat1b::ones(3, 3);
    }

    cv::Mat1b image = source.clone();
    destination = cv::Mat1b::zeros(image.rows, image.cols);

    for (int x = 1; x < image.cols - 1; x++)
    {
        for (int y = 1; y < image.rows - 1; y++)
        {
            // Lista do przechowywania przetwarzanych pikseli
            std::vector<uchar> pixelsList = std::vector<uchar>();
            // Gromadzenie przetwarzanych pikseli
            for (int i = 0; i < kernel.cols; i++)
            {
                for (int j = 0; j < kernel.rows; j++)
                {
                    if (kernel(j, i) != 0)
                    {
                        pixelsList.push_back(image(y - kernel.rows / 2 + j, x - kernel.cols / 2 + i));
                    }
                }
            }

            if (dilate)
            {
                destination(y, x) = *std::max_element(pixelsList.begin(), pixelsList.end());
            }
            else
            {
                destination(y, x) = *std::min_element(pixelsList.begin(), pixelsList.end());
            }
        }
    }

}

void ImageProcessor::dilate(cv::Mat1b source, cv::Mat1b& destination)
{
    ImageProcessor::morphologyFilter(source, destination);
}

void ImageProcessor::erode(cv::Mat1b source, cv::Mat1b& destination)
{
    ImageProcessor::morphologyFilter(source, destination, false);
}

bool ImageProcessor::isInRange(int value, int low, int high)
{
    return low <= value && value <= high;
}
