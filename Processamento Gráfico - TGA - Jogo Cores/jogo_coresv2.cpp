#include <Windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <sstream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <math.h> 

#pragma region Classes
class RGBColor {
public:
	GLubyte r;
	GLubyte g;
	GLubyte b;
	RGBColor() {}
	RGBColor(GLubyte red, GLubyte green, GLubyte blue){
		r = red;
		g = green;
		b = blue;
	}
	GLubyte getRed() {
		return r;
	}
	GLubyte getGreen() {
		return g;
	}
	GLubyte getBlue() {
		return b;
	}
};

class mRectangle {
public:
	GLfloat x;
	GLfloat y;
	RGBColor color;
	bool isVisible;
	mRectangle(float mX, float mY, RGBColor mColor){
		x = mX;
		y = mY;
		color = mColor;
		isVisible = true;
	}
	void setVisibility(bool newState) {
		isVisible = newState;
}
};
#pragma endregion Classes

#pragma region Constantes e definições
const char* GAME_NAME = "Jogo das Cores";
const int MAX_NAME_SIZE = 20;
const std::string INFO_USER_INPUT = "Digite seu nome e tecle Enter para continuar.";
const std::string INFO_IN_GAME = "Clique na cor predominante para marcar mais pontos";
const std::string INFO_GAME_OVER = "O Jogo acabou. Para jogar novamente, pressione a tecla R(Restart) ou X(Exit) para sair do jogo.";
const float DISTANCE_TOLERANCE = 0.2;
const std::string PLAYER = "Player ";
const int BASE_SCORE = 10;
enum round {
	FIRST = 1;
	SECOND = 2;
	THIRD = 3;
}
#define MAX_RGB_DISTANCE 441.6729559300639498817084;
#pragma endregion Constantes e definições

#pragma region Variáveis Globais
GLfloat winW = 1280;
GLfloat winH = 720;
GLint winStX = 100;
GLint winStY = 100;
GLint nRows = 40;
GLint nCols = 20;
GLint nRect = nRows * nCols;
GLint nRounds = 4;
GLint currentRound = 0;
GLint score = 0;
GLint firstRoundMultiplier = 10;
GLint maxScore = nRect * firstRoundMultiplier;
std::vector<mRectangle> rectangles;
GLint rectW = winW / nCols;
GLint rectH = winH / nRows;
GLint headerSize = 80;
GLint drawOffset = 2;
std::string username = "";
std::string infoLine = "";
float xNewSize = 1;
float yNewSize = 1;
#pragma endregion Variáveis Globais

#pragma region Métodos
std::string getInfoLine() {
	if (currentRound == 0) {
		return INFO_USER_INPUT;
	}
	else if (currentRound > nRounds) {
		return INFO_GAME_OVER;
	}
	else {
		return INFO_IN_GAME;
	}
}

void buildLowerHeader(std::string name, float x, float y, float z) {
	name = PLAYER + username;
	glColor3f(1, 1, 1);
	glRasterPos3f(x, y, z);
	for (int i = 0; i < name.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, name.at(i));
	}
}

void buildInfoLine(std::string info, float x, float y, float z) {
	glColor3f(0.5, 0.5, 0.5);
	glRasterPos3f(x, y, z);
	for (int i = 0; i < info.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, info.at(i));
	}
}

void buildUpperHeader(float x, float y, float z) {
	int curRound;
	if (currentRound <= nRounds)
		curRound = currentRound;
	else curRound = nRounds;
	std::stringstream builder;
	builder << "Round: " << curRound << "    Score atual: " << score << "\\" << maxScore <<
		               "       % -> " <<  (score*100/maxScore) <<  "       Tolerancia de cor: "
		               <<  DISTANCE_TOLERANCE *100 <<  " %";
	std::string info = builder.str();
	glColor3f(1, 1, 1);
	glRasterPos3f(x, y, z);
	for (int i = 0; i < info.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, info.at(i));
	}
}

void loadVector(void) {
	for (int i = 0; i < nRows; i++)
		for (int j = 0; j < nCols; j++) {
			rectangles.push_back(mRectangle(j * rectW, i * rectH + headerSize / 2,
				RGBColor(rand() % 256, rand() % 256, rand() % 256)));
			rectangles[j + i*nCols].setVisibility(true);
		}
}

