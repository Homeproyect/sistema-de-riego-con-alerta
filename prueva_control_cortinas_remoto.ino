#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>               
#include <WebServer.h>
#include <DHT.h>
#include <DHT_U.h>

Adafruit_SSD1306 display =Adafruit_SSD1306 (128, 64, &Wire);

//definimos los pines de los botones

#define start_btn 26
#define select_btn 18
#define up_btn 19
#define down_btn 23
#define Type DHT11
#define relay1 14
#define relay2 33
//conexion

const char* ssid ="NAVE_2_GX" ;      
const char* password =  "123456789";

     WebServer server(80);

//definimos las variables

bool state_start=0;
bool state_select=0;
bool state_up=0;
bool state_down=0;
bool mode=0;
bool modo=0;
bool abierto=0;
bool cerrado=0;
bool stop=0;
bool manual=0;
bool automatico=0;
float Humedad;
float temperatura;
int rango=20;

bool remote_start=0;
bool remote_select=0;
bool remote_up=0;
bool remote_down=0;

int dhtpin=4;
DHT HT(dhtpin, Type);

void setup() {
 //iniciamos cominicacion con el monitor serie
  Serial.begin(115200);
 //iniciamos cominicacion con el sensor de temperatura
  HT.begin();
 //punto de acceso
  WiFi.softAP(ssid, password);   
  IPAddress ip =WiFi.softAPIP();

server.on("/", handleConnectionRoot);
server.on("/ok",handleOk);
server.on("/select",handleSelect);
server.on("/suma",handleAumento);
server.on("/resta",handleDecremento);

server.onNotFound(handleNotFound);

server.begin();

 display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
 display.clearDisplay();
 display.setTextColor(SSD1306_WHITE);

 //difinimos las entradas como enttradas pulldown
  pinMode(start_btn, INPUT_PULLUP);
  pinMode(select_btn, INPUT_PULLUP);
  pinMode(up_btn, INPUT_PULLUP);
  pinMode(down_btn, INPUT_PULLUP);
  pinMode(relay1 , OUTPUT);  
  pinMode(relay2 , OUTPUT); 
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.println("  SELECT ");
  display.println("   PARA ");
  display.println(" INICIAR");
  display.setTextSize(1);
  display.println(ip);
   display.setTextSize(2);
  display.display();
 

}

void loop(){

readBtn();  
temperatura=HT.readTemperature();
Humedad=HT.readHumidity();
server.handleClient();
IPAddress ip =WiFi.softAPIP();
}
//control remoto
String device="";
String answer="<!DOCTYP html<html><head><meta charset='utf-8'><title>Servidor Local ESP32</title></head><body><center><h1>Nave 2 GX</h1><p><h1>Temperatura: "; 
String answer1="</h1></p><p><h1>rango:";
String answer2="</h1></p><p><a href='/ok'><button style='height:100px;width:200px'><h1>Ok</h1></button></a>\<a href='/select'><button style='height:100px;width:200px'><h1>Select</h1></button></a></p>\<p><a href='/suma'><button style='height:100px;width:200px'><h1>Up</h1></button></a>\<a href='/resta'><button style='height:100px;width:200px'><h1>Down</h1></button></a></p></center></body></html>";

