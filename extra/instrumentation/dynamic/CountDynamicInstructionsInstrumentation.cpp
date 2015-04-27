#include <stdio.h>
#include <map>

using namespace std;

map <char *, int> dynamicInstCounter;

void Record(char* opcode, int count)
{
	dynamicInstCounter[opcode] += count;
}

void PrintInstructionCount()
{
	int sum = 0;
	for(map<char*, int>::iterator iterator = dynamicInstCounter.begin(); iterator != dynamicInstCounter.end(); ++iterator) {
		printf("%-20s\t%d\n", iterator->first, iterator->second);
		sum += iterator->second;
	}
	printf("%-20s\t%d\n", "TOTAL", sum);
}
