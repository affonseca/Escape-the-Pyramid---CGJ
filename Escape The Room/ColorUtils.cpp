#include "ColorUtils.h"

Vector3 rgbTohsv(Vector3 rgb) {
	float value, saturation, hue;


	float max = rgb.x;
	if (rgb.y > max) {
		max = rgb.y;
	}
	if (rgb.z > max) {
		max = rgb.z;
	}

	float min = rgb.x;
	if (rgb.y < min) {
		min = rgb.y;
	}
	if (rgb.z < min) {
		min = rgb.z;
	}

	value = max / 255;
	if (max > 0) {
		saturation = 1 - min / max;
	}
	else {
		saturation = 0;
	}

	float squaredValue = sqrt(rgb.x*rgb.x + rgb.y*rgb.y + rgb.z*rgb.z - rgb.x*rgb.y - rgb.x*rgb.z - rgb.y*rgb.z);
	float otherValue = rgb.x - rgb.y / 2 - rgb.z / 2;

	hue = acosf(otherValue / squaredValue) * 57.29578f; // radian to degree
	if (rgb.z > rgb.y) {
		hue = 360 - hue;
	}

	return Vector3(hue, saturation, value);
}


Vector3 hsvTorgb(Vector3 hsv) {
	float red, green, blue;

	float max = 255 * hsv.z;
	float min = max*(1 - hsv.y);

	float z = (max - min)*(1 - fabsf(fmodf(hsv.x / 60, 2) - 1));


	if (hsv.x >= 0 && hsv.x < 60) {
		red = max;
		green = z + min;
		blue = min;
	}
	else if (hsv.x >= 60 && hsv.x < 120) {
		red = z + min;
		green = max;
		blue = min;
	}
	else if (hsv.x >= 120 && hsv.x < 180) {
		red = min;
		green = max;
		blue = z + min;
	}
	else if (hsv.x >= 180 && hsv.x < 240) {
		red = min;
		green = z + min;
		blue = max;
	}
	else if (hsv.x >= 240 && hsv.x < 300) {
		red = z + min;
		green = min;
		blue = max;
	}
	else { //if (hsv.x >= 300 && hsv.x < 360)
		red = max;
		green = min;
		blue = z + min;
	}

	return Vector3(red, green, blue);

}