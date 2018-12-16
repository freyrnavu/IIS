#pragma once
#include <string>
#include "Tmorf.h"
#include <vector>


enum class PartOfSpeech;
enum class Case
{
	undefined,
	nominative = 1,
	genitive = 2,
	dative = 3,
	accusative = 4,
	instrumental = 5,
	prepositional = 6
};

class WordInfo
{
public:
	WordInfo(const std::string& word, const TMorfAnswer& ans);
	~WordInfo() = default;

	std::string getWord() const;

	std::string getWordInfo() const;

private:
	std::string getComparativeCode(const TMorfAnswer& ans) const;

	std::string getTimeCode(const TMorfAnswer& ans) const;

	std::string getPersonCode(const TMorfAnswer& ans) const;

	std::string getCaseCode(const TMorfAnswer& ans) const;

	std::string getGenderCode(const TMorfAnswer& ans) const;

	std::string toString(const TMorfAnswer& ans);

	std::string getVoiceCode(const TMorfAnswer &ans);

	std::string getAspectCode(const TMorfAnswer &ans);

	std::string getAnimationCode(const TMorfAnswer &ans);

private:
	PartOfSpeech _partOfSpeech;
	std::string _word;
	std::string _wordInfo;
	const std::vector<char> _delimiters = {
		':',
		',',
		'-',
		'.',
		'!',
		'(',
		';',
		')',
		'?',
		'\\',
		'\'',
		'–',
		'\n',
		'«',
		'»'
	};
};

