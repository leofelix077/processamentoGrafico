#include <windows.h>
#include <GL/glut.h>
#include "PTMReader.h"
#include "Image.h"
#include "TilemapCreator.h"
#include "Vertex.h"
#include "Tilemap.h"
#include "IsoDiamond.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <time.h>
#include <math.h>
#include <array>
#include "Timer.h"
//-----------------------------------------------------------------------------
//DECLARAÇÕES------------------------------------------------------------------
using namespace std;
//Seta a taxa de atualização do jogo
const int GAME_UPDATE_FPS = 10;
//Seta altura da janela
int screenHeight = INITIAL_SCREEN_HEIGHT;
//seta largura da janela
int screenWidth = INITIAL_SCREEN_WIDTH;
//seta posição inicial do "player" em x
int plx = 0;
//seta posição inicial do "player" em y
int ply = 9;
//seta posição inicial do mouse em x
int mx = 0;
//seta posição inicial do mouse em xy
int my = 0;
//Seta a compensação da janela em x
int screenWidthOffset = 0;
//Seta o compensação da janela em y
int screenHeightOffset = -INITIAL_SCREEN_HEIGHT/2;
//Teclas correspondentes de movimento
enum movementKey {
	KEY_NORTH = 'w',
	KEY_SOUTH = 's',
	KEY_EAST = 'd',
	KEY_WEST = 'a',
	KEY_NORTHEAST = 'e',
	KEY_NORTHWEST = 'q',
	KEY_SOUTHEAST = 'c',
	KEY_SOUTHWEST = 'z',
	KEY_EXIT = 'k',
	KEY_RESTART = 'r'
};
enum specialTiles {
	TILE_COLLISION = 5,
	TILE_WATER = 7,
};
//inicializações de objetos do jogo
IsoDiamond* iso;
Tilemap* tilemap;
GLuint *TextId;
Timer *timer = new Timer();
//Criação do tilemap
char *tilesetName = "Tileset.ptm";
//Tamanho do tile em x
int tileWidth = 64;
//Tamanho do tile em y
int tileHeight = 64;
//Quantidade de tiles em X
int tilemapWidth = 10;
//Quantidade de tiles em Y
int tilemapHeight = 10;
//Estado do jogo - Se está em jogo ou não
bool playing = true;
//Se ocorreu colisão com algum tile específico do mapa
bool collided = false;
//Round inicial do jogo
int gameRound = 0;
//Armazena o tempo que o jogador ficou sobre um tile do tipo "água"
int timeOnWater = 0;
//Primeiro mapa
array<int,100> tileMap1 = {
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 3, 3, 7, 7, 7, 3, 3, 3, 2,
	2, 7, 7, 7, 7, 7, 7, 5, 5, 2,
	2, 3, 3, 3, 3, 3, 3, 5, 3, 2,
	2, 5, 5, 3, 3, 3, 3, 5, 3, 2,
	2, 5, 5, 5, 5, 5, 5, 5, 5, 2,
	2, 5, 7, 3, 3, 3, 3, 3, 3, 2,
	2, 3, 7, 3, 3, 3, 3, 3, 3, 2,
	2, 3, 7, 3, 3, 3, 3, 3, 3, 2,
	7, 2, 2, 2, 2, 2, 2, 2, 2, 2,
};
//Segundo mapa
array<int,100>tileMap2 = {
	25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
	25, 3, 3, 7, 7, 7, 3, 5, 3, 25,
	25, 7, 7, 7, 7, 7, 7, 5, 5, 25,
	25, 3, 3, 5, 3, 5, 3, 5, 3, 25,
	25, 5, 5, 3, 3, 3, 3, 5, 3, 25,
	25, 5, 5, 5, 5, 5, 5, 5, 5, 25,
	25, 5, 3, 3, 3, 5, 3, 3, 5, 25,
	25, 3, 3, 3, 5, 3, 3, 5, 5, 25,
	25, 3, 3, 3, 3, 3, 3, 3, 3, 25,
	3, 25, 25, 25, 25, 25, 25, 25, 25, 25,
};
//Terceiro mapa
array<int, 100> tileMap3 = {
	28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
	28, 7, 7, 7, 7, 7, 7, 7, 7, 28,
	28, 7, 7, 7, 7, 7, 7, 5, 5, 28,
	28, 7, 7, 7, 7, 7, 7, 5, 7, 28,
	28, 5, 5, 7, 3, 3, 3, 5, 3, 28,
	28, 5, 5, 5, 5, 5, 5, 5, 5, 28,
	28, 5, 3, 7, 5, 3, 3, 3, 3, 28,
	28, 3, 3, 7, 3, 5, 5, 5, 5, 28,
	28, 3, 3, 3, 3, 5, 3, 3, 5, 28,
	3, 28, 28, 28, 28, 28, 28, 28, 28, 28,
};
//Quarto mapa
array<int, 100> tileMap4 = {
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
	4, 5, 5, 7, 7, 7, 5, 5, 5, 4,
	4, 5, 5, 5, 5, 5, 5, 5, 5, 4,
	4, 5, 5, 5, 7, 5, 5, 5, 5, 4,
	4, 5, 5, 7, 3, 3, 3, 5, 3, 4,
	4, 5, 5, 5, 5, 5, 5, 5, 5, 4,
	4, 5, 3, 7, 5, 3, 3, 3, 3, 4,
	4, 3, 3, 7, 3, 5, 5, 5, 5, 4,
	4, 3, 3, 3, 3, 5, 3, 3, 5, 4,
	3, 4, 4, 4, 4, 4, 4, 4, 4, 4,
};
//Guarda os mapas definidos no jogo, para que possa ser utilizado no "round"
array <array<int,100>,4> tileMapRound = { tileMap1, tileMap2,tileMap3,tileMap4 };
//array de backup para estado inicial do jogo
array <array<int, 100>,4> backupMap = tileMapRound;
//-----------------------------------------------------------------------------
//MÉTODOS----------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Cria os tiles do jogo de acordo com o estado que o jogo se encontra
void createTile(int x, int y, int tile, int c, int r) {
	//Se estiver no ultimo round
	if (gameRound == sizeof(tileMapRound) - 1)
		glColor3f(.1, .1, .1);
	//Se for o tile em que o jogador estiver
	else if (c == plx && r == ply)
		glColor3f(1, .6, .6);
	//Se for o tile 5 (tile de colisão)
	else if (tileMapRound[gameRound][plx + ply*tilemapWidth] == TILE_COLLISION)
		glColor3ub(255, 20, 0);
	//Se for o tile 7 (Tile de água)
	else if (tileMapRound[gameRound][plx + ply*tilemapWidth] == TILE_WATER)
		glColor3ub(10, 20, 100);
	//Se chegou na posição final do jogo
	else if (plx == tilemapWidth-1 && ply == 0)
		glColor3ub(0, 255, 0);
	//Fallback, pinta de branco
	else
		glColor3f(1, 1, 1);
	//adiciona textura ao grid
	glBindTexture(GL_TEXTURE_2D, TextId[tile]);
	vert poliVert = calculate(x, y);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(poliVert.ax, poliVert.ay);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(poliVert.bx, poliVert.by);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(poliVert.cx, poliVert.cy);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(poliVert.dx, poliVert.dy);
	glEnd();
}
//-----------------------------------------------------------------------------
//Desenha os tiles na tela
void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	int x, y;
	for (int r = 0; r < tilemap->getWidth(); r++) {
		for (int c = 0; c < tilemap->getHeight(); c++) {
			int pos = tileMapRound[gameRound][c + r*tilemap->getWidth()];
			int tile = tilemap->getTile(pos);
			if (tile == -1) continue;
			iso->calcPosDesenho(c, r, x, y);
			createTile(x, y, tile, c, r);
		}
	}
	glFlush();
}
//-----------------------------------------------------------------------------
//Faz a movimentação do personagem - Utilizado no método keyboard
void moveCharacter(char lKey) {
	//Identifica a tecla pressionada e move de acordo 
	switch (lKey) {
	case KEY_NORTH:
		if (plx < tilemapWidth-1 && ply > 0)
			iso->tileWalking(plx, ply, NORTH);
		break;
	case KEY_SOUTH:
		if (ply < tilemapHeight-1 && plx > 0)
			iso->tileWalking(plx, ply, SOUTH);
		break;
	case KEY_EAST:
		if (plx < tilemapHeight - 1 && ply < tilemapWidth - 1)
			iso->tileWalking(plx, ply, EAST);
		break;
	case KEY_WEST:
		if (plx > 0 && ply > 0)
			iso->tileWalking(plx, ply, WEST);
		break;
	case KEY_NORTHWEST:
		if (ply > 0 && gameRound <2)
			iso->tileWalking(plx, ply, NORTHWEST);
		break;
	case KEY_NORTHEAST:
		if (plx < tilemapWidth-1 && gameRound <2)
			iso->tileWalking(plx, ply, NORTHEAST);
		break;
	case KEY_SOUTHWEST:
		if (plx > 0 && gameRound <2)
			iso->tileWalking(plx, ply, SOUTHWEST);
		break;
	case KEY_SOUTHEAST:
		if (ply < tilemapHeight-1 && gameRound <2)
			iso->tileWalking(plx, ply, SOUTHEAST);
		break;
	case KEY_EXIT:
		exit(0);
		break;
	}
}
//-----------------------------------------------------------------------------
//Move o personagem de acordo com o tile pressionado no mapa
void moveCharacterOnMouse() {
	//se a posição do mouse estiver de acordo com as definidas no tileWalking, move o personagem
	if (mx == plx + 1 && my == ply - 1) {
		moveCharacter(KEY_NORTH);
	}
	else if (mx == plx -1 && my == ply +1) {
		moveCharacter(KEY_SOUTH);
	}
	else if  (mx == plx -1 && my == ply - 1) {
		moveCharacter(KEY_WEST);
	}
	else if (mx == plx + 1 && my == ply + 1) {
		moveCharacter(KEY_EAST);
	}
	else if (mx == plx + 1 && my == ply) {
		moveCharacter(KEY_NORTHEAST);
	}
	else if (my == ply - 1 && mx == plx) {
		moveCharacter(KEY_NORTHWEST);
	}
	else if (my == ply + 1 && mx == plx) {
		moveCharacter(KEY_SOUTHEAST);
	}
	else if (mx == plx - 1 && my == ply) {
		moveCharacter(KEY_SOUTHWEST);	
	}
}
//-----------------------------------------------------------------------------
//Callback de mouse
void mouse(int button, int state, int xclick, int yclick) {
	//Se estiver em jogo e o round for 0 ou 1, aceita o callback do mouse.
	if (playing && gameRound < 2) {
		switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN) {
				iso->mouseMap(xclick + screenWidthOffset, screenHeight - yclick + screenHeightOffset, mx, my);
				moveCharacterOnMouse();
			}
			break;
		default:
			break;
		}
		glutPostRedisplay();
	}
}
//-----------------------------------------------------------------------------
//Callback de teclado
void keyboard(unsigned char key, int mousex, int mousey) {
	//Se estiver em jogo
	if (playing) {
		//transforma a tecla pressionada para lowerCase e movimenta o personagem
		char lowerKey = tolower(key);
		moveCharacter(lowerKey);
		glutPostRedisplay();
	} 
	//se deseja reiniciar o jogo
	if (key == KEY_RESTART) {
		//reinicia o round e a posição do player
		gameRound = 0;
		plx = 0;
		ply = 9;
		playing = true;
		//zera o contador de tempo na água
		timeOnWater = 0;
		//reinicializa os mapas para as posições originais
		tileMapRound = backupMap;
	}
}
//-----------------------------------------------------------------------------
//Redimensionamento de janela
void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	screenHeight = h;
	screenWidth = w;
	glOrtho(screenWidthOffset, w + screenWidthOffset, screenHeightOffset, h + screenHeightOffset, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
//-----------------------------------------------------------------------------
//Inicializações do openGl
void init() {
	srand(time(0));
	glClearColor(0.10, 0.10, 0.10, 0.10);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, screenWidth, 0, screenHeight, -1.0, 1.0);
	//lê e cria um mapa de tiles a ser projetado na tela 
	TilemapCreator* tc = new TilemapCreator(tilesetName, tileWidth, tileHeight, tilemapWidth, tilemapHeight);
	tilemap = tc->createMap();
	int tileCount = tilemap->getTilesCounter();
	TextId = new GLuint[tileCount];
	//inicializa o "array" de texturas openGL
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glGenTextures(tileCount, TextId);
	for (int i = 0; i < tileCount; i++) {
		glBindTexture(GL_TEXTURE_2D, TextId[i]);
		Image* image = tilemap->getTileById(i);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->getWidth(),
			image->getHeight(), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE,
			image->getPixels());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		delete image;
	}
	//cria o grid isométrico
	iso = new IsoDiamond(TILE_HEIGHT, TILE_WIDTH, tilemap->getHeight(), tilemap->getWidth());
}
//-----------------------------------------------------------------------------
//Cria a janela do jogo
void createWindow(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("EscapeEscape");
}
//----------------------------------------------------------------------
//calcula o tempo desejado para a atualização dos quadros
float calcWaitingTime(int fps, float elapsedTime) {
	int wt = 1000 / fps - elapsedTime;
	return wt;
}
//-----------------------------------------------------------------------------
//Verifica se ocorreu colisão com os tiles especificados
void checkCollision() {
	//Se for tile de colisão
	if (tileMapRound[gameRound][plx + ply*tilemapWidth] == TILE_COLLISION)
		collided = true;
	//se for tile de água - Adiciona o valor no tempo que ficou na água
	else if (tileMapRound[gameRound][plx + ply*tilemapWidth] == 7) {
		timeOnWater++;
		std::cout << timeOnWater<<'\n';
	}
	else timeOnWater = 0;
}
//-----------------------------------------------------------------------------
//Verifica se pode passar para o próximo nível
void checkNextRound() {
	//se chegou no tile superior
	if (plx == 9 && ply == 0) {
		//muda o round e reinicia a posição do jogador
		gameRound++;
		plx = 0;
		ply = 9;
	}
}
//-----------------------------------------------------------------------------
//Muda os tiles no mapa, de forma aleatória
void switchTiles() {
	//pega uma posição aleatória do mapa e troca com sua próxima
	int tileChange = rand() % (tilemapWidth*tilemapHeight);
	int aux = tileMapRound[gameRound][tileChange];
	tileMapRound[gameRound][tileChange] = tileMapRound[gameRound][(tileChange + 1) % (tilemapWidth*tilemapHeight)];
	tileMapRound[gameRound][(tileChange + 1) % (tilemapWidth*tilemapHeight)] = aux;
}
//-----------------------------------------------------------------------------
//Atualiza o estado do jogo
void update(int value) {
	//se estiver jogando
	if (playing) {
		timer->start();
		//faz a troca dos tiles
		switchTiles();
		//verifica se pode passar de round
		checkNextRound();
		//verifica se ocorreu colisão
		checkCollision();
		glutPostRedisplay();
		timer->finish();
		//calcula o o tempo de atualização necessario para os quadros
		int waitingTime = calcWaitingTime(GAME_UPDATE_FPS, timer->getElapsedTimeMs());
		// waitingTime é um valor em milisegundos
		glutTimerFunc(waitingTime, update, value);
		//se colidiu ou passou do último mapa, acaba o jogo
		if (collided || (plx == 9 && ply == 0) && gameRound == sizeof(tileMapRound))
			playing = false;
		//se ficou mais de 20 unidades na água, acaba o jogo
		if (timeOnWater > 20) {
			playing = false;
		}
	}
}
//-----------------------------------------------------------------------------
//Método principal do programa, gameLoop e inicializações
int main(int argc, char** argv) {
	createWindow(argc, argv);
	srand(time(NULL));
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	update(0);
	glutMainLoop();
	return 0; 
}

