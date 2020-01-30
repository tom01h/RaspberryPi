# Raspberry Pi 3 Model A+ に Raspbian Stretch Lite をインストール

Wifi の使えるところでは Wifi 接続、使えないところでは PC とシリアル接続して使用します。  
USB-シリアル変換基板から電源を取っても動いていますが、供給能力が不足しているようで危険です。

Raspbian desktop と Windows10 を使って準備します。  
シリアル接続の設定以外は全体的に [ここの前半](https://tool-lab.com/raspberrypi-startup-8/) [後半](https://tool-lab.com/raspberrypi-startup-9/) がわかりやすいと思います。

### OS イメージのダウンロード

[公式のダウンロードサイト](https://www.raspberrypi.org/downloads/) からダウンロードする。  
今回は右側の Rasbian から Raspbian Stretch Lite を選びます。  
バージョンは September 2019 でした。

### Micro SD にデータをコピー

今回は 16GB の Micro SD カードを準備しました。  
データのコピーには Rasbian を使います。  
Micro SD は /dev/sdb でした。/dev/sdb? をアンマウントしてから、

```
$ unzip -p /…/2019-09-26-raspbian-buster-lite.zip | sudo dd bs=4M of=/dev/sdb conv=fsync
```

### シリアル接続の設定

続きは Windows10 での作業。  

Windows PC から見える boot パーティションに ```cmdline.txt, config.txt, ssh``` を準備する。(ssh は今はなくてもよい)

- config.txt の一番下に enable_uart=1 を追加
- cmdline.txt に console=serial0,115200 があることを確認
- 中身が空の ssh ファイルを追加

### Windows の設定

TeraTerm と USB-シリアル のドライバが必要です。  
私の PC には既に入っているので…

で、起動すると…

```
[    7.751852] Under-voltage detected! (0x00050005)
```

とりあえず動いているけど…

### 地域設定

```
$ sudo raspi-config
```

4 Localisation Options  - I1 Change Locale  
ja_JP.UTF-8 UTF-8 を選択  
Default locale を ja_JP.UTF-8 に設定

4 Localisation Options  - I2 Change Timezone  
Asia - Tokyo に設定

4 Localisation Options - I4 Change Wi-fi Country  
JP Japan に設定

### ネットワークの設定
```
$ sudo raspi-config
```
##### ホスト名の変更
2 Network Options - N1 Hostname にて新しい名前を設定
##### Wifi 接続
2 Network Options - N2 Wi-fi にて SSID とパスワードを入力  
この方法だとパスワードを平文で保存するみたいなので、wpa_passphrase 使ってちゃんと設定しましょう！

### ソフト更新

```
$ sudo apt update
$ sudo apt upgrade
```
