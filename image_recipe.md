# Recipe for creating the Mark 1 Image

## Install and Configure Base Raspbian Image

* Burn jessie to SD card.  Use:<br/>
  [2016-09-23-raspbian-jessie-lite.img](http://downloads.raspberrypi.org/raspbian_lite/images/raspbian_lite-2016-09-28/)
* Boot Raspberry Pi and login as pi/raspberry
* <code>sudo passwd pi</code> and change to 'mycroft'
* <code>sudo raspi-config</code> then select<br/>
	* '1 Expand Filesystem'
	* '3 Boot Options'
		*   --> 'B2 Console Autologin'
	* '4 Wait for network at boot'
		*   --> 'No'
	* '5 Internationalization Options'
		*  --> 'I3 Change Keyboard Layout'
		*  --> 'Generic 101-key PC'
		*  --> 'Other'
		*  --> 'English (US)'
		*  --> 'English (US)'
		*  --> 'The default for the keyboard layout'
		*  --> 'No compose key' 
	* '9 Advanced Options'
		*  --> 'A2 Hostname'
		*  --> Enter:  'mark_1'
	* Finish and allow the reboot

## Connect to Mycroft repos and pull down current image

* Plug in network cable (if not already)
* (Optional) Connect via SSH so you can copy/paste all of the following.
* <code>sudo apt-get update</code>
* <code>sudo apt-get install apt-transport-https -y</code>
* <code>sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys F3B1AA8B</code>
* <code>sudo bash -c 'echo "deb http://repo.mycroft.ai/repos/apt/debian debian main" > /etc/apt/sources.list.d/repo.mycroft.ai.list'</code>
* <code>sudo apt-get update</code>
* <code>sudo apt-get install mycroft-core -y</code>

## Install vim (for Arron's general health and well being)
<code>sudo apt-get install vim -y</code>

## Install ufw for a simple firewall setup
<code>sudo apt-get install ufw -y</code>

## Set 'll' alias (for Steve's general health and well being)
<code>nano .bashrc</code> and uncomment 'alias ll='ls -l' (about 25 lines from the bottom)

## Add initial config for platform detection
* <code>sudo mkdir /etc/mycroft</code>
* <code>sudo nano /etc/mycroft/mycroft.conf</code> and add the following:
```json
{
    "enclosure": {
        "platform": "mycroft_mark_1",
        "platform_build": 3,
        "port": "/dev/ttyAMA0",
        "rate": 9600,
        "timeout": 5.0,
        "update": true,
        "test": false
    },
    "VolumeSkill": {
    "default_level": 6,
    "min_volume": 0,
    "max_volume": 83
    }
}
```
Platform build should be updated with each new image.

## Set default sample rate for better sound quality
<code>sudo nano /etc/pulse/daemon.conf</code>  and uncomment (9 lines from the bottom)
```
 default-sample-rate = 44100
```

## Disable kernel boot TTY on GPIO UART
* <code>sudo systemctl stop serial-getty@ttyAMA0.service</code>
* <code>sudo systemctl disable serial-getty@ttyAMA0.service</code>

## Enable systemd-timesyncd
* <code>sudo timedatectl set-ntp true</code>

## Edit boot configuration settings
<code>sudo nano /boot/config.txt</code>
* Uncomment all of these to enable the optional hardware interfaces (about 10 lines from bottom)
	* dtparam=i2c_arm=on
	* dtparam=i2s=on
	* dtparam=spi=on
* Comment out the following to disable audio (loads snd_bcm2835)
	```
    #dtparam=audio=on
    ```
* Add the following lines at the bottom:
	```
    # Disable Bluetooth, it interferes with serial port
	dtoverlay=pi3-disable-bt
	dtoverlay=pi3-miniuart-bt
	# Enable Mark 1 soundcard drivers
	dtoverlay=rpi-proto
	```

* <code>sudo nano /boot/cmdline.txt</code>
	* Make sure it contains no mention of ttyAMA0, as this is where the boot logging to serial would be enabled in the past.
	* Delete this option: 'console=serial0,115200'

* Reboot
<code>sudo reboot now</code>

## Setup soundcard
Set correct volume and sound card settings
* <code>alsamixer</code>
* Press F6 and select 'snd_rpi_proto'
* Press F5 to show all settings
* Adjust settings to match as shown below.  User right-arrow to change selection, Up to change numbers, 'M' to change between options
* Raise 'Master' volume to 46 with up arrow presses
* Press right and enable 'Master Playback' with press of 'M'
* Press right three times and enable 'Mic' 'Capture' with press of spacebar
* Press right three times and enable 'Playback Deemp' with press of 'M'
* Press right three times and set 'Input Mux' to 'Mic' with press of up arrow
* Press right and enable 'Output Mixer HiFi' with press of 'M'
* Press right three times and enable 'Store DC Offset' with press of 'M'
* ESC to save and exit
  

## Enable ufw for a simple firewall allowing only port 22 incoming as well as dns, dhcp, and the Mycroft web socket
Block all incoming by default
* <code>sudo ufw default deny incoming</code>
 
Allow ssh on port 22 when enabled
* <code>sudo ufw allow 22</code>
 
WiFi setup client:
Allow tcp connection to websocket
* <code>sudo ufw allow in from 172.24.1.0/24 to any port 8181 proto tcp</code>
 
Allow tcp to web server
* <code>sudo ufw allow in from 172.24.1.0/24 to any port 80 proto tcp</code>
 
Allow udp for dns
* <code>sudo ufw allow in from 172.24.1.0/24 to any port 53 proto udp</code>
 
Allow udp for dhcp
* <code>sudo ufw allow in from 0.0.0.0 port 68 to 255.255.255.255 port 67 proto udp</code>

Turn on the firewall
* <code>sudo ufw enable</code>

## Create RAM disk for IPC

<code>sudo mkdir /ramdisk</code>

edit <code>/etc/fstab</code> and add the following line:

<code>tmpfs           /ramdisk        tmpfs   rw,nodev,nosuid,size=20M          0  0</code>

Edit <code>/etc/mycroft/mycroft.conf</code> and add the line

<code>   "ipc_path": "/ramdisk/mycroft/ipc/"</code>

at the end. (remember the comma on the line before)

## Upgrade packages

Upgrade all packages except raspberrypi-kernel

<code>sudo apt-get update</code>

<code>echo "raspberrypi-kernel hold" | sudo dpkg --set-selections</code>

<code>sudo apt-get upgrade</code>

## Setup packagekit
Install the packagekit package.

<code>apt-get install packagekit</code>

Set a policy allowing the mycroft user to install packages using this by creating the file /etc/polkit-1/localauthority/50-local.d/allow_mycroft_to_install_package.pkla

and enter the following
```
 [Allow mycroft to install packages using packagekit]
 
 Identity=unix-user:mycroft
 
 Action=org.freedesktop.packagekit.package-eula-accept;org.freedesktop.packagekit.package-install
 ResultAny=yes
```

## Allow mycroft user to install with pip

Pip doesn't interact with policy kit so a sudoer rule is needed to allow <code>pip install</code> from mycroft without password. Edit /etc/sudoers.d/011_mycroft-nopasswd and add:

<code>mycroft ALL=(ALL) NOPASSWD: /usr/local/bin/pip install *</code>

Afterwards remove write flag of the file 

<code>chmod -w /etc/sudoers.d/011_mycroft-nopasswd</code>

## Install librespot

<code>curl -sL https://dtcooper.github.io/raspotify/install.sh | sh</code>

Then disable the raspotify service:

<code>sudo systemctl stop raspotify</code>
<code>sudo systemctl disable raspotify</code>

The spotify connect client can now be started from a skill by executing <code>librespot --user USER --password PASS --name Mycroft</code>

## Clear the Bash History

* <code>history -c</code>
* <code>history -w</code>
