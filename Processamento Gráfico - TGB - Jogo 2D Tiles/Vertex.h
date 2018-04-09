#pragma once
#include <cmath>
//define as informa��es de tamanho da janela
#define INITIAL_SCREEN_HEIGHT 720
#define INITIAL_SCREEN_WIDTH 1280
//define o tipo "dir" para ser usado nos m�todos de movimenta��o
typedef int dir;
//define o tamanho padr�o de cada pol�gono no grid
#define TILE_HEIGHT 64
#define TILE_WIDTH 128
//----------------------------------------------------------------------
//cria uma struct para guardar e facilitar o acesso aos v�rtices de cada pol�gono no grid
typedef struct {
	float ax, bx, cx, dx;
	float ay, by, cy, dy;
} vert;
//calcula os v�rtices baseado na posi��o x,y inicial levando em considera��o o tamanho padrao dos pol�gonos
vert calculate(int x, int y) {
	vert ver;
	ver.ax = x;
	ver.bx = x + TILE_WIDTH / 2;
	ver.cx = x + TILE_WIDTH;
	ver.dx = x + TILE_WIDTH / 2;
	ver.ay = y + TILE_HEIGHT / 2;
	ver.by = y + TILE_HEIGHT;
	ver.cy = y + TILE_HEIGHT / 2;
	ver.dy = y;
	return ver;
}
//----------------------------------------------------------------------
//calcula a �rea do tr~i�ngulo conforme slides
int areaTriangle(int ax, int ay, int bx, int by, int cx, int cy) {
	return abs((ax * (by - cy) + bx * (cy - ay) + cx * (ay - by)) / 2);
}