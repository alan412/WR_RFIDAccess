/*
  
  Parallax RFID Reader: Basic Demonstration       
                                                         
  Author: Joe Grand [www.grandideastudio.com]             
  Contact: support@parallax.com                            
  
  Program Description:
  
  This program provides a simple demonstration of the Parallax RFID Card
  Reader (#28140). If a valid RFID tag is placed in front of the reader,
  its unique ID is displayed in the Arduino Serial Monitor. 
  
  Please refer to the product manual for full details of system functionality 
  and capabilities.

  Revisions:
  
  1.0 (April 30, 2014): Initial release
  
*/

// include the SoftwareSerial library so we can use it to talk to the RFID Reader
#include <SoftwareSerial.h>

#define enablePin  2   // Connects to the RFID's ENABLE pin
#define rxPin      8  // Serial input (connects to the RFID's SOUT pin)
#define txPin      11  // Serial output (unused)

#define BUFSIZE    11  // Size of receive buffer (in bytes) (10-byte unique ID + null character)

#define RFID_START  0x0A  // RFID Reader Start and Stop bytes
#define RFID_STOP   0x0D

// set up a new serial port
SoftwareSerial rfidSerial =  SoftwareSerial(rxPin, txPin);

void setup()  // Set up code called once on start-up
{
  // define pin modes
  pinMode(enablePin, OUTPUT);
  pinMode(rxPin, INPUT);

  digitalWrite(enablePin, HIGH);  // disable RFID Reader
  
  // setup Arduino Serial Monitor
  Serial.begin(9600);
  while (!Serial);   // wait until ready
  Serial.println("\n\nParallax RFID Card Reader");
  
  // set the baud rate for the SoftwareSerial port
  rfidSerial.begin(2400);

  Serial.flush();   // wait for all bytes to be transmitted to the Serial Monitor
}

//TODO: Alan to clean up this ugliness so he'll stop cringing whenever he sees it.
void read_card (char *rfidData, int maxLength)
{
  char offset = 0;         // Offset into buffer
  rfidData[0] = 0;         // Clear the buffer    

  digitalWrite(enablePin, LOW);   // enable the RFID Reader

  while(1)
  {
    if (rfidSerial.available() > 0) // If there are any bytes available to read, then the RFID Reader has probably seen a valid tag
    {
      rfidData[offset] = rfidSerial.read();  // Get the byte and store it in our buffer
      if (rfidData[offset] == RFID_START)    // If we receive the start byte from the RFID Reader, then get ready to receive the tag's unique ID
      {
        offset = -1;     // Clear offset (will be incremented back to 0 at the end of the loop)
      }
      else if (rfidData[offset] == RFID_STOP)  // If we receive the stop byte from the RFID Reader, then the tag's entire unique ID has been sent
      {
        rfidData[offset] = 0; // Null terminate the string of bytes we just received
        return;                // Break out of the loop
      }          
      offset++;  // Increment offset into array
      if (offset >= BUFSIZE) offset = 0; // If the incoming data string is longer than our buffer, wrap around to avoid going out-of-bounds
    }
  }
} 

#define RATINGS_UNKNOWN 0
#define RATINGS_red 1
#define RATINGS_green 2
#define RATINGS_orange 3
#define RATINGS_blue 4
#define RATINGS_instructor 5

struct person{
  char *rfid;
  char *name;
  int rating;
};

#define NUM_PEOPLE 2

struct person people[NUM_PEOPLE] =
 {{"2F00E69AEF", "Alan", RATINGS_red},
  {"06008E9BF6", "Lisa", RATINGS_blue}};

int getRating (char*rfidData) 
{

  for(int i = 0; i < NUM_PEOPLE; i++){
    if(strcmp(rfidData, people[i].rfid) == 0){
      return people[i].rating;
    }
  }

  return RATINGS_UNKNOWN;
}
 
void loop()  // Main code, to run repeatedly
{ 
  char rfidData[BUFSIZE]; 

  read_card(rfidData, BUFSIZE);

  int currentRating = getRating(rfidData);
  

  Serial.println(rfidData);       // The rfidData string should now contain the tag's unique ID with a null termination, so display it on the Serial Monitor
  Serial.println(currentRating);
   
  Serial.flush();                 // Wait for all bytes to be transmitted to the Serial Monitor
}

void identify(char *id)
{
  if(strcmp(id,"2F00E69AEF") == 0){
    Serial.println("Alan");
  }
  else if(strcmp(id, "06008E9BF6") == 0){
    Serial.println("Lisa");
  }
  else{
    Serial.println("Unknown");
  }
}  



