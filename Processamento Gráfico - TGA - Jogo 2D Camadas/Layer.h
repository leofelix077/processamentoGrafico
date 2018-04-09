#pragma once
#include "Image.h"
enum direction{RIGHT = 0, LEFT = 1};
class Layer {
private:
	float rateX;
	float rateY;
	float posX;
	float posY;
	Image* layer;
	int layerDepth;
public:
	Layer(Image* layerImg, int posXX,int posYY, int newLayerDepth) {
		layer = layerImg;
		posX = posXX;
		posY = posYY;
		layerDepth = newLayerDepth;
	}
	void scroll(int direction, int amount) {
		if (direction == LEFT) {
			posX += amount * rateX;
		}
		else if (direction == RIGHT) {
			posX += amount * rateX;
		}
	}
	void computeScrollRateX(int layerNumber) {
		rateX = 1 + 0.5*layerNumber;
	}
Image* getLayer() {
		return layer;
	}
	float getPosX() {
		return posX;
	}
	float getPosY() {
		return posY;
	}
	int getLayerDepth() {
		return layerDepth;
	}
	void setLayerDepth(int newDepth) {
		layerDepth = newDepth;
	}
};