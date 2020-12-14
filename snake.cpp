#include <Windows.h>
#include <iostream>
#include <ctime>
#include <conio.h>
#include <iomanip>
using namespace std;

#define getrand(a, b) (rand() % b + a)  // ��ȡ�����

int len = 3;  // �ߵĳ���
int score = 0;  // �÷�
string diff;  // �Ѷ�
int speed = 3;  // �ߵ��ٶ�(0->3)
bool food = true;  // �����Ƿ����ʳ��
int direction[5][2] = { {0, 0},
									{0, -1},  // ��(y��--)
									{0, 1},  // ��(y��++)
									{-1, 0},  // ��(x��--)
									{1, 0} };  // ��(x��++)
int xl = 25, yl = 16;  // ��ͼ�ĳ��Ϳ�(�������߽�)
int d = 2;  // ��ǰ�ߵķ���(�ֹ���ʼ����default���캯����ʼ��Ч�ʸ�)
COORD head;  // ��ͷ���꣬�����ṹ��COORD����(��ʾ�ַ��ڿ���̨��Ļ�ϵ�����)
int room[32][32];  // ���ڴ洢��ͼ

void go(int x, int y)  // ����ƶ�����
{
	COORD coord;
	coord.X = x * 2;  // ���ｫint����ֵ����short(COORD���ݳ�ԱΪshort)�������������漰������ֵ�����ᳬ��short��Χ
	coord.Y = y;
	HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);  // ��ñ�׼�����������(������ʶ��ͬ�豸����ֵ)
	SetConsoleCursorPosition(a, coord);  // �Ա�׼����ľ��Ϊ�������ÿ���̨�������
}

void home(void (*go)(int, int))  // ������(����go����)
{
	cout << endl;
	cout << setw(15) << "*" << "��ӭ����̰����!*\n" << endl;
	cout << setw(3) << "\n " << " <�Ѷ�ѡ��>" << endl;
	cout << setw(6) << " " << "����[1]�����Ѷ�" << endl;
	cout << setw(6) << " " << "����[2]��һ���Ѷ�" << endl;
	cout << setw(6) << " " << "����[3]�������Ѷ�" << endl;
	for (int i = 0; i <= xl + 1; i++)  // �������ƶ�
		for (int j = 0; j <= yl + 1; j++)
			if (i == 0 || i == xl + 1 || j == 0 || j == yl + 1)
			{
				room[i][j] = -1;  // ��-1����ʾ�߽�(-1)
				go(i, j);  // ����ƶ�
				cout << "��";
			}  // ���ñ߽�  ��ӡ��ͼ
}

void move()  // �ߵ��ƶ�
{
	head.X += direction[d][0];  // ����̨��Ļ�ϵ��������Ű����ƶ�, dֵ����
	head.Y += direction[d][1];  // ��ͷ��˷����ƶ�(��ʱ��δ��ӡ��ͷ)
	if (room[head.X][head.Y] != 0 && room[head.X][head.Y] != -2)  // �����ͷλ�ò��ǿյĻ�ʳ��(-2)
	{
		return;  // �������������ı�room����
	}
	if (room[head.X][head.Y] == -2)  // �����ͷλ����ʳ��
	{
		len++;  // ����++
		score++;  // �÷�++
		food = false;  // ʳ��״̬��Ϊ������
	}
	for (int i = 1; i < xl + 1; i++)
		for (int j = 1; j < yl + 1; j++)  // ����(��1��ʼ����Ϊ0��y1+1Ϊ�߽�(-1))
			if (room[i][j] >= 1)  // �����λ������
			{
				room[i][j]--;  // ��λ����ֵ-1�����λ�������е�λ������仯(����ͷ����βlen��εݼ���1)����β��Ϊ��
				if (room[i][j] == 0)  // �����λ���Ǳ�Ϊ�յ���β
				{
					go(i, j); cout << "  ";  // ���
				}
			}
	room[head.X][head.Y] = len;  // �����ƶ����ߵĳ���(�������ͷ)
	go(head.X, head.Y);
	cout << "��";  // ��ӡ��ͷ
}

void makefood()  // ����ʳ��
{
	int randx = getrand(1, xl);
	int randy = getrand(1, yl);
	while (room[randx][randy] != 0)  // ��ͼ�����λ�ò�Ϊ��
	{
		randx = getrand(1, xl);
		randy = getrand(1, yl);  // �������
	}
	room[randx][randy] = -2;  // ���ô�λ��Ϊʳ��(-2)
	go(randx, randy);  // ���ù���ƶ������������λ�ô�ӡʳ��
	cout << "��";  // ��ӡʳ��
	food = true;  // ʳ��״̬��Ϊ״̬
}