void drawRectangle(mRectangle rectangle) {
	glColor3ub(rectangle.color.getRed(), rectangle.color.getGreen(), rectangle.color.getBlue());
	glBegin(GL_QUADS);
	glVertex2d(rectangle.x + drawOffset, rectangle.y + drawOffset);
	glVertex2d(rectangle.x + rectW - drawOffset, rectangle.y + drawOffset);
	glVertex2d(rectangle.x + rectW - drawOffset, rectangle.y + rectH - drawOffset);
	glVertex2d(rectangle.x + drawOffset, rectangle.y + rectH - drawOffset);
	glEnd();
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	buildLowerHeader(username, drawOffset, drawOffset * 5, 0);
	buildInfoLine(getInfoLine(), winW / 4, drawOffset * 5, 0);
	buildUpperHeader(winW/4, rectH * nRows + 3*rectH, 0);
	for (int i = 0; i < nRect; i++)
		if (rectangles[i].isVisible) {
			drawRectangle(rectangles[i]);
		}
	glFlush();
}

void handleUsernameInput(unsigned char key) {
	switch (key) {
	case 13:
		if (username.empty() || username == " ")
			username += "Unknown";
		currentRound++;
		break;
	case 8:
		username.pop_back();
		break;
	default:
		username.push_back(key);
		break;
	}
}

float compareRectangles(mRectangle rectA, mRectangle rectB) {
	float distR = pow(rectA.color.r - rectB.color.r, 2);
	float distG = pow(rectA.color.g - rectB.color.g, 2);
	float distB = pow(rectA.color.b - rectB.color.b, 2);
	float distance = sqrt(distR + distG + distB);
	distance /= MAX_RGB_DISTANCE;
 	return distance;
}

void calcPoints() {
	switch (currentRound) {
	case round.FIRST:
		score += BASE_SCORE/currentRound;
		break;
	case round.SECOND:
		score += BASE_SCORE/currentRound;
		break;
	case round.THIRD:
		score += BASE_SCORE/currentRound;
		break;
	default:
		score += BASE_SCORE/currentRound;
		break;
	}
}

void updateRectangleState (mRectangle rect){
	for (int i = 0; i < nRect; i++) 
		if (compareRectangles(rect, rectangles[i]) <= DISTANCE_TOLERANCE) {
			rectangles[i].setVisibility(false);
			calcPoints();
		}
}

int getRectangle(int x, int y) {
	y = winH - y;
	int rectX = x / rectW;
	int rectY = y / rectH;
	int rectSelected = rectX + rectY * nCols;
	return rectSelected;
}

void updateGame(int x, int y) {
	int rectIndex = getRectangle(x, y);
	if (rectangles[rectIndex].isVisible) {
		updateRectangleState(rectangles[rectIndex]);
		currentRound++;
	}
}

void mouse(int button, int state, int x, int y) {
	float yRatio = ((yNewSize - headerSize*yNewSize/winH) / winH);
	float xRatio = xNewSize / winW;
	x = x/xRatio;
	y = y/yRatio - headerSize/2;
	if (currentRound > 0 && !(currentRound > nRounds))
		if (y <= winH && x <= winW)
			updateGame(x, y);
}

void reshape(int w, int h){
	glViewport(0, 0, xNewSize = w, yNewSize = h);
}

void startGame() {
	rectangles.clear();
	srand(time(0));
	currentRound = 0;
	score = 0;
	username = "";
	loadVector();
}

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, winW, 0.0, winH + headerSize, -1.0, 1.0);
}
#pragma endregion Métodos

void handleGameState(unsigned char key) {
	switch (key) {
	case 'R':
	case 'r':
		startGame();
		break;
	case 'X':
	case 'x':
		exit(0);
		break;
	}
}

void keyboard(unsigned char key, int x, int y) {
	if (currentRound == 0) {
		handleUsernameInput(key);
	}
	else if (currentRound >= nRounds) {
		handleGameState(key);
	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(winW, winH + headerSize);
	glutInitWindowPosition(winStX, winStY);
	glutCreateWindow(GAME_NAME);
	init();
	startGame();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;   /* ISO C requires main to return int. */
}