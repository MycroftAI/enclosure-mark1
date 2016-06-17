class MycroftArduino {

public:
    MycroftArduino(uint8_t speakerPin);

    void start();

    void reset();

    void mute();

    void unmute();

    void blink(long times, unsigned long wait);

private:
    uint8_t speakerPin;

};
