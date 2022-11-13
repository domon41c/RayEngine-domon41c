#include <iostream>
#include <chrono>
#include <Windows.h>
using namespace std;

//Display settings
int DisplayWidth = 120;
int DisplayHeight = 40;
//Camera settings
float CamX = 8.0f;
float CamY = 8.0f;
float CamA = 0.0f;
//Map settings
int MapHeight = 16;
int MapWidth = 16;
//FOV settings
float FOV = 3.14159 / 4.0;
//Deth
float Depth = 16.0f;
int main()
{
	//Display
	wchar_t* display = new wchar_t[DisplayWidth * DisplayHeight];
	HANDLE AConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(AConsole);
	DWORD WriteBytes = 0;

	//Map
	wstring map;
	map += L"################";
	map += L"#..............#";
	map += L"###.......######";
	map += L"#..............#";
	map += L"#......#.......#";
	map += L"#......#.......#";
	map += L"#####..#.......#";
	map += L"#......#.......#";
	map += L"#..............#";
	map += L"#####....####..#";
	map += L"#..............#";
	map += L"#....######....#";
	map += L"#..............#";
	map += L"#...#..........#";
	map += L"#...#..........#";
	map += L"################";
	//chrono
	auto A1 = chrono::system_clock::now();
	auto A2 = chrono::system_clock::now();

	//Engine while
	while(1)
	{
		//Time elapse
		A2 = chrono::system_clock::now();
		chrono::duration<float> TimeElapse = A2 - A1;
		A1 = A2;
		float TimeElapseA = TimeElapse.count();

		//Camera move
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
			CamA -= (1.0f) * TimeElapseA;
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
			CamA += (1.0f) * TimeElapseA;
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
		{
			CamX += sinf(CamA) * 5.0f * TimeElapseA;
			CamY += cosf(CamA) * 5.0f * TimeElapseA;
		}
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
		{
			CamX -= sinf(CamA) * 5.0f * TimeElapseA;
			CamY -= cosf(CamA) * 5.0f * TimeElapseA;
		}
			
		for (int x = 0; x < DisplayWidth; x++)
		{
			float RayAngle = (CamA - FOV / 2.0f) + ((float)x / (float)DisplayWidth) * FOV;
			//Wall
			float WallDistance = 0;
			bool WallCollision = false;
			//View
			float ViewX = sinf(RayAngle);
			float ViewY = cosf(RayAngle);

			while (!WallCollision && WallDistance < Depth)
			{
				WallDistance += 0.1;
				int TestX = (int)(CamX + ViewX * WallDistance);
				int TestY = (int)(CamY + ViewY * WallDistance);
				//Test Ray
				if (TestX < 0 || TestX >= MapWidth || TestY < 0 || TestY >= MapHeight) {
					WallCollision = true;
					WallDistance = Depth;
				}
				else {
					if (map[TestY * MapWidth + TestX] == '#') {
						WallCollision = true;
					}
				}
			}
			//Distance to Floor
			int Ceiling = (float)(DisplayHeight / 2.0) - DisplayHeight / ((float)WallDistance);
			int Floor = DisplayHeight - Ceiling;
			short Shader = ' ';

			if (WallDistance <= Depth / 4.0f)			Shader = 0x2588;
			else if (WallDistance < Depth / 3.0f)		Shader = 0x2593;
			else if (WallDistance < Depth / 2.0f)		Shader = 0x2592;
			else if (WallDistance < Depth)				Shader = 0x2591;
			else                                        Shader = ' ';

			for (int y = 0; y < DisplayHeight; y++)
			{
				if (y < Ceiling)
					display[y * DisplayWidth + x] = ' ';
				else if(y > Ceiling && y <= Floor)
					display[y * DisplayWidth + x] = Shader;
				else
					display[y * DisplayWidth + x] = ' ';
			}
		}
		display[DisplayWidth * DisplayHeight - 1] = '\0';
		WriteConsoleOutputCharacter(AConsole, display, DisplayWidth * DisplayHeight, { 0,0 }, &WriteBytes);
	}
	return 0;
}