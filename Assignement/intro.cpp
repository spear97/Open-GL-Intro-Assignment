/*
* GL03Viewport.cpp: Clipping-area and Viewport
* Implementing   to ensure same aspect ratio between the
* clipping-area and the viewport.
*/
//#include <windows.h>  // for MS Windows
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

#include <GL/glut.h>  // GLUT, include glu.h and gl.h

float r1 = 1;
float g1 = 0;
float b1 = 0;
float winW = 640;
float winH = 480;
vector< pair<float, float> > boxes;
enum graphics { dt, ln, hx, bx, st };
graphics curr = bx;
graphics last;

const double PI = 3.14159;
const double TWOPI = 6.28318;

double MyRand() {
	return (1.0 * rand()) / RAND_MAX;
}

/* Initialize OpenGL Graphics */
void initGL() {
	// Set "clearing" or background color
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
	glClearColor(0.22352941176471f, 1.0f, 0.07843137254902f, 1.0f);
}

void drawBox(float x, float y, float size) {
	glColor3f(0, 0, 1);
	glBegin(GL_POLYGON);
	glVertex2f(x + size, y + size);
	glVertex2f(x - size, y + size);
	glVertex2f(x - size, y - size);
	glVertex2f(x + size, y - size);
	glEnd();
}

void drawPolygon(float x, float y, float size)
{
	glColor3f(0, 0, 1);
	glBegin(GL_POLYGON);
	glVertex2f(x - (size / 2), y + size);
	glVertex2f(x + (size / 2), y + size);
	glVertex2f(x + (size + 0.005f), y);
	glVertex2f(x + (size / 2), y - size);
	glVertex2f(x - (size / 2), y - size);
	glVertex2f(x - (size + 0.005), y);
	glEnd();
}

void drawStar(float x, float y, float s, int tips) {
	glColor3f(0, 0, 1);
	/*
	glBegin(GL_LINE_STRIP);
	for(int i=0; i<tips*2; i++) {
	  double theta = (2.0*PI*i)/(tips/2);
	  double hyp = s;
	  //if(i%2==1) hyp /= 2;
	  double xl = x + hyp * sin(theta);
	  double yl = y + hyp * cos(theta);
	  cout << "xl = " << xl << " yl = " << yl << endl;
	  glVertex2f(xl, yl);
	}
	glEnd();
	*/
	///*
	double radius = s;
	double delta = 0.4f;
	glPushMatrix();
	glTranslatef(x, y, 0);
	//glBegin(GL_LINE_STRIP);
	glBegin(GL_POLYGON);
	int i = 0;
	for (float theta = 0; theta < TWOPI + delta; theta += delta, i++) {
		double r2 = radius;
		if (i % 2 == 0) r2 /= 2.0;
		glVertex3f(r2 * sin(theta), r2 * cos(theta), 0);
		if (theta > TWOPI) theta = TWOPI;
	}
	glEnd();
	glPopMatrix();
	//*/
}

void drawDot(float x, float y, float size) {
	glColor3f(0, 0, 1);
	glBegin(GL_POINTS);
	glPointSize(size);
	glVertex2f(x, y);
	glEnd();
}

void drawLine(float size)
{
	glColor3f(0, 0, 1);
	glLineWidth(size);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < boxes.size(); i++)
		glVertex2f(boxes[i].first, boxes[i].second);
	glEnd();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color

	// Define shapes enclosed within a pair of glBegin and glEnd
	glBegin(GL_QUADS);              // Each set of 4 vertices form a quad

	glColor3f(0.69019607843137f, 0.14901960784314f, 1.f);
	glVertex2f(-0.7f, -0.6f);
	glVertex2f(0.7f, -0.6f);
	glVertex2f(0.7f, 0.6f);
	glVertex2f(-0.7f, 0.6f);

	glColor3f(1.f, 0.51372549019608f, 0.f);
	glVertex2f(-0.9f, 0.7f);
	glVertex2f(-0.7f, 0.6f);
	glVertex2f(-0.7f, -0.6f);
	glVertex2f(-0.9f, -0.5f);

	glColor3f(0.12156862745098f, 0.31764705882353f, 1.f);
	glVertex2f(-0.9f, 0.7f);
	glVertex2f(0.5f, 0.7f);
	glVertex2f(0.7f, 0.6f);
	glVertex2f(-0.7f, 0.6f);

	glColor3f(0.19607843137255f, 0.090196078431373f, 0.30196078431373f);
	glVertex2f(-0.5f, 0.35f);
	glVertex2f(-0.5f, 0.20f);
	glVertex2f(-0.25f, 0.20f);
	glVertex2f(-0.25f, 0.35f);

	glColor3f(0.19607843137255f, 0.090196078431373f, 0.30196078431373f);
	glVertex2f(0.5f, 0.35f);
	glVertex2f(0.5f, 0.20f);
	glVertex2f(0.25f, 0.20f);
	glVertex2f(0.25f, 0.35f);

	glColor3f(0.f, 0.f, 0.f);
	glVertex2f(0.4, -0.2);
	glVertex2f(0.4, -0.4);
	glVertex2f(-0.4, -0.4);
	glVertex2f(-0.4, -0.2);

	glEnd();

	glBegin(GL_TRIANGLES);          // Each set of 3 vertices form a triangle

	glColor3f(1.f, 0.11372549019608f, 0.8078431372549f);
	glVertex2f(-0.1f, 0.1f);
	glVertex2f(0.1f, 0.1f);
	glVertex2f(0.f, -0.1f);

	glEnd();

	glBegin(GL_LINE_STRIP);

	glColor3f(0.18823529411765f, 0.83529411764706f, 0.7843137254902f);
	glVertex2f(-0.4f, -0.1f);
	glVertex2f(0.4f, -0.1f);

	glEnd();

	for (int i = 0; i < boxes.size(); i++) {
		pair<float, float> box = boxes[i];
		if (curr == bx)
			drawBox(box.first, box.second, 0.05);
		if (curr == ln)
			drawLine(5);
		if (curr == hx)
			drawPolygon(box.first, box.second, 0.05);
		if (curr == st)
			drawStar(box.first, box.second, 0.05, 5);
		if (curr == dt)
			drawDot(box.first, box.second, 50);
	}

	glFlush();  // Render now
}

