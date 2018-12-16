#pragma once
#include <wtypes.h>
#include <string>

class LibraryManager
{
public:
	LibraryManager(const std::string& libName);
	~LibraryManager();

	HINSTANCE getLibrary() const;
private:
	HINSTANCE _hLib;
};

