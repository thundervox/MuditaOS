# フォントの管理

## フォントの形式

 MuditaOS は独自バイナリ形式のフォント (拡張子 .mpf) を使用しています。
 
 変換ツールを使うことで 一般的なフォント形式 (.otf, .ttf など) から MuditaOS 独自フォント形式にフォントを変換できます。
 
https://github.com/mudita/fontbuilder

独自バイナリ形式のフォントを使う理由として、
- GPU ユニットが搭載されていないため CPU の負荷を削減。
- ディスクと RAM においてフォントの占有量を削減。

## フォントの配布元

フォントはアセット `download_assets.py` スクリプトで専用リポジトリからダウンロードします(参照: [アセットのダウンロードに関する解説](download_assets.md))。構成の異なる二種類のリポジトリから構成指定 (コミュニティ版・商用製品版)に従いフォントをダウンロードします。

## フォントマッピング

配布元に関わらず、どちらのアセットパッケージであっても MuditaOS のコードで使われるフォントタイプにはフォントファイルを割り当てるための fontmap.json ファイルがあります。

正式による用例は一般公開アセットのリポジトリにあります。
 https://github.com/mudita/MuditaOSPublicAssets/blob/master/image/assets/fonts/fontmap.json

略式による用例:
```
{
    "info" : {
        "fallback_font" : "dejavu_sans_bold_27",
        "default_font": "gt_pressura_bold_27",
        "default_font_family" : "gt_pressura"
    },
    "style": {
        "small":      "gt_pressura/gt_pressura_regular_24.mpf",
        ...
    }
}
```
`info` 部に記述する必要のある値は以下のとおりです。
- `fallback_font` は指定のフォントで表示できないグリフがある時に代替使用するフォールバック用のフォントです。
- `default_font` は指定のフォントがロードできない時に代替使用するデフォルトのフォントです。
- `default_font_family` はフォントのサイズとウェイトのみを指定するメソッドで使用するデフォルトのフォントファミリ名です。

MuditaOS のフォントリストで使用するフォントタイプは `style` 部のキーごとに定義します。読み込み後のフォントは`FontManager::getFont` メソッド (例えば、 `getFont("supersizemelight")`) で使用可能です。

## MuditaOS におけるフォントの用法

FontManager は `fontmap.json` の記載内容をもとにフォントの一括ロードをします。読み込み後のフォントは以下の専用 GUI メソッドからアクセスできます。

- `getFont(std::string fontType)` - フォントの種類 (例えば、 `verysmall`, `supersizemelight`) から得た情報によりフォントの `RawFont*` を返します。
- `getFontByName(std::string fontName)` - フォントディスクリプタ (例えば、 `dejavu_sans_regular_20`, `dejavu_sans_light_90`) から得たフォント名によりフォントの `RawFont*` を返します。
