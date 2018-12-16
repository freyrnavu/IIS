#pragma once
#include <string>

class IStemmer
{
public:
	virtual ~IStemmer() = default;

	virtual std::string stem(const std::string& word) const = 0;
};

