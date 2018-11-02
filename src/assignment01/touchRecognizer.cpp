#include "touchRecognizer.hpp"

#include <algorithm>

void TouchRecognizer::calibrate(const cv::Mat& background)
{
    /*~~~~~~~~~~~*
     * YOUR CODE *
     * GOES HERE *
     *~~~~~~~~~~~*/
	m_background = background.clone();
}

std::vector<cv::RotatedRect> TouchRecognizer::recognize(const cv::Mat& depthFrame) const
{
    std::vector<cv::RotatedRect> positions;

	cv::Mat difference;
	cv::Mat thresholdHigh;
	cv::Mat thresholdLow;
	cv::Mat blur;
	cv::Mat lowPassFiltered;

	std::vector<std::vector<cv::Point>> contours;

	cv::absdiff(depthFrame, m_background, difference);
	cv::threshold(difference, thresholdHigh, 600, UINT16_MAX, 4);
	cv::threshold(thresholdHigh, thresholdLow, 300, UINT16_MAX, 0);
	cv::blur(thresholdLow, blur, cv::Size(15,15));
	blur.convertTo(blur, CV_8UC1);
	cv::threshold(blur, lowPassFiltered, 254, UINT8_MAX, 0);

	cv::findContours(lowPassFiltered, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	for (std::vector<cv::Point> contour : contours) {
		if (cv::contourArea(contour) < 1600)
			continue;
		cv::RotatedRect rect = cv::fitEllipse(contour);
		positions.push_back(rect);
		std::cout << "detected foot press at: x" << rect.center.x << "y" << rect.center.y << std::endl;
		cv::circle(lowPassFiltered, rect.center, 20, UINT16_MAX);
	}
    /*~~~~~~~~~~~*
     * YOUR CODE *
     * GOES HERE *
     *~~~~~~~~~~~*/

    return positions;
}
