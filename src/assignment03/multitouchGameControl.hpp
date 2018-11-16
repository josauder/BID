#pragma once

#include <vector>
#include <opencv2/opencv.hpp>
#include "game.hpp"

class MultitouchGameControl
{
public:
	MultitouchGameControl(Game& game, HomograpyCalibrator& calibrator);
	void handleInput(std::vector<cv::RotatedRect> positions);
protected:
	Game& m_game;
	HomograpyCalibrator& m_calibrator;

	void processClickPlayer(cv::Point2f pos, Player& player);

	const int HALF_FIELD_WIDTH = 420;
		/*~~~~~~~~~~~~~~~~*
		 * YOU MAY DEFINE *
		 * VARIABLES HERE *
		 *~~~~~~~~~~~~~~~~*/
};
