#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	//Depth Testing
	glEnable(GL_DEPTH_TEST);
	coutModelviewMatrix();
	//Background color
	ofBackground(0.05, 0.05, 0.05);
	//Line width
	glLineWidth(5);
	
	//Frames
	ofSetFrameRate(60);
	ofSetVerticalSync(false); //synchronizing buffer swap with monitor's refresh rate

	//Texture
	ofDisableArbTex();//to utilize normalized texture coordinates
	img0.load("snow.png");
	img1.load("building.png");
	img2.load("water.gif");
	img3.load("stars.png");

	for (int i = 0; i < 10; i++) {
		freqs[i] = 0.25 + 0.25 * i * 5;
		amps[i] = 1. / (i * 0.25 + 1);
		cout << endl << amps[i];
	}

	phaseX = 0.;
	phaseY = 0.;

	//View
	psp = false; //perspective
	top = 0; //top view
	lensAngle = 80; //angles
	alpha = 120;
	beta = 120;

	//Colors - not needed anymore
	for (int i = 0; i < 35; i++) {
		colors[i][0] = rand() / (float)RAND_MAX; //red
		colors[i][1] = rand() / (float)RAND_MAX; //green
		colors[i][2] = rand() / (float)RAND_MAX; //blue
	}

	//Heights - random building heights
	for (int i = 0; i < 35; i++) {
		heights[i][0] = 5.0 + (6.0 * rand() / (float)RAND_MAX); //smaller height
		heights[i][1] = 10.0 + (10.0 * rand() / (float)RAND_MAX); //mediun height
		heights[i][2] = 20.0 + (10.0 * rand() / (float)RAND_MAX); //bigger height
	}

	//Floor parameters
	resX = 20;
	resY = 20;
	floorWidth = gw() * 0.5;
	floorHeight = gw() * 0.5;
	floorHeightPos = 0.;//gh() * 0.75;

	baseWidth = floorWidth / GLfloat(resX);
	baseHeight = floorHeight / GLfloat(resY);
	baseDepth = baseWidth * 0.2;

	basePosX = floor(resX / 2);
	basePosY = floor(resY / 2);
	basePos.z = floorHeightPos + baseDepth * 0.5;

	//Rotate
	rx = ry = rz = 0.;

	//Translate
	trainPosX = basePos.x;
	trainPosY = basePos.y - baseWidth/2 - baseWidth * 9;
	trainPosX_2 = basePos.x + baseWidth/2;
	trainPosY_2 = basePos.y - baseWidth / 2 - baseWidth * 9;
	
	leftLimit= basePos.x - floorWidth/2 + baseWidth/2;
	rightLimit = basePos.x + floorWidth/2 - baseWidth/2;
	topLimit = basePos.y - 9*baseHeight - baseHeight/2;
	bottomLimit = basePos.y + 2*baseHeight + baseHeight/2;

	trackX = 0;
	trackY = 0;
	trackX_2 = 0;
	trackY_2 = 0;

	//Light
	dirOn = 0;
	pointOn = 0;
	spotOn = 0;
	lightOn = 0;
	ambientOn = true;

	pointZtheta = 0;
	dirVecTheta = 0.;
	dirVecTheta1 = 0.;

	light_high = 0; //still not working

}

//--------------------------------------------------------------
void ofApp::update() {
	ofSetWindowTitle(ofToString(ofGetFrameRate()));

	//Base position
	basePos.x = -floorWidth * 0.5 + (basePosX * floorWidth / GLfloat(resX)) + baseWidth * 0.5;
	basePos.y = -floorHeight * 0.5 + (basePosY * floorHeight / GLfloat(resY)) + baseHeight * 0.5;
	basePos.z = floorHeightPos + baseDepth * 0.5;

	//Directional light position (always moving)
	float z = gh() * 0.5;
	float y = 0;
	float x = gh() * 0.25 * cos(dirVecTheta * PI / 180.);
	dirVec3f = ofVec3f(x, y, z) - ofVec3f(0, 0, 0);
	dirVecTheta += 0.1;

	//Ponctual light (carrossel)
	pointPos[0] = basePos.x + (baseWidth / 2) + 6 * baseWidth;
	pointPos[1] = basePos.y + (baseHeight / 2) + 6 * baseHeight;
	pointPos[2] = baseDepth * 4 + baseDepth * 2 + 2.5;

	//Focus light (train)
	spotDir[0] = 0;
	spotDir[1] = 0;
	
}

