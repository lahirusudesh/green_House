# 1 "g:\\wireless-data-collector -Lahiru\\ARDUINO-SRC\\NODE\\DatatransferProtocall.h"
# 1 "g:\\wireless-data-collector -Lahiru\\ARDUINO-SRC\\NODE\\DatatransferProtocall.h"
# 1 "g:\\wireless-data-collector -Lahiru\\ARDUINO-SRC\\NODE\\node.ino"
# 2 "g:\\wireless-data-collector -Lahiru\\ARDUINO-SRC\\NODE\\node.ino" 2
# 3 "g:\\wireless-data-collector -Lahiru\\ARDUINO-SRC\\NODE\\node.ino" 2
# 4 "g:\\wireless-data-collector -Lahiru\\ARDUINO-SRC\\NODE\\node.ino" 2
# 5 "g:\\wireless-data-collector -Lahiru\\ARDUINO-SRC\\NODE\\node.ino" 2
# 6 "g:\\wireless-data-collector -Lahiru\\ARDUINO-SRC\\NODE\\node.ino" 2
# 7 "g:\\wireless-data-collector -Lahiru\\ARDUINO-SRC\\NODE\\node.ino" 2
# 8 "g:\\wireless-data-collector -Lahiru\\ARDUINO-SRC\\NODE\\node.ino" 2
# 9 "g:\\wireless-data-collector -Lahiru\\ARDUINO-SRC\\NODE\\node.ino" 2

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
# 25 "g:\\wireless-data-collector -Lahiru\\ARDUINO-SRC\\NODE\\node.ino"
dht11 DHT11;

void setup()
{
    Serial.begin(115200);
    printf_begin();
    buzInit(10);
    // Setup and configure rf radio
    while (!radio.begin())
    {
        beep(100, 3); // Indicate radio error
    }

    radio.setPALevel(RF24_PA_MIN);
    //radio.setDataRate(RF24_250KBPS);
    radio.enableAckPayload(); // We will be using the Ack Payload feature, so please enable it
    radio.enableDynamicPayloads(); // Ack payloads are dynamic payloads

    // Open pipes to other node for communication
    radio.openWritingPipe(CONTROLLER_ADDRESS);
    radio.openReadingPipe(1, NODE_ADDRESS);
    radio.startListening();
    radio.writeAckPayload(1, &message_count, sizeof(message_count)); // Add an ack packet for the next time around.  This is a simple
    ++message_count;

    radio.printDetails(); // Dump the configuration of the rf unit for debugging
    delay(50);
    attachInterrupt(0, checkRadio, 0x0); // Attach interrupt handler to interrupt #0 (using pin 2) on BOTH the sender and receiver
    beep(100, 2);
}

/********************** Main Loop *********************/
void loop()
{
    tempNHum = DHT11.read(4);

    if (successFlag)
    {
        // Idicate successfully send the data;
        beep(100, 1);
        successFlag = false;
    }
    else if (errorflag)
    {
        // Idicate Tere is an error sending failed
        beep(100, 3);
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
        Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 84 "g:\\wireless-data-collector -Lahiru\\ARDUINO-SRC\\NODE\\node.ino" 3
                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 84 "g:\\wireless-data-collector -Lahiru\\ARDUINO-SRC\\NODE\\node.ino"
                      "Send:OK"
# 84 "g:\\wireless-data-collector -Lahiru\\ARDUINO-SRC\\NODE\\node.ino" 3
                      ); &__c[0];}))
# 84 "g:\\wireless-data-collector -Lahiru\\ARDUINO-SRC\\NODE\\node.ino"
                      )));
        successFlag = true;
    }

    if (fail)
    {
        // Have we failed to transmit request?
        // Try to resend data again if we fails
        Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 92 "g:\\wireless-data-collector -Lahiru\\ARDUINO-SRC\\NODE\\node.ino" 3
                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 92 "g:\\wireless-data-collector -Lahiru\\ARDUINO-SRC\\NODE\\node.ino"
                      "Send:Failed"
# 92 "g:\\wireless-data-collector -Lahiru\\ARDUINO-SRC\\NODE\\node.ino" 3
                      ); &__c[0];}))
# 92 "g:\\wireless-data-collector -Lahiru\\ARDUINO-SRC\\NODE\\node.ino"
                      )));
        errorflag = true;
    }

    if (rx || radio.available())
    {
        // Did we receive data  request form centrall ?
        Serial.println("data request received");
        //String code; // Get the request code
        char dataFromCenterl[32] = "";

        radio.read(&dataFromCenterl, sizeof(dataFromCenterl));

        Serial.println(dataFromCenterl);

        char data[32];

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
            beep(100, 2);
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

uint16_t voltageTest()
{
    return analogRead(A3);
}

String getTemp()
{
    return String(DHT11.temperature);
}

String getHum()
{
    Serial.println(DHT11.humidity);
    return String(DHT11.humidity);
}
