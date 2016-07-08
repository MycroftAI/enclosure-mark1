#include "MycroftEyes.h"
#include "MycroftMouth.h"
class MycroftWeather
{

public:
  MycroftWeather(MycroftMouth *mouth, MycroftEyes *eyes);

  void display(int8_t condition, String current);

private:
  MycroftMouth *mouth;

  MycroftEyes *eyes;

  char imgPos, tempPos;
};
