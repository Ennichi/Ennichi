#pragma once
#include "stdafx.h"
#include "functions.h"
#define CSVMODE (std::ios::in | std::ios::out | std::ios::ate)

class csvManager
{
	using ULL = unsigned long long;

private:
	std::fstream iofs;
	std::string file_path;
	unsigned int column_num = 0;
	unsigned int location = 0;

protected:
	std::list<std::string> detail;
	std::list<std::string>::iterator alocitr;

public:
	//コンストラクタ
	csvManager(std::string fpath
	): iofs(), 
		file_path{fpath}, 
		detail{},
		alocitr{detail.begin()}
	{
		if(!file_exists(fpath))
		{
			std::ofstream ofs(fpath);
		}
		iofs.open(fpath, CSVMODE);
	}

	csvManager(const csvManager&) = delete;
	csvManager(csvManager&&)noexcept = default;

	csvManager& operator=(const csvManager&) = delete;
	csvManager& operator=(csvManager&&) = default;

	//デストラクタ
	~csvManager()
	{
		if(iofs.is_open())iofs.close();
	}
	
	//ファイルを読み込むだけ
	void readAll()
	{
		std::string tmp;
		iofs.clear();
		iofs.seekg(std::ios::beg);
		detail.clear();
		while (1)
		{
			std::getline(iofs, tmp);
			if (iofs.eof())break;
			detail.push_back(tmp);
		}
		iofs.clear();
		iofs.seekg(std::ios::beg);
		alocitr = detail.begin();
		location = 0;
	}

	//任意の位置に移動
	inline void locate(unsigned int index)
	{
		for (; location < index; ++location)
		{
			iofs.seekg((long long)(*(alocitr++)).length() + 2ll, std::ios::cur);
		}
		for (; location > index; --location)
		{
			iofs.seekg(-(long long)(*(--alocitr)).length() - 2ll, std::ios::cur);
		}
	}

	//行を指定して読み込み
	std::vector<std::string> readAt(unsigned int index)
	{
		locate(index);
		std::stringstream strstr(*alocitr);
		std::string tmp;
		std::vector<std::string> v;
		while (std::getline(strstr, tmp, ','))
		{
			v.push_back(tmp);
		}
		iofs.clear();
		return v;
	}

protected:

	void write_string_insert(const std::string& wstr)
	{
		alocitr = detail.insert(alocitr, wstr);
		for (; alocitr != detail.end(); ++alocitr)
		{
			iofs << *alocitr << std::endl;
			++location;
		}
		iofs.clear();
	}

public:

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
		write_string_insert(insstr);
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
		*alocitr = insstr;
		for (; alocitr != detail.end(); ++alocitr)
		{
			iofs << *alocitr << std::endl;
			++location;
		}
		iofs.clear();
	}

	//指定の行を削除
	void del(unsigned int index)
	{
		iofs.close();
		iofs.open(file_path, std::ios::in | std::ios::out | std::ios::trunc);
		alocitr = std::next(detail.begin(), index);
		detail.erase(alocitr);
		for (auto& tmp : detail)
		{
			iofs << tmp << std::endl;
		}
		iofs.close();
		iofs.open(file_path, CSVMODE);
		iofs.clear();
		iofs.seekg(std::ios::beg);
		location = 0;
		alocitr = detail.begin();
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
		alocitr = std::next(detail.begin(), a);
		detail.erase(alocitr, std::next(alocitr, static_cast<ULL>(b) - static_cast<ULL>(a) + 1));
		for (auto& tmp : detail)
		{
			iofs << tmp << std::endl;
		}
		iofs.close();
		iofs.open(file_path, CSVMODE);
		iofs.clear();
		iofs.seekg(std::ios::beg);
		location = 0;
		alocitr = detail.begin();
	}
};


//次に作る
class Ranking :public csvManager
{
private:
	std::list<int> scoreList;
public:

	Ranking(std::string fpath) : csvManager(fpath){}

	Ranking(const Ranking&) = delete;
	Ranking(Ranking&&) = default;

	//挿入
	void insert(const std::string& username, int score)
	{
		std::list<int>::iterator itr{};
		unsigned int cnt = 0;
		for (itr = scoreList.begin(); itr != scoreList.end(); ++itr)
		{
			if (score >= *itr)break;
			++cnt;
		}
		locate(cnt);
		scoreList.insert(itr, score);
		std::string insstr = username + "," + std::to_string(score) + ",";
		write_string_insert(insstr);
	}
};