#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
#include "setting.h"
#include "buzzer.h"
#include "radio.h"
//#include <dht11.h>
#include "dht.h"

static uint32_t message_count = 0;
bool sendData = false;
bool successFlag = false;
bool errorflag = false;
int tempNHum = 0;

// Optimal position
int optTemp;
int optHum;
int optCo2;

/**
 * Sensor pin definition
 */
#define DHT11PIN A0
dht DHT;

void setup()
{
    DHT.read11(DHT11PIN);
    Serial.begin(115200);
    printf_begin();
    buzInit(10);
    // Setup and configure rf radio
    while (!radio.begin())
    {
        beep(BEEP_TIME_INTERVAL, ERROR_BEEP); // Indicate radio error
    }

    radio.setPALevel(RF24_PA_MIN);
    //radio.setDataRate(RF24_250KBPS);
    radio.enableAckPayload();      // We will be using the Ack Payload feature, so please enable it
    radio.enableDynamicPayloads(); // Ack payloads are dynamic payloads

    // Open pipes to other node for communication
    radio.openWritingPipe(CONTROLLER_ADDRESS);
    radio.openReadingPipe(1, NODE_ADDRESS);
    radio.startListening();
    radio.writeAckPayload(1, &message_count, sizeof(message_count)); // Add an ack packet for the next time around.  This is a simple
    ++message_count;

    radio.printDetails(); // Dump the configuration of the rf unit for debugging
    delay(50);
    attachInterrupt(0, checkRadio, LOW); // Attach interrupt handler to interrupt #0 (using pin 2) on BOTH the sender and receiver
    beep(BEEP_TIME_INTERVAL, INFO_BEEP);
}

/********************** Main Loop *********************/
void loop()
{
    DHT.read11(DHT11PIN);

    if (successFlag)
    {
        // Idicate successfully send the data;
        beep(BEEP_TIME_INTERVAL, SUCCESS_BEEP);
        successFlag = false;
    }
    else if (errorflag)
    {
        // Idicate Tere is an error sending failed
        beep(BEEP_TIME_INTERVAL, ERROR_BEEP);
        errorflag = false;
    }
}

void checkRadio(void)
{

    bool tx, fail, rx;
    radio.whatHappened(tx, fail, rx); // What happened to trigger interrupt?

    if (tx)
    {
        // Have we successfully transmitted request?
        Serial.println(F("Send:OK"));
        successFlag = true;
    }

    if (fail)
    {
        // Have we failed to transmit request?
        // Try to resend data again if we fails
        Serial.println(F("Send:Failed"));
        errorflag = true;
    }

    if (rx || radio.available())
    {
        // Did we receive data  request form centrall ?
        Serial.println("data request received");
        //String code; // Get the request code
        char dataFromCenterl[BUFF_SIZE] = "";

        radio.read(&dataFromCenterl, sizeof(dataFromCenterl));

        Serial.println(dataFromCenterl);

        char data[BUFF_SIZE];

        if (dataFromCenterl[0] == REQUEST_CODE)
        {
            strcpy(data, getData().c_str());
            radio.writeAckPayload(1, &data, sizeof(data));
        }
        else
        {
            
            //Serial.println(F("Invalid request"));
            radio.writeAckPayload(1, &OPTIMAL_VALUE_CHANE_ACK, sizeof(OPTIMAL_VALUE_CHANE_ACK));
            changeOptValues(dataFromCenterl);
            beep(BEEP_TIME_INTERVAL, INFO_BEEP);
        }
    }
}

String getData()
{
    String massage = getHum() + "&" + getTemp();
    Serial.println(massage);
    return massage;
}

void changeOptValues(String data){
    Serial.println ("Optimal values " +data);
}

String getTemp()
{
    return String(DHT.temperature);
}

String getHum()
{
    return String(DHT.humidity);
}
