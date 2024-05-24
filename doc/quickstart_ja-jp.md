開発環境の構築
=============
* [はじめに](#introduction)
   * [リポジトリと関連モジュールのダウンロード](#download-repository-and-submodules)
   * [依存関係のインストール](#install-dependencies)
      * [ブートストラップ](#bootstrap)
      * [JLink ソフトウェア](#jlink-software)
      * [GitHub トークンの追加](#add-github-token)
      * [プロジェクトの構成](#project-configuration)
      * [プロジェクトのビルド](#project-build)
      * [プロジェクトのロードと実行](#project-load-and-run)
         * [Linux 版の実行方法](#running-on-linux)
         * [eMMC を用いた rt1051 版の実行方法](#running-on-rt1051-from-emmc)
   * [コミット前の確認](#checking-commits)
   * [CMake オプション](#cmake-options)
      * [UART によるログの取得](#catching-logs-using-uart)
* [ローカル環境の準備に関する詳細解説](#more-details-on-preparing-your-local-environment)
   * [git フックによるコードスタイルの検査](#code-style-with-git-hooks)
   * [コミットメッセージのテンプレート](#commit-message-template)
   * [コミットメッセージのフック](#commit-message-hook)
      * [フックの追加](#adding-a-hook)
      * [フックの用法](#using-a-hook)
* [Docker でのビルド方法](#build-using-docker)
   * [独自の Docker イメージのビルド](#building-your-own-docker-image)
* [パッケージの準備](#preparing-packages)

## はじめに

プロジェクトは以下の環境でビルドできます。

- Linux
- Windows の [WSL](https://docs.microsoft.com/en-us/windows/wsl/) 環境
- Docker イメージ

注意: Mac においてネイティブコンパイルは**非対応**です。

### リポジトリと関連モジュールのダウンロード

MuditaOS では関連モジュールを使用しますので、リポジトリのクローンを行ってください。この作業は数十GBの通信容量を消費します。通信回線の契約内容によっては高額請求の恐れがありますので、ご注意ください。
```
git clone --recurse-submodules git@github.com:mudita/MuditaOS.git
```

GitHub関連の登録が正常にされていないなどの理由で正常にクローンできない場合は https 接続をお試しください。

```
git clone --recurse-submodules https://github.com/mudita/MuditaOS.git
```

通信容量を節約したい場合は以下をお試しください。通信容量は約2.0GBです。最新のコミットの master ブランチのみ取得します。

```
git clone -b master --depth=1 --single-branch --recurse-submodules https://github.com/mudita/MuditaOS.git
```

`--recurse-submodules` を使わないか関連モジュールの更新を行う場合は、
```
git submodule update --init --recursive
```

### 依存関係のインストール

#### ブートストラップ

基本依存関係をインストールするには以下を実行します。
```
cd config && ./bootstrap.sh 0-
```

このスクリプト Ubuntu 用に開発したものであり、動作検証は Ubuntu 20.04 で行いました。

**注意:** スクリプトにおいて、
-sudo で実行する必要があるものは、
    - `get.docker.com` からシェルスクリプトをダウンロード後に sudo で実行して Docker のインストールをします。
    - 必要とされるパッケージ (そのリストは `cat config/bootstrap_config` で確認できます) を root 権限でインストールします。
- `GCCv10` と `CMake` を `$HOME` ディレクトリにインストールします。
- この作業は一度だけ実行する必要があります。

プロビジョニングの実行後はサポートしているプラットフォームのチェックアウトとプロジェクトのビルドに関する準備が完了します。

この `help` は `bootstrap.sh` に何も引数を渡さなければ表示できます。
```
[I] ⋊> ~/w/m/M/config on master ⨯ ./bootstrap.sh                                                                                                                                                                                                                                    09:00:01
test_if_run_as_root
Available build steps:
	0 install_hooks
	1 add_ignore_revs_for_blame
	2 install_ubuntu_packages
	3 setup_arm_toolchain
	4 setup_cmake
	5 setup_gcc_alternatives
	6 add_to_path ARM_GCC /home/pholat/gcc-arm-none-eabi-10-2020-q4-major/bin
	7 add_to_path CMAKE /home/pholat/cmake-3.21.3-linux-x86_64/bin
	8 install_docker
	9 add_to_docker_group
起動方法:
./bootstrap.sh <ビルド手順番号>[-]
用例:
./bootstrap.sh 1       - ビルド手順 1 の実行 (add_ignore_revs_for_blame)
./bootstrap.sh 3       - ビルド手順 3 の実行 (setup_arm_toolchain)
./bootstrap.sh 3-      - ビルド手順 3 〜最後まで実行
./bootstrap.sh 0-      - 全ビルド手順の実行
```

#### J-Link ソフトウェア

J-Link ドライバについては J-Link v634f ([Ubuntu 版のダウンロード](https://www.segger.com/downloads/jlink/JLink_Linux_V634f_x86_64.deb)) のバージョンを使用します。

**注意:** この手順は J-Link でソフトウェアのロードとデバッグをする場合に限り必要です。この機能はコミュニティ版では使えません。つまり、この機能を使うにはプログラマー用の開発基板、およびその基板にハンダ付けされたデバッグ用の接続端子が必要だからです。

**警告:** J-Link ソフトウェアの新しいバージョンでも正常に動作するものと錯覚するかもしれませんが、そのバージョンではハードウェアターゲットにおいてソフトウェアの正常なロードはできません。 

#### GitHub トークンの追加

GitHub トークンの構成方法は[アセットのダウンロードに関する解説](download_assets.md)をお読みください。

#### プロジェクトの構成

プロジェクトの構成には補助スクリプトである `./configure.sh` を使うことで各プラットフォームにおける製品ごとの基本的な cmake の構成を渡せます。
なお、 make よりもコンパイルの処理時間が短くなることが判明しているため `ninja` の利用を強く推奨します。
商用ファイルへアクセスせずに MuditaOS をビルドするには、 cmake に `-DASSETS_TYPE=Community` フラグを指定してください。

このスクリプトは以下の引数を指定して実行できます。
```
# コマンド      # 製品              # プラットフォーム     # ビルド形態
./configure.sh [PurePhone|BellHybrid] [rt1051|linux] [release|debug|relwithdebinfo] [ここには cmake の追加引数とオプションを指定します。例えば、 -DENABLE_DEVELOPER_MODE_ENDPOINT=1 または "-G Ninja"]
```

__用例:__

1. rt1051 プラットフォーム版の PurePhone を ninja でビルド

**備考:** ソフトウェアの規模による制限のため rt1051 ターゲットではフルデバッグビルドは扱えません。代わりに relwithdebinfo オプションをお使いください。

```
./configure.sh pure rt1051 relWithDebInfo "-G Ninja"
```

2. Linux プラットフォーム版の PurePhone を make でビルド

```
./configure.sh pure linux debug
```

3. Linux プラットフォーム版の Bell を ninja でビルド

```
./configure.sh bell linux debug "-G Ninja"
```

4. Linux プラットフォーム版の Bell を ninja とコミュニティ版のアセットのみでビルド

```
./configure.sh bell linux debug -DASSETS_TYPE=Community "-G Ninja"
```

#### プロジェクトのビルド

`configure.sh` の実行ごとに、  `build-{製品名}-{プラットフォーム}-{最適化レベル}` フォルダを作成します。例えば、 `build-PurePhone-rt1051-RelWithDebInfo` です。
1. 選択したビルドカタログを記入します。
2. 選択時の構成に従い `ninja` か `make` を実行します。
3. これで選択したターゲットのコンパイルとイメージを作成します。

ビルドターゲットについて詳しく知りたいのでしたら、[ビルドターゲットに関する解説資料](build_targets_ja-jp.md)を参照してください。

**警告:** MuditaOS のソースコードはオープンソースです。それだけではなく、このプロジェクトをコミュニティに受け入れられるようにするための作業を進めています。現在の進捗状況は、 

1. ソフトウェアのバイナリはビルドできます。
2. イメージの作成はできません。理由としてサードパーティー製の商用アセットを切り離して配布することは困難です。

**備考:** ccache をインストールして使用するとコンパイル処理速度の向上になります。

#### プロジェクトのロードと実行

プロジェクトのロードと実行方法はプラットフォームにより異なります。現在、  Linux と rt1051 ターゲットのみ対応しています。

**備考:** オープンソースライセンスのアセットに関する追加作業は現在進行中ですが数週間以内に完了予定です。現在、フォントと LUT に必要なファイルが不足しているため、追加作業が終わるまでオープンソースコミュニティ側での独自イメージはビルドできません。このサポートは段階的に追加する予定です。
- オープンソースライセンスのフォントを追加
- オープンソース開発貢献者向けの未署名更新パッケージの追加

##### Linux 版の実行方法

default ターゲットのビルド後に `./PurePhone.elf` を実行すると phone ターゲットに、あるいは `./BellHybrid.elf`  を実行すると BellHybrid ターゲットを起動します。

##### eMMC を用いた rt1051 版の実行方法

[rt1051 プラットフォーム版の実行方法](running_on_phone.md)を参照してください。

### コミット前の確認

コミット時には以下のすべての条件を満たす必要があります。

 * 1. コミットフック前に確認すること。
    * 1.1  Mudita 社指定のライセンスでコードの著作権を保護する必要があります。
    * 1.2 リポジトリで指定されている clang の書式にコードが準拠する必要があります。
    * 1.3 ソフトウェア汚染の原因となるバイナリデータ (binary blobs) の追加状況をコードで検査する。
 * 2. コミットで受け入れ可能な形式であるかマージ前の CI ジョブで検査する。

詳しくは[開発作業の流れについて](development_workflow.md)を参照してください。

### CMake オプション

`./configure.sh` でプロジェクトの構成をする前にご自身の要望に応じてオプションの有効化・無効化をするとプロジェクトの微調整がてきます。

この設定するには、 `.cmake` ファイルの手動編集 (非推奨)、または `ccmake` などのCLI/GUI ツールを使います。

| オプション                        | 概要                                                               | デフォルト値 |
| ------------------------------|:-------------------------------------------------------------------------:|:-------------:|
| `COVERAGE_ENABLE`             | コードカバレッジのレポート生成を有効化                                    | OFF           |
| `COLOR_OUTPUT`                | RTT ログとコンパイラに診断メッセージで色分け出力を使用                   | ON            |
| `SYSTEMVIEW`                  | Segger 社製 SystemView の使用を有効化                                       | OFF           |
| `USBCDC_ECHO`                 | USB-CDC からのエコー出力を有効化                                           | OFF           |
| `MUDITA_USB_ID`               | Mudita 社の登録済み USB ベンダー ID および Pure Phone の USB プロダクト ID を有効化| OFF           |
| `ENABLE_APP_X`                | 指定されたアプリケーション X のビルドと有効化                                        | ON            |
| `OPTIMIZE_APP_X`              | デバッグビルドでアプリケーション X を最適化                                     | ON            |
| `LINUX_ENABLE_SANITIZER`      | Linux版でアドレスのサニタイズを有効化                                        | OFF           |
| `ENABLE_SECURE_BOOT`          | セキュアブート用の署名付きバイナリをビルド                                       | OFF           |
| `THIRD_PARTY_DEBUG_OPTIMIZE`  | デバッグビルドでサードパーティー製ライブラリを最適化                                             | ON            |
| `ENABLE_TEST_LOGS`            | ユニットテストでログの取得を有効化                                                 | OFF           |
| `GENERATE_STACK_USAGE`        | スタックの使用状況に関するレポートを生成                                               | OFF           |
| `BUILD_DOC_WITH_ALL`          | `all` ターゲットでドキュメントのビルド                                     | OFF           |
| `SYSTEM_PROFILE`              |  MuditaOS にプロファイル機能を追加                             | OFF           |
| `WITH_DEVELOPMENT_FEATURES`   | USB 接続によるテストハーネスなどの全開発機能を有効化       | OFF           |

`ENABLE_APP_X` (`X` はアプリケーション名) でアプリケーションの有効化と無効化を指定できます。

CMake 全体のシステム設定に関しては[プロジェクトの構成](ProjectConfig_ja-jp.md)を参照してください。

#### UART によるログの取得

Mudita Pure のログを UART から取得したいときは以下の手順に従ってください。

1. `config/ProjectConfig.cmake` の `RTT_JLINK` を `RTT_LUART` へ変更
2. プロジェクトのリビルド
3. ログの取得: `picocom /dev/ttyACM0 -b 115200  --imap lfcrlf`

UART を用いたログの取得は高負荷であるため、電話機本体の動作速度が遅くなる恐れがあることに注意してください。この悪影響を防ぐには `release` ビルドのテストでは `uart` の有効化を検討してくだい。

## ローカル環境の準備に関する詳細解説

### git フックによるコードスタイルの検査

`bootstrap.sh` スクリプトはコードスタイルの検査で用いる git フックをインストールします。 `pre-commit.hook` はコミット時にスタイルを自動更新をします。 `bootstrap.sh` を実行していない状態で git フックを使うときは `pre-commit.hook` を git の設定ディレクトリである `.git/config/hooks` へコピー、または `ln -s ` pwd`/config/pre-commit.hook .git/hooks/pre-commit` でシンボリックリンクを作成します。

デフォルトでは、変更時にスタイルとして正しいかをコミットフックで検査するだけですので、 `git commit` の実行時にスタイルの自動修正を適用する場合は `user.fixinstage` 変数を新規追加してその値を
`true` に設定してから `git config user.fixinstage true` を実行することで git の設定をします。

作業方法として「通知後に修正 (変更内容の確認がしたい)」するほうが好ましいならば、デフォルトのフックによる動作 (通知用) を用いて `./config/pre-commit.hook --fix` を呼び出すことで「ステージ」にあるファイルの検査と修正をします。ただし、「変更済み」の状態にあるファルはテストから除外します。

```bash
git commit 
<stele error - commit aborted>
./config/pre-commit.hook --fix
git diff
git add -u
git commit
<commit accepted>
```

プルリクエスト時に CI でスタイルを修正するには、

`./config/pre-commit.hook --branch-fix`

コードスタイルの修正には clang を使います。

### コミットメッセージのテンプレート

コミットメッセージのテンプレートを追加するには、このコマンドを使います。

```bash
git config commit.template .gitmessage
```

これにより新規コミットの度に正しいメッセージの書き方に従った内容の記入を促すために用意したテンプレートを挿入します。詳しい解説は[開発作業の流れについて](development_workflow.md#commit-changes)を参照してください。

### コミットメッセージのフック
このフックはブランチ名からコミットタイトルへ自動変換します。

変換規則として、 EGD-xxxx で始まるブランチならば jira チケットとして扱います。その場合、 `-` と `_` はすべて半角空白文字に変換します。

#### フックの追加
```bash
cd .git/hooks
ln -s ../../config/prepare-commit-msg
```
#### フックの用法

ブランチの作成

```bash
git checkout origin/master -b EGD-1234-Add_change_description
```

変更後にコミットの準備

```bash
git commit
```

## Docker でのビルド方法
MuditaOS は Docker コンテナでのビルドに対応しています。このビルド方法は、以下の手順で行います。

1. Docker を入手するには `./config/bootstrap.sh 8` の実行、あるいは[手動でインストール](https://www.docker.com/get-started)します (以前に `./config/bootstrap.sh 0-` でインストールしなかった場合)。

ログインの処理段階でグループの設定を行う関係でインストールの完了後はログアウトと再ログインをしてください。ユーザのアカウントが docker グループに所属しているか確認するには `groups` コマンドを実行します。これにより、ユーザが所属しているグループのリストを表示します。

2. Linux 向けデバッグ版の構成
```bash
./in_docker.sh config PurePhone|BellHybrid linux Debug
```

3. Linux 向けデバッグ版のビルド
```bash
./in_docker.sh make build-PurePhone|BellHybrid-linux-Debug
```

4. RT1051 向けリリース版のビルド
```bash
./in_docker.sh config PurePhone|BellHybrid rt1051 Release
./in_docker.sh make build-PurePhone|BellHybrid-rt1051-Release
```
5. ビルド後にテストを実行 (Linux 専用)

```bash
./in_docker.sh make build-PurePhone|BellHybrid-linux-Debug
./in_docker.sh make build-PurePhone|BellHybrid-linux-Debug check
```

Docker イメージに接続した作業ディレクトリにて選択したテストをビルドするには以下を実行します。

```bash
cd ./build-PurePhone|BellHybrid-linux-Debug ; make <テスト名>; ./<テスト名>
```
用例
```bash
cd ./build-PurePhone|BellHybrid-linux-Debug ; make unittest_gui && ./unittest_gui
```

テストの実行時はメモリリークも併せて検査します。これはウィジェットの新規開発時に便利でしょう。

### 独自の Docker イメージのビルド

なんらかの理由で独自の Docker イメージ (例えば新規依存関係のテスト) の作成が必要な場合は、以下の手順で作成します。

1. Docker ファイルの設定と必要なツールチェーンのパッケージをダウンロードします。 

```bash
./config/build_runner_docker
```

2. 新規 Docker イメージのビルド

```bash
docker build docker -f docker/Dockerfile -t "wearemudita/mudita_os_builder:latest"
```

注意点としてカスタムイメージのビルド時はタグ (`build_runner_docker` のデフォルトは `latest`) を指定する必要があります。一部のスクリプトでもタグ (`in_docker.sh` では `CONTAINER_TAG` など) の指定が必要です。未指定時、 Docker は Mudita 社製の純正イメージをダウンロードして使用します。

## パッケージの準備

アプリケーションの実行に必要なものまとめたパッケージが必要なときは [build_targets.md](build_targets.md) の解説をお読みください。
