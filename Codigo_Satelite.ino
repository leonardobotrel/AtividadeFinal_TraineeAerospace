#include <Satelite.h>
#include <SoftwareSerial.h>
 
//SIM800 TX is connected to Arduino D8
#define SIM800_TX_PIN 8
 
//SIM800 RX is connected to Arduino D7
#define SIM800_RX_PIN 7

String msg = "teste";
int x = 10;
int y = 5;
bool escolha = false;
float area;
int frenquencia;
 
//Create software serial object to communicate with SIM800
SoftwareSerial serialSIM800(SIM800_TX_PIN,SIM800_RX_PIN);

Satelite satelite;
 
void setup() {
  //Begin serial comunication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  while(!Serial);
   
  //Being serial communication witj Arduino and SIM800
  serialSIM800.begin(9600);
  delay(1000);
   
  Serial.println("Setup Complete!");
  Serial.println("Sending SMS...");
   
  //Set SMS format to ASCII
  serialSIM800.write("AT+CMGF=1\r\n");
  delay(1000);
 
  //Send new SMS command and message number
  serialSIM800.write("AT+CMGS=\"07194XXXXX\"\r\n");
  delay(1000);
   
  //Send SMS content
  serialSIM800.write("TEST");
  delay(1000);
   
  //Send Ctrl+Z / ESC to denote SMS message is complete
  serialSIM800.write((char)26);
  delay(1000);
     
  Serial.println("SMS Sent!");
}

/*Handle serial input from the serial monitor. While there is serial information available, 
read it into the char array. If a newline character is encountered, terminate the array 
and return to the main program.*/
int readSerial(char result[])
{
  int i = 0;
  while(1)
  {
    while (Serial.available() > 0)
    {
      char inChar = Serial.read();
      if (inChar == '\n')
      {
        result[i] = '\0';
        return 0;
      }
      if(inChar!='\r')
      {
        result[i] = inChar;
        i++;
      }
    }
  }
}
 
void loop() {
  //satelite.imprimirMensagem();
  //escolha = satelite.escolheArea();

  //Authentication process for sending messages
  //Use the readSerial() function to get the bytes from the serial monitor
  char login[50];
  Serial.print("Enter your login: ");
  readSerial(login);
  Serial.println(login);

  //Use the readSerial() function to get the bytes from the serial monitor
  char password[20];
  Serial.print("Enter your password: ");
  readSerial(password);

  //Verify that the password and login are correct
  Serial.println("authenticating...");
  if(!satelite.authenticate(login, password))
  {
    Serial.println("Login or Password invalid.");
  }
  else
  {
    Serial.println("authenticated.");
  }
  
  
  if (escolha == true)
     area = satelite.areaRetangulo(x, y);
  else
     area = satelite.areaTriangulo(x, y);

  serialSIM800.write(area);
}
