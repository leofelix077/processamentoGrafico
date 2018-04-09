#include <Windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <sstream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <math.h> 
//---------------------------------------------------------------------------------
#pragma region Classes
//define a cor 
class RGBColor {
public:
	//Canal RGBA utilizado no programa
	GLubyte r;
	GLubyte g;
	GLubyte b;
	//construtor padr�o
	RGBColor() {}
	//construtor recebendo cores
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
//---------------------------------------------------------------------------------
//Define o ret�ngulo, com cor, posi��o e estado
class mRectangle {
public:
//posi��o do ret�ngulo
	GLfloat x;
	GLfloat y;
//cor do ret�ngulos
	RGBColor color;
//estado do retangulo
	bool isVisible;
//Construtor recebe cor e posi��o
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
//---------------------------------------------------------------------------------
#pragma region Constantes e defini��es
//define o nome do jogo
const char* GAME_NAME = "Jogo das Cores";
//define o n�mero m�ximo de caract�res para o nome do jogador
const int MAX_NAME_SIZE = 20;
//linha de informa��o para o usu�rio digitar o nome
const std::string INFO_USER_INPUT = "Digite seu nome e tecle Enter para continuar.";
//linha de informa��o para jogo em andamento
const std::string INFO_IN_GAME = "Clique na cor predominante para marcar mais pontos";
//linha de informa��o para fim de jogo
const std::string INFO_GAME_OVER = "O Jogo acabou. Para jogar novamente, pressione a tecla R(Restart) ou X(Exit) para sair do jogo.";
//toler�ncia da dist�ncia para o c�lculo de cor / altera��o do estado do ret�ngulo
const float DISTANCE_TOLERANCE = 0.2;
//Literal "jogador"
const std::string PLAYER = "Player ";
//dist�ncia m�xima de uma cor com outra em RGB (0 a 255)
#define MAX_RGB_DISTANCE 441.6729559300639498817084;
#pragma endregion Constantes e defini��es
//---------------------------------------------------------------------------------
#pragma region Vari�veis Globais
//Largura da janela
GLfloat winW = 1280;
//ALtura da janela
GLfloat winH = 720;
//Posi��o inicial da janela em X
GLint winStX = 100;
//Posi��o inicial da janela em Y
GLint winStY = 100;
//Numero de linhas
GLint nRows = 40;
//numero de colunas
GLint nCols = 20;
//total de ret�ngulos
GLint nRect = nRows * nCols;
//N�mero de rounds
GLint nRounds = 4;
//Round atual || Inicio - Default = 1;
GLint currentRound = 0;
//Score feito na partida
GLint score = 0;
//Multiplicador de pontos por elimina��o no primeiro round 
GLint firstRoundMultiplier = 10;
//score m�ximo
GLint maxScore = nRect * firstRoundMultiplier;
//Vetor de ret�ngulos
std::vector<mRectangle> rectangles;
//largura do ret�ngulo
GLint rectW = winW / nCols;
//altura do ret�ngulo
GLint rectH = winH / nRows;
//Tamanho do cabe�alho de informa��es em pixels
GLint headerSize = 80;
//offset de divis�o entre os ret�ngulos
GLint drawOffset = 2;
//Nome do jogador;
std::string username = "";
//linha de informa��es ao jogador
std::string infoLine = "";
//escala da janela em X
float xNewSize = 1;
//escala da janela em Y
float yNewSize = 1;
#pragma endregion Vari�veis Globais
//---------------------------------------------------------------------------------
#pragma region M�todos
//---------------------------------------------------------------------------------
//retorna a linha de ajuda a ser desenhada
std::string getInfoLine() {
	//se estiver abrindo o jogo
	if (currentRound == 0) {
		return INFO_USER_INPUT;
	}
	//se estiver no final do jogo
	else if (currentRound > nRounds) {
		return INFO_GAME_OVER;
	}
	//se estiver no meio do jogo
	else {
		return INFO_IN_GAME;
	}
}
//---------------------------------------------------------------------------------
//Renderiza string-----------------------------------------------------------------
void buildLowerHeader(std::string name, float x, float y, float z) {
	name = PLAYER + username;
	glColor3f(1, 1, 1);
	glRasterPos3f(x, y, z);
	for (int i = 0; i < name.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, name.at(i));
	}
}
//---------------------------------------------------------------------------------
//Renderiza as strings de informa��o ao usu�rio
void buildInfoLine(std::string info, float x, float y, float z) {
	glColor3f(0.5, 0.5, 0.5);
	glRasterPos3f(x, y, z);
	for (int i = 0; i < info.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, info.at(i));
	}
}
//---------------------------------------------------------------------------------
//carrega o cabe�alho de informa��es do jogo %, pontua��o, round, etc
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
//---------------------------------------------------------------------------------
//Carrega o vetor com as cores pr�-definidas
void loadVector(void) {
	for (int i = 0; i < nRows; i++)
		for (int j = 0; j < nCols; j++) {
			rectangles.push_back(mRectangle(j * rectW, i * rectH + headerSize / 2,
				RGBColor(rand() % 256, rand() % 256, rand() % 256)));
			rectangles[j + i*nCols].setVisibility(true);
		}
}
//---------------------------------------------------------------------------------
//desenha um ret�ngulo
void drawRectangle(mRectangle rectangle) {
	glColor3ub(rectangle.color.getRed(), rectangle.color.getGreen(), rectangle.color.getBlue());
	glBegin(GL_QUADS);
	glVertex2d(rectangle.x + drawOffset, rectangle.y + drawOffset);
	glVertex2d(rectangle.x + rectW - drawOffset, rectangle.y + drawOffset);
	glVertex2d(rectangle.x + rectW - drawOffset, rectangle.y + rectH - drawOffset);
	glVertex2d(rectangle.x + drawOffset, rectangle.y + rectH - drawOffset);
	glEnd();
}
//---------------------------------------------------------------------------------
//desenha na tela
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
//---------------------------------------------------------------------------------
//Gerencia o input do nome do usu�rio
void handleUsernameInput(unsigned char key) {
	switch (key) {
		//se der enter, sai da aceita��o do nome
	case 13:
		//Se o nome estiver vazio
		if (username.empty() || username == " ")
			username += "Unknown";
		currentRound++;
		break;
		//se der backspace, apaga a ultima letra
	case 8:
		username.pop_back();
		break;
		//se nao for tecla especial, concatena a string
	default:
		username.push_back(key);
		break;
	}
}
//---------------------------------------------------------------------------------
//Compara um retangulo com outro
float compareRectangles(mRectangle rectA, mRectangle rectB) {
	float distR = pow(rectA.color.r - rectB.color.r, 2);
	float distG = pow(rectA.color.g - rectB.color.g, 2);
	float distB = pow(rectA.color.b - rectB.color.b, 2);
	float distance = sqrt(distR + distG + distB);
	distance /= MAX_RGB_DISTANCE;
 	return distance;
}
//---------------------------------------------------------------------------------
//calcula a pontua��o do jogo 
void calcPoints() {
	switch (currentRound) {
	case 1:
		score += 10;
		break;
	case 2:
		score += 5;
		break;
	case 3:
		score += 2;
		break;
//se o jogo tiver mais de quatro rounds
	default:
		score += 1;
		break;
	}
}
//---------------------------------------------------------------------------------
//compara o retangulo selecionado com o vetor, e atualiza o estado individual
void updateRectangleState (mRectangle rect){
	for (int i = 0; i < nRect; i++) 
		if (compareRectangles(rect, rectangles[i]) <= DISTANCE_TOLERANCE) {
			rectangles[i].setVisibility(false);
			calcPoints();
		}
}
//---------------------------------------------------------------------------------
//seleciona o retangulo a partir da posi��o do mouse
int getRectangle(int x, int y) {
// transforma em coordenadas OpenGL
	y = winH - y;
//define a "linha e coluna" do retangulo
	int rectX = x / rectW;
	int rectY = y / rectH;
//calcula qual retangulo foi selecionado a partir da posi��o do mouse
	int rectSelected = rectX + rectY * nCols;
	return rectSelected;
}
//---------------------------------------------------------------------------------
//Atualiza o estado do jogo
void updateGame(int x, int y) {
	int rectIndex = getRectangle(x, y);
	if (rectangles[rectIndex].isVisible) {
		updateRectangleState(rectangles[rectIndex]);
		currentRound++;
	}
}
//---------------------------------------------------------------------------------
//gerencia callback de mouse
void mouse(int button, int state, int x, int y) {
//calcula a nova taxa da janela para c�lculo de sele��o do ret�ngulo
	float yRatio = ((yNewSize - headerSize*yNewSize/winH) / winH);
	float xRatio = xNewSize / winW;
//redefine o x e o y
	x = x/xRatio;
	y = y/yRatio - headerSize/2;
//apenas responde ao callback do mouse se estiver no meio do jogo
	if (currentRound > 0 && !(currentRound > nRounds))
		if (y <= winH && x <= winW)
			updateGame(x, y);
}
//---------------------------------------------------------------------------------
//Gerencia redimens�o da tela
void reshape(int w, int h){
	glViewport(0, 0, xNewSize = w, yNewSize = h);
}
//---------------------------------------------------------------------------------
//Inicializa as vari�veis do jogo
void startGame() {
	rectangles.clear();
	srand(time(0));
	currentRound = 0;
	score = 0;
	username = "";
	//carrega o vetor com as cores pr�-definidas / aleat�rias
	loadVector();
}
//---------------------------------------------------------------------------------
//inicializa o programa
void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, winW, 0.0, winH + headerSize, -1.0, 1.0);
}
#pragma endregion M�todos
//---------------------------------------------------------------------------------
//Gerencia se deseja terminar ou reiniciar o jogo
void handleGameState(unsigned char key) {
	switch (key) {
		//se apertar R, reinicia o jogo
	case 'R':
	case 'r':
		startGame();
		break;
		//se apertar X, sai do jogo
	case 'X':
	case 'x':
		exit(0);
		break;
	}
}
//---------------------------------------------------------------------------------
//callback de teclado 
void keyboard(unsigned char key, int x, int y) {
//se est� no come�o do jogo, aceita o nome do jogador
	if (currentRound == 0) {
		handleUsernameInput(key);
	}
//se est� no fim do jogo, trata sa�da ou restart
	else if (currentRound >= nRounds) {
		handleGameState(key);
	}
	glutPostRedisplay();
}
//----------------------------------------------------------------------------------
#pragma region Main Function
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
#pragma endregion Main Function