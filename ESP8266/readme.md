# ESP8266 でリモートセンサ

ESP8266 に BME280 と MH-Z19B と BH1750 を接続し、温度・湿度・気圧・CO2濃度・照度を測定して、Raspberry Pi 上の Munin のリモートセンサーとして動作させます。

今のところ `light.ino` は M5ATOM 用のコードです。

### BME280 のライブラリとサンプルコード

SparkFun のライブラリとサンプルコードを利用します。  
ライセンスは LICENSE_BME280.md を参照してください。  
オリジナルはいかにあります。

[BME280](https://github.com/sparkfun/SparkFun_BME280_Arduino_Library)

### MH-Z19B のライブラリとサンプルコード

ライセンスは LICENSE_MHZ を参照してください。  
オリジナルはいかにあります。  
このリポジトリのファイルは、オリジナルから変更を加えています。

[MH-Z19](https://github.com/tobiasschuerg/MH-Z-CO2-Sensors)

### BH1750 のライブラリとサンプルコード

ライセンスは LICENSE_BH1750 を参照してください。  
オリジナルはいかにあります。

[BH1750](https://github.com/claws/BH1750)

### Munin に追加する

[Munin からデータを読み出す方法](https://www.grezzo.co.jp/tech/archives/745) を真似して、ラズパイのリモートセンサーにします。  
ここの `esp8266_*.pl` を `/usr/local/bin` にコピーして、`../weather` の代わりにここの `temp, pres` などを `/usr/share/munin/plugins/` にコピーします。また、`munin-node` を `/etc/munin/plugin-conf.d/` にコピーします。

Prel にパッケージの追加が必要でした。

```
sudo cpan （yes/noを聞かれたら、とりあえずエンター)
install LWP
install JSON
exit
```

