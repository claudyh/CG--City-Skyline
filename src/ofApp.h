#pragma once

#include "ofMain.h"
#include "cg_extras.h"
#include "cg_drawing_extras.h"
#include "cg_cam_extras.h"
#include "materiais.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	float rx, ry, rz;
	float colors[36][3];
	int heights[36][3];
	float trainPosX, trainPosY, trainPosX_2, trainPosY_2;
	float leftLimit, rightLimit, topLimit, bottomLimit;
	int trackX, trackY, trackX_2, trackY_2;

	//floor
	GLint resX, resY;
	GLfloat floorWidth, floorHeight, floorHeightPos;
	GLfloat baseWidth, baseDepth, baseHeight;
	GLint basePosX, basePosY;
	ofVec3f basePos;

	//view
	bool psp;
	int top;
	int lensAngle, alpha, beta;

	//lights
	int dirOn;
	int pointOn;
	int spotOn;
	int lightOn;
	bool localViewer;
	bool ambientOn;

	GLfloat ambientLight[4];

	GLfloat dirVec[4];
	GLfloat dirAmb[4];
	GLfloat dirDif[4];
	GLfloat dirSpec[4];
	ofVec3f dirVec3f;
	GLfloat dirVecTheta;

	GLfloat pointPos[4];
	GLfloat pointAmb[4];
	GLfloat pointDif[4];
	GLfloat pointSpec[4];
	GLfloat pointZtheta;
	GLfloat pointAtC, pointAtL, pointAtQ;

	GLfloat spotPos[4];
	GLfloat spotDir[3];
	GLfloat spotAmb[4];
	GLfloat spotDif[4];
	GLfloat spotSpecular[4];
	GLfloat spotExponent;
	GLfloat spotCutoff;
	GLfloat spotTheta;
	GLfloat spotAtC, spotAtL, spotAtQ;

	GLfloat lightPos[4];
	GLfloat lightDir[3];
	GLfloat lightAmb[4];
	GLfloat lightDif[4];
	GLfloat lightSpecular[4];
	GLfloat lightExponent;
	GLfloat lightCutoff;
	GLfloat lightTheta;
	GLfloat lightAtC, lightAtL, lightAtQ;
	
	//materiais
	int mat;
	bool customMat;
	GLint customMatCoef;
	//sphere
	bool drawSphere;
	GLfloat mview[4][4];
	//print data
	bool printData;
	void printAllData();
	GLfloat customMatAmb[4];
	GLfloat customMatDif[4];
	GLfloat customMatSpec[4];

	GLfloat light_high;

	//texturas
	ofImage img0, img1, img2, img3;
	float w0, h0;

	void river();
	void directionalLight();
	void ponctualLight();
	void focusLight();
	void choiceLights(GLfloat lightPos0, GLfloat lightPos1, GLfloat lightPos2, GLenum GL_LIGHT);

	//Water
	GLfloat dirVecTheta1;

	float phaseX;
	float phaseY;
	float freqs[10];
	float amps[10];
	vector<ofVec3f> vertexes;
	vector<ofVec3f> normals;
	vector<ofVec2f> texCoords;

	//skybox
	void skybox();
	float phaseX1;
	float phaseY1;
	float rx1, ry1, rz1;
	ofImage sky;
	ofFbo nightSky;
	ofImage nightSkyImg;
	unsigned char* skyPixels = NULL;
	int px, py, numPixels;

	//texturas
	ofImage bump;
	unsigned char* pixels;
	unsigned char* normalPixels = NULL;
	bool originalOrBump;
	float bumpScale;
	bool orto;
	bool wireframe;
	int view;
	int sketch;
};