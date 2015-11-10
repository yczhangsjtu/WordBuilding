#ifndef _BUILDING_H
#define _BUILDING_H

#include <cstring>
#include <map>
#include "wordType.h"

#define TITLE "Building Words"
#define WORD_FILE "words.txt"

using namespace std;

const int inputBufferLength = 4096;

const GLfloat screenRollSpeed = 1.0;
const GLint rollTime = 1000;
GLfloat screenRollvx = 0.0;
GLfloat screenRollvy = 0.0;

GLfloat currScaleWidth = (GLfloat)WindowWidth;
GLfloat currScaleHeight = (GLfloat)WindowHeight;

int ww, hh;//Record the current window size

GLfloat bgR = 1.0;//Back ground color
GLfloat bgG = 1.0;
GLfloat bgB = 0.0;
GLfloat bgA = 1.0;

GLfloat bgLineColor[] = {0.0,0.0,0.0};//Color of the background lines
GLfloat wordsColor[] = {0.0,0.0,1.0};//Color of the words
GLfloat highlightColor[] = {0.0,1.0,0.0};//Color of highlighted grid
GLfloat sentenceColor[] = {0.0,0.0,0.0};//Color of highlighted grid
GLfloat sentenceBackColor[] = {1.0,1.0,1.0};
GLfloat inputColor[]={0.0,0.0,0.0};

GLfloat viewx = 200.0;//The point that is to be drawn at left bottom corner
GLfloat viewy = 200.0;

int mousex;
int mousey;
int imousex;
int imousey;

void init();
void display();
void reshape(int w, int h);
void passiveMotion(int x, int y);
void motion(int x, int y);
void mouse(int state, int button, int x, int y);
void specialKey(int keyCode, int x, int y);
void specialKeyUp(int keyCode, int x, int y);
void key(unsigned char keyCode, int x, int y);
void idle();
void outputWords(const char * fileName);

Word * baseWord;
map<ipair,Word*> wordMap;
map<ipair,Connect*> connectMap;
char currSentence[MaxSentenceLength];
int sentencex, sentencey;
int inputPointer;
char inputBuffer[inputBufferLength];
#endif