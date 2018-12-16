#pragma once
#include <functional>
#include <map>
#include <boost\algorithm\string\case_conv.hpp>
#include "WordInfoPract.h"

struct CaseInsensitiveComparator
{
	bool operator()(const std::string& left, const std::string& right) const
	{
		return boost::to_lower_copy(left, std::locale("rus")) < boost::to_lower_copy(right, std::locale("rus"));
	}
};

using DictionaryMap = std::multimap<std::string, WordInfoLab, CaseInsensitiveComparator>;