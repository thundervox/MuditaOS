MuditaOS 開発資料
================

このフォルダには MuditaOS に関する開発資料があります。

## 開発者ガイド

- [開発環境](quickstart_ja-jp.md)
- [MuditaOS で対応しているビルドターゲット](build_targets.md)
- [MuditaOS の起動と更新](boot_and_update.md)
- [統合開発環境のセットアップ](setup_ide.md)
- [Mudita Pure シミュレーターの用法](howto_simulator_ja-jp.md)
- [Linux 版のキーバインド](host_keyboard_bindings_ja-jp.md)
- [MuditaOS 実機動作](running_on_phone.md)
- [Windows または macOS で Pure の実機にシステムイメージを書き込むには](flashing_win_macos.md)
- [テストハーネス](../test/README.md)
- [テザリング](tethering_ja-jp.md)
- [コーディング規約](MuditaCppCodingGuidelines.md)

## システム関連資料

以下は機能別によるシステム関連の開発資料です。

- システムアーキテクチャ
- モジュール
   - [アプリケーション](../module-apps/ModuleApps.md)
        - [アプリケーションデスクトップ](../module-apps/application-desktop/doc/README.md)
        - [通知](../module-apps/apps-common/notifications/README.md)
    - [オーディオ](../module-audio/README.md)
    - [Bluetooth](../module-bluetooth/README.md)
    - データベース
        - [クエリ](../module-db/queries/README.md)
        - [テーブル](../module-db/Tables/README.md)
        - [SMS と連絡先データベース](database_v2.md)
    - [ファイルシステム](../module-vfs/README.md)
    - [GUI](../module-gui/README.md)
    - [モデム](../module-cellular/modem/README.md)
    - [システム](../module-sys/README.md)
- [サービス](../module-services/ModuleServices.md)
    - [アプリケーションマネージャー](../module-services/service-appmgr/doc/README.md)
    - [Bluetooth](../module-services/service-bluetooth/doc/readme.md)
    - [セルラー](../module-services/service-cellular/doc/README.md)
    - [デスクトップ](../module-services/service-desktop/README.md)
    - [GUI](../module-services/service-gui/doc/README.md)
- ユーティリティ
    - [ログ記録エンジンの用法](../module-utils/log/doc/logging_engine.md)
- ツール
    - [MapParser](https://github.com/mudita/misc-tools/blob/master/mapparser/README.md)
    - [PureGDB](https://github.com/mudita/misc-tools/blob/master/puregdb/README.md)
- [システムの cmake 設定](ProjectConfig.md)
- [開発作業の流れ](development_workflow.md)
- [開発協力](../CONTRIBUTING.md)
- [国際化](i18n.md)

## 更新履歴
- [記載方法について](changelog_howto.md)
- [最新の更新履歴](../changelog.md)
