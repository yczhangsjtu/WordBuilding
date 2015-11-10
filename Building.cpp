#include <GL/freeglut.h>
//#include <iostream>
#include <fstream>
#include "wordType.h"
#include "draw.h"
#include "Building.h"

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WindowWidth,WindowHeight);
	glutInitWindowPosition(0,0);
	glutCreateWindow(TITLE);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutPassiveMotionFunc(passiveMotion);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutSpecialFunc(specialKey);
	glutKeyboardFunc(key);
	glutSpecialUpFunc(specialKeyUp);
	glutIdleFunc(idle);
	init();
	glutMainLoop();
}



void init()
{
	glClearColor(bgR,bgG,bgB,bgA);
	readWords(baseWord,WORD_FILE);
	addWordToMap(baseWord,wordMap);
	addConnectToMap(baseWord,connectMap);
	viewx = (baseWord->x)*gridSize;
	viewy = (baseWord->y)*gridSize;
	currSentence[0] = '\0';
	sentencex = -1;
	sentencey = -1;
	inputPointer = 0;
	inputBuffer[inputPointer] = '\0';
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	drawBackground
		(viewx,viewy,bgLineColor,highlightColor,
		currScaleWidth,currScaleHeight,imousex,imousey);
	drawWord(*baseWord,wordsColor,viewx,viewy);
	drawSentence(currSentence,sentenceColor,sentenceBackColor,
				sentencex,sentencey,viewx,viewy);
	drawText(inputBuffer,inputColor);
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	ww = w;//Update window size
	hh = h;
	GLfloat fw = ww;
	GLfloat fh = hh;
	
	if( fw/fh > scaleWidth/scaleHeight )
		currScaleWidth = scaleHeight*fw/fh;
	else
		currScaleHeight = scaleWidth*fh/fw;
	
	glViewport(0,0,ww,hh);//Change viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();//Update projection matrix
	
	if( fw/fh > scaleWidth/scaleHeight )
		gluOrtho2D(	scaleLeft,scaleLeft+currScaleWidth,
					scaleBottom,scaleBottom+scaleHeight);
	else
		gluOrtho2D(	scaleLeft,scaleLeft+scaleWidth,
					scaleBottom,scaleBottom+currScaleHeight);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-viewx,-viewy,0);
}


void passiveMotion(int x, int y)
{
	mousex = x;
	mousey = y;
	imousex = (GLint) ((GLfloat)(mousex+viewx)/gridSize);
	imousey = (GLint) ((GLfloat)(hh-mousey+viewy)/gridSize);
	ipair ip = {imousex,imousey};
	map<ipair,Connect*>::iterator iter = connectMap.find(ip);
	if(iter!=connectMap.end())
	{
		strcpy(currSentence,iter->second->sentence);
		sentencex = imousex;
		sentencey = imousey;
	}
	else
	{
		currSentence[0]='\0';
		sentencex = -1;
		sentencey = -1;
	}
	glutPostRedisplay();
}

void motion(int x, int y)
{
	mousex = x;
	mousey = y;
	imousex = (GLint) ((GLfloat)(mousex+viewx)/gridSize);
	imousey = (GLint) ((GLfloat)(hh-mousey+viewy)/gridSize);
	glutPostRedisplay();
}

