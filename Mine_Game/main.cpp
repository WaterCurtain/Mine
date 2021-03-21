#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <graphics.h>    //ͼ�ν����
#pragma comment(lib, "winmm.lib")           //����winmm�⣬�Զ�ý���̣����Լ��뱳�����ֵ�


#define ROW 50   //��
#define COL 30   //��
#define MINE 150 //�׵�����
#define SIZE 20 //ͼƬ�ߴ�

int map[ROW + 2][COL + 2];
IMAGE img[12];     //img���12��ͼƬ

//��ӡ����
//9 ��   10 �հ�    11���
/****************************************
		Ԫ��      ͼƬ
		0-8       ����         +20   20-28
		-1        imh[9]         +20   19    ��
		19-28     img[10]                    �հ�
		>30       img[11]                    ���
*****************************************/

//ROW * COL - MINE = count ʱӮ����Ϸ
int count = 0;     //�㿪�ĸ���

//��Ϸ����Ļ���
void GameDraw()
{
	for (int i = 1; i < ROW + 1; i++)
	{
		for (int j = 1; j < COL + 1; j++)
		{
			printf("%2d ", map[i][j]);
			if (map[i][j] == -1)              //��
			{
				putimage((i-1) * SIZE, (j-1) * SIZE, &img[9]); //��������ʽ������Ϊi/j - 1 
			}
			else if (map[i][j] >= 0 && map[i][j] <= 8)
			{
				putimage((i-1 ) * SIZE, (j -1) * SIZE, &img[map[i][j]]);
			}
			else if (map[i][j] >= 19 && map[i][j] <= 28)
			{
				putimage((i-1 ) * SIZE, (j-1 ) * SIZE, &img[10]); // �հ�
			}
			else if (map[i][j] > 30)
			{
				putimage((i-1 ) * SIZE, (j -1) * SIZE, &img[11]);  //���ͼƬ
			}
		}
		printf("\n");
	}
}

//�����ʼ��
void GameIint()
{
	//���������
	srand(time(NULL));
	for (int i = 0; i < ROW + 2; i++)    //���Ӹ�������ʹ��Ϸ��ÿ���о��оŹ���
	{
		for (int j = 0; j < COL + 2; j++)
		{
			map[i][j] = 0;
		}
	}
	//��  -1��ʾ��
	int n = 0;
	while (n < MINE)
	{
		//����õ��к�������
		int r = rand() % ROW + 1;   // ��Ҫ��1-ROW��
		int c = rand() % COL + 1;
		if (map[r][c] != -1)
		{
			map[r][c] = -1;
			n++;
		}
		
	}
	//������λ�������Ϸ�� �ҵ������׵�
	for (int i = 1; i <= ROW; i ++)
	{
		for (int j = 1; j <= COL; j ++)
		{
			if (map[i][j] != -1)   //���������׵ľŹ���
			{
				for (int m = i - 1; m <= i + 1; m ++)
				{
					for (int k = j - 1; k <= j + 1; k ++)
					{
						if (map[m][k] == -1)       //�ҵ�һ����
						{
							map[i][j] ++;
						}
					}
				}
			}
		}
	}
	//���ܣ�ʹ����Ϸ��ȫ����Ϊ�հ׿� -1+20 = 19   0-8 + 20 = 20-28
	for (int i = 1; i <= ROW; i++)
	{
		for (int j = 1; j <= COL; j++)
		{
			map[i][j] += 20;
		}
	}
}

//��0,��Χ����,�������
void OpenZero(int r, int c)                 
{
	//�ȴ����0
	map[r][c] -= 20;
	count++;
	for (int m = r - 1; m <= r + 1; m++)          //����0��Χ�Ź���
	{
		for (int n = c - 1; n <= c + 1; n++)
		{
			if (m >= 1 && m <= ROW && n >= 1 && n <= COL)
			{
				if (map[m][n] >= 19 && map[m][n] <= 28)
				{
					if (map[m][n] == 20)             //���ַ���0����ݹ����
					{
						OpenZero(m, n);
					}
					else                             //���򷭿���count++
					{
						map[m][n] -= 20;
						count++;
					}
				} 
			}
		}
	}
}

//��Ϸ����
int PlayGame()
{
	//����һ�������Ϣ
	MOUSEMSG msg = { 0 };
	int r, c;    //��������к���
	while (1)
	{
		msg = GetMouseMsg();
		switch (msg.uMsg)
		{
			case WM_LBUTTONDOWN://���������ɨ��
			{	r = msg.x / SIZE + 1;     //msg.x/y�ǵ�ǰ��Ϸ�����꣬Ҫ�����и�����
				c = msg.y / SIZE + 1;
				if (map[r][c] >= 19 && map[r][c] <= 28)    //���н��ܣ������˴�
				{
					if (map[r][c] == 20)    //�㵽����0
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
			case WM_RBUTTONDOWN:          //�Ҽ� ��ǿհ�ͼƬ��ȡ��һ�����
			{	r = msg.x / SIZE + 1;     //msg.x/y�ǵ�ǰ��Ϸ�����꣬Ҫ�����и�����
				c = msg.y/ SIZE + 1;
				if (map[r][c] >= 19 && map[r][c] <= 28)
				{
					map[r][c] += 50;               // >30
				}
				else if (map[r][c] > 30)       //ȡ�����
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
	HWND hwnd = initgraph(ROW * SIZE, COL * SIZE, SHOWCONSOLE); //����̨��ʾ����

	mciSendString("open �ܽ���-��ͤ��.mp3 alias music", 0, 0, 0);     //alias ��music�����ļ�����
	mciSendString("play music", 0, 0, 0);                            //play + �ļ�����music)

        //50 * 50��С��ͼƬ
		loadimage(&img[0], "0.jpg", SIZE, SIZE);  //�ַ���Ϊunicodeʱ���ַ�ǰҪ��L
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
		if (PlayGame() == -1)   //��Ϸ������������ʾ��
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
	closegraph();           //�ر�ͼ�ν���
	return 0;
}