#pragma once
#include "Image.h"
//----------------------------------------------------------------------
//classe descontinuada
class Tile {
public:
	Tile(int id, Image *img) {
		this->id = id;
		this->img = img;
	}
	int getId() {
		return id;
	}
	//----------------------------------------------------------------------
private:
	int id;
	Image *img;
};
