#pragma warning(disable:4996)
#include<conio.h>
#include<Windows.h>
#include <ctime>
#include <iostream>
#include <vector>
#include "Snake.h"
using namespace std;

// 設定遊戲格子大小
const int spaceHeight = 16;
const int spaceWidth = 16;

// 建立遊戲分數與難度
int score = 0;
int level = 1;

// 更新遊戲格子畫面
void refreshWindow(vector<vector<char> > space)
{
	system("cls");
	for (int i = 0; i < spaceHeight; i++)
	{
		for (int j = 0; j < spaceWidth; j++)
		{
			cout << space[i][j];
		}
		cout << endl;
	}
	cout << endl << "難度: " << level << endl;
	cout << endl << "分數: " << score << endl;
}

// 更新食物位置
vector<int> getNewFoodPos()
{
	// 只取上下左右各1格以外的格子
	srand((unsigned int)time(0));
	int y = (rand() % (spaceHeight - 2)) + 1;
	int x = (rand() % (spaceWidth - 2)) + 1;
	return { y, x };
}

int main()
{
	// 說明遊戲規則
	cout << "規則: 蛇撞到牆壁以及蛇身及失敗" << endl
		<< "分數計算:每吃一個食物獲得10分" << endl
		<< "開始遊戲請按1" << endl;

	// 等待使用者輸入
	while (true)
	{
		if (kbhit())
		{
			char ch = getch();
			if (ch == '1') break;
		}
	}

	// 建立遊戲格子陣列與蛇初始參數
	vector<vector<char>> space(spaceHeight, vector<char>(spaceWidth, 0));
	for (int j = 0; j < spaceWidth; j++)
	{
		space[0][j] = '#';
		space[spaceHeight - 1][j] = '#';
	}
	for (int i = 1; i < spaceHeight - 1; i++)
	{
		space[i][0] = '#';
		space[i][spaceWidth - 1] = '#';
	}

	// 建立蛇節陣列，並初始化位置
	vector<Snake> snakeVec;
	snakeVec.push_back(Snake({ 1, 1 }, { 1, 0 }));

	// 建立方向陣列，用來記錄鍵盤指定方向
	vector<vector<int>> dirVec;
	
	// 建立食物位置
	vector<int> foodPos = getNewFoodPos();

	// 開始遊戲
	clock_t timeBegin = clock();
	while (true)
	{
		// 遊戲主要判斷區塊
		if (clock() - timeBegin >= 250 - 50 * (level - 1))
		{
			timeBegin = clock();
			vector<vector<char>> newSpace = vector<vector<char>>(space);
			vector<vector<int>> newDirVec;

			// 改變每蛇節方向
			for (int i = 0; i < (int)dirVec.size(); i++)
			{
				// 取得蛇節 index
				int index = dirVec[i][(int)dirVec[i].size() - 1];
				
				// 取得方向
				vector<int> dir = vector<int>(dirVec[i]);
				dir.pop_back(); // 移除 index

				if (index < (int)snakeVec.size())
				{
					// 一次只改變一蛇節方向
					snakeVec[index].setDir(dir);

					// 增加蛇節 index
					dirVec[i][2]++; 

					// 保留方向（排除該方向已經被所有蛇節設定完），下一輪繼續設定
					if (dirVec[i][2] < (int)snakeVec.size())
					{
						newDirVec.push_back(dirVec[i]);
					}
				}
			}
			dirVec = newDirVec;

			// 將每蛇節設定在 space 上
			for (int i = 0; i < (int)snakeVec.size(); i++)
			{
				// 蛇移動一步
				snakeVec[i].move();
				newSpace[snakeVec[i].getPos()[0]][snakeVec[i].getPos()[1]] = 'X';
			}

			// 取得蛇頭位置
			int y = snakeVec[0].getPos()[0];
			int x = snakeVec[0].getPos()[1];
			
			// 確認是否遊戲失敗 (撞到牆壁)
			if (y == 0 || y == spaceHeight-1 || x == 0 || x == spaceWidth-1) break;

			// 確認是否遊戲失敗 (撞到蛇身)
			bool gameover = false;
			for (int i = 1; i < (int)snakeVec.size(); i++)
			{
				int y2 = snakeVec[i].getPos()[0];
				int x2 = snakeVec[i].getPos()[1];
				if (y == y2 && x == x2)
				{
					gameover = true;
					break;
				}
			}
			if (gameover == true) break;

			// 確認是否吃到食物，吃到才會進入
			if (y == foodPos[0] && x == foodPos[1])
			{
				// 取得最後一蛇節位置
				vector<int> pos = snakeVec[(int)snakeVec.size() - 1].getPos();
				vector<int> dir = snakeVec[(int)snakeVec.size() - 1].getDir();
				pos[0] -= dir[0];
				pos[1] -= dir[1];

				// 增加蛇節
				snakeVec.push_back(Snake(pos, dir));

				// 更新食物位置
				foodPos = getNewFoodPos();

				// 增加分數
				score += 10;

				// 累計增加40分就增加難度
				if (score % 40 == 0 && level < 5) level++;
			}

			// 將食物設定在 space 上
			newSpace[foodPos[0]][foodPos[1]] = '$';

			// 更新遊戲格子畫面
			refreshWindow(newSpace);
		}

		// 使用者有按鍵盤才會進去
		if (kbhit())
		{
			char ch = getch();
			if (ch == 72 || ch == 80 || ch == 75 || ch == 77)
			{
				vector<int> dir; // {dirY, dirX, index}
				if (ch == 72) // UP
					dir = { -1, 0 };
				else if (ch == 80) // DOWN
					dir = { 1, 0 };
				else if (ch == 75) // LEFT
					dir = { 0, -1 };
				else if (ch == 77) // RIGHT
					dir = { 0, 1 };
				dir.push_back(0);
				dirVec.push_back(dir);
			}
		}
	}

	cout << endl << "遊戲結束！" << endl << endl;
	system("pause");
	return 0;
}
