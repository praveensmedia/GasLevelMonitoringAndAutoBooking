# GasLevelMonitoringAndAutoBooking
The aim of this project is to monitor the Gas level in the cylinder and to make automatic booking of new cylinder when the gas level drops threshold. I used Arduino-UNO, SIM900a,Load cell,HX711,and display.

Working of this project
Connect the weighing sensor to the arduino with the cable provided, then insert
the power cable to its port on arduino from 12v adapter.
Switch it ON.
First it shows your project name then calibration will be start. The display shows
put 180 grams and wait. Then put that weight and wait. Calibration completes in
3sec then GSM initialization starts this will completes in 3 sec.
Now its ready .
It displays Gas left percentage and weight.
If the Gas percentage is falls to 10% then it sends a message to distributor.
Remember the percentage is upto 2Kg only. If the weight is greater than 2kg it
shows 100%only. But weight shows accurately upto 20KG.
Fix the weighing pan to the bench.
