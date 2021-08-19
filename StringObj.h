#pragma once
class StringObj
{
public:
	// �����o�ϐ�
	int x = 0, y = 0; // ����̍��W
	const char* label = NULL; // �\�����镶����
	unsigned int color = 0x000000;
	int font = NULL;

	// �����o�֐�
	StringObj(
		int x,
		int y,
		const char* label,
		unsigned int color,
		int font
	) : x(x), y(y), label(label), color(color), font(font)
	{}

	void draw() {
		DrawStringToHandle(x, y, label, color, font);
	}
};

