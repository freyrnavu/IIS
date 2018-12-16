#include "RussianStemmer.h"
#include <regex>

std::string RussianStemmer::stem(const std::string& word) const
{
	std::string result = word;
	size_t posRv = findRV(result);
	size_t posR1 = findR1(result);
	size_t posR2 = findR2(result, posR1);
	result = firstStep(result, posRv);
	result = secondStep(result, posRv);
	result = thirdStep(result, posR2);
	return fourthStep(result, posRv);
}

size_t RussianStemmer::findRV(const std::string& word) const
{
	std::regex vowelRegex{ _vowelsRegexp };
	std::smatch m;
	if (!std::regex_search(word, m, vowelRegex))
		return word.length();

	return  std::distance(word.cbegin(), m[0].second);
}

size_t RussianStemmer::findR1(const std::string& word) const
{
	std::regex r1{_vowelsRegexp + _nonVowelsRegexp};
	std::smatch m;

	if (!std::regex_search(word, m, r1))
		return word.length();

	return  std::distance(word.cbegin(), m[0].second);
}

size_t RussianStemmer::findR2(const std::string& word, size_t r1Pos) const
{
	std::regex r1{ _vowelsRegexp + _nonVowelsRegexp };
	std::smatch m;

	if (!std::regex_search(word.begin() + r1Pos, word.end(), m, r1))
		return word.length();

	return  std::distance(word.cbegin(), m[0].second);
}

std::string RussianStemmer::firstStep(const std::string& word, size_t rvPos) const
{
	std::regex perfectGerund{ _perfectGerundRegexp };
	std::smatch m;
	std::string result = word;

	if (auto res = cutWithRegexpr(result, _perfectGerundRegexp, rvPos, true); res.match)
	{
		return res.result;
	}
	if (auto res = cutWithRegexpr(result, _reflexiveRegexp, rvPos); res.match)
	{
		result = res.result;
	}
	if (auto res = cutWithRegexpr(result, _adjectivesRegexp, rvPos); res.match)
	{
		res = cutWithRegexpr(res.result, _participleRegexp, rvPos, true);
		return res.result;
	}
	else if (auto res = cutWithRegexpr(result, _verbRegexp, rvPos, true); res.match)
	{
		return res.result;
	}
	else if (auto res = cutWithRegexpr(result, _nounRegexp, rvPos); res.match)
	{
		return res.result;
	}

	return result;
}

std::string RussianStemmer::secondStep(const std::string& word, size_t r2Pos) const
{
	return cutWithRegexpr(word, _iRegexp, r2Pos).result;
}

std::string RussianStemmer::thirdStep(const std::string& word, size_t rvPos) const
{
	return cutWithRegexpr(word, _derivationalRegexp, rvPos).result;
}

std::string RussianStemmer::fourthStep(const std::string& word, size_t rvPos) const
{
	if (auto res = cutWithRegexpr(word, _nnRegexp, rvPos); res.match)
		return res.result;
	else if (auto res = cutWithRegexpr(word, _superlativeRegexp, rvPos); res.match)
		return cutWithRegexpr(res.result, _nnRegexp, rvPos).result;
	else if (auto res = cutWithRegexpr(word, _bRegexp, rvPos); res.match)
		return res.result;

	return word;
}

auto RussianStemmer::cutWithRegexpr(const std::string& word, const std::string& regexpr, size_t r2Pos, bool haveGroups) const -> Result
{
	std::regex regexp{ regexpr };
	std::smatch m;
	Result result;

	if (word.length() < r2Pos)
		return { false, word };

	if (std::regex_search(word.cbegin() + r2Pos, word.cend(), m, regexp))
	{
		auto it = m[0].first;
		if (haveGroups)
			it = *it == 'à' || *it == 'ÿ' ? it + 1 : it;
		return { true, std::string{word.cbegin(), it} };
	}

	return { false, word };
}
