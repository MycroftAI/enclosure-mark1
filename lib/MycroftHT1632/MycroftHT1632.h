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

#include "HT1632.h"
#include <avr/pgmspace.h>
#include <Arduino.h>

class MycroftHT1632 : public HT1632Class {
public:
	void drawImagePgm(const char * img, char width, char height, char x, char y, int offset = 0);
	void drawTextPgm(const char [], int x, int y, const char font [], const char font_width [], char font_height, int font_glyph_step, char gutter_space = 1);
};
