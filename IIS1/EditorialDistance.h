#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <array>

class EditorialDist {
	std::array<std::array<int, 100>, 100> a;
	std::array<std::array<int, 100>, 100> t;
public:
	int findEditDist(const std::string& s1, const std::string& s2);

private:
	int Min(int a, int b, int c);
};
