#ifndef IO_H
#define IO_H

#include <Arduino.h>
#include <RC_Reciever/RC_Reciever.h>
/**********************************************************************
 *
 *                      Global pin interface variables
 *
 * *******************************************************************/
#define STATUS_LED_PIN 2

#define Out1 0
#define Out2 12
#define Out3 15

#define Aux1 33
#define Aux2 14
#define Aux3 27
#define Aux4 25
#define Aux5 32
#define Aux6 4

#define SpareIO_1 13
#define SpareIO_2 26
#define SpareIO_3 36

//pin use, pin #, init'd, Detected, Seq pos, seq #, ontime
struct Pin {
    uint8_t pin_use;               //0 not used, 1 input, 2 LED output, 3 RC PWM OUT
    uint8_t PinNumber;
    char Name[20];                     //gives the pin a name
    bool Initialized;              //has given port been initialized

    /* Section for inputs*/
    volatile uint8_t Port_Number;
    volatile bool Detected;                 //has given port triggered its interrupt
    volatile uint8_t Sequence_Position;     //if the ports are triggered in sequence, which port is 1st, 2nd, etc
    volatile uint8_t Measurement_Sequence;  //which state the port is in. Triggered, captured, or Reset
    volatile long On_Time;                  //current on time for he given port

    /* Section for LED's*/
    double LED_Status; // should return frequency, i think
    uint8_t LED_Channel; // 0 - 16
    const uint32_t LED_Frequency;
    const uint8_t LED_Resolution; // how many bits
    uint8_t duty_cycle;


    /**********************************************************************
    *
    *
    *
    * *******************************************************************/
    void InitializeRX() {
        pinMode(PinNumber, INPUT_PULLDOWN);
        char buffer[60];
        snprintf(buffer, sizeof(buffer), "Initializing....Input Port %u \"%s\" Interrupt", Port_Number, Name);
        Serial.println(buffer);

        if (Port_Number == 1) {
            attachInterrupt(PinNumber, &Port_1_Input, CHANGE);
        }
        else if (Port_Number == 2) {
            attachInterrupt(PinNumber, &Port_2_Input, CHANGE);
        }
        else if (Port_Number == 3) {
            attachInterrupt(PinNumber, &Port_3_Input, CHANGE);
        }

        Initialized = 1;
    }

    /**********************************************************************
    *
    *
    *
    * *******************************************************************/
    void InitializeLED() {
        char buffer[40];
        snprintf(buffer, sizeof(buffer), "Initializing....%s", Name);
        Serial.println(buffer);
        pinMode(PinNumber, OUTPUT);
        LED_Status = ledcSetup(LED_Channel, LED_Frequency, LED_Resolution);
        ledcAttachPin(PinNumber, LED_Channel);

        Initialized = 1;
    }

    /**********************************************************************
    *
    *
    *
    * *******************************************************************/
    void Brightness() {
        if (duty_cycle >= 100)ledcWrite(LED_Channel, 65536);
        if (duty_cycle > 0 && duty_cycle < 100)ledcWrite(LED_Channel, duty_cycle * 648);
        if (duty_cycle <= 0)ledcWrite(LED_Channel, 0);
    }

    /**********************************************************************
    *
    *
    *
    * *******************************************************************/
    void InitializeAux() {
        if (pin_use == 1)InitializeRX();
        if (pin_use == 2)InitializeLED();
    }
};

extern Pin Port[3];
extern Pin Lights[3];
extern Pin Aux[2];

#endif