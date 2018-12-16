#include "EnglishStemmer.h"
#include <unordered_map>
#include <boost\algorithm\string\replace.hpp>
#include <boost\algorithm\string\predicate.hpp>

std::string EnglishStemmer::stem(const std::string& word) const
{
	if (std::string res = isSpecial(word); !res.empty())
		return res;

	std::string result = transform(word);
	size_t r1Pos = findR1(result);
	size_t r2Pos = findR2(result, r1Pos);

	result = step0(result);
	result = step1A(result);
	result = step1B(result, r1Pos);
	result = step1C(result);
	result = step2(result, r1Pos);
	result = step3(result, r1Pos, r2Pos);
	result = step4(result, r2Pos);
	result = step5(result, r1Pos, r2Pos);

	boost::replace_all(result, "Y", "y");

	return result;
}

size_t EnglishStemmer::findR1(const std::string& word) const
{
	auto it = std::find_if(word.begin(), word.end(), [this](char ch) {
		return isVowel(ch);
	});
	it = std::find_if(it, word.end(), [this](char ch) {
		return !isVowel(ch);
	});

	return std::distance(word.begin(), it);
}

size_t EnglishStemmer::findR2(const std::string& word, size_t r1Pos) const
{
	auto it = std::find_if(word.begin() + r1Pos, word.end(), [this](char ch) {
		return isVowel(ch);
	});
	it = std::find_if(it, word.end(), [this](char ch) {
		return !isVowel(ch);
	});

	return std::distance(word.begin(), it);
}

bool EnglishStemmer::isVowel(char ch) const
{
	return _vowels.find(ch) != std::string::npos;
}

std::string EnglishStemmer::transform(const std::string& word) const
{
	std::string result = word;
	if (result[0] == 'y')
		result[0] = 'Y';

	for (size_t i = 1; i < result.length(); ++i)
	{
		if (result[i] == 'y' && isVowel(result[i - 1]))
			result[i] = 'Y';
	}

	return result;
}

std::string EnglishStemmer::isSpecial(const std::string& word) const
{
	static const std::unordered_map<std::string, std::string> exceptions = {
		{ "skis", "ski" },
		{ "skies", "sky" },
		{ "dying", "die" },
		{ "lying", "lie" },
		{ "tying", "tie" },
		{ "idly", "idl" },
		{ "gently", "gentl" },
		{ "ugly", "ugli" },
		{ "early", "earli" },
		{ "only", "onli" },
		{ "singly", "singl" }
	};

	auto ex = exceptions.find(word);
	if (ex != exceptions.end())
		return ex->second;

	return word.size() >= 3 && word.size() <= 5
		&& (word == "sky" || word == "news" || word == "howe"
			|| word == "atlas" || word == "cosmos" || word == "bias"
			|| word == "andes") ? word : "";
}

bool EnglishStemmer::replace(std::string& word, const std::string& suffix, const std::string& replacement, size_t start) const
{
	if (suffix.size() > word.size())
		return false;
	size_t last = word.length() - suffix.length();
	if (last < start)
		return false;

	auto pos = word.find(suffix, last);
	if (pos != std::string::npos)
	{
		word.replace(pos, suffix.size(), replacement);
		return true;
	}
	return false;
}

std::string EnglishStemmer::step0(const std::string& word) const
{
	std::string result = word;
	replace(result, "'s'", "", 0) || replace(result, "'s", "", 0) || replace(result, "'", "", 0);

	return result;
}

std::string EnglishStemmer::step1A(const std::string& word) const
{
	std::string result = word;
	if (!replace(result, "sses", "ss", 0))
	{
		if (boost::ends_with(result, "ied") || boost::ends_with(result, "ies"))
		{
			// if preceded by only one letter
			if (word.size() <= 4)
				result.pop_back();
			else
			{
				result.pop_back();
				result.pop_back();
			}
		}
		else if (boost::ends_with(result, "s") && !boost::ends_with(result, "us")
			&& !boost::ends_with(result, "ss"))
		{
			if (result.size() > 2 && containsVowel(result.begin(), result.end() - 2))
				result.pop_back();
		}
	}
	return result;
}

std::string EnglishStemmer::step1B(const std::string& word, size_t r1) const
{
	std::string result = word;
	bool exists = boost::ends_with(word, "eedly") || boost::ends_with(word, "eed");
	if (exists)
	{
		replace(result, "eedly", "ee", r1) || replace(result, "eed", "ee", r1);
	}
	else
	{
		size_t size = result.size();
		bool deleted = (containsVowel(result.begin(), result.end() - 2) && replace(result, "ed", "", 0))
			|| (containsVowel(result.begin(), result.end() - 4) && replace(result, "edly", "", 0))
			|| (containsVowel(result.begin(), result.end() - 3) && replace(result, "ing", "", 0))
			|| (containsVowel(result.begin(), result.end() - 5) && replace(result, "ingly", "", 0));

		if (deleted && (boost::ends_with(word, "at") || boost::ends_with(word, "bl") || boost::ends_with(word, "iz")))
		{
			result.push_back('e');
		}
		else if (deleted && std::any_of(_doubles.begin(), _doubles.end(), [result](const auto& end) {
			return boost::ends_with(result, end);
		}))
		{
			result.pop_back();
		}
		else if (deleted && r1 == word.size() && isShort(word))
		{
			result.push_back('e');
		}
	}
	return result;
}

