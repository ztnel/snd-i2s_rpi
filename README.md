# snd-i2s\_rpi

Driver for Adafruit's I2S MEMS Microphone

Installing the I2S microphone easy way
====================================

If you use Raspbian or any Debian-derived distribution, go to the releases tab, download the newest deb version.

Then do the following

```bash

# Installing raspberrypi-kernel-headers works only if you haven't messed with
# the rpi-update thing.
# If you did, then you would have to do the rpi-source method
# to get the kernel headers. See: 
# https://learn.adafruit.com/adafruit-i2s-mems-microphone-breakout/raspberry-pi-wiring-and-test#kernel-compiling

$ sudo apt install dkms raspberrypi-kernel-headers

$ sudo dpkg -i snd-i2s-rpi-dkms_0.0.2_all.deb

# For this to work, remember to modify these first:
# /boot/config.txt -> dtparam=i2s=on
# and /etc/modules -> snd-bcm2835

$ sudo modprobe snd-i2s_rpi rpi_platform_generation=0

# rpi_platform_generation=0 -> Raspberry Pi 1 B/A/A+, 0
# do not add anything -> everything else (2/3).

# see if it works

$ dmesg

# If you want it to load automatically at startup

# 1. Add to /etc/modules
# snd-i2s_rpi

# 2. If you have a Pi old-gen, you need to do this:
# create file called /etc/modprobe.d/snd-i2s_rpi.conf
# add this line
# options snd-i2s_rpi rpi_platform_generation=0

```


Installing as a stand-alone module
====================================

    make
    sudo make install

To load the driver manually, run this as root:

    modprobe snd-i2s_rpi

You may also specify custom toolchains by using the `CROSS_COMPILE` flag:

    CROSS_COMPILE=/usr/local/bin/arm-eabi-


Installing as a part of the kernel
======================================

Instructions to come later. Who would ever wants to do that?




Installing as a DKMS module
=================================

You can have even more fun with snd-i2s\_rpi by installing it as a DKMS module has the main advantage of being auto-compiled (and thus, possibly surviving) between kernel upgrades.

First, get dkms. On Raspbian this should be:

	sudo apt install dkms

Then copy the root of this repository to `/usr/share`:

	sudo cp -R . /usr/src/snd-i2s_rpi-0.0.2 (or whatever version number declared on dkms.conf is)
	sudo dkms add -m snd-i2s_rpi -v 0.0.2

Build and load the module:

	sudo dkms build -m snd-i2s_rpi -v 0.0.2
	sudo dkms install -m snd-i2s_rpi -v 0.0.2

Now you have a proper dkms module that will work for a long time... hopefully.


