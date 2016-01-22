#version 330 core

in vec3 ex_Normal;
in vec3 ex_Position;
in vec2 ex_TexCoord;

in mat3 ex_camaraToTangentMatrix;

layout(location = 0) out vec3 outColor;
layout(location = 1) out vec3 outNormal;

//Needed to recalculate the light position
uniform mat4 View;

//Light properties 
const int numberOfLights = 8;
uniform vec3 LightPosition[numberOfLights];
uniform vec3 LightAmbientColor[numberOfLights];
uniform vec3 LightDiffuseColor[numberOfLights];
uniform vec3 LightSpecularColor[numberOfLights];
uniform float LightRange[numberOfLights];
uniform vec3 LightAttenuation[numberOfLights];

//Material properties 
uniform vec3 MaterialEmissiveColor;
uniform vec3 MaterialAmbientColor;
uniform vec3 MaterialDiffuseColor;
uniform vec3 MaterialSpecularColor;
uniform float MaterialShininess;

uniform sampler2D tex;

uniform int hasNormalMapping;
uniform sampler2D normalTex;

// returns intensity of emissive lighting
vec3 emissiveLighting()
{
	return MaterialEmissiveColor;
}

// returns intensity of ambient lighting
vec3 ambientLighting(in int index)
{
	return LightAmbientColor[index] * MaterialAmbientColor;
}

// returns intensity of diffuse reflection
vec3 diffuseLighting(in int index, in vec3 N, in vec3 L)
{
	float diffuseTerm = clamp(dot(N, L), 0, 1);
	return LightDiffuseColor[index] * MaterialDiffuseColor * diffuseTerm;
}

// returns intensity of specular reflection
vec3 specularLighting(in int index, in vec3 N, in vec3 L, in vec3 V)
{
	float specularTerm = 0;

	// calculate specular reflection only if
	// the surface is oriented to the light source
	if (dot(N, L) > 0)
	{
		//calculating the Blinn half vector
		vec3 H = normalize(L + V);
		specularTerm = pow(max(dot(N, H), 0.0), MaterialShininess * 4.0);
	}
	return LightSpecularColor[index] * MaterialSpecularColor * specularTerm;
}


//A standard phong lighting model
void main()
{

	//calculating values in view space
	vec3 P = ex_Position;
	vec3 N = normalize(ex_Normal);
	vec3 V = normalize(-P);

	//if normal mapping convert all to tangent space
	if (hasNormalMapping != 0) {
		P = ex_camaraToTangentMatrix * P;
		N = normalize(texture(normalTex, ex_TexCoord).rgb*2.0 - 1.0);
		V = normalize(ex_camaraToTangentMatrix * (-P));
	}

	vec3 emissive = emissiveLighting();

	vec3 totalLight = vec3(0f, 0f, 0f);
	//Calculating the light for the multiple light sources
	for (int i = 0; i < numberOfLights; i++) {
		//Calculating light position in view space
		vec3 Lpos = vec3(View * vec4(LightPosition[i], 1.0));

		//Converting from view space to tangent space
		if (hasNormalMapping != 0) {
			Lpos = ex_camaraToTangentMatrix * Lpos;
		}

		vec3 L = Lpos - P;
		//Getting light distance
		float Ldistance = length(L);
		L = normalize(L);

		if (Ldistance < LightRange[i]) {
			//Getting the reflection components
			vec3 ambient = ambientLighting(i);
			vec3 diffuse = diffuseLighting(i, N, L);
			vec3 specular = specularLighting(i, N, L, V);

			float attenuation = 1.0 / (
				LightAttenuation[i].x +
				LightAttenuation[i].y * Ldistance +
				LightAttenuation[i].z * pow(Ldistance, 2.0));

			totalLight += ambient + (diffuse + specular)*attenuation;
		}
	}

	outColor = texture(tex, ex_TexCoord).rgb *totalLight + emissive;

	//necessary for the outline shader
	outNormal = (N + 1.0f) / 2.0f;

}