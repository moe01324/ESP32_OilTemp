/**************************************************************************
ESP32_OilTemp

Use on your own risk.

More info at https://github.com/moe01324/ESP32_OilTemp

Really not beautiful, but works. ;-)
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "BluetoothSerial.h"
#include <VdoTemperatureSender.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

const int PushButton = 15;
const int oilPin = 32;

const char* host = "Porsche Esp32";
const char* ssid = "xxxx";
const char* password = "xxxxx";

WebServer server(80);

/* Style */
String style =
"<style>#file-input,input{width:100%;height:44px;border-radius:4px;margin:10px auto;font-size:15px}"
"input{background:#f1f1f1;border:0;padding:0 15px}body{background:#3498db;font-family:sans-serif;font-size:14px;color:#777}"
"#file-input{padding:0;border:1px solid #ddd;line-height:44px;text-align:left;display:block;cursor:pointer}"
"#bar,#prgbar{background-color:#f1f1f1;border-radius:10px}#bar{background-color:#3498db;width:0%;height:10px}"
"form{background:#fff;max-width:258px;margin:75px auto;padding:30px;border-radius:5px;text-align:center}"
".btn{background:#3498db;color:#fff;cursor:pointer}</style>";

/* Login page */
String loginIndex = 
"<form name=loginForm>"
"<h1>Porsche ESP32 Login</h1>"
"<input name=userid placeholder='User ID'> "
"<input name=pwd placeholder=Password type=Password> "
"<input type=submit onclick=check(this.form) class=btn value=Login></form>"
"<script>"
"function check(form) {"
"if(form.userid.value=='admin' && form.pwd.value=='admin')"
"{window.open('/serverIndex')}"
"else"
"{alert('Error Password or Username')}"
"}"
"</script>" + style;
 
/* Server Index Page */
String serverIndex = 
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
"<input type='file' name='update' id='file' onchange='sub(this)' style=display:none>"
"<label id='file-input' for='file'>   Choose file...</label>"
"<input type='submit' class=btn value='Update'>"
"<br><br>"
"<div id='prg'></div>"
"<br><div id='prgbar'><div id='bar'></div></div><br></form>"
"<script>"
"function sub(obj){"
"var fileName = obj.value.split('\\\\');"
"document.getElementById('file-input').innerHTML = '   '+ fileName[fileName.length-1];"
"};"
"$('form').submit(function(e){"
"e.preventDefault();"
"var form = $('#upload_form')[0];"
"var data = new FormData(form);"
"$.ajax({"
"url: '/update',"
"type: 'POST',"
"data: data,"
"contentType: false,"
"processData:false,"
"xhr: function() {"
"var xhr = new window.XMLHttpRequest();"
"xhr.upload.addEventListener('progress', function(evt) {"
"if (evt.lengthComputable) {"
"var per = evt.loaded / evt.total;"
"$('#prg').html('progress: ' + Math.round(per*100) + '%');"
"$('#bar').css('width',Math.round(per*100) + '%');"
"}"
"}, false);"
"return xhr;"
"},"
"success:function(d, s) {"
"console.log('success!') "
"},"
"error: function (a, b, c) {"
"}"
"});"
"});"
"</script>" + style;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define LOGO_HEIGHT   128
#define LOGO_WIDTH    32

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
BluetoothSerial ESP_BT; //Object for Bluetooth

// Oil Temperature Table
/** for VDO Oil Sensor, with 220 Ohm Resistor
 */
unsigned int vdoTemp[] =
{3269,
3213,
3110,
2969,
2771,
2531,
2248,
1947,
1640,
1363,
1110,
908,
723,
573,
486,
416
};

VdoTemperatureSender *oilTemperatureSensor;

