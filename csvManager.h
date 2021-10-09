﻿#pragma once
#include "functions.h"
#define CSVMODE (std::ios::in | std::ios::out | std::ios::ate)

class csvManager
{
	using ULL = unsigned long long;

	enum class MODE : char
	{
		OPEN = 0,
		CLOSED
	};

private:
	std::fstream iofs;
	std::string file_path;
	unsigned int column_num = 0;
	std::list<std::string> detail;
	std::list<std::string>::iterator itr;
	unsigned int location = 0;

public:
	//コンストラクタ
	csvManager(std::string fpath, MODE mode = MODE::OPEN
	): iofs(fpath, CSVMODE), 
		file_path{fpath}, 
		detail{},
		itr{detail.begin()}
	{
		if (mode == MODE::CLOSED)
		{
			iofs.close();
		}
	}

	//コピーコンストラクタ(ファイルを閉じてるときだけ)
	csvManager(const csvManager& manager
	): file_path{manager.file_path}, 
		column_num{manager.column_num}, 
		detail{manager.detail},
		itr{detail.begin()}
	{
		if (!manager.iofs.is_open())
		{
			iofs.open(file_path, CSVMODE);
		}
	}

	csvManager(csvManager&&)noexcept = default;
	//csvファイルを新しく生成
	static csvManager create(const std::string& fpath)
	{
		{
			std::ofstream tmp(fpath);
		}
		csvManager manager(fpath, MODE::CLOSED);
		return manager;
	}

	//ファイルを読み込むだけ
	void readAll()
	{
		std::string tmp;
		iofs.clear();
		iofs.seekg(std::ios::beg);
		while (1)
		{
			std::getline(iofs, tmp);
			if (iofs.eof())break;
			detail.push_back(tmp);
		}
		iofs.clear();
		iofs.seekg(std::ios::beg);
		itr = detail.begin();
		location = 0;
	}

	//任意の位置に移動
	inline void locate(unsigned int index)
	{
		for (; location < index; ++location)
		{
			iofs.seekg((long long)(*(itr++)).length() + 2ll, std::ios::cur);
		}
		for (; location > index; --location)
		{
			iofs.seekg(-(long long)(*(--itr)).length() - 2ll, std::ios::cur);
		}
	}

	//行を指定して読み込み
	std::vector<std::string> readAt(unsigned int index)
	{
		locate(index);
		std::stringstream strstr(*itr);
		std::string tmp;
		std::vector<std::string> v;
		while (std::getline(strstr, tmp, ','))
		{
			v.push_back(tmp);
		}
		iofs.clear();
		return v;
	}

	//指定の行に挿入
	template<class... cArgs>
	void insert(unsigned int index, const cArgs&... str)
	{
		std::string insstr = "";
		for (std::string strtmp: std::initializer_list<std::string>{str...})
		{
			insstr += strtmp + ",";
		}
		locate(index);
		itr = detail.insert(itr, insstr);
		for (; itr != detail.end(); ++itr)
		{
			iofs << *itr << std::endl;
			++location;
		}
	}

	//指定の行を差し替え
	template<class... cArgs>
	void swap(unsigned int index, const cArgs&... str)
	{
		std::string insstr = "";
		for (std::string strtmp : std::initializer_list<std::string>{ str... })
		{
			insstr += strtmp + ",";
		}
		locate(index);
		iofs << "\0";
		*itr = insstr;
		for (; itr != detail.end(); ++itr)
		{
			iofs << *itr << std::endl;
			++location;
		}
		iofs.clear();
	}

	//指定の行を削除
	void del(unsigned int index)
	{
		iofs.close();
		iofs.open(file_path, std::ios::in | std::ios::out | std::ios::trunc);
		itr = std::next(detail.begin(), index);
		detail.erase(itr);
		for (auto& tmp : detail)
		{
			iofs << tmp << std::endl;
		}
		iofs.close();
		iofs.open(file_path, CSVMODE);
		iofs.clear();
		iofs.seekg(std::ios::beg);
		location = 0;
		itr = detail.begin();
	}

	//指定の範囲の行を削除
	void del(unsigned int a, unsigned int b)
	{
		{
			unsigned int tmp = a;
			a = (std::min)(a, b);
			b = (std::max)(tmp, b);
		}
		iofs.close();
		iofs.open(file_path, std::ios::in | std::ios::out | std::ios::trunc);
		itr = std::next(detail.begin(), a);
		detail.erase(itr, std::next(itr, static_cast<ULL>(b) - static_cast<ULL>(a) + 1));
		for (auto& tmp : detail)
		{
			iofs << tmp << std::endl;
		}
		iofs.close();
		iofs.open(file_path, CSVMODE);
		iofs.clear();
		iofs.seekg(std::ios::beg);
		location = 0;
		itr = detail.begin();
	}
};


class Ranking :public csvManager
{

};