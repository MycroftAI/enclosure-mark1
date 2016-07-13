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
