#include <Satelite.h>
//#include <Base64.h>
#include <JPEGDecoder.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
 
//SIM800 TX is connected to Arduino D8
#define SIM800_TX_PIN 8
 
//SIM800 RX is connected to Arduino D7
#define SIM800_RX_PIN 7

File jpgFile;

String msg = "teste";
int x = 10;
int y = 5;
float area;
 
//Create software serial object to communicate with SIM800
SoftwareSerial serialSIM800(SIM800_TX_PIN,SIM800_RX_PIN);

Satelite satelite;
 
void setup() {
  //Begin serial comunication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  while(!Serial);
   
  //Being serial communication with Arduino and SIM800
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

  //Begin SD card setup
  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
  }
  else {
  Serial.println("initialization done.");

    // open the file.
    jpgFile = SD.open("test.jpeg");

    // if the file opened okay, start encode process:
    if (jpgFile) {
      Serial.println("Image OK!");

      // Decode the JPEG file
      JpegDec.decodeSdFile(jpgFile);

      // Create a buffer for the packet
      char dataBuff[240];

      // Fill the buffer with zeros
      initBuff(dataBuff);

      // Create a header packet with info about the image
      String header = "$ITHDR,";
      header += JpegDec.width;
      header += ",";
      header += JpegDec.height;
      header += ",";
      header += JpegDec.MCUSPerRow;
      header += ",";
      header += JpegDec.MCUSPerCol;
      header += ",";
      header += jpgFile.name();
      header += ",";
      header.toCharArray(dataBuff, 240);

      // Send the header packet
      for(int j=0; j<240; j++) {
        Serial.write(dataBuff[j]);
      }

      // Pointer to the current pixel
      uint16_t *pImg;

      // Color of the current pixel
      uint16_t color;

      // Create a data packet with the actual pixel colors
      strcpy(dataBuff, "$ITDAT");
      uint8_t i = 6;

      // Repeat for all MCUs(Minimum Coded Units) in the image
      while(JpegDec.read()) {
        // Save pointer the current pixel
        pImg = JpegDec.pImage;

        // Get the coordinates of the MCU(Minimum Coded Units) we are currently processing
        int mcuXCoord = JpegDec.MCUx;
        int mcuYCoord = JpegDec.MCUy;

        // Get the number of pixels in the current MCU(Minimum Coded Units)
        uint32_t mcuPixels = JpegDec.MCUWidth * JpegDec.MCUHeight;

        // Repeat for all pixels in the current MCU(Minimum Coded Units)
        while(mcuPixels--) {
          // Read the color of the pixel as 16-bit integer
          color = *pImg++;
        
          // Split it into two 8-bit integers
          dataBuff[i] = color >> 8;
          dataBuff[i+1] = color;
          i += 2;

          // If the packet is full, send it
          if(i == 240) {
            for(int j=0; j<240; j++) {
              Serial.write(dataBuff[j]);
            }
            i = 6;
          }

          // If we reach the end of the image, send a packet
          if((mcuXCoord == JpegDec.MCUSPerRow - 1) && 
            (mcuYCoord == JpegDec.MCUSPerCol - 1) && 
            (mcuPixels == 1)) {
          
            // Send the pixel values
            for(int j=0; j<i; j++) {
              Serial.write(dataBuff[j]);
            }
          
            // Fill the rest of the packet with zeros
            for(int k=i; k<240; k++) {
              Serial.write(0);
            }
          }
        }
      }
      
    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening test.jpeg");
    }
    
    jpgFile.close();
     
  }
  
}

// Function to fill the packet buffer with zeros
void initBuff(char* buff) {
  for(int i = 0; i < 240; i++) {
    buff[i] = 0;
  }
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

    /*Interface for communication, allows the user to choose what information they want to receive 
    and the frequency of reception*/
    //Use the readSerial() function to get the bytes from the serial monitor to choose the area.
    char escolha[2];
    Serial.println("Enter R to receive the rectangle area or T to receive the triangle area: ");
    readSerial(escolha);
    Serial.println(escolha);

    //Select the area.
    if(escolha[0] == 'R')
      area = satelite.rectangleArea(x, y);
    else if(escolha[0] == 'T')
      area = satelite.triangleArea(x, y);
    else
       Serial.println("Invalid command");

    //Use the readSerial() function to get the bytes from the serial monitor to choose the frequency.
    //read the frequence.
    char frequenciaAux[20];
    Serial.println("Enter the frequency of reception in ms: ");
    readSerial(frequenciaAux);

    //Convert char frequenciaAux to int frequencia
    int frequencia = atoi(frequenciaAux);

    //Sends the chosen message with the desired frequency
    char control = 'a';
    while (control != 'e') {
      Serial.println("enter 'e' to exit.");
      serialSIM800.write(area);//Send the message
      delay(frequencia);

      if (Serial.available())
      {
        control = Serial.read(); 
      }
    }
  }
}
