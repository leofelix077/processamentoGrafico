#pragma once
#include "Image.h"
#include "Tileset.h"
//----------------------------------------------------------------------
class Tilemap {
public:
	Tilemap(int x, int y, Tileset *tileset) {
		//adiciona as informaçoes de dimensões(colunas e linhas) para o objeto tilemap
		this->tileset = tileset;
		this->height = y;
		this->width = x;
		//instancia o array bidimensional de inteiros
		tilemap = new int*[y];
		for (int i = 0; i < y; i++) {
			tilemap[i] = new int[x];
			for (int j = 0; j < x; j++) {
				//popula o array com "-1", para sinalizar as posições vazias
				tilemap[i][j] = -1;
			}
		}
	}
	//----------------------------------------------------------------------
	//retorna a qtd de tiles no tileset
	int getTilesCounter() {
		return this->tileset->getQtdTiles();
	}
	//retorna a informação de imagem de um tile no tileset pelo seu id
	Image* getTileById(int id) {
		return this->tileset->getTileById(id);
	}
	//retorna o tileset
	Tileset *getTileset() {
		return tileset;
	}
	//retorna o id de um tile na coluna e linha informada
	int getTile(int x, int y) {
		return this->tilemap[x][y];
	}
	//retorna o id tile na posição infomada
	int getTile(int pos) {
		return this->tilemap[pos/width][pos%height];
	}
	//adiciona o id de um tile na posição informada
	void setTile(int id, int x, int y) {
		this->tilemap[y][x] = id;
	}
	int getWidth() {
		return this->width;
	}
	int getHeight() {
		return this->height;
	}
	//----------------------------------------------------------------------
private:
	Tileset *tileset;
	int **tilemap;
	int height, width;
};
