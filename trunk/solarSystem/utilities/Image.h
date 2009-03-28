#ifndef IMAGE_H_
#define IMAGE_H_

#include <stdint.h>
#include <string>

class Image {
private:
	int width;
	int height;
	uint8_t* data;
public:
	Image();
	bool readFile(std::string);
	int getHeight();
	int getWidth();
	uint8_t* getData();
	virtual ~Image();
};

#endif /* IMAGE_H_ */
