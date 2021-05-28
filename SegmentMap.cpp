#include "SegmentMap.h"

SegmentMap::SegmentMap(int cols, int rows)
{
	mMap = cv::Mat3b::zeros(rows, cols);
	mUsedColors = std::vector<cv::Vec3b>();
	mRng = cv::RNG(12345);
	mUsedColors.push_back(cv::Vec3b(0, 0, 0));
}

cv::Vec3b SegmentMap::uniqueRandomColor()
{
	cv::Vec3b color;
	do
	{
		color = cv::Vec3b((uchar)mRng.uniform(0, 255), (uchar)mRng.uniform(0, 255), (uchar)mRng.uniform(0, 255));
	} while (std::find(mUsedColors.begin(), mUsedColors.end(), color) != mUsedColors.end());
	
	return color;
}

cv::Vec3b& SegmentMap::operator()(cv::Point2i p)
{
	return mMap(p);
}

std::vector<Segment> SegmentMap::generateSegments()
{
	return std::vector<Segment>();
}

cv::Mat3b SegmentMap::getMap()
{
	return mMap;
}
