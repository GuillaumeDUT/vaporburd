#include <stdlib.h>
#include <stdio.h>
#include "geometry.h"
#include "shape.h"

Cube createCube(Point3D minimum,Point3D maximum,Color3f couleur){
	Cube temp;
	temp.minimum = minimum;
	temp.maximum = maximum;
	temp.couleur = couleur;
	return temp;
}

Sphere createSphere(Point3D centre, float rayon, Color3f couleur){
	Sphere temp;
	temp.centre = centre;
	temp.rayon = rayon;
	temp.couleur = couleur;
	return temp;
}
