#pragma once
#include <string>
#include "Tmorf.h"
#include <vector>

enum class PartOfSpeech;

class WordInfoLab
{
public:
	WordInfoLab(const std::string& word, const TMorfAnswer& ans);
	~WordInfoLab() = default;

	std::string getWord() const;

	std::string getPartOfSpeechString() const;

	std::string getMainForm() const;

	std::string getComparativeString() const;

	std::string getTimeString() const;

	std::string getPersonString() const;

	std::string getCaseString() const;

	std::string getGenderString() const;
	
	std::string getVoiceString();

	std::string getAspectString();

	std::string getAnimationString();
	
	std::string getNumberString();

	std::string getReflectionString();

private:
	PartOfSpeech _partOfSpeech;
	std::string _word;
	TMorfAnswer _ans;
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

