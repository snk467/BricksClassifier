#include "Segment.h"

Segment::Segment()
{
	mXCenter = std::numeric_limits<float>::max();
	mYCenter = std::numeric_limits<float>::max();
	m_00 = std::numeric_limits<float>::max();
	M_02 = std::numeric_limits<float>::max();
	M_03 = std::numeric_limits<float>::max();
	M_11 = std::numeric_limits<float>::max();
	M_12 = std::numeric_limits<float>::max();
	M_20 = std::numeric_limits<float>::max();
	M_21 = std::numeric_limits<float>::max();
	M_30 = std::numeric_limits<float>::max();
}

void Segment::addPoint(cv::Point2i p)
{
	mPoints.push_back(p);
	mGeometricMomentsUpToDate = false;
}

void Segment::calculateGeometricMoments(bool verbose)
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

	if (verbose)
	{		
		std::wcout << "area: " << area() << std::endl;
		std::wcout << "m_00: " << m_00 << std::endl;
		std::wcout << "M1: " << M1() << std::endl;
		std::wcout << "M2: " << M2() << std::endl;
		std::wcout << "M3: " << M3() << std::endl;
		std::wcout << "M4: " << M4() << std::endl;
		std::wcout << "M5: " << M5() << std::endl;
		std::wcout << "M6: " << M6() << std::endl;
		std::wcout << "M7: " << M7() << std::endl;
		std::wcout << "M8: " << M8() << std::endl;
		std::wcout << "M9: " << M9() << std::endl;
		std::wcout << "M10: " << M10() << std::endl;
	}
}

void Segment::drawBox(cv::Mat& image, std::string label)
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
	if (label != "")
	{
		int baseline = 0;
		int thickness = 2;
		cv::Size size = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 1, thickness, &baseline);
		
		baseline += thickness;

		cv::Point2i org = cv::Point2i(bottomRight.x, bottomRight.y + size.height);

		cv::putText(image, label, org, cv::FONT_HERSHEY_SIMPLEX, 1, color, thickness);
	}
}

float Segment::area()
{
	return mPoints.size();
}

Segment Segment::merge(Segment s1, Segment s2)
{
	std::vector<cv::Point2i> mergedPoints;
	mergedPoints.insert(mergedPoints.end(), s1.mPoints.begin(), s1.mPoints.end());
	mergedPoints.insert(mergedPoints.end(), s2.mPoints.begin(), s2.mPoints.end());

	Segment mergedSegment = Segment();
	mergedSegment.mPoints = mergedPoints;

	mergedSegment.calculateGeometricMoments();

	return mergedSegment;
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


Segment::Label Segment::whoAmI()
{
	// 0
	if (isZero())
	{
		return Label::zero;
	}
	// 1
	if (isOne())
	{
		return Label::one;
	}
	// 3
	if (isThree())
	{
		return Label::three;
	}
	// 4
	if (isFour())
	{
		return Label::four;
	}
	// 5
	if (isFive())
	{
		return Label::five;
	}
	// 8
	if (isEight())
	{
		return Label::eight;
	}
	// 10
	if (isTen())
	{
		return Label::ten;
	}

	return Label::unknown;
}

bool Segment::isZero()
{
	return isInRange(M1(), 0.33f, 0.44f) && isInRange(M2(), 0.025f , 0.1f) && isInRange(M3(), 0.f, 0.00031f) && isInRange(M6(), -0.0001f, 0.00003f);
}
bool Segment::isOne()
{
	return 
		isInRange(M1(), 0.44f, 0.67f) &&
		isInRange(M2(), 0.1f, 0.4f) &&
		isInRange(M3(), 0.01f, 0.042f) &&
		isInRange(M4(), 0.003f, 0.022f) &&
		isInRange(M6(), 0.001f, 0.014f) &&
		isInRange(M7(), 0.01f, 0.05f) &&
		isInRange(M8(), -0.003f, -0.0009f) &&
		isInRange(M9(), 0.00028f, 0.0021f);
}

bool Segment::isThree()
{
	return isInRange(M1(), 0.39f, 0.7f) && isInRange(M2(), 0.04f, 0.07f) && isInRange(M7(), 0.05f, 0.07f) && isInRange(M4(), 0.0042f, 0.05f);
}

bool Segment::isFour()
{
	return isInRange(M1(), 0.4f, 0.5f) && isInRange(M3(), 0.02f, 0.025f) && isInRange(M7(), 0.035f, 0.041f);
}

bool Segment::isFive()
{
	return 0.39f <= M1() && M1() <= 0.7f && 0.02f <= M2() && M2() <= 0.2f && 0.03f <= M7() && M7() <= 0.095f && M4() <= 0.0042f && M3() <= 0.0071f;
}

bool Segment::isEight()
{
	return isInRange(M1(), 0.24f, 0.32f) && isInRange(M2(), 0.005f, 0.007f) && isInRange(M7(), 0.01f, 0.023f);
}

bool Segment::isTen()
{
	return isInRange(M1(), 0.35f, 0.36f) && isInRange(M2(), 0.0032f, 0.0034f) && isInRange(M3(), 0.001f, 0.0012f);
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

bool Segment::isInRange(float value, float low, float high)
{
	return low <= value && value <= high;
}