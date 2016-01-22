#include "LightSource.h"


LightSource::LightSource(Vector3 lightPosition, Vector3 lightAmbientColor, Vector3 lightDiffuseColor, Vector3 lightSpecularColor,
						float lightRange, Vector3 lightAttenuation, int lightNumber)
{
	this->lightPosition = lightPosition;
	this->lightAmbientColor = lightAmbientColor;
	this->lightDiffuseColor = lightDiffuseColor;
	this->lightSpecularColor = lightSpecularColor;
	this->lightRange = lightRange;
	this->lightAttenuation = lightAttenuation;
	this->lightNumber = lightNumber;
}

void LightSource::load(Matrix4 globalMatrix,ShaderProgram* program)
{
	program->setUniformVector3InArray("LightPosition", globalMatrix*this->lightPosition, lightNumber);
	program->setUniformVector3InArray("LightAmbientColor", this->lightAmbientColor, lightNumber);
	program->setUniformVector3InArray("LightDiffuseColor", this->lightDiffuseColor, lightNumber);
	program->setUniformVector3InArray("LightSpecularColor", this->lightSpecularColor, lightNumber);
	program->setUniformFloatInArray("LightRange", this->lightRange, lightNumber);
	program->setUniformVector3InArray("LightAttenuation", this->lightAttenuation, lightNumber);
}
