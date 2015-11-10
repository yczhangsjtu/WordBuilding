#ifndef _DRAW_H
#define _DRAW_H

#include <cstring>
#include "wordType.h"

using namespace std;

const GLfloat MaxViewX = 13500;
const GLfloat MaxViewY = 7000;

const GLint WindowWidth = 1350;
const GLint WindowHeight = 700;
const GLfloat letterSize = 0.8;
const GLfloat letterx = 0.2;
const GLfloat lettery = 0.4;
const GLfloat sentenceSize = 0.6;
const GLfloat sentenceBackHeight = 1.5;

const GLfloat scaleLeft = 0;
const GLfloat scaleWidth = (GLfloat)WindowWidth;
const GLfloat scaleBottom = 0;
const GLfloat scaleHeight = (GLfloat)WindowHeight;
const GLint rowWordNumber = 5;

const GLfloat gridSize = 30;

void drawBackground(
	GLfloat viewx,
	GLfloat viewy,
	GLfloat bgLineColor[],
	GLfloat highlightColor[],
	GLfloat currScaleWidth,
	GLfloat currScaleHeight,
	int imousex,
	int imousey
);

void drawWord(
	const Word & word,
	GLfloat wordsColor[],
	GLfloat viewx,
	GLfloat viewy
);

void drawLetter(
	char letter,
	int ix,
	int iy,
	GLfloat letterColor[],
	GLfloat viewx,
	GLfloat viewy
);

void drawSentence(
	char sentence[MaxSentenceLength],
	GLfloat sentenceColor[],
	GLfloat sentenceBackColor[],
	GLint ix,
	GLint iy,
	GLfloat viewx,
	GLfloat viewy
);

void drawText(char text[],GLfloat textColor[]);
#endif