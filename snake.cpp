#include <Windows.h>
#include <iostream>
#include <ctime>
#include <conio.h>
#include <iomanip>
using namespace std;

#define getrand(a, b) (rand() % b + a)  // 获取随机数

int len = 3;  // 蛇的长度
int score = 0;  // 得分
string diff;  // 难度
int speed = 3;  // 蛇的速度(0->3)
bool food = true;  // 现在是否存在食物
int direction[5][2] = { {0, 0},
									{0, -1},  // 上(y轴--)
									{0, 1},  // 下(y轴++)
									{-1, 0},  // 左(x轴--)
									{1, 0} };  // 右(x轴++)
int xl = 25, yl = 16;  // 地图的长和宽(不包括边界)
int d = 2;  // 当前蛇的方向(手工初始化比default构造函数初始化效率高)
COORD head;  // 蛇头坐标，建立结构体COORD对象(表示字符在控制台屏幕上的坐标)
int room[32][32];  // 用于存储地图

void go(int x, int y)  // 光标移动函数
{
	COORD coord;
	coord.X = x * 2;  // 这里将int类型值传给short(COORD数据成员为short)，不过程序中涉及的坐标值均不会超过short范围
	coord.Y = y;
	HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);  // 获得标准输出句柄，句柄(用来标识不同设备的数值)
	SetConsoleCursorPosition(a, coord);  // 以标准输出的句柄为参数设置控制台光标坐标
}

void home(void (*go)(int, int))  // 主界面(传递go函数)
{
	cout << endl;
	cout << setw(15) << "*" << "欢迎来到贪吃蛇!*\n" << endl;
	cout << setw(3) << "\n " << " <难度选择>" << endl;
	cout << setw(6) << " " << "按键[1]：简单难度" << endl;
	cout << setw(6) << " " << "按键[2]：一般难度" << endl;
	cout << setw(6) << " " << "按键[3]：困难难度" << endl;
	for (int i = 0; i <= xl + 1; i++)  // 按照行移动
		for (int j = 0; j <= yl + 1; j++)
			if (i == 0 || i == xl + 1 || j == 0 || j == yl + 1)
			{
				room[i][j] = -1;  // 用-1来表示边界(-1)
				go(i, j);  // 光标移动
				cout << "■";
			}  // 设置边界  打印地图
}

void move()  // 蛇的移动
{
	head.X += direction[d][0];  // 控制台屏幕上的坐标随着按键移动, d值决定
	head.Y += direction[d][1];  // 蛇头向此方向移动(此时并未打印蛇头)
	if (room[head.X][head.Y] != 0 && room[head.X][head.Y] != -2)  // 如果蛇头位置不是空的或食物(-2)
	{
		return;  // 结束函数，不改变room数组
	}
	if (room[head.X][head.Y] == -2)  // 如果蛇头位置是食物
	{
		len++;  // 长度++
		score++;  // 得分++
		food = false;  // 食物状态变为不存在
	}
	for (int i = 1; i < xl + 1; i++)
		for (int j = 1; j < yl + 1; j++)  // 搜索(从1开始，因为0和y1+1为边界(-1))
			if (room[i][j] >= 1)  // 如果此位置有蛇
			{
				room[i][j]--;  // 此位置数值-1，则此位置在蛇中的位置往后变化(即蛇头到蛇尾len逐次递减到1)，蛇尾变为空
				if (room[i][j] == 0)  // 如果此位置是变为空的蛇尾
				{
					go(i, j); cout << "  ";  // 清空
				}
			}
	room[head.X][head.Y] = len;  // 更新移动后蛇的长度(存放在蛇头)
	go(head.X, head.Y);
	cout << "□";  // 打印蛇头
}

void makefood()  // 生成食物
{
	int randx = getrand(1, xl);
	int randy = getrand(1, yl);
	while (room[randx][randy] != 0)  // 地图随机的位置不为空
	{
		randx = getrand(1, xl);
		randy = getrand(1, yl);  // 重新随机
	}
	room[randx][randy] = -2;  // 设置此位置为食物(-2)
	go(randx, randy);  // 调用光标移动函数，在随机位置打印食物
	cout << "·";  // 打印食物
	food = true;  // 食物状态设为状态
}

