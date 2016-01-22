#version 330 core

in vec2 ex_TexCoord;

out vec4 outColor;

uniform float LineWidth;
uniform vec2 WindowResolution;
uniform sampler2D ColorTexture;
uniform sampler2D NormalTexture;

vec4 toonify(in vec4 color) {

	float intensity = max(color.r, max(color.g, color.b));
	float change;

	if (intensity > 0.8)
		change = 1.0;
	else if (intensity > 0.6)
		change = 0.8;
	else if (intensity > 0.4)
		change = 0.6;
	else if (intensity > 0.2)
		change = 0.4;
	else
		change = 0.2;

	
	vec4 outVec = vec4(color.rgb * change, color.a);
	return outVec;
}

void main()
{
	outColor = vec4(1.0, 1.0, 1.0, 1.0);
	
	float dx = LineWidth / WindowResolution.x;
	float dy = LineWidth / WindowResolution.y;

	vec3 center = texture(NormalTexture, ex_TexCoord).xyz;

	// sampling just these 3 neighboring fragments keeps the outline thin.
	vec3 top = texture(NormalTexture, vec2(ex_TexCoord.x, ex_TexCoord.y + dy)).xyz;
	vec3 right = texture(NormalTexture, vec2(ex_TexCoord.x + dx, ex_TexCoord.y)).xyz;
	vec3 topRight = texture(NormalTexture, vec2(ex_TexCoord.x + dx, ex_TexCoord.y + dy)).xyz;

	vec3 t = center - top;
	vec3 r = center - right;
	vec3 tr = center - topRight;

	t = abs(t);
	r = abs(r);
	tr = abs(tr);

	//Do not evaluate the corners 
	if (ex_TexCoord.y + dy >= 1.0f) {
		t *= 0.0f;
		tr *= 0.0f; //if top is impossible so is top right
	}
	if (ex_TexCoord.x + dx >= 1.0f) {
		r *= 0.0f;
		tr *= 0.0f; //if right is impossible so is top right
	}

	float n;
	n = max(n, length(t));
	n = max(n, length(r));
	n = max(n, length(tr));

	n = 1.0 - clamp(n*2.0, 0.0, 1.0);

	outColor = texture(ColorTexture, ex_TexCoord);
	outColor = toonify(outColor);
	outColor.rgb *= n;

	if (ex_TexCoord.x >= 0.48 && ex_TexCoord.x<=0.52 && ex_TexCoord.y >= 0.498 && ex_TexCoord.y <= 0.502) {
		outColor = vec4(1, 0, 0, 1);
	}

	if (ex_TexCoord.y >= 0.46 && ex_TexCoord.y <= 0.54 && ex_TexCoord.x >= 0.499 && ex_TexCoord.x <= 0.501) {
		outColor = vec4(1, 0, 0, 1);
	}

}