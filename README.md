# RC_Device
 This project is a public version of a multi-functional controller for RC vehicles.
 Primarily, this is intended for land vehicles due to the size and potential weight.
 However, i can see it being useful to boats and planes as well. I would say drones,
 but they already have crazy tiny control modules.

 # Features
 The base features of this project are:
 - **Lights**
   - **Headlights & taillights**
     - on all the time
     - on after channel is out of deadzone
     - animations and transitions
     - brake lights when brake is applied
     - high beams if configured and what triggers them
   - **2 additional fixed output channels**
     - typically these would be used for either turn signal, but can be configured for
       other lights as well
  - **4-Auxillary ports**
    - These 4 ports can be configured as additional lights, additional input channels, or
      if the code functions correctly they might be useable as channel passthroughs to allow
      signal manipulation
 - **Webserver**
   - Shows status of devices connected
   - GUI for configuring ports, channels, lights, lighting effects, datalogging visualization
 - **IMU**
   - Currently developing with MPU6050 because of inexpensive modules on amazon
   - Will expand useable IMU's in the future
   - If a custom circuit board is made then a LISxxx IMU will be used
   - The IMU will measure 3 axis gyro and 3 axis acceleration
     - base intentions for use of the IMU to help assist in visualizing forces while operating
       the vehicle. Visualizing how turns and accelerations can or need to be adjusted for better
       race perfomance or better control during speed runs
     - Potential for active acceleration control for speed runs.
     - Provide maximum throttle until IMU detects vehicle lifting from torque, then countering
       this lift by decellerating.
- **GPS**
  - addition of GPS module would allow for tracking if vehicle gets of LOS (provided main battery power is not lost)

# Hardware
![alt text](https://github.com/Psylenceo/RC_Device/blob/683b829e938ea9c14517846d7e230cc802156c6b/Images/ESp32%20io%20layout.png)
![alt text](https://github.com/Psylenceo/RC_Device/blob/683b829e938ea9c14517846d7e230cc802156c6b/Images/Wemos%20D1%20Schematic.jpeg)
![alt text](https://github.com/Psylenceo/RC_Device/blob/683b829e938ea9c14517846d7e230cc802156c6b/Images/Pin%20layout.png)
![alt text](https://github.com/Psylenceo/RC_Device/blob/683b829e938ea9c14517846d7e230cc802156c6b/Images/BS-280%20GPS%20Pinout.jpg)
![alt text](https://github.com/Psylenceo/RC_Device/blob/main/20230225_035814.mp4)
