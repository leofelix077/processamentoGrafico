#pragma once
#include "Animation.h"
class GameObject {
public:
	GameObject() {
		currentFrame = 15;
		//altera posição do sprite
		posX = 400;
		posY = 90;
	}
	//passa uma nova posição aos frames a serem demonstrados
	void changeSpriteOrientation(int newOrientation) {
		orientation = newOrientation;
	}
	//mantem os frames de uma linha em ciclo
	void incCurrentFrame() {
		currentFrame = orientation * 4 + (currentFrame + 1) % 4;
	}
	void incCurrentFrameFull() {
		if(currentFrame > 0)
		currentFrame = (currentFrame - 1);
		else currentFrame++ % 4;
	}
	void setSprite(Animation * anim) {
		a = anim;
	}
	Image * getCurrentFrame() {
		return a->getFrame(currentFrame);
	}
	int getPosX() {
		return posX;
	}
	int getPosY() {
		return posY;
	}
private:
	Animation *a;
	int currentFrame;
	int posX, posY;
	int orientation;
};

