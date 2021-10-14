---
title: "プロジェクトのビルド方法"
tags: "build","Visual Studio","DXLib"
categories: [ ビルド方法 ]

---

# はじめに
ここでは本プログラムをビルドするために必要になるソフトウェアのセットアップについて解説します。
## Visual Studioについて
本プログラムはVisual Studio 2019にて、開発しています。Visual Studio 2019 Community版は、この文章を書いている時点では、特定の用途において無料です。詳細については[Visual Studio 2019 の各エディションの比較
](https://visualstudio.microsoft.com/ja/vs/compare/)をご確認ください。
### Visual Studioのセットアップ(Windows 10向け)
Windows 11でも、このような方法でインストールできますが、後述する、Microsoft Storeからインストールしたほうが、容易にインストールできます。
1. [Visual Studio Toolsのダウンロード](https://visualstudio.microsoft.com/ja/downloads/)へアクセスし、
> コミュニティ
> 
> 強力な IDE, 学生, オープンソース貢献者, 個人に無料

からダウンロードします。

2. インストーラーを起動すると、ワークロードが表示されます。
3. その**C++によるデスクトップ開発**にチェックを入れます。
4. チェックを入れたら、**ダウンロードしながらインストール**を選び、インストールが終わるまで待ちます。
5. 初めて立ち上げたときは、Microsoftアカウントへサインインを要求されるので、お使いのアカウントでログインしてください。
6. Visual Studioがインストールできました!
## Githubからのクローン方法
このプロジェクトはGithubにて管理されています。そこで、Githubから本プロジェクトをクローンする方法について書きます。
1. [拙著](https://qiita.com/312k/items/b2e6f3a8446447c8404f)ですが、こちらを見るか、ほかの方が書かれたものを見るかして、Gitをインストールしてください。
2. 適当なディレクトリに移動してください。
3. 移動先で、下記に示すコマンドを入力してください。
```shell
git clone https://github.com/Ennichi/Ennichi.git
```
このコマンドは、Githubからローカルへ、複製するコマンドで、ローカルで編集できるようになります。
## DXライブラリのセットアップ
本プロジェクトは[DXライブラリ](https://dxlib.xsrv.jp/)という、Windows APIのラッパライブラリを使用しています。

1. [DXライブラリのダウンロードページ](https://dxlib.xsrv.jp/dxdload.html)から
> ＤＸライブラリ Windows版 VisualStudio( C++ )用(Verx.xx)をダウンロードする(zip圧縮形式(約xxxMB))

をダウンロードしてください。

2. ZIPファイルを解凍して、DxLib_VC\プロジェクトに追加すべきファイル_VC用 を取り出してください。
3. **プロジェクトに追加すべきファイル_VC用**を**3.22c_VS**へリネームして、先ほどクローンしたディレクトリに移動してください。
## ビルド
ここまでの手順でビルドの手筈が整いました。
1. ディレクトリ内の、**Ennichi.sln**をダブルクリックして、Visual Studioを起動させてください。
2. タイトルバーのデバッグ->デバッグなしで開始をクリックすると実行できました!!
