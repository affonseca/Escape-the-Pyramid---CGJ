#include "ShaderProgram.h"
#include "Texture.h"

ShaderProgram::ShaderProgram() {}

void ShaderProgram::addAttribute(std::string attribute, GLuint index) {
	attributes[attribute] = { index };
}

void ShaderProgram::addUniform(std::string uniform) {
	uniforms[uniform] = { -1 };
}

void ShaderProgram::addUniformBlock(std::string uniformBlock, GLuint bindingPoint) {
	uniformBlocks[uniformBlock] = { 0, bindingPoint };
}

void ShaderProgram::create(std::string vertexShaderFilePath, std::string fragmentShaderFilePath) {

	std::ifstream vertexFile(vertexShaderFilePath);
	std::string vertexShader((std::istreambuf_iterator<char>(vertexFile)),
		std::istreambuf_iterator<char>());

	std::ifstream fragmentFile(fragmentShaderFilePath);
	std::string fragmentShader((std::istreambuf_iterator<char>(fragmentFile)),
		std::istreambuf_iterator<char>());


	const char * c_vertexShader = vertexShader.c_str();
	const char * c_fragmentShader = fragmentShader.c_str();

	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &c_vertexShader, 0);
	glCompileShader(vertexShaderID);

	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &c_fragmentShader, 0);
	glCompileShader(fragmentShaderID);

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	for (std::map<std::string, attributeInfo>::iterator iterator = attributes.begin(); iterator != attributes.end(); ++iterator) {
		std::string attributeLocation = iterator->first;
		const char * c_attributeLocation = attributeLocation.c_str();
		glBindAttribLocation(programID, iterator->second.index, c_attributeLocation);
	}

	glLinkProgram(programID);

	for (std::map<std::string, uniformInfo>::iterator iterator = uniforms.begin(); iterator != uniforms.end(); ++iterator) {
		std::string uniformLocation = iterator->first;
		const char * c_uniformLocation = uniformLocation.c_str();
		iterator->second.index = glGetUniformLocation(programID, c_uniformLocation);
	}
}


std::string ShaderProgram::getCompilationResult() {
	GLint success = 0;

	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
		return getErrorLog(vertexShaderID);

	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
		return getErrorLog(fragmentShaderID);
	{

		glGetProgramiv(programID, GL_LINK_STATUS, (int *)&success);
		if (success == GL_FALSE)
		{
			return getErrorLog(programID);
		}

		return std::string();

	}
}

std::string ShaderProgram::getErrorLog(GLuint id) {
	GLint maxLength = 0;
	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &maxLength);
	if (maxLength == 0) {
		return "unknown error\n";
	}

	std::vector<GLchar> infoLog(maxLength);
	glGetProgramInfoLog(id, maxLength, &maxLength, &infoLog[0]);
	std::string out(infoLog.begin(), infoLog.end());

	dispose();

	return out;
}

void ShaderProgram::dispose() {
	glUseProgram(0);
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);

	glDeleteShader(fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteProgram(programID);
}


void ShaderProgram::begin() {
	glUseProgram(programID);
}
void ShaderProgram::end() {
	glUseProgram(0);
}

void ShaderProgram::postProcess()
{
}

void ShaderProgram::setUniformMatrix(std::string name, Matrix matrix) {
	glUniformMatrix4fv(uniforms[name].index, 1, GL_FALSE, matrix.getValues());
}

void ShaderProgram::setUniformVector2(std::string name, Vector2 vector2) {
	glUniform2f(uniforms[name].index, vector2.x, vector2.y);
}

void ShaderProgram::setUniformVector3(std::string name, Vector3 vector3) {
	glUniform3f(uniforms[name].index, vector3.x, vector3.y, vector3.z);
}

void ShaderProgram::setUniformVector3InArray(std::string name, Vector3 vector3, int index) {
	std::string accessString = name + "[" + std::to_string(index) + "]";
	GLuint locationID = glGetUniformLocation(programID, accessString.c_str());
	glUniform3f(locationID, vector3.x, vector3.y, vector3.z);
}

void ShaderProgram::setUniformFloat(std::string name, float value) {
	glUniform1f(uniforms[name].index, value);
}

void ShaderProgram::setUniformFloatInArray(std::string name, float value, int index)
{
	std::string accessString = name + "[" + std::to_string(index) + "]";
	GLuint locationID = glGetUniformLocation(programID, accessString.c_str());
	glUniform1f(locationID, value);
}

