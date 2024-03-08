#include <iostream>
#include <windows.h>
using namespace std;

#pragma comment(lib, "winmm.lib")

CRITICAL_SECTION cs;
int backcolor = 1;
int previous_color = 0;
int message_line = 0;

void SetRandomPosition(int k = -1);
void SetRandomBackgroundColor();
DWORD WINAPI PlayMusic(void*);
DWORD WINAPI ShowHeart(void*);
void SetConsoleSize(int, int);
void SetWindowOptions();
int random(int, int);
void ShowGreetings();
void HideCursor();
void ShowNames();
int main();

int main() {
	SetWindowOptions();
	ShowNames();
	ShowGreetings();
	Sleep(INFINITE);
}

DWORD WINAPI PlayMusic(void* param) {
	PlaySoundA("C:\\!Files\\music\\wav\\krasivo.wav", NULL, SND_NODEFAULT | SND_LOOP);
	return 0;
}

DWORD WINAPI ShowHeart(void* param) {
	while (true) {
		EnterCriticalSection(&cs);
		SetRandomPosition();
		SetConsoleTextAttribute(GetStdHandle(-11), backcolor * 16 + random(9, 15));
		if (backcolor == 12) SetConsoleTextAttribute(GetStdHandle(-11), backcolor * 16 + 15);
		int r = rand() % 3;
		if (r == 0) cout << (char)3;
		if (r == 1) cout << (char)15;
		if (r == 2) cout << (char)2;
		LeaveCriticalSection(&cs);
		Sleep(random(15, 50));
	}
	return 0;
}

int random(int min, int max) {
	return rand() % (max - min + 1) + min;
}

void SetRandomPosition(int k) {
	int columns = 55;
	int rows = 18;
	int x = random(0, columns - 1);
	int y = random(0, rows - 2);
	if (k == 0) {
		x = random(1, columns / 2);
		y = random(1, rows / 2);
	}
	else if (k == 1) {
		x = random(2, columns / 7);
		y = random(2, rows - 6);
		message_line = y;
	}
	else if (k == 2) {
		x = random(10, 30);
		y = random(5, rows / 2);
	}
	else if (k == -1) {
		if (y == message_line) y++;
	}

	COORD coord{ x, y };
	SetConsoleCursorPosition(GetStdHandle(-11), coord);
}

void SetRandomBackgroundColor() {
	int normal_colors[] = { 1,2,3,4,5,6 };
	int random_color = normal_colors[rand() % 6];
	while (random_color == previous_color)
		random_color = normal_colors[rand() % 6];

	previous_color = random_color;
	backcolor = random_color;
	int forecolor = backcolor + 8;
	if (rand() % 2) swap(backcolor, forecolor);
	if (rand() % 3 && backcolor != 14) forecolor = 15;
	if (rand() % 4 && backcolor != 14) forecolor = 14;
	int backgroundColor = backcolor * 16 + forecolor;
	SetConsoleTextAttribute(GetStdHandle(-11), backgroundColor);
	system("cls");
}

void SetConsoleSize(int width, int height) {
	HWND console = GetConsoleWindow();
	RECT ConsoleRect;
	GetWindowRect(console, &ConsoleRect);
	MoveWindow(console,
		(GetSystemMetrics(SM_CXSCREEN) - width) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - height) / 2,
		width, height, TRUE);

	COORD coord{ width, height };
	SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = height - 1;
	Rect.Right = width - 1;
	SetConsoleScreenBufferSize(GetStdHandle(-11), coord);
	SetConsoleWindowInfo(GetStdHandle(-11), TRUE, &Rect);
}

void ShowNames() {
	string names[] = { "Ирина!", "Лилия!", "Евгения!" };

	SetRandomBackgroundColor();
	cout << "\n ";
	for (int i = 0; i < 45; i++) {
		setlocale(0, "C");
		cout << (char)219;
		Sleep(35);
	}
	setlocale(0, "");
	for (const auto& name : names) {
		SetRandomBackgroundColor();
		SetRandomPosition(2);
		cout << name;
		Sleep(1970);
	}
}

void ShowGreetings() {
	CreateThread(0, 0, ShowHeart, 0, 0, 0);

	string greetings[] = {
	"С праздником весны!!!",
	"Пусть сбудутся все Ваши мечты!",
	"Пусть каждый день несёт Вам радость и улыбки!",
	"Все мужчины нашей группы желают Вам счастья!",
	"Здоровья и удачи!",
	"Красоты и гармонии!",
	"Семейного благополучия!",
	"Неисчерпаемого вдохновения!",
	"Ппрофессионального и карьерного роста!",
	"Делать ДЗ минимум на 12 баллов!",
	"Приятных эмоций!",
	"Ярких впечатлений!",
	"Новых достижений!",
	"Пусть будет легко на душе!",
	"Будьте всегда весёлыми и позитивными!",
	"Крепко обнимаем!",
	"Мы Вас любим и очень ценим!",
	"Ваш Александр, и вся группа ПВ312! :)"
	};

	for (const auto& greeting : greetings) {
		EnterCriticalSection(&cs);
		SetRandomBackgroundColor();
		SetRandomPosition(1);
		cout << greeting;
		LeaveCriticalSection(&cs);
		Sleep(1950);
	}
	cout << (char)3 << "\n";
}

void HideCursor() {
	CONSOLE_CURSOR_INFO cursor{ 100, false };
	SetConsoleCursorInfo(GetStdHandle(-11), &cursor);
}

void SetWindowOptions() {
	srand((time(0)));
	SetConsoleSize(55, 18);
	HideCursor();
	setlocale(0, "");
	system("title Дорогие и прекрасные! Ирина, Лилия, Евгения! Поздравляю с 8 марта!");
	CreateThread(0, 0, PlayMusic, 0, 0, 0);
	MoveWindow(GetConsoleWindow(), 340, 150, 1000, 1000, TRUE);
	InitializeCriticalSectionAndSpinCount(&cs, 0x00000400);
}