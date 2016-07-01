#ifndef SYSTEM_PROCESSOR_H
#define SYSTEM_PROCESSOR_H

#include <BaseProcessor.h>
#include <MycroftArduino.h>

class SystemProcessor : public BaseProcessor
{
public:
    SystemProcessor(MycroftArduino &arduino);

private:
    void process(String cmd);

    MycroftArduino &arduino;
};

#endif /* SYSTEM_PROCESSOR_H */
