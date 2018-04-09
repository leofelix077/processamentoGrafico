#pragma once
#include <iostream>
#include "Image.h"
//----------------------------------------------------------------------
class Tileset {
public:
	//cria um array de tiles
	Tileset(Image *tileset, int tWidth, int tHeight) {
		//adiciona as informa��o de dimens�o de cada tile individual ao tileset
		this->tWidth = tWidth;
		this->tHeight = tHeight;
		//calcula a quantidade de tiles em cada eixo, baseado nas dimens�es de tile informadas e da imagem base
		int qtdTileVer = tileset->getHeight() / tHeight;
		int qtdTileHor = tileset->getWidth() / tWidth;
		qtdTiles = qtdTileVer * qtdTileHor;
		//instancia um novo array de Imagens que conter� os tiles
		tile = new Image*[qtdTiles];
		for (int id = 0; id < qtdTiles; id++) {
			//atribui as informa��es de largura e altura para cada tile espec�fico
			tile[id] = new Image(tWidth, tHeight);
			//calcula posi��o do tile na imagem base
			for (int x = 0; x < tWidth; x++) {
				for (int y = 0; y < tHeight; y++) {
					int xPos = (id % (qtdTileHor)) * tWidth;
					int yPos = (id / qtdTileHor) * tHeight;
					//busca as informa��es de cor e tranpar�ncia para o tile no array
					int argb = tileset->getPixel(x + xPos, y + yPos);
					//adiciona as informa��es de cores ao tile id no array
					tile[id]->setPixel(argb, x, y);
				}
			}
		}
	}
	//----------------------------------------------------------------------
	//retorna um tile baseado em seu ID �nico no array
	Image *getTileById(int id) {
		return tile[id];
	}
	//retorna qtd de tiles no tileset
	int getQtdTiles() {
		return qtdTiles;
	}
	int getTileWidth() {
		return tWidth;
	}
	int getTileHeight() {
		return tHeight;
	}
	//----------------------------------------------------------------------
private:
	Image **tile;
	int qtdTiles;
	int tWidth, tHeight;

	
};