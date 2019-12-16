/***********************************************************************************
 Includes
 ***********************************************************************************/
#include "arduino_actuator.h"
#include "Arduino.h"

/***********************************************************************************
 Global variables
 ***********************************************************************************/
int openPin = -1;    // Pin which controls door opening
int closePin = -1;   // Pin which controls door closing
int enablePin = -1;  // Pin whihc enables actuators
int enableTime = 0;  // Time in ms during which actuators are enabled

/***********************************************************************************
 Functions
 ***********************************************************************************/

/*----------------------------------------------------------
 * Function name: ACTUATOR_setOpenPin
 * 
 * Arguments: pin - pin to be set
 * 
 * Return value: void
 * 
 * Description: Set pin which controls door opening
 ---------------------------------------------------------*/
void ACTUATOR_setOpenPin(int pin)
{
  openPin = pin;
}

/*----------------------------------------------------------
 * Function name: ACTUATOR_getOpenPin
 * 
 * Arguments: none
 * 
 * Return value: pin which controls door opening
 * 
 * Description: Get pin which controls door opening
 ---------------------------------------------------------*/
int ACTUATOR_getOpenPin()
{
  return openPin;
}

/*----------------------------------------------------------
 * Function name: ACTUATOR_setClosePin
 * 
 * Arguments: pin - pin to be set
 * 
 * Return value: void
 * 
 * Description: Set pin which controls door closing
 ---------------------------------------------------------*/
void ACTUATOR_setClosePin(int pin)
{
  closePin = pin;
}

/*----------------------------------------------------------
 * Function name: ACTUATOR_getClosePin
 * 
 * Arguments: none
 * 
 * Return value: pin which controls door closing
 * 
 * Description: Get pin which controls door closing
 ---------------------------------------------------------*/
int ACTUATOR_getClosePin()
{
  return closePin;
}

/*----------------------------------------------------------
 * Function name: ACTUATOR_setEnablePin
 * 
 * Arguments: pin - pin to be set
 * 
 * Return value: void
 * 
 * Description: Set pin which enables actuator
 ---------------------------------------------------------*/
void ACTUATOR_setEnablePin(int pin)
{
  enablePin = pin;
}

/*----------------------------------------------------------
 * Function name: ACTUATOR_getEnablePin
 * 
 * Arguments: none
 * 
 * Return value: pin which enables actuator
 * 
 * Description: Get pin which enables actuator
 ---------------------------------------------------------*/
int ACTUATOR_getEnablePin()
{
  return enablePin;
}

/*----------------------------------------------------------
 * Function name: ACTUATOR_actuatorSetup
 * 
 * Arguments: timeInMs - time in miliseconds during which
 *                       actuators whill be enabled
 * 
 * Return value: void
 * 
 * Description: Set the pin modes and enable time
 ---------------------------------------------------------*/
void ACTUATOR_actuatorSetup(int timeInMs) 
{
  // Initialize digital pins as an output.
  pinMode(openPin, OUTPUT);
  digitalWrite(openPin, LOW);
  pinMode(closePin, OUTPUT);
  digitalWrite(closePin, LOW);
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW);

  enableTime = timeInMs;
}

/*----------------------------------------------------------
 * Function name: ACTUATOR_open
 * 
 * Arguments: none
 * 
 * Return value: void
 * 
 * Description: Open the door
 ---------------------------------------------------------*/
void ACTUATOR_open()
{
  digitalWrite(openPin, HIGH);      // Turn the pin on (HIGH is the voltage level)
  digitalWrite(closePin, LOW);      // Turn the pin off (LOW is the voltage level)

  /* Since we don't have information if the doors are opened
     we will hold the enable long enough untill operation is completed */
  digitalWrite(enablePin, HIGH);
  delay(enableTime);
  digitalWrite(enablePin, LOW);
}

/*----------------------------------------------------------
 * Function name: ACTUATOR_close
 * 
 * Arguments: none
 * 
 * Return value: void
 * 
 * Description: Close the door
 ---------------------------------------------------------*/
void ACTUATOR_close()
{
  digitalWrite(openPin, LOW);      // Turn the pin on (HIGH is the voltage level)
  digitalWrite(closePin, HIGH);    // Turn the pin off (LOW is the voltage level)

  /* Since we don't have information if the doors are closed
     we will hold the enable long enough untill operation is completed */
  digitalWrite(enablePin, HIGH);
  delay(enableTime);
  digitalWrite(enablePin, LOW);
}
