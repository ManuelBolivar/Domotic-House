#include <NexButton.h>
#include <NexCheckbox.h>
#include <NexConfig.h>
#include <NexCrop.h>
#include <NexDualStateButton.h>
#include <NexGauge.h>
#include <NexGpio.h>
#include <NexHardware.h>
#include <NexHotspot.h>
#include <NexNumber.h>
#include <NexObject.h>
#include <NexPage.h>
#include <NexPicture.h>
#include <NexProgressBar.h>
#include <NexRadio.h>
#include <NexRtc.h>
#include <NexScrolltext.h>
#include <NexSlider.h>
#include <NexText.h>
#include <NexTimer.h>
#include <NexTouch.h>
#include <NexUpload.h>
#include <NexVariable.h>
#include <NexWaveform.h>
#include <Nextion.h>
#include <doxygen.h>




// Version 1.0

#include <ESP8266WiFi.h>

///////////////Parameters & Constants/////////////////
// WIFI params
char* WIFI_SSID = "SSID";    // Configure here the SSID of your WiFi Network
char* WIFI_PSWD = "PassWord"; // Configure here the PassWord of your WiFi Network
int WIFI_DELAY  = 100; //ms

// oneM2M : CSE params
String CSE_IP      = "IP Address"; //Configure here the IP Address of your oneM2M CSE
int   CSE_HTTP_PORT = 8080;
String CSE_NAME    = "cse-in";
String CSE_RELEASE = "3"; //Configure here the release supported by your oneM2M CSE
bool ACP_REQUIRED = true; //Configure here whether or not ACP is required controlling access
String ACPID = "";
  
// oneM2M : resources' params
String DESC_CNT_NAME = "DESCRIPTOR";
String DATA_CNT_NAME = "DATA";
String CMND_CNT_NAME = "COMMAND";
String ACP_NAME = "MYACP";
int TY_ACP  = 1;   
int TY_AE  = 2;   
int TY_CNT = 3; 
int TY_CI  = 4;
int TY_SUB = 23;
String originator = "Undefined";

// HTTP constants
int LOCAL_PORT = 80;
char* HTTP_CREATED = "HTTP/1.1 201 CREATED";
char* HTTP_OK    = "HTTP/1.1 200 OK\r\n";
int REQUEST_TIME_OUT = 10000; //ms
int REQUEST_NR = 0;

//MISC
int LUMINOSITY_PIN = A0;  // Adapt it accordding to your wiring 
int LED_PIN = 4;  // Adapt it accordding to your wiring. Use BUILTIN_LED for onboarded led
#define OFF_STATE  LOW // *** External LED is active at HIGH, while Onboarded LED is active at LOW. 
#define ON_STATE HIGH // *** Adapat it is according to your config
int SERIAL_SPEED  = 115200;
String sensorValue;
#define DEBUG

//NEXTION

NexButton l_r_door_on = NexButton(1 , 12 , "b0");
NexButton l_r_door_off = NexButton(1 , 13 , "b1");
NexButton living_room_on = NexButton(1 , 18 , "b6");
NexButton living_room_off = NexButton(1 , 19 , "b7");
NexButton fireplace_on = NexButton(1 , 14 , "b2");
NexButton fireplace_off = NexButton(1 , 15 , "b3");
NexButton kitchen_on = NexButton(3 , 3 , "b10");
NexButton kitchen_off = NexButton(3 , 4 , "b11");
NexButton worktop_on = NexButton(3 , 7 , "b12");
NexButton worktop_off = NexButton(3 , 8 , "b13");
NexButton right_hearth_on = NexButton(3 , 13 , "b14");
NexButton right_hearth_off = NexButton(3 , 14 , "b15");
NexButton left_hearth_on = NexButton(3 , 15 , "b16");
NexButton left_hearth_off = NexButton(3 , 16 , "b17");
NexRadio bathroom_on = NexRadio(4 , 6 , "r0");
NexRadio bathroom_off = NexRadio(4 , 7 , "r1");
NexRadio bedroom_on = NexRadio(4 , 10 , "r2");
NexRadio bedroom_off = NexRadio(4 , 11 , "r3");
NexDSButton frontdoor = NexDSButton(2 , 1 , "sw0");
NexDSButton source = NexDSButton(2 , 6 , "sw1");
NexDSButton pool = NexDSButton(2 , 10 , "sw2");

//NexSwitch frontdoor
 
