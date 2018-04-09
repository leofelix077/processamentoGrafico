#pragma once
#include "PTMReader.h"
#include "Image.h"
#include "Tileset.h"
#include "Tilemap.h"
//----------------------------------------------------------------------
class TilemapCreator {
public:
	//adiciona as informações de nome do arquivo, dimensões do tile e dimensões do map ao objeto
	TilemapCreator(char *tilesetName, int tileWidth, int tileHeight, int tilemapWidth, int tilemapHeight) {
		this->tilesetName = tilesetName;
		this->tileWidth = tileWidth;
		this->tileHeight = tileHeight;
		this->tilemapWidth = tilemapWidth;
		this->tilemapHeight = tilemapHeight;
	}
	//----------------------------------------------------------------------
	//Cria e retorna um tilemap baseado nas informções de entrada
	Tilemap* createMap() {
		//Instacia o leitor para arquivos de imagem
		PTMReader* ptm = new PTMReader();
		//cria um tileset e o lê a partir de um arquivo de imagem
		Tileset* tileset = new Tileset(ptm->read(tilesetName), tileWidth, tileHeight);
		//cria o tilemap a ser retornado
		Tilemap* tilemap = new Tilemap(tilemapWidth, tilemapWidth, tileset);
		//Popula o tilemap
		  for (int y = 0; y < tilemapWidth; y++) {
			for (int x = 0; x < tilemapHeight; x++) {
				tilemap->setTile(x+ y*tilemapWidth, x, y);
			}
		}
		return tilemap;
	}
	//----------------------------------------------------------------------
private:
	char *tilesetName;
	int tileWidth, tileHeight;
	int tilemapWidth, tilemapHeight;
};