void readBtn(){ 
 //menu pincipal
   if(digitalRead(select_btn)==LOW && state_select==0 && manual==0 || remote_select==1 && manual==0){
      state_select=1;

     if(mode==0){
        mode=1;
        modo=0;
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println(">Automa");
        display.println("Manual");
        display.display(); 
          remote_select=0;
        }
      
     else if(mode==1){
         mode=0;
         modo=1;
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Automa");
        display.println(">Manual");
        display.display();
     remote_select=0;
      }
      
   }
   //seleciion automatica
    if(digitalRead(start_btn)==LOW && state_start==0 && mode==1 && modo==0&& automatico==0 || remote_start==1 && mode==1&& automatico==0 &&modo==0){
    state_start=1;
   automatico=1;
   modo=0;
        display.clearDisplay();
        display.setCursor(0, 10);
        display.setTextSize(2);
        display.print("Temp=");
        display.println(temperatura);
        display.print("Hume=");
        display.println(Humedad);
        display.print("Rango=");
        display.println(rango);
        display.display();

        mod_automatico();
remote_start=0;
    
    }
    
    
   else if(digitalRead(select_btn)==HIGH && state_select==1 && automatico==1){
    state_select=0;
   }

if(mode==1 && automatico==1 && modo==0)
{


 mod_automatico(); 
 
    }
 if(digitalRead(up_btn)==LOW && state_up==0 && mode==1 && automatico==1 || remote_up == 1 && mode == 1 && automatico==1){
 state_up=1;
 
if(modo==0){
if(rango<40) rango++;


        display.clearDisplay();
        display.setCursor(10, 10);
        display.print("Rango= ");
        display.println(rango);
        display.display(); 
        delay(800);  
        display.clearDisplay();
        display.setCursor(0, 10);
        display.print("Temp=");
        display.println(temperatura);
        display.print("Hume=");
        display.println(Humedad);
        display.print("Rango=");
        display.println(rango);
        display.display();
        remote_up =0;
   }

}
else if(digitalRead(up_btn)==HIGH && state_up==1){
  state_up=0;
  }
    if (digitalRead(down_btn)==LOW && state_down == 0 && mode==1 || remote_down == 1 && mode == 1){
      state_down=1;
      
      if(modo==0){
        if(rango>0)  rango--;
        
        display.clearDisplay();
        display.setCursor(0, 10);
        display.print("Rango= ");
        display.println(rango);
        display.display();  
         delay(800);  
        display.clearDisplay();
        display.setCursor(0, 10);
        display.print("Temp=");
        display.println(temperatura);
        display.print("Hume=");
        display.println(Humedad);
        display.print("Rango=");
        display.println(rango);
        display.display(); 
        remote_down=0;
      }
 }
 else if (digitalRead(down_btn)==HIGH && state_down==1){
 state_down=0;}
//menu de manual

if(digitalRead(start_btn)== LOW && mode==0 && modo==1 && manual==0 || remote_start==1 && mode==0 && modo==1 && manual==0){
        state_start=1;
        abierto=1;
        manual=1;
        automatico=0;
        display.clearDisplay();
        display.setCursor(0, 10);
        display.println(">abierto");
        display.println("cerrado");
        display.println("stop");
        display.display(); 
        delay(200);
        remote_start=0;
 } else if (digitalRead(start_btn)==HIGH && state_start == 1 && manual==1){
      state_start=0; 
  }
  if(digitalRead(select_btn)== LOW && mode==0 && modo==1 && abierto==1 && manual==1 || remote_select==1 && mode==0 && modo==1 && abierto==1 && manual==1){
        state_select=1;
        abierto=0;
        cerrado=1;
        display.clearDisplay();
        display.setCursor(0, 10);
        display.println("abierto");
        display.println(">cerrado");
        display.println("stop");
        display.display(); 
     delay(200);
remote_select=0;
  } else if (digitalRead(select_btn)==HIGH && state_select == 1){
      state_select=0;}
  
 if(digitalRead(select_btn)== LOW && mode==0 && modo==1 && cerrado==1 && manual==1 || remote_select==1 && mode==0 && modo==1 && cerrado==1 && manual==1){
        state_select=1;
        cerrado=0;
        stop=1;
        display.clearDisplay();
        display.setCursor(0, 10);
        display.println("abierto");
        display.println("cerrado");
        display.println(">stop");
        display.display();
        delay(200);
        remote_select=0;
 } else if (digitalRead(select_btn)==HIGH && state_select == 1){
      state_select=0; 
}
if(digitalRead(select_btn)== LOW && mode==0 && modo==1 && stop==1 && manual==1 || remote_select==1 && mode==0 && modo==1 && stop==1 && manual==1){
        state_select=1;
        abierto=1;
        stop=0;
        display.clearDisplay();
        display.setCursor(0, 10);
        display.println(">abierto");
        display.println("cerrado");
        display.println("stop");
        display.display(); 
        delay(200);
        remote_select=0;
 } else if (digitalRead(select_btn)==HIGH && state_select == 1){
      state_select=0; 
}
//seleccion de modo
 if(digitalRead(start_btn)== LOW && mode==0 && modo==1 && abierto==1 && manual==1 || remote_start==1  && mode==0 && modo==1 && abierto==1 && manual==1){
        state_start=1;
        manual=0;
        display.clearDisplay();
        display.setCursor(10, 10);
        display.println("abriendo.");
        display.display(); 
        digitalWrite(relay2,LOW);
        delay(50);
        digitalWrite(relay1,HIGH);
     delay(100);
remote_start=0;
  } else if (digitalRead(start_btn)==HIGH && state_start == 1){
      state_start=0;}

 if(digitalRead(start_btn)== LOW && mode==0 && modo==1 && cerrado==1 && manual==1 || remote_start==1 && mode==0 && modo==1 && cerrado==1 && manual==1){
        state_start=1;
        manual=0;
        display.clearDisplay();
        display.setCursor(10, 10);
        display.println("cerrando.");
        display.display();
        digitalWrite(relay1,LOW);
        delay(50);
        digitalWrite(relay2,HIGH);
        delay(100);
remote_start=0;
  } else if (digitalRead(start_btn)==HIGH && state_start == 1){
      state_start=0;}
       if(digitalRead(start_btn)== LOW && mode==0 && modo==1 && stop==1 && manual==1 || remote_start==1 && mode==0 && modo==1 && stop==1 && manual==1){
        state_start=1;
        manual=0;
        display.clearDisplay();
        display.setCursor(10, 10);
        display.println("stop.");
        display.display();
        digitalWrite(relay1, LOW);
        digitalWrite(relay2,LOW);
        delay(100);
        remote_start=0;
        }

   else if (digitalRead(start_btn)==HIGH && state_start == 1){
      state_start=0;}
}
void mod_automatico()
{
  if(temperatura>=rango+3){
    
    digitalWrite(relay2,LOW);
    delay(50);
    digitalWrite(relay1,HIGH);
    }
 else if(temperatura<=rango-3){
    digitalWrite(relay1,LOW);
    delay(50);
    digitalWrite(relay2,HIGH);
    }
 else if(temperatura>=rango-3 && temperatura<=rango+3){
    digitalWrite(relay1,LOW);
    delay(50);
    digitalWrite(relay2,LOW);
    }
     display.clearDisplay();
        display.setCursor(0, 10);
        display.print("Temp=");
        display.println(temperatura);
        display.print("Hume=");
        display.println(Humedad);
        display.print("Rango=");
        display.println(rango);
        display.display();
delay(200);
  }

  
  void handleConnectionRoot(){
  server.send(200, "text/html",answer+temperatura+answer1+rango+answer2);
  }
 void handleOk(){
  remote_start=1;
    handleConnectionRoot();
    
    }
    void handleSelect(){
  remote_select=1;
   handleConnectionRoot();
    
    }
  void handleAumento(){
  remote_up=1;
    handleConnectionRoot();
    
    }
    void handleDecremento(){
remote_down=1;
   handleConnectionRoot();
    
    }
   
    void handleNotFound(){
      server.send(404,"text/html","no funciona");
      }