void ShaderProgram::setUniformInt(std::string name, int value)
{
	glUniform1i(uniforms[name].index, value);
}

void ShaderProgram::setUniform2DSampler(std::string name, GLuint samplerID, GLuint textureUnit)
{
	std::map<const std::string, uniformInfo>::iterator it = uniforms.find(name);
	if (it == uniforms.end())
		return;

	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, samplerID);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(uniforms[name].index, textureUnit);
}

MultiPassShaderProgram::MultiPassShaderProgram(Vector2 screenSize)
{
	this->screenSize = screenSize;


	//Creating the depth buffer
	glGenRenderbuffers(1, &dboID);
	glBindRenderbuffer(GL_RENDERBUFFER, dboID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, (GLsizei)screenSize.x, (GLsizei)screenSize.y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenRenderbuffers(1, &sboID);
	glBindRenderbuffer(GL_RENDERBUFFER, sboID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (GLsizei)screenSize.x, (GLsizei)screenSize.y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void MultiPassShaderProgram::addOutputTexture(std::string name)
{
	Texture texture = Texture((GLsizei)screenSize.x, (GLsizei)screenSize.y, NULL);
	texture.create();
	textures.push_back({ texture.getTexId(), name });
}

void MultiPassShaderProgram::create(std::string vertexShaderFilePath, std::string fragmentShaderFilePath, ShaderProgram * next)
{
	ShaderProgram::create(vertexShaderFilePath, fragmentShaderFilePath);
	this->next = next;

	glGenFramebuffers(1, &frameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

	drawBuffers = new GLenum[textures.size()];

	int i = 0;
	for (std::vector<textureInfo>::iterator iterator = textures.begin(); iterator != textures.end(); ++iterator) {
		glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, iterator->index, 0);
		drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i; //TODO set value by user?
		i++;
	}
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, dboID);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, sboID);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//creating the post-processing vertices
	static const GLfloat quad_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,
	};

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	glGenBuffers(1, &vertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertex_buffer_data), quad_vertex_buffer_data, GL_STATIC_DRAW);

	glVertexAttribPointer(MultiPassShaderProgram::screenCoordIndex, 3, GL_FLOAT, GL_FALSE, 0, 0); //TODO set value by user?
	glEnableVertexAttribArray(MultiPassShaderProgram::screenCoordIndex);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

std::string MultiPassShaderProgram::getCompilationResult()
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return "Error creating framebuffer";
	return ShaderProgram::getCompilationResult();
}

void MultiPassShaderProgram::reshape(Vector2 screenSize)
{
	this->screenSize = screenSize;

	for (std::vector<textureInfo>::iterator iterator = textures.begin(); iterator != textures.end(); ++iterator) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, iterator->index);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)screenSize.x, (GLsizei)screenSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		Texture::setParams();
	}

	glBindRenderbuffer(GL_RENDERBUFFER, dboID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, (GLsizei)screenSize.x, (GLsizei)screenSize.y);
	glBindRenderbuffer(GL_RENDERBUFFER, sboID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (GLsizei)screenSize.x, (GLsizei)screenSize.y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void MultiPassShaderProgram::dispose()
{
	if (next != NULL)
		next->dispose();
	ShaderProgram::dispose();

	for (std::vector<textureInfo>::iterator iterator = textures.begin(); iterator != textures.end(); ++iterator) {
		glDeleteTextures(1, &(iterator->index));
	}
	glDeleteRenderbuffers(1, &dboID);

	glDeleteFramebuffers(1, &frameBufferID);
	delete drawBuffers;
}

void MultiPassShaderProgram::begin()
{
	if (next != NULL) {
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
		glDrawBuffers(textures.size(), drawBuffers);
	}
	ShaderProgram::begin();
}

void MultiPassShaderProgram::end()
{
	ShaderProgram::end();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void MultiPassShaderProgram::postProcess()
{
	//draw the postProcessing
	if (next != NULL) {

		next->begin();
		GLuint i = 0;
		for (std::vector<textureInfo>::iterator iterator = textures.begin(); iterator != textures.end(); ++iterator) {
			next->setUniform2DSampler(iterator->name, iterator->index, i);
			i++;
		}

		next->setUniformVector2("WindowResolution", screenSize);
		glBindVertexArray(vaoID);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		next->end();
		next->postProcess();
	}
}

void MultiPassShaderProgram::clear()
{
	if (next != NULL) {
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClearStencil(0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
