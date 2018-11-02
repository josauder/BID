#include "touchVisualizer.hpp"

void TouchVisualizer::draw(cv::Mat & frame, const cv::RotatedRect & position, cv::Mat & output)
{
    /*~~~~~~~~~~~*
     * YOUR CODE *
     * GOES HERE *
     *~~~~~~~~~~~*/
	if (!initialized) {
		canvas = cv::Mat(output.rows, output.cols, CV_8UC3, cv::Scalar(0, 0, 0));
		mask = cv::Mat(output.rows, output.cols, CV_8UC3, cv::Scalar(1,1,1));
		initialized = true;
	}
	cv::circle(frame, position.center, 20, cv::Scalar(255,0,0));
	cv::ellipse(canvas, position.center, cv::Size(20, 20), 0, 0, 360, cv::Scalar(255, 0, 0), -1);
	cv::ellipse(mask, position.center, cv::Size(20, 20), 0, 0, 360, cv::Scalar(0,0,0), -1);
	cv::multiply(mask, output, output);
	cv::add(output, canvas, output);
}

void TouchVisualizer::refresh(cv::Mat & output) {
	if(!initialized) {
		canvas = cv::Mat(output.rows, output.cols, CV_8UC3, cv::Scalar(0, 0, 0));
		mask = cv::Mat(output.rows, output.cols, CV_8UC3, cv::Scalar(1, 1, 1));
		initialized = true;
	}
	cv::multiply(mask, output, output);
	cv::add(output, canvas, output);
}

void TouchVisualizer::reset() {
	if (initialized) {
		canvas = cv::Scalar(0, 0, 0);
		mask = cv::Scalar(1, 1, 1);
		initialized = true;
	}
}