//--------------------------------------------------------------
void ofApp::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	if (!psp) {
		//Ortogonal
		glViewport(0, 0, gw(), gh());
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//ortho(left, right, bottom, top, near, far) 
		glOrtho(gw() * 0.5, -gw() * 0.5, -gh() * 0.5, gh() * 0.5, 0, 2000);
	}
	else {
		//Perspective
		glViewport(0, 0, gw(), gh());
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//perspective(vision field, proportion (between width and height), clipping plane)
		perspective(lensAngle, alpha, beta);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix(); //master push ------------------------------
	glLineWidth(1.0);

	//Views
	switch (top) {
	case 0:
		//top view
		lookat(0, 0, 700, 0, 0, 0, 0, -1, 0);
		break;
	case 1:
		//front view
		lookat(0, 700, 0, 0, 0, 0, 0, 0, 1);
		break;
	case 2:
		//personalised view
		lookat(gw() * 0.5, gh() * 0.75, 500, 0, 0, 0, 0, 0, 1);
		break;
	}

	//Floor ------------------------

	//Floor material: White plastic
	loadMaterial(16);

	glPushMatrix();//floor push
	glColor3f(0.7, 0.7, 0.7);

	//Floor texture
	glEnable(GL_TEXTURE);
	img0.bind();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //Modulate!
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); //texture coordinate
	glVertex3f(-floorWidth/2, floorHeight/2, 0.5); //vertex(x, y, z)

	glTexCoord2f(0, 1); //texture coordinate
	glVertex3f(-floorWidth/2, -floorHeight/2, 0.5); //vertex(x, y, z)

	glTexCoord2f(1, 1); //texture coordinate
	glVertex3f(floorWidth/2, -floorHeight/2, 0.5); //vertex(x, y, z)

	glTexCoord2f(1, 0); //texture coordinate
	glVertex3f(floorWidth/2, floorHeight/2, 0.5); //vertex(x, y, z)
	glEnd();

	img0.unbind();
	glDisable(GL_TEXTURE);

	glScalef(floorWidth, floorHeight, 0.5);
	malha_unit(resX, resY);
	glPopMatrix();//floor pop


	//River ------------------------
	
	//River material: Cyan plastic
	loadMaterial(5);
	
	//river();
	
	glPushMatrix();//river push
	glColor3f(0.2, 0.4, 1);

	//River texture
	glEnable(GL_TEXTURE);
	img2.bind();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBegin(GL_QUADS);

	glTexCoord2f(0, 0); //texture coordinate
	glVertex3f(basePos.x - 9*baseWidth - baseWidth/2, basePos.y + 10*baseHeight - baseHeight/2, 3); //vertex(x, y, z)

	glTexCoord2f(0, 1); //texture coordinate
	glVertex3f(basePos.x - 6*baseWidth - baseWidth/2, basePos.y + 10*baseHeight - baseHeight/2, 3); //vertex(x, y, z)

	glTexCoord2f(1, 1); //texture coordinate
	glVertex3f(basePos.x - 6*baseWidth - baseWidth/2, basePos.y - 10*baseHeight - baseHeight/2, 3); //vertex(x, y, z)

	glTexCoord2f(1, 0); //texture coordinate
	glVertex3f(basePos.x - 9*baseWidth - baseWidth/2, basePos.y - 10*baseHeight - baseHeight/2, 3); //vertex(x, y, z)

	glEnd();

	img2.unbind();
	glDisable(GL_TEXTURE);

	glTranslatef(basePos.x - 8*baseWidth, basePos.y - 0.5*baseHeight, 2);
	glScalef(3 * (baseWidth), floorHeight, 1.);
	malha_unit(resX, resY);

	glPopMatrix();//river pop
	
	//Train track ------------------------

	//Train track material: Bronze
	loadMaterial(7);

	glPushMatrix();//train track master push
	glColor3f(0.4, 0.2, 0);

	glPushMatrix();//back track push
	glTranslatef(basePos.x - (floorWidth/2), basePos.y - (floorHeight/2), 0);
	glTranslatef(baseWidth * 11 + (baseWidth/2), 0, baseDepth * 3 + 1);
	glScalef(baseWidth * 16, baseHeight, baseDepth * 6);
	cube_malha_unit(baseWidth, baseDepth);
	glPopMatrix();//back track pop

	glPushMatrix();//back bridge push
	glTranslatef(basePos.x - (floorWidth/2), basePos.y - (floorHeight/2), 0);
	glTranslatef(baseWidth * 1.5 + (baseWidth / 2), 0, baseDepth * 5 + 1);
	glScalef(baseWidth * 3, baseHeight, baseDepth * 2);
	cube_malha_unit(baseWidth, baseDepth);
	glPopMatrix();//back bridge pop

	glPushMatrix();//front track push
	glTranslatef(basePos.x - (floorWidth / 2), basePos.y + 2*baseHeight, 0);
	glTranslatef(baseWidth * 11 + (baseWidth / 2), 0, baseDepth * 3 + 1);
	glScalef(baseWidth * 16, baseHeight, baseDepth * 6);
	cube_malha_unit(baseWidth, baseDepth);
	glPopMatrix();//front track pop

	glPushMatrix();//front bridge push
	glTranslatef(basePos.x - (floorWidth / 2), basePos.y + 2*baseHeight, 0);
	glTranslatef(baseWidth * 1.5 + (baseWidth / 2), 0, baseDepth * 5 + 1);
	glScalef(baseWidth * 3, baseHeight, baseDepth * 2);
	cube_malha_unit(baseWidth, baseDepth);
	glPopMatrix();//front bridge pop
	
	glPushMatrix();//left track push
	glTranslatef(basePos.x - (floorWidth/2), basePos.y - (floorHeight/2 + 4*baseHeight), 0);
	glTranslatef(0, baseHeight * 10, baseDepth * 3 + 1);
	glScalef(baseWidth, baseHeight * 13, baseDepth * 6);
	cube_malha_unit(baseDepth, baseHeight);
	glPopMatrix();//left track pop

	glPushMatrix();//right track push
	glTranslatef(basePos.x + (floorWidth/2 - baseWidth), basePos.y - (floorHeight/2 + 2*baseHeight), 0);
	glTranslatef(0, baseHeight * 8, baseDepth * 3 + 1);
	glScalef(baseWidth, baseHeight * 11, baseDepth * 6);
	cube_malha_unit(baseDepth, baseHeight);
	glPopMatrix();//right track pop

	glPopMatrix();//train track pop


	//Train ------------------------

	//Train material: Ruby
	loadMaterial(4);

	glPushMatrix();//train master push
	glColor3f(1, 0, 0);

	glPushMatrix();//carriage 1 push

	//Movement
	if (trackX == 0 && trainPosX > leftLimit) {
		trainPosX -= 0.3;
		//luz foco direcao: esquerda
		spotDir[0] = -1.;
		spotDir[1] = 0.;
		spotDir[2] = 0;
	}
	else {
		trackX = 1;

		if (trackY == 0 && trainPosY < bottomLimit) {
			trainPosY += 0.3;
			//luz foco direcao: baixo
			spotDir[0] = 0.;
			spotDir[1] = 1.;
			spotDir[2] = 0;
		}
		else {
			if (trainPosX < rightLimit) {
				trainPosX += 0.3;
				//luz foco direcao: direita
				spotDir[0] = 1.;
				spotDir[1] = 0.;
				spotDir[2] = 0;
			}
			else {
				trackY = 1;

				if (trainPosY > topLimit) {
					trainPosY -= 0.3;
					//luz foco direcao: esquerda
					spotDir[0] = 0.;
					spotDir[1] = -1.;
					spotDir[2] = 0;
				}
				else {
					trackX = 0;
					trackY = 0;
				}
			}
		}
	}

	glPushMatrix();
	glTranslatef(trainPosX, trainPosY, baseDepth * 7 + 1);
	glScalef(baseHeight * 0.5, baseHeight * 0.5, baseDepth * 2.5);
	cube_unit();
	glPopMatrix();

	glPopMatrix();//carriage 1 pop

	glPushMatrix();//carriage 2 push

	//Movement
	if (trackX_2 == 0 && trainPosX_2 > leftLimit) {
		trainPosX_2 -= 0.3;
	}
	else {
		trackX_2 = 1;

		if (trackY_2 == 0 && trainPosY_2 < bottomLimit) {
			trainPosY_2 += 0.3;
		}
		else {
			if (trainPosX_2 < rightLimit) {
				trainPosX_2 += 0.3;
			}
			else {
				trackY_2 = 1;

				if (trainPosY_2 > topLimit) {
					trainPosY_2 -= 0.3;
				}
				else {
					trackX_2 = 0;
					trackY_2 = 0;
				}
			}
		}
	}

	glPushMatrix();
	glTranslatef(trainPosX_2, trainPosY_2, baseDepth * 7 + 1);
	glScalef(baseHeight * 0.5, baseHeight * 0.5, baseDepth * 2.5);
	cube_unit();
	glPopMatrix();

	glPopMatrix();//carriage 2 pop

	/*
	glPushMatrix();//iron push
	glTranslatef(basePos.x - (floorWidth/2), basePos.y - (floorHeight/2), 0);
	glTranslatef(baseWidth * (-0.5) + (baseWidth / 4), 0, baseDepth * 7 + 1);
	glScalef(baseWidth/2, baseHeight * 0.25, baseDepth * 0.5);
	for (int i = 0; i < 2; i++) {
		glTranslatef(5, 0, 0);
		glColor3f(0.5, 0.5, 0.5);
		cube_malha_unit(baseWidth, baseDepth);
	}
	glPopMatrix();//iron pop
	*/

	glPopMatrix();//train master pop


	//Buildings ------------------------

	//Buildings material: Silver
	loadMaterial(11);

	glPushMatrix();//buildings push
	float h;
	int counter = 0;

	glTranslatef(basePos.x - (floorWidth/2) + 17*(baseWidth), basePos.y - (floorHeight/2), 0);
	for (int j = 0; j < 5; j++) {
		glTranslatef((- 14 * baseWidth), 2 * (baseHeight), 0);
		for (int i = 0; i < 7; i++) {
			glColor3f(colors[counter][0], colors[counter][1], colors[counter][2]);

			if (j == 0 | j == 4 | (0<j<4 && (i == 0 | i == 6))) {
				h = heights[i][0];
			}
			else if (((j == 1 | j == 3) && 0 < i < 6) | (j == 2 && (i == 1 | i == 5))) {
				h = heights[i][1];
			}
			else {
				h = heights[i][2];
			}

			float hHalf = h / 2.0;
			glTranslatef(2 * (baseWidth), 0, 0);

			glPushMatrix();//single building push

			//Building texture
			glEnable(GL_TEXTURE);
			img1.bind();

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			//Front
			glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex3f(-baseWidth/2, baseHeight/2, baseDepth*h);
			glTexCoord2f(0, 1);
			glVertex3f(-baseWidth/2, baseHeight/2, 1);
			glTexCoord2f(1, 1);
			glVertex3f(-baseWidth/2+baseWidth, baseHeight/2, 1);
			glTexCoord2f(1, 0);
			glVertex3f(-baseWidth/2+baseWidth, baseHeight/2, baseDepth*h);
			glEnd();

			//Back
			glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex3f(-baseWidth / 2, baseHeight/2 - baseHeight, baseDepth * h);
			glTexCoord2f(0, 1);
			glVertex3f(-baseWidth / 2, baseHeight/2 - baseHeight, 1);
			glTexCoord2f(1, 1);
			glVertex3f(-baseWidth / 2 + baseWidth, baseHeight/2 - baseHeight, 1);
			glTexCoord2f(1, 0);
			glVertex3f(-baseWidth / 2 + baseWidth, baseHeight/2 - baseHeight, baseDepth * h);
			glEnd();

			//Left
			glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex3f(-baseWidth/2, baseHeight/2, baseDepth*h);
			glTexCoord2f(0, 1);
			glVertex3f(-baseWidth/2, baseHeight/2, 1);
			glTexCoord2f(1, 1);
			glVertex3f(-baseWidth/2, -baseHeight/2, 1);
			glTexCoord2f(1, 0);
			glVertex3f(-baseWidth/2, -baseHeight/2, baseDepth*h);
			glEnd();

			//Right
			glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex3f(baseWidth/2, -baseHeight/2, baseDepth * h);
			glTexCoord2f(0, 1);
			glVertex3f(baseWidth/2, -baseHeight/2, 1);
			glTexCoord2f(1, 1);
			glVertex3f(-baseWidth/2 + baseWidth, baseHeight/2, 1);
			glTexCoord2f(1, 0);
			glVertex3f(-baseWidth/2 + baseWidth, baseHeight/2, baseDepth * h);
			glEnd();

			img1.unbind();

			//Top
			if (dirOn == 4) {
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			}
			img0.bind();
			glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex3f(-baseWidth/2, -baseHeight/2, baseDepth * h + 1.5);
			glTexCoord2f(0, 1);
			glVertex3f(-baseWidth/2, baseHeight/2, baseDepth * h + 1.5);
			glTexCoord2f(1, 1);
			glVertex3f(baseWidth/2, baseHeight/2, baseDepth * h + 1.5);
			glTexCoord2f(1, 0);
			glVertex3f(baseWidth/2, -baseHeight/2, baseDepth * h + 1.5);
			glEnd();
			img0.unbind();

			glDisable(GL_TEXTURE);

			glTranslatef(0, 0, baseDepth * hHalf +1);
			glScalef(baseWidth, baseHeight, baseDepth * h);
			cube_malha_unit(3, 3);
			glPopMatrix();//single building pop
			counter++;
		}
	}

	glPopMatrix();//buildings pop


	//Xmas trees ------------------------

	//Trees material: Esmerald
	loadMaterial(0);

	glPushMatrix();//tree push
	glColor3f(0, 0.5, 0);
	
	for (int i = 0; i < 7; i++) {
		glTranslatef(2 * (baseWidth), 0, 0);
		cone(basePos.x - 7*baseWidth, basePos.y + (floorHeight / 4) - baseHeight, 1, baseWidth / 2, 20, baseDepth * 5);
	}

	glPopMatrix();//tree pop


	//Carousel ------------------------

	//Carroussel material: Ruby
	loadMaterial(4);

	glPushMatrix();//carousel push
	glColor3f(1, 0, 0);
	
	//Rotation
	glTranslatef(basePos.x + (baseWidth/2) + 6*baseWidth, basePos.y + (baseHeight/2) + 6*baseHeight, 0);
	glRotated(rz++, 0, 0, 1);
	glTranslatef(-(basePos.x + (baseWidth/2) + 6*baseWidth), -(basePos.y + (baseHeight/2) + 6*baseHeight), 0);

	//Base
	circle(basePos.x + (baseWidth/2) + 6*baseWidth, basePos.y + (baseHeight/2) + 6*baseHeight, 1, baseWidth, 20);

	//Top
	cone(basePos.x + (baseWidth/2) + 6*baseWidth, basePos.y + (baseHeight/2) + 6*baseHeight, baseDepth*4, baseWidth, 20, baseDepth*2);
	
	//Carroussel pillars material: White plastic
	loadMaterial(16);

	//Pillars
	glColor3f(1, 1, 1);
	cylinder(basePos.x + (baseWidth/2) + 6*baseWidth, basePos.y + (baseHeight/2) + 6*baseHeight, baseDepth*100, baseWidth/18, 20, baseDepth*4);

	cylinder(basePos.x + (baseWidth/2) + 5*baseWidth + 4*baseWidth/18, basePos.y + (baseHeight/2) + 6*baseHeight, baseDepth*100, baseWidth/18, 20, baseDepth*4);
	circle(basePos.x + (baseWidth/2) + 5*baseWidth + 4*baseWidth/18, basePos.y + (baseHeight/2) + 6*baseHeight, baseDepth*1.25, baseWidth*0.20, 20);
	
	cylinder(basePos.x + (baseWidth/2) + 7*baseWidth - 4*baseWidth/18, basePos.y + (baseHeight/2) + 6*baseHeight, baseDepth*100, baseWidth/18, 20, baseDepth*4);
	circle(basePos.x + (baseWidth/2) + 7*baseWidth - 4*baseWidth/18, basePos.y + (baseHeight/2) + 6*baseHeight, baseDepth*1.25, baseWidth*0.20, 20);

	cylinder(basePos.x + (baseWidth/2) + 6*baseWidth, basePos.y + (baseHeight/2) + 5*baseHeight + 4*baseWidth/18, baseDepth*100, baseWidth/18, 20, baseDepth*4);
	circle(basePos.x + (baseWidth/2) + 6*baseWidth, basePos.y + (baseHeight/2) + 5*baseHeight + 4*baseWidth/18, baseDepth * 1.25, baseWidth * 0.20, 20);

	cylinder(basePos.x + (baseWidth/2) + 6*baseWidth, basePos.y + (baseHeight/2) + 7*baseHeight - 4*baseWidth/18, baseDepth*100, baseWidth/18, 20, baseDepth*4);
	circle(basePos.x + (baseWidth/2) + 6*baseWidth, basePos.y + (baseHeight/2) + 7 * baseHeight - 4*baseWidth/18, baseDepth * 1.25, baseWidth * 0.20, 20);

	glPopMatrix();//carousel pop


	//Lights ------------------------
	
	glGetFloatv(GL_MODELVIEW_MATRIX, &mview[0][0]);
	glEnable(GL_LIGHTING); //allows illumination use
	glEnable(GL_NORMALIZE); //uses inverters for normals (standardized normals)
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, localViewer);

	//Ambient light --------------

	if (ambientOn) {
		ambientLight[0] = 1.;//R
		ambientLight[1] = 1.;//G
		ambientLight[2] = 1.;//B
		ambientLight[3] = 1;//useless
	}
	else {
		ambientLight[0] = 0.;
		ambientLight[1] = 0.;
		ambientLight[2] = 0.;
		ambientLight[3] = 1;
	}
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	//Directional light --------------

	loadMaterial(11); //Silver
	directionalLight();

	//Ponctual light --------------

	loadMaterial(16); //White plastic
	ponctualLight();

	//Focus light --------------

	focusLight();

	//Choice light --------------

	loadMaterial(10); //Gold
	choiceLights(basePos.x - 7 * baseWidth + (1 * 2 * (baseWidth)), basePos.y + (floorHeight / 4) - baseHeight, baseDepth * 5 + 1, GL_LIGHT3);
	choiceLights(basePos.x - 7 * baseWidth + (3 * 2 * (baseWidth)), basePos.y + (floorHeight / 4) - baseHeight, baseDepth * 5 + 1, GL_LIGHT4);
	choiceLights(basePos.x - 7 * baseWidth + (5 * 2 * (baseWidth)), basePos.y + (floorHeight / 4) - baseHeight, baseDepth * 5 + 1, GL_LIGHT5);
	choiceLights(basePos.x - 7 * baseWidth + (7 * 2 * (baseWidth)), basePos.y + (floorHeight / 4) - baseHeight, baseDepth * 5 + 1, GL_LIGHT6);
	//On or Off
	if (lightOn == 4) {
		glDisable(GL_LIGHT3);
		glDisable(GL_LIGHT4);
		glDisable(GL_LIGHT5);
		glDisable(GL_LIGHT6);
	}
	else {
		glEnable(GL_LIGHT3);
		glEnable(GL_LIGHT4);
		glEnable(GL_LIGHT5);
		glEnable(GL_LIGHT6);
	}
	
	loadMaterial(16); //White plastic
	skybox();

	glPopMatrix(); //master pop --------------------------------
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	case '1':
		glDisable(GL_CULL_FACE);
		break;
	case '2':
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		break;
	case '3':
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		break;
	case '4':
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT_AND_BACK);
		break;
	case 'g':
		//grid
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'f':
		//fill
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 'p':
		//perspective
		psp = !psp;
		break;
	case 'v':
		//view
		if (top < 2) {
			top += 1;
		}
		else {
			top = 0;
		}
		break;
	case 'd':
		//directional light (on/off)
		if (dirOn < 4) {
			dirOn+=1;
		}
		else {
			dirOn = 0;
		}
		break;
	case 'c':
		//ponctual light (on/off) - carroussel
		if (pointOn < 4) {
			pointOn += 1;
		}
		else {
			pointOn = 0;
		}
		break;
	case 't':
		//focus light (on/off) - train
		if (spotOn < 4) {
			spotOn += 1;
		}
		else {
			spotOn = 0;
		}
		break;
	case 'l':
		//choice light (on/off) - trees
		if (lightOn < 4) {
			lightOn += 1;
		}
		else {
			lightOn = 0;
		}
		break;
	case 'a':
		ambientOn = !ambientOn;
		break;
	}
	//case "a" -> comp ambiente on/off
	//case "d" -> comp difusa on/off
	//case "e" -> comp especular on/off
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	setup();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

