#pragma once
#include "stdafx.h"
#include "Obj.h"
#include "StringObj.h"


class Button : public Obj
{
public:
    /* メンバ変数 */
    StringObj* label = NULL;
    /* メンバ関数 */
    // コンストラクタ
    Button(
        int x,
        int y,
        const std::vector<int>& image_handle
    ) : Obj(x, y,FALSE, image_handle) {}
    Button(
        int x,
        int y,
        const std::vector<int>& image_handle,
        const char* str,
        unsigned int color,
        int font
    ) : Obj(x, y, FALSE,image_handle) {
        label = new StringObj(x, y, str, color, font);
    }
    bool isContain(int px, int py) {
        if ((x <= px && px <= x + xlength) && (y <= py && py <= y + ylength)) return true;
        return false;
    }
    void next(int px, int py) {
        /* 次のゲームループでの状態を決定する
        * ゲームループにつき1回実行
        * px, pyはマウスのポインタの座標
        *  ボタンの上にマウスがあるか調べ, stateを更新
        */
        if (isContain(px, py)) {
            // マウス形状の取得
            HCURSOR hand = GetCursor();
            // マウス形状の設定
            hand = LoadCursor(NULL, IDC_HAND);  //カーソルをハンドにする
            SetCursor(hand);
            state = 1;
        }
        else {
			state = 0;
        }
    }
    bool isPushedLeft(int click_event, int button_type, int cx, int cy, int log_type) {
        /* マウス左ボタンが押されたかを判定 */
		if (click_event == 0 && isContain(cx, cy) && log_type == MOUSE_INPUT_LOG_DOWN && (button_type & MOUSE_INPUT_LEFT)) {
			return true;
		}
        return false;
    }
    bool isReleasedLeft(int click_event, int button_type, int cx, int cy, int log_type) {
        /* マウス右ボタンが離されたかを判定 */
		if (click_event == 0 && isContain(cx, cy) && log_type == MOUSE_INPUT_LOG_UP && (button_type & MOUSE_INPUT_LEFT)) {
			return true;
		}
        return false;
    }
    bool isPushedRight(int click_event, int button_type, int cx, int cy, int log_type) {
        /* マウス左ボタンが押されたかを判定 */
		if (click_event == 0 && isContain(cx, cy) && log_type == MOUSE_INPUT_LOG_DOWN && (button_type & MOUSE_INPUT_RIGHT)) {
			return true;
		}
        return false;
    }
    bool isReleasedRight(int click_event, int button_type, int cx, int cy, int log_type) {
        /* マウス右ボタンが離されたかを判定 */
		if (click_event == 0 && isContain(cx, cy) && log_type == MOUSE_INPUT_LOG_UP && (button_type & MOUSE_INPUT_RIGHT)) {
			return true;
		}
        return false;
    }
    void draw() override {
        Obj::draw();
        if (label != NULL) label->draw();
    }
};

