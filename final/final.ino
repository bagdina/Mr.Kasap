#include <Servo.h>
#include <SoftwareSerial.h>
Servo srvbtn1;
Servo srvbtn2;
char ch;
int y=20;
Servo srv;// create servo object to control a servo
int angle = 0; // variable to store the servo position
const int pwmPin = 3;
const int pot = A0; 
int c1 = 0;  
int c2 = 0;  
String opn ="open";
String knife ="knife";
String mv="move";
String SSID = "Nara";
String PASS = "bluebird";
boolean f=false;
SoftwareSerial wifiSerial(6,5); // RX, TX
void setup(){
  pinMode(pwmPin, OUTPUT); // declares pin 12 as output
  pinMode(pot, INPUT);  // declares pin A0 as input
  srvbtn1.attach(10);
  srvbtn2.attach(9); 
  srv.attach(8); // attaches the servo on pin 9 to the servo object



  Serial.begin(115200);
  Serial.println("ESP8266 Demo");
  wifiSerial.begin(115200);  
  wifiSerial.setTimeout(5000);
  delay(1000);
  //test if the module is ready
  wifiSerial.println("AT+RST");
  String responce = "";
  for(int i=0; i<100; i++){
    while(wifiSerial.available()){responce += (char)wifiSerial.read();}
    delay(10);
  }
  if(responce.indexOf("OK")>=0){
    Ser ial.println("Module is Ready!");
  }else{
    Serial.println("Module have no response.");
    while (1);
  }
  delay(1000);
  //connect to the wifi
  boolean connected = false;
  for (int i = 0; i < 5; i++){
    if (connectWiFi()){
      connected = true;
      break;
    }
  }
  if (!connected) {
    Serial.println("Error connecting to WIFI!");
    while (1);
  }
  delay(3000);
  wifiSerial.println("AT+CWDHCP=1,1");
  delay(2000);
  wifiSerial.println("AT+CIFSR");
  delay(1000);
  //print the ip addr
  Serial.println("ip address:");
  for(int i=0; i < 100; i++){
    while (wifiSerial.available()){Serial.write(wifiSerial.read());}
    delay(10);
  }
  //set the single connection mode
  wifiSerial.println("AT+CIPMUX=0");
  delay(1000);
  wifiSerial.println("AT+CWDHCP=1,1");
  delay(2000);
  wifiSerial.println("AT+CIFSR");
  delay(1000);
  wifiSerial.println("AT+CIPMUX=1");
  delay(1000);
  wifiSerial.println("AT+CIPSERVER=1,2222");
  delay(1000);

}
void loop(){
  c2= analogRead(pot);
  c1= 1024-c2;           
  digitalWrite(pwmPin, HIGH); // sets pin 12 HIGH
  delayMicroseconds(c1);   // waits for c1 uS (high time)
  digitalWrite(pwmPin, LOW);  // sets pin 12 LOW
  delayMicroseconds(c2);   // waits for c2 uS (low time)

  if(wifiSerial.available() && !f){
    String c = wifiSerial.readString();
    wifiSerial.println("AT+CIPSEND=0,152");
    if(c.substring((c.length()-4), (c.length())) ==opn){
      for(angle = 0; angle < 180; angle += 1){ // goes from 0 degrees to 180
        srvbtn2.write(angle); //tell servo to go to position in variable 'angle'
        delay(1); // waits 20ms between servo commands
      }
      for(angle = 180; angle >= 1; angle -= 1){ // goes from 180 degrees to 0
        srvbtn2.write(angle); 
        delay(1); 
      }
      Serial.println("Ok, the food is here");
      wifiSerial.println("Ok, the food is here");
    }
    if(c.substring((c.length()-5), (c.length())) ==knife){
      while(y!=0){
        for(angle = 0; angle < 90; angle += 1){ // goes from 0 degrees to 180
          srv.write(angle); //tell servo to go to position in variable 'angle'
          delay(2); // waits 20ms between servo commands
        }
        for(angle = 90; angle >= 1; angle -= 1){ // goes from 180 degrees to 0
          srv.write(angle); 
          delay(2); 
        }y=y-1;
      }
      Serial.println("bon appetite!");
      wifiSerial.println("bon appetite!");
    }y=20;
    f=true;
  }f=false;
  
}




boolean connectWiFi(){
  wifiSerial.println("AT+CWMODE=1");
  delay(100);
  String cmd = "AT+CWJAP=\"";
  cmd += SSID;
  cmd += "\",\"";
  cmd += PASS;
  cmd += "\"";
  Serial.println(cmd);
  wifiSerial.println(cmd);
  delay(3000);
  if (wifiSerial.find("OK")){
    Serial.println("OK, Connected to WiFi.");
    return true;
  } else{
    Serial.println("Can not connect to the WiFi.");
    return false;
  }
}
