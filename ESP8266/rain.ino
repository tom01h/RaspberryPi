#include <ESP8266WiFi.h>
#include <Wire.h>

//const char* ssid = "abcxyz";
//const char* password = "password123";

boolean Ini_html_on = false;//ブラウザからの初回HTTPレスポンス完了したかどうかのフラグ

#include "BH1750.h"
BH1750 lightMeter(0x23);

WiFiServer server(80);
WiFiClient client;

const unsigned short l_time = 60 * 3 / 5; // 3hour
const unsigned short m_time = 60 * 1 / 5; // 1hou idx;

volatile unsigned char count[l_time];    // max 255cps
volatile int idx;
volatile unsigned int time_int;

void ICACHE_RAM_ATTR signal() {
  if(millis() - time_int > 300){
    time_int = millis();
    count[idx] ++;
  }
}

void setup() {
  Serial.begin(115200);

  // Connect to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while(true);  // don't continue
  }

  IPAddress ip(192, 168, 0, 12);
  IPAddress gateway(192, 168, 0, 1);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress DNS(0, 0, 0, 0);

  bool connected = false;
  WiFi.config(ip, gateway, subnet, DNS);
  WiFi.mode(WIFI_STA);
  for( int i = 0; i < 10; i ++ ){
    WiFi.begin(ssid, password);
    delay(100);

    for( int j = 0; j < 100; j ++ ){
      delay(500);
      if(WiFi.status() == WL_CONNECTED ){
        connected = true;
        break;
      }
      Serial.print(".");
    }
    if( connected == true ){
      break;
    }
    Serial.println("Retry");
  }
  if( connected != true ){
    Serial.println("failed WiFi connect");
    while(true);  // don't continue
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  Wire.begin(D5, D6);
  lightMeter.begin();

  for(int i=0; i<l_time; i++)  count[i] = 0;
  idx = 0;
  
  pinMode(D7, INPUT);
  attachInterrupt(D7, signal, FALLING);
}

void loop() {
  if(Ini_html_on == false){
      Ini_HTTP_Response();
  }else if(client.available()){
    Serial.print(client.read());
  }
  delay(1);//これは重要かも。これがないと動作かも。
}

void Ini_HTTP_Response()
{
  double lux = 0.0;
  int l_cnt, m_cnt, s_cnt;
  int ini;

  client = server.available();//クライアント生成
  delay(1);
  String req;
  char l_s[16], m_s[16], s_s[16], lx[16];

  while(client){
    if(client.available()){
      req = client.readStringUntil('\n');
      Serial.println(req);
      if (req.indexOf("GET / HTTP") >= 0 || req.indexOf("GET /favicon") >= 0 || req.indexOf("GET /ini") >= 0){
        if(req.indexOf("GET /ini") >= 0){ini=1;}
        else                            {ini=0;}
        Serial.println("-----from Browser FirstTime HTTP Request---------");
        Serial.println(req);
        //ブラウザからのリクエストで空行（\r\nが先頭になる）まで読み込む
        while(req.indexOf("\r") != 0){
          req = client.readStringUntil('\n');//\nまで読み込むが\n自身は文字列に含まれず、捨てられる
          Serial.println(req);
        }
        req = "";
        delay(10);//10ms待ってレスポンスをブラウザに送信
        
        lux = lightMeter.readLightLevel();
        dtostrf(lux,5,2,lx);

        s_cnt = count[idx];
        m_cnt = 0;
        for(int i =0; i < m_time; i++){m_cnt += count[(idx-i+l_time)%l_time];}
        l_cnt = 0;
        for(int i =0; i < l_time; i++){l_cnt += count[i];}

        dtostrf(l_cnt*0.2794,5,2,l_s);
        dtostrf(m_cnt*0.2794,5,2,m_s);
        dtostrf(s_cnt*0.2794,5,2,s_s);

        client.print(F("HTTP/1.1 200 OK\r\n"));
        client.print(F("Content-Type:application/json; charset=utf-8\r\n"));
        client.print(F("\r\n\r\n"));
        client.print(F("{\r\n"));
        client.print(F("  \"id\": 1,\r\n"));

        client.print(F("  \"3 hour\": "));
        client.print(l_s);
        client.print(F(",\r\n"));

        client.print(F("  \"1 hour\": "));
        client.print(m_s);
        client.print(F(",\r\n"));

        client.print(F("  \"5 min\": "));
        client.print(s_s);
        client.print(F(",\r\n"));

        client.print(F("  \"light\": "));
        client.print(lx);
        client.print(F("\r\n"));
        client.print(F("}\r\n"));

        delay(1);//これが重要！これが無いと切断できないかもしれない。
        client.stop();//一旦ブラウザとコネクション切断する。

        Serial.println("\nGET HTTP client stop--------------------");
        req = "";
        Ini_html_on = false;  //一回切りの接続にしたい場合、ここをtrueにする

        if(ini){
          idx++;
          if(idx == l_time){idx=0;}
          count[idx] = 0;
        }
      }
    }
  }
}
