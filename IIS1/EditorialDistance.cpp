#include "EditorialDistance.h"

int EditorialDist::findEditDist(const std::string& firstWord, const std::string& secondWord)
{
	std::string s1 = ' ' + firstWord;
	std::string s2 = ' ' + secondWord;
	for (unsigned i = 0; i < s1.size(); i++)
		a[i][0] = i;
	for (unsigned i = 0; i < s2.size(); i++)
		a[0][i] = i;

	for (unsigned i = 1; i < s1.size(); i++)
		for (unsigned j = 1; j < s2.size(); j++)
			if (s1[i] == s2[j])
				t[i][j] = 0;
			else
				t[i][j] = 1;

	for (unsigned i = 1; i < s1.size(); i++)
		for (unsigned j = 1; j < s2.size(); j++)
			a[i][j] = Min(a[i - 1][j] + 1, a[i][j - 1] + 1, a[i - 1][j - 1] + t[i][j]);

	return a[s1.size() - 1][s2.size() - 1];
}

int EditorialDist::Min(int a, int b, int c)
{
	if (a > b) std::swap(a, b);
	if (b > c) std::swap(b, c);
	if (a > b) std::swap(a, b);

	return a;
}
