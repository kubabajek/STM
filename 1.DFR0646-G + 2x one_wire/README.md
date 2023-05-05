Read temperature from two DS18B20 
(uart1 halfduplex one wire)

Send them by uart2 and to DFR0646-G segment display.

Library to use display has been written based on arduino library, maybe it helps someone who also had problem to run DFR0646-G on STM32. 
Its first version where you can only display 2 double, 3 digit numbers or whatever you want on each segment separately.
In free time I will update it to print dot independently which allow me to tidy print function and make it more professional.