void ofApp::skybox() {
	glColor3f(1, 1, 1);

	glEnable(GL_TEXTURE);
	img3.bind();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glPushMatrix();
	if (psp) {
		glRotatef(ry, 0, 1, 0);
		ry += 0.05;
	}
	glScalef(gw() * 1.05, gw() * 1.05, gw() * 1.05);

	sphereSkyBox(128);
	glPopMatrix();

	img3.unbind();
	glDisable(GL_TEXTURE);
}

void ofApp::river() {
	phaseX += 0.002 * cos(dirVecTheta1 * 0.001);
	phaseY += 0.003 * cos(dirVecTheta1 * 0.0001);
	dirVecTheta1 += 0.5;

	//desenhamos a malha
	int m = 120;
	int n = 90;

	GLfloat x_start = -0.5;
	GLfloat y_start = 0.5;
	GLfloat x_step = 1.0 / GLfloat(m);
	GLfloat y_step = 1.0 / GLfloat(n);
	GLfloat noise = 0.;
	GLfloat z;
	ofVec3f normal;

	texCoords.clear();
	vertexes.clear();

	for (int j = 0; j < n; j++) {
		for (int i = 0; i < m; i++) {
			noise = getMultiNoise((i)*x_step + phaseX, (j)*y_step + phaseY, freqs, amps, 10);
			float x = i * x_step + x_start;
			float y = -j * y_step + y_start;
			float z = noise - 0.5;
			vertexes.push_back(ofVec3f(x, y, z));
			texCoords.push_back(ofVec2f(i * x_step, j * y_step));
		}
	}

	normals.clear();
	for (int j = 0; j < n; j++) {
		for (int i = 0; i < m; i++) {
			if (i < m - 1 && j < n - 1) {
				int i1 = j * m + i;
				int i2 = j * m + (i + 1);
				int i3 = (j + 1) * m + i;
				ofVec3f v1 = vertexes[i2] - vertexes[i1];
				ofVec3f v2 = vertexes[i3] - vertexes[i1];
				normal = cross(v2, v1).normalized();
				normals.push_back(normal);
			}
			else {
				normals.push_back(normals[normals.size() - 1]);
			}

		}
	}


	//finalmente, desenhamos a malha
	glPushMatrix();
	//glScalef(1.75 * gw(), 1. * gh(), 250 * (0.5 * cos(dirVecTheta1 * PI / 180.) + 0.5) + 50.);
	glTranslatef(basePos.x - 8 * baseWidth, basePos.y - 0.5 * baseHeight, 2);
	glScalef(3 * (baseWidth), floorHeight, 6.);

	glBegin(GL_QUADS);
	for (int j = 0; j < n - 1; j++) {
		for (int i = 0; i < m - 1; i++) {
			int i1 = j * m + i;
			int i2 = (j + 1) * m + i;
			int i3 = (j + 1) * m + (i + 1);
			int i4 = j * m + (i + 1);
			glTexCoord2f(texCoords[i1].x, texCoords[i1].y);
			glNormal3f(normals[i1].x, normals[i1].y, normals[i1].z);
			glColor3f(normals[i1].x, normals[i1].y, normals[i1].z);
			glVertex3f(vertexes[i1].x, vertexes[i1].y, vertexes[i1].z);

			glTexCoord2f(texCoords[i2].x, texCoords[i2].y);
			glNormal3f(normals[i2].x, normals[i2].y, normals[i2].z);
			glColor3f(normals[i2].x, normals[i2].y, normals[i2].z);
			glVertex3f(vertexes[i2].x, vertexes[i2].y, vertexes[i2].z);

			glTexCoord2f(texCoords[i3].x, texCoords[i3].y);
			glNormal3f(normals[i3].x, normals[i3].y, normals[i3].z);
			glColor3f(normals[i3].x, normals[i3].y, normals[i3].z);
			glVertex3f(vertexes[i3].x, vertexes[i3].y, vertexes[i3].z);

			glTexCoord2f(texCoords[i4].x, texCoords[i4].y);
			glNormal3f(normals[i4].x, normals[i4].y, normals[i4].z);
			glColor3f(normals[i4].x, normals[i4].y, normals[i4].z);
			glVertex3f(vertexes[i4].x, vertexes[i4].y, vertexes[i4].z);
		}
	}
	glEnd();
	glPopMatrix();

}

