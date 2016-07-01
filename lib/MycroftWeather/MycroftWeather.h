#include "MycroftEyes.h"
#include "MycroftMouth.h"
class MycroftWeather
{

public:
  MycroftWeather(MycroftMouth *mouth, MycroftEyes *eyes);

  void display(int8_t condition, int8_t current);

private:
  MycroftMouth *mouth;

  MycroftEyes *eyes;


};
