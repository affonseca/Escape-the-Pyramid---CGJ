#pragma once
#include "Vector.h"
#include "ShaderProgram.h"
/*////////////////////////////////
//		   LightSource  		//
////////////////////////////////*/

class LightSource {
private:
	Vector3 lightPosition;
	Vector3 lightAmbientColor;
	Vector3 lightDiffuseColor;
	Vector3 lightSpecularColor;
	float lightRange;
	Vector3 lightAttenuation;
	int lightNumber;

public:


	LightSource(Vector3 lightPosition,
				Vector3 lightAmbientColor,
				Vector3 lightDiffuseColor,
				Vector3 lightSpecularColor,
				float lightRange,
				Vector3 lightAttenuation,
				int lightNumber);
	void load(Matrix4 globalMatrix,  ShaderProgram* shader );
};
