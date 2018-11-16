#pragma once

#include <vector>
#include <opencv2/opencv.hpp>

class TouchRecognizer
{
public:
    void calibrate(const cv::Mat & background);
    std::vector<cv::RotatedRect> recognize(const cv::Mat & depthFrame) const;
private:
    cv::Mat m_background;
    /*~~~~~~~~~~~~~~~~*
     * YOU MAY DEFINE *
     * CONSTANTS HERE *
     *~~~~~~~~~~~~~~~~*/
	bool m_calibrated = false;
	cv::uint16_t shift = UINT16_MAX / 2;
	bool calibrated = false;
};
