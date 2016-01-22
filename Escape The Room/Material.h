#pragma once
#include "Vector.h"
#include "ShaderProgram.h"
/*////////////////////////////////
//			Material 			//
////////////////////////////////*/

class Material {
private:
	Vector3 materialEmissiveColor;
	Vector3 materialAmbientColor;
	Vector3 materialDiffuseColor;
	Vector3 materialSpecularColor;
	float materialShininess;
	

public:
	Material(   Vector3 materialEmissiveColor,
				Vector3 materialAmbientColor,
				Vector3 materialDiffuseColor,
				Vector3 materialSpecularColor,
				float materialShininess);
	void load(ShaderProgram* program);
};
