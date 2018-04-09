#pragma once
#include "Vertex.h"
//define as coordenadas  de movimentação no mapa isométrico
#define NONE 0
#define NORTH 1
#define NORTHEAST 21
#define EAST 20
#define SOUTHEAST 22
#define SOUTH 2
#define SOUTHWEST 12
#define WEST 10
#define NORTHWEST 11
//----------------------------------------------------------------------
class IsoDiamond {
public:
	//instancia a visão isómetrica, com os valores dimensionais de cada tile e do mapa
	IsoDiamond(int th, int tw, int mh, int mw) {
		tileHeight = th;
		tileWidth = tw;
		mapHeight = mh;
		mapWidth = mw;
		//calcula as metades da altura e largura
		tileHeightHalf = th / 2;
		tileWidthHalf = tw / 2;
	}
	//----------------------------------------------------------------------
	//realiza a movimentação dentro das coordenadas da matriz
	void tileWalking(int &c, int &r, dir dir) {
		switch (dir) {
		case NORTH:
			c++;
			r--;
			break;
		case NORTHEAST:
			c++;
			break;
		case EAST:
			c++;
			r++;
			break;
		case SOUTHEAST:
			r++;
			break;
		case SOUTH:
			c--;
			r++;
			break;
		case SOUTHWEST:
			c--;
			break;
		case WEST:
			c--;
			r--;
			break;
		case NORTHWEST:
			r--;
			break;
		}
	}
	//----------------------------------------------------------------------
	//calcula a posição do mapa em relação à tela, "retornando" x e y por referência
	void calcPosDesenho(int c, int r, int &x, int &y) {
		x = c * tileWidthHalf + r * tileWidthHalf;
		y = c * tileHeightHalf - r * tileHeightHalf;
	}
	//----------------------------------------------------------------------
	//calcula a posição da tela em relação ao mapa, "retornando" c e r por referência
	void mouseMap(int x, int y, int &c, int &r) {
		c = ((x / tileWidthHalf) + (y / tileHeightHalf)) / 2.0;
		r = (-2.0 * y / tileHeight + c) + 0.5;
		int iniX = x;
		int iniY = y;
		calcPosDesenho(c, r, iniX, iniY);
		vert mc = calculate(iniX, iniY);
		if (mouseClick(mc, x, y)) {
			return;
		}
		dir walkDirection = NONE;
		if (mc.ay < y) {
			walkDirection += NORTH;
		}
		else if (mc.ay > y) {
			walkDirection += SOUTH;
		}
		if (mc.bx < x) {
			walkDirection += EAST;
		}
		else if (mc.bx > x) {
			walkDirection += WEST;
		}
		tileWalking(c, r, walkDirection);
	}
	//----------------------------------------------------------------------
	//calcula a posição na matriz onde ocorreu o click do mouse
	bool mouseClick(vert mc, int x, int y) {
		//testa o triangulo esquerdo
		int leftArea = areaTriangle(mc.ax, mc.ay, mc.bx, mc.by, mc.dx, mc.dy);
		//calcula a área de cada triangulo menos individualmente
		int leftClickArea1 = areaTriangle(x, y, mc.bx, mc.by, mc.dx, mc.dy);
		int leftClickArea2 = areaTriangle(x, y, mc.ax, mc.ay, mc.dx, mc.dy);
		int leftClickArea3 = areaTriangle(x, y, mc.ax, mc.ay, mc.bx, mc.by);
		//atribui a área total do triangulo
		int leftClickArea = leftClickArea1 + leftClickArea2 + leftClickArea3;
		//compara os módulos dos dois triângulos e conclui se houve colisão
		if (abs(leftArea - leftClickArea) <= 1) {
			return true;
		}
		int rightArea = areaTriangle(mc.cx, mc.cy, mc.dx, mc.dy, mc.bx, mc.by);
		int rightClickArea1 = areaTriangle(x, y, mc.dx, mc.dy, mc.bx, mc.by);
		int rightClickArea2 = areaTriangle(x, y, mc.cx, mc.cy, mc.bx, mc.by);
		int rightClickArea3 = areaTriangle(x, y, mc.cx, mc.cy, mc.dx, mc.dy);
		int rightClickArea = rightClickArea1 + rightClickArea2 + rightClickArea3;
		if (abs(rightArea - rightClickArea) <= 1) {
			return true;
		}
		else {
			return false;
		}
	}
	//----------------------------------------------------------------------
private:
	int tileHeight;
	int tileWidth;
	float tileHeightHalf;
	float tileWidthHalf;
	int mapHeight;
	int mapWidth;
};