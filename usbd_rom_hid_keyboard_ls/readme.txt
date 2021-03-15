LPC11U6x Keyboard example (low speed)
=====================================

Example description
-------------------
The example shows how to us USBD ROM stack to creates a XTAL-less low speed HID keyboard.


Special connection requirements
-------------------------------
For low-speed USB operation USB_DP and USB_DM lines have swapped on board. That
is USB_DP should be connected to D- pin on the connector similarly USB_DM to D+.
On Manley boards R8 and R11 resistors can be removed to do the swap.
The tiny joystick that is surface mounted on the eval board has 4 positions
and a button press. They act as number keys '2', '4', '6', '8', and NUMLOCK.
The joystick press toggles NUMLOCK. When NUMLOCK is ON the joystick positions
are mapped to the arrow keys. For most OSs, no drivers are needed.


