# munin の作ったグラフを M5PAPER に表示する

[M5Stack Docs - The reference docs for M5Stack products.](https://docs.m5stack.com/#/en/quick_start/m5paper/quick_start_arduino) に書いてある通りに Arduino IDE をインストールして設定する。

プログラム `DISPLAY_URL_PNG.ino` を書き込む。

プログラムの内容は見ての通り↓です。

1. Wifi をつなぐ
2. `http://192.168.0.100/munin/` 以下から PNG をとってきて描画する
3. 4回繰り返す
4. 5分休んでで再起動

SSID PASSWORD URL は書き換える必要があります。

画像はこのリポジトリのほかのところに書いているようにやるとできているはずです。