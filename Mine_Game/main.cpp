#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <graphics.h>    //图形界面库
#pragma comment(lib, "winmm.lib")           //导入winmm库，对多媒体编程，可以加入背景音乐等


#define ROW 50   //行
#define COL 30   //列
#define MINE 150 //雷的数量
#define SIZE 20 //图片尺寸

int map[ROW + 2][COL + 2];
IMAGE img[12];     //img存放12张图片

//打印数组
//9 雷   10 空白    11标记
/****************************************
		元素      图片
		0-8       数字         +20   20-28
		-1        imh[9]         +20   19    雷
		19-28     img[10]                    空白
		>30       img[11]                    标记
*****************************************/

//ROW * COL - MINE = count 时赢得游戏
int count = 0;     //点开的个数

//游戏界面的绘制
void GameDraw()
{
	for (int i = 1; i < ROW + 1; i++)
	{
		for (int j = 1; j < COL + 1; j++)
		{
			printf("%2d ", map[i][j]);
			if (map[i][j] == -1)              //雷
			{
				putimage((i-1) * SIZE, (j-1) * SIZE, &img[9]); //以坐标形式，所以为i/j - 1 
			}
			else if (map[i][j] >= 0 && map[i][j] <= 8)
			{
				putimage((i-1 ) * SIZE, (j -1) * SIZE, &img[map[i][j]]);
			}
			else if (map[i][j] >= 19 && map[i][j] <= 28)
			{
				putimage((i-1 ) * SIZE, (j-1 ) * SIZE, &img[10]); // 空白
			}
			else if (map[i][j] > 30)
			{
				putimage((i-1 ) * SIZE, (j -1) * SIZE, &img[11]);  //标记图片
			}
		}
		printf("\n");
	}
}

//数组初始化
void GameIint()
{
	//随机数种子
	srand(time(NULL));
	for (int i = 0; i < ROW + 2; i++)    //增加辅助行列使游戏区每行列均有九宫格
	{
		for (int j = 0; j < COL + 2; j++)
		{
			map[i][j] = 0;
		}
	}
	//雷  -1表示雷
	int n = 0;
	while (n < MINE)
	{
		//随机得到行和列埋雷
		int r = rand() % ROW + 1;   // 雷要在1-ROW行
		int c = rand() % COL + 1;
		if (map[r][c] != -1)
		{
			map[r][c] = -1;
			n++;
		}
		
	}
	//遍历二位数组的游戏区 找到不是雷的
	for (int i = 1; i <= ROW; i ++)
	{
		for (int j = 1; j <= COL; j ++)
		{
			if (map[i][j] != -1)   //遍历不是雷的九宫格
			{
				for (int m = i - 1; m <= i + 1; m ++)
				{
					for (int k = j - 1; k <= j + 1; k ++)
					{
						if (map[m][k] == -1)       //找到一个雷
						{
							map[i][j] ++;
						}
					}
				}
			}
		}
	}
	//加密，使得游戏区全部变为空白块 -1+20 = 19   0-8 + 20 = 20-28
	for (int i = 1; i <= ROW; i++)
	{
		for (int j = 1; j <= COL; j++)
		{
			map[i][j] += 20;
		}
	}
}

//打开0,周围无雷,翻开多个
void OpenZero(int r, int c)                 
{
	//先打开这个0
	map[r][c] -= 20;
	count++;
	for (int m = r - 1; m <= r + 1; m++)          //遍历0周围九宫格
	{
		for (int n = c - 1; n <= c + 1; n++)
		{
			if (m >= 1 && m <= ROW && n >= 1 && n <= COL)
			{
				if (map[m][n] >= 19 && map[m][n] <= 28)
				{
					if (map[m][n] == 20)             //若又翻到0，则递归调用
					{
						OpenZero(m, n);
					}
					else                             //否则翻开，count++
					{
						map[m][n] -= 20;
						count++;
					}
				} 
			}
		}
	}
}

//游戏函数
int PlayGame()
{
	//定义一个鼠标信息
	MOUSEMSG msg = { 0 };
	int r, c;    //鼠标点击的行和列
	while (1)
	{
		msg = GetMouseMsg();
		switch (msg.uMsg)
		{
			case WM_LBUTTONDOWN://左键，翻开扫雷
			{	r = msg.x / SIZE + 1;     //msg.x/y是当前游戏区坐标，要考虑有辅助区
				c = msg.y / SIZE + 1;
				if (map[r][c] >= 19 && map[r][c] <= 28)    //进行解密，翻开此处
				{
					if (map[r][c] == 20)    //点到的是0
					{
						OpenZero(r, c);
					}
					else 
					{
						map[r][c] -= 20;
						count++;
					}
				}
				return map[r][c];
			}break;
			case WM_RBUTTONDOWN:          //右键 标记空白图片，取消一个标记
			{	r = msg.x / SIZE + 1;     //msg.x/y是当前游戏区坐标，要考虑有辅助区
				c = msg.y/ SIZE + 1;
				if (map[r][c] >= 19 && map[r][c] <= 28)
				{
					map[r][c] += 50;               // >30
				}
				else if (map[r][c] > 30)       //取消标记
				{
					map[r][c] -= 50;
				}
				return map[r][c];
			}	break;
		} 
	}
}



int main()
{
	HWND hwnd = initgraph(ROW * SIZE, COL * SIZE, SHOWCONSOLE); //控制台显示出来

	mciSendString("open 周杰伦-兰亭序.mp3 alias music", 0, 0, 0);     //alias 用music代替文件名，
	mciSendString("play music", 0, 0, 0);                            //play + 文件名（music)

        //50 * 50大小的图片
		loadimage(&img[0], "0.jpg", SIZE, SIZE);  //字符集为unicode时，字符前要加L
		loadimage(&img[1], "1.jpg", SIZE, SIZE);
		loadimage(&img[2], "2.jpg", SIZE, SIZE);
		loadimage(&img[3], "3.jpg", SIZE, SIZE);
		loadimage(&img[4], "4.jpg", SIZE, SIZE);
		loadimage(&img[5], "5.jpg", SIZE, SIZE);
		loadimage(&img[6], "6.jpg", SIZE, SIZE);
		loadimage(&img[7], "7.jpg", SIZE, SIZE);
		loadimage(&img[8], "8.jpg", SIZE, SIZE);
		loadimage(&img[9], "9.jpg", SIZE, SIZE);
		loadimage(&img[10], "10.jpg", SIZE, SIZE);
		loadimage(&img[11], "11.jpg", SIZE, SIZE);
	GameIint();
	
	while (1)
	{
		GameDraw();
		if (PlayGame() == -1)   //游戏结束，弹出提示框
		{
			GameDraw();
			MessageBox(hwnd, "GAME OVER!", "", MB_OK);
			break;
		}
		if (ROW * COL - MINE == count)
		{
			GameDraw();
			MessageBox(hwnd, "You are win!", "", MB_OK );
			break;
		}
	}
	closegraph();           //关闭图形界面
	return 0;
}