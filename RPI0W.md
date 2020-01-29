# Raspberry Pi Zero W に Raspbian Stretch Lite をインストール

Wifi の使えるところでは Wifi 接続、使えないところでは USB 接続で PC と ssh 接続して使用します。

Raspbian desktop と Windows10 を使って準備します。  
USB 接続の設定は [ここ](https://qiita.com/Liesegang/items/dcdc669f80d1bf721c21)  の 1.2 パソコンからSSH接続しよう を参考にしています。  
他、全体的に [ここの前半](https://tool-lab.com/raspberrypi-startup-8/) [後半](https://tool-lab.com/raspberrypi-startup-9/) がわかりやすいと思います。

### OS イメージのダウンロード

[公式のダウンロードサイト](https://www.raspberrypi.org/downloads/) からダウンロードする。  
今回は右側の Rasbian から Raspbian Stretch Lite を選びます。  
バージョンは September 2019 でした。

### Micro SD にデータをコピー

今回は 16GB の Micro SD カードを準備しました。  
データのコピーには Rasbian を使います。  
Micro SD は /dev/sda でした。/dev/sda1 をアンマウントしてから、

```
$ sudo dd bs=4M if=/media/pi/…/2019-09-26-raspbian-buster-lite.img of=/dev/sda conv=fsync
```

### USB 接続の設定

続きは Windows10 での作業。  

Windows PC から見える boot パーティションに ```cmdline.txt, config.txt, ssh``` を準備する。

- config.txt の一番下に dtoverlay=dwc2 を追加
- cmdline.txt の rootwait の後に modules-load=dwc2,g_ether を追加
- 中身が空の ssh ファイルを追加

### Windows にドライバを追加

PC に接続すると USB シリアルデバイスとして認識されます。  
[RPI Driver OTG](https://caron.ws/wp-content/uploads/telechargement/RPI%20Driver%20OTG.zip) からダウンロードしたファイルを、デバイスマネージャ - ポート - USB シリアルデバイス を右クリックして、ドライバーの更新 - コンピューターを参照して… に指定してインストールする。  
USB Ethernet/RNDIS Gadget デバイスに変わり、ssh 接続可能になります。  
TeraTerm から raspberrypi.local で ssh ログインします (user:pi / pass:raspberry)。

### インターネット接続

設定 - ネットワークとインターネット - アダプタオプションを変更する を選んで、ネットワーク接続を開く。  
USB Ethernet/RNDIS Gadget の名前 (家では イーサネット2) を確認し、インターネットに接続している名前 (家では Wi-Fi) を右クリックしてプロパティ選択、イーサネットのプロパティ - 共有 - ネットワークのほかのユーザーに…許可する を選択して、ホームネットワーク接続に、先ほど確認した名前 (イーサネット2) を指定する。  
次のコマンドが通れば成功。

```
$ ping -c 4 8.8.8.8
```

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

### ホスト名の変更

```
$ sudo raspi-config
```

2 Network Options - N1 Hostname にて新しい名前を設定

### ソフト更新

```
$ sudo apt update
$ sudo apt upgrade
```

### Wifi の設定

```
$ sudo raspi-config
```

2 Network Options - N2 Wi-fi にて SSID とパスワードを入力

この方法だとパスワードを平文で保存するみたいなので、wpa_passphrase 使ってちゃんと設定しましょう！