std::string EnglishStemmer::step1C(const std::string& word) const
{
	std::string result = word;
	size_t size = result.size();
	if (size > 2 && (result[size - 1] == 'y' || result[size - 1] == 'Y'))
		if (!isVowel(result[size - 2]))
			result[size - 1] = 'i';

	return result;
}

std::string EnglishStemmer::step2(const std::string& word, size_t r1) const
{
	static const std::pair<std::string, std::string> subs[] = { 
		{ "ational", "ate" },
		{ "tional", "tion" },
		{ "enci", "ence" },
		{ "anci", "ance" },
		{ "abli", "able" },
		{ "entli", "ent" },
		{ "izer", "ize" },
		{ "ization", "ize" },
		{ "ation", "ate" },
		{ "ator", "ate" },
		{ "alism", "al" },
		{ "aliti", "al" },
		{ "alli", "al" },
		{ "fulness", "ful" },
		{ "ousli", "ous" },
		{ "ousness", "ous" },
		{ "iveness", "ive" },
		{ "iviti", "ive" },
		{ "biliti", "ble" },
		{ "bli", "ble" },
		{ "fulli", "ful" },
		{ "lessli", "less" }
	};

	std::string result = word;
	for (const auto& sub : subs)
		if (replace(result, sub.first, sub.second, r1))
			return result;

	if (!replace(result, "logi", "log", r1))
	{
		if (boost::ends_with(result, "li") && !boost::ends_with(result, "abli")
			&& !boost::ends_with(result, "entli") && !boost::ends_with(result, "aliti")
			&& !boost::ends_with(result, "alli") && !boost::ends_with(result, "ousli")
			&& !boost::ends_with(result, "bli") && !boost::ends_with(result, "fulli")
			&& !boost::ends_with(result, "lessli"))
		{
			if (result.size() > 3 && result.size() - 2 >= r1
				&& _validEndings.find(result[result.size() - 3]) != std::string::npos)
			{
				result.pop_back();
				result.pop_back();
			}
		}
	}
	return result;
}

std::string EnglishStemmer::step3(const std::string& word, size_t r1, size_t r2) const
{
	static const std::pair<std::string, std::string> subs[] = 
	{ 
		{ "ational", "ate" },
		{ "tional", "tion" },
		{ "alize", "al" },
		{ "icate", "ic" },
		{ "iciti", "ic" },
		{ "ical", "ic" },
		{ "ful", "" },
		{ "ness", "" } 
	};
	std::string result = word;
	for (auto& sub : subs)
		if (replace(result, sub.first, sub.second, r1))
			return result;

	replace(result, "ative", "", r2);

	return result;
}

std::string EnglishStemmer::step4(const std::string& word, size_t r2) const
{
	static const std::pair<std::string, std::string> subs[] =
	{
		{ "al", "" },
		{ "ance", "" },
		{ "ence", "" },
		{ "er", "" },
		{ "ic", "" },
		{ "able", "" },
		{ "ible", "" },
		{ "ant", "" },
		{ "ement", "" },
		{ "ment", "" },
		{ "ent", "" },
		{ "ism", "" },
		{ "ate", "" },
		{ "iti", "" },
		{ "ous", "" },
		{ "ive", "" },
		{ "ize", "" },
		{ "sion", "s" },
		{ "tion", "t" }
	};

	std::string result = word;
	for (auto& sub : subs)
		if (replace(result, sub.first, sub.second, r2))
			return result;

	return result;
}

std::string EnglishStemmer::step5(const std::string& word, size_t r1, size_t r2) const
{
	size_t size = word.length();
	std::string result = word;
	if (result.back() == 'e')
	{
		if (size - 1 >= r2 || (size - 1 >= r1 && !isShort(word.substr(0, size - 1))))
			result.pop_back();
	}
	else if (result.back() == 'l')
	{
		if (result.length() - 1 >= r2 && result[result.length() - 2] == 'l')
			result.pop_back();
	}

	return result;
}

bool EnglishStemmer::containsVowel(std::string::const_iterator begin, std::string::const_iterator end) const
{
	return std::find_if(begin, end, [this](char c) {
		return isVowel(c);
	}) != end;
}

bool EnglishStemmer::isShort(const std::string& word) const
{
	size_t size = word.size();

	if (size >= 3)
	{
		if (!isVowel(word[size - 3]) && isVowel(word[size - 2])
			&& !isVowel(word[size - 1]) && word[size - 1] != 'w'
			&& word[size - 1] != 'x' && word[size - 1] != 'Y')
			return true;
	}
	return size == 2 && isVowel(word[0]) && !isVowel(word[1]);
}
