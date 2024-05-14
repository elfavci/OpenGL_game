#include <GL/glut.h>
#include <cstdlib>
#include <memory>
#include <vector>
using namespace std;

static void Timer(int value) //enemylerin hareketi icin timer fonksiyonu olustururuz
{
	glutPostRedisplay(); //ekrana yeniden cizmesi icin

	glutTimerFunc(10, Timer, value); //enemy kuplerin ilk hizlari, ilk parametreyi arttirdikca yavas basliyor
}

class Cube
{
public:
	float x;
	float y;
	float width;
	float height;
	float speed;

	Cube(float xIn, float yIn, float wIn, float hIn, float sIn)
	{
		x = xIn;
		y = yIn;
		width = wIn;
		height = hIn;
		speed = sIn;
	}

	virtual void draw() = 0;
	virtual void move() {}
};

class Player :public Cube
{
public:
	int score;
	int life;
	int array[4];
	int level;

	Player(float xIn, float yIn, float wIn, float hIn, float sIn) : Cube(xIn, yIn, wIn, hIn, sIn)
	{
		level = 0;
		score = 0;
		life = 6;
		for (int i = 0; i < 4; i++)
		{
			array[i] = 0;
		}
	}

	void draw()
	{
		glColor3f(0.0, 0.5, 0.0);
		glBegin(GL_POLYGON);
		glVertex3f(x, y, 0.0);
		glVertex3f(x + width, y, 0.0);
		glVertex3f(x + width, y + height, 0.0);
		glVertex3f(x, y + height, 0.0);
		glEnd();
		glFlush();
	}

	float moveIn(float coordinate, float speedIn)
	{
		return coordinate + speedIn;
	}

	void move()
	{
		for (int i = 0; i < 4; i++)
		{
			if (array[i] == 1)
			{
				array[i] = 0;

				if (i == 0)
				{
					x = moveIn(x, -speed);
				}

				if (i == 1)
				{
					x = moveIn(x, +speed);
				}

				if (i == 2)
				{
					y = moveIn(y, -speed);
				}

				if (i == 3)
				{
					y = moveIn(y, +speed);
				}
				if (x <= 0)
				{
					x = 0;
				}
				if (y <= 0)
				{
					y = 0;
				}
				if (x + width >= 480)
				{
					x = 480 - width;
				}
				if (y + height >= 640)
				{
					x = 215;
					y = 95;

					score++;
					level = 1;
				}
			}
		}
	}

	void collision(float xIn, float wIn, float yIn, float hIn) //Carpisma ani
	{
		if ((xIn >= x && xIn <= x + width) || (xIn + wIn >= x && xIn + wIn <= x + width))  // X eksenini ayarliyoruz
		{
			if ((yIn >= y && yIn <= y + height) || (yIn + hIn >= y && yIn + hIn <= y + height))  //y eksenini ayarliyoruz
			{
				life--;  //Eger bunlar olursa cani bir azalacak ve eski koordinata geri donecek
				x = 215;
				y = 95;
				if (life <= 0)
				{
					exit(0);
				}
			}
		}
	}
};

class Enemy :public Cube
{
public:
	float fanLength; //pervanenin uzunlugu
	float fanSpeed; //pervanenin donus hizi
	float fanX; //pervanenin x koordinati
	float fanY;  //pervanenin y koordinati
	float rColor;
	float gColor;
	float bColor;

	Enemy(float xIn, float yIn, float wIn, float hIn, float sIn, float rIn, float gIn, float bIn) : Cube(xIn, yIn, wIn, hIn, sIn)
	{
		fanLength = 48;
		fanSpeed = 2;
		fanX = xIn + wIn / 2;
		fanY = yIn + hIn + +5;
		rColor = rIn;
		gColor = gIn;
		bColor = bIn;
	}

	void levelUp()
	{
		speed *= 1.2; // Oyunu gectikce tum kuplerin hizi artacak
	}

	void move()
	{
		if (speed <= 0)
		{
			if (x >= 0)
			{
				x += speed;
			}
			else {
				speed *= -1;
			}
		}
		else {
			if (x + width <= 480)
			{
				x += speed;
			}
			else {
				speed *= -1;
			}
		}
		fanX = x + width / 2;  //enemy ilerlerken pervane de hareket etsin


	}

