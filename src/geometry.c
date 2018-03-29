#include <stdio.h>
#include <math.h>
#include "geometry.h"

Point3D pointXYZ(float x, float y, float z){
		Point3D temp;
		temp.x = x;
		temp.y = y;
		temp.z = z;

		return temp;
}

Vector3D vectorXYZ(float x, float y, float z){
		Vector3D temp;
		temp.x = x;
		temp.y = y;
		temp.z = z;

	return temp;
}

Vector3D vector(Point3D A, Point3D B){
	Vector3D temp;
	temp.x = B.x - A.x;
	temp.y = B.y - A.y;
	temp.z = B.z - A.z;

	return temp;
}

Point3D pointPlusVector(Point3D P, Vector3D V){
	Point3D temp;
	temp.x = P.x + V.x;
	temp.y = P.y + V.y;
	temp.z = P.z + V.z;

	return temp;
}

Vector3D addVectors(Vector3D A, Vector3D B){
		Vector3D temp;
		temp.x = A.x + B.x;
		temp.y = A.y + B.y;
		temp.z = A.z + B.z;

		return temp;
}
Vector3D subVectors(Vector3D A, Vector3D B){
	Vector3D temp;

	temp.x = A.x - B.x;
	temp.y = A.y - B.y;
	temp.z = A.z - B.z;

	return temp;

}
//→ fonctions d’addition et soustraction de vecteurs
Vector3D multVector(Vector3D V, float a){
	Vector3D temp;

	temp.x = V.x *a;
	temp.y = V.y *a;
	temp.z = V.z *a;

	return temp;
}
Vector3D divVector(Vector3D V,float a){
	Vector3D temp;

	temp.x = V.x /a;
	temp.y = V.y /a;
	temp.z = V.z /a;

	return temp;
}
//→ fonctions de multiplication et division d’un vecteur par un scalaire
float dot(Vector3D A, Vector3D B){

	return  A.x*B.x + A.y*B.y + A.z*B.z;
}
//→ Fonction calculant le produit scalaire de deux vecteurs
float norm(Vector3D A){
	float norme = sqrt(pow(A.x,2)+pow(A.y,2)+pow(A.z,2));
	return norme;
}
//→ fonction calculant la norme d’un vecteur
Vector3D normalize(Vector3D A){
	Vector3D normalisationVecteur;

	normalisationVecteur.x = A.x / norm(A);
	normalisationVecteur.y = A.y / norm(A);
	normalisationVecteur.z = A.z / norm(A);

	return normalisationVecteur;
}


void printVector3D(Vector3D v){
	printf("Vecteur X : %f \n",v.x);
	printf("Vecteur Y : %f \n",v.y);
	printf("Vecteur Z : %f \n",v.z);
}

void printPoint3D(Point3D p){
	printf("Point X : %f \n",p.x);
	printf("Point Y : %f \n",p.y);
	printf("Point Z : %f \n",p.z);
}
