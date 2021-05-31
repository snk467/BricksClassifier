#include "Segment.h"

Segment::Segment()
{
	mXCenter = std::numeric_limits<double>::max();
	mYCenter = std::numeric_limits<double>::max();
	m_00 = std::numeric_limits<double>::max();
	M_02 = std::numeric_limits<double>::max();
	M_03 = std::numeric_limits<double>::max();
	M_11 = std::numeric_limits<double>::max();
	M_12 = std::numeric_limits<double>::max();
	M_20 = std::numeric_limits<double>::max();
	M_21 = std::numeric_limits<double>::max();
	M_30 = std::numeric_limits<double>::max();

	M1_value  = std::numeric_limits<double>::max();
	M2_value  = std::numeric_limits<double>::max();
	M3_value  = std::numeric_limits<double>::max();
	M4_value  = std::numeric_limits<double>::max();
	M5_value  = std::numeric_limits<double>::max();
	M6_value  = std::numeric_limits<double>::max();
	M7_value  = std::numeric_limits<double>::max();
	M8_value  = std::numeric_limits<double>::max();
	M9_value  = std::numeric_limits<double>::max();
	M10_value = std::numeric_limits<double>::max();
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

double Segment::m(int p, int q)
{
	double m = 0.0;

	for (auto& point : mPoints)
	{
		m += pow(point.x, p) * pow(point.y, q);
	}

	return m;
}

double Segment::M(int p, int q)
{
	double M = 0.0;

	for (auto& point : mPoints)
	{
		M += pow(point.x - mXCenter, p) * pow(point.y - mYCenter, q);
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
	return isInRange(M1(), 0.33f, 0.44f) && isInRange(M2(), 0.0034f , 0.1f) && isInRange(M3(), 0.00001f, 0.00031f) && isInRange(M6(), -0.0001f, 0.00003f) && isInRange(M7(), 0.022f, 0.033f);
}
bool Segment::isOne()
{
	return 
		isInRange(M1(), 0.38f, 0.67f) &&
		isInRange(M2(), 0.1f, 0.4f) &&
		isInRange(M3(), 0.007f, 0.042f) &&
		isInRange(M4(), 0.0029f, 0.022f) &&
		isInRange(M6(), 0.0009f, 0.014f) &&
		isInRange(M7(), 0.01f, 0.05f) &&
		isInRange(M8(), -0.003f, -0.0005f) &&
		isInRange(M9(), 0.00028f, 0.0021f);
}

//isInRange(M1(), 0.44f, 0.67f) &&
//isInRange(M2(), 0.1f, 0.4f) &&
//isInRange(M3(), 0.01f, 0.042f) &&
//isInRange(M4(), 0.003f, 0.022f) &&
//isInRange(M6(), 0.001f, 0.014f) &&
//isInRange(M7(), 0.01f, 0.05f) &&
//isInRange(M8(), -0.003f, -0.0009f) &&
//isInRange(M9(), 0.00028f, 0.0021f);

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
	return isInRange(M1(), 0.35f, 0.39f) && isInRange(M2(), 0.0032f, 0.011f) && isInRange(M3(), 0.001f, 0.0017f);
}

double Segment::M1()
{
	if (!mGeometricMomentsUpToDate)
	{
		M1_value = (M_20 + M_02) / pow(m_00, 2);
	}
	return M1_value;
}

double Segment::M2()
{
	if (!mGeometricMomentsUpToDate)
	{
		M2_value = (pow(M_20 - M_02, 2) + 4.0 * pow(M_11, 2)) / pow(m_00, 4);
	}
	
	return M2_value;
}

double Segment::M3()
{
	if (!mGeometricMomentsUpToDate)
	{
		M3_value = (pow(M_30 - 3.0 * M_12, 2) + pow(3.0 * M_21 - M_03, 2)) / pow(m_00, 5);
	}
	return M3_value;
}

double Segment::M4()
{
	if (!mGeometricMomentsUpToDate)
	{
		M4_value = (pow(M_30 + M_12, 2) + pow(M_21 + M_03, 2)) / pow(m_00, 5);
	}
	return M4_value;
}

double Segment::M5()
{
	if (!mGeometricMomentsUpToDate)
	{
		M5_value = ((M_30 - 3.0 * M_12) * (M_30 + M_12) * (pow(M_30 + M_12, 2) - 3.0 * pow(M_21 + M_03, 2)) + (3.0 * M_21 - M_03) * (M_21 + M_03) * (3.0 * pow(M_30 + M_12, 2) - pow(M_21 + M_03, 2))) / pow(m_00, 10);
	}
	return M5_value;
}

double Segment::M6()
{
	if (!mGeometricMomentsUpToDate)
	{
		M6_value = ((M_20 - M_02) * (pow(M_30 + M_12, 2) - pow(M_21 + M_03, 2)) + 4.0 * M_11 * (M_30 + M_12) * (M_21 + M_03)) / pow(m_00, 7);
	}
	return M6_value;
}

double Segment::M7()
{
	if (!mGeometricMomentsUpToDate)
	{
		M7_value = (M_20 * M_02 - pow(M_11, 2)) / pow(m_00, 4);
	}
	return M7_value;
}

double Segment::M8()
{
	if (!mGeometricMomentsUpToDate)
	{
		M8_value = (M_30 * M_12 + M_21 * M_03 - pow(M_12, 2) - pow(M_21, 2)) / pow(m_00, 5);
	}
	return M8_value;
}

double Segment::M9()
{
	if (!mGeometricMomentsUpToDate)
	{
		M9_value = (M_20 * (M_21 * M_03 - pow(M_12, 2)) + M_02 * (M_03 * M_12 - pow(M_21, 2)) - M_11 * (M_30 * M_03 - M_21 * M_12)) / pow(m_00, 7);
	}
	return M9_value;
}

double Segment::M10()
{
	if (!mGeometricMomentsUpToDate)
	{
		M10_value = (pow(M_30 * M_03 - M_12 * M_21, 2) - 4.0 * (M_30 * M_12 - pow(M_21, 2)) * (M_03 * M_21 - M_12)) / pow(m_00, 10);
	}
	return M10_value;
}

bool Segment::isInRange(double value, double low, double high)
{
	return low <= value && value <= high;
}