#version 330 core

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TextureCoord;
in vec3 in_Tangent;
in vec3 in_Bitangent;

out vec3 ex_Normal;
out vec3 ex_Position;

out mat3 ex_camaraToTangentMatrix;

out vec2 ex_TexCoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Normal;

uniform int hasNormalMapping;

void main(void) {
	vec4 eyePosition = View * Model * vec4(in_Position, 1.0);

	//Calculating normal, light and position
	ex_Normal = (Normal * vec4(in_Normal, 1.0)).xyz;
	ex_Position = eyePosition.xyz;


	//changing lighting vectors to tangent space if using normal mapping

	if (hasNormalMapping != 0) {

		//changing tangents and bitangents to camara space
		mat3 viewModel3x3 = mat3(View * Model);
		vec3 tangentInCamera = viewModel3x3 * normalize(in_Tangent);
		vec3 biTangentInCamera = viewModel3x3 * normalize(in_Bitangent);

		vec3 normalNoTranslate = mat3(Normal) * normalize(in_Normal);

		//getting the space transformation matrix
		ex_camaraToTangentMatrix = transpose(
			mat3(
				tangentInCamera,
				biTangentInCamera,
				normalNoTranslate
				));

	}

	ex_TexCoord = in_TextureCoord;
	ex_TexCoord.y = 1.0 - ex_TexCoord.y; //because SOIL is dumb!

	gl_Position = Projection * eyePosition;
}