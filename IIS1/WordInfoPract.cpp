#include "WordInfoPract.h"
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
		return "Слово не найдено в словаре";
	}

	PartOfSpeech convert(EnSClass en)
	{
		switch (en)
		{
		case Particles:
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

WordInfoLab::WordInfoLab(const std::string& word, const TMorfAnswer& ans) : _word(word)
	, _ans(ans)
{
	_partOfSpeech = convert(_ans.SClass);
}

std::string WordInfoLab::getVoiceString()
{
	if (_ans.Voice == evActive)
		return "Действительное";
	else if (_ans.Voice == evPassive)
		return "Страдательное";

	return "";
}

std::string WordInfoLab::getAspectString()
{
	if (_ans.Aspect == easPerfect)
		return "Совершенный вид";
	else if (_ans.Aspect == easImperfect)
		return "Несовершенный вид";

	return "";
}

std::string WordInfoLab::getAnimationString()
{
	switch (_ans.Animation)
	{
	case eaAnimated:
		return "Одушевленное";
	case eaUnAnimated:
		return "Неодушевленное";
	}
	return "";
}

std::string WordInfoLab::getNumberString()
{
	switch (_ans.Number)
	{
	case enSingle:
		return "Единственное число";
	case enMultiple:
		return "Множественное число";
	}
	return "";
}

std::string WordInfoLab::getReflectionString()
{
	switch (_ans.Reflection)
	{
	case erReflexive:
		return "Возвратное";
	case erNonreflexive:
		return "Невозвратное";
	}

	return "";
}

std::string WordInfoLab::getWord() const
{
	return _word;
}

std::string WordInfoLab::getPartOfSpeechString() const
{
	return partOfSpeechToString(_partOfSpeech);
}

std::string WordInfoLab::getMainForm() const
{
	return _ans.m_MainForm;
}

std::string WordInfoLab::getComparativeString() const
{
	if (_ans.ComparativeDegree == ecdWeak)
		return "Cравнительная степень";
	else if (_ans.ComparativeDegree == ecdStrong)
		return "Превосходная степень";
	return "";
}

std::string WordInfoLab::getTimeString() const
{
	switch (_ans.Time)
	{
	case etPresent:
		return "Настоящее время";
	case etFuture:
		return "Прошедшее время";
	case etPast:
		return "Будущее время";
	}
	return "";
}

std::string WordInfoLab::getPersonString() const
{
	switch (_ans.Person)
	{
	case epFirst:
		return "1 лицо";
	case epSecond:
		return "2 лицо";
	case epThird:
		return "3 лицо";
	}
	return "";
}

std::string WordInfoLab::getCaseString() const
{
	switch (_ans.Case)
	{
	case ecNominative:
		return "Именительный падеж";
	case ecGenitive:
	case ecSecondGenitive:
		return "Родительный падеж";
	case ecDative:
		return "Дательный падеж";
	case ecAccusative:
		return "Винительный падеж";
	case ecInstrumental:
		return "Творительный падеж ";
	case ecPrepositional:
	case ecSecondPrepositional:
		return "Предложный падеж";
	}
	return "";
}

std::string WordInfoLab::getGenderString() const
{
	if (_ans.Gender == egMasculine)
		return "Мужской род";
	else if (_ans.Gender == egFeminine)
		return "Женский род";
	else if (_ans.Gender == egNeuter)
		return "Средний род";

	return "";
}
