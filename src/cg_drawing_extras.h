#pragma once

#include "ofMain.h"
#include "cg_extras.h"

//Function for unitary mesh: floor, river
inline void malha_unit(GLint m, GLint n) {
	GLfloat x_start = -0.5;
	GLfloat y_start = -0.5;
	GLfloat x_step = 1.0 / GLfloat(m);
	GLfloat y_step = 1.0 / GLfloat(n);

	glBegin(GL_QUADS);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			glVertex2d(i * x_step + x_start, j * y_step + y_start);
			glVertex2d(i * x_step + x_start, (j + 1) * y_step + y_start);
			glVertex2d((i + 1) * x_step + x_start, (j + 1) * y_step + y_start);
			glVertex2d((i + 1) * x_step + x_start, j * y_step + y_start);
		}
	}
	glEnd();
}

//Function for unitary cube: train
inline void cube_unit() {
	GLfloat p = 0.5;
	glBegin(GL_QUADS);

	//front
	glVertex3f(-p, -p, p);
	glVertex3f(-p, p, p);
	glVertex3f(p, p, p);
	glVertex3f(p, -p, p);

	//back
	glVertex3f(-p, -p, -p);
	glVertex3f(p, -p, -p);
	glVertex3f(p, p, -p);
	glVertex3f(-p, p, -p);

	//up
	glVertex3f(-p, -p, -p);
	glVertex3f(-p, -p, p);
	glVertex3f(p, -p, p);
	glVertex3f(p, -p, -p);

	//down
	glVertex3f(-p, p, p);
	glVertex3f(-p, p, -p);
	glVertex3f(p, p, -p);
	glVertex3f(p, p, p);

	//left
	glVertex3f(-p, -p, p);
	glVertex3f(-p, -p, -p);
	glVertex3f(-p, p, -p);
	glVertex3f(-p, p, p);

	//right
	glVertex3f(p, -p, p);
	glVertex3f(p, p, p);
	glVertex3f(p, p, -p);
	glVertex3f(p, -p, -p);

	glEnd();

}

//Cube with unitary mesh: a lot of things
inline void cube_malha_unit(GLint m, GLint n) {

	//front
	glPushMatrix();
	glTranslated(0., 0., 0.5);
	malha_unit(m, n);
	glPopMatrix();

	//back
	glPushMatrix();
	glTranslated(0., 0., -0.5);
	glRotated(180, 0, 1, 0);
	malha_unit(m, n);
	glPopMatrix();

	//up
	glPushMatrix();
	glTranslated(0., -0.5, 0.);
	glRotated(90, 1, 0, 0);
	malha_unit(m, n);
	glPopMatrix();

	//down
	glPushMatrix();
	glTranslated(0., 0.5, 0.);
	glRotated(-90, 1, 0, 0);
	malha_unit(m, n);
	glPopMatrix();

	//left
	glPushMatrix();
	glTranslated(-0.5, 0., 0.);
	glRotated(-90, 0, 1, 0);
	malha_unit(m, n);
	glPopMatrix();

	//right
	glPushMatrix();
	glTranslated(0.5, 0., 0.);
	glRotated(90, 0, 1, 0);
	malha_unit(m, n);
	glPopMatrix();

}

//Circle 2D: carroussel base
inline void circle(GLfloat x, GLfloat y, GLint z, GLfloat r, GLint n) {
	double angle = 2 * PI / n; //angle of each segment
	glBegin(GL_POLYGON);
	for (int i = 0; i < n; i++) {
		glVertex3d(x + r * cos(angle * i), y + r * sin(angle * i), z);
	}
	glEnd();
}

//Cone 3D: trees and carroussel top
inline void cone(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLint n, GLint h) {
	double angle = 2 * PI / n;
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(x, y, z + h);
	for (int i = 0; i < n + 1; i++) {
		glVertex3d(x + r * cos(angle * i), y + r * sin(angle * i), z);
	}
	glEnd();
}

//Cilinder 3D: carroussel pillars
inline void cylinder(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLint n, GLint h) {
	double angle = 2 * PI / n;
	glBegin(GL_QUAD_STRIP);

	for (int i = 0; i <= n; i++) {
		double angle1 = angle * i; //angule calculation
		double angle2 = angle * (i + 1);

		GLfloat x1 = x + r * cos(angle1); //position calculation
		GLfloat y1 = y + r * sin(angle1);
		GLfloat x2 = x + r * cos(angle2);
		GLfloat y2 = y + r * sin(angle2);

		for (int j = 0; j <= z; j++) {
			GLfloat z1 = h * (j / z); //height calculation
			GLfloat z2 = h * ((j + 1) / z);

			glVertex3d(x1, y1, z1); //top base vertice
			glVertex3d(x2, y2, z2); //down base vertice
		}
	}
	glEnd();
}

inline void sphereSkyBox(int res) {
	float dtheta = PI / float(res);
	float dphi = 2 * PI / float(res);

	for (int i = 0; i < res; i++) {
		for (int j = 0; j < res; j++) {
			glBegin(GL_QUADS);

			float x1 = sin(i * dtheta) * cos(j * dphi);
			float y1 = sin(i * dtheta) * sin(j * dphi);
			float z1 = cos(i * dtheta);
			float tx1 = i / float(res);
			float ty1 = j / float(res);
			ofVec3f n1 = ofVec3f(-x1, -y1, -z1);
			glTexCoord2f(ty1, tx1);
			glNormal3f(n1.x, n1.y, n1.z);
			glVertex3f(x1, y1, z1);

			float x2 = sin(i * dtheta) * cos((j + 1) * dphi);
			float y2 = sin(i * dtheta) * sin((j + 1) * dphi);
			float z2 = cos(i * dtheta);
			float tx2 = i / float(res);
			float ty2 = (j + 1) / float(res);
			ofVec3f n2 = ofVec3f(-x2, -y2, -z2);
			glTexCoord2f(ty2, tx2);
			glNormal3f(n2.x, n2.y, n2.z);
			glVertex3f(x2, y2, z2);

			float x3 = sin((i + 1) * dtheta) * cos((j + 1) * dphi);
			float y3 = sin((i + 1) * dtheta) * sin((j + 1) * dphi);
			float z3 = cos((i + 1) * dtheta);
			float tx3 = (i + 1) / float(res);
			float ty3 = (j + 1) / float(res);
			ofVec3f n3 = ofVec3f(-x3, -y3, -z3);
			glTexCoord2f(ty3, tx3);
			glNormal3f(n3.x, n3.y, n3.z);
			glVertex3f(x3, y3, z3);

			float x4 = sin((i + 1) * dtheta) * cos(j * dphi);
			float y4 = sin((i + 1) * dtheta) * sin(j * dphi);
			float z4 = cos((i + 1) * dtheta);
			float tx4 = (i + 1) / float(res);
			float ty4 = j / float(res);
			ofVec3f n4 = ofVec3f(-x4, -y4, -z4);
			glTexCoord2f(ty4, tx4);
			glNormal3f(n4.x, n4.y, n4.z);
			glVertex3f(x4, y4, z4);

			glEnd();
		}
	}
}
