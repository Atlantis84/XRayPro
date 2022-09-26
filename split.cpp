#include "split.h"

//返回分割后的string数组，第一个参数输入要分割的string，第二个参数输入以什么分割（"-"）
vector<string> split_string(const string &str, const string &pattern)
{
	vector<string> res;
	if (str == "")
		return res;

	string strs = str + pattern;

	size_t pos = strs.find(pattern);
	size_t size = strs.size();


	while (pos != string::npos)
	{
		string x = strs.substr(0, pos);
		res.push_back(x);
		strs = strs.substr(pos + 1, size);
		pos = strs.find(pattern);

	}
	return res;
}




