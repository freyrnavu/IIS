#include "StemmerFactory.h"
#include "RussianStemmer.h"
#include "EnglishStemmer.h"

std::unique_ptr<IStemmer> StemmerFactory::createRussianStemmer()
{
	return std::make_unique<RussianStemmer>();
}

std::unique_ptr<IStemmer> StemmerFactory::createEnglishStemmer()
{
	return std::make_unique<EnglishStemmer>();
}
