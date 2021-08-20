#pragma once

class Sprite
{
private:
	/* 外部アクセス不可 */
	int __length;//コマ数
	int* image_handle;//グラフィックのハンドル(動的配列)

public:
	/* アクセス可能 */
	int width;//横幅
	int height;//高さ

	//デフォルトコンストラクタ
	Sprite():__length{0},width{0},height{0}
	{
		image_handle = nullptr;
	}

	//コンストラクタ
	Sprite(const std::vector<const char*>& image_path
	):__length{static_cast<int>(image_path.size())}
	{
		int i = 0;
		image_handle = new int[__length];
		for (auto& path : image_path)
		{
			image_handle[i] = LoadGraph(path);
			++i;
		}
		GetGraphSize(image_handle[0], &width, &height);
	}

	//コンストラクタ
	Sprite(std::initializer_list<const char*> path_list
	) :__length{static_cast<int>(path_list.size())}
	{
		int i = 0;
		image_handle = new int[__length];
		for (auto& path : path_list)
		{
			image_handle[i] = LoadGraph(path);
			++i;
		}
		GetGraphSize(image_handle[0], &width, &height);
	}

	//コピーコンストラクタ
	Sprite(const Sprite& base):__length{base.__length}, width{base.width}, height{base.height}
	{
		if (__length == -1)
			throw new std::invalid_argument("destroyしたオブジェクトはコピーできません");
		image_handle = new int[__length];
		for (int i = 0; i < __length; ++i)
		{
			image_handle[i] = base.image_handle[i];
		}
	}

	//コピー代入演算子
	Sprite& operator=(const Sprite& base)
	{
		if (base.__length == -1)
			throw new std::invalid_argument("destroyしたオブジェクトはコピーできません");
		__length = base.__length;
		width = base.width;
		height = base.height;
		image_handle = new int[__length];
		for (int i = 0; i < __length; ++i)
		{
			image_handle[i] = base.image_handle[i];
		}
	}

	//ムーブコンストラクタを自動生成
	Sprite(Sprite&&) = default;
	Sprite& operator=(Sprite&&) = default;

	//デストラクタ
	virtual ~Sprite()
	{
		if(image_handle!=nullptr)
			delete[] image_handle;
	}

	//添え字演算子
	int operator[](int index)const&
	{
		if (index < 0 || index >= __length)
			throw new std::out_of_range("インデックスが範囲外");
		return image_handle[index];
	}

	//スプライトを完全に破棄
	void destroy()
	{
		for (int i = 0; i < __length; ++i)
		{
			DeleteGraph(image_handle[i]);
		}
		__length = 0;
		width = 0;
		height = 0;
		delete[] image_handle;
		image_handle = nullptr;
	}
};