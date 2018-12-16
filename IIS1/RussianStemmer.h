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
	const std::string _vowelsRegexp = "[аеиоуыэю€]";
	const std::string _nonVowelsRegexp = "[^аеиоуыэю€]";
	const std::string _perfectGerundRegexp = "(([а€](в|вши|вшись))|(ив|ивши|ившись|ыв|ывши|ывшись))$";
	const std::string _adjectivesRegexp = "(ее|ие|ые|ое|ими|ыми|ей|ий|ый|ой|ем|им|ым|ом|его|ого|ему|ому|их|ых|ую|юю|а€|€€|ою|ею)$";
	const std::string _participleRegexp = "(([а€](ем|нн|вш|ющ|щ))|(ивш|ывш|ующ))$";
	const std::string _reflexiveRegexp = "(с€|сь)$";
	const std::string _verbRegexp = "(([а€](ла|на|ете|йте|ли|й|л|ем|н|ло|но|ет|ют|ны|ть|ешь|нно))|"
		"(ила|ыла|ена|ейте|уйте|ите|или|ыли|ей|уй|ил|ыл|им|ым|ен|ило|ыло|ено|€т|ует|уют|ит|ыт|ены|ить|ыть|ишь|ую|ю))$";
	const std::string _nounRegexp = "(а|ев|ов|ие|ье|е|и€ми|€ми|ами|еи|ии|и|ией|ей|ой|ий|й|и€м|€м|ием|ем|ам|ом|о|у|ах|и€х|€х|ы|ь|ию|ью|ю|и€|ь€|€)$";
	const std::string _superlativeRegexp = "(ейш|ейше)$";
	const std::string _derivationalRegexp = "(ост|ость)$";
	const std::string _iRegexp = "и$";
	const std::string _bRegexp = "ь$";
	const std::string _nnRegexp = "ь$";
};