void mouse(int state, int button, int x, int y)
{
	if(state==GLUT_DOWN && button==GLUT_LEFT_BUTTON)
	{
		mousex = x;
		mousey = y;
		imousex = (GLint) ((GLfloat)(mousex+viewx)/gridSize);
		imousey = (GLint) ((GLfloat)(hh-mousey+viewy)/gridSize);
		ipair ip = {imousex,imousey};
		{
			map<ipair,Connect*>::iterator iter = connectMap.find(ip);
			if(iter!=connectMap.end()) return;
		}
		map<ipair,Word*>::iterator iter;
		char letter = '\0'; int firstIndex = 0;
		for(iter=wordMap.begin(); iter!=wordMap.end(); iter++)
		{
			if(iter->second->x == imousex && iter->second->y >= imousey
				&& (iter->second->y)-(iter->second->l) < imousey 
				&& !(iter->second->hor))
			{
				firstIndex = (iter->second->y)-imousey;
				letter = iter->second->letters[firstIndex];
				break;
			}
			if(iter->second->y == imousey && iter->second->x <= imousex
				&& (iter->second->x)+(iter->second->l) > imousex
				&& iter->second->hor)
			{
				firstIndex = imousex-(iter->second->x);
				letter = iter->second->letters[firstIndex];
				break;
			}
		}
		if(iter==wordMap.end()) return;
		string input(inputBuffer);
		int index1 = input.find_first_of("\n");
		if(index1==string::npos) return;
		string word = input.substr(0,index1);
		int secondIndex = 0;
		if((secondIndex=word.find(letter))==string::npos) return;
		int index2 = input.find_first_of("\n",index1+1);
		if(index2==string::npos) return;
		string sentence = input.substr(index1+1, index2-index1-1);
		bool result = addConnectTo(iter->second,firstIndex,word.c_str(),secondIndex,
									sentence.c_str());
		if(result)
		{
			addConnectToMap(iter->second,connectMap);
			addWordToMap(iter->second->connects[firstIndex]->right,wordMap);
			input = input.substr(index2+1);
			strcpy(inputBuffer,input.c_str());
			inputPointer -= (index2+1);
		}
	}
}

void specialKey(int keyCode, int x, int y)
{
	switch(keyCode)
	{
	case GLUT_KEY_LEFT:
		screenRollvx = -screenRollSpeed;
		break;
	case GLUT_KEY_RIGHT:
		screenRollvx = screenRollSpeed;
		break;
	case GLUT_KEY_UP:
		screenRollvy = screenRollSpeed;
		break;
	case GLUT_KEY_DOWN:
		screenRollvy = -screenRollSpeed;
		break;
	case GLUT_KEY_F1:
		//outputWords("tmp.txt");
		{
			ofstream fout("tmp.txt");
			outputWord(*baseWord,fout,true);
			fout.close();
		}
		deleteWord(baseWord);
		exit(0);
	case GLUT_KEY_F2:
		{
			ofstream fout("tmp.txt");
			outputWord(*baseWord,fout,true);
			fout.close();
		}
		break;
	}
}

void specialKeyUp(int keyCode, int x, int y)
{
	switch(keyCode)
	{
	case GLUT_KEY_LEFT:
	case GLUT_KEY_RIGHT:
		screenRollvx = 0;
		break;
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN:
		screenRollvy = 0;
		break;
	}
}

void key(unsigned char keyCode, int x, int y)
{
	if(keyCode == 13)
		inputBuffer[inputPointer++] = '\n';
	else if(keyCode == 8)
	{
		if(inputPointer>0)
			inputBuffer[inputPointer--] = '\0';
	}
	else
		inputBuffer[inputPointer++] = keyCode;
	inputBuffer[inputPointer] = '\0';
}

void idle()
{
	viewx += screenRollvx;
	if(viewx>MaxViewX) viewx = MaxViewX;
	if(viewx<0) viewx = 0;
	viewy += screenRollvy;
	if(viewy>MaxViewY) viewy = MaxViewY;
	if(viewy<0) viewy = 0;
	imousex = (GLint) ((GLfloat)(mousex+viewx)/gridSize);
	imousey = (GLint) ((GLfloat)(hh-mousey+viewy)/gridSize);
	glutPostRedisplay();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-viewx,-viewy,0);
}
/*
void outputWords(const char * fileName)
{
	ofstream fout(fileName);
	fout<< (baseWord->letters) << ' ' << (baseWord->x) << ' ' << (baseWord->y) << ' '
		<< (baseWord->hor) << endl;
	map<ipair,Connect*>::iterator iter;
	for(iter=connectMap.begin(); iter!=connectMap.end(); iter++)
	{
		if((iter->second->left)==NULL) continue;
		if((iter->second->right)==NULL) continue;
		fout<<(iter->second->left->letters)<<' ';
		fout<<(iter->second->right->letters)<<' ';
		for(int i=0; i<MaxWordLength; i++)
			if(iter->second->left->connects[i]==iter->second)
			{
				fout << i <<' ';
				break;
			}
		for(int i=0; i<MaxWordLength; i++)
			if(iter->second->right->connects[i]==iter->second)
			{
				fout << i <<' ';
				break;
			}
		fout<<(iter->second->sentence)<<endl;
	}
	fout.close();
}*/
