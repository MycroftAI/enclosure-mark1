/**                                                                         
#Copyright 2017, Mycroft AI, Inc.
#
#Licensed under the Apache License, Version 2.0 (the "License");
#you may not use this file except in compliance with the License.
#You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
#Unless required by applicable law or agreed to in writing, software
#distributed under the License is distributed on an "AS IS" BASIS,
#WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#See the License for the specific language governing permissions and
#limitations under the License.
**/
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
