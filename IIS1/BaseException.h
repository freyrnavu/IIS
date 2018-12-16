#pragma once
#include <exception>

class BaseException : public std::exception
{
public:
	BaseException(std::string message) : std::exception(message.c_str())
	{
	};
};