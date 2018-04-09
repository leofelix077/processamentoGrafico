#pragma once
#include "PTMReader.h"
#include <windows.h>

using namespace std;

class Image {
public:
	Image() { pixels = 0;  }
	Image(int w, int h) {
		width = w; height = h;
		pixels = new int[w*h];
	}
	void setPixel(int rgb, int x, int y) {
		pixels[x + y*width] = rgb;
	}
	int getPixel(int x, int y) {
		return pixels[x + y*width];
	}
	int getWidth() { return width; }
	int getHeight() { return height; }

	int *getPixels() { return pixels; }
	void setPixel(int r, int g, int b, int x, int y) {
		int rgb = (r << 16) | (g << 8) | b;
		pixels[x + y*width] = rgb;
	}
	void setPixel(int a, int r, int g, int b, int x, int y) {
		int argb = (a << 24) | (r << 16) | (g << 8) | b;
		pixels[x + y*width] = argb;
	}

	void plot(Image *foreground, int startx, int starty, char *zbuffer, char z) {
		for (int y = starty; y < foreground->getHeight() + starty; y++) {
			for (int x = startx; x < foreground->getWidth() + startx; x++) {
				int rgbFG = foreground->getPixel(x - startx, y - starty);
				//separa os canais alfa da imagem posterior
				int alphaFG = (rgbFG >> 24) & 0xff;
				int rFG = (rgbFG >> 16) & 0xff;
				int gFG = (rgbFG >> 8) & 0xff;
				int bFG = rgbFG & 0xff;
				//quando o pixel é "transparente", pula um ciclo do loop
				if (alphaFG == 0) continue;
				//se a camada for maior que a anterior armazenada
				if (z>= zbuffer[x + y*width])
					//quando o pixel é opaco o copia a frente do plano de fundo
					if (alphaFG == 255) {
						rgbFG = (alphaFG << 24) | (rFG << 16) | (gFG << 8) | bFG;
						this->setPixel(rgbFG, x, y);
						//atribui a camada que foi desenhada ao zbuffer
						zbuffer[x + y  * width] = z;
					}
				//se o pixel tem algum grau de transparência calcula a interpolação de cores
					else if (alphaFG > 0) {
						int a = (int)(alphaFG / 255.0f);
						int rgbBG = getPixel(x, y);
						int alphaBG = (rgbBG >> 24) & 0xff;
						int rBG = (rgbBG >> 16) & 0xff;
						int gBG = (rgbBG >> 8) & 0xff;
						int bBG = rgbBG & 0xff;
						rFG = (rBG*(1 - a) + rFG*a);
						gFG = (gBG*(1 - a) + gFG*a);
						bFG = (bBG*(1 - a) + bFG*a);
						rgbFG = (alphaFG << 24) | (rFG << 16) | (gFG << 8) | bFG;
						this->setPixel(foreground->getPixel(x - startx, y - starty), x, y);
					}
			}
		}
	}
	void subimage(Image *dest, int srcX, int srcY) {
		for (int y = 0; y < dest->getHeight(); y++) {
			for (int x = 0; x < dest->getWidth(); x++) {
				int RGB = this->getPixel(x + srcX, y + srcY);
				dest->setPixel(RGB, x, y);
			}
		}
	}
private:
	int *pixels;
	int width, height;
};