NexTouch *components[] = {
  &l_r_door_on,
  &l_r_door_off,
  &living_room_on,
  &living_room_off,
  &fireplace_on,
  &fireplace_off,
  &kitchen_on,
  &kitchen_off,
  &worktop_on,
  &worktop_off,
  &right_hearth_on,
  &right_hearth_off,
  &left_hearth_on,
  &left_hearth_off,
  &bathroom_on,
  &bathroom_off,
  &bedroom_on,
  &bedroom_off,
  &frontdoor,
  &source,
  &pool,
   NULL
};

///////////////////////////////////////////


// Global variables
const long interval = 2000;
long currentMillis;

WiFiServer server(LOCAL_PORT);    // HTTP Server (over WiFi). Binded to listen on LOCAL_PORT constant
WiFiClient client0;
WiFiClient client1;
String context = "";        // The targeted actuator
String command = "";        // The received command

// Method for creating an HTTP POST with preconfigured oneM2M headers
// param : url  --> the url path of the targted oneM2M resource on the remote CSE
// param : ty --> content-type being sent over this POST request (2 for ae, 3 for cnt, etc.)
// param : rep  --> the representaton of the resource in JSON format
String doPOST(String url, String originator1, int ty, String rep) {

  String relHeader = "";
  if (CSE_RELEASE != "1") {
    relHeader = "X-M2M-RVI: " + CSE_RELEASE + "\r\n";
  }
  String postRequest = String() + "POST " + url + " HTTP/1.1\r\n" +
                       "Host: " + CSE_IP + ":" + CSE_HTTP_PORT + "\r\n" +
                       "X-M2M-Origin: " + originator + "\r\n" +
                       "Content-Type: application/vnd.onem2m-res+json;ty=" + ty + "\r\n" +
                       "Content-Length: " + rep.length() + "\r\n" +
                       relHeader +
                       "X-M2M-RI: req"+REQUEST_NR+"\r\n" +
                       "Connection: close\r\n\r\n" +
                       rep;
  // Connect to the CSE address
  Serial.println("connecting to " + CSE_IP + ":" + CSE_HTTP_PORT + " ...");

  // Get a client
  if (!client1.connect(CSE_IP, CSE_HTTP_PORT)) {
    Serial.println("Connection failed !");
    return "error";
  }

  // if connection succeeds, we show the request to be send
  #ifdef DEBUG
  Serial.println(postRequest);
  #endif

  // Send the HTTP POST request
  client1.print(postRequest);

  //Update request number after each sending
  REQUEST_NR += 1;
  
  // Manage a timeout
  unsigned long startTime = millis();
  while (client1.available() == 0) {
    if (millis() - startTime > REQUEST_TIME_OUT) {
      Serial.println("Client Timeout");
      client1.stop();
      return "error";
    }
  }

  // If success, Read the HTTP response
  String result = "";
  client1.setTimeout(500);
  if (client1.available()) {
    result = client1.readStringUntil('\r');
    Serial.println(result);
  }
  while (client1.available()) {
    String line = client1.readStringUntil('\r');
    Serial.print(line);
        }    
  Serial.println();
  Serial.println("closing connection...");
  return result;
}

// Method for creating an ApplicationEntity(AE) resource on the remote CSE (this is done by sending a POST request)
// param : ae --> the AE name (should be unique under the remote CSE)
String createAE(String ae) {
  String srv = "";
    if(CSE_RELEASE != "1"){
      srv = ",\"srv\":[\""+CSE_RELEASE+"\"]";
    }
  String aeRepresentation =
    "{\"m2m:ae\": {"
    "\"rn\":\"" + ae + "\","
    "\"api\":\"Norg.demo." + ae + "\","
    "\"rr\":true,"
    "\"poa\":[\"http://" + WiFi.localIP().toString() + ":" + LOCAL_PORT + "/" + ae + "\"]" +
    srv +
    "}}";
  #ifdef DEBUG
  Serial.println(aeRepresentation);
  #endif
  return doPOST("/" + CSE_NAME, originator, TY_AE, aeRepresentation);
}

