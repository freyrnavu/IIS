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
			return "�������";
		case PartOfSpeech::interjection:
			return "����������";
		case PartOfSpeech::pronoun:
			return "�����������";
		case PartOfSpeech::cardinal_number:
			return "������������";
		case PartOfSpeech::short_adjective:
			return "��������������";
		case PartOfSpeech::substantive:
			return "���������������";
		case PartOfSpeech::adjective:
			return "��������������";
		case PartOfSpeech::verb:
			return "������";
		case PartOfSpeech::preposition:
			return "�������";
		case PartOfSpeech::conjunction:
			return "����";
		case PartOfSpeech::sub_conjunction:
			return "����";
		case PartOfSpeech::adverb:
			return "�������";
		case PartOfSpeech::participle:
			return "���������";
		case PartOfSpeech::possessive_adjectives:
			return "��������������";
		case  PartOfSpeech::adverbs_degree:
			return "�������";
		}
		return "����� �� ������� � �������";
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
		return "��������������";
	else if (_ans.Voice == evPassive)
		return "�������������";

	return "";
}

std::string WordInfoLab::getAspectString()
{
	if (_ans.Aspect == easPerfect)
		return "����������� ���";
	else if (_ans.Aspect == easImperfect)
		return "������������� ���";

	return "";
}

std::string WordInfoLab::getAnimationString()
{
	switch (_ans.Animation)
	{
	case eaAnimated:
		return "������������";
	case eaUnAnimated:
		return "��������������";
	}
	return "";
}

std::string WordInfoLab::getNumberString()
{
	switch (_ans.Number)
	{
	case enSingle:
		return "������������ �����";
	case enMultiple:
		return "������������� �����";
	}
	return "";
}

std::string WordInfoLab::getReflectionString()
{
	switch (_ans.Reflection)
	{
	case erReflexive:
		return "����������";
	case erNonreflexive:
		return "������������";
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
		return "C������������ �������";
	else if (_ans.ComparativeDegree == ecdStrong)
		return "������������ �������";
	return "";
}

std::string WordInfoLab::getTimeString() const
{
	switch (_ans.Time)
	{
	case etPresent:
		return "��������� �����";
	case etFuture:
		return "��������� �����";
	case etPast:
		return "������� �����";
	}
	return "";
}

std::string WordInfoLab::getPersonString() const
{
	switch (_ans.Person)
	{
	case epFirst:
		return "1 ����";
	case epSecond:
		return "2 ����";
	case epThird:
		return "3 ����";
	}
	return "";
}

std::string WordInfoLab::getCaseString() const
{
	switch (_ans.Case)
	{
	case ecNominative:
		return "������������ �����";
	case ecGenitive:
	case ecSecondGenitive:
		return "����������� �����";
	case ecDative:
		return "��������� �����";
	case ecAccusative:
		return "����������� �����";
	case ecInstrumental:
		return "������������ ����� ";
	case ecPrepositional:
	case ecSecondPrepositional:
		return "���������� �����";
	}
	return "";
}

std::string WordInfoLab::getGenderString() const
{
	if (_ans.Gender == egMasculine)
		return "������� ���";
	else if (_ans.Gender == egFeminine)
		return "������� ���";
	else if (_ans.Gender == egNeuter)
		return "������� ���";

	return "";
}
