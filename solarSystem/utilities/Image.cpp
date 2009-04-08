/*
 * Image.cpp
 *
 *  Created on: 27-Mar-2009
 *      Author: sscull
 */

#include <fstream>
#include <iostream>
#include "Image.h"

Image::Image() {
}

int Image::getHeight() {
	return height;
}

int Image::getWidth() {
	return width;
}

uint8_t* Image::getData() {
	return data;
}

bool Image::readFile(std::string filename) {
	std::ifstream imgFile(filename.c_str(), std::ios::in | std::ios::binary);
	if (!imgFile) {
		std::cout<<filename.c_str() << " FAIL!" << std::endl;
		return 0;
	}
	uint16_t dummy16;
	uint32_t w, h, dummy32;

	/*
	 imgFile >>
	 dummy16 >> // "BM"
	 dummy32 >> // file size
	 dummy16 >> // reserved 1
	 dummy16 >> // reserved 2
	 dummy32 >> // offset to image (unreliable)
	 dummy32 >> // header size
	 w >>       // image width
	 h >>       // image height
	 dummy16 >> // planes
	 dummy16 >> // BPP
	 dummy32 >> // compression
	 dummy32 >> // image size in bytes
	 dummy32 >> // xpels
	 dummy32 >> // ypels
	 dummy32 >> // size of palette
	 dummy32;   // impColors
	 */

	imgFile.read((char*) &dummy16, sizeof(dummy16));
	imgFile.read((char*) &dummy32, sizeof(dummy32));
	imgFile.read((char*) &dummy16, sizeof(dummy16));
	imgFile.read((char*) &dummy16, sizeof(dummy16));

	imgFile.read((char*) &dummy32, sizeof(dummy32));
	imgFile.read((char*) &dummy32, sizeof(dummy32));

	imgFile.read((char*) &w, sizeof(dummy32));
	imgFile.read((char*) &h, sizeof(dummy32));

	imgFile.read((char*) &dummy16, sizeof(dummy16));
	imgFile.read((char*) &dummy16, sizeof(dummy16));

	imgFile.read((char*) &dummy32, sizeof(dummy32));
	imgFile.read((char*) &dummy32, sizeof(dummy32));
	imgFile.read((char*) &dummy32, sizeof(dummy32));
	imgFile.read((char*) &dummy32, sizeof(dummy32));
	imgFile.read((char*) &dummy32, sizeof(dummy32));
	imgFile.read((char*) &dummy32, sizeof(dummy32));

	width = w;
	height = h;
	data = new uint8_t[3 * w * h];
	for (uint32_t i = 0; i < h; ++i) {
		for (uint32_t j = 0; j < w; ++j) {
			uint8_t r, g, b;
			uint8_t* pix = data + (3 * (i * w + j));
			imgFile.read((char*) &b, sizeof(b));
			imgFile.read((char*) &g, sizeof(g));
			imgFile.read((char*) &r, sizeof(r));
			*(pix + 0) = r;
			*(pix + 1) = g;
			*(pix + 2) = b;
		}
	}
	imgFile.close();
	return 1;
}

Image::~Image() {
}
