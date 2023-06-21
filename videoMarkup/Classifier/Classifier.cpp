#include "Classifier.h"

Classifier::Classifier():
	values_(0)
{
}

Classifier::~Classifier()
{
}

void Classifier::setClassifier(const std::string& fileName)
{
    std::ifstream fileWithClassifire{ fileName };
    if (fileWithClassifire.is_open())
    {
        unsigned int quantityValues{};
        fileWithClassifire >> quantityValues;
        fileWithClassifire.ignore();
        values_.resize(quantityValues);
        for (size_t i{}; i < quantityValues; ++i)
            getline(fileWithClassifire, values_[i]);
        fileWithClassifire.close();
    }
    else
    {
        throw (std::string{ "File not found" });
    }
}

void Classifier::setClassifier(const std::vector<std::string>& values)
{
    values_.assign(values.begin(), values.end());
}

void Classifier::addValue(const std::string& newValue)
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

std::string& Classifier::operator[](size_t const pos)
{
    return values_[pos];
}

const std::string& Classifier::operator[](size_t const pos) const
{
    return values_[pos];
}

void Classifier::deleteByIndex(const size_t index)
{
    if (index >= values_.size())
        throw(std::string{ "Out of array" });
    else
        values_.erase(values_.begin() + index);
}

void Classifier::deleteByName(const std::string& value)
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

void Classifier::clear()
{
    values_.clear();
}

void Classifier::getClassifier(std::vector<std::string>& values) const
{
    values.assign(values_.begin(), values_.end());
}

void Classifier::getClassifier(const std::string& fileName) const
{
    std::ofstream outFile(fileName, std::ofstream::out | std::ofstream::trunc);
    if (outFile.is_open())
    {
        outFile << values_.size() << std::endl;
        for (size_t i{ }; i < values_.size(); ++i)
            outFile << values_[i] << std::endl;
        outFile.close();
    }
    else
    {
        throw (std::string{ "File not found" });
    }
}

size_t Classifier::size() const
{
    return values_.size();
}
