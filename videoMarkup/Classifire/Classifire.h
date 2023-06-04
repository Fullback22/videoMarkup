#pragma once
#include <iostream>
#include <fstream>
#include<vector>
#include<string>

class Classifire
{
protected:
	std::vector<std::string> values_;

public:
	Classifire();
	~Classifire();
	void setClassifire(const std::string& fileName);
	void setClassifire(const std::vector<std::string>& values);
	void addValue(const std::string& newValue);

	void deleteByIndex(const size_t index);
	void deleteByName(const std::string& value);

	void getClassifire(std::vector<std::string>& values) const;
	void getClassifire(const std::string& fileName) const;
};

