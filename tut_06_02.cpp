#include <GLFW\glfw3.h>
#include "linmath.h"
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <windows.h>
#include <time.h>

using namespace std;

const float DEG2RAD = 3.14159 / 180;

void processInput(GLFWwindow* window);

enum BRICKTYPE { REFLECTIVE, DESTRUCTABLE };
enum ONOFF { ON, OFF };

float pl = -0.2;
float pm = 0;
float pr = 0.2;
int hit = 0;
int paddleMade = 0;

class Brick
{
public:
	float red, green, blue;
	float x, y, width;
	BRICKTYPE brick_type;
	ONOFF onoff;

	Brick(BRICKTYPE bt, float xx, float yy, float ww, float rr, float gg, float bb)
	{
		brick_type = bt; x = xx; y = yy, width = ww; red = rr, green = gg, blue = bb;
		onoff = ON;
	};

	void movePaddle() 
	{
		
	}

	void drawBrick()
	{
		if (onoff == ON)
		{
			double halfside = width / 2;

			glColor3d(red, green, blue);
			glBegin(GL_POLYGON);

			glVertex2d(x + halfside, y + halfside);
			glVertex2d(x + halfside, y - halfside);
			glVertex2d(x - halfside, y - halfside);
			glVertex2d(x - halfside, y + halfside);

			glEnd();
		}
	}
	
};



class Circle
{
public:
	float red, green, blue;
	float radius;
	float x;
	float y;
	float speed = 0.03;
	int direction; // 1=up 2=right 3=down 4=left 5 = up right   6 = up left  7 = down right  8= down left

	Circle(double xx, double yy, double rr, int dir, float rad, float r, float g, float b)
	{
		x = xx;
		y = yy;
		radius = rr;
		red = r;
		green = g;
		blue = b;
		radius = rad;
		direction = dir;
	}

	void CheckCollision(Brick* brk)
	{
		if (brk->brick_type == REFLECTIVE)
		{
			if ((x > brk->x - brk->width && x <= brk->x + brk->width) && (y > brk->y - brk->width && y <= brk->y + brk->width))
			{
				direction = GetRandomDirection();
				x = x + 0.03;
				y = y + 0.04;
				brk->blue = myRand();
				brk->green = myRand();
				brk->red = myRand();
			}
		}
		else if (brk->brick_type == DESTRUCTABLE)
		{
			if ((x > brk->x - brk->width && x <= brk->x + brk->width) && (y > brk->y - brk->width && y <= brk->y + brk->width))
			{
				brk->onoff = OFF;
			}
		}
	}

	float myRand()
	{
		float rn = rand() % 100 + 1; // random # 1 -100
		rn = rn / 100; // change number from 0.01 to 1.0
		return rn;  // send random number back
	}

	int GetRandomDirection()
	{
		return (rand() % 8) + 1;
	}

	void MoveOneStep()
	{
		if (direction == 1 || direction == 5 || direction == 6)  // up
		{
			if (y > -1 + radius)
			{
				y -= speed;
			}
			else
			{
				// 1=up 2=right 3=down 4=left 5 = up right   6 = up left  7 = down right  8= down left
				//direction = GetRandomDirection();
				switch (direction)
				{
				case 1:
					direction = 3; // from up to down
					break;
				case 5:
					direction = 7; // from up right to down right
					break;
				case 6:
					direction = 8; // from up left to down left
				default:
					break;
				}
			}
		}

		if (direction == 2 || direction == 5 || direction == 7)  // right
		{
			if (x < 1 - radius)
			{
				x += speed;
			}
			else
			{
				//direction = GetRandomDirection();
				// 1=up 2=right 3=down 4=left 5 = up right   6 = up left  7 = down right  8= down left
				switch (direction)
				{
				case 2:
					direction = 4; // from left to right
					break;
				case 5:
					direction = 6; // from up right to up left
					break;
				case 7:
					direction = 8; // from down right to down left
				default:
					break;
				}
			}
		}

		if (direction == 3 || direction == 7 || direction == 8)  // down
		{
			if (y < 1 - radius) {
				y += speed;
			}
			else
			{
				//direction = GetRandomDirection();
				// 1=up 2=right 3=down 4=left 5 = up right   6 = up left  7 = down right  8= down left
				switch (direction)
				{
				case 3:
					direction = 1; // from down to up
					break;
				case 7:
					direction = 5; // from down right to up right
					break;
				case 8:
					direction = 6; // from down left to up left
					break;
				default:
					break;
				}
			}
		}

		if (direction == 4 || direction == 6 || direction == 8)  // left
		{
			if (x > -1 + radius) {
				x -= speed;
			}
			else
			{
				//direction = GetRandomDirection();
				// 1=up 2=right 3=down 4=left 5 = up right   6 = up left  7 = down right  8= down left
				switch (direction)
				{
				case 4:
					direction = 2; // from left to right
					break;
				case 6:
					direction = 5; // from up left to up right
					break;
				case 8:
					direction = 7; // down left to down right
					break;
				default:
					break;
				}
			}
		}
	}

