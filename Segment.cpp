#include "Segment.h"

void Segment::addPoint(cv::Point2i p)
{
	mPoints.push_back(p);
}

void Segment::calculateGeometricMoments()
{
    mXCenter = m(1, 0) / m(0, 0);
    mYCenter = m(0, 1) / m(0, 0);
}

void Segment::drawBox(cv::Mat& image)
{

    int topY = std::numeric_limits<int>::max();
    int bottomY = std::numeric_limits<int>::min();
    int leftX = std::numeric_limits<int>::max();
    int rightX = std::numeric_limits<int>::min();


    for (auto& point : mPoints)
    {
        if (topY > point.y)
        {
            topY = point.y;
        }
        if (bottomY < point.y)
        {
            bottomY = point.y;
        }
        if (leftX > point.x)
        {
            leftX = point.x;
        }
        if (rightX < point.x)
        {
            rightX = point.x;
        }
    }

    cv::Point2i topLeft = cv::Point2i(leftX, topY);
    cv::Point2i bottomRight = cv::Point2i(rightX, bottomY);
    cv::Scalar color = cv::Scalar(0, 255, 127, 255);

    cv::rectangle(image, topLeft, bottomRight, color, 2);
}

float Segment::m(int p, int q)
{
    float m = 0.0f;

    for (auto& point : mPoints)
    {
        m += (float)(pow(point.x, p) * pow(point.y, q));
    }

    return m;
}

float Segment::M(int p, int q)
{
    float M = 0.0f;

    for (auto& point : mPoints)
    {
        M += (float)(pow(point.x - mXCenter, p) * pow(point.y - mYCenter, q));
    }

    return M;
}