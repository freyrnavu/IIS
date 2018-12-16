#include "LibraryManager.h"
#include "LibraryNotFoundException.h"


LibraryManager::LibraryManager(const std::string& libName)
{
	_hLib = LoadLibrary(libName.c_str());
	if (!_hLib)
		throw LibraryNotFoundException("Library not found " + libName);
}


LibraryManager::~LibraryManager()
{
	FreeLibrary(_hLib);
}

HINSTANCE LibraryManager::getLibrary() const
{
	return _hLib;
}
