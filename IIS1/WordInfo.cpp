#include "WordInfo.h"
#include <sstream>
#include "PartOfSpeech.h"

namespace
{
	std::string partOfSpeechToString(PartOfSpeech p)
	{
		switch (p)
		{
		case PartOfSpeech::particle:
			return "Частица";
		case PartOfSpeech::interjection:
			return "Междометие";
		case PartOfSpeech::pronoun:
			return "Местоимение";
		case PartOfSpeech::cardinal_number:
			return "Числительное";
		case PartOfSpeech::short_adjective:
			return "Прилагательное";
		case PartOfSpeech::substantive:
			return "Существительное";
		case PartOfSpeech::adjective:
			return "Прилагательное";
		case PartOfSpeech::verb:
			return "Глагол";
		case PartOfSpeech::preposition:
			return "Предлог";
		case PartOfSpeech::conjunction:
			return "Союз";
		case PartOfSpeech::sub_conjunction:
			return "Союз";
		case PartOfSpeech::adverb:
			return "Наречие";
		case PartOfSpeech::participle:
			return "Причастие";
		case PartOfSpeech::possessive_adjectives:
			return "Прилагательное";
		case  PartOfSpeech::adverbs_degree:
			return "Наречие";
		}
		return "";
	}

	PartOfSpeech convert(EnSClass en)
	{
		switch (en)
		{
		case Particles:
		case ShortParticiples:
		case AdverbialParticiples:
			return PartOfSpeech::particle;
		case Interjections:
			return PartOfSpeech::interjection;
		case PersonalPronouns:
		case Pronouns:
			return PartOfSpeech::pronoun;
		case CardinalNumbers:
			return PartOfSpeech::cardinal_number;
		case ShortAdjectives:
			return PartOfSpeech::short_adjective;
		case Substantives:
			return PartOfSpeech::substantive;
		case Adjectives:
		case CompDegreeAdjectives:
			return PartOfSpeech::adjective;
		case Verbs:
			return PartOfSpeech::verb;
		case Prepositions:
			return PartOfSpeech::preposition;
		case Conjunctions:
			return PartOfSpeech::conjunction;
		case Adverbs:
			return PartOfSpeech::adverb;
		case Participles:
			return PartOfSpeech::participle;
		case SubConjunctions:
			return PartOfSpeech::sub_conjunction;
		case PossessiveAdjectives:
			return PartOfSpeech::possessive_adjectives;
		case AdverbsDegree:
			return PartOfSpeech::adverbs_degree;
		}
		return PartOfSpeech::undefined;
	}
}

WordInfo::WordInfo(const std::string& word, const TMorfAnswer& ans) : _word(word)
{
	_partOfSpeech = convert(ans.SClass);
	_wordInfo = toString(ans);
}

std::string WordInfo::toString(const TMorfAnswer& ans)
{
	std::stringstream ss;
	if (_partOfSpeech == PartOfSpeech::undefined)
		return _word;

	auto it = std::find(_delimiters.begin(), _delimiters.end(), _word.back());
	if (it != _delimiters.end())
		_word.pop_back();

	ss << _word << "_" << partOfSpeechToString(_partOfSpeech);

	ss << getComparativeCode(ans);
	if (ans.Reflection == erReflexive)
		ss << "L";
	ss << getVoiceCode(ans);
	ss << getAnimationCode(ans);

	ss << getGenderCode(ans);
	ss << getCaseCode(ans);
	ss << getPersonCode(ans);
	ss << getTimeCode(ans);
	ss << getAspectCode(ans);
	if (it != _delimiters.end())
		ss << *it;

	return ss.str();
}

std::string WordInfo::getVoiceCode(const TMorfAnswer &ans)
{
	if (ans.Voice == evActive)
		return "A";
	else if (ans.Voice == evPassive)
		return "P";

	return "";
}

std::string WordInfo::getAspectCode(const TMorfAnswer &ans)
{
	if (ans.Aspect == easPerfect)
		return "P";
	else if (ans.Aspect == easImperfect)
		return "I";

	return "";
}

std::string WordInfo::getAnimationCode(const TMorfAnswer &ans)
{
	switch (ans.Animation)
	{
	case eaAnimated:
		return "A";
	case eaUnAnimated:
		return "I";
	}
	return "";
}

std::string WordInfo::getWord() const
{
	return _word;
}

std::string WordInfo::getWordInfo() const
{
	return _wordInfo;
}

std::string WordInfo::getComparativeCode(const TMorfAnswer& ans) const
{
	if (ans.ComparativeDegree == ecdWeak)
		return "R";
	else if (ans.ComparativeDegree == ecdStrong)
		return "T";
	return "";
}

std::string WordInfo::getTimeCode(const TMorfAnswer& ans) const
{
	switch (ans.Time)
	{
	case etPresent:
		return "R";
	case etFuture:
		return "F";
	case etPast:
		return "P";
	}
	return "";
}

std::string WordInfo::getPersonCode(const TMorfAnswer& ans) const
{
	switch (ans.Person)
	{
	case epFirst:
		return "1";
	case epSecond:
		return "2";
	case epThird:
		return "3";
	}
	return "";
}

std::string WordInfo::getCaseCode(const TMorfAnswer& ans) const
{
	switch (ans.Case)
	{
	case ecNominative:
		return "O";
	case ecGenitive:
		return "G";
	case ecDative:
		return "D";
	case ecAccusative:
		return "A";
	case ecInstrumental:
		return "I";
	case ecPrepositional:
		return "R";
	case ecSecondGenitive:
		return "G";
	case ecSecondPrepositional:
		return "R";
	}
	return "";
}

std::string WordInfo::getGenderCode(const TMorfAnswer& ans) const
{
	if (ans.Gender == egMasculine)
		return "M";
	else if (ans.Gender == egFeminine)
		return "F";
	else if (ans.Gender == egNeuter)
		return "N";
	else if (ans.Number == enMultiple)
		return "P";

	return "";
}
