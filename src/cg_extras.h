/*CG_EXTRAS
Some functions of a collection of functions to help speed up programming in OF

Developed for the Computer Graphics course @:
Informatics Engineering Department
University of Coimbra
by
Andre Perrotta & Evgheni Polisciuc
Coimbra, 09/2023
*/

#ifndef CG_EXTRAS_H
#define CG_EXTRAS_H

#include "ofMain.h"

//short named function to get screen width
inline float gw() {
	return ofGetWidth();
}

//short named function to get screen height
inline float gh() {
	return ofGetHeight();
}

//prints ModelViewMatrix
inline void coutModelviewMatrix() {

	GLfloat Matriz[4][4];
	cout << endl << "Modelview Matrix" << endl;
	glGetFloatv(GL_MODELVIEW_MATRIX, &Matriz[0][0]);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cout << Matriz[j][i] << " ";
		}
		cout << endl;
	}
	//return;
}

inline ofVec3f cross(ofVec3f A, ofVec3f B) {
	ofVec3f aux;
	aux.x = A.y * B.z - A.z * B.y;
	aux.y = A.z * B.x - A.x * B.z;
	aux.z = A.x * B.y - A.y * B.x;

	return aux;
}

inline GLfloat getMultiNoise(float x, float y, float f[], float a[], int numBands) {
	float noise = 0;
	float maxAmp = 0.;

	for (int i = 0; i < numBands; i++) {
		noise += a[i] * ofNoise(f[i] * x, f[i] * y);
		maxAmp += a[i];
	}
	return noise / maxAmp;
}

#endif