#include <ESP8266WiFi.h>
#include <Wire.h>

//const char* ssid = "abcxyz";
//const char* password = "password123";

boolean Ini_html_on = false;//ブラウザからの初回HTTPレスポンス完了したかどうかのフラグ

#include "SparkFunBME280.h"
BME280 myBMP280;

WiFiServer server(80);
WiFiClient client;

void setup()
{
  Serial.begin(115200);

  // Connect to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while(true);  // don't continue
  }

  IPAddress ip(192, 168, 0, 15);
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

  Serial.println("Reading basic values from BME280");
  myBMP280.setI2CAddress(0x76);
  if (myBMP280.beginI2C() == false) //Begin communication over I2C
  {
    Serial.println("The sensor did not respond. Please check wiring.");
    while(1); //Freeze
  }
}

void loop()
{
  if(Ini_html_on == false){
      Ini_HTTP_Response();
  }else if(client.available()){
    Serial.print(client.read());
  }
  delay(1);//これは重要かも。これがないと動作かも。
}

void Ini_HTTP_Response()
{
  double temperature = 0.0, pressure = 0.0;

  client = server.available();//クライアント生成
  delay(1);
  String req;
  char temps[16], presss[16];

  while(client){
    if(client.available()){
      req = client.readStringUntil('\n');
      Serial.println(req);
      if (req.indexOf("GET / HTTP") >= 0 || req.indexOf("GET /favicon") >= 0){
        Serial.println("-----from Browser FirstTime HTTP Request---------");
        Serial.println(req);
        //ブラウザからのリクエストで空行（\r\nが先頭になる）まで読み込む
        while(req.indexOf("\r") != 0){
          req = client.readStringUntil('\n');//\nまで読み込むが\n自身は文字列に含まれず、捨てられる
          Serial.println(req);
        }
        req = "";
        delay(10);//10ms待ってレスポンスをブラウザに送信

        temperature = myBMP280.readTempC();
        pressure = myBMP280.readFloatPressure()/100;

        dtostrf(temperature,5,2,temps);
        dtostrf(pressure,5,2,presss);

        client.print(F("HTTP/1.1 200 OK\r\n"));
        client.print(F("Content-Type:application/json; charset=utf-8\r\n"));
        client.print(F("\r\n\r\n"));
        client.print(F("{\r\n"));
        client.print(F("  \"id\": 1,\r\n"));
        client.print(F("  \"temperature\": "));
        client.print(temps);
        client.print(F(",\r\n"));
        client.print(F("  \"pressure\": "));
        client.print(presss);
        client.print(F("\r\n"));
        client.print(F("}\r\n"));

        delay(1);//これが重要！これが無いと切断できないかもしれない。
        client.stop();//一旦ブラウザとコネクション切断する。

        Serial.println("\nGET HTTP client stop--------------------");
        req = "";
        Ini_html_on = false;  //一回切りの接続にしたい場合、ここをtrueにする

        Serial.print(" Temp: ");
        Serial.print(temperature, 2);
        Serial.print(" Pressure: ");
        Serial.print(pressure, 2);
        Serial.println();
      }
    }
  }
}