	void DrawCircle()
	{
		glColor3f(red, green, blue);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++) {
			float degInRad = i * DEG2RAD;
			glVertex2f((cos(degInRad) * radius) + x, (sin(degInRad) * radius) + y);
		}
		glEnd();
	}
};


vector<Circle> world;


int main(void) {
	srand(time(NULL));

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	GLFWwindow* window = glfwCreateWindow(480, 480, "Random World of Circles", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);


	//          BRICKTYPE bt, float xx, float yy, float ww, float rr, float gg, float bb
	// pl = -0.2, pm = 0, pr = 0.2

	Brick brick(REFLECTIVE, 0.5, 0.33, 0.2, 1, 1, 0);
	Brick brick2(DESTRUCTABLE, -0.5, 0.33, 0.2, 0, 1, 0);
	Brick brick3(DESTRUCTABLE, -0.5, 0.33, 0.2, 0, 1, 1);
	Brick brick4(REFLECTIVE, 0, 0, 0.2, 1, 0.5, 0.5);
	Brick paddleM(REFLECTIVE, pm, -0.6, 0.2, 1.0, 0, 0);
	Brick paddleL(REFLECTIVE, pl, -0.6, 0.2, 1.0, 0, 0);
	Brick paddleR(REFLECTIVE, pr, -0.6, 0.2, 1.0, 0, 0);
		
	
	

	while (!glfwWindowShouldClose(window)) {
		//Setup View
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		processInput(window);

		//Movement
		for (int i = 0; i < world.size(); i++)
		{
			world[i].CheckCollision(&brick);
			world[i].CheckCollision(&brick2);
			world[i].CheckCollision(&brick3);
			world[i].CheckCollision(&brick4);
			world[i].CheckCollision(&paddleL);
			world[i].CheckCollision(&paddleM);
			world[i].CheckCollision(&paddleR);
			world[i].MoveOneStep();
			world[i].DrawCircle();
		}

		

		brick.drawBrick();
		brick2.drawBrick();
		brick3.drawBrick();
		brick4.drawBrick();
		paddleL.drawBrick();
		paddleM.drawBrick();
		paddleR.drawBrick();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate;
	exit(EXIT_SUCCESS);
}


void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		double r, g, b;
		r = rand() / 10000;
		g = rand() / 10000;
		b = rand() / 10000;
		Circle B(0, 0, 02, 2, 0.05, r, g, b);
		world.push_back(B);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		pr = pr -0.01;
		pm = pm -0.01;
		pl = pl -0.01;
		Brick paddleM(REFLECTIVE, pm, -0.6, 0.2, 1.0, 0, 0);
		Brick paddleL(REFLECTIVE, pl, -0.6, 0.2, 1.0, 0, 0);
		Brick paddleR(REFLECTIVE, pr, -0.6, 0.2, 1.0, 0, 0);
		paddleM.drawBrick();
		paddleL.drawBrick();
		paddleR.drawBrick();
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		pr = pr + 0.01;
		pm = pm + 0.01;
		pl = pl + 0.01;
		Brick paddleM(REFLECTIVE, pm, -0.6, 0.2, 1.0, 0, 0);
		Brick paddleL(REFLECTIVE, pl, -0.6, 0.2, 1.0, 0, 0);
		Brick paddleR(REFLECTIVE, pr, -0.6, 0.2, 1.0, 0, 0);
		paddleM.drawBrick();
		paddleL.drawBrick();
		paddleR.drawBrick();
	}

}

