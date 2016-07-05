#ifndef SYSTEM_PROCESSOR_H
#define SYSTEM_PROCESSOR_H

#include <BaseProcessor.h>
#include <MycroftSystem.h>

class SystemProcessor : public BaseProcessor
{
public:
    SystemProcessor(uint8_t speakerPin);
    void setup();

private:
    void process(String cmd);

    MycroftSystem system;
};

#endif /* SYSTEM_PROCESSOR_H */