void ofApp::directionalLight() {

	dirVec[0] = dirVec3f.x;//x
	dirVec[1] = dirVec3f.y;//y
	dirVec[2] = dirVec3f.z;//z
	dirVec[3] = 0;//vetor - direction!

	//Ambient
	if (dirOn == 1) {
		//Off
		dirAmb[0] = 0;//R
		dirAmb[1] = 0;//G
		dirAmb[2] = 0;//B
		dirAmb[3] = 1.;//constant
	}
	else {
		//On
		dirAmb[0] = 0.5;//R
		dirAmb[1] = 0.5;//G
		dirAmb[2] = 0.5;//B
		dirAmb[3] = 1.;//constant
	}

	//Difuse
	if (dirOn == 2) {
		//Off
		dirDif[0] = 0;//R
		dirDif[1] = 0;//G
		dirDif[2] = 0;//B
		dirDif[3] = 1.;//constant
	}
	else {
		//On
		dirDif[0] = 0.5;//R
		dirDif[1] = 0.5;//G
		dirDif[2] = 0.5;//B
		dirDif[3] = 1.;//constant
	}

	//Specular
	if (dirOn == 3) {
		//Off
		dirSpec[0] = 0;//R
		dirSpec[1] = 0;//G
		dirSpec[2] = 0;//B
		dirSpec[3] = 1.;//constant
	}
	else {
		//On
		dirSpec[0] = 0.5;//R
		dirSpec[1] = 0.5;//G
		dirSpec[2] = 0.5;//B
		dirSpec[3] = 1.;//constant
	}

	glLightfv(GL_LIGHT0, GL_POSITION, dirVec);
	glLightfv(GL_LIGHT0, GL_AMBIENT, dirAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dirDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, dirSpec);
	glEnable(GL_LIGHT0);

	//On or Off
	if (dirOn == 4) {
		glDisable(GL_LIGHT0);
	}
	else {
		glEnable(GL_LIGHT0);
	}

	//Light0 representation
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE);
	img0.bind();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glPushMatrix();
	glTranslatef(dirVec3f.x, dirVec3f.y - floorWidth/2, dirVec3f.z - 80);
	glScalef(30, 30, 30);
	sphereSkyBox(64);
	glPopMatrix();
	img0.unbind();
	glDisable(GL_TEXTURE);

	glPushMatrix();
	glBegin(GL_LINES);
	glVertex3f(0, -floorWidth/2, 0);
	glVertex3f(dirVec3f.x, dirVec3f.y - floorWidth/2, dirVec3f.z - 80);
	glEnd();
	glPopMatrix();
}

