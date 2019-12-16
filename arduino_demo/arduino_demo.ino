/***********************************************************************************
 * Project: Arduino Demo for Remote Access Control
 * 
 * Description: This is simple web-server application which controls 
 * digital pins connected to a motor driver on user demand. After user is 
 * successfully connected to a Arduino board using IP address which will be 
 * available on serial port, GET method can be used in order to send 'O' or 'C'
 * commands for opening or closing the doors, respectively.
 * 
 * Designed by: Strahinja Golic
 * 
 * Date: 12/12/19
 ***********************************************************************************/

/***********************************************************************************
 Includes
 ***********************************************************************************/
#include <WiFi101.h>
#include "arduino_secrets.h" 
#include "arduino_actuator.h"

/***********************************************************************************
 Defines
 ***********************************************************************************/
#define BAUD_RATE 9600
#define SERVER_PORT 80
#define ASCII_y 121
#define ASCII_n 110
#define ASCII_ENTER 13
#define MAX_CHARS 32
#define PIN_OPEN 3
#define PIN_CLOSE 4
#define PIN_ENABLE 5
#define ENABLE_TIME_MS 5000
#define ANSWER_WAIT 10

/***********************************************************************************
 Global variables
 ***********************************************************************************/
/* Default SSID and PASS values are stired in tab/arduino_secrets.h
   User will be able to change these values via serial port. */
char ssid[MAX_CHARS] = SECRET_SSID;      // Your network SSID (name)
char pass[MAX_CHARS] = SECRET_PASS;      // Your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                        // Your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;             // Network status

WiFiServer server(SERVER_PORT);          // Server data

/***********************************************************************************
 Functions
 ***********************************************************************************/

 /*----------------------------------------------------------
  * Function name: readStringSerial
  * 
  * Arguments: strBuf - buffer that needs to be filled with
  *                     incoming string
  *            bufLen - size of the buffer in characters
  *            classified - if serial input is classified
  *                         it won't be shown in terminal
  * 
  * Return value: true - success
  *               false - failed
  * 
  * Description: Read string from serial bus
  ---------------------------------------------------------*/
boolean readStringSerial(char *strBuf, int bufLen, boolean classified)
{
  boolean newData = false;          // New data is received
  byte ndx = 0;                     // Array element index
  char rc;                          // Received character
  char receivedChars[MAX_CHARS];    // An array to store the received data

  // Check input parameters
  if (strBuf == NULL)
  {
    return false;
  }

  while (newData == false) 
  {
    if (Serial.available() > 0)
    {
      // Read from serial bus
      rc = Serial.read();

      if (!classified)
      {
        Serial.write(rc);
      }
      else
      {
        Serial.write('*');
      }
  
      // Check if end of the string is received
      if (rc != ASCII_ENTER) 
      {
        // Add character to string
        receivedChars[ndx] = rc;
        ndx++;
  
        // If overflow is detected, overwrite last character
        if (ndx >= MAX_CHARS) 
        {
          ndx = MAX_CHARS - 1;
        }
      }
      else 
      {
        /* If more characters are received than the buffer length, 
           extra characters will be lost */
        if (ndx >= bufLen)
        {
          ndx = bufLen - 1;
        }
        
        receivedChars[ndx] = '\0'; // Terminate the string
        memcpy(strBuf, receivedChars, ndx);
        newData = true;
      }
    }
  }

  return true;
}

 /*----------------------------------------------------------
  * Function name: setNetwork
  * 
  * Arguments: none
  * 
  * Return value: void
  * 
  * Description: Set SSID and PASS over serial bus
  ---------------------------------------------------------*/
void setNetwork()
{
  memset(ssid, 0, MAX_CHARS);
  memset(pass, 0, MAX_CHARS);
  
  // Print new line
  Serial.println("");
  
  Serial.println("Insert new SSID: ");

  if (!readStringSerial(ssid, MAX_CHARS, false))
  {
    Serial.println("Error while setting SSID");
    return;
  }
  else
  {
    // Print new line
    Serial.println("");
  }

  Serial.println("Insert new password: ");

  if (!readStringSerial(pass, MAX_CHARS, true))
  {
    Serial.println("Error while setting password");
    return;
  }
  else
  {
    // Print new line
    Serial.println("");
  }
}

 /*----------------------------------------------------------
  * Function name: printWiFiStatus
  * 
  * Arguments: none
  * 
  * Return value: void
  * 
  * Description: Print network parameters
  ---------------------------------------------------------*/
