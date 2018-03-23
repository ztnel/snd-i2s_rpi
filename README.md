# snd-i2s\_rpi

Driver for Adafruit's I2S MEMS Mic


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

Instructions to come later. Who ever wants to do that?




Installing as a DKMS module:
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


