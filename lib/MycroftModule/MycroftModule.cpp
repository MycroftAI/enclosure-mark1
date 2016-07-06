#include "MycroftModule.h"

MycroftModule::DelayFunc MycroftModule::delay;

void MycroftModule::setDelayFunc(DelayFunc func) {
    delay = func;
}
