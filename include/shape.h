#ifndef SHAPE_H
#define SHAPE_H
#include "geometry.h"
#include "colors.h"


typedef struct Sphere{
	Point3D centre;
	float rayon;
	Color3f couleur;
}Sphere;

typedef struct Cube{
	Point3D minimum;
	Point3D maximum;
	Color3f couleur;
}Cube;

Cube createCube(Point3D minimum,Point3D maximum,Color3f couleur);

Sphere createSphere(Point3D centre, float rayon, Color3f couleur);

#endif
