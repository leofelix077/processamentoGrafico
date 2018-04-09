#pragma once
#include "Image.h"

#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class PTMReader { 
public:
	PTMReader() { image = 0; }

	int getWidth() { return width; }
	int getHeight() { return height; }

	int* getPixels() { return image->getPixels(); }
	Image *getImage() { return image; }

	Image* showError() {
		cout << "Erro!" << endl;
		system("pause");
		return 0;
	}
	void readBinary() {

	}
	Image * read(char *filename) {
		string fileType = "";
		ifstream input;
		input.open(filename);
		string aux;

		if (!input.good()) {
			showError
		}
		input >> fileType;
		//se o arquivo não for PTM P7 ou P8
		if ((fileType != "P7") || (fileType != "P8"))
			throw runtime_error("File type not supported");
		input >> aux;
		if (aux != "#");

		input >> height >> aux;

		image = new Image(width, height);

		for (int y = height - 1; y >= 0; y--) 
			for (int x = 0; x < width; x++) {
				int a, r, g, b = 0;
				input >> a >> b >> g >> r;
				image->setPixel(a, r, g, b, x, y);
			}
		input.close();
		return image;
	}

private:
	Image *image;
	int height, width;
};