#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

#include <DNSServer.h>
#include <AsyncElegantOTA.h>

#include "ESPPin.h"


const char* PARAM_INPUT = "value";

DNSServer dnsServer;
AsyncWebServer server(80);

String sliderValue1 = "0";
String sliderValue2 = "0";
String sliderValue3 = "0";
String sliderValue4 = "0";

int valueSlider1 = 0;
int valueSlider2 = 0;
int valueSlider3 = 0;
int valueSlider4 = 0;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .slider2 { -webkit-appearance: none; margin: 14px; width: 360px; height: 25px; background: #FFD65C; outline: none; -webkit-transition: .2s; transition: opacity .2s;}
    .slider2::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 35px; height: 35px; background: #003249; cursor: pointer;}
    .slider2::-moz-range-thumb { width: 35px; height: 35px; background: #003249; cursor: pointer; }
  </style>
</head>
<body>
  <h2>AA Chanel Controller</h2>
    <p><span id="textSliderValue1">%SLIDERVALUE1%</span></p>
    <p><input type="range" onchange="updateSliderPWM1(this)" id="pwmSlider1" min="0" max="1023" value="%SLIDERVALUE1%" step="1" class="slider2"></p>
    <p><span id="textSliderValue2">%SLIDERVALUE2%</span></p>
    <p><input type="range" onchange="updateSliderPWM2(this)" id="pwmSlider2" min="0" max="1023" value="%SLIDERVALUE2%" step="1" class="slider2"></p>
    <p><span id="textSliderValue3">%SLIDERVALUE3%</span></p>
    <p><input type="range" onchange="updateSliderPWM3(this)" id="pwmSlider3" min="0" max="1023" value="%SLIDERVALUE3%" step="1" class="slider2"></p>
    <p><span id="textSliderValue4">%SLIDERVALUE4%</span></p>
    <p><input type="range" onchange="updateSliderPWM4(this)" id="pwmSlider4" min="0" max="1023" value="%SLIDERVALUE4%" step="1" class="slider2"></p>
    
<script>
function updateSliderPWM1(element) {
  var sliderValue = document.getElementById("pwmSlider1").value;
  document.getElementById("textSliderValue1").innerHTML = sliderValue;
  console.log(sliderValue);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider1?value="+sliderValue, true);
  xhr.send();
}
function updateSliderPWM2(element) {
  var sliderValue = document.getElementById("pwmSlider2").value;
  document.getElementById("textSliderValue2").innerHTML = sliderValue;
  console.log(sliderValue);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider2?value="+sliderValue, true);
  xhr.send();
}
function updateSliderPWM3(element) {
  var sliderValue = document.getElementById("pwmSlider3").value;
  document.getElementById("textSliderValue3").innerHTML = sliderValue;
  console.log(sliderValue);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider3?value="+sliderValue, true);
  xhr.send();
}
function updateSliderPWM4(element) {
  var sliderValue = document.getElementById("pwmSlider4").value;
  document.getElementById("textSliderValue4").innerHTML = sliderValue;
  console.log(sliderValue);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider4?value="+sliderValue, true);
  xhr.send();
}
</script>


</body>
</html>
)rawliteral";


String processor(const String& var){
  if (var == "SLIDERVALUE1"){
    return sliderValue1;
  }else if (var == "SLIDERVALUE2"){
    return sliderValue2;
  }else if (var == "SLIDERVALUE3"){
    return sliderValue3;
  }else if (var == "SLIDERVALUE4"){
    return sliderValue4;
  }
  return String();
}

int flag1 = 0;
int flag2 = 0;
int flag3 = 0;
int flag4 = 0;

void setup(){
  WiFi.softAP("Control");   
  dnsServer.start(53, "*", WiFi.softAPIP());  

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/slider1", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValue1 = inputMessage;

      if(valueSlider1<sliderValue1.toInt()){
        flag1=1;
      }else{
        flag1=2;
      }
     }
     
    request->send(200, "text/plain", "OK");
  });
    
  server.on("/slider2", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValue2 = inputMessage;

      if(valueSlider2<sliderValue2.toInt()){
        flag2=1;
      }else{
        flag2=2;
      }
     }
 
    request->send(200, "text/plain", "OK");
  });
 
  server.on("/slider3", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;

    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValue3 = inputMessage;
      if(valueSlider3<sliderValue3.toInt()){
        flag3=1;
      }else{
        flag3=2;
      }
      
    }

    request->send(200, "text/plain", "OK");
  });

  server.on("/slider4", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;

    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValue4 = inputMessage;
      
      if(valueSlider4<sliderValue4.toInt()){
        flag4=1;
      }else{
        flag4=2;
      }
      
    }

    request->send(200, "text/plain", "OK");
  });

  
  AsyncElegantOTA.begin(&server);
  server.begin();


}

void loop() {
  if(flag1==1){
    for(int x = valueSlider1; x<=sliderValue1.toInt(); x++){
      PWM(A1, A2, x);
      delay(5);
     }
     valueSlider1 = sliderValue1.toInt();
     flag1 = 0;
  }
  if(flag1==2){
    for(int x = valueSlider1; x>=sliderValue1.toInt(); x--){
      PWM(A1, A2, x);
      delay(5);
     }
     valueSlider1 = sliderValue1.toInt();
     flag1 = 0;
  }
    if(flag2==1){
    for(int x = valueSlider2; x<=sliderValue2.toInt(); x++){
      PWM(B1, B2, x);
      delay(5);
     }
     valueSlider2 = sliderValue2.toInt();
     flag2 = 0;
  }
  if(flag2==2){
    for(int x = valueSlider2; x>=sliderValue2.toInt(); x--){
      PWM(B1, B2, x);
      delay(5);
     }
     valueSlider2 = sliderValue2.toInt();
     flag2 = 0;
  }
      if(flag3==1){
    for(int x = valueSlider3; x<=sliderValue3.toInt(); x++){
      PWM(B1, B2, x);
      delay(5);
     }
     valueSlider3 = sliderValue3.toInt();
     flag3 = 0;
  }
  if(flag3==2){
    for(int x = valueSlider3; x>=sliderValue3.toInt(); x--){
      PWM(C1, C2, x);
      delay(5);
     }
     valueSlider3 = sliderValue3.toInt();
     flag3 = 0;
  }
        if(flag4==1){
    for(int x = valueSlider4; x<=sliderValue4.toInt(); x++){
      PWM(C1, C2, x);
      delay(5);
     }
     valueSlider4 = sliderValue4.toInt();
     flag4 = 0;
  }
  if(flag4==2){
    for(int x = valueSlider4; x>=sliderValue4.toInt(); x--){
      PWM(C1, C2, x);
      delay(5);
     }
     valueSlider4 = sliderValue4.toInt();
     flag4 = 0;
  }
  dnsServer.processNextRequest(); 
}
