#pragma once
#include "../../../header/stdafx.h"
#define CSVMODE (std::ios::in | std::ios::out | std::ios::ate)

class csvManager
{
	using ULL = unsigned long long;

private:
	std::string file_path;

protected:
	std::fstream iofs;
	std::list<std::string> detail;
	std::list<std::string>::iterator allocitr;
	unsigned int location = 0;

public:
	//コンストラクタ
	csvManager(std::string fpath) : iofs(),
									file_path{fpath},
									detail{},
									allocitr{detail.begin()}
	{
		if (!file_exists(fpath))
		{
			std::ofstream ofs(fpath);
		}
		iofs.open(fpath, CSVMODE);
	}

	csvManager(const csvManager &) = delete;
	csvManager(csvManager &&) noexcept = default;

	csvManager &operator=(const csvManager &) = delete;
	csvManager &operator=(csvManager &&) = default;

	//デストラクタ
	~csvManager()
	{
		if (iofs.is_open())
			iofs.close();
	}

	//ファイルを読み込むだけ
	virtual void readAll()
	{
		std::string tmp;
		iofs.clear();
		iofs.seekg(std::ios::beg);
		detail.clear();
		while (1)
		{
			std::getline(iofs, tmp);
			if (iofs.eof())
				break;
			detail.push_back(tmp);
		}
		iofs.clear();
		iofs.seekg(std::ios::beg);
		allocitr = detail.begin();
		location = 0;
	}

	//任意の位置に移動
	inline void locate(unsigned int index)
	{
		for (; location < index; ++location)
		{
			iofs.seekg((long long)(*(allocitr++)).length() + 2ll, std::ios::cur);
		}
		for (; location > index; --location)
		{
			iofs.seekg(-(long long)(*(--allocitr)).length() - 2ll, std::ios::cur);
		}
	}

	//行を指定して読み込み
	std::vector<std::string> readAt(unsigned int index)
	{
		locate(index);
		std::stringstream strstr(*allocitr);
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
	void write_string_insert(const std::string &wstr)
	{
		allocitr = detail.insert(allocitr, wstr);
		for (; allocitr != detail.end(); ++allocitr)
		{
			iofs << *allocitr << std::endl;
			++location;
		}
		iofs.clear();
	}

	void del_base(unsigned int index)
	{
		iofs.close();
		iofs.open(file_path, std::ios::in | std::ios::out | std::ios::trunc);
		allocitr = std::next(detail.begin(), index);
		detail.erase(allocitr);
		for (auto &tmp : detail)
		{
			iofs << tmp << std::endl;
		}
		iofs.close();
		iofs.open(file_path, CSVMODE);
		iofs.clear();
		iofs.seekg(std::ios::beg);
		location = 0;
		allocitr = detail.begin();
	}

public:
	//指定の行に挿入
	template <class... cArgs>
	void insert(unsigned int index, const cArgs &...str)
	{
		std::string insstr = "";
		for (std::string strtmp : std::initializer_list<std::string>{str...})
		{
			insstr += strtmp + ",";
		}
		locate(index);
		write_string_insert(insstr);
	}

	//指定の行を差し替え
	template <class... cArgs>
	void swap(unsigned int index, const cArgs &...str)
	{
		std::string insstr = "";
		for (std::string strtmp : std::initializer_list<std::string>{str...})
		{
			insstr += strtmp + ",";
		}
		locate(index);
		iofs << "\0";
		*allocitr = insstr;
		for (; allocitr != detail.end(); ++allocitr)
		{
			iofs << *allocitr << std::endl;
			++location;
		}
		iofs.clear();
	}

	std::size_t column_num() const noexcept
	{
		return detail.size();
	}

	//指定の行を削除
	virtual void del(unsigned int index)
	{
		this->del_base(index);
	}

	//指定の範囲の行を削除
	virtual void del(unsigned int a, unsigned int b)
	{
		{
			unsigned int tmp = a;
			a = (std::min)(a, b);
			b = (std::max)(tmp, b);
		}
		iofs.close();
		iofs.open(file_path, std::ios::in | std::ios::out | std::ios::trunc);
		allocitr = std::next(detail.begin(), a);
		detail.erase(allocitr, std::next(allocitr, static_cast<ULL>(b) - static_cast<ULL>(a) + 1));
		for (auto &tmp : detail)
		{
			iofs << tmp << std::endl;
		}
		iofs.close();
		iofs.open(file_path, CSVMODE);
		iofs.clear();
		iofs.seekg(std::ios::beg);
		location = 0;
		allocitr = detail.begin();
	}
};

//次に作る
class Ranking : public csvManager
{
private:
	std::list<int> scoreList;

public:
	Ranking(std::string fpath) : csvManager(fpath) {}

	Ranking(const Ranking &) = delete;
	Ranking(Ranking &&) = default;

	Ranking &operator=(const Ranking &) = delete;
	Ranking &operator=(Ranking &&) = default;

	void readAll() override
	{
		std::string tmp;
		std::stringstream ss;
		iofs.clear();
		iofs.seekg(std::ios::beg);
		detail.clear();
		while (1)
		{
			std::string score_tmp;
			std::getline(iofs, tmp);
			if (iofs.eof())
				break;
			ss << tmp;
			std::getline(ss, score_tmp, ',');
			std::getline(ss, score_tmp, ',');
			detail.push_back(tmp);
			scoreList.push_back(stoi(score_tmp));
		}
		iofs.clear();
		iofs.seekg(std::ios::beg);
		allocitr = detail.begin();
		location = 0;
	}
	//挿入
	void insert(const std::string &username, int score)
	{
		std::list<int>::iterator itr{};
		unsigned int cnt = 0;
		for (itr = scoreList.begin(); itr != scoreList.end(); ++itr)
		{
			if (score >= *itr)
				break;
			++cnt;
		}
		locate(cnt);
		scoreList.insert(itr, score);
		std::string insstr = username + "," + std::to_string(score) + ",";
		write_string_insert(insstr);
	}

	//削除
	void del(unsigned int index) override
	{
		scoreList.erase(std::next(scoreList.begin(), index));
		del_base(index);
	}

	int at(unsigned int index)
	{
		return stoi(readAt(index).at(1));
	}
};