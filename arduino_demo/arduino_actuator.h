/***********************************************************************************
 Function prototypes
 ***********************************************************************************/

void ACTUATOR_setOpenPin(int pin);
int ACTUATOR_getOpenPin();

void ACTUATOR_setClosePin(int pin);
int ACTUATOR_getClosePin();

void ACTUATOR_setEnablePin(int pin);
int ACTUATOR_getEnablePin();

void ACTUATOR_actuatorSetup(int timeInMs);

void ACTUATOR_open();
void ACTUATOR_close();
