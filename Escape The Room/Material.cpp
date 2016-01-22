#include "Material.h"

Material::Material(Vector3 materialEmissiveColor, Vector3 materialAmbientColor, Vector3 materialDiffuseColor, Vector3 materialSpecularColor, float materialShininess)
{
	this->materialAmbientColor = materialAmbientColor;
	this->materialDiffuseColor = materialDiffuseColor;
	this->materialEmissiveColor = materialEmissiveColor;
	this->materialShininess = materialShininess;
	this->materialSpecularColor = materialSpecularColor;
	
}

void Material::load(ShaderProgram* program)
{

	//Material stuff
	program->setUniformVector3("MaterialEmissiveColor", this->materialEmissiveColor);
	program->setUniformVector3("MaterialAmbientColor", this->materialAmbientColor);
	program->setUniformVector3("MaterialDiffuseColor", this->materialDiffuseColor);
	program->setUniformVector3("MaterialSpecularColor", this->materialSpecularColor);
	program->setUniformFloat("MaterialShininess", this->materialShininess);


}
