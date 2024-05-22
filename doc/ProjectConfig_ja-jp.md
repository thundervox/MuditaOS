構成オプションについて
====================

# ProjectConfig.cmake

## ログの取得

### デバイスのログ取得と構成

| LOG_USE_COLOR | 動作 |
|---:|---:|
| 0 | RTT ログメッセージの色分けをしない。 |
| 1 | RTT の色分けをします。なお、 SysView は解析(処理)しません。 | 

| LOG_REDIRECT | 動作 |
|---:|---:|
| RTT_JLINK | J-Link のシリアルから RTT ログを出力します。|
| RTT_LUART | RT1051 に内蔵された UART のシリアルからRTT ログを出力します。| 
| RTT_SYSTEMVIEW | J-Linkの SystemView から RTT ログを出力します。| 

| SYSTEM_VIEW_ENABLED | 動作 |
|---:|---:|
| 0 | SystemView は無効化とコードの出力をしません。|
| 1 | SystemView の有効化とコードの出力をします。 | 

### LOG_SENSITIVE_DATA

通常時は無効化されている個人情報に関するログの取得を有効化します。

## システムのトレース

## SystemView オプション

これは以下の理由から完璧と言えるソリューションとは言えません。

- RTT からの送信データ (および損失データ) は膨大な量になります。
-  CPU の動作周波数による負荷調整はありません。

とはいえ、 Segger 社製 SystemView はある程度の実用にはなります。 SystemView の使用時は  `SYSTEMVIEW` オプションを `ON` にすると有効になります。

### システムプロファイル

MuditaOS には最小構成のシステムプロファイリングツールがあります。この機能を有効化するには、 `SYSTEM_PROFILE` を `ON` にします。

システムのプロファイルデータは RTT の msgpack から送信されます。デコードと確認方法は、

- 動作周波数変更前における最新 100ms の負荷
- 動作周波数変更時間
- 期間内における消費電力量

このアプリ [MuditaOSTop](https://github.com/mudita/MuditaOSTop) でもログと同じように確認できます。 

あるいは、  `CPUStatistics.cpp` の出力先に関する設定を別の messagepack (ログなど) の出力先に変更します。

## CurrentMeasurement の有効化オプション

ダイレクトカレントポーリングでログを記録するには `CURRENT_MEASUREMENT` を `ON` にします。これにより [plot_current_measurement.py](../tools/plot_current_measurement.py) でプロットが使えるようになります。

# USB

## USB-CDC エコーのテスト

USB-CDC の動作をテストする場合は `USBCDC_ECHO` で USB-CDC エコーを設定します。

## USB MTP の有効・無効

- `ENABLE_USB_MTP` オプションは USB-MTP プロトコルを有効化します。デフォルトは `ON` です。

## Mudita 社 USB ベンダーとプロダクト ID 情報

- `MUDITA_USB_ID` オプションのデフォルトは `ON` です。

# lwext4 用の設定オプション

以下はサードパーティ製の ext4 ライブラリである [lwext4](third-party/lwext4/) 用の設定です。

## LWEXT4 デバッグオプション

- `LWEXT4_DEBUG_PRINTF` オプションのデフォルトは `ON` です。
- `LWEXT4_DEBUG_ASSERT` オプションのデフォルトは `OFF` です。

## LWEXT4 セクタのキャッシュサイズ

- `LWEXT4_CACHE_SIZE` オプションにおけるデフォルト値の設定は `256`  です。

# 開発機能の設定

- option `WITH_DEVELOPMENT_FEATURES` オプションは以下のシステム開発関連機能をすべてを有効化します。
    - `DEVELOPER_SETTINGS_OPTIONS_DEFAULT`
    - `ENABLE_DEVELOPER_MODE_ENDPOINT_DEFAULT`
    - `LOG_SENSITIVE_DATA_ENABLED`
デフォルトは `OFF`

# 商用版・コミュニティ版のビルド指定
- `ASSETS_TYPE` オプションのデフォルトは `Proprietary` です。

`Community` を指定するとコミュニティ版では利用できない商用版のファイルのダウンロードと関連テストを無効化します。
