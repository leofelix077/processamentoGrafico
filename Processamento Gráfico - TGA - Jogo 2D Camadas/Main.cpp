#include "PTMReader.h"
#include "Image.h"
#include "Timer.h"
#include <windows.h>
#include <cstdlib>
#include <iostream>
#include "Animation.h"
#include "GameObject.h"
#include "glut.h"
#include "Layer.h"
enum spriteOrientation {
	SPRITE_FRONT = 0,
	SPRITE_LEFT = 1,
	SPRITE_RIGHT = 2,
	SPRITE_BACK = 3
};
enum explosionStage {
	EXP_START = 0,
	EXP_FINAL = 3,
};
//----------------------------------------------------------------------
//inicializa dimens�es do fundo prim�rio
int width = 1024;
int height = 500;
//quantidade de quadros do sprite/seg
const int SPRITE_FPS = 8;
//Imagens do programa
char* Background_Img = "PkmJustSky.ptm";
char* Layer_1_Img = "PkmGround.ptm";
char* Layer_2_Img = "Fences.ptm";
char* Character_Sprite = "Character.ptm";
char* Fireball_Sprite = "explosion_4.ptm";
//Inicializa��es
PTMReader file;
Timer *timer = new Timer();
Animation *ani = new Animation();
Animation *expAni = new Animation();
GameObject *gameObj = new GameObject();
GameObject *expObj = new GameObject();
Image *scene,*backup, *fore, *fence;
vector<Layer*> layers;
char* zbuffer, *zbuffer2;
vector<char*> files;
int spritePosX, spritePosY = 0;
//----------------------------------------------------------------------
//desenha as imagens a partir da cena de fundo
void drawScene(Image *scene) {
	if (scene->getPixels() != 0) {
		glDrawPixels(scene->getWidth(),scene->getHeight(),
		GL_BGRA_EXT, GL_UNSIGNED_BYTE, scene->getPixels());
	}
}
//----------------------------------------------------------------------
// Fun��o callback de redesenho da janela de visualiza��o
void display(void) {
	// Limpa a janela de visualiza��o com a cor branca
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	drawScene(scene);
	//Executa os comandos OpenGL 
	glFlush();
}
void loadLayers() {
	files.push_back(Background_Img);
	files.push_back(Layer_1_Img);
	files.push_back(Layer_2_Img);
	for (int i = 0; i < files.size(); i++){
		Layer* layer = new Layer(file.read(files[i]), 0, 0, i);
		layer->computeScrollRateX(i);
		layers.push_back(layer);
	}
}
//----------------------------------------------------------------------
//Cria a cena, levando em considera��o camadas, frame selecionado, etc
void buildScene() {
	//Cria a cena com as dimens�es da imagem de fundo; impede a c�pia de pixels levando a rastros
	scene = new Image(layers[0]->getLayer()->getWidth(), layers[0]->getLayer()->getHeight());
	for (int i = layers.size() - 1; i >= 0; i--){
		scene->plot(layers[i]->getLayer(), 0, 0, zbuffer, i);
	}
}
//----------------------------------------------------------------------
//calcula o tempo desejado para a atualiza��o dos quadros
float calcWaitingTime(int fps, float elapsedTime) {
	int wt = 1000 / fps - elapsedTime;
	return wt;
}
//Inicia o ZBuffer
void initZbuffer() {
	int zbufferSize = layers[0]->getLayer()->getWidth()*layers[0]->getLayer()->getHeight();
	zbuffer = new char[zbufferSize];
	for (int i = 0; i < zbufferSize; i++) {
		zbuffer[i] = -1;
	}
}
//----------------------------------------------------------------------
//Metodo update conforme os slides
void update(int value) {
	//inicia contagem de tempo de processamento
	timer->start();
	//FALTA IMPLEMENTAR O ZBUFFER 2 E O BACKUP, pra testar quando ocorreu mudan�a na tela e s� redesenhar o que mudou
	//cria uma nova imagem
	scene = new Image(layers[0]->getLayer()->getWidth(), layers[0]->getLayer()->getHeight());
	initZbuffer();
	scene->plot(gameObj->getCurrentFrame(), 300, 120, zbuffer, 2);
	scene->plot(expObj->getCurrentFrame(), spritePosX, 120, zbuffer, 2);
	for (int i = layers.size() -1; i >= 0; i--){
		int deltaX = (int)(layers[i]->getPosX() + width) % width;
		scene->plot(layers[i]->getLayer(), deltaX, layers[i]->getPosY(), zbuffer, i);
	}
	// para rederizar quadro "atual"
	glutPostRedisplay();
	timer->finish();
	//calcula o o tempo de atualiza��o necessario para os quadros
	int waitingTime = calcWaitingTime(SPRITE_FPS, timer->getElapsedTimeMs());
	//if (playing) {
	// waitingTime � um valor em milisegundos
	glutTimerFunc(waitingTime, update, value);
	//AQUI EU FIZ APARECER UM SULLY, MAS SERIA UM GAME OVER, QUANDO A EXPLOS�O ENCOSTAR NO SULLY TEM QUE TERMINAR
	//TEM QUE LEVAR EM CONSIDERA��O O Y TAMBEM.
	if (spritePosX > 240) {
		scene->plot(gameObj->getCurrentFrame(), 700, 120, zbuffer, 2);
	}
	//d� pra fazer o sully s� se mexer pra cima e pra baixo
	// e desviar das fireballs. se alguma acertar ele, o jogo termina. Sempre que uma bolota passar da posi��o da tela
	//tem que deletar o objeto, ou resetar a posi��o e usar sempre o mesmo set de 3 ou 4 fireballs;
}
//----------------------------------------------------------------------
//Registra callback de teclado
//TODO SOLUCIONAR O PROBLEMA DOS UPDATES; o jogo perde responsividade quando se usa qualquer tecla
void keyboard(unsigned char key, int xx, int yy) {
	switch (key) {
	case 'D':
	case 'd':
		for (int i = layers.size() - 1; i >= 0; i--)
			layers[i]->scroll(RIGHT, 10);
		gameObj->changeSpriteOrientation(SPRITE_RIGHT);
		expObj->incCurrentFrameFull();
		spritePosX += 10;
		break;
	case 'A':
	case 'a':
		for (int i = layers.size()-1; i >= 0; i--)
			layers[i]->scroll(LEFT, 10);
		gameObj->changeSpriteOrientation(SPRITE_LEFT);
		spritePosX -= 10;
		break;
	case 'W':
	case 'w':
		spritePosY += 10;
		gameObj->changeSpriteOrientation(SPRITE_BACK);
		break;
	case 'S':
	case 's':
		spritePosY -= 10;
		gameObj->changeSpriteOrientation(SPRITE_FRONT);
		break;
	}
	gameObj->incCurrentFrame();
	glutPostRedisplay();
}
void loadSprites() {
	ani->addFrame(file.read(Character_Sprite));
	gameObj->setSprite(ani);
	expAni->addFrame(file.read(Fireball_Sprite));
	expObj->setSprite(expAni);
}
//----------------------------------------------------------------------
// Fun��o respons�vel por inicializar par�metros e vari�veis
void init(void) {
	loadLayers();
	initZbuffer();
	loadSprites();
	// Define a janela de visualiza��o 2D
	glMatrixMode(GL_PROJECTION);
	glOrtho(0.0f, width, 0.0, height, 0, 0);
	buildScene();
}
//----------------------------------------------------------------------
// Programa Principal 
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	// Define do modo de opera��o da GLUT
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	// Especifica o tamanho inicial em pixels da janela GLUT
	glutInitWindowSize(width, height);
	// Cria a janela passando como argumento o t�tulo da mesma
	glutCreateWindow("Processando Imagens");
	// Chama a fun��o respons�vel por fazer as inicializa��es 
	init();
	// Registra a fun��o callback de redesenho da janela de visualiza��o
	glutDisplayFunc(display);
	//registra callback de teclado;
	glutKeyboardFunc(keyboard);
	// Inicia o processamento e aguarda intera��es do usu�rio
	//Atualiza a cena ap�s todos os comando terem sido executados
	update(0);
	glutMainLoop();
	return 0;
}