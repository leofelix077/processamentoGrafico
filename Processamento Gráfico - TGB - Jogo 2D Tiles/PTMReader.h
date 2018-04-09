#pragma once
#include "Image.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
//----------------------------------------------------------------------
using namespace std;
//----------------------------------------------------------------------
class PTMReader {
private:
	Image*image;
	int height, width;
public:
	//construtor padrão vazio
	PTMReader() {
		image = 0;
	}
	int getWidth() {
		return width;
	}
	int getHeight() {
		return height;
	}
	int* getPixels() {
		return image->getPixels();
	}
	Image *getImage() {
		return image;
	}
	//mostra mensagem de erro
	Image* showError() {
		cout << "Erro!" << endl;
		system("pause");
		return 0;
	}
	//----------------------------------------------------------------------
	//le o arquivo em modo binário
	void readBinary(ifstream *input) {
		//calcula o tamanho do buffer
		int elements = width * height * 4;
		//instancia o buffer
		char* buffer = new char[elements];
		//faz a leitura do resto do arquivo em um bloco
		input->read(buffer, elements);
		//iterador do buffer
		int i = 0;
		//preenche a imagem com os pixels
		for (int y = height - 1; y >= 0; y--)
			for (int x = 0; x < width; x++) {
				int r, g, b, a = 0;
				r = (unsigned char)buffer[i++];
				g = (unsigned char)buffer[i++];
				b = (unsigned char)buffer[i++];
				a = (unsigned char)buffer[i++];
				image->setPixel(r, g, b, a, x, y);
			}
	}
	//----------------------------------------------------------------------
	//le o arquivo em modo texto
	void readASCII(ifstream *input) {
		for (int y = height - 1; y >= 0; y--)
			for (int x = 0; x < width; x++) {
				int a, r, g, b = 0;
				*input >> a >> b >> g >> r;
				image->setPixel(a, r, g, b, x, y);
			}
	}
	//----------------------------------------------------------------------
	//faz leitura do arquivo
	Image * read(char *filename) {
		string fileType;
		ifstream input;
		input.open(filename, ios::binary);
		string aux;

		if (!input.good()) {
			showError();
		}
		//le o tipo de arquivo
		input >> fileType;
		char bufferA[1];
		input.getline(bufferA, 1);
		//se for linha de comentário, pula a linha
		if (bufferA[0] == '#')
			input >> aux;
		//le a altura e a largura
		input >> width >> height;
		image = new Image(width, height);
		//le o valor máximo do alpha (255)
		input >> aux;
		//pula o caractér separador de linha
		input.get();
		//se o arquivo for P7 -> Le em modo texto
		if (fileType == "P7") {
			readASCII(&input);
		}
		//se for P8 -> le em modo binario
		else if (fileType == "P8") {
			readBinary(&input);
			//caso contrário, lança uma exceçao
		}
		else throw new invalid_argument("Tipo de arquivo não suportado");
		input.close();
		return image;
	}
};