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

	void isr();


private:
	int16_t last, value;

	Direction direction;
};
