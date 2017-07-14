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

#include "BaseProcessor.h"
#include "MycroftEyes.h"

class EyesProcessor : public BaseProcessor {
public:
	EyesProcessor();
	void setup();

private:
	void updateEyesColor(long code);
	void process(const String& cmd);
	MycroftEyes::Side toSide(const char SIDE_CHAR);
	bool checkEyeAnim(const String& cmd, const String& term, MycroftEyes::Animation anim);
};
