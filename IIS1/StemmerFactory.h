#pragma once
#include <memory>
#include "IStemmer.h"

class StemmerFactory
{
public:
	static std::unique_ptr<IStemmer> createRussianStemmer();
	static std::unique_ptr<IStemmer> createEnglishStemmer();
};

