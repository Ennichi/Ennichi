---
title: "Script_Document.md"
tags: ""
---

# メンバー用スクリプトドキュメント
# Goldfish.h
# Goldfishクラス



## コンストラクタ
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
## メンバ関数
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

```c++
void draw();
```
金魚を描画する関数  
毎フレーム実行する必要がある  
実装は `Obj` クラスの `draw()` と同じ
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
