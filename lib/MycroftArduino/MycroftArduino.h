class MycroftArduino {

public:
    MycroftArduino(uint8_t buttonPin, uint8_t speakerPin);

    void start();

    void reset();

    void mute();

    void unmute();

    bool isButtonPress();

    void blink(long times, unsigned long wait);

private:
    uint8_t buttonPin;

    uint8_t speakerPin;

};