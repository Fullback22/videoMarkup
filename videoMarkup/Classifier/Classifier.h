#pragma once
#include <iostream>
#include <fstream>
#include<vector>
#include<string>

class Classifier
{
protected:
	std::vector<std::string> values_;

public:
	Classifier();
	~Classifier();
	void setClassifier(const std::string& fileName);
	void setClassifier(const std::vector<std::string>& values);
	void addValue(const std::string& newValue);
	
	std::string& operator[](size_t const pos);
	const std::string& operator[](size_t const pos) const;

	void deleteByIndex(const size_t index);
	void deleteByName(const std::string& value);
	void clear();

	void getClassifier(std::vector<std::string>& values) const;
	void getClassifier(const std::string& fileName) const;

	size_t size() const;
};

