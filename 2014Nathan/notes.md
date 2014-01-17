NOTES
=====

Getters and Setters
-------------------

All access to class variables in this code is done through getters and setters. It makes the code longer but safer.

Class Team1257Robot
-------------------

Drive Funcions
--------------
I use multiple drive functions:
	
TankDrive() and ArcadeDrive()
-----------------------------
Self-explanatory.
XYDrive()
---------
Uses the GetX() and GetY() functions of joysticks.
ZTwistDrive()
-------------
Uses GetZ() and GetTwist().
Drive()
-------
Simply sets left and right outputs, for autonomous use.
CheckSetDriveSticks()
---------------------
This void-returning function checks pressed buttons and sets enabled sticks and drive-type base on its findings.
SetArmsX() and SetArmsY()
-------------------------
Sets the speeds of the Victors for the arms in the respective directions, X being horizontal and Y being vertical.
Printf()
--------
Basically simplifies the DriverStationLCD::Printf() usage.
Arm1 and Arm2
-------------
1 is left, 2 is right.
Get...() and Set...()
---------------------
Self-explanatory
	
<cassert> inclusion
--------------------
Need to remove but too lazy...

THE END!!!
----------
