#include <GL/freeglut.h>
#include <cassert>
#include "draw.h"

void drawBackground(
	GLfloat viewx,
	GLfloat viewy,
	GLfloat bgLineColor[],
	GLfloat highlightColor[],
	GLfloat currScaleWidth,
	GLfloat currScaleHeight,
	int imousex,
	int imousey
)
{
	GLint ix = (GLint) (viewx/gridSize);
	GLint iy = (GLint) (viewy/gridSize);
	glColor3fv(bgLineColor);
	glBegin(GL_LINES);
	while(ix*gridSize <= viewx+currScaleWidth)
	{
		glVertex2f(ix*gridSize,viewy);
		glVertex2f(ix*gridSize,viewy+currScaleHeight);
		ix ++;
	}
	while(iy*gridSize <= viewy+currScaleHeight)
	{
		glVertex2f(viewx,iy*gridSize);
		glVertex2f(viewx+currScaleWidth,iy*gridSize);
		iy ++;
	}
	glEnd();
	glColor3fv(highlightColor);
	glPolygonMode(GL_FRONT, GL_FILL);
	glBegin(GL_POLYGON);
		glVertex2f(imousex*gridSize,imousey*gridSize);
		glVertex2f((imousex+1)*gridSize,imousey*gridSize);
		glVertex2f((imousex+1)*gridSize,(imousey+1)*gridSize);
		glVertex2f(imousex*gridSize,(imousey+1)*gridSize);
	glEnd();
}

void drawWord(
	const Word & word,
	GLfloat wordsColor[],
	GLfloat viewx,
	GLfloat viewy
)
{
	if(word.hor)
		for(int j = 0; j<word.l; j++)
			drawLetter(	word.letters[j],word.x+j,word.y,
						wordsColor,viewx,viewy);
	else
		for(int j = 0; j<word.l; j++)
			drawLetter(	word.letters[j],word.x,word.y-j,
						wordsColor,viewx,viewy);
	for(int i=0; i<MaxWordLength; i++)
	{
		if(word.connects[i]!=NULL)
		{
			if(&word!=((word.connects[i])->right))
				drawWord(*((word.connects[i])->right),wordsColor,viewx,viewy);
		}
	}
}

void drawLetter(
	char letter,
	int ix,
	int iy,
	GLfloat letterColor[],
	GLfloat viewx,
	GLfloat viewy
)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	glTranslatef(-viewx+ix*gridSize,-viewy+iy*gridSize,0);
	GLfloat ratio = 
		(GLfloat)gridSize/glutStrokeWidth(GLUT_STROKE_MONO_ROMAN,letter)*letterSize;
	glScalef(ratio,ratio,1);
	glTranslatef(letterx*gridSize,lettery*gridSize,0);
	glColor3fv(letterColor);
	glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,letter);
	
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void drawSentence(
	char sentence[MaxSentenceLength],
	GLfloat sentenceColor[],
	GLfloat sentenceBackColor[],
	GLint ix,
	GLint iy,
	GLfloat viewx,
	GLfloat viewy
)
{
	//Set the model view
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	int l = strlen(sentence);
	int h = glutStrokeWidth(GLUT_STROKE_MONO_ROMAN,sentence[0]);
	int w = h * rowWordNumber * 10;
	glTranslatef(-viewx+ix*gridSize,-viewy+iy*gridSize,0);
	GLfloat ratio = 
		(GLfloat)gridSize/glutStrokeWidth(GLUT_STROKE_MONO_ROMAN,sentence[0])*sentenceSize;
	glScalef(ratio,ratio,1);
	//Draw the sentence background
	glColor3fv(sentenceBackColor);
	glPolygonMode(GL_FRONT, GL_FILL);
	glBegin(GL_POLYGON);
		glVertex2f(0,-h*(sentenceBackHeight-1));
		glVertex2f(w,-h*(sentenceBackHeight-1));
		glVertex2f(w,h);
		glVertex2f(0,h);
	glEnd();
	int row = 1, wordNum = 0;
	for(int i=0; i<MaxSentenceLength && sentence[i]!='\0'; i++)
	{
		char letter = sentence[i];
		glColor3fv(sentenceColor);
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,letter);
		//Deal with end of line
		if(letter==' ') wordNum++;
		if(wordNum>row*rowWordNumber)
		{
			row ++;
			//Change the Model View
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef(-viewx+ix*gridSize,-viewy+iy*gridSize-(row-1)*gridSize,0);
			GLfloat ratio = 
				(GLfloat)gridSize/glutStrokeWidth(GLUT_STROKE_MONO_ROMAN,sentence[0])*sentenceSize;
			glScalef(ratio,ratio,1);
			//Draw the new background rectangle
			glColor3fv(sentenceBackColor);
			glPolygonMode(GL_FRONT, GL_FILL);
			glBegin(GL_POLYGON);
				glVertex2f(0,-h*(sentenceBackHeight-1));
				glVertex2f(w,-h*(sentenceBackHeight-1));
				glVertex2f(w,h);
				glVertex2f(0,h);
			glEnd();
		}
	}
	
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
}

void drawText(char text[],GLfloat textColor[])
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0,scaleHeight-gridSize,0);
	GLfloat ratio = 
		(GLfloat)gridSize/glutStrokeWidth(GLUT_STROKE_MONO_ROMAN,'a')*letterSize;
	glScalef(ratio,ratio,1);
	glColor3fv(textColor);
	int row = 1;
	for(int i = 0; text[i]!='\0'; i++)
	{
		if(text[i]=='\n')
		{
			row ++;
			glLoadIdentity();
			glTranslatef(0,scaleHeight-row*gridSize,0);
			glScalef(ratio,ratio,1);
		}
		else
			glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,text[i]);
	}
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
