#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <DHT.h>

const char* ssid = "KMMM";
const char* password = "piast425";
int count;
const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";

#define trigPin 12
#define echoPin 13
#define DHTPIN 14 
#define yellowLED 2
#define blueLED 4
#define greenLED 15
#define redLED 27
#define button 32
DHT dht;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readDHTTemperature() {
  int t = dht.getTemperature();
  if (isnan(t) || t == 2147483647) {    
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}

String readDHTHumidity() {
  int h = dht.getHumidity();
  if (isnan(h) || h == 2147483647) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}

int distance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  int d = pulseIn(echoPin, HIGH)/58;
  return d;
}

int counter(int count){
  if(digitalRead(button) == LOW){
    count = count + 1;
    Serial.print("licznik ");
    Serial.println(count);
  }
  return count;
}

String readDistance(int d = distance()) {
  if (isnan(d)) {
    Serial.println("Failed to read from HC_SR04 sensor!");
    return "--";
  }
  else {
    Serial.println(d);
    return String(d);
  }
}

String signaling(int d = distance()){
  if (isnan(d)) {
    Serial.println("Failed");
    return "--";
  }
  else {
    if(d < 20){
      digitalWrite(redLED, HIGH);
      return "ALARM";
    }
    else{
      digitalWrite(redLED, LOW); 
      return "Brak alarmu";
    }
  }
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" 
      href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" 
      integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" 
      crossorigin="anonymous">
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css" 
      rel="stylesheet" 
      integrity="sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC" 
      crossorigin="anonymous">
    <style>
      h4 {
      text-align: center;
      }
      .switch {position: relative; display: center; width: 120px; height: 68px} 
      .switch input {display: center}
      .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 6px}
      .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 3px}
      input:checked+.slider {background-color: #b30000}
      input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
    </style>
  </head>

  <body>
    <div class="container">
        <h2 class="d-flex justify-content-center">Centrum kontroli inteligentnego domu</h2>
        <div class="container">
          <div class="row">
            <div class="col">
              <p class="d-flex justify-content-center">
                <i class="fas fa-ruler" style="color:#996515;"></i> 
                <span class="dht-labels">Distance&nbsp;</span>
                <span id="distance">%DISTANCE% </span>
                <span>&nbsp;cm</span>
              </p>

              <p class="d-flex justify-content-center">
                 
                <span class="dht-labels">Status:&nbsp;</span>
                <span id="buzzer">%BUZZER% </span>
                <span>&nbsp;!</span>
              </p>

	            <p class="d-flex justify-content-center">
               %BUTTONPLACEHOLDER%
              <script>
                function toggleCheckbox(element) {
                var xhr = new XMLHttpRequest();
                if(element.checked){ xhr.open("GET", "/update?output="+element.id+"&state=1", true); }
                else { xhr.open("GET", "/update?output="+element.id+"&state=0", true); }
                xhr.send();
                }
              </script>
              </p>

            </div>
          <div class="col">
            <p class="d-flex justify-content-center">
              <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
              <span class="dht-labels">Temperature&nbsp;</span> 
              <span id="temperature">%TEMPERATURE%</span>
              <span>&nbsp;&deg;C</span>
            </p>
            <p class="d-flex justify-content-center">
              <i class="fas fa-tint" style="color:#00add6;"></i> 
              <span class="dht-labels">Humidity&nbsp;</span>
              <span id="humidity">%HUMIDITY% </span>
              <span>&nbsp;&percnt;</span>
            </p>
          </div>
        </div>
      </div>
    </div>
  </body>
  <script>
  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("temperature").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/temperature", true);
    xhttp.send();
  }, 2000 ) ;

  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("humidity").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/humidity", true);
    xhttp.send();
  }, 2000 ) ;

  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("distance").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/distance", true);
    xhttp.send();
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("buzzer").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/buzzer", true);
    xhttp.send();
  }, 1000 ) ;
  
  </script>
</html>)rawliteral";

String outputState(int output){
  if(digitalRead(output)){
    return "checked";
  }
  else {
    return "";
  }
}

// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return readDHTTemperature();
  }
  else if(var == "HUMIDITY"){
    return readDHTHumidity();
  }
  else if(var == "DISTANCE"){
    return readDistance(distance());
  }
  else if(var == "BUZZER"){
    return signaling(distance());
  }
  else if(var == "BUTTONPLACEHOLDER"){
    String buttons = "";
    buttons += "<h4>Green LED</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"15\" " + outputState(greenLED) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Blue LED</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"4\" " + outputState(blueLED) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Yellow LED</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"2\" " + outputState(yellowLED) + "><span class=\"slider\"></span></label>";
    return buttons;
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  dht.setup(DHTPIN);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  pinMode(button, INPUT_PULLUP);
  pinMode(yellowLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(greenLED , OUTPUT);
  pinMode(redLED, OUTPUT);

  digitalWrite(yellowLED, LOW);
  digitalWrite(blueLED, LOW);
  digitalWrite(greenLED, LOW); 
  digitalWrite(redLED, LOW);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTHumidity().c_str());
  });
    server.on("/distance", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDistance(distance()).c_str());
  });
    server.on("/buzzer", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", signaling(distance()).c_str());
  });

    // Send a GET request to <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1;
    String inputMessage2;
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
    }
    else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
    Serial.print("GPIO: ");
    Serial.print(inputMessage1);
    Serial.print(" - Set to: ");
    Serial.println(inputMessage2);
    request->send(200, "text/plain", "OK");
  });

  // Start server
  server.begin();
}

void loop(){
}