---
title: "Script_Document"
tags: ""
---

# メンバー用スクリプトドキュメント
---
# Goldfish.h
---
# Goldfishクラス
---


## コンストラクタ
---
```c++
Goldfish(int x,
         int y,
         bool can_collision,
         const std::vector<int>& image_handle);
Goldfish(int x,
         int y,
         double angle,
         bool can_collision,
         const std::vector<int>& image_handle);
```
1,2ともObjクラスと動きは同じ

---
## メンバ関数
---
```c++ 
void setdifficulty(unsigned int dif)noexcept;
```
金魚の捕まりにくさを設定する関数
### 引数
* `unsigned int dif`  
設定する難易度  
0以外の値を指定できる
### 返り値
無し
### 例外
投げない
  
---
```c++
int getDifficulty()noexcept;
```
金魚の捕まりにくさを取得する関数　　
### 引数
無し
### 返り値
`setdifficulty()`で設定した値
### 例外
投げない

---
```c++
void Next();
```
金魚の座標や状態を更新する  
毎フレーム更新する必要がある
### 引数
無し
### 返り値
無し
### 例外
基本的に投げない
 
---
```c++
void SetMovement(MOV_OPTION option, double p1, double p2);
```
金魚の動きを設定する  
一回実行すればよい
### 引数
* `MOV_OPTION option`
  金魚の動き方を設定  
  現在、  
  線形 `MOV_OPTION::LINEAR`  
  円弧 `MOV_OPTION::CIRCLE`  
  波形 `MOV_OPTION::WAVE`  
  の三つのモードがある
* `double p1, double p2`
  具体的な動きを決めるパラメータ
  * `MOV_OPTION::LINEAR` を渡したとき  
    何を渡しても関係ない
  * `MOV_OPTION::CIRCLE` を渡したとき  
    `p1` : 半径  
    `p2` : 中心の位置を決める変数(1.0か-1.0を指定)  
  * `MOV_OPTION::WAVE` を渡したとき
    `p1` : 振幅
    `p2` : 波長
### 返り値
無し
### 例外
`MOV_OPTION::CIRCLE` を渡し、`p2` に1.0と-1.0以外の数を渡したとき

---
```c++
double setSpeed(double spmin, double spmax);
```
金魚が前進するスピードを設定する関数
### 引数
* `double spmin, double spmax`
  スピードは区間 `[spmin, spmax)` の乱数で決まる  
### 返り値  
実際に代入された値  
### 例外  
`spmin` や `spmax` を負に設定したとき  
`spmax < spmin` となる値を設定したとき  

---
```c++
void draw();
```
金魚を描画する関数  
毎フレーム実行する必要がある  
実装は `Obj` クラスの `draw()` と同じ

---
```c++
static void center(int& x, int& y, int xlength, int ylength)noexcept;
```
金魚の中心を座標を取得する関数  
メインスクリプトで使うことはない  
### 引数
* `int& x, int& y`  
  求めた座標がここの参照先に入る
* `int xlength, int ylength`  
  画像のx方向の長さとy方向の長さを与える
### 返り値  
無し
### 例外
投げない

---
```c++
static std::vector<bool> isCought(
	const std::vector<Goldfish>& goldfish,
	const Poi& poi,
	std::mt19937_64& mt,
	std::uniform_int_distribution<int>& dice);
```
金魚を捕まえることができたか調べる関数
### 引数  
* `const std::vector<Goldfish>& goldfish`  
  調べたい金魚の配列
* `const Poi& poi`  
  金魚を捕まえるポイ
* `std::mt19337_64& mt`  
  乱数器本体のインスタンス
* `std::uniform_int_distribution<int>& dice`  
  離散一様分布クラス(整数)のインスタンス
### 返り値  
金魚の配列と添え字が対応させて金魚が捕まったかどうかを格納されたbool型配列
### 例外  
`dice` の一様分布の範囲が  `[1, 1000]` でないとき  

