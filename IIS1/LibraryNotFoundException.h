#pragma once
#include "BaseException.h"

class LibraryNotFoundException : public BaseException
{
	using BaseException::BaseException;
};