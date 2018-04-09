#pragma once
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <sstream>

#include "PTMReader.h"
#include "Tilemap.h"
#include "Tileset.h"

using namespace std;

class MapReader {
private:
	int width, height;
	int tileDefault;
	int widthSizePx, heightSizePx;
	char pathTileset[256];

public:
	MapReader() {

	}

	Tilemap *showError() {
		cout << "Leitor MF: Erro de abertura de arquivo!" << endl;
		system("pause");
		return 0;
	}

	Tilemap* read(char* filename) {
		char buffer[256];
		ifstream arq(filename);

		// Get the tileset name
		arq >> buffer;
		char tilesetName[256];
		// strcpy_s(tilesetName, buffer);
		strcpy_s(tilesetName, buffer);

		// Get the width and height
		int tileDefault;
		int height;
		int width;
		arq >> buffer;
		tileDefault = atoi(buffer);
		arq >> buffer;
		width = atoi(buffer);
		arq >> buffer;
		height = atoi(buffer);

		// Create the tilemap
		PTMReader* ptm = new PTMReader();
		Tileset* tileset = new Tileset(ptm->read(tilesetName), 16, 16);
		Tilemap* tilemap = new Tilemap(width, height, tileset);

		while (!arq.eof()) {
			int tile, x, y;
			arq >> buffer;
			if (buffer[0] == '#') {
				arq.getline(buffer, 256);
				arq >> buffer;
			}
			tile = atoi(buffer);
			arq >> buffer;
			x = atoi(buffer);
			arq >> buffer;
			y = atoi(buffer);
			if (arq.eof()) break;
			tilemap->setTile(tile, x, y);
		}

		arq.close();

		delete ptm;

		return tilemap;
	}
};