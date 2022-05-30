#include <windows.h>
#include <iostream>
#include <gl/gl.h>
#include <gl/glut.h> // (or others, depending on the system in use)
#include <math.h>
#include <iostream>
#include <stdio.h>   // Header File For Standard Input/Output
#include <stdarg.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <cstdio>

using namespace std;
#define	width 			500
#define	height			600
#define	PI				3.1415
#define	polygon_num		50

int		left_ = 0;
int		bottom = 0;

int		collision_count = 0;

float	radius1, moving_ball_radius;

float	gameover_colorr = 0.0, gameover_colorg = 0.0, gameover_colorb = 0.0;

int count_ = 0;

bool	intro = false;
bool	check = false;
bool	start = false;
bool	restart = false;

float xrot = 0.00, yrot = 0.00, zoom = -30;

class Point {
public:
	float x;
	float y;

	Point(float a, float b) {
		this->x = a;
		this->y = b;
	}

	Point() {
		this->x = 0;
		this->y = 0;
	}
};

Point moving_ball, item, velocity, rectangle[4], bar_point, bottom_point, gameover_point, start_point;

// 벽돌의 위치를 저장하는 class
class BRICKS {
public:
	Point rectangle[4];
};

BRICKS block_array[7][10]; // 가로 10개씩 7줄

class BAR {
public:
	Point rectangle[4];
};

BAR bar;

class BOTTOM{
public:
	Point rectangle[4];
};

BOTTOM bott;

class GAMEOVER_WINDOW {
public:
	Point rectangle[4];
};

GAMEOVER_WINDOW gameover;

class START_SCREEN {
public:
	Point rectangle[4];
};

START_SCREEN start_screen;

//class ITEM {
//public:
//	Point item;
//};
//
//ITEM item[3];

int w, h;

unsigned char* LoadMeshFromFile(const char* texFile)
{
	
	GLuint texture;
	glGenTextures(1, &texture);
	FILE* fp = NULL;
	if (fopen_s(&fp, texFile, "rb")) {
		printf("ERROR : No %s. \n fail to bind %d\n", texFile, texture);
		return (unsigned char*)false;
	}
	int channel;
	unsigned char* image = stbi_load_from_file(fp, &w, &h, &channel, 4);
	fclose(fp);
	return image;
}

void intro_init()
{
	GLuint texID;

	unsigned char* bitmap;
	bitmap = LoadMeshFromFile((char*)"sunsetbeach.jpg");
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);

	free(bitmap);
}

void init_gameout()
{
	GLuint texID;

	unsigned char* bitmap;
	bitmap = LoadMeshFromFile((char*)"gameover1.png");
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
	
	free(bitmap);
}

void drawBox()
{
	glBegin(GL_POLYGON);
	glTexCoord2d(0.0, 1.0);      glVertex2d(0, 0);
	glTexCoord2d(0.0, 0.0);      glVertex2d(0, h);
	glTexCoord2d(1.0, 0.0);      glVertex2d(w, h);
	glTexCoord2d(1.0, 1.0);      glVertex2d(w, 0);
	glEnd();
}

void init(void) {

	int start_x = 5;
	int start_y = 595;

	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 10; j++) {
			block_array[i][j].rectangle[0].x = block_array[i][j].rectangle[1].x = start_x;
			block_array[i][j].rectangle[0].y = block_array[i][j].rectangle[3].y = start_y;

			block_array[i][j].rectangle[2].x = block_array[i][j].rectangle[3].x = start_x + 40;
			block_array[i][j].rectangle[1].y = block_array[i][j].rectangle[2].y = start_y - 30;

			start_x += 50;
		}
		start_x = 5;
		start_y -= 40;
	}

	bottom_point.x = 0;
	bottom_point.y = 5;

	bar_point.x = 200;
	bar_point.y = 40;

	gameover_point.x = 0;
	gameover_point.y = height;

	moving_ball_radius = 10.0;
	moving_ball.x = width/2;
	moving_ball.y = 500;

	velocity.x = 0.05;
	velocity.y = 0.05;

	collision_count = 1;

}

void MyReshape(int w, int h) {

	float aspectRatio = (float)w / (float)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left_, left_ + width, bottom, bottom + height);
	glMatrixMode(GL_MODELVIEW);

}