void printWiFiStatus() 
{
  // Print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // Print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // Print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

 /*----------------------------------------------------------
  * Function name: setup
  * 
  * Arguments: none
  * 
  * Return value: void
  * 
  * Description: Performs needed settings before main loop
  ---------------------------------------------------------*/
void setup() 
{
  int timer = 0; //Counts for needed time
  int incomingByte = 0; // Incoming serial data
   
  // Initialize serial and wait for port to open:
  Serial.begin(BAUD_RATE);
  while (!Serial) 
  {
    ; // Wait for serial port to connect. Needed for native USB port only
  }

  // Check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) 
  {
    Serial.println("WiFi shield not present");
    // Don't continue:
    while (true);
  }

  // Set actuator parameters
  ACTUATOR_setOpenPin(PIN_OPEN);
  ACTUATOR_setClosePin(PIN_CLOSE);
  ACTUATOR_setEnablePin(PIN_ENABLE);
  ACTUATOR_actuatorSetup(ENABLE_TIME_MS);

  // If SSID is already set, check if user wants to change it
  if (ssid[0] != '\0')
  {
    // Prompt if user wants to connect to a network other than default
    Serial.print("Current SSID is: ");
    Serial.print(ssid);
    Serial.println(". Would you like to connect to a different SSID? y/n");

    timer = ANSWER_WAIT; // Wait 10 seconds for the answer
  }
  else
  {
    setNetwork(); 
  }

  while (timer)
  {
    if (Serial.available() > 0) 
    {
      // Read the incoming byte:
      incomingByte = Serial.read();
      Serial.write(incomingByte);
    }

    switch (incomingByte)
    {
      case ASCII_y:
        // Set new network parameters and exit the loop
        setNetwork();
        timer = 0;
        break;
        
      case ASCII_n:
        // Use default network parameters and exit the loop
        timer = 0;
        break;

      default:
        break;      
    }

    if (timer)
    {
      // Wait for one second and decrement the timer
      delay(1000);
      timer--;
    }
  }

  // Print new line
  Serial.println("");

  // Attempt to connect to WiFi network:
  while (status != WL_CONNECTED) 
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // Wait 10 seconds for connection:
    delay(10000);
  }
  
  server.begin();
  
  // You're connected now, so print out the status:
  printWiFiStatus();
}

 /*----------------------------------------------------------
  * Function name: loop
  * 
  * Arguments: none
  * 
  * Return value: void
  * 
  * Description: Main function
  ---------------------------------------------------------*/
void loop() 
{
  // Listen for incoming clients
  WiFiClient client = server.available();

  if (client) 
  {
    Serial.println("");
    Serial.println("----------------------------------------------------------------------");
    Serial.println("New client");
    
    String currentLine = "";                // String to hold incoming data from the client
    while (client.connected()) 
    {
      if (client.available()) 
      {
        char c = client.read();             // Read a byte
        Serial.write(c);                    // Print it out the serial monitor

        // If the byte is a newline character
        if (c == '\n') 
        {

          /* if the current line is blank, you got two newline characters in a row.
             that's the end of the client HTTP request, so send a response: */
          if (currentLine.length() == 0) 
          {
            /* HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
               and a content-type so the client knows what's coming, then a blank line: */
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // The content of the HTTP response follows the header:
            client.print("<br><br><br>Click <a href=\"/O\">here</a> to open the door.<br>");
            client.print("<br><br><br>Click <a href=\"/C\">here</a> to close the door.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // Break out of the while loop:
            break;
          }
          else 
          {      
            // If you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') 
        {    
          // if you got anything else but a carriage return character, add it to the end of the currentLine
          currentLine += c;
        }

        // Check to see if the client request was "GET /O" or "GET /C":
        if (currentLine.endsWith("GET /O")) 
        {
          ACTUATOR_open();               // GET /O opens the door
        }
        if (currentLine.endsWith("GET /C")) 
        {
          ACTUATOR_close();              // GET /C closes the door
        }
      }
    }
    
    // Close the connection:
    client.stop();
    Serial.println("Client disonnected");
    Serial.println("----------------------------------------------------------------------");
    Serial.println("");
  }
}