void ofApp::ponctualLight(){
	
	pointPos[3] = 1;//point - position!

	//Ambient
	if (pointOn == 1) {
		//Off
		pointAmb[0] = 0;//R
		pointAmb[1] = 0;//G
		pointAmb[2] = 0.;//B
		pointAmb[3] = 1.;//constant
	}
	else {
		//On
		pointAmb[0] = 0.20;//R
		pointAmb[1] = 0.20;//G
		pointAmb[2] = 0.;//B
		pointAmb[3] = 1.;//constant
	}

	//Difuse
	if (pointOn == 2) {
		//Off
		pointDif[0] = 0;//R
		pointDif[1] = 0;//G
		pointDif[2] = 0.;//B
		pointDif[3] = 1.;//constant
	}
	else {
		//On
		pointDif[0] = 0.20;//R
		pointDif[1] = 0.20;//G
		pointDif[2] = 0.;//B
		pointDif[3] = 1.;//constant
	}

	//Specular
	if (pointOn == 3) {
		//Off
		pointSpec[0] = 0;//R
		pointSpec[1] = 0;//G
		pointSpec[2] = 0.;//B
		pointSpec[3] = 1.;//constant
	}
	else {
		//On
		pointSpec[0] = 0.20;//R
		pointSpec[1] = 0.20;//G
		pointSpec[2] = 0.;//B
		pointSpec[3] = 1.;//constant
	}

	glLightfv(GL_LIGHT1, GL_POSITION, pointPos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, pointAmb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, pointDif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, pointSpec);

	//atenuation
	pointAtC = 1;
	pointAtL = 0.001;
	pointAtQ = 0.0001;
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, pointAtC);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, pointAtL);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, pointAtQ);
	glEnable(GL_LIGHT1);

	//On or Off
	if (pointOn == 4) {
		glDisable(GL_LIGHT1);
	}
	else {
		glEnable(GL_LIGHT1);
	}

	//Light1 representation
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(pointPos[0], pointPos[1], pointPos[2]);
	glScalef(4, 4, 4);
	cube_unit();
	glPopMatrix();
}

