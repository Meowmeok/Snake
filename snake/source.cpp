#include <iostream>
#include <cstdlib>
#include <raylib.h>
#define MAXSIZE 50
#define DISTANCE 8
#define screenWidth 800
#define screenHeight 500 

struct Part
{
	Vector2 sizeOfRec{8, 8};
	Vector2 position;
};

struct Snake
{
	Part part[MAXSIZE];
	int numOfPart;
};

struct Ball
{
	Vector2 centerBall;
	float ballRadius;
};

char *getStr(int n);
void initSnake(Snake &snk);
void initBall(Ball &ball);
bool checkLose(Snake &snk);
int getReverseNumber(int n);
void changeCheck(bool *check);
void addPart(Snake &snk, bool *check);
void makeNewPositionOfPart(Snake &snk);
void checkOutOfScreen(Snake &snk, bool *check);
void changePosition(Vector2 &position, bool *check);
void getPoint(Snake &snk, Ball &ball, bool &flagBall, bool *check, int &point);

int main()
{
	int point = 0;
	Snake snk;
	Ball ball;
	initSnake(snk);
	initBall(ball);
	bool flagBall = false;
	//up->down->right->left
	bool check[4] = {true, false, false, false};
	InitWindow(screenWidth, screenHeight, "Snake");
	SetTargetFPS(30);
	while(WindowShouldClose() == false)
	{
		changeCheck(check);
		makeNewPositionOfPart(snk);
		changePosition(snk.part[0].position, check);
		if(checkLose(snk)) break;
		BeginDrawing();
		ClearBackground(BLACK);
		for(int i = 0; i < snk.numOfPart; i++)
		{
			DrawRectangleV(snk.part[i].position, snk.part[i].sizeOfRec, WHITE);
		}
		DrawCircle(ball.centerBall.x, ball.centerBall.y, ball.ballRadius, WHITE);				
		getPoint(snk, ball, flagBall, check, point);
		char *str = getStr(point);
		DrawText(str, 750, 20, 50, RED);
		checkOutOfScreen(snk, check);
		delete[] str;
		EndDrawing();
	}
	CloseWindow();
	return 0;
}

void initSnake(Snake &snk)
{
	snk.numOfPart = 4;
	snk.part[0].position = {screenWidth/2, screenHeight/2};
	snk.part[1].position = {screenWidth/2, screenHeight/2 + 1*DISTANCE};
	snk.part[2].position = {screenWidth/2, screenHeight/2 + 2*DISTANCE};
	snk.part[3].position = {screenWidth/2, screenHeight/2 + 3*DISTANCE};
}

void initBall(Ball &ball)
{
	ball.centerBall = {rand()%screenWidth, rand()%screenHeight};
	ball.ballRadius = DISTANCE;
}

void changeCheck(bool *check)
{
	if(IsKeyPressed(KEY_UP))
	{
		check[0] = true;
		check[1] = false;
		check[2] = false;
		check[3] = false;
	}
	else if(IsKeyPressed(KEY_DOWN))
	{
		check[0] = false;
		check[1] = true;
		check[2] = false;
		check[3] = false;
	}
	else if(IsKeyPressed(KEY_RIGHT))
	{
		check[0] = false;
		check[1] = false;
		check[2] = true;
		check[3] = false;			
	}
	else if(IsKeyPressed(KEY_LEFT))
	{
		check[0] = false;
		check[1] = false;
		check[2] = false;
		check[3] = true;			
	}
}

void changePosition(Vector2 &position, bool *check)
{
	if(check[0] == true)
	{
		position.y -= DISTANCE;
	}
	else if(check[1] == true)
	{
		position.y += DISTANCE;
	}
	else if(check[2] == true)
	{
		position.x += DISTANCE;
	}
	else if(check[3] == true)
	{
		position.x -= DISTANCE;
	}
}

void makeNewPositionOfPart(Snake &snk)
{
	for(int i = snk.numOfPart-1; i > 0; i--)
	{
		snk.part[i].position = snk.part[i-1].position;
	}
}

void addPart(Snake &snk, bool *check)
{
	snk.numOfPart++;
	for(int i = snk.numOfPart-1; i > 0; i--)
	{
		snk.part[i].position = snk.part[i-1].position;
	}
	if(check[0])
	{
		snk.part[0].position.y = snk.part[1].position.y - DISTANCE;
	}
	else if(check[1])
	{
		snk.part[0].position.y = snk.part[1].position.y + DISTANCE;		
	}
	else if(check[2])
	{
		snk.part[0].position.x = snk.part[1].position.x + DISTANCE;
		
	}
	else if(check[3])
	{
		snk.part[0].position.x = snk.part[1].position.x - DISTANCE;		
	}
}

bool checkLose(Snake &snk)
{
	Rectangle rec1{snk.part[0].position.x, snk.part[0].position.y, snk.part[0].sizeOfRec.x, snk.part[0].sizeOfRec.y};
	for(int i = 1; i < snk.numOfPart; i++)
	{
		Rectangle rec2{snk.part[i].position.x, snk.part[i].position.y, snk.part[i].sizeOfRec.x, snk.part[i].sizeOfRec.y};
		if(CheckCollisionRecs(rec1, rec2))
			return true;
	}
	return false;
}

void getPoint(Snake &snk, Ball &ball, bool &flagBall, bool *check, int &point)
{
	for(int i = 0; i < snk.numOfPart; i++)
	{
		Rectangle temp{snk.part[i].position.x, snk.part[i].position.y, snk.part[i].sizeOfRec.x, snk.part[i].sizeOfRec.y};
		if(CheckCollisionCircleRec(ball.centerBall, ball.ballRadius, temp))
		{
			flagBall = true;
			break;
		}
	}
	if(flagBall == true)
	{
		point++;
		initBall(ball);
		addPart(snk, check);
		flagBall = false;
	}
}

void checkOutOfScreen(Snake &snk, bool *check)
{
	if(check[0] || check[1])
	{
		if(snk.part[0].position.y >= screenHeight)
		{
			snk.part[0].position.y = 0;
		}
		else if(snk.part[0].position.y <= 0)
		{
			snk.part[0].position.y = screenHeight;
		}
	}
	if(check[2] || check[3])
	{
		if(snk.part[0].position.x >= screenWidth)
		{
			snk.part[0].position.x = 0;
		}
		else if(snk.part[0].position.x <= 0)
		{
			snk.part[0].position.x = screenWidth;
		}
	}
}

char *getStr(int n)
{
	int count = 0;
	int reverseN = getReverseNumber(n);
	char *str = new char[10];
	if(n == 0)
	{
		str[0] = '0';
		str[1] = '\0';
		return str;
	}
	while(reverseN > 0)
	{
		str[count++] = (reverseN % 10) + '0';
		reverseN /= 10;
	}
	str[count] = '\0';
	return str;
}

int getReverseNumber(int n)
{
	int result = 0;
	while(n > 0)
	{
		result = result*10 + n%10;
		n /= 10;
	}
	return result;
}