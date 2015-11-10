#ifndef _WORDTYPE_H
#define _WORDTYPE_H
#include <string>
#include <map>

using namespace std;

const int MaxWordLength = 32;
const int MaxSentenceLength = 1024;

struct Connect;
typedef struct Connect Connect;

struct Word
{
	int x;//Coordinate of the word on the map
	int y;//which is the position of the leading letter
	int l;//Length of the word
	bool hor;//Direction of the word, true means horizontal, otherwise vertical
	char letters[MaxWordLength+1];//Buffer for the word
	Connect * connects[MaxWordLength+1];
};

typedef struct Word Word;

typedef struct
{
	int x;
	int y;
} ipair;

//Connection between two words
struct Connect
{
	char letter;//The common letter of the two words
	int x;
	int y;
	Word * left;//Pointer to the words
	Word * right;
	//A connection should contain a sentence involving both words
	char sentence[MaxSentenceLength];
};

void clearConnect(Word * word);
bool connect
(Word * left, int index1, Word * right, int index2, const char sentence[]);
void deleteWord(Word * word);
bool addConnectTo
(Word * word, int index1, const char newWord[], int index2, const char sentence[]);
bool readRule(const string & rule, string & sentence,
	string & left, string & right, int &index1, int &index2);
void readWords(Word * &word, const char * fileName);
void addWordToMap(Word * word, map<ipair,Word*> & wordMap);
void addConnectToMap(Word * word, map<ipair,Connect*> & connectMap);
bool operator<(const ipair& left, const ipair& right);
void outputWord(const Word & word,ofstream & fout,bool root);
#endif