// Method for creating an Access Control Policy(ACP) resource on the remote CSE under a specific AE (this is done by sending a POST request)
// param : ae --> the targeted AE name (should be unique under the remote CSE)
// param : acp  --> the ACP name to be created under this AE (should be unique under this AE)
String createACP(String ae, String acp) {
  String acpRepresentation =
    "{\"m2m:acp\": {"
    "\"rn\":\"" + acp + "\","
    "\"pv\":{\"acr\":[{\"acor\":[\"all\"],\"acop\":63}]},"
    "\"pvs\":{\"acr\":[{\"acor\":[\"all\"],\"acop\":63}]}"
    "}}";
  return doPOST("/" + CSE_NAME + "/" + ae, originator, TY_ACP, acpRepresentation);
}

// Method for creating an Container(CNT) resource on the remote CSE under a specific AE (this is done by sending a POST request)
// param : ae --> the targeted AE name (should be unique under the remote CSE)
// param : cnt  --> the CNT name to be created under this AE (should be unique under this AE)
String createCNT(String ae, String cnt) {
  String cntRepresentation =
    "{\"m2m:cnt\": {"
    "\"mni\":10,"         // maximum number of instances
    "\"rn\":\"" + cnt + "\"" +
    ACPID + //IF ACP created, it is associated to the container so that anyone has access 
    "}}";
  return doPOST("/" + CSE_NAME + "/" + ae, originator, TY_CNT, cntRepresentation);
}

// Method for creating an ContentInstance(CI) resource on the remote CSE under a specific CNT (this is done by sending a POST request)
// param : ae --> the targted AE name (should be unique under the remote CSE)
// param : cnt  --> the targeted CNT name (should be unique under this AE)
// param : ciContent --> the CI content (not the name, we don't give a name for ContentInstances)
String createCI(String ae, String cnt, String ciContent) {
  String ciRepresentation =
    "{\"m2m:cin\": {"
    "\"con\":\"" + ciContent + "\""
    "}}";
  return doPOST("/" + CSE_NAME + "/" + ae + "/" + cnt, originator,  TY_CI, ciRepresentation);
}

// Method for creating an Subscription (SUB) resource on the remote CSE (this is done by sending a POST request)
// param : ae --> The AE name under which the SUB will be created .(should be unique under the remote CSE)
//          The SUB resource will be created under the COMMAND container more precisely.
String createSUB(String ae) {
  String subRepresentation =
    "{\"m2m:sub\": {"
    "\"rn\":\"SUB_" + ae + "\","
    "\"nu\":[\"" + CSE_NAME + "/" + ae  + "\"], "
    "\"nct\":1,"
    "\"enc\":{\"net\":[3]}"
    "}}";
  return doPOST("/" + CSE_NAME + "/" + ae + "/" + CMND_CNT_NAME, originator,  TY_SUB, subRepresentation);
}


// Method to register a module (i.e. sensor or actuator) on a remote oneM2M CSE
void registerModule(String module, bool isActuator, String intialDescription, String initialData) {
  if (WiFi.status() == WL_CONNECTED) {
    String result;
    
    // 1. Create the ApplicationEntity (AE) for this sensor
    result = createAE(module);
    if (result.equalsIgnoreCase(HTTP_CREATED)) {
      #ifdef DEBUG
      Serial.println("AE " + module + " created  !");
      #endif
      // 1.1 Create the AccessControlPolicy (ACP) for this sensor so that monitor can subscribe to it
      if(ACP_REQUIRED) {
        result = createACP(module, ACP_NAME);
        if (result.equalsIgnoreCase(HTTP_CREATED)) {
          ACPID = ",\"acpi\":[\"" + CSE_NAME + "/" + module + "/" + ACP_NAME + "\"]";
          #ifdef DEBUG
          Serial.println("ACP " + module + " created  !");
        #endif
        }
      }
      // 2. Create a first container (CNT) to store the description(s) of the sensor
      result = createCNT(module, DESC_CNT_NAME);
      if (result.equalsIgnoreCase(HTTP_CREATED)) {
        #ifdef DEBUG
        Serial.println("CNT " + module + "/" + DESC_CNT_NAME + " created  !");
        #endif

        // Create a first description under this container in the form of a ContentInstance (CI)
        result = createCI(module, DESC_CNT_NAME, intialDescription);
        if (result.equalsIgnoreCase(HTTP_CREATED)) {
          #ifdef DEBUG
          Serial.println("CI " + module + "/" + DESC_CNT_NAME + "/{initial_description} created !");
          #endif
        }
      }

      // 3. Create a second container (CNT) to store the data  of the sensor
      result = createCNT(module, DATA_CNT_NAME);
      if (result.equalsIgnoreCase(HTTP_CREATED)) {
        #ifdef DEBUG
        Serial.println("CNT " + module + "/" + DATA_CNT_NAME + " created !");
        #endif

        // Create a first data value under this container in the form of a ContentInstance (CI)
        result = createCI(module, DATA_CNT_NAME, initialData);
        if (result.equalsIgnoreCase(HTTP_CREATED)) {
          #ifdef DEBUG
          Serial.println("CI " + module + "/" + DATA_CNT_NAME + "/{initial_data} created !");
          #endif
        }
      }

      // 4. if the module is an actuator, create a third container (CNT) to store the received commands
      if (isActuator) {
        result = createCNT(module, CMND_CNT_NAME);
        if (result.equalsIgnoreCase(HTTP_CREATED)) {
          #ifdef DEBUG
          Serial.println("CNT " + module + "/" + CMND_CNT_NAME + " created !");
          #endif

          // subscribe to any ne command put in this container
          result = createSUB(module);
          if (result.equalsIgnoreCase(HTTP_CREATED)) {
            #ifdef DEBUG
            Serial.println("SUB " + module + "/" + CMND_CNT_NAME + "/SUB_" + module + " created !");
            #endif
          }
        }
      }
    }
  }
}

