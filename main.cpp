#include <iostream>
#include <string>
#include <conio.h>
#include <stdio.h>
#include <Windows.h>
#include <thread>
/*
full block - 219
3/4 block - 178
1/2 block - 177
1/4 block - 176
*/

const char* map =
"#                                                                                                                      #"
"#                         #####                                                                                        #"
"#                                                                                                                      #"
"#                                                                                                                      #"
"#        #########                                     ####          ####                                        ####  #"
"#                                                                                                                      #"
"#                                                                                                                      #"
"#                                                                                                                      #"
"#                                                                                                                      #"
"#                                       ####                                                                           #"
"#                                                                                  ##          ################        #"
"#                                                                                                                      #"
"#                                                                                                                      #"
"#                                                                                                                      #"
"#                                                                                                                      #"
"#       ################                                                                                               #"
"#                                                                                                                      #"
"#                                                      ##############                                                  #"
"#                                                                                                                      #"
"#                                                                                                                      #"
"#                                                                                                                      #"
"#                                                                                                                      #"
"#                          ###############                                                                             #"
"#                                                                                                                      #"
"#                                                                                                                      #"
"#                                                                  ############                                        #"
"#                                                                                                                      #"
"#                                                                                                                      #"
"#                                                                                                                      #"
"#                                                                                                                      #"
"#                                                                                                                      #"
"#                                                #################                                                     #"
"#                                                                                                                      #"
"#                                                                                                                      #"
"#                                                                                                                      #"
"#                                                                                                                      #"
"#                                                                                                                      #"
"#                                                                                                                      #"
"#                                                                                                                      #"
"#######################################################################################################################";

const unsigned int screenWidth = 120, screenHeight = 40;
float playerX = screenWidth / 2;
float playerY = screenHeight - 2;
float frameTime = 0.f;

int iPlayerX, iPlayerY;

bool grounded;

int fpsLimit = 0;

int main()
{
	using namespace std::literals::chrono_literals;
	char screenBuffer[screenWidth*screenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, 0, CONSOLE_TEXTMODE_BUFFER, 0);
	SetConsoleActiveScreenBuffer(hConsole);
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 8;
	cfi.dwFontSize.Y = 8;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	SetCurrentConsoleFontEx(hConsole, false, &cfi);
	DWORD dwBytesWritten = 0;
	auto t1 = std::chrono::high_resolution_clock::now();
	auto t2 = std::chrono::high_resolution_clock::now();

	float moveY = 0.f;

	while (true)
	{
		//ClearBuffer(screenBuffer);
		strcpy_s(screenBuffer, map);
		//frametime calculation
		t1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> deltaTime = t1 - t2;
		frameTime = deltaTime.count();
		t2 = t1;

		iPlayerX = (int)playerX;
		iPlayerY = (int)playerY;

		//Draw player
		screenBuffer[iPlayerY * screenWidth + iPlayerX] = '8';

		//Ground check
		grounded = screenBuffer[screenWidth*(iPlayerY + 1) + iPlayerX] != ' ';

		//User intput + basic collision detection
		//Left-Right
		if (GetAsyncKeyState('A') & 0x8000 && screenBuffer[screenWidth*iPlayerY + iPlayerX - 1] == ' ')
			playerX -= 50.0f * frameTime;
		if (GetAsyncKeyState('D') & 0x8000 && screenBuffer[screenWidth*iPlayerY + iPlayerX + 1] == ' ')
			playerX += 50.0f * frameTime;
		//Jump
		if (GetAsyncKeyState('W') & 0x8000 && grounded && moveY >= 0 && screenBuffer[screenWidth*(iPlayerY - 1) + iPlayerX] == ' ')
			moveY = -50.f;

		//Gravity
		if (!grounded)
			moveY += 150.f * frameTime;

		if (moveY < 0 || !grounded)
			playerY += moveY * frameTime;

		if (playerY < 0)
			playerY = 0;

		std::string title = "Gameska | X: " + std::to_string(iPlayerX) + " Y: " + std::to_string(iPlayerY) + " | FPS: " + std::to_string((int)(1.f / frameTime));
		SetConsoleTitle(title.data());
		WriteConsoleOutputCharacterA(hConsole, screenBuffer, screenHeight*screenWidth, { 0,0 }, &dwBytesWritten);
		if(fpsLimit)
			std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<float>(1.f / (float)fpsLimit)));
	}
	std::cin.get();
}

void DrawLine(int x0, int y0, int x1, int y1, char* screen)
{
	if (x0 > x1)
	{
		int tx = x0;
		x0 = x1;
		x1 = tx;
	}
	if (y0 > y1)
	{
		int ty = y0;
		y0 = y1;
		y1 = ty;
	}
	float dx = x1 - x0;
	float dy = y1 - y0;
	for (int x = x0; x <= x1; x++)
		screen[(int)std::roundl((dy / dx) * (x - x0) + y0) * screenWidth + x] = 219;
}