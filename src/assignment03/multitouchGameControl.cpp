#include "multitouchGameControl.hpp"

MultitouchGameControl::MultitouchGameControl(Game& game, HomograpyCalibrator& calibrator) : m_game(game), m_calibrator(calibrator)
{
}

void MultitouchGameControl::handleInput(std::vector<cv::RotatedRect> positions)
{
    /*~~~~~~~~~~~*
     * YOUR CODE *
     * GOES HERE *
     *~~~~~~~~~~~*/

    // Use m_calibrator to access and modify the m_game's player's pedals.
    // How you do it is your call, but you should probably use active(), setActive(), position() and setTargetPosition().
	for (auto rect : positions) {
		cv::Point2f pos = m_calibrator.cameraToGame(rect.center);
		std::cout << "TouchPoint:" << pos << std::endl;
		if (pos.x <= HALF_FIELD_WIDTH) {
			processClickPlayer(pos, m_game.players().at(0));
		}
		else {
			processClickPlayer(pos, m_game.players().at(1));
		}
	}
}

void MultitouchGameControl::processClickPlayer(cv::Point2f pos, Player& player) {
	/*for (auto& paddle : player.paddles()) {
		if (paddle.active()) {
			paddle.setTargetPosition(pos.y);
			paddle.setActive(false);
		}
		if (paddle.boundingBox().contains(pos)) {
			paddle.setActive(true);
			std::cout << "HIT PADDLE" << std::endl;
		}
	}*/
	for (auto& paddle : player.paddles()) {
		auto rect = paddle.boundingBox();
		rect.width += 20;
		rect.height += 20;
		if (rect.contains(pos)) {
			paddle.setActive(true);
			std::cout << "HIT PADDLE" << std::endl;
			return;
		}
	}
	for (auto& paddle : player.paddles()) {
		if (paddle.active()) {
			paddle.setTargetPosition(pos.y);
			paddle.setActive(false);
		}
	}
}