STM32 Digital Clock
_____________________________________________________________________________________

Description

This project is intented to create an Digital Clock taking the advantages of the STM32 microcontroller.
The microcontroller used for this project was an STM32F44.

One way to track the time is using the build-in RTC module in the STM32F44 MC.
The Clock displays the information through and LCD, in order to operate this peripheral a driver was created to control it.

One long therm goal is to show the time and date via TFT tocuh screen from Nextion.
To accomplish this feature a driver will be created as well as the communication will be driven with UART protocol.
______________________________________________________________________________________________________


Supported functionalities

* Display hour in a 12 hour format
* Display date in a format MM/DD/YYYY
* Adjustable time and date trought fisical buttons
* One adjustable Alarm
