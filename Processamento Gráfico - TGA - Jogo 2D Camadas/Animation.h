#pragma once
#include "Image.h"
#include <vector>

//classe Animation baseada no esquema dos slides

class Animation {
public:
	Animation() {
		frameCount = 16;
		for (int i = 0; i < frameCount; i++) {
			f.push_back(new Image(64, 64));
		}
	}
	void addFrame(Image *frame) {
		for (int i = 0; i < 4; i++) 
			for (int j = 0; j < 4; j++){
				frame->subimage(f[j + 4*i], j * 64, 192 - i*64);
			}
	}
	Image * getFrame(int index) {
		return f[index];
	}
private:
	int frameCount;
	vector <Image*> f;
};