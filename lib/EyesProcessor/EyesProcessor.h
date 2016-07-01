#ifndef EYE_PROCESSOR_H
#define EYE_PROCESSOR_H

#include <BaseProcessor.h>
#include <MycroftEyes.h>


class EyesProcessor : public BaseProcessor
{
public:
    EyesProcessor(MycroftEyes &eyes);

private:
    void updateEyesColor(long code);
    void process(String cmd);

    MycroftEyes &eyes;
};

#endif /* EYE_PROCESSOR_H */
