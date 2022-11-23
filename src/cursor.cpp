#include "cursor.h"

//#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_resize.h"
#include "stb_image_write.h"


customCursor::customCursor(const std::string& filepath, const uint8_t& size, bool isAligned) :
	stb_img(filepath, size, size),
	tex_img()
{
	tex_img.height = stb_img.height();
	tex_img.width = stb_img.width();
	tex_img.pixels = stb_img.getData();

	if(!isAligned) cursor = glfwCreateCursor(&tex_img, size / 2, size / 2);
	else cursor = glfwCreateCursor(&tex_img, 0, 0);
}

customCursor::~customCursor() {
	stb_img.~image();
}
