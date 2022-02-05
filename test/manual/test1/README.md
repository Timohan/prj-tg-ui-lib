# prj-tg-ui-lib test1

This tests basic functionalities in lib with some buttons and click callbacks.

This tests also mouse button count at the same time (default 1).

If mouse button count is 1 -> only latest mouse to down button is in the use.  
For example with button  
* left mouse button down
* right mouse button down (now left mouse button release will be ignored, and only right mouse button release is in the use).
* left mouse release - is ignored
* right mouse button release is registered and click callback is sent with right mouse button type

