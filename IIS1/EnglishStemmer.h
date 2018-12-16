#pragma once
#include "IStemmer.h"
#include <array>
#include <vector>
class EnglishStemmer : public IStemmer
{
public:
	std::string stem(const std::string& word) const override;

private:
	size_t findR1(const std::string& word) const;
	size_t findR2(const std::string& word, size_t r1Pos) const;

	bool isVowel(char ch) const;

	std::string transform(const std::string& word) const;
	std::string isSpecial(const std::string& word) const;
	bool replace(std::string& word, const std::string& suffix, const std::string& replacement, size_t start) const;
	std::string step0(const std::string& word) const;
	std::string step1A(const std::string& word) const;
	std::string step1B(const std::string& word, size_t r1) const;
	std::string step1C(const std::string& word) const;
	std::string step2(const std::string& word, size_t r1) const;
	std::string step3(const std::string& word, size_t r1, size_t r2) const;
	std::string step4(const std::string& word, size_t r2) const;
	std::string step5(const std::string& word, size_t r1, size_t r2) const;

	bool containsVowel(std::string::const_iterator begin, std::string::const_iterator end) const;

	bool isShort(const std::string& word) const;

private:
	const std::string _vowels = "aeiouy";
	const std::vector<std::string> _doubles = { "bb", "dd", "ff", "gg", "mm", "nn", "pp", "rr", "tt" };
	const std::string _validEndings = "cdeghkmnrt";
};

