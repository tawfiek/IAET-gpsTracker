#include <SoftwareSerial.h> // Library for using serial communication
SoftwareSerial A9G(7, 8); // Pins 7, 8 are used as used as software serial pins
#include <Arduino.h>

String incomingData;   // for storing incoming serial data

static void receive_message();
static void SendMessage();
static void MakeCall();
static void HangupCall();
static void RedialCall();
static void ReceiveCall();
static void ReadLocation();
static void sendATCommand(int command); 

char msg;
char call;
void setup()
{
  Serial.begin(115200); // baudrate for serial monitor
  A9G.begin(115200); // baudrate for GSM shield
  

  Serial.println("GSM A9G BEGIN");
  Serial.println("Enter character for control option:");
  Serial.println("h : to disconnect a call");
  Serial.println("i : to receive a call");
  Serial.println("s : to send message");
  Serial.println("c : to make a call");  
  Serial.println("e : to redial");
  Serial.println("l : to read location");
  Serial.println();
  delay(100);

  
  /*A9G.println("AT+GPS=1");
  delay(100);
  A9G.println("AT+GPSRD=5");
  delay(5000);*/
  
  // set SMS mode to text mode
  A9G.print("AT+CMGF=1\r");  
  delay(100);
  
  // set gsm module to tp show the output on serial out
  A9G.print("AT+CNMI=2,2,0,0,0\r"); 
  delay(100);
}

void loop()
{
  receive_message();


if (Serial.available()>0) 
{
   int command = Serial.read();
   switch(command)
  {
    case 's':
      SendMessage();
      break;
    case 'c':
      MakeCall();
      break;
    case 'h':
      HangupCall();
      break;
    case 'e':
      RedialCall();
      break;
    case 'i':
      ReceiveCall();
      break;
    case 'l':
      ReadLocation();
      break;
    default: 
      sendATCommand(command);
  }
       
}
  
}

void sendATCommand(int command)
{
  A9G.println(command);
  Serial.println(command + "Command Sent");
  delay(1000);
}

void receive_message()
{
  // Serial.print("Trying to recive from Module !!");
  // Serial.print("Module serial avaible buytes to read: " + A9G.available());
  if (A9G.available() > 0)
  {
    incomingData = A9G.readString(); // Get the data from the serial port.
    Serial.print(incomingData); 
    delay(10); 
  }
}

void SendMessage()
{
  A9G.println("AT+CMGF=1");  //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  A9G.println("AT+CMGS=\"+201118772996\"\r"); // Replace x with mobile number
  delay(1000);
  A9G.println("sim900a sms");// The SMS text you want to send
  delay(100);
   A9G.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

void ReceiveMessage()
{
  A9G.println("AT+CNMI=2,2,0,0,0"); // AT Command to recieve a live SMS
  delay(1000);
  if (A9G.available()>0)
  {
    msg=A9G.read();
    Serial.print(msg);
  }
}

void MakeCall()
{
  A9G.println("ATD+201118772996;"); // ATDxxxxxxxxxx; -- watch out here for semicolon at the end!!
  Serial.println("Calling  "); // print response over serial port
  delay(1000);
}


void HangupCall()
{
  A9G.println("ATH");
  Serial.println("Hangup Call");
  delay(1000);
}

void ReceiveCall()
{
  A9G.println("ATA");
  delay(1000);
  {
    call=A9G.read();
    Serial.print(call);
  }
}

void RedialCall()
{
  A9G.println("ATDL");
  Serial.println("Redialing");
  delay(1000);
}
void ReadLocation(){

  A9G.println("AT+GPS=1");
  delay(1000);
  A9G.println("AT+GPSRD=5");
  delay(1000);
  
  }