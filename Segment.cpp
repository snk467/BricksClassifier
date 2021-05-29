#include "Segment.h"

void Segment::addPoint(cv::Point2i p)
{
	mPoints.push_back(p);
	mGeometricMomentsUpToDate = false;
}

void Segment::calculateGeometricMoments()
{
	mXCenter = m(1, 0) / m(0, 0);
	mYCenter = m(0, 1) / m(0, 0);

	m_00 = m(0, 0);
	M_02 = M(0, 2);
	M_03 = M(0, 3);
	M_11 = M(1, 1);
	M_12 = M(1, 2);
	M_20 = M(2, 0);
	M_21 = M(2, 1);
	M_30 = M(3, 0);

	(void)M1();
	(void)M2();
	(void)M3();
	(void)M4();
	(void)M5();
	(void)M6();
	(void)M7();
	(void)M8();
	(void)M9();
	(void)M10();

	mGeometricMomentsUpToDate = true;
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

float Segment::M1()
{
	static float value;

	if (!mGeometricMomentsUpToDate)
	{
		value = (M_20 + M_02) / (float)pow(m_00, 2);
	}

	return value;
}

float Segment::M2()
{
	static float value;
	if (!mGeometricMomentsUpToDate)
	{
		value = ((float)pow(M_20 - M_02, 2) + 4.f * (float)pow(M_11, 2)) / (float)pow(m_00, 4);
	}
	return value;
}

float Segment::M3()
{
	static float value;
	if (!mGeometricMomentsUpToDate)
	{
		value = ((float)pow(M_30 - 3.f * M_12, 2) + (float)pow(3.f * M_21 - M_03, 2)) / (float)pow(m_00, 5);
	}
	return value;
}

float Segment::M4()
{
	static float value;
	if (!mGeometricMomentsUpToDate)
	{
		value = ((float)pow(M_30 + M_12, 2) + (float)pow(M_21 + M_03, 2)) / (float)pow(m_00, 5);
	}
	return value;
}

float Segment::M5()
{
	static float value;
	if (!mGeometricMomentsUpToDate)
	{
		value = ((M_30 - 3.f * M_12) * (M_30 + M_12) * ((float)pow(M_30 + M_12, 2) - 3.f * (float)pow(M_21 + M_03, 2)) + (3.f * M_21 - M_03) * (M_21 + M_03) * (3.f * (float)pow(M_30 + M_12, 2) - (float)pow(M_21 + M_03, 2))) / (float)pow(m_00, 10);
	}
	return value;
}

float Segment::M6()
{
	static float value;
	if (!mGeometricMomentsUpToDate)
	{
		value = ((M_20 - M_02) * ((float)pow(M_30 + M_12, 2) - (float)pow(M_21 + M_03, 2)) + 4.f * M_11 * (M_30 + M_12) * (M_21 + M_03)) / (float)pow(m_00, 7);
	}
	return value;
}

float Segment::M7()
{
	static float value;
	if (!mGeometricMomentsUpToDate)
	{
		value = (M_20 * M_02 - (float)pow(M_11, 2)) / (float)pow(m_00, 4);
	}
	return value;
}

float Segment::M8()
{
	static float value;
	if (!mGeometricMomentsUpToDate)
	{
		value = (M_30 * M_12 + M_21 * M_03 - (float)pow(M_12, 2) - (float)pow(M_21, 2)) / (float)pow(m_00, 5);
	}
	return value;
}

float Segment::M9()
{
	static float value;
	if (!mGeometricMomentsUpToDate)
	{
		value = (M_20 * (M_21 * M_03 - (float)pow(M_12, 2)) + M_02 * (M_03 * M_12 - (float)pow(M_21, 2)) - M_11 * (M_30 * M_03 - M_21 * M_12)) / (float)pow(m_00, 7);
	}
	return value;
}

float Segment::M10()
{
	static float value;
	if (!mGeometricMomentsUpToDate)
	{
		value = ((float)pow(M_30 * M_03 - M_12 * M_21, 2) - 4.f * (M_30 * M_12 - (float)pow(M_21, 2)) * (M_03 * M_21 - M_12)) / (float)pow(m_00, 10);
	}
	return value;
}