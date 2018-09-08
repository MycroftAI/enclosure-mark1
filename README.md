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

# IO pins
When looking at the device from the back, pins are laid out as:

```
             -----------------------------------------------------------------------------------------------
RCA Port    | 2 | 4 | 6 | 8 | 10 | 12 | 14 | 16 | 18 | 20 | 22 | 24 | 26 | 28 | 30 | 32 | 34 | 36 | 38 | 40 |
            | 1 | 3 | 5 | 7 |  9 | 11 | 13 | 15 | 17 | 19 | 21 | 23 | 25 | 27 | 29 | 31 | 33 | 35 | 37 | 39 |
             -----------------------------------------------------------------------------------------------
             
 HDMI                         Ethernet                        USB     USB
```

Where:

| Pin| Description          |
|:--:|:---------------------|
| 1  | GND                  |
| 2  | +12V                 |
| 3  | GND                  |
| 4  | +5V                  |
| 5  | GND                  |
| 6  | +3.3V                |
| 7  | Arduino Reset        |
| 8  | Arduino D5           |
| 9  | Arduino D6           |
| 10 | Arduino D10          |
| 11 | Arduino A2           |
| 12 | Arduino A3           |
| 13 | +3.3V                |
| 14 | +3.3V                |
| 15 | +3.3V                |
| 16 | GND                  |
| 17 | GND                  |
| 18 | GND                  |
| 19 | +5V                  |
| 20 | +5V                  |
| 21 | +5V                  |
| 22 | +5V                  |
| 23 | GND                  |
| 24 | GND                  |
| 25 | Raspberry Pi ID_SC   |
| 26 | Raspberry Pi ID_SD   |
| 27 | Raspberry Pi GPIO 4  |
| 28 | Raspberry Pi GPIO 5  |
| 29 | Raspberry Pi GPIO 6  |
| 30 | Raspberry Pi GPIO 7  |
| 31 | Raspberry Pi GPIO 8  |
| 32 | Raspberry Pi GPIO 9  |
| 33 | Raspberry Pi GPIO 10 |
| 34 | Raspberry Pi GPIO 11 |
| 35 | Raspberry Pi GPIO 12 |
| 36 | Raspberry Pi GPIO 16 |
| 37 | Raspberry Pi GPIO 25 |
| 38 | Raspberry Pi GPIO 26 |
| 39 | GND                  |
| 40 | GND                  |

**WARNING: This is not the same as the standard Raspberry Pi GPIO headers!**
  
