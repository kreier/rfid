# Projects folder

The main programs are located here, just download, compile and upload to your Arduino. Missing libraries can be found in the /library subfolder. Initially started November 2018

* 1602 blank

This program is just to test the 1602 display connected to the I2C bus.

Update October 2019:

The RFID.flush() does not clear the input buffer anymore, so the code was adjusted

### AISVN update 2019

The nametags of AISVN have a 125kHz chip that identifies the cardholder and is used for payments at the canteen. We can read it out and display. Use nametag_AISVN.ino
