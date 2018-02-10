フハハエンジン
======================

## フハハエンジンってなんじゃらぽん

シンプルかつ軽量なマルチプラットフォームゲームエンジン。
androidとiosとブラウザで遊べるゲームが作れます。
ゲームのコアな部分はc言語で記述します。
個人制作用に最低限の機能しか実装していません。
あなたのゲームエンジンとして使うにはパワー不足だと思いますが、
ソースコードの参考に役立てていただけたら嬉しいです。

## TODO

NULL安全対応

## 使用例

[ホッピンパペット](http://totetero.com/project/hoppingpuppet/index.html) ([ソースコード:git](https://github.com/totetero/hopping_puppet))

[グルリンダッシュ](http://gururin-dash.appspot.com) ([ソースコード:git](https://github.com/totetero/gururin2))

## ファイル構造

| ファイル/ディレクトリ名 | 概要 |
|---------------------------------|---|
| contents                        | 画像や音楽などのリソースを入れておくディレクトリ |
| src_client/fuhahaEngine         | ゲームエンジンのソースコード |
| src_client/main                 | ゲーム本体のソースコード |
| src_client/main/core/gameMain.c | ゲームはここから始まる |
| src_client/platform_android     | android用プロジェクト |
| src_client/platform_ios         | ios用プロジェクト |
| src_client/platform_web         | web用プロジェクト |
| src_data                        | 加工前のリソース |
| src_server                      | web版動作確認用 |


## ビルド準備

本エンジンはdockerを用いた開発を想定しております。

### android/web版の開発

* dockerのインストール

#### docker開発環境の準備と開始

dockerはシェルスクリプトにより管理されます。
下記のシェルスクリプトコマンドを実行して開発環境を準備します。
時間がかかります。

```bash
cd ${FUHAHAROOT}
sh docker_builder.sh first
```

下記のシェルスクリプトコマンドを実行すれば準備した開発環境を立ち上げることができます。

```bash
cd ${FUHAHAROOT}
sh docker_builder.sh start
```

#### docker開発環境の停止と削除

下記のシェルスクリプトコマンドを実行すれば立ち上げた開発環境を止めることができます。

```bash
cd ${FUHAHAROOT}
sh docker_builder.sh stop
```

下記のシェルスクリプトコマンドを実行すれば開発環境を取り除いて綺麗な状態にすることができます。

```bash
cd ${FUHAHAROOT}
sh docker_builder.sh last
```

### ios版の開発

* XCodeのインストール

## リソース管理

`${FUHAHAROOT}/contents`に画像などのリソースを入れます。

## デバッグビルド

### web版のビルド

docker開発環境を立ち上げ、下記のシェルスクリプトコマンドを実行します。

```bash
cd ${FUHAHAROOT}
sh docker_builder.sh make web
```

ビルドが成功すれば`${FUHAHAROOT}/src_client/platform_web/bin`以下に必要なファイルがまとまります。
同時にdocker上で`node.js`が立ち上がり http://127.0.0.1:8080/game.html でアクセスできます。

### android版のビルド

docker開発環境を立ち上げ、下記のシェルスクリプトコマンドを実行します。

```bash
cd ${FUHAHAROOT}
sh docker_builder.sh make copy-and and-debug
```

ビルドが成功すれば`${FUHAHAROOT}/src_client/platform_android/build/outputs/apk/android-all-debug.apk`が生成されます。
adbなどを用いてandroid実機にインストールして確認してください。

### ios版のビルド

下記のmakeコマンドを実行し、必要なリソースをコピーします。

```bash
cd ${FUHAHAROOT}
make copy-ios
```

XCodeで`${FUHAHAROOT}/src_client/platform_ios/fuhaha.xcodeproj`を開きます。

* XCodeの作法に従いビルドします。
* プロジェクトにソースコードを追加した場合は以下の手順に従います。
    * XCodeで`core`と`ios`が2個づつあるので削除します。
    * `${FUHAHAROOT}/src_client/fuhahaEngine/core`を追加します。
    * `${FUHAHAROOT}/src_client/fuhahaEngine/ios `を追加します。
    * `${FUHAHAROOT}/src_client/main/core`を追加します。
    * `${FUHAHAROOT}/src_client/main/ios `を追加します。
    * 削除する際は`Remove References`で、追加する際は`Create groups`を選択します。

## ライセンス

MIT
