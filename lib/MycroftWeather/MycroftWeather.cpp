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
#include "MycroftWeather.h"
#include "MycroftMouth.h"
#include "MouthImages.h"

MycroftWeather::MycroftWeather()
{
}

void MycroftWeather::calculateTemperaturePosition(String temperature) {
	char first = temperature.charAt(0);
	if (temperature.length() == 1) {
		tempPos = 18;
	} else if (temperature.length() == 2) {
		tempPos = 16;
	} else if (temperature.length() == 3) {
		if (first == '-') {
			tempPos = 13;
		} else {
			tempPos = 14;
		}
	}
}

void MycroftWeather::display(const String& temperature, const String& icon) {
	MycroftMouth&	mouth = MycroftMouth::instance();

	calculateTemperaturePosition(temperature);
	mouth.reset();
        mouth.showIcon(icon.c_str());
        // "\\" is mapped as the degree symbol
	mouth.staticText(temperature+"\\", tempPos, 1);
	mouth.ht1632.render();
}
