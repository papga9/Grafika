#include "DrawableTexture.h"
#include "stb_image.h"

void DrawableTexture::refreshVertices()
{
	if (!shouldFlip)
	{
		vertices[0] = currentPosition.first + renderWidth / 2.0;
		vertices[1] = currentPosition.second + renderWidth / 2.0;
		vertices[8] = currentPosition.first + renderWidth / 2.0;
		vertices[9] = currentPosition.second - renderWidth / 2.0;
		vertices[16] = currentPosition.first - renderWidth / 2.0;
		vertices[17] = currentPosition.second - renderWidth / 2.0;
		vertices[24] = currentPosition.first - renderWidth / 2.0;
		vertices[25] = currentPosition.second + renderWidth / 2.0;
	}
	else {
		vertices[24] = currentPosition.first + renderWidth / 2.0;
		vertices[25] = currentPosition.second + renderWidth / 2.0;
		vertices[16] = currentPosition.first + renderWidth / 2.0;
		vertices[17] = currentPosition.second - renderWidth / 2.0;
		vertices[8] = currentPosition.first - renderWidth / 2.0;
		vertices[9] = currentPosition.second - renderWidth / 2.0;
		vertices[0] = currentPosition.first - renderWidth / 2.0;
		vertices[1] = currentPosition.second + renderWidth / 2.0;
	}
}


std::pair<double, double> DrawableTexture::getPosition() const
{
	return currentPosition;
}

DrawableTexture::DrawableTexture(std::string vs, std::string fs, std::string _path, double _rw,  std::pair<double, double> spawnPoint) :
	Drawable(vs, fs),
	renderWidth(_rw),
	currentPosition(spawnPoint),
	path(_path)
{
	std::cout << spawnPoint.first << " " << spawnPoint.second << std::endl;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// texture 1
	// ---------
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);    // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
	// either set it manually like so:
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture"), 0);
}

DrawableTexture::~DrawableTexture()
{
	//TODO fix
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
}

void DrawableTexture::draw()
{
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	static const unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	refreshVertices();

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	// render container
	ourShader.use();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
