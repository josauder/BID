#pragma once

#include <opencv2/opencv.hpp>

class TouchVisualizer
{
public:
    void draw(cv::Mat & frame, const cv::RotatedRect & position, cv::Mat & output);
	void refresh(cv::Mat & output);
	void reset();

private:
	cv::Mat canvas;
	cv::Mat mask;
	bool initialized = false;
};