int main()
{
	system("color 0A");  // console����ɫ
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };  // ����1.���ٷֱȺ�ȣ�2.�Ƿ�ɼ�
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),  // ����Ϊһ������̨���������	
											&cursor_info); // ���ع�꣬����Ϊָ��CONSOLE_CURSOR_INFO�ṹ���ָ��
	unsigned seed = time(0);
	home(go);  // ������
	switch (getch())  // ��ȡ�û����µ��ַ�
	{
	case '1': speed = 1; diff = "��"; break;
	case '2': speed = 2; diff = "һ��"; break;
	case '3': speed = 3; diff = "����"; break;
	default: cout << "\nERROR!!"; return 0;  // ��������ѡ����Ѷȣ� �˳���Ϸ
	}
	system("cls");  // ʵ����������
	srand(seed);  // �����������
	for (int i = 0; i <= xl + 1; i++)  // �������ƶ�
		for (int j = 0; j <= yl + 1; j++)
			if (i == 0 || i == xl + 1 || j == 0 || j == yl + 1)
			{
				room[i][j] = -1;  // ��-1����ʾ�߽�(-1)
				go(i, j);  // ����ƶ�
				cout << "��";
			}  // ���ñ߽�  ��ӡ��ͼ
	makefood();  // ���ɵ�һ��ʳ��
	go(xl + 3, 3);  // ����̨��λ
	cout << "�Ѷȣ�";
	cout << diff;  // ��¼�Ѷ�
	go(xl + 3, 5);
	cout << "�ƶ�����������" << endl;
	go(xl + 3, 6);
	cout << "��ͣ���ո�" << endl;
	go(xl + 3, 7);
	cout << "�����ͣ�������" << endl;
	go(xl + 3, 8);
	cout << "�˳���Ϸ��ESC" << endl;
	while (!kbhit()) { Sleep(10); }  // kbhit����Ƿ��м����û������ޣ���ʹ���������ʮ����(ʹ��Ϸ��δ������ʱ��ͣ)
	d = -3;  // ��ʼ������Ϊ����ʹ�䲻�����������ͻ
	head.X = xl / 2 + 1;
	head.Y = yl / 2 + 1;  // ������ͷλ��Ϊ����̨��Ļ����
	while (1)
	{
		int ld = d;  // ����δ���ĵķ���
		for (int i  = 1; i <= 20 - speed * 5; i++)  // �ֶ��ʱ����ⷽ�򣬶�����ֻ��һ��ʱ��㣬�����������������ָ�(speed��ֵԽ�ͣ��ߵ������ٶ�Խ��)
		{
			if (kbhit())  // ����а�������(����conioͷ�ļ�)
			{
				int key = 0;
				key = getch();  // ��ȡ(�û�����ĳ���ַ���getch�Զ���ȡ)������conioͷ�ļ�
				if (key == 224)  // ��ȡ�����ʱ���ȡ���Σ���һ�εķ���ֵ�ض���224(0xE0)
				{
					key = getch();  // ��Ϊ��������ٶ�ȡһ��(�ڶ��ε���), ����ʵ�ʵļ�ֵ
					if (key == 72) d = 1;  // �ϣ�d��ֵΪ�ߵķ���,
					else if (key == 80) d = 2; // ��
					else if (key == 75) d = 3; // ��
					else if (key == 77) d = 4; // ��
				}
				else if (key == 32)  // ������Ϊ�ո�����ͣ
				{
					go(0, yl + 3);
					system("pause");
				}
				else if (key == 27)  // ������Ϊesc���˳���Ϸ
				{
					return 0;
				}
			}
			Sleep(10);  // ʹ���������10����
		}
		if (ld != d && (ld + d == 3 || ld + d == 7))  // ������ĺ�ķ�����ԭ���ķ����ͻ(�ɷ�����) �����¼�d���Ϊ3�����Ҽ�d���7���߼�&&��һ������(��������ͬ�����������ж�)
			d = ld;  // ���ı䷽��
		move();  // �ƶ���
		go(xl + 3, 2);
		cout << "�÷֣�";
		cout << score;  // ��¼�÷�
		if (room[head.X][head.Y] != len) // �����ͷλ�õ����������ߵĳ��ȣ�˵���ƶ��������ж�Ϊײ���߽��������δ�ı���ͷ��ֵ��
		{
			go(xl / 2 - 1, yl / 2);  // ����Ϸ������ʾ����λ����Ļ����
			cout << "Game Over!!";
			go(0, yl + 3);  // ʹ�����������ڵ�����
			Sleep(250);
			return 0;  // ��������
		}
		if (food == false) makefood();  // �������û��ʳ�������һ��ʳ��
	}
	return 0;
}