void init_IO() {

}
void task_IO() {
}

void init_WiFi() {
  Serial.println("Connecting to  " + String(WIFI_SSID) + " ...");
  WiFi.persistent(false);
  WiFi.begin(WIFI_SSID, WIFI_PSWD);

  // wait until the device is connected to the wifi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(WIFI_DELAY);
    Serial.print(".");
  }

  // Connected, show the obtained ip address
  Serial.println("WiFi Connected ==> IP Address = " + WiFi.localIP().toString());
}
void task_WiFi() {
}

void init_HTTPServer() {
  server.begin();
  Serial.println("Local HTTP Server started !");
}
void task_HTTPServer() {
  // Check if a client is connected
  client0 = server.available();
  if (client0){  
    // Wait until the client sends some data
    Serial.println("New client connected. Receiving request <=== ");
    
    while (!client0.available()) {
      #ifdef DEBUG
      Serial.print(".");
      #endif
      delay(5);
    }

    // Read the request
    client0.setTimeout(500);
    String request = client0.readString();
    Serial.println(request);

    int start, end;
    // identify the right module (sensor or actuator) that received the notification
    // the URL used is ip:port/ae
    start = request.indexOf("/");
    end = request.indexOf("HTTP") - 2;
    context = request.substring(start+1, end);
    #ifdef DEBUG
    Serial.println(String() + "context = [" + start + "," + end + "] -> " + context);
    #endif

    // ingore verification messages 
    if (request.indexOf("vrq") > 0) {
      client0.flush();
      client0.print(HTTP_OK);
      delay(5);
      client0.stop();
      Serial.println("Client disconnected");
      return;
    }

    //Parse the request and identify the requested command from the device
    //Request should be like "[operation_name]"
    start = request.indexOf("[");  
    end = request.indexOf("]"); // first occurence fo 
    command = request.substring(start+1, end);
    #ifdef DEBUG
    Serial.println(String() + + "command = [" +  start + "," + end + "] -> " + command);
    #endif

    client0.flush();
    client0.print(HTTP_OK);
    delay(5);
    client0.stop();
    Serial.println("Client disconnected");

  }
}


void init_nextion() {
  String initialDescription = "Name = Pantalla_nextion Location = Entrada";
  String initialData = "0";
  originator = "Cae-Pantalla_nextion";
  registerModule("Pantalla_nextion", false, initialDescription, initialData);
}
void task_nextion() {
  #ifdef DEBUG
  Serial.println("Pantalla nextion value = " + sensorValue);
  #endif
  String ciContent = String(sensorValue);
  originator = "Cae-Pantalla_nextion";
  createCI("Pantalla_nextion", DATA_CNT_NAME, ciContent); 
}
void command_nextion(String cmd) {
}

