#include "Classifire.h"

Classifire::Classifire():
	values_(0)
{
}

Classifire::~Classifire()
{
}

void Classifire::setClassifire(const std::string& fileName)
{
    std::ifstream fileWithClassifire(fileName);
    if (fileWithClassifire.is_open())
    {
        unsigned int quantityValues{};
        fileWithClassifire >> quantityValues;
        values_.resize(quantityValues);
        for (size_t i{}; i < quantityValues; ++i)
            getline(fileWithClassifire, values_[i]);
    }
    else
    {
        throw (std::string{ "File not found" });
    }
}

void Classifire::setClassifire(const std::vector<std::string>& values)
{
    values_.assign(values.begin(), values.end());
}

void Classifire::addValue(const std::string& newValue)
{
    bool isUniqueValue(true);
    for (size_t i{}; i < values_.size(); ++i)
    {
        if (values_[i] == newValue)
        {
            isUniqueValue = false;
            i = values_.size();
        }
    }
    if (isUniqueValue)
        values_.push_back(newValue);
}

void Classifire::deleteByIndex(const size_t index)
{
    if (index >= values_.size())
        throw(std::string{ "Out of array" });
    else
        values_.erase(values_.begin() + index);
}

void Classifire::deleteByName(const std::string& value)
{
    for (size_t i{}; i < values_.size(); ++i)
    {
        if (values_[i] == value)
        {
            values_.erase(values_.begin() + i);
            i = values_.size();
        }
    }
}

void Classifire::getClassifire(std::vector<std::string>& values) const
{
    values.assign(values_.begin(), values_.end());
}

void Classifire::getClassifire(const std::string& fileName) const
{
    std::ofstream outFile(fileName, std::ofstream::out | std::ofstream::trunc);
    if (outFile.is_open())
    {
        outFile << values_.size() << std::endl;
        for (size_t i{ }; i < values_.size(); ++i)
            outFile << values_[i] << std::endl;
    }
    else
    {
        throw (std::string{ "File not found" });
    }
}
