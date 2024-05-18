Doxygen によるドキュメンテーションの生成
=============================================

**備考:** Doxygen ドキュメンテーションはターゲットの一種です。
基本的なターゲットに関するドキュメンテーションは
[こちら](build_targets.md)にあります。
ターゲットをビルドするための環境構築は [quickstart](quickstart_ja-jp.md) にあります。

 [Doxygen](https://www.doxygen.nl/index.html) を用いてローカルでビルドするための方法は Doxygen のドキュメンテーションに詳しい解説があります。

以下のコマンドでコードのドキュメンテーションをビルドできます。
```sh
make doc
```
常に all ターゲットでドキュメンテーションのビルドをするのであれば、 `cmake` の構成として `BUILD_DOC_WITH_ALL` オプションを `ON` に設定してください。例えば、
```sh
cmake -DBUILD_DOC_WITH_ALL=ON . build/
```

ビルドディレクトリ内の `doc/html` サブディレクトリにドキュメンテーションを生成します。このドキュメンテーションを閲覧するにはウェブブラウザで `index.html` を開きます。例えば、

```sh
firefox build/doc/html/index.html
```
