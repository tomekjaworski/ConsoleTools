/*
 * Simple Console Tools for Windows (WinAPI)
 * Author: Tomasz Jaworski, 2017-2019
 */
#if !defined(__NEW_CONIO_H_)
#define __NEW_CONIO_H_

#include <Windows.h>
#include <assert.h>

static HANDLE houtput = NULL;
static HANDLE hinput = NULL;

static int current_foreground = 0;
static int current_background = 0;

//
// kolory znaku - ciemne
//
#define FG_BLACK	0
#define FG_BLUE		FOREGROUND_BLUE
#define FG_GREEN	FOREGROUND_GREEN
#define FG_RED		FOREGROUND_RED
#define FG_YELLOW	FOREGROUND_RED | FOREGROUND_GREEN
#define FG_PURPLE	FOREGROUND_RED | FOREGROUND_BLUE
#define FG_AQUA		FOREGROUND_GREEN | FOREGROUND_BLUE
#define FG_WHITE	FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE

//
// kolory znaku - jasne
//
#define FG_LBLUE	FOREGROUND_INTENSITY | FOREGROUND_BLUE
#define FG_LGREEN	FOREGROUND_INTENSITY | FOREGROUND_GREEN
#define FG_LRED		FOREGROUND_INTENSITY | FOREGROUND_RED
#define FG_LYELLOW	FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN
#define FG_LPURPLE	FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE
#define FG_LAQUA	FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE
#define FG_LGRAY	FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE

//
// kolory tła - ciemne
//
#define BG_BLACK	0
#define BG_BLUE		BACKGROUND_BLUE
#define BG_GREEN	BACKGROUND_GREEN
#define BG_RED		BACKGROUND_RED
#define BG_YELLOW	BACKGROUND_RED | BACKGROUND_GREEN
#define BG_PURPLE	BACKGROUND_RED | BACKGROUND_BLUE
#define BG_AQUA		BACKGROUND_GREEN | BACKGROUND_BLUE
#define BG_WHITE	BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE

//
// kolory tła - jasne
//
#define BG_LBLUE	BACKGROUND_INTENSITY | BACKGROUND_BLUE
#define BG_LGREEN	BACKGROUND_INTENSITY | BACKGROUND_GREEN
#define BG_LRED		BACKGROUND_INTENSITY | BACKGROUND_RED
#define BG_LYELLOW	BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN
#define BG_LPURPLE	BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE
#define BG_LAQUA	BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE
#define BG_LGRAY	BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE


//
// kolory znaku - ciemne
//
#define BLACK			0
#define BLUE			FOREGROUND_BLUE
#define GREEN			FOREGROUND_GREEN
#define RED				FOREGROUND_RED
#define YELLOW			FOREGROUND_RED | FOREGROUND_GREEN
#define PURPLE			FOREGROUND_RED | FOREGROUND_BLUE
#define AQUA			FOREGROUND_GREEN | FOREGROUND_BLUE
#define WHITE			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE

//
// kolory znaku - jasne
//
#define LIGHT_BLUE		FOREGROUND_INTENSITY | FOREGROUND_BLUE
#define LIGHT_GREEN		FOREGROUND_INTENSITY | FOREGROUND_GREEN
#define LIGHT_RED		FOREGROUND_INTENSITY | FOREGROUND_RED
#define LIGHT_YELLOW	FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN
#define LIGHT_PURPLE	FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE
#define LIGHT_AQUA		FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE
#define BRIGHT_WHITE		FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE

static void set_foreground(int fore_color);
static void set_background(int back_color);
static void hide_cursor(void);
static void show_cursor(void);
static void set_cursor_visibility(int visible);
static void set_screen_size(int width, int height);
static void sleep(int time_ms);
static int get_time(void);
static void gotoxy(int x, int y);
static int read_key(DWORD milliseconds);


static void set_screen_size(int width, int height)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	SMALL_RECT r;
	COORD      c;
	houtput = GetStdHandle(STD_OUTPUT_HANDLE);
	hinput = GetStdHandle(STD_INPUT_HANDLE);

	set_background(BLACK);
	set_foreground(WHITE);

	BOOL ret = GetConsoleScreenBufferInfo(houtput, &csbi);
	assert(ret && "GetConsoleScreenBufferInfo");

	r.Left = r.Top = 0;
	r.Right = width - 1;
	r.Bottom = height - 1;
	ret = SetConsoleWindowInfo(houtput, TRUE, &r);
	assert(ret && "SetConsoleWindowInfo");

	c.X = width;
	c.Y = height;
	ret = SetConsoleScreenBufferSize(houtput, c);
	assert(ret && "SetConsoleScreenBufferSize");

}

static void gotoxy(int x, int y)
{
	COORD pos = { x, y };
	BOOL ret = SetConsoleCursorPosition(houtput, pos);
	assert(ret && "SetConsoleCursorPosition");
}

static void set_cursor_visibility(int visible)
{
	CONSOLE_CURSOR_INFO cci;
	BOOL ret = GetConsoleCursorInfo(houtput, &cci);
	assert(ret && "GetConsoleCursorInfo");
	
	cci.bVisible = visible;
	ret = SetConsoleCursorInfo(houtput, &cci);
	assert(ret && "SetConsoleCursorInfo");
}

static void show_cursor(void)
{
	set_cursor_visibility(1);
}

static void hide_cursor(void)
{
	set_cursor_visibility(0);
}

static void set_background(int back_color)
{
	current_background = back_color & 0x0F;
	BOOL ret = SetConsoleTextAttribute(houtput, (current_background << 4) | current_foreground);
	assert(ret && "SetConsoleTextAttribute");
}

static void set_foreground(int fore_color)
{
	current_foreground = (fore_color & 0x0F);
	SetConsoleTextAttribute(houtput, (current_background << 4) | current_foreground);
}

static int get_time(void)
{
	return (int)GetTickCount();
}

static void sleep(int time_ms)
{
	Sleep(time_ms);
}



static int read_key(DWORD milliseconds)
{
	ULONGLONG start_time = GetTickCount64();

	INPUT_RECORD record;
	DWORD numRead;
	while (TRUE)
	{
		// czy minął czas?
		ULONGLONG now = GetTickCount64();
		if (now - start_time > milliseconds)
			return -1; // czas się skończył

		DWORD result = WaitForSingleObject(hinput, 10);
		if (result == WAIT_TIMEOUT)
			continue;

		BOOL ret = ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &record, 1, &numRead);
		assert(ret && "ReadConsoleInput");

		if (record.EventType != KEY_EVENT)
			continue;

		// stałe klawiszy dostępne pod VK_?????
		if (record.Event.KeyEvent.bKeyDown)
			return record.Event.KeyEvent.wVirtualKeyCode;
	}

}


#endif // __NEW_CONIO_H_