---
```c++
bool isCought(
    const Poi& poi,
    std::mt19937_64& mt,
    std::uniform_int_distribution& dice);
```
金魚が捕まったか一匹ずつ調べる関数  
### 引数  
`static std::vector<bool> isCought(...);`
と同じ機能  
### 返り値  
金魚が捕まっているなら `true` 、金魚が捕まってないなら `false`  

---
# Text.h
---
# InputTextクラス
---
## メンバ変数
---
```c++
int NmlStr = 0xFF000000;
int NmlCur = 0xFF000000;
int IMEStrBack = 0xFFFFFF00;
int IMECur = 0xFF000000;
int IMELine = 0xFFFFFFFF;
int IMESelectStr = 0xFF000000;
int IMESelectStrBack = 0xFFFF0000;
int IMEConvWinStr = 0xFFFFFFFF;
```
1. `NmlStr` : 入力文字列の色
1. `NmlCur` : IME非使用時のカーソルの色
1. `IMEStrBack` : IME使用時の入力文字列の周りの色
1. `IMECur` : IME使用時のカーソルの色
1. `IMELine` : IME使用時の変換文字列の下線の色
1. `IMESelectStr` : IME使用時の選択対象の変換候補文字列の色
1. `IMESelectStrBack` :IME使用時の選択対象の変換候補文字列の周りの色
1. `IMEConvWinStr` : IME使用時の変換候補ウインドウ内の文字列の色

---
## コンストラクタ
---
```c++
InputText(int x0, int y0, unsigned int maxlen0, int font = -1);
InputText(
	int x0,
	int y0,
	unsigned int maxlen0,
	bool CancelValidFlag0,
	bool SingleCharOnlyFlag0,
	bool NumCharOnlyFlag0,
	int font = -1);
InputText(const InputText& base);
```
1. x,y座標、入力できる最大文字数、フォントハンドルで初期化
2. 1に加えて細かなオプション付きで初期化(詳細はDXライブラリのリファレンスを参照)
3. コピーコンストラクタ
### 例外
テキストオブジェクトを作成失敗したとき

---
## メンバ関数
---
```c++
void draw();
```
描画関数  
### 引数  
無し  
### 返り値  
無し  
### 例外  
投げない

---
```c++
void text(char* str)const&;
```
入力したテキストを取得する関数
### 引数  
* `char* str`  
  文字列を受けとるポインタ  
### 返り値  
無し  
### 例外
投げない

---
```c++
void set()const& noexcept;
```
入力を有効化する関数  
入力はエンターで終了する毎に無効化される  
### 引数  
無し  
### 返り値  
無し  
### 例外  
投げない

---
# Obj.h
---
# ObjGroupクラス
```c++
template<class T> class ObjGroup;
```

---
## コンストラクタ
---
```c++
ObjGroup();
```
内部にある配列を初期化する

---
## メンバ関数
---
```c++
void addcpy(T& object, unsigned int number);
```
指定のオブジェクトをコピーしてグループに追加する
### 引数  
* `T& object`  
  コピー元のオブジェクト
* `int number`  
  コピーする数
### 返り値  
無し
### 例外  
`std::vector<T>::push_back()` のみ例外を投げる

---
```c++
void draw();
```
グループに所属している全てのオブジェクトを描画する
### 引数  
無し  
### 返り値  
無し  
### 例外  
`T::draw()`が見つからないとき

---
```c++
void Next();
```
グループに所属している全てのオブジェクトを更新する
### 引数  
無し  
### 返り値  
無し  
### 例外  
`T::Next()`が見つからないとき

---
```c++
void destroy(std::size_t index);
```
グループに所属している任意のオブジェクトを削除する
### 引数  
* `std::size_t index`  
  削除するオブジェクトの添え字
### 返り値  
無し  
### 例外  
添え字がグループの範囲を超えているとき  
`T& T::operator=(const T&);` (コピー代入演算子)が定義されていないとき  

---
```c++
T& operator[](std::size_t index)&;
```
グループに所属している任意のオブジェクトを操作する
### 引数  
* `std::size_t index`  
  操作するオブジェクトの添え字  
### 返り値  
その添え字に対応するオブジェクトの参照  
### 例外  
添え字がグループの範囲を超えているとき
