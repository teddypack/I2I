LPC11U6x HID generic example (low speed)
========================================

Example description
-------------------
The example shows how to us USBD ROM stack to create a generic HID low speed
device. 
The example supports 1 byte report and loops back the data received in
SET_REPORT message.


Special connection requirements
-------------------------------
For low-speed USB operation USB_DP and USB_DM lines have swapped on board. That
is USB_DP should be connected to D- pin on the connector similarly USB_DM to D+.
On Manley boards R8 and R11 resistors can be removed to do the swap.
Connect the USB cable between micro connector on board and to a host.
The HID reports sent/received by this example are supported by HID_client.exe
program distributed as part of Keil installation "(C:\Keil\ARM\Utilities\HID_Client)".


