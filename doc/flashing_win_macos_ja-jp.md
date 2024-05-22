# Windows または macOS - Pure イメージの書き換え

1. Mudita Pure を USB MSC (Mass Storage Class) モードで実行します。

-  電話機の電源を落とした後にバッテリーを取り外します (書き換え時に装着する必要はありません)
- 中ボタンを押したままにします (選択)。
- USB ケーブルを接続します。
- テキストメニューが画面下部から現れるまで待ちます。
- 中ボタンを離します。
- `USB MSC モード` を選択します。

2. Windows または macOS で書き換えます。

- お使いのオペレーティングシステムに対応した [balenaEtcher](https://www.balena.io/etcher/) をウェブサイトからダウンロードします。 ダウンロード後のファイル名は `balenaEtcher-Setup-{バージョン}.exe` (Windows 版) または `balenaEtcher-{バージョン}.dmg` (MacOS 版) であり、コンピューターにアプリケーションをインストールするために使います。

![balenaEtcher_download](Images/balenaetcher/ss4.png)

- balenaEtcher アプリケーションをインストールします。
- 書き換え用の MuditaOS イメージのダウンロード後にローカル環境へ展開します。

**注意: 展開後のイメージ容量は 16 GB です。**

`.xz` 形式のイメージは `tar` と `xz` 形式で二重圧縮したものです。展開プログラムによっては `.tar` 形式のみの展開となるため `PurePhone.img` ファイルを取り出すには再度展開しなおしてください。

- balenaEtcher の起動後に  `Flash from file` からイメージの種類として ISO イメージを選択します。これで展開後のイメージはターゲットデバイスへロードされます。このような画面表示になります。

![balenaEtcher 画面のスクリーンショット](Images/balenaetcher/ss1.png)

![balenaEtcher 画面のスクリーンショット](Images/balenaetcher/ss2.png)

- ``Flash!`` を押した後にパスワードの入力を求められた場合はシステム管理者に問い合わせてください。
- 書き換えが完了するまでコンピューターやケーブルなどに手を触れず待機します。

![balenaEtcher 操作画面のスクリーンショット](Images/balenaetcher/ss3.png)

- 電話機を再起動します。

注意: 電話機を再起動するには、以下の手順で進めます。
* オペレーティングシステムからドライブをイジェクトします (Windows は `ハードウェアの安全な取り外し` を、 Linux では `ドライブの安全な取り外し`)。あるいは、
* 携帯電話を電源 (USB ケーブルとバッテリー) から取り外した後にそれまで使用していた電源に再接続すると再起動します。