int main()
{
	system("color 0A");  // console的颜色
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };  // 参数1.光标百分比厚度，2.是否可见
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),  // 参数为一个控制台缓冲区句柄	
											&cursor_info); // 隐藏光标，参数为指向CONSOLE_CURSOR_INFO结构体的指针
	unsigned seed = time(0);
	home(go);  // 主界面
	switch (getch())  // 获取用户按下的字符
	{
	case '1': speed = 1; diff = "简单"; break;
	case '2': speed = 2; diff = "一般"; break;
	case '3': speed = 3; diff = "困难"; break;
	default: cout << "\nERROR!!"; return 0;  // 若不是所选择的难度， 退出游戏
	}
	system("cls");  // 实现清屏操作
	srand(seed);  // 设置随机种子
	for (int i = 0; i <= xl + 1; i++)  // 按照行移动
		for (int j = 0; j <= yl + 1; j++)
			if (i == 0 || i == xl + 1 || j == 0 || j == yl + 1)
			{
				room[i][j] = -1;  // 用-1来表示边界(-1)
				go(i, j);  // 光标移动
				cout << "■";
			}  // 设置边界  打印地图
	makefood();  // 生成第一个食物
	go(xl + 3, 3);  // 控制台定位
	cout << "难度：";
	cout << diff;  // 记录难度
	go(xl + 3, 5);
	cout << "移动：↑↓←→" << endl;
	go(xl + 3, 6);
	cout << "暂停：空格" << endl;
	go(xl + 3, 7);
	cout << "解除暂停：任意键" << endl;
	go(xl + 3, 8);
	cout << "退出游戏：ESC" << endl;
	while (!kbhit()) { Sleep(10); }  // kbhit检测是否有键盘敲击，若无，则使计算机休眠十毫秒(使游戏在未按按键时暂停)
	d = -3;  // 初始化方向为负数使其不与其他方向冲突
	head.X = xl / 2 + 1;
	head.Y = yl / 2 + 1;  // 设置蛇头位置为控制台屏幕中心
	while (1)
	{
		int ld = d;  // 储存未更改的方向
		for (int i  = 1; i <= 20 - speed * 5; i++)  // 分多个时间点检测方向，而不是只有一个时间点，这样可以提升操作手感(speed数值越低，蛇的运行速度越快)
		{
			if (kbhit())  // 如果有按键按下(属于conio头文件)
			{
				int key = 0;
				key = getch();  // 读取(用户按下某个字符，getch自动读取)，属于conio头文件
				if (key == 224)  // 读取方向键时需读取两次，第一次的返回值必定是224(0xE0)
				{
					key = getch();  // 若为方向键则再读取一次(第二次调用), 返回实际的键值
					if (key == 72) d = 1;  // 上，d的值为蛇的方向,
					else if (key == 80) d = 2; // 下
					else if (key == 75) d = 3; // 左
					else if (key == 77) d = 4; // 右
				}
				else if (key == 32)  // 若按键为空格则暂停
				{
					go(0, yl + 3);
					system("pause");
				}
				else if (key == 27)  // 若按键为esc则退出游戏
				{
					return 0;
				}
			}
			Sleep(10);  // 使计算机休眠10毫秒
		}
		if (ld != d && (ld + d == 3 || ld + d == 7))  // 如果更改后的方向与原来的方向冲突(成反方向) ，上下键d相加为3，左右键d相加7，逻辑&&第一个条件(即按下相同按键就跳过判断)
			d = ld;  // 不改变方向
		move();  // 移动蛇
		go(xl + 3, 2);
		cout << "得分：";
		cout << score;  // 记录得分
		if (room[head.X][head.Y] != len) // 如果蛇头位置的数不等于蛇的长度（说明移动函数中判定为撞到边界或者蛇身，未改变蛇头数值）
		{
			go(xl / 2 - 1, yl / 2);  // 将游戏结束提示符定位到屏幕中心
			cout << "Game Over!!";
			go(0, yl + 3);  // 使结束反馈不遮挡画面
			Sleep(250);
			return 0;  // 结束程序
		}
		if (food == false) makefood();  // 如果场上没有食物，则生成一个食物
	}
	return 0;
}