// 'cayman', 128x32px
const unsigned char porsche [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 
  0x00, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xe0, 
  0x00, 0x1f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xc0, 
  0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 
  0x01, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 
  0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 
  0x0f, 0x00, 0x00, 0x1c, 0x00, 0x80, 0x00, 0x06, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x3f, 0xf8, 0x00, 
  0x1e, 0x00, 0x00, 0xfe, 0xe3, 0x87, 0x1f, 0xff, 0x0f, 0xee, 0x3f, 0xc0, 0x00, 0x07, 0xff, 0x00, 
  0x1c, 0x00, 0x03, 0xe1, 0xe3, 0x8f, 0x3f, 0xff, 0x3e, 0x1e, 0x7f, 0xc0, 0x00, 0x00, 0x1f, 0x80, 
  0x3c, 0x00, 0x07, 0x83, 0x87, 0x1c, 0x71, 0xc6, 0x78, 0x78, 0xe1, 0x80, 0x00, 0x00, 0x0f, 0xc0, 
  0x3c, 0x00, 0x0f, 0x0e, 0x0e, 0x38, 0xe3, 0x8c, 0x70, 0xf1, 0xc3, 0x00, 0x00, 0x00, 0x3f, 0x80, 
  0x1f, 0x00, 0xff, 0x3e, 0x3c, 0x71, 0xc3, 0x1c, 0xf3, 0xe3, 0x87, 0x00, 0x00, 0x03, 0xff, 0x00, 
  0x1f, 0xff, 0xff, 0xff, 0xef, 0xff, 0x86, 0x1f, 0xff, 0xff, 0x07, 0x83, 0xff, 0xff, 0xfc, 0x00, 
  0x07, 0xfe, 0x03, 0x06, 0x01, 0xf9, 0x04, 0x1c, 0x10, 0x34, 0x07, 0x87, 0xff, 0xff, 0x80, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

int menuNumber = 0;

void setup() {
  Serial.begin(115200); 
  pinMode(PushButton, INPUT);
  oilTemperatureSensor = new VdoTemperatureSender(oilPin, vdoTemp);

  ESP_BT.begin("Porsche_ESP32_Oiltemp"); //Name of your Bluetooth Signal
  Serial.println("Bluetooth Device is Ready to Pair");

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
  }

  display.display();
  delay(1000);
  screenOne();

 // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.println("");
  //to allow network connection
  delay(10000);

  Serial.println(WiFi.localIP());

  /*use mdns for host name resolution*/
  MDNS.begin(host);
  //to allow network connection
  delay(10000);
 
  Serial.println("mDNS responder started");
  /*return index page which is stored in serverIndex */
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", loginIndex);
  });
  server.on("/serverIndex", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  /*handling uploading firmware file */
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
  server.begin();
}
  
void loop() {       
    int oilTemperature = oilTemperatureSensor->getTemperature();
    Serial.print("Oil T: ");
    Serial.println(oilTemperature);
    ESP_BT.println("Oil T: ");
    ESP_BT.println(oilTemperature);
    delay(200);
    
  int Push_button_state = digitalRead(PushButton); 
  
  if (Push_button_state == LOW) {
    delay(200);
    menuNumber = menuNumber + 1;

    if (menuNumber == 4) {
      menuNumber = 0;
    }

    Serial.println("low");
    Serial.println(menuNumber);
  }
    switch (menuNumber) {
      case 0:
      screenZero(oilTemperature);
      break;
      case 1:
      screenOne();
      break;
      case 2:
      screenTwo(oilTemperature);
      break;
      case 3:
      screenThree();
      break;
    }  

  if (Push_button_state == HIGH) {
      Serial.println("high");
  } 
  server.handleClient();
}

void screenZero(int oiltemp) {
  display.display();
  if(oiltemp < 80){
     screenTwo(oiltemp);
   } else {
    screenThree();
  }   
  if(oiltemp > 110){
     screenTwo(oiltemp);
  } else {
    screenThree();
  }  
}

void screenOne() {
  display.clearDisplay();
  display.drawBitmap(0, 0,  porsche, 128, 32, WHITE);
  display.display();
}

void screenTwo(int oiltemp) {
  display.clearDisplay();
  display.setTextSize(2);             
  display.setTextColor(SSD1306_WHITE);        
  display.setCursor(10,10);             
  display.print("Oil: ");
  display.print(oiltemp);
  display.print((char)247);
  display.display();
}

void screenThree() {
  display.clearDisplay();
  display.display();
}
