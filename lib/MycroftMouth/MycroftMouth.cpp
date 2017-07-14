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
#include "MycroftMouth.h"
#include "MouthImages.h"
#include "font_5x4.h"
#include "font_8x4.h"

// The "mouth" consists of 4 8x8 pixel plates
// #define NUM_PLATES	4
#define PLATE_WIDTH	8
#define PLATE_HEIGHT	8

MycroftMouth::MycroftMouth(int pinCS1, int pinWR, int pinDATA, int plates) {
	ht1632 = MycroftHT1632();
	ht1632.begin(pinCS1, pinWR, pinDATA);
	reset();
	this->plates = plates;
	lastState = state = NONE;
}

void MycroftMouth::setPanel(int8_t pos, const char (&IMG)[16]) {
	ht1632.drawImage(IMG, PLATE_WIDTH, PLATE_HEIGHT, pos, 0);
}

void MycroftMouth::render() {
	ht1632.render();
}

void MycroftMouth::staticText(const String& text, int8_t pos, int8_t fontIndex) {
	if (fontIndex == 0) {
		ht1632.drawTextPgm(text.c_str(), pos, 0, FONT_5X4, FONT_5X4_WIDTH, FONT_5X4_HEIGHT, FONT_5X4_STEP_GLYPH);
	} else if (fontIndex == 1) {
		ht1632.drawTextPgm(text.c_str(), pos, 0, FONT_8X4, FONT_8X4_WIDTH, FONT_8X4_HEIGHT, FONT_8X4_STEP_GLYPH);
	}
}

void MycroftMouth::reset() {
	state = NONE;
	textWd = 0;
	textIdx = 0;
	ht1632.clear();
	ht1632.render();
}

void MycroftMouth::update() {
	switch (state) {
		case TALK:
			this->talk();	// animates
			break;
		case LISTEN:
			this->listen();	// animates
			break;
		case THINK:
			this->think();	// animates
			break;
		case TEXT:
			this->updateText();	// scrolls
			break;
		case VISEME:
		case ICON:
			// no animation, but we don't want to reset state
			break;
		default:
			if (lastState != NONE) {
				this->reset();
			}
	}
	lastState = state;
}

void MycroftMouth::talk() {
	state = TALK;
	drawFrame(0, state);
}

#define SIZE_ANIM_FAKETALK	4
void MycroftMouth::fakeTalk() {
	if (state != TALK) {
		resetCounters(TALK);
		total = (SIZE_ANIM_FAKETALK * 2) - 2;
	}
	if (millis() > nextTime) {
		drawFrame(i, state);
		if (i < SIZE_ANIM_FAKETALK - 1) {
			i++;
		} else {
			i--;
		}
		nextTime = millis() + 70;
		total--;
	}
	if (total == 0) {
		resetCounters(TALK);
		total = (SIZE_ANIM_FAKETALK * 2) - 2;
	}
}

#define SIZE_ANIM_LISTEN	6
void MycroftMouth::listen() {
	if (state != LISTEN) {
		resetCounters(LISTEN);
	}
	if (millis() > nextTime) {
		drawFrame(i, state);
		if (i < (SIZE_ANIM_LISTEN - 1)) {
			i++;
		} else {
			i = 0;
		}
		nextTime = millis() + 70;
	}
}

#define SIZE_ANIM_THINK		7
void MycroftMouth::think() {
	if (state != THINK) {
		resetCounters(THINK);
		total = (SIZE_ANIM_THINK * 2);
	}
	if (millis() > nextTime) {
		// This animation plays in the following order:
		// 0,1,2,3,4,5,6,6,5,4,3,2,1,0...
		if (i >= SIZE_ANIM_THINK)
			drawFrame(i-((i-SIZE_ANIM_THINK+1)*2-1), state);
		else
			drawFrame(i, state);
		i++;
		nextTime = millis() + 120;
		total--;
	}
	if (total == 0) {
		resetCounters(THINK);
		total = (SIZE_ANIM_THINK*2);
	}
}

void MycroftMouth::drawFrame(byte i, State anim) {
	ht1632.clear();
	for (byte j = 0; j < this->plates; j++) {
		byte idx = (i * this->plates) + j;
		byte x = j * 8;
		readBufferState(idx, anim);
		ht1632.drawImage(buffer, PLATE_WIDTH, PLATE_HEIGHT, x, 0);
	}
	ht1632.render();
}