void ofApp::focusLight(){
	
	//Position (train)
	spotPos[0] = trainPosX;
	spotPos[1] = trainPosY;
	spotPos[2] = baseDepth * 7 + 1;
	spotPos[3] = 1.;

	//Ambient
	if (spotOn == 1) {
		//Off
		spotAmb[0] = 0.;//R
		spotAmb[1] = 0.;//G
		spotAmb[2] = 0.;//B
		spotAmb[3] = 0.;//constant
	}
	else {
		//On
		spotAmb[0] = 0.;//R
		spotAmb[1] = 0.;//G
		spotAmb[2] = 0.;//B
		spotAmb[3] = 0.;//constant
	}
	
	//Difuse: yellow light
	if (spotOn == 2) {
		//Off
		spotDif[0] = 0.;//R
		spotDif[1] = 0.;//G
		spotDif[2] = 0.;//B
		spotDif[3] = 1.;//constant
	}
	else {
		//On
		spotDif[0] = 1.;//R
		spotDif[1] = 1.;//G
		spotDif[2] = 0.;//B
		spotDif[3] = 1.;//constant
	}

	//Specular
	if (spotOn == 3) {
		//Off
		spotSpecular[0] = 0.;//R
		spotSpecular[1] = 0.;//G
		spotSpecular[2] = 0.;//B
		spotSpecular[3] = 1.;//constant
	}
	else {
		//On
		spotSpecular[0] = 1.;//R
		spotSpecular[1] = 1.;//G
		spotSpecular[2] = 1.;//B
		spotSpecular[3] = 1.;//constant
	}

	//Concentration
	spotExponent = 10;//0 - 128

	//Angle
	spotCutoff = 60;//0 - 180

	glLightfv(GL_LIGHT2, GL_POSITION, spotPos);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDir);

	glLightfv(GL_LIGHT2, GL_AMBIENT, spotAmb);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, spotDif);
	glLightfv(GL_LIGHT2, GL_SPECULAR, spotSpecular);

	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, spotExponent);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, spotCutoff);

	//Atenuation
	spotAtC = 1.; //Constant Attenuation
	spotAtL = 0.01; //Linear Attenuation - Less to reduce the distance
	spotAtQ = 0; //Quadratic Attenuation - Less to reduce even more the distance
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, spotAtC);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, spotAtL);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, spotAtQ);
	glEnable(GL_LIGHT2);

	//On or Off
	if (spotOn == 4) {
		glDisable(GL_LIGHT2);
	}
	else {
		glEnable(GL_LIGHT2);
	}

	//Light2 representation
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(spotPos[0], spotPos[1], spotPos[2]);
	glScalef(10, 10, 10);
	cube_unit();
	glPopMatrix();
}

