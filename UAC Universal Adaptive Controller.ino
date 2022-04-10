#include <SPI.h>      /* to handle the communication interface with the modem*/
#include <nRF24L01.h> /* to handle this particular modem driver*/
#include <RF24.h>     /* the library which helps us to control the radio modem*/
#include <Chrono.h>
#include <ArduinoJson.h>
#include <Packer.h>

#define VRX_pin A0
#define VRY_pin A1
#define SW_pin 2

#define SCK 13
#define MISO 12
#define MOSI 11
#define CSN 8
#define CE 7

#define LISTEN_RADIO       true
#define FR_ACTIVITY_PERIOD 1000 //Time too wait till new command will be received, if no command disable motor

#define DEVICE_CODE "NONONOOK" //RTC device code

RF24 radio(CE, CSN);                         /* Creating instance 'radio'  ( CE , CSN )   CE -> D7 | CSN -> D8 */                              
const byte readingPipeAddress[6] = "00001";  /* Address to which data to be received*/
Packer                             packer;
Chrono                             rfActivityChecker;
DynamicJsonDocument                doc(64); //Document for decoding data from rf messages

int powerLeftSide =                0;
int powerRightSide =               0;
bool motorsWereUpdated =           true; //In case if motors values were changed

void messageReceiveHandler(char arr[], int size);

void setup () {
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Preparing...");
  // packer.onMessageReady(messageReceiveHandler);

  radio.begin();
  // initializeRFForWriting();

  //Pins mode defining and initializing
  pinMode(VRX_pin, INPUT_PULLUP);
  pinMode(VRY_pin, INPUT_PULLUP);
  pinMode(SW_pin, INPUT_PULLUP);

  Serial.println("READY;");
}
 
void loop () {
  // listenRF();

  int xValue = analogRead(VRX_pin);
  int yValue = analogRead(VRY_pin);
  Serial.print("Value X: ");
  Serial.println(xValue);
  Serial.print("Value Y: ");
  Serial.println(yValue);
  Serial.print("Value BTN: ");
  Serial.println(digitalRead(SW_pin));
  handleMove(1023-xValue, 1023-yValue);
  Serial.println("# - #");
  delay(200);
}


void handleMove (int xAnalog, int yAnalog) {
  double x = map(xAnalog, 0, 1023, -100, 100);
  double y = map(yAnalog, 0, 1023, -100, 100);
  double powerL = x;
  double powerR = x;

  if(y > 20) powerR -= y*2 + 20;
  if(y < 20) powerL -= y*2 + 20;
  
  if(powerL >= 100) powerL = 100;
  if(powerR >= 100) powerR = 100;
  if(powerL <= -100) powerL = -100;
  if(powerR <= -100) powerR = -100;

  //Send data
  Serial.println(String("PowerL: ") + powerL);
  Serial.println(String("PowerR: ") + powerR);
}