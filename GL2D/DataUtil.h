#pragma once
#include <vector>
#include <string>

class DataUtil {
private:
	std::vector<std::string> RegexDataList;
	std::string DataVersionStr{ "Data Version" };

public:
	void Init();
	void CheckDataVersion();
	void ResetData();
	void UpdateData(const std::string& target, float value);
	float LoadData(std::string target);
};
extern DataUtil dataUtil;