	void draw()
	{
		glColor3f(rColor, gColor, bColor);
		glBegin(GL_POLYGON);
		glVertex3f(x, y, 0.0);
		glVertex3f(x + width, y, 0.0);
		glVertex3f(x + width, y + height, 0.0);
		glVertex3f(x, y + height, 0.0);
		glEnd();
		glFlush();

		fanLength += fanSpeed;
		if (fanLength >= 50)
		{
			fanSpeed *= -1;
			fanLength = 48;
		}
		if (fanLength <= 0)
		{
			fanSpeed *= -1;
			fanLength = 2;
		}


		glColor3f(0.0, 0.0, 0.0);  //fan rengi
		glBegin(GL_LINES);
		glVertex2f(fanX - fanLength / 2, fanY);
		glVertex2f(fanX + fanLength / 2, fanY);
		glEnd();
		glFlush();  // faným cizildi ancak henuz donmesi saglanmadi
	}
};

shared_ptr<Player> myCube;
vector<shared_ptr<Enemy>> arr;

void keyboard(unsigned char key, int x, int y)
{
	if (key == 97)
	{
		myCube->array[0] = 1;
	}
	if (key == 100)
	{
		myCube->array[1] = 1;
	}
	if (key == 115)
	{
		myCube->array[2] = 1;
	}
	if (key == 119)
	{
		myCube->array[3] = 1;
	}

	glutPostRedisplay();
}


void render()
{

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0); //arka plan rengi belirlendi
	glBegin(GL_POLYGON);	//dortgen cizimi yapildi
	glVertex3f(0, 0, 0.0);
	glVertex3f(600, 0, 0.0);
	glVertex3f(600, 100, 0.0);
	glVertex3f(0, 100, 0.0);
	glEnd();
	glFlush();   //Dortgen ekrana cizildi


	myCube->move();

	if (myCube->level == 1)
	{
		myCube->level = 0;
		for (int i = 0; i < arr.size(); i++)
		{
			arr[i]->levelUp(); //tum kuplerin leveli artar
		}
	}



	for (int i = 0; i < arr.size(); i++)
	{
		arr[i]->move();  //enemy hareket ettir
		arr[i]->draw();  //enemyleri cizdir
		myCube->collision(arr[i]->x, arr[i]->width, arr[i]->y, arr[i]->height);
	}

	myCube->draw();


	glColor3f(0.0, 0.5, 0.0);
	glRasterPos2f(250, 630); //yazacagin yerin koordinati
	char puantext[5] = { 'P','o','i','n','t' };
	for (int i = 0; i < 5; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, puantext[i]);
	}

	char puan = myCube->score + '0'; //puan chara donustu
	glColor3f(0, 0, 0);
	glRasterPos2f(300, 630);
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, puan); //puanin degerini yazdirma

	glColor3f(0.0, 0.5, 0.0);
	glRasterPos2f(350, 630);
	char lifetext[5] = { 'P','o','w','e','r' };
	for (int i = 0; i < 5; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, lifetext[i]);
	}

	char life = myCube->life + '0';
	glColor3f(0, 0, 0);
	glRasterPos2f(400, 630);
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, life); //canin degerini yazdirma

	glutSwapBuffers();

}


void myInit()
{
	glClearColor(0.0, 1.0, 1.0, 0.0);

	glColor3f(1.0f, 0.0f, 0.0f); //ekran rengi belirlendi

	glMatrixMode(GL_PROJECTION); //projeksiyon perspektifi kazandýrýr

	glOrtho(0.0, 480.0, 0.0, 640.0, -1.0, 1.0); //x 0.0 da baslar 480 de biter , y 0.0 da baslar 640 da biter
}



int main(int argc, char** argv)
{
	myCube = make_shared<Player>(215, 95, 50, 50, 10);  //heapte yer actik
	arr.push_back(make_shared<Enemy>(0, 250, 50, 50, 0.2, 1.0, 0.5, 0.0));
	arr.push_back(make_shared<Enemy>(430, 320, 50, 50, 0.3, 1.0, 0.5, 0.0));
	arr.push_back(make_shared<Enemy>(430, 400, 50, 50, 0.4, 1.0, 0.5, 0.0));
	arr.push_back(make_shared<Enemy>(0, 470, 50, 50, 0.5, 1.0, 0.5, 0.0));

	glutInit(&argc, argv); //Bu fonksiyon cagrilmadan glut kitapligindan hicbir fonksiyon kullanilamaz

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);  //Ekran kartina gonderilen kodlarin nasil render edilecegi soylenir

	glutInitWindowSize(1000, 1000);  //Ekran boyutlari belirlendi

	glutInitWindowPosition(400, 0);  //pencere pozisyonu girildi

	glutCreateWindow("KUP OYUNU"); //olusturulan pencere icinde yeni bir oyun penceresi olusturuldu

	myInit();

	glutKeyboardFunc(keyboard);

	glutDisplayFunc(render); //render fonksiyonunu icine gonderdik

	Timer(0);

	glutMainLoop();

	return 0;
}
