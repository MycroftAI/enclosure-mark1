#pragma once

class ClickEncoder;

class MycroftEncoder {
public:
	static MycroftEncoder& instance() { return *m_instance; }

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
        static MycroftEncoder* m_instance;

	const byte PIN_BUTTON;
	int16_t last, value;
	int framesHeld;
	bool clicked;
	Direction direction;
};