void	Modeling_Circle(float radius, Point CC) {
	float	delta;

	delta = 2 * PI / polygon_num;
	glBegin(GL_POLYGON);

	for (int i = 0; i < polygon_num; i++)
		glVertex2f(CC.x + radius * cos(delta * i), CC.y + radius * sin(delta * i));
	glEnd();
}

void	Modeling_brick() {

	for (int i = 0; i < 7; i++) {
		if (i == 0) {
			glColor3i(1.0, 1.0, 0.2);
		}
		if (i == 1) {
			glColor3f(1.0, 1, 0.3);
		}
		if (i == 2) {
			glColor3f(1.0, 1, 0.4);
		}
		if (i == 3) {
			glColor3f(1.0, 1.0, 0.5);
		}
		if (i == 4) {
			
			glColor3f(1.0, 1.0, 0.6);
		}
		if (i == 5) {
			glColor3f(1.0, 1.0, 0.8);
		}
		if (i == 6) {
			glColor3f(1.0, 1.0, 1);
		}

		for (int j = 0; j < 10; j++) {
			for (int k = 0; k < 4; k++) {
				glBegin(GL_QUADS);
				glVertex2i(block_array[i][j].rectangle[k].x, block_array[i][j].rectangle[k].y);
			}
		}
	}
	glColor3f(1, 1, 1);
	
	glEnd();

}

float distance(Point a, Point b) {
	return sqrt(pow((b.x - a.x),2) + pow((b.y - a.y), 2));
}

