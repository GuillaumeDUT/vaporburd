#ifndef RAYTRACER_H
#define RAYTRACER_H
#include "geometry.h"
#include "shape.h"


typedef struct Ray{
	Point3D origine;
	Vec3 direction;
}Ray;

typedef struct Intersection{
	Point3D position;
	Color3f couleur;
}Intersection;

Ray createRay(Point3D origine,Vec3 direction);

int intersectsSphere(Ray r, Sphere s, Intersection* intersect);

int intersectsCube(Ray r,Cube s, Intersection* intersect);

#endif
