#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "SegmentMap.h"
#include "Converter.h"
#include "../helpers/IOHelper.h"
#include <stack>

class ImageProcessor
{
public:
	static void colorReduce(cv::Mat source, cv::Mat& destination, int div = 64);
	static void threshold(cv::Mat source, cv::Mat1b& destination);
	static void generateSegmentMap(cv::Mat3b source, SegmentMap& destination, cv::Mat1b mask);
	static void rankFilter(cv::Mat source, cv::Mat& destination, int N, int index);
	static void dilate(cv::Mat1b source, cv::Mat1b& destination);
	static void erode(cv::Mat1b source, cv::Mat1b& destination);
	static std::vector<Segment> process(cv::Mat3b image);
private: 
	static int imageId;
	static void drawBoundingBoxes(cv::Mat3b source, std::vector<Segment> segments);
	static void morphologyFilter(cv::Mat1b source, cv::Mat1b& destination, bool dilate, cv::Mat1b kernel);
	static bool isInRange(int value, int low, int high);
	static void floodFill(cv::Mat3b source, SegmentMap& destination, cv::Mat1b& mask,
						  cv::Vec3b color, cv::Point2i seed, int lowThreshold = 70,
						  int highThreshold = 50);
};