void Collision_Detection_Between_Bricks() {

	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 10; j++) {
			//아래 충돌
			if (moving_ball.y >= block_array[i][j].rectangle[1].y - moving_ball_radius
				&& moving_ball.x >= block_array[i][j].rectangle[1].x - moving_ball_radius 
				&& moving_ball.x <= block_array[i][j].rectangle[2].x + moving_ball_radius
				&& velocity.y > 0
				&& distance(Point(moving_ball.x, moving_ball.y), Point(moving_ball.x, block_array[i][j].rectangle[1].y)) < moving_ball_radius) {
				
				velocity.y *= -1;
				cout << "아래쪽" << endl;
				count_++;
				cout << count_ << endl;
				for (int k = 0; k < 4; k++) {
					block_array[i][j].rectangle[k] = Point(0,0);
				}

			}
			//위쪽 충돌
			if (moving_ball.y <= block_array[i][j].rectangle[3].y + moving_ball_radius
				&& moving_ball_radius + moving_ball.x >= block_array[i][j].rectangle[1].x 
				&& moving_ball.x - moving_ball_radius <= block_array[i][j].rectangle[2].x
				&& velocity.y < 0
				&& distance(Point(moving_ball.x, moving_ball.y), Point(moving_ball.x, block_array[i][j].rectangle[3].y)) < moving_ball_radius) {
				
				velocity.y *= -1;
				cout << "위쪽" << endl;
				count_++;
				cout << count_ << endl;
				for (int k = 0; k < 4; k++) {
					block_array[i][j].rectangle[k] = Point(0, 0);
				}
			}
			//왼쪽 충돌
			if (moving_ball.x >= block_array[i][j].rectangle[1].x - moving_ball_radius
				&& moving_ball_radius + moving_ball.y >= block_array[i][j].rectangle[1].y 
				&& moving_ball.y - moving_ball_radius <= block_array[i][j].rectangle[3].y
				&& velocity.x > 0
				&& distance(Point(moving_ball.x, moving_ball.y), Point(block_array[i][j].rectangle[1].x, moving_ball.y)) < moving_ball_radius) {
				
				velocity.x *= -1;
				cout << "왼쪽" << endl;
				count_++;
				cout << count_ << endl;
				for (int k = 0; k < 4; k++) {
					block_array[i][j].rectangle[k] = Point(0, 0);
				}
			}
			//오른쪽 충돌
			if (moving_ball.x <= block_array[i][j].rectangle[2].x + moving_ball_radius
				&& moving_ball_radius + moving_ball.y >= block_array[i][j].rectangle[1].y 
				&& moving_ball.y - moving_ball_radius <= block_array[i][j].rectangle[3].y
				&& velocity.x < 0
				&& distance(Point(moving_ball.x, moving_ball.y), Point(block_array[i][j].rectangle[2].x, moving_ball.y)) < moving_ball_radius) {
				
				velocity.x *= -1;
				cout << "오른쪽" << endl;
				count_++;
				cout << count_ << endl;
				for (int k = 0; k < 4; k++) {
					block_array[i][j].rectangle[k] = Point(0, 0);
				}
			}
			//모서리 충돌(0)
			if (distance(Point(moving_ball.x, moving_ball.y), Point(block_array[i][j].rectangle[0])) < moving_ball_radius+3) {
				if (velocity.x > 0 && velocity.y < 0) {
					cout << "모서리0" << endl;
					velocity.x *= -1;
					velocity.y *= -1;
					count_++;
					for (int k = 0; k < 4; k++) {
						block_array[i][j].rectangle[k] = Point(0, 0);
					}
				}
				if (velocity.x < 0 && velocity.y < 0) {
					cout << "모서리0" << endl;
					velocity.y *= -1;
					count_++;
					for (int k = 0; k < 4; k++) {
						block_array[i][j].rectangle[k] = Point(0, 0);
					}
				}
				if (velocity.x > 0 && velocity.y > 0) {
					cout << "모서리0" << endl;
					velocity.x *= -1;
					count_++;
					for (int k = 0; k < 4; k++) {
						block_array[i][j].rectangle[k] = Point(0, 0);
					}
				}
			}
			//모서리 충돌(1)
			if (distance(Point(moving_ball.x, moving_ball.y), Point(block_array[i][j].rectangle[1])) < moving_ball_radius+3) {
				if (velocity.x > 0 && velocity.y > 0) {
					cout << "모서리01" << endl;
					velocity.x *= -1;
					velocity.y *= -1;
					count_++;
					for (int k = 0; k < 4; k++) {
						block_array[i][j].rectangle[k] = Point(0, 0);
					}
				}
				if (velocity.x < 0 && velocity.y > 0) {
					cout << "모서리01" << endl;
					velocity.y *= -1;
					count_++;
					for (int k = 0; k < 4; k++) {
						block_array[i][j].rectangle[k] = Point(0, 0);
					}
				}
				if (velocity.x > 0 && velocity.y < 0) {
					cout << "모서리01" << endl;
					velocity.x *= -1;
					count_++;
					for (int k = 0; k < 4; k++) {
						block_array[i][j].rectangle[k] = Point(0, 0);
					}
				}
			}
			//모서리 충돌(2)
			if (distance(Point(moving_ball.x, moving_ball.y), Point(block_array[i][j].rectangle[2]))+2 < moving_ball_radius+3) {
				if (velocity.x < 0 && velocity.y > 0) {
					cout << "모서리02" << endl;
					velocity.x *= -1;
					velocity.y *= -1;
					count_++;
					for (int k = 0; k < 4; k++) {
						block_array[i][j].rectangle[k] = Point(0, 0);
					}
				}
				if (velocity.x > 0 && velocity.y > 0) {
					cout << "모서리02" << endl;
					velocity.y *= -1;
					count_++;
					for (int k = 0; k < 4; k++) {
						block_array[i][j].rectangle[k] = Point(0, 0);
					}
				}
				if (velocity.x < 0 && velocity.y < 0) {
					cout << "모서리02" << endl;
					velocity.x *= -1;
					count_++;
					for (int k = 0; k < 4; k++) {
						block_array[i][j].rectangle[k] = Point(0, 0);
					}
				}
			}
			//모서리 충돌(3)
			if (distance(Point(moving_ball.x, moving_ball.y), Point(block_array[i][j].rectangle[3]))+2 < moving_ball_radius+3) {
				if (velocity.x < 0 && velocity.y < 0) {
					cout << "모서리03" << endl;
					velocity.x *= -1;
					velocity.y *= -1;
					count_++;
					for (int k = 0; k < 4; k++) {
						block_array[i][j].rectangle[k] = Point(0, 0);
					}
				}
				if (velocity.x > 0 && velocity.y < 0) {
					cout << "모서리03" << endl;
					velocity.y *= -1;
					count_++;
					for (int k = 0; k < 4; k++) {
						block_array[i][j].rectangle[k] = Point(0, 0);
					}
				}
				if (velocity.x < 0 && velocity.y > 0) {
					cout << "모서리03" << endl;
					velocity.x *= -1;
					count_++;
					for (int k = 0; k < 4; k++) {
						block_array[i][j].rectangle[k] = Point(0, 0);
					}
				}
			}
		}
	}
}

