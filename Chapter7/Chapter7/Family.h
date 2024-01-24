#pragma once
#include <string>
using namespace std;
class Family
{
public:
	int keyFamily;
	string familyName;
	int countChildren;
	long phon;
	bool isChug[6];

	Family() {
		keyFamily=0;
		familyName="                    ";
		countChildren=0;
		phon=0;
		for (int i = 0; i < 6; i++)
		{
			isChug[i] = false;
		}
	}
};

