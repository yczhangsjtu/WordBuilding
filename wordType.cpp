//#include <iostream>
#include <cassert>
#include <cstring>
#include <fstream>
#include <sstream>
#include "wordType.h"

using namespace std;

void clearConnect(Word * word)
{
	if(word==NULL) return;
	for(int i=0; i<MaxWordLength; i++)
		(word->connects)[i] = NULL;
}

bool connect
(Word * left, int index1, Word * right, int index2, const char sentence[])
{
	assert(index1<MaxWordLength && index1>=0);
	assert(index2<MaxWordLength && index2>=0);
	char c = (left->letters)[index1];
	if((right->letters)[index2]!=c) return false;
	int x, y;
	if(left->hor && !(right->hor))
	{
		x = left->x + index1;
		y = left->y;
		if(x!=(right->x) || y!=(right->y)-index2) return false;
	}
	else if(!(left->hor) && right->hor)
	{
		x = left->x;
		y = left->y - index1;
		if(y!=(right->y) || x!=(right->x)+index2) return false;
	}
	else return false;
	if((left->connects)[index1]==NULL && (right->connects)[index2]==NULL)
	{
		(left->connects)[index1] = new Connect;
		(right->connects)[index2] = (left->connects)[index1];
		(left->connects)[index1]->letter = c;
		(left->connects)[index1]->x = x;
		(left->connects)[index1]->y = y;
		(left->connects)[index1]->left = left;
		(left->connects)[index1]->right = right;
		strcpy((left->connects)[index1]->sentence,sentence);
	}
	return true;
}

//The order of deletion must be from left to right
void deleteWord(Word * word)
{
	if(word==NULL) return;
	for(int i=0; i<MaxWordLength; i++)
	{
		if((word->connects)[i]!=NULL)
		{
			if(((word->connects)[i]->left)==word)
			{
				((word->connects)[i]->left)=NULL;
				deleteWord((word->connects)[i]->right);
				delete (word->connects)[i];
			}
		}
	}
	delete word;
}

bool addConnectTo
(Word * word, int index1, const char newWord[], int index2, const char sentence[])
{
	Word * newword = new Word;
	if(word->hor)
	{
		newword->x = word->x + index1;
		newword->y = word->y + index2;
	}
	else
	{
		newword->x = word->x - index2;
		newword->y = word->y - index1;
	}
	strcpy(newword->letters,newWord);
	newword->l = strlen(newWord);
	newword->hor = !(word->hor);
	clearConnect(newword);
	if(!connect(word,index1,newword,index2,sentence))
	{
		delete newword;
		return false;
	}
	else
		return true;
}

bool readRule(const string & rule, string & sentence,
	string & left, string & right, int &index1, int &index2)
{
	istringstream is(rule);
	left=right="";
	index1=index2=-1;
	is >> left >> right >> index1 >> index2;
	if(left=="" || right=="" || index1==-1 || index2==-1) return false;
	sentence = is.str().substr(is.tellg());
	sentence = sentence.substr(1);
	return true;
}

void readWords(Word * &word, const char * fileName)
{
	ifstream fin(fileName);
	if(!fin) return;
	
	map<string,Word*> dict;
	
	word = new Word;
	int tmp;
	fin >> word->letters;
	fin >> (word->x) >> (word->y);
	fin >> tmp;
	word->hor = tmp==1;
	word->l = strlen(word->letters);
	clearConnect(word);
	dict[string(word->letters)] = word;
	string rule;
	string str;
	fin.ignore();
	while(!fin.eof())
	{
		getline(fin,rule);
		string left, right, sentence;
		int index1, index2;
		if(!readRule(rule,sentence,left,right,index1,index2)) continue;
		map<string,Word*>::iterator iter = dict.find(left);
		if(iter==dict.end()) continue;
		bool state =
			addConnectTo(iter->second,index1,right.c_str(),index2,sentence.c_str());
		if(state) dict[right]=iter->second->connects[index1]->right;
	}
	
	fin.close();
}

void addWordToMap(Word * word, map<ipair,Word*> & wordMap)
{
	ipair ip = {word->x,word->y};
	wordMap[ip] = word;
	for(int i=0; i<MaxWordLength; i++)
	{
		if(word->connects[i]!=NULL)
		{
			if(word!=((word->connects[i])->right))
				addWordToMap((word->connects[i])->right,wordMap);
		}
	}
}

void addConnectToMap(Word * word, map<ipair,Connect*> & connectMap)
{
	for(int i=0; i<MaxWordLength; i++)
	{
		if(word->connects[i]!=NULL)
		{
			ipair ip = {word->connects[i]->x,word->connects[i]->y};
			connectMap[ip] = word->connects[i];
			map<ipair,Connect*>::iterator iter;
			if(word!=((word->connects[i])->right))
				addConnectToMap((word->connects[i])->right,connectMap);
		}
	}
}

bool operator<(const ipair& left, const ipair& right)
{
	if(left.x < right.x) return true;
	if(left.x > right.x) return false;
	if(left.y < right.y) return true;
	return false;
}


void outputWord(const Word & word,ofstream & fout,bool root)
{
	if(root) fout<< (word.letters) << ' ' << (word.x) << ' ' << (word.y) << ' '
		<< ((word.hor) ? 1 : 0) <<endl;
	for(int i=0; i<MaxWordLength; i++)
	{
		if(word.connects[i]!=NULL)
		{
			if(&word!=((word.connects[i])->right))
			{
				fout << (word.letters) << ' '
					 << ((word.connects[i])->right->letters) << ' '
					 << i << ' ';
				for(int j=0; j<MaxWordLength; j++)
					if((word.connects[i])->right->connects[j]==(word.connects[i]))
					{
						fout << j << ' ';
						break;
					}
				fout << (word.connects[i])->sentence << endl;
				outputWord(*((word.connects[i])->right),fout,false);
			}
		}
	}
}