void Collision_Detection_to_Walls(void) {

	if (moving_ball.y >= bottom + moving_ball_radius) {
		velocity.y *= -1;
		velocity.x = velocity.x;
	}
	if (moving_ball.y <= bottom + height - moving_ball_radius) {
		velocity.y = -velocity.y;
		velocity.x = velocity.x;
	}
	if (moving_ball.x <= left_ + moving_ball_radius) {
		velocity.y = velocity.y;
		velocity.x = -velocity.x;
	}
	if (moving_ball.x >= left_ + width - moving_ball_radius) {
		velocity.y = velocity.y;
		velocity.x = -velocity.x;
	}

}

void Modeling_bar() {

	glBegin(GL_QUADS);
	glColor3f(0.0, 1.0, 1.0);
	for (int i = 0; i < 4; i++) {
		glVertex2i(bar.rectangle[i].x, bar.rectangle[i].y);
	}
	
}

void Collision_Detection_Between_bar() {

	if (moving_ball.y <= bar.rectangle[3].y + moving_ball_radius
		&& moving_ball_radius + moving_ball.x >= bar.rectangle[1].x
		&& moving_ball.x - moving_ball_radius <= bar.rectangle[2].x
		&& velocity.y < 0
		&& distance(Point(moving_ball.x, moving_ball.y), Point(moving_ball.x, bar.rectangle[3].y)) < moving_ball_radius) {

		velocity.y *= -1;
	}

}

void Modeling_bottom_range() {

	glBegin(GL_QUADS);
	for (int i = 0; i < 4; i++) {
		glVertex2i(bott.rectangle[i].x, bott.rectangle[i].y);
	}

}

void detection_bottom_failed() {

	if (moving_ball.y <= bott.rectangle[3].y + moving_ball_radius
		&& moving_ball_radius + moving_ball.x >= bott.rectangle[1].x
		&& moving_ball.x - moving_ball_radius <= bott.rectangle[2].x
		&& velocity.y < 0
		&& distance(Point(moving_ball.x, moving_ball.y), Point(moving_ball.x, bott.rectangle[3].y)) < moving_ball_radius) {

		velocity.y = 0;
		velocity.x = 0;
		moving_ball.x = -10;
		moving_ball.y = -10;
		gameover_colorr = 0.0, gameover_colorg = 0.0, gameover_colorb = 0.0;
		check = true;

		cout << "Game Over" << endl;
	}

}

void Modeling_GameOver_Window() {
	glColor3f(gameover_colorr, gameover_colorg, gameover_colorb);
	
	glBegin(GL_QUADS);
	for (int i = 0; i < 4; i++) {
		glVertex2i(gameover.rectangle[i].x, gameover.rectangle[i].y);
	}
}

void Modeling_GameStart_Window(){

	glBegin(GL_QUADS);
	for (int i = 0; i < 4; i++) {
		glVertex2i(start_screen.rectangle[i].x, start_screen.rectangle[i].y);
	}

}

void Modeling_begin_button() {
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex2i(width / 2, height / 2);
	glVertex2i(width / 2-50, height / 2-50);
	glVertex2i(width / 2+50, height / 2-50);
}

void Falling_item() {
	//아이템을 만들자!



}

