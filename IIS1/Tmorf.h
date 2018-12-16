#include <atlstr.h>
// TMorf
#ifndef __CMORF_H     /* prevent multiple includes */
#define __CMORF_H

//#include _T"cbyte.h_T"

struct  mylinfo {
	int	curword;
   int	sik;
	int	OkFormCount;
   int	basesize;
   int	maxtrans;
//   int	maxphrase;
   int	mainlgk;  // for participles = P_VERB
   int	lgk;      // for participles = P_ADJECTIVE
//   unsigned char match;
   unsigned long result;
	unsigned long m_DuplicateForm;
   long	wordnum;
   unsigned long offset;
   int	wordcase;
   int	basecase;   // case in stem dictionary
   int	prich_type;
//	int	used;
//	int	subject;
	int	yo_to_e_replacement_position;
	int	e_to_yo_replacement_position;
//	Cbyte stem;
   struct mylinfo *next;  // this element should be the last in structure !!!
   };


enum EnSClass { Particles, Interjections, PersonalPronouns, CardinalNumbers,
                PossessiveAdjectives, ShortAdjectives, Substantives,
                Adjectives, Verbs, Prepositions, Conjunctions,
                Adverbs, Predicates, PunctuationMarks, ShortParticiples,
                CompDegreeAdjectives, Participles, AdverbialParticiples, 
                   
                /* Мои добавления */
                Pronouns,         // Местоимения-сущ. типа _T"кто-то_T"
                SubConjunctions,  // Подчинительные союзы
                AdverbsDegree,     // Наречия типа степени
				StateCategory
                // Как обозначается т. н. _T"категория состояния_T"?
					 ,esUndefined //pom
};

enum EnAnimation { eaAny, eaAnimated, eaUnAnimated, eaUndefined };

enum EnGender { egAny, egMasculine, egFeminine, egNeuter, egUndefined };

enum EnNumber { enAny, enSingle, enMultiple, enUndefined };

enum EnCase { ecAny, ecNominative, ecGenitive, ecDative,
              ecAccusative, ecInstrumental, ecPrepositional,
              ecSecondGenitive, ecSecondPrepositional, ecUndefined };

enum EnAspect { easAny, easImperfect, easPerfect, easUndefined };

enum EnPerson { epAny, epFirst, epSecond, epThird, epUndefined };

enum EnVoice { evAny, evActive, evPassive, evUndefined };

enum EnReflection { erAny, erReflexive, erNonreflexive, erUndefined };

enum EnTime { etAny, etPresent, etFuture, etPast, etUndefined };

enum EnMood { emAny, emPresent, emFuture, emPast, emSubjunctive,
              emImperative, emInfinitive, emUndefined };

enum EnComparativeDegree { ecdAny, ecdStrong, ecdWeak, ecdUndefined };

enum EnAlternativeForm { ealAny, ealYes, ealUndefined };

struct TMorfAnswerList
{
  struct TMorfAnswer* Answers;
  int size;
};

struct TCaseFrame
{
  char* Trans;
  char* Case_Frame;
};

struct TMorfAnswer
{
  char m_MainForm[50];
  enum EnSClass SClass;
  enum EnAnimation Animation;
  enum EnGender Gender;
  enum EnNumber Number;
  enum EnCase Case;
  enum EnAlternativeForm Alternative;
  enum EnAspect Aspect;
  enum EnPerson Person;
  enum EnVoice Voice;
  enum EnReflection Reflection;
  enum EnTime Time;
  enum EnMood Mood;
  enum EnComparativeDegree ComparativeDegree;
  struct TCaseFrame* CaseFrames;
  int size;
  DWORD wordnum;
  struct mylinfo m_MyLinfo;
};


#endif	// __CMORF_H
