
// Select your modem:
#define TINY_GSM_MODEM_SIM800

#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial

// Use Hardware Serial on Mega, Leonardo, Micro
#define SerialAT Serial3

/* **
 *  Variable definition for server details
 */
const char apn[] = "mobitel3g";
const char user[] = "";
const char pass[] = "";

const char server[] = "wireless-data-collector.herokuapp.com";
const char resource[] = "/TinyGSM/logo.txt";
const int port = 80;

String urlpath = "/node/test";
String type = "application/json";
String message = "{\"node0\":{\"data0\": \"1234\",\"data1\": \"2345\", \"retry\": \"4\"},\"node1\":{\"data0\": \"1234\",\"data1\": \"2345\", \"retry\": \"4\" }, \"controlName\": \"new name\", \"date\": \"2019/4/5\", \"time\": \"12:02\", \"packets\": 8 }";
/* **
 *  END
 */
#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

TinyGsmClient client(modem);
HttpClient http(client, server, port);

void simInit()
{
  delay(10);

  // Set GSM module baud rate
  SerialAT.begin(115200);
  delay(3000);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  SerialMon.println(F("Initializing modem..."));
  modem.restart();

  String modemInfo = modem.getModemInfo();
  SerialMon.print(F("Modem: "));
  SerialMon.println(modemInfo);

  SerialMon.print(F("Waiting for network..."));
  if (!modem.waitForNetwork())
  {
    SerialMon.println(" fail");
    delay(1000);
    return;
  }
  SerialMon.println(" OK");
}

void sendMessage(String data)
{
  SerialMon.print(F("Connecting to "));
  SerialMon.print(apn);
  if (!modem.gprsConnect(apn, user, pass))
  {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" OK");

  SerialMon.print(F("Performing HTTP POST request... "));
  int err2 = http.post(urlpath, type, data);
  if (err2 != 0)
  {
    SerialMon.println(F("failed to send"));
    delay(10000);
    return;
  }
  else
  {
    SerialMon.println(F("done"));
    delay(10000);
  }

  http.stop();
  SerialMon.println(F("Server disconnected"));

  modem.gprsDisconnect();
  SerialMon.println(F("GPRS disconnected"));
}