void ofApp::choiceLights(GLfloat lightPos0, GLfloat lightPos1, GLfloat lightPos2, GLenum GL_LIGHT){
	
	//Tree lights
	
	//Direction: down
	lightDir[0] = 0.;
	lightDir[1] = 0.;
	lightDir[2] = -1.;

	//Position (trees)
	lightPos[0] = lightPos0;
	lightPos[1] = lightPos1;
	lightPos[2] = lightPos2;
	lightPos[3] = 1.;

	//Ambient
	if (lightOn == 1) {
		//Off
		lightAmb[0] = 0.;//R
		lightAmb[1] = 0.;//G
		lightAmb[2] = 0.;//B
		lightAmb[3] = 0.;//constant
	}
	else {
		//On
		lightAmb[0] = 0.;//R
		lightAmb[1] = 0.;//G
		lightAmb[2] = 0.;//B
		lightAmb[3] = 0.;//constant
	}

	//Difuse: yellow light
	if (lightOn == 2) {
		//Off
		lightDif[0] = 0.;//R
		lightDif[1] = 0.;//G
		lightDif[2] = 0.;//B
		lightDif[3] = 1.;//constant
	}
	else {
		//On
		lightDif[0] = 1.;//R
		lightDif[1] = 1.;//G
		lightDif[2] = 0.;//B
		lightDif[3] = 1.;//constant
	}

	//Specular
	if (lightOn == 3) {
		//Off
		lightSpecular[0] = 0.;//R
		lightSpecular[1] = 0.;//G
		lightSpecular[2] = 0.;//B
		lightSpecular[3] = 1.;//constant
	}
	else {
		//On
		lightSpecular[0] = 1.;//R
		lightSpecular[1] = 1.;//G
		lightSpecular[2] = 1.;//B
		lightSpecular[3] = 1.;//constant
	}

	//Concentration
	lightExponent = 20;//0 - 128

	//Angle
	lightCutoff = 45;//0 - 180

	glLightfv(GL_LIGHT, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT, GL_SPOT_DIRECTION, lightDir);

	glLightfv(GL_LIGHT, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT, GL_DIFFUSE, lightDif);
	glLightfv(GL_LIGHT, GL_SPECULAR, lightSpecular);

	glLightf(GL_LIGHT, GL_SPOT_EXPONENT, lightExponent);
	glLightf(GL_LIGHT, GL_SPOT_CUTOFF, lightCutoff);

	//Atenuation
	lightAtC = 1.; //Constant Attenuation
	lightAtL = 0.01; //Linear Attenuation - Less to reduce the distance
	lightAtQ = 0; //Quadratic Attenuation - Less to reduce even more the distance
	glLightf(GL_LIGHT, GL_CONSTANT_ATTENUATION, lightAtC);
	glLightf(GL_LIGHT, GL_LINEAR_ATTENUATION, lightAtL);
	glLightf(GL_LIGHT, GL_QUADRATIC_ATTENUATION, lightAtQ);
	glEnable(GL_LIGHT);

	//Light3 representation
	glColor3f(1, 1, 0);
	glPushMatrix();
	glTranslatef(lightPos[0], lightPos[1], lightPos[2]);
	glScalef(3, 3, 3);
	cube_unit();
	glPopMatrix();
}