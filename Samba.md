# Raspberry Pi をファイルサーバにする

Raspberry Pi に Samba をインストールして Windows 10 からネットワークアクセスできるファイルサーバにします。  
接続するディスクは NTFS フォーマットされたディスクです。  
[参考にしたサイト](https://qiita.com/Cinosura/items/6ab435331ea2b3671a1d) には無い情報もいくつか追加してまりますが、基本的にはリンク先のほうがわかりやすいと思います。

#### Windows と同じユーザを追加する

私の場合は tom01h を追加します。sudo 権限を与えます。

```
$ sudo adduser tom01h
$ sudo gpasswd -a tom01h sudo
```

#### ランレベルを設定する

```
$ systemctl get-default
graphical.target
$ sudo systemctl set-default multi-user.target
Created symlink from /etc/systemd/system/default.target → /lib/systemd/system/multi-user.target.
```

#### HDD(NTFS) をマウント

NTFS は繋ぐだけで認識されるようです。しかも、なぜか /dev/sda1 が未使用で /dev/sda2 を使っていました。  
なので、

```
$ sudo blkid /dev/sda2
```

で調べた UUID (うちのは 14F6EB08F6EAE8C6) を /etc/fstab に追加する。

```
UUID="14F6EB08F6EAE8C6" /mnt/hdd1 ntfs-3g defaults,nofail 0       0
```

#### HDDの自動停止

[さらに参考にしたサイトのリンク](http://kassyjp.ninja-web.net/ras/jessie/spindown.htm) をたどります。うちの環境では hdparm が動きました。

```
$ sudo apt-get install hdparm
$ sudo hdparm -y /dev/sda
```

リンク先に従い /root/hdd_spindown.sh を作成。パーミッションを設定する。

```
(hdd_spindown.sh 作成)
$ sudo chmod 700 /root/hdd_spindown.sh
```

次にデバイスの Unit 名を調べて、/lib/systemd/system/user_hdd_spindown.service を作成。  
その後登録。

```
$ systemctl list-units| grep -F .mount
(user_hdd_spindown.service 作成)
$ sudo systemctl enable user_hdd_spindown.service
```

#### Samba をインストール

```
$ sudo apt install samba
```

WINS 云々聞かれたけど「いいえ」を選びました。  
/etc/samba/smb.conf は [homes] の上に以下を追加して、

```
[nas]
   path = /mnt/hdd1
   read only = no
   force user = pi
   guest ok = yes
   unix extensions = no
```

以下を書き換えました。

```
   valid users = tom01h ; <- %S
```

#### Munin をインストール

ファイルサーバに Munin は必須ではないですが、リモートから状況を確認できると便利かと思いインストールしました。

Apache をインストールしてから、Munin をインストールする必要があるようです。

```
$ sudo apt install apache2
$ sudo apt install munin munin-node
```

/etc/munin/apache24.conf を変更

```
<Directory /var/cache/munin/www>
     Allow from all
     Require all granted
     Options None
</Directory>
```

ApacheにMuninを反映して再起動。

```
$ sudo ln -s /etc/munin/apache24.conf /etc/apache2/sites-enabled/munin.conf
$ sudo /etc/init.d/apache2 restart
```

Samba を Munin で監視するには

```
$ sudo ln -s /usr/share/munin/plugins/samba /etc/munin/plugins/samba
```

Windows から以下のアドレスでアクセス出来るようになりました。

```
http://rpi-server.local/munin/
```

