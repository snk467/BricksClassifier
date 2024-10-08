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

void SegmentMap::setMap(cv::Point2i p, cv::Vec3b color)
{
	if (std::find(mUsedColors.begin(), mUsedColors.end(), color) == mUsedColors.end())
	{
		mUsedColors.push_back(color);
	}


	if (!mSegments.empty())
	{
		mMap(p) = color;
		mSegments.back().addPoint(p);
	}
}

void SegmentMap::newSegment()
{
	mSegments.push_back(Segment());
}

std::vector<Segment> SegmentMap::getSegments(bool filterSmall)
{
	if (filterSmall)
	{
		std::vector<Segment> filteredSegments;
		for (auto& segment : mSegments)
		{
			if (segment.area() > 300)
			{
				filteredSegments.push_back(segment);
			}
		}

		return filteredSegments;
	}

	return mSegments;
}

cv::Mat3b SegmentMap::getMap()
{
	return mMap;
}
