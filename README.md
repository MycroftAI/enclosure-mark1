# Mycroft Enclosure
![mycroft-front]

This repository holds the code run on the Arduino within a Mycroft unit. It manages the eyes, the mouth, and the button. The code is written entirely in C++ with Arduino's standard library.

# Getting Started in Linux
### Compiling
```
sudo ./install-arduino.sh
./compile.sh
```
**Explanation:**
To compile the application you need to ensure Arduino IDE is installed to `/opt/arduino`. To do this automatically you can run the included script with `sudo ./install-arduino.sh`. If you have Arduino IDE installed in a different location you can set that in `compile.sh`. Finally, to compile run `./compile.sh`.

### Uploading to board
First download this repository to the unit in the folder `/opt/enclosure` provided it doesn't already exists. Next copy the generated file `build/enclosure.ino.hex` to the Mycroft unit in the folder `/opt/enclosure/`. This can be done quickly if the unit is connected to your network with the following script: `./deploy.sh pi@192.168.X.X` with the correct IP instead of `192.168.X.X`.

Finally, on the Mycroft Unit run the following:
```
sudo ./install-avrdude.sh
./upload.sh
```

This will write the generated hex file to the Arduino. After running through the setup steps all that is necessary to compile and upload the hex to the arduino is the following:
```
# From host machine:
./deploy.sh pi@192.168.X.X

# From Mycroft unit:
./upload.sh
```

[mycroft-front]:http://i.imgbox.com/AvlCUgie.png
[arduino-ide]:https://www.arduino.cc/en/Main/Software

# Version number location
enclosure/lib/MycroftArduino/MycroftArduino.h

# Serial Port Protocols

See the file [protocols.txt](../master/protocols.txt) for a description of commands that can be sent to the faceplate.

Commands can be sent from the command line on a Raspberry Pi, such as this:
```
$ echo "eyes.blink" > /dev/ttyAMA0
$ echo "eyes.color=16711680" > /dev/ttyAMA0
$ echo "mouth.text=I am angry!" > /dev/ttyAMA0
```
Which will blink the eyes, turn them red, then display the phrase on the faceplate.

# Mycroft Skill API Interface
Skills are written in Python.  Access to the Mark 1 features can be had using the mycroft.client.enclosure.api,
which is configured and available inside a skill using `self.enclosure`.  For example:
```
self.enclosure.mouth_text("I am angry!")
```

# Graphics
The `mouth.icon=` command takes a custom format for it's black and white images.  You can use the [HTML editor](http://htmlpreview.github.io/?https://raw.githubusercontent.com/MycroftAI/enclosure-mark1/master/editor.html) to create the image strings that can be sent.  Within Mycroft you can send PNG files using `self.enclosure.display_png()` and they will be automatically converted to the correct format for you.