void setup() {
  // intialize the serial liaison
  Serial.begin(SERIAL_SPEED);
  nexInit();
  // configure sensors and actuators HW
  init_IO();

  // Connect to WiFi network
  init_WiFi();

  // Start HTTP server
  init_HTTPServer();

  // register sensors and actuators
  init_nextion();
  //init_led();
  l_r_door_on.attachPush(door_on, &l_r_door_on );
  l_r_door_off.attachPush(door_off, &l_r_door_off );
  living_room_on.attachPush(room_on, &living_room_on );
  living_room_off.attachPush(room_off, &living_room_off );
  fireplace_on.attachPush(firep_on, &fireplace_on);
  fireplace_off.attachPush(firep_off, &fireplace_off);
  kitchen_on.attachPush(cocina_on, &kitchen_on);
  kitchen_off.attachPush(cocina_off, &kitchen_off);
  worktop_on.attachPush(encimera_on, &worktop_on);
  worktop_off.attachPush(encimera_off, &worktop_off);
  right_hearth_on.attachPush(right_h_on, &right_hearth_on);
  right_hearth_off.attachPush(right_h_off, &right_hearth_off);
  left_hearth_on.attachPush(left_h_on, &left_hearth_on);
  left_hearth_off.attachPush(left_h_off, &left_hearth_off);
  bathroom_on.attachPush(bano_on, &bathroom_on);
  bathroom_off.attachPush(bano_off, &bathroom_off);
  bedroom_on.attachPush(dormitorio_on, &bedroom_on);
  bedroom_off.attachPush(dormitorio_off, &bedroom_off);
  frontdoor.attachPush(cochera, &frontdoor);
  source.attachPush(fuente, &source);
  pool.attachPush(piscina, &pool);
  pinMode(2,OUTPUT);

}

// Main loop of the µController
void loop() {

   nexLoop( components );
  
}
void door_on(void *ptr)  
{  
  sensorValue="door_on"; task_HTTPServer(); task_nextion();
 
}  
 
void door_off(void *ptr)  
{  
  sensorValue="door_off"; task_HTTPServer(); task_nextion();
} 
void room_on(void *ptr)  
{  
  sensorValue="room_on"; task_HTTPServer(); task_nextion();
}  
 
void room_off(void *ptr)  
{  
  sensorValue="room_off"; task_HTTPServer(); task_nextion();
} 
void firep_on(void *ptr)  
{  
  sensorValue="fireplace_on"; task_HTTPServer(); task_nextion();
}  
 
void firep_off(void *ptr)  
{  
  sensorValue="fireplace_off"; task_HTTPServer(); task_nextion();
} 

void cocina_on(void *ptr)  
{  
  sensorValue="kitchen_on"; task_HTTPServer(); task_nextion();
}  
 
void cocina_off(void *ptr)  
{  
  sensorValue="kitchen_off"; task_HTTPServer(); task_nextion();
} 

void encimera_on(void *ptr)  
{  
  sensorValue="worktop_on"; task_HTTPServer(); task_nextion();
}  
 
void encimera_off(void *ptr)  
{  
  sensorValue="worktop_off"; task_HTTPServer(); task_nextion();
}
void right_h_on(void *ptr)  
{  
  sensorValue="right_h_on"; task_HTTPServer(); task_nextion();
}  
 
void right_h_off(void *ptr)  
{  
  sensorValue="right_h_off"; task_HTTPServer(); task_nextion();
}
void left_h_on(void *ptr)  
{  
  sensorValue="leftt_h_on"; task_HTTPServer(); task_nextion();
}  
 
void left_h_off(void *ptr)  
{  
  sensorValue="left_h_off"; task_HTTPServer(); task_nextion();
}
void bano_on(void *ptr)  
{  
  sensorValue="bathroom_on"; task_HTTPServer(); task_nextion();
}  
 
void bano_off(void *ptr)  
{  
  sensorValue="bathroom_off"; task_HTTPServer(); task_nextion();
}
void dormitorio_on(void *ptr)  
{  
  sensorValue="bedroom_on"; task_HTTPServer(); task_nextion();
}  
 
void dormitorio_off(void *ptr)  
{  
  sensorValue="bedroom_off"; task_HTTPServer(); task_nextion();
}

void cochera(void *ptr)  
{  
  if (cochera) {sensorValue="frontdoor_on"; task_HTTPServer(); task_nextion();}
  else sensorValue="frontdoor_off";
}
 void fuente(void *ptr)  
{  
  if (fuente) {sensorValue="source_on"; task_HTTPServer(); task_nextion();}
  else sensorValue="source_off";
} 
 void piscina(void *ptr)  
{  
  if (piscina) {sensorValue="pool_on"; task_HTTPServer(); task_nextion();}
  else sensorValue="pool_off";
} 