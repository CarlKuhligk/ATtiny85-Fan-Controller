# Use
This project was created for using an nvidia tesla graphics card in a virtual machine. The host system is Proxmox. Proxmox forwards the GPU to the VM. In order to achieve this, Proxmox is not allowed to use the GPU, which is why no drivers are installed. That's why proxmox cannot determine the temperature of the GPU directly.

The usb controller can be passed through at proxmox, so that the virtual machine can control the fan.

Finally, a small python script is required, which reads the gpu temperature and regulates the fan accordingly.

# USB Fan pwm controller based on ATtiny45/85
It uses [V-USB](https://www.obdev.at/products/vusb/index.html) to build a [USB communication device class (CDC)](https://en.wikipedia.org/wiki/USB_communications_device_class) for serial communication via USB with the ATtiny. The PWM value is send via the USB device. If the Value is "0" the Fan gets turned off. Example "echo "65" > /dev/ttyACM0"

![pic1.jpg](https://github.com/CarlKuhligk/ATtiny85-Fan-Controller/blob/main/documentation/IMG_20220520_110823.jpg)


# Hardware
The schematic is shown below:
![schematic.png](https://github.com/CarlKuhligk/ATtiny85-Fan-Controller/blob/main/documentation/Simple_Schematic.png)

Not pretty but functional
![pic2.jpg](https://github.com/CarlKuhligk/ATtiny85-Fan-Controller/blob/main/documentation/IMG_20220520_110706.jpg)
![pic3.jpg](https://github.com/CarlKuhligk/ATtiny85-Fan-Controller/blob/main/documentation/IMG_20220520_110726.jpg)

Housing for the circuit board
![box](https://user-images.githubusercontent.com/87583841/169691579-dab364ed-fa7d-433d-a732-912c28a1b767.png)

Adapter for Nvidia Tesla

STL: https://www.thingiverse.com/thing:5391731

# Software
The communication via USB is handled by the V-USB software-only implementation of a low-speed USB device. To simplify the software development with the Arduino IDE the [VUSB_AVR board package](https://github.com/wagiminator/VUSB-AVR) is used. It includes the VUSB_CDC library, which makes it easy to implement a serial communication via USB.

# Compiling, Uploading and Testing
- Open Arduino IDE.
- [Install VUSB-AVR](https://github.com/wagiminator/VUSB-AVR#Installation).
- Go to **Tools -> Board -> VUSB AVR** and select **VUSB-AVR**.
- Go to **Tools -> CPU** and select **ATtiny85 (16.5 MHz internal)**.
- Connect your programmer to your PC and to the ICSP header of the device.
- Go to **Tools -> Programmer** and select your ISP programmer.
- Go to **Tools -> Burn Bootloader** to burn the fuses.
- Open the TinyTerminal sketch and click **Upload**.
- The buzzer will make some noise during the upload as it is connected to one of the ICSP pins.
- Disconnect the programmer and connect the device via USB to your PC.

Alternatively, the precompiled hex file can be uploaded:

```
avrdude -c usbasp -p t85 -U lfuse:w:0xe1:m -U hfuse:w:0xdd:m -U efuse:w:0xff:m -U flash:w:TinyTerminal_t85.hex
```

The device will work under Linux out-of-the-box. Windows users need to install the [Digistump drivers](https://raw.githubusercontent.com/digistump/DigistumpArduino/master/tools/micronucleus-2.0a4-win.zip).

To test the device you can open the Serial Monitor of the Arduino IDE and send a message. With Linux the port is usually /dev/ttyACM0. You can also send a message via a Terminal:

```
echo "65" > /dev/ttyACM0
```

**The device was only tested with Linux!**
