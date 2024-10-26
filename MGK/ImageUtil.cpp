#include "ImageUtil.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLfloat ImagePannel[][48] = {  // default size 2.0 * 2.0
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0, 0.0,
	 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0, 0.0,
	 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0, 1.0,
	 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0, 1.0,
	-1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0, 1.0,
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0, 0.0
};

void ImageUtil::Init() {
	GLuint VBO{};
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenFramebuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(ImagePannel), ImagePannel, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	stbi_set_flip_vertically_on_load(true);
}

void ImageUtil::Import(Image& ImageStruct, const char* FileName, int Type) {
	int Width{}, Height{}, Channel{};

	glGenTextures(1, &ImageStruct.Texture);
	glBindTexture(GL_TEXTURE_2D, ImageStruct.Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if (Type == IMAGE_TYPE_LINEAR) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	else if (Type == IMAGE_TYPE_NEAREST) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	unsigned char* texture_data = stbi_load(FileName, &Width, &Height, &Channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	stbi_image_free(texture_data);

	ImageStruct.Width = Width;
	ImageStruct.Height = Height;
}

void ImageUtil::Render(Image& ImageStruct) {
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, ImageStruct.Texture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void ImageUtil::Release(Image& ImageStruct) {
	glDeleteTextures(1, &ImageStruct.Texture);
}