#include <stdio.h>
#include "colors.h"

Color3f addColors(Color3f c1, Color3f c2){
	Color3f temp;
	temp.r = c1.r + c2.r;
	temp.g = c1.g + c2.g;
	temp.b = c1.b + c2.b;

	return temp;
}

Color3f subColors(Color3f c1, Color3f c2){
	Color3f temp;
	temp.r = c1.r - c2.r;
	temp.g = c1.g - c2.g;
	temp.b = c1.b - c2.b;

	return temp;
}

Color3f multColors(Color3f c1, Color3f c2){
	Color3f temp;
	temp.r = c1.r * c2.r;
	temp.g = c1.g * c2.g;
	temp.b = c1.b * c2.b;

	return temp;
}
Color3f divColors(Color3f c1, Color3f c2){
	Color3f temp;
	temp.r = c1.r / c2.r;
	temp.g = c1.g / c2.g;
	temp.b = c1.b / c2.b;

	return temp;
}

Color3f multColor(Color3f c, float a){
	Color3f temp;
	temp.r = c.r *a;
	temp.g = c.g *a;
	temp.b = c.b *a;

	return temp;
}

Color3f divColor(Color3f c, float a){
	Color3f temp;
	temp.r = c.r /a;
	temp.g = c.g /a;
	temp.b = c.b /a;

	return temp;
}
