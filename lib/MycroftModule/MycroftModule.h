#ifndef MYCROFT_MODULE_H
#define MYCROFT_MODULE_H

class MycroftModule {
public:
    typedef void (*DelayFunc)(int);
    static void setDelayFunc(DelayFunc func);

protected:
    static DelayFunc delay;
};

#endif
