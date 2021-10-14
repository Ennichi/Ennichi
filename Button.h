#pragma once
#include "stdafx.h"
#include "Obj.h"
#include "StringObj.h"


class Button : public Obj
{
public:
    /* �����o�ϐ� */
    StringObj* label = NULL;
    /* �����o�֐� */
    // �R���X�g���N�^
    Button(
        int x,
        int y,
        bool is_collision,
        const std::vector<int>& image_handle
    ) : Obj(x, y, can_collision, image_handle) {}
    Button(
        int x,
        int y,
        bool is_collision,
        const std::vector<int>& image_handle,
        const char* str,
        unsigned int color,
        int font
    ) : Obj(x, y, can_collision, image_handle) {
        label = new StringObj(x, y, str, color, font);
    }
    bool isContain(int px, int py) {
        if ((x <= px && px <= x + xlength) && (y <= py && py <= y + ylength)) return true;
        return false;
    }
    void next(int px, int py) {
        /* ���̃Q�[�����[�v�ł̏�Ԃ����肷��
        * �Q�[�����[�v�ɂ�1����s
        * px, py�̓}�E�X�̃|�C���^�̍��W
        *  �{�^���̏�Ƀ}�E�X�����邩����, state���X�V
        */
        if (isContain(px, py)) {
            // �}�E�X�`��̎擾
            HCURSOR hand = GetCursor();
            // �}�E�X�`��̐ݒ�
            hand = LoadCursor(NULL, IDC_HAND);  //�J�[�\�����n���h�ɂ���
            SetCursor(hand);
            state = 1;
        }
        else {
			state = 0;
        }
    }
    bool isPushedLeft(int click_event, int button_type, int cx, int cy, int log_type) {
        /* �}�E�X���{�^���������ꂽ���𔻒� */
		if (click_event == 0 && isContain(cx, cy) && log_type == MOUSE_INPUT_LOG_DOWN && (button_type & MOUSE_INPUT_LEFT)) {
			return true;
		}
        return false;
    }
    bool isReleasedLeft(int click_event, int button_type, int cx, int cy, int log_type) {
        /* �}�E�X�E�{�^���������ꂽ���𔻒� */
		if (click_event == 0 && isContain(cx, cy) && log_type == MOUSE_INPUT_LOG_UP && (button_type & MOUSE_INPUT_LEFT)) {
			return true;
		}
        return false;
    }
    bool isPushedRight(int click_event, int button_type, int cx, int cy, int log_type) {
        /* �}�E�X���{�^���������ꂽ���𔻒� */
		if (click_event == 0 && isContain(cx, cy) && log_type == MOUSE_INPUT_LOG_DOWN && (button_type & MOUSE_INPUT_RIGHT)) {
			return true;
		}
        return false;
    }
    bool isReleasedRight(int click_event, int button_type, int cx, int cy, int log_type) {
        /* �}�E�X�E�{�^���������ꂽ���𔻒� */
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

