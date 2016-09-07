フハハエンジン
======================

## フハハエンジンってなんじゃらぽん

シンプルかつ軽量なマルチプラットフォームゲームエンジン。
androidとiosとブラウザで遊べるゲームが作れます。
ゲームのコアな部分はc言語で記述します。
個人制作用に最低限の機能しか実装していません。
あなたのゲームエンジンとして使うにはパワー不足だと思いますが、
ソースコードの参考に役立てていただけたら嬉しいです。

## 使用例

[ホッピンパペット](http://totetero.com/project/hoppingpuppet/index.html) ([ソースコード:git](https://github.com/totetero/hopping_puppet))
[グルリンダッシュ](http://gururin-dash.appspot.com) ([ソースコード:git](https://github.com/totetero/gururin2))

## ファイル構造

| ファイル/ディレクトリ名 | 概要 |
|------------------------|-------------|
| contents                   | 画像や音楽などのリソースを入れておくディレクトリ |
| src_client                 | ゲーム固有のソースコード |
| src_client/core            | ゲーム本体のソースコード |
| src_client/core/gameMain.c | エンジンライフサイクルのソースコード ゲームエンジンはここから始まる |
| src_client/core/engine     | ゲームでよく使う処理をまとめたソースコード |
| src_client/core/plugin     | 各プラットフォームと連携するためのソースコード |
| src_client/platform_*      | 各プラットフォーム固有のソースコード |
| src_platform               | 各プラットフォーム固有のソースコード 上記との違いは、プラットフォームのバージョンアップなどにより変化してしまう可能性が高いソースコード |
| src_server                 | web版動作確認用 |


## ビルド準備

本エンジンはmac上での開発を想定しております。

### android版の開発

* android SDK/NDKのインストール

### ios版の開発

* XCodeのインストール

### web版の開発

* emscriptenのインストール
* node.jsのインストール

## リソース管理

`${FUHAHAROOT}/contents`に画像などのリソースを入れます。

下記のmakeコマンドを実行します。

```bash
cd ${FUHAHAROOT}
make copy
```

すると必要な場所にリソースがコピーされます。

## デバッグビルド

### android版のビルド

下記のmakeコマンドを実行します。

```bash
cd ${FUHAHAROOT}
make android-debug
```

ビルドが成功すれば`${FUHAHAROOT}/src_platform/android/build/outputs/apk/android-all-release.apk`が生成されます。
adbなどを用いてandroid実機にインストールして確認してください。

### ios版のビルド

XCodeで`${FUHAHAROOT}/src_platform/ios/fuhaha.xcodeproj`を開きます。
XCodeの作法に従いビルドします。

* プロジェクトにソースコードを追加した場合は以下の手順に従います。
    * XCodeで`core`を削除し`${FUHAHAROOT}/src_client/core`を追加します。
    * XCodeで`platform_ios `を削除し`${FUHAHAROOT}/src_client/platform_ios `を追加します。
    * 削除する際は`Remove References`で、追加する際は`Create groups`を選択します。

### web版のビルド

下記のmakeコマンドを実行します。

```bash
cd ${FUHAHAROOT}
make web-debug
```

ビルドが成功すれば`${FUHAHAROOT}/src_platform/web/bin`以下に必要なファイルがまとまります。
動作確認をするには、下記のmakeコマンドを実行します。

```bash
cd ${FUHAHAROOT}
make web-node
```

すると`node.js`が立ち上がり http://127.0.0.1:8080/game.html でアクセスできます。

## ライセンス

MIT
