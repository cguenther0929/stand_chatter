# BLDC Motor Controller #

This repository contains source files for the stand chatter project.  This device operates like a simple texting device allowing for written communication between parties that posses a device.  

### TOOL SUITE ###

The source files are to be compiled using the XC 8 compiler **V2.00**.  The IDE used at the time of development was MPLAB X **v5.10**.  The subsequent .hex file is to be deployed on the PIC18F66K22 MCU.  

### VERSIONS ###
* V0.0.1 -- All peripheral devices seem to work.  The MCU can read the buttons, talk to the display, and communicate with the radio.  However, the incorrect radio is installed, so this version is getting checked in as-is, and the code will be drastically modified to communicate with the correct radio - RFM95W-868S2.

* V0.0.2 -- This code base has the radio successfully transmitting to a Feather M0 wireless module.  

* V0.1.0 -- Transmitting and receiving messages per the intended FSM.  Could easily deploy this version on two communication modules.  

* V1.0.0 -- Initial release.  Received messages are now subjected to a crude check in which message length is verified.  Code greatly cleaned up and well commented.  

* V1.1.0 -- Fixed bug in which a message having a length of 16 characters would register as a message with a length of 21.  Added a few messages to the lineup.  Performed a bit of testing now that there are two modules.     

* V1.2.0 -- Simply added/modified messages just prior to opening day.       

* V1.3.0 -- The method for storing pre-loaded messages has been modified.  Memory wasn't being properly allocated previously, but now things seem much more stable (no more Chinese characters).  Array size changed to 17 characters (16 char + null).  Increased the over current limit by adjusting the OcpTrim config register within the RFM69 radio module.  If using maximum power, the current limit must be increased! No additions/modification to messages themselves.  