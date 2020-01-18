#include <SPI.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include "RF24.h"
#include "printf.h"
#include "setting.h"


RF24 radio(7, 8); // Set up nRF24L01 radio on SPI bus plus pins 7 & CE/CS

/* jason declarations */
StaticJsonDocument<512> doc;
String jsonOutput = "";
/*************************/

static uint32_t message_count = 0;
static bool dataWaitingFlag = false;
static bool dataDecriptionFlag = false;
static bool badDataFlag = false;
unsigned int startTime = 0;
unsigned int endTime = 0;
unsigned long packetCounter = 0;

String incomingData; // hold incoming data before store
String currentNode; // hold current node name

String serialInput= "";
static bool OptimalSendAvailable = false;

void setup()
{
  Serial.begin(115200);
  printf_begin();
  // Setup and configure rf radio
  while (!radio.begin())
  {
    Serial.println("Radio error"); // Indicate radio error
    delay(1000);
  }
  radio.setPALevel(RF24_PA_MIN);

  radio.enableAckPayload();      // We will be using the Ack Payload feature, so please enable it
  radio.enableDynamicPayloads(); // Ack payloads are dynamic payloads

  // Open pipes to other node for communication
  radio.openWritingPipe(NODE_ADDRESS[0]);
  radio.openReadingPipe(1, CONTROLLER_ADDRESS);

  radio.printDetails(); // Dump the configuration of the rf unit for debugging
  delay(50);
  attachInterrupt(0, checkRadio, LOW); // Attach interrupt handler to interrupt #0 (using pin 2) on BOTH the sender and receiver
}

void loop()
{

  Serial.println(F("\tNew cycle bigin..."));
  packetCounter++;                         // incriment packet Count
  String nodeData[NUMITEMS(NODE_ADDRESS)]; // hold every node data to this array;

  for (uint8_t i = 0; i < NUMITEMS(NODE_ADDRESS); i++)
  {
    radio.openWritingPipe(NODE_ADDRESS[i]);
    delay(200); // add some delay to configure radio
    currentNode = NODE_ADDRESS[i]; // Hold current node for json object
    // nodeData[i] = getData();
    String node_data_for_serial = getData();
    Serial.println(node_data_for_serial);
  }

  if(OptimalSendAvailable){
    SendOptimalValue(serialInput,0);
    OptimalSendAvailable =false;
  }

  delay(10000); // Wait some time before beign next cycle
}

String getData()
{
  uint8_t retry = 0;
  bool badCommunicationLinkFlag = false; // Indicate link is not working

  if (!dataWaitingFlag && !dataDecriptionFlag) // send data request if we not wait data from node
  {
    sendRequest();
    dataWaitingFlag = true;
  }
  while (dataWaitingFlag)
  {
    // Do nothing wait for data receive
    //If bad Data resived try Again
    if (DEBUG)
      Serial.println(F("Waiting for data"));
    if (badDataFlag)
    {
      sendRequest();
      if (retry > RETRY_TIMES)
      { // if reach the maximum retrys break the loop
        if (DEBUG)
          Serial.println(F("Communication error"));
        badCommunicationLinkFlag = true;
        break;
      }
      retry++;
      delay(500);
    }
    else
    {
      delay(100);
    }
  }
  while (!dataDecriptionFlag && !badCommunicationLinkFlag)
  {
    //Wait for data to set for decript
    if (DEBUG)
      Serial.println(F("\tWaiting for Decript"));
    delay(50);
  }

  if (dataDecriptionFlag)
  {
    dataDecriptionFlag = false;
    return decriptData(incomingData, retry);
  }
  return decriptData(ERROR_MSG, retry);
}

void sendRequest()
{
  if (DEBUG)
    Serial.println(F("Now sending request"));
  radio.startWrite(&REQUEST_CODE, sizeof(REQUEST_CODE), 0);
}

void checkRadio(void)
{

  bool tx, fail, rx;
  radio.whatHappened(tx, fail, rx); // What happened to trigger interrupt?

  if (tx)
  {
    // Have we successfully transmitted request?
    // If we successfully send the request wait for the data
    if (DEBUG)
      Serial.println(F("Send:OK"));
    badDataFlag = false;
  }

  if (fail)
  {
    // Have we failed to transmit request?
    // The retry to get data from node
    if (DEBUG)
      Serial.println(F("Send:Failed retrying now"));
    badDataFlag = true;
  }

  if (rx || radio.available())
  {
    // Did we receive data form node ?
    char dataFromNode[BUFF_SIZE] = "";
    radio.read(&dataFromNode, sizeof(dataFromNode));
    incomingData = dataFromNode; // convert to string for futer use
    if (DEBUG)
      Serial.println(incomingData);
    if (incomingData == "")
    {
      if (DEBUG)
        Serial.println(F("Receive: Bad data retrying now"));
      badDataFlag = true;
    }
    else
    {
      if (DEBUG)
        Serial.println(F("Receive: OK"));
      dataDecriptionFlag = true;
      dataWaitingFlag = false;
    }
  }
}

String decriptData(String Nodedata, uint8_t retry)
{
  uint8_t j = 0;
  String subData[10];
  for (uint8_t i = 0; i < Nodedata.length(); i++)
  {
    if (Nodedata[i] == '&')
    {
      j++;
    }
    else
    {
      subData[j] = subData[j] + Nodedata[i];
    }
  }
  jsonOutput = "";
  JsonObject data = doc.to<JsonObject>(); // create json object
  for (uint8_t i = 0; i <= j; i++)
  {
    data["data" + String(i)] = subData[i];
  }
  data["retry"] = retry;
  data["node"] = currentNode;
  serializeJson(data, jsonOutput);
  if (DEBUG)
    Serial.println(jsonOutput); // print to serilal monitoro for debug purpuses
  return jsonOutput;
}

void serialEvent() {
  serialInput = "";
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    serialInput += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      //SendOptimalValue (serialInput);
      OptimalSendAvailable = true;
    }
  }
}

void SendOptimalValue(String data,int id){

  char send[BUFF_SIZE];
  strcpy(send, data.c_str());

  radio.openWritingPipe(NODE_ADDRESS[id]);
    delay(200); // add some delay to configure radio
   radio.startWrite(&send, sizeof(send), 0);
}