/* Handler for window re-size event. Called back when the window first appears and
whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
	// Compute aspect ratio of the new window
	if (height == 0) height = 1;                // To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;
	winW = width;
	winH = height;

	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);

	// Set the aspect ratio of the clipping area to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset the projection matrix
	if (width >= height) {
		// aspect >= 1, set the height from -1 to 1, with larger width
		gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
	}
	else {
		// aspect < 1, set the width to -1 to 1, with larger height
		gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
	}
}

void mykeyboard(unsigned char key, int x, int y) {

	//Quit Program
	if (key == 'Q' || key == 'q') exit(-1);
	else if (key == 27) exit(-1);

	//Random Color
	else if (key == 'R' || key == 'r') {
		//set a random color
		r1 = MyRand();
		g1 = MyRand();
		b1 = MyRand();
		glutPostRedisplay();
	}

	//Clear
	else if (key == 'C' || key == 'c') {
		boxes.clear();
		glutPostRedisplay();
	}

	//Box
	else if (key == 'b' || key == 'B') {

		/* Lines will be drawn */
		if (curr != bx)
		{
			last = curr;
			curr = bx;
		}
		else
		{
			curr = last;
		}
		glutPostRedisplay();
	}

	//Polygon
	else if (key == 'p' || key == 'P') {

		/* Polygons will be drawn */
		if (curr != hx)
		{
			last = curr;
			curr = hx;
		}
		else
		{
			curr = last;
		}
		glutPostRedisplay();
	}

	//Dot
	else if (key == 'd' || key == 'D') {

		/* Polygons will be drawn */
		if (curr != dt)
		{
			last = curr;
			curr = dt;
		}
		else
		{
			curr = last;
		}
		glutPostRedisplay();
	}

	//Line
	else if (key == 'l' || key == 'L') {
		/* Line will be drawn */
		if (curr != ln)
		{
			last = curr;
			curr = ln;
		}
		else
		{
			curr = last;
		}
	}

	//Star
	else if (key == 's' || key == 'S') {

		/* Stars will be drawn */
		if (curr != st)
		{
			last = curr;
			curr = st;
		}
		else
		{
			curr = last;
		}
		glutPostRedisplay();
	}
	else {
		cout << "unknown key: " << key << endl;
	}
}

void mymouse(int btn, int state, int x, int y) {
	/*
	if(btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN) {
	  cout << "Right button click" << endl;
	  exit(0);
	}
	*/
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		cout << "Left button click x = " << x << " y = " << y << endl;
		GLfloat aspect = (GLfloat)winW / (GLfloat)winH;
		float xcoord = ((1.0 * x) / winW) * 2 * aspect - aspect;
		float ycoord = 2 * (1 - ((1.0 * y) / winH)) - 1;
		cout << "xcoord = " << xcoord << " ycoord = " << ycoord << endl;
		pair<float, float> box(xcoord, ycoord);
		boxes.push_back(box);
		glutPostRedisplay();
	}

}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
	glutInit(&argc, argv);          // Initialize GLUT
	glutInitWindowSize(640, 480);   // Set the window's initial width & height - non-square
	glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
	glutCreateWindow("Viewport Transform");  // Create window with the given title
	glutDisplayFunc(display);       // Register callback handler for window re-paint event
	glutReshapeFunc(reshape);       // Register callback handler for window re-size event
	glutKeyboardFunc(mykeyboard);
	glutMouseFunc(mymouse);
	initGL();                       // Our own OpenGL initialization
	glutMainLoop();                 // Enter the infinite event-processing loop
	return 0;
}
