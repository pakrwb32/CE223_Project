 /*
 Created byt Saroj Meesook
 https://www.youtube.com/watch?v=i5gw1-02DAk&t=188s
*/
#include <ESP8266WiFi.h>


const char* ssid = "KaewKanya";
const char* password = "05051214";
const char* host = "www.yourwebsite.com";
String path = "/yourpath/esp_gauge_valve.php";
String url = path;
String id = "node1";   // name of node
String value = "0";    // sensor data
String sw1 ="OFF";     // on/off valve or motor
String req ="1";       // 1= Request 0=send data
const int relay = 2;     // hardware output  contorl valve
const int activeHi =1; // for relay active hi
const int activeLo =0; // for relay active low
int    sensor =0;      // keep value of sensor
long   prevtime = 0;
long   currtimve= millis(); 
long   interval = 1000;

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(relay, OUTPUT);
  relay_ON_OFF(activeHi,relay,"OFF");
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  digitalWrite(2, HIGH);
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


String stat="OFF";
String line="";
String line2="";

void loop() {
  //control realy
  relay_ON_OFF(activeLo,relay,stat);
  delay(800);
  Serial.println(stat);
  value=readSensor();
  //Serial.println(value);
  sw1=stat;
  req="0";              // 1= Request 0=send data

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  // We now create a URI for the request
  url=path;
  url += "?id=" + id;
  url += "&req=" + req;
  url += "&sw1=" + sw1;
  url += "&value=" + value;
 
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  //url = path;
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  // Read all the lines of the reply from server
   while (client.available()) {
    line = client.readStringUntil('\n');
    //Serial.println(line);
    if(char(line[5])=='>'){
      line.trim();
      line2=line;
    }
  }
  stat=GetField(line2,2);
}
String readSensor(){
  String str="";
  sensor+=1;           //update sensor
  if(sensor>100){
     sensor = 0;
  }
  str=String(sensor);
  return(str);
}

void relay_ON_OFF(int act, int io, String stat){
   if(act==1){
           digitalWrite(io,(act & stat=="ON"));
   }
   else  {
           digitalWrite(io,(act | stat=="OFF")); 
    }        
}


String GetField(String str,int fcount){
 int i=0;
 char ch;
 int len=str.length();
 String str2="";
 while(fcount>=1 and i<len)
 {
  ch=str[i];
  //str2+=ch;
  if(ch!=',' & ch!=' ') {
    str2+=ch;
  }
  else
  {
   fcount--;
   if(fcount>0){
    str2="";
   }
  }
    i++;
 }
 return(str2);
}
