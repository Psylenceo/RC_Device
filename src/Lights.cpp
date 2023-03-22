#include <Global_Variables.h>
#include <Lights/Lights.h>

Pin Lights[3] =
{
    //simple on and off lights appear to work with all 0's for settings????
    //first 3 are required, next 5 are inputs, last 5 are output settings
  //{R,R   ,0,X,X,X,X,X,status,CH,Hz,Bits,duty cycle}  
    {2,Out1,0,0,0,0,0,0,0,0,60,8,0},
    {2,Out2,0,0,0,0,0,0,0,1,60,8,0},
    {2,Out3,0,0,0,0,0,0,0,2,60,8,0}
};

