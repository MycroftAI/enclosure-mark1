#pragma once

class ClickEncoder;

class MycroftEncoder {
public:

	enum class Direction {
		LEFT, RIGHT, NONE
	};
	ClickEncoder* clickEncoder;
	MycroftEncoder(uint8_t pinEncoderOne, uint8_t pinEncoderTwo, uint8_t pinButton);
	Direction getDirection();
	int getFramesHeld();
	bool isClicked();
	void isr();

private:
	const byte PIN_BUTTON;
	int16_t last, value;
	int framesHeld;
	bool clicked;
	Direction direction;
};
