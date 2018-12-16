#pragma once
#include "IStemmer.h"

class RussianStemmer : public IStemmer
{
public:
	std::string stem(const std::string& word) const override;

private:
	size_t findRV(const std::string& word) const;
	size_t findR1(const std::string& word) const;
	size_t findR2(const std::string& word, size_t r1Pos) const;

	std::string firstStep(const std::string& word, size_t r2Pos) const;
	std::string secondStep(const std::string& word, size_t r2Pos) const;
	std::string thirdStep(const std::string& word, size_t r2Pos) const;
	std::string fourthStep(const std::string& word, size_t r2Pos) const;

	struct Result
	{
		bool match = false;
		std::string result;
	};

	Result cutWithRegexpr(const std::string& word, const std::string& regexpr, size_t r2Pos, bool haveGroups = false) const;
private:
	const std::string _vowelsRegexp = "[���������]";
	const std::string _nonVowelsRegexp = "[^���������]";
	const std::string _perfectGerundRegexp = "(([��](�|���|�����))|(��|����|������|��|����|������))$";
	const std::string _adjectivesRegexp = "(��|��|��|��|���|���|��|��|��|��|��|��|��|��|���|���|���|���|��|��|��|��|��|��|��|��)$";
	const std::string _participleRegexp = "(([��](��|��|��|��|�))|(���|���|���))$";
	const std::string _reflexiveRegexp = "(��|��)$";
	const std::string _verbRegexp = "(([��](��|��|���|���|��|�|�|��|�|��|��|��|��|��|��|���|���))|"
		"(���|���|���|����|����|���|���|���|��|��|��|��|��|��|��|���|���|���|��|���|���|��|��|���|���|���|���|��|�))$";
	const std::string _nounRegexp = "(�|��|��|��|��|�|����|���|���|��|��|�|���|��|��|��|�|���|��|���|��|��|��|�|�|��|���|��|�|�|��|��|�|��|��|�)$";
	const std::string _superlativeRegexp = "(���|����)$";
	const std::string _derivationalRegexp = "(���|����)$";
	const std::string _iRegexp = "�$";
	const std::string _bRegexp = "�$";
	const std::string _nnRegexp = "�$";
};

