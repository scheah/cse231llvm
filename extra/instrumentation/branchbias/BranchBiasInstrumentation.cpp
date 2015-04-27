#include <stdio.h>
#include <map>

using namespace std;

map <char *, map <char *, int> > dynamicBranchCounter;

void Record(char* function, int condition)
{
	dynamicBranchCounter[function][(char*)"Total"]++;
	if (condition) {
		dynamicBranchCounter[function][(char*)"Taken"]++;
	}
}

void PrintBranchBias()
{
	printf("%-40s\tBias\tTaken\tTotal\n", "Function");
	double bias = 0;
	for(map<char*, map <char*, int> >::iterator iterator = dynamicBranchCounter.begin(); iterator != dynamicBranchCounter.end(); ++iterator) {
		bias = (double)(iterator->second)[(char*)"Taken"]/(double)(iterator->second)[(char*)"Total"];
		printf("%-40s\t%.2f\t%d\t%d\n", iterator->first, bias, (iterator->second)[(char*)"Taken"], (iterator->second)[(char*)"Total"]);
	}
}
