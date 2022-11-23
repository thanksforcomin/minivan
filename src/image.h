#pragma once

#include <string>
#include <cstring>
#include <iostream>

#include "stb_image.h"

class image
{
private:
	std::string m_filepath; //a filepath, how unobvious
	unsigned char* m_data; //local data buffer
	int m_width;
	int m_height;
	int m_colorChannels; //variables of image itself
public:
	int m_imageId; //debug info

	friend image;
public:
	image(const std::string& filepath); //creator with filepath
	image(const std::string& filepath, const int& width, const int& height); //creator with filepath and size parameters
	image(const image& source); //copying 
	image(image&& source); //moving

	~image(); //destructor

	//overloaded operator (why)
	image& operator=(const image& img) {
		if (this != &img) {
			std::cout << "Copying image " << img.m_imageId << " to " << this->m_imageId << "\n";	
			stbi_image_free(m_data);
			m_data = new unsigned char[(img.m_height * img.m_width * img.m_colorChannels)];
			m_filepath = img.m_filepath;
			memcpy(m_data, img.m_data, img.m_height * img.m_width * img.m_colorChannels);
			//m_data = img.m_data;
			m_height = img.m_height;
			m_width = img.m_width;
			m_colorChannels = img.m_colorChannels;
		}
		return *this;
	}

	inline unsigned char* getData() { return m_data; }
	inline int height() { return m_height; }
	inline int width() { return m_width; }
	inline int channels() { return m_colorChannels; }
	bool isLoaded() {
		if (m_data) return true;
		else return false;
	}
	inline std::string filepath() { return m_filepath; }

	void resize(const int& width, const int& height);
};
