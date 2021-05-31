
#include "ImageProcessor.h"

int ImageProcessor::imageId = 0;

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
    int lowH = 3;
    int highH = 16;
    int lowS = 65;
    int highS = 190;
    int lowL = 0;
    int highL = 145;

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

std::vector<Segment> ImageProcessor::process(cv::Mat3b image)
{
    cv::Mat3b source = image.clone();

    IOHelper::outputImageInfo(std::wcout, imageId);

    /*Filtracja rankingowa*/

    IOHelper::outputLabel(std::wcout, L"Filtering...");
    cv::Mat filteredImage;
    ImageProcessor::rankFilter(source, filteredImage, 3, 5);
    IOHelper::outputDoneInformation(std::wcout);
    IOHelper::outputImage(filteredImage, "filter" + std::to_string(imageId));

    /*Progowanie*/

    IOHelper::outputLabel(std::wcout, L"Thresholding...");
    cv::Mat1b mask;
    ImageProcessor::threshold(filteredImage, mask);
    IOHelper::outputDoneInformation(std::wcout);
    IOHelper::outputImage(mask, "threshold" + std::to_string(imageId));

    /*Dylacja i erozja*/

    IOHelper::outputLabel(std::wcout, L"Dilate and erode...");
    ImageProcessor::dilate(mask, mask);
    ImageProcessor::dilate(mask, mask);
    ImageProcessor::erode(mask, mask);
    IOHelper::outputDoneInformation(std::wcout);
    IOHelper::outputImage(mask, "mask" + std::to_string(imageId));

    /*Generowanie mapy segmentów*/

    IOHelper::outputLabel(std::wcout, L"Generating segment map...");
    SegmentMap segmentMap;
    ImageProcessor::generateSegmentMap(filteredImage, segmentMap, mask);
    IOHelper::outputDoneInformation(std::wcout);
    IOHelper::outputImage(segmentMap.getMap(), "segmentMap" + std::to_string(imageId));


    /*Rozpoznawanie*/

    IOHelper::outputLabel(std::wcout, L"Classifying segments...");
    std::vector<Segment> recognised;
    std::vector<Segment> segments = segmentMap.getSegments();
    std::vector<Segment> ones;
    std::vector<Segment> zeros;

    for (auto& segment : segments)
    {
        segment.calculateGeometricMoments(false);
        Segment::Label label = segment.whoAmI();
        if (label != Segment::Label::unknown)
        {
            if (label != Segment::Label::one && label != Segment::Label::zero)
            {
                //segment.drawBox(source, IOHelper::mapLabel(label));
                recognised.push_back(segment);
            }
            else if (label == Segment::Label::one)
            {
                ones.push_back(segment);
            }
            else
            {
                zeros.push_back(segment);
            }
        }
    }

    std::vector<Segment>::iterator onesIt = ones.begin();
    Segment::Label label = Segment::Label::unknown;
    while (onesIt != ones.end())
    {

        std::vector<Segment>::iterator zerosIt = zeros.begin();
        while (zerosIt != zeros.end())
        {
            Segment potentialTen = Segment::merge(*onesIt, *zerosIt);
            label = potentialTen.whoAmI();
            if (label == Segment::Label::ten)
            {
                //potentialTen.drawBox(source, IOHelper::mapLabel(label));
                recognised.push_back(potentialTen);
                zerosIt = zeros.erase(zerosIt);
                break;
            }
            else
                zerosIt++;
        }

        if (label == Segment::Label::ten)
        {
            onesIt = ones.erase(onesIt);
            label = Segment::Label::unknown;
        }
        else
            onesIt++;
    }

    recognised.insert(recognised.end(), ones.begin(), ones.end());
    recognised.insert(recognised.end(), zeros.begin(), zeros.end());
    IOHelper::outputDoneInformation(std::wcout);
    
    drawBoundingBoxes(source, recognised);

    imageId++;

    return recognised;
}

void ImageProcessor::drawBoundingBoxes(cv::Mat3b source, std::vector<Segment> segments)
{
    /*Rysowanie bounding boxów*/
    IOHelper::outputLabel(std::wcout, L"Drawing bounding boxes...");
    std::wstring listOfNumbers = L"";
    for (auto& segment : segments)
    {
        std::string label = IOHelper::mapLabel(segment.whoAmI());
        segment.drawBox(source, label);
        std::wstring wLabel(label.begin(), label.end());
        listOfNumbers += wLabel + L" ";
    }

    IOHelper::outputResults(std::wcout, listOfNumbers);
    IOHelper::outputDoneInformation(std::wcout);
    IOHelper::outputImage(source, "out" + std::to_string(imageId), Args::showOutput, true);
}

bool ImageProcessor::isInRange(int value, int low, int high)
{
    return low <= value && value <= high;
}
