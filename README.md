# pclink-i2c

This project contains information on the PC-Link hardware interface for some early 2000 Sony HiFi decks, such as the CMT-DC500MD (also known as HCD-J300), DHC-MD595, and more. These decks can communicate with a PC using a PCLK-MN10 adaptor and M-Crew software. There exists open-source software for controlling these decks using the official PCLK-MN10 adaptor. This project aims to document how to achieve the same, but **without** adaptor, using off the shelf electronics.

## Demo

See https://www.reddit.com/r/minidisc/comments/s78dyj/didnt_have_a_pclink_for_my_fancy_new_sony_hcdj300/

## Hardware
A 6-pin mini-din connector (such as is used by PS2 keyboard/mice) is used for the interface. Communication is essentially done using I2C, with some minor MOSFETs and I2C buffers to create a more stable external interface. The I2C bus for the interface is used-by/connected-to at least two peripherals inside of the deck (in the case of the HCD-J300), the main digital board handling the logical glue between the components of the deck, and the main controller board, handling input (buttons, remote, display).

The service documentation for the HCD-J300 has this helpful diagram:

![I2C Diagram](hardware/i2c-j300.png)

As you can see, aside from the I2C bus, there is one additional interface available named "PC POWER". Presumably, this is used to turn on the HiFi deck when it's in standby mode (as the internal peripherals listening on the I2C bus are disabled in this state), but this **needs to be tested** in order to confirm.

## Protocol, I2C bus

Scanning reveals that there are 2 peripherals available on the I2C bus, 0x09, and 0x10. Sniffing on the bus reveals that pushing buttons on the device trigger broadcast commands to 0x00 (i.e. it will be received by all peripherals).

Testing reveals that 0x09 is the MD board, 0x10 the main board. If you send commands to 0x00, both boards will process any commands that they can handle, but ignore commands intended for another board. For example, 0xB0 MD commands will only be processed by the MD board, 0x90 CD commands by the mainboard.

| Peripheral | I2C addr | response/command prefix |
| -----------|----------|-------------------------|
| Main board | 0x10     | 0x20 (== 0x10 << 1)     |
| MD board   | 0x09     | 0x12 (== 0x09 << 1)     |

### Sending commands

Initial testing reveals that the commands are essentially the same as what the M-Crew software sends to the official adaptor (without some packaging). Commands as previously documented by other reverse engineering projects for this adaptor (such as https://github.com/4gra/pclk-mn10/blob/master/commands.json) can easily be transmitted using I2C transactions.

To send a command, initiate a transaction for address 0x00, prefixed with any number (generally your slave address << 1), following with the bytes for the commands.

E.g., in the case of the Arduino framework, to open the CD drive:

```
Wire.beginTransmission(0x0); // or 0x10 to directly target the main board
Wire.write("\x20\x90\x54\x00\x01"); // 0x20 is the slave address from the main board, this value seems to be ignored, so you can put anything else
Wire.endTransmission();
```

### Reading data

All responses are broadcasted to 0x00 as well. Most responses for the MD board are prefixed by 0x12, and 0x20 for the main board. To receive updates / read data, you need to join the bus as a slave and listen for broadcast data (in the case of AVR board this means setting the lower bit of TWAR).

## Resources

https://github.com/4gra/pclk-mn10/

https://github.com/Ircama/Sony_SLink
