void initializeRFForWriting(void) {
  radio.setPALevel(RF24_PA_MIN);
  radio.openReadingPipe(1, readingPipeAddress); /* Sets the address of receiver from which program will receive the data*/
  radio.startListening();          /*Setting modem in Receiver mode*/
}


/* Listen IR if there is a pack. When message will be generated, callback will be callde*/
void listenRF() {
  if(!LISTEN_RADIO) return;

  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    builtPack built = packer.getBuiltPack(text, sizeof(text));
    packer.pushPack(packer.restorePack(built));
  }
}

/*Handler called when message was received via RF communication*/
void messageReceiveHandler(char arr[], int size) {
  if(size > 80) {
    Serial.println(F("Message too big, aborting"));
    return;
  }
  
  deserializeJson(doc, arr, size);
  int powerL = doc["powerL"];
  int powerR = doc["powerR"];
  String deviceCode = doc["deviceCode"];
  
  // if(deviceCode == String(DEVICE_CODE)) {
  //   setBothSidePower(powerL, powerR);
  // }
}