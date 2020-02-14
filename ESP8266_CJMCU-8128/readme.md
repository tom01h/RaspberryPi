# ESP8266 でリモートセンサ

今はまだ BMP280 だけ繋いだテスト

ESP8266 に CJMCU-8128 = CCS811 + SI7021 + BMP280 を接続し、温度・湿度・気圧・CO2濃度を測定して、Raspberry Pi 上の Munin のリモートセンサーとして動作させます。

### SparkFun のライブラリとサンプルコード

SparkFun のライブラリとサンプルコードを利用します。  
ライセンスは LICENSE.md を参照してください。  
オリジナルはいかにあります。

[BME280](https://github.com/sparkfun/SparkFun_BME280_Arduino_Library)

[CCS881](https://github.com/sparkfun/SparkFun_CCS811_Arduino_Library)

[Si7021](https://github.com/sparkfun/SparkFun_Si701_Breakout_Arduino_Library)

### Munin に追加する

[Munin からデータを読み出す方法](https://www.grezzo.co.jp/tech/archives/745) を真似して、ラズパイのリモートセンサーにします。← まだ

