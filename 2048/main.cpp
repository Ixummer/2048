#include <graphics.h>      // 引用图形库头文件
#include <conio.h>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

const int picNums = 12;
IMAGE num[picNums];
int numWidth, numHeight;

int board[4][4];
int score = 0;
int max = 0;

void drawGame();
void displayScore();
void goUp();
void goDown();
void goLeft();
void goRight();
void init();
void loadPicture();
void randomProduct();
void restartGame();
void solve2048merge(int cellsNum, int *solve, int pZero, int pNotZero, int pNotZero2);
void solve2048(int *p1, int *p2, int *p3, int *p4);
void setTextStyle();
void updateWithInput();

int main(){
	init();
	while (true) {
		displayScore();
		drawGame();
		updateWithInput();
	}
	_getch();               
	closegraph();         
}

void loadPicture(){
	loadimage(&num[0], _T("pic\\0.jpg"));
	loadimage(&num[1], _T("pic\\2.jpg"));
	loadimage(&num[2], _T("pic\\4.jpg"));
	loadimage(&num[3], _T("pic\\8.jpg"));
	loadimage(&num[4], _T("pic\\16.jpg"));
	loadimage(&num[5], _T("pic\\32.jpg"));
	loadimage(&num[6], _T("pic\\64.jpg"));
	loadimage(&num[7], _T("pic\\128.jpg"));
	loadimage(&num[8], _T("pic\\256.jpg"));
	loadimage(&num[9], _T("pic\\512.jpg"));
	loadimage(&num[10], _T("pic\\1024.jpg"));
	loadimage(&num[11], _T("pic\\2048.jpg"));
}

void init(){
	loadPicture();
	numWidth = num[0].getwidth();
	numHeight = num[0].getheight();
	initgraph(numWidth * 4, numHeight * 5);

	setbkcolor(RGB(102, 107, 103));
	cleardevice();

	setTextStyle();
	outtextxy(160,  20, _T("Press 'R' to Restart"));


	HWND hwnd = GetHWnd();
	SetWindowText(hwnd, _T("2048"));

	score = 0;
	srand((unsigned int)time(NULL));	

	randomProduct();
	randomProduct();
}

void drawGame(){
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			int whichImg = board[row][col];
			putimage(numWidth * col, numHeight * (row + 1), &num[whichImg]);
		}
	}
}

void moveEffect() {
	//TODO
}

void randomProduct(){
	int time = 256;
	int r;
	while (time--) {
		r = rand() % 16;
		int row = r >> 2;
		int col = r & 3;
		if (board[row][col] == 0) {
			board[row][col] = 1;
			break;
		}
	}
}

void updateWithInput(){
	if (_kbhit()) {
		char input = _getch();
		switch (input) {
		case 'w' | 'W':
			goUp();
			randomProduct();
			break;
		case 's' | 'S':
			goDown();
			randomProduct();
			break;
		case 'a' | 'A':
			goLeft();
			randomProduct();
			break;
		case 'd' | 'D':
			goRight();
			randomProduct();
			break;
		case 'r' | 'R':
			restartGame();
			break;
		default:
			break;
		}
	}
}

void solve2048(int *p1, int *p2, int *p3, int *p4){
	int solve[4];
	solve[0] = *p1;
	solve[1] = *p2;
	solve[2] = *p3;
	solve[3] = *p4;

	int pZero = 0;
	int pNotZero = 0;
	int pNotZero2 = 0;
	int cellsNum = 4;

	do {
		if (solve[4 - cellsNum] > 0) {
			pNotZero = 4 - cellsNum;
			break;
		}
	} while (--cellsNum);
	if (cellsNum > 0) 
		solve2048merge(cellsNum, solve, pZero, pNotZero, pNotZero2);
	else {
		int temp = solve[pNotZero];
		solve[pNotZero] = 0;
		solve[pZero] = temp;
	}

	*p1 = solve[0];
	*p2 = solve[1];
	*p3 = solve[2];
	*p4 = solve[3];
}

void solve2048merge(int cellsNum, int *solve, int pZero, int pNotZero, int pNotZero2){
	if (cellsNum > 0) {
		while (--cellsNum) {
			if (solve[4 - cellsNum] > 0) {
				pNotZero2 = 4 - cellsNum;
				break;
			}
		}

		if (pNotZero2 > pNotZero){
			if (solve[pNotZero2] == solve[pNotZero]) {
				++solve[pNotZero];
				score += 1 << solve[pNotZero];					// 添加分数
				solve[pNotZero2] = 0;
				while (--cellsNum) {
					if (solve[4 - cellsNum] > 0) {
						pNotZero2 = 4 - cellsNum;
						break;
					}
				}
			}
		}
		int temp = solve[pNotZero];
		solve[pNotZero] = 0;
		solve[pZero] = temp;
		++pZero;
		pNotZero = pNotZero2;
		if (cellsNum > 0) 
			solve2048merge(cellsNum, solve, pZero, pNotZero, pNotZero2);
	}
}

void goUp(){
	for (int i = 0; i < 4; ++i) 
		solve2048(&board[0][i], &board[1][i], &board[2][i], &board[3][i]);
}

void goDown(){
	for (int i = 0; i < 4; ++i) 
		solve2048(&board[3][i], &board[2][i], &board[1][i], &board[0][i]);
}

void goLeft(){
	for (int i = 0; i < 4; ++i) 
		solve2048(&board[i][0], &board[i][1], &board[i][2], &board[i][3]);
}

void goRight(){
	for (int i = 0; i < 4; ++i) 
		solve2048(&board[i][3], &board[i][2], &board[i][1], &board[i][0]);
}

void displayScore(){
	char display[6];
	_stprintf_s(display, _T("%d"), score);
	outtextxy(20, 20, _T("Score"));
	outtextxy(50, 80, display);
}

void restartGame(){
	cleardevice();
	score = 0;

	for (int row = 0; row < 4; ++row) 
		for (int col = 0; col < 4; ++col) 
			board[row][col] = 0;

	randomProduct();
	randomProduct();
	outtextxy(160, 20, _T("Press 'R' to Restart"));
}

void setTextStyle(){
	settextcolor(RGB(248, 247, 241));
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 45;
	f.lfQuality = ANTIALIASED_QUALITY;  //抗锯齿
	settextstyle(&f);
}