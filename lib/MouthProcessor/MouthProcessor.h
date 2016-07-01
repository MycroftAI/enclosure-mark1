#ifndef MOUTH_PROCESSOR_H
#define MOUTH_PROCESSOR_H

#include <BaseProcessor.h>
#include <MycroftMouth.h>


class MouthProcessor : public BaseProcessor
{
public:
    MouthProcessor(MycroftMouth &mouth);

private:
    void process(String cmd);

    MycroftMouth &mouth;
};

#endif /* MOUTH_PROCESSOR_H */