void MycroftMouth::readBufferState(byte idx, State anim) {
	if (anim == THINK) {
		this->readBuffer(idx, THINK_ANIMATION);
	}
	else if (anim == LISTEN) {
		this->readBuffer(idx, LISTEN_ANIMATION);
	}
	else if (anim == TALK) {
		this->readBuffer(idx, TALK_ANIMATION);
	}
	else if (anim == VISEME) {
		this->readBuffer(idx, MOUTH_VISEMES);
	}
}

void MycroftMouth::showIcon(const String& icon) {
	byte 	xOfs = 0;
	byte	yOfs = 0;
	byte	c = 0;
	if (icon[c] == 'x' && icon[c+1] == '=')
	{
		// parse the xOfs
		c += 2;
		while (icon[c] != ',' && c < icon.length())
		{
			xOfs = xOfs*10 + icon[c]-'0';	// assumes '0'-'9'
			c++;
		}
		c++;
	}
	if (icon[c] == 'y' && icon[c+1] == '=')
	{
                // parse the yOfs
                c += 2; 
                while (icon[c] != ',' && c < icon.length())
                {
                        yOfs = yOfs*10 + icon[c]-'0';	// assumes '0'-'9'
                        c++;
                }
                c++;
        }

	// icon is an encoded string.  The encoding includes 
	// two leading characters to indicate width and height
	// of the icon which follows.  Subsequent characters
	// are encodings of the 4-bit blocks.  The data is in
	// column order from the top-down.
	// Chars = ABCDEFGHIJKLMNOP, represent 0b0000 to 0b1111
	// The low bit is the first pixel, running top to bottom
	if (c+2 > (int)icon.length())
		return;

        // NOTE: For some reason no string longer than 48 characters is
        // coming through.

//	String	strY(icon.length());
//        String  str("W=");
//        str += strY;
//        write(str.c_str());
//        return;

	byte	w = icon[c++]-'A';	// this encoding works well up to 65
	byte 	h = icon[c++]-'A';


	if (icon.length()-c < (int)w*2)
		return;


	char	buf[2];
	ht1632.clear();
	for (; w && c < icon.length(); c++)
	{
		if (icon[c] < 'A')
			continue;

		if (c % 2)
		{
			buf[1] = icon[c]-'A';
			ht1632.drawImage(buf, 1, h, xOfs++, yOfs);
			w--;
		}
		else
			buf[0] = icon[c]-'A';
	}

	state = ICON;
	ht1632.render();
}


void MycroftMouth::viseme(const String& vis) {
	if (state == TEXT || state == ICON)
	{
		// Don't show visemes over text or images on the display
		return;
	}

	state = VISEME;
	int iVis = int(vis[0])-int('0');
	if (iVis < 0)
		iVis = 0;
	if (iVis > 6)
		iVis = 6;
	drawFrame(iVis, VISEME);
}

void MycroftMouth::write(const char *value) {
	textBuf = value;
	textWd = HT1632.getTextWidth(textBuf.c_str(), FONT_5X4_WIDTH, FONT_5X4_HEIGHT);
	textIdx = 0;
	resetCounters(TEXT);
	this->updateText();
}

void MycroftMouth::updateText() {
	if (millis() > nextTime) {
		ht1632.transition(TRANSITION_BUFFER_SWAP);
		ht1632.clear();
		ht1632.drawTextPgm(textBuf.c_str(), OUT_SIZE - textIdx, 2, FONT_5X4, FONT_5X4_WIDTH, FONT_5X4_HEIGHT, FONT_5X4_STEP_GLYPH);
		ht1632.render();

		if (textWd > OUT_SIZE) {
			// scroll long text
			textIdx = (textIdx + 1) % (textWd + OUT_SIZE);
		}
		else {
			// center short text
			textIdx = OUT_SIZE - (OUT_SIZE - textWd) / 2;
		}
		nextTime = millis() + 150;
	}
}

void MycroftMouth::resetCounters(State anim) {
	state = anim;
	i = 0;
	nextTime = 0;
}

