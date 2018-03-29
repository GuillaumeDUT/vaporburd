#include <stdlib.h>
#include <stdio.h>
#include "geometry.h"
#include "raytracer.h"


Ray createRay(Point3D origine,Vec3 direction){
	Ray temp;
	temp.origine = origine;
	temp.direction = direction;
	return temp;
}


int intersectsCube(Ray r, Sphere s, Intersection* intersect){
	float a = powl( norm( normalize(r.direction) ), 2 );
	float b = 2*norm( vectorXYZ(r.origine,r.direction) );
	float c = powl( norm( r.origine ) ,2) - powl(s.rayon,2);

	float delta = powl(b,2) - 4*a*c;

	if( delta >0){
		t1 = (-b + sqrt(delta) ) / 2*;
		t2 = (-b - sqrt(delta) ) / 2*;

		if( t1 > && t2 > 0 ){
			if(t1 > t2 ){
				intersect.position = pointPlusVector()
			}else{

			}
		}

	}else if{

	}else{
		return 0;
	}

}
