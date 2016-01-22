#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include "GL\glew.h"
#include "Matrix.h"

struct attributeInfo {
	GLuint index;
};

struct uniformInfo {
	GLint index;
};

struct uniformBlockInfo {
	GLuint index;
	GLuint bindingPoint;
};

struct textureInfo {
	GLuint index;
	std::string name;
};

class ShaderProgram {
public:
	ShaderProgram();
	void addAttribute(std::string attribute, GLuint index);
	void addUniform(std::string uniform);
	void addUniformBlock(std::string uniformBlock, GLuint bindingPoint);
	virtual void create(std::string vertexShaderFilePath, std::string fragmentShaderFilePath);
	virtual std::string getCompilationResult();
	virtual void dispose();
	virtual void begin();
	virtual void end();
	virtual void postProcess();
	void setUniformMatrix(std::string name, Matrix matrix);
	void setUniformVector2(std::string name, Vector2 vector2);
	void setUniformVector3(std::string name, Vector3 vector3);
	void setUniformVector3InArray(std::string name, Vector3 vector3, int index);
	void setUniformFloat(std::string name, float value);
	void setUniformFloatInArray(std::string name, float value, int index);
	void setUniformInt(std::string name, int value);
	void setUniform2DSampler(std::string name, GLuint samplerID, GLuint textureUnit);

protected:
	GLuint programID, vertexShaderID, fragmentShaderID;
	std::map <std::string, attributeInfo> attributes;
	std::map <std::string, uniformInfo> uniforms;
	std::map <std::string, uniformBlockInfo> uniformBlocks;
	std::string getErrorLog(GLuint id);
};

class MultiPassShaderProgram : public ShaderProgram {
public:
	const static GLuint screenCoordIndex = 0;

	MultiPassShaderProgram(Vector2 screenSize);
	void addOutputTexture(std::string name);
	virtual void create(std::string vertexShaderFilePath, std::string fragmentShaderFilePath, ShaderProgram* next);
	virtual std::string getCompilationResult();
	void reshape(Vector2 screenSize);
	virtual void dispose();
	virtual void begin();
	virtual void end();
	virtual void postProcess();
	void clear();

private:
	float counter = 0;
	std::vector<textureInfo> textures;
	ShaderProgram * next;
	GLuint frameBufferID, vaoID, vertexVBO, dboID, sboID;
	Vector2 screenSize;
	GLuint* drawBuffers;
};