void RenderScene(void) {

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	drawBox();
	
	if (intro) {
		glColor3f(1.0, 1, 1);
		drawBox();
		GLuint texID;

		unsigned char* bitmap;
		bitmap = LoadMeshFromFile((char*)"Introduce.png");
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);

		free(bitmap);
	}

	void Modeling_GameStart_Window();

	void Modeling_begin_button();

	if (start == true) {

		bott.rectangle[0] = Point(bottom_point.x, bottom_point.y);
		bott.rectangle[1] = Point(bottom_point.x, bottom_point.y - 50);
		bott.rectangle[2] = Point(bottom_point.x + width, bottom_point.y - 50);
		bott.rectangle[3] = Point(bottom_point.x + width, bottom_point.y);
		glColor3f(1.0, 1.0, 1.0);
		Modeling_bottom_range();


		glColor3f(1.0, 1.0, 1.0);
		Modeling_bar();

		bar.rectangle[0] = Point(bar_point.x, bar_point.y);
		bar.rectangle[1] = Point(bar_point.x, bar_point.y - 30);
		bar.rectangle[2] = Point(bar_point.x + 100, bar_point.y - 30);
		bar.rectangle[3] = Point(bar_point.x + 100, bar_point.y);
		Modeling_brick();

		Collision_Detection_Between_Bricks();


		// 충돌 처리 부분
		Collision_Detection_to_Walls();				// 공과 벽의 충돌 함수 

		Collision_Detection_Between_bar();

		detection_bottom_failed();

		// 움직이는 공의 위치 변화 
		moving_ball.x += velocity.x;
		moving_ball.y += velocity.y;

		// 움직이는 공 그리기 
		glColor3f(1.0, 1.0, 1.0);
		Modeling_Circle(moving_ball_radius, moving_ball);

		gameover.rectangle[0] = Point(gameover_point.x, gameover_point.y);
		gameover.rectangle[1] = Point(gameover_point.x, gameover_point.y - height);
		gameover.rectangle[2] = Point(gameover_point.x + width, gameover_point.y - height);
		gameover.rectangle[3] = Point(gameover_point.x + width, gameover_point.y);

		if (count_ >= 10) {
			glColor3f(0.8, 0.8, 0.8);
		}
		if (count_ >= 20) {
			glColor3f(0.6, 0.6, 0.6);
		}
		if (count_ >= 35) {
			glColor3f(0.4, 0.4, 0.4);
		}
		if (count_ >= 50) {
			glColor3f(0.2, 0.2, 0.2);
		}

		if (count_ == 70) {
			glColor3f(1.0, 1, 1);
			drawBox();
			GLuint texID;

			unsigned char* bitmap;
			bitmap = LoadMeshFromFile((char*)"gameover1.png");
			glEnable(GL_TEXTURE_2D);
			glGenTextures(1, &texID);
			glBindTexture(GL_TEXTURE_2D, texID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);

			free(bitmap);
		}

	}
	if (check) {
		glColor3f(1.0, 1, 1);
		drawBox();
		GLuint texID;

		unsigned char* bitmap;
		bitmap = LoadMeshFromFile((char*)"gameover_failed.png");
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);

		free(bitmap);
	}
	if (restart) {
		
	}

	glutSwapBuffers();
	glFlush();
}

void MyKey(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_LEFT:
		if (bar_point.x >= 0 && bar_point.x <= width-100) {
			bar_point.x -= 10;
		}
		if (bar_point.x <= 0) {
			bar_point.x += 10;
		}
		//cout << bar_point.x << endl;
		break;
	case GLUT_KEY_RIGHT:
		if (bar_point.x >= 0 && bar_point.x <= width-100) {
			bar_point.x += 10;
		}
		if (bar_point.x >= width - 100) {
			bar_point.x -= 10;
		}
		break;
	case GLUT_KEY_END:		//goto-introduce
		intro = true;

		break;
	case GLUT_KEY_PAGE_DOWN: //Pause
		start = false;
		break;
	case GLUT_KEY_PAGE_UP:	//restart
		init();
		check = false;
		intro_init();
		drawBox();
		count_ = 0;
		break;
	case GLUT_KEY_HOME: 
		intro = false;
		start = true;
		glColor3f(1.0, 1, 1);
		drawBox();
		GLuint texID;

		unsigned char* bitmap;
		bitmap = LoadMeshFromFile((char*)"sunsetbeach.jpg");
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);

		free(bitmap);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}


void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(width, height);
	glutCreateWindow("Brick-Breaker");
	init();
	glutReshapeFunc(MyReshape);
	intro_init();
	glutDisplayFunc(RenderScene);
	glutIdleFunc(RenderScene);
	glutSpecialFunc(MyKey);
	glutMainLoop();
}