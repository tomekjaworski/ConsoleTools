/*
 * Simple Console Tools for Windows (WinAPI)
 * Author: Tomasz Jaworski, 2017-2019
 */
#if !defined(__NEW_CONIO_H_)
#define __NEW_CONIO_H_

#include <Windows.h>
#include <assert.h>
#include <stdio.h>

__declspec(selectany) extern struct __conio_t {
	HANDLE houtput;
	HANDLE hinput;

	int current_foreground;
	int current_background;

	int initialized;
} __conio = { 0 };

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

//
// Funkcje
//


static void set_screen_size(int screen_width, int screen_height);
static void set_screen_buffer_size(int screen_width, int screen_height, int buffer_height);

static void set_cursor_position(COORD pos);
static COORD get_cursor_position(void);
static void set_cursor_visibility(int visible);
static void gotoxy(int x, int y);
static void hide_cursor(void);
static void show_cursor(void);

static void set_foreground(int fore_color);
static void set_background(int back_color);

static int read_key(DWORD milliseconds);
static int cwprintf(int color, const wchar_t* format, ...);
static int cprintf(int color, const char* format, ...);
static void wait(const wchar_t* msg);

static void sleep(int time_ms);
static int get_time(void);

//
//
// #######################################################################################
//
//

static int __conio_init(void) {

	if (__conio.initialized)
		return 0;

	__conio.houtput = GetStdHandle(STD_OUTPUT_HANDLE);
	__conio.hinput = GetStdHandle(STD_INPUT_HANDLE);

	// Ok, inicjalizacja zakończona
	__conio.initialized = 1;

	//
	// Od tego momentu można używać funkcji konsolowych do zainicjowania jej stanu
	//

	set_background(BLACK);
	set_foreground(WHITE);

	//gotoxy(0, 0);

	return 0;
}

//
// ============================================================
// Funkcje inicjujące konsolę
//

static void set_screen_size(int screen_width, int screen_height)
{
	__conio_init();

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	SMALL_RECT r;
	COORD      c;
	   
	BOOL ret = GetConsoleScreenBufferInfo(__conio.houtput, &csbi);
	assert(ret && "GetConsoleScreenBufferInfo");

	c.X = screen_width;
	c.Y = screen_height;
	ret = SetConsoleScreenBufferSize(__conio.houtput, c);
	assert(ret && "SetConsoleScreenBufferSize");

	r.Left = r.Top = 0;
	r.Right = screen_width - 1;
	r.Bottom = screen_height - 1;
	ret = SetConsoleWindowInfo(__conio.houtput, TRUE, &r);
	assert(ret && "SetConsoleWindowInfo");
}

static void set_screen_buffer_size(int screen_width, int screen_height, int buffer_height)
{
	__conio_init();

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	SMALL_RECT r;
	COORD      c;

	BOOL ret = GetConsoleScreenBufferInfo(__conio.houtput, &csbi);
	assert(ret && "GetConsoleScreenBufferInfo");

	c.X = screen_width;
	c.Y = buffer_height;
	ret = SetConsoleScreenBufferSize(__conio.houtput, c);
	assert(ret && "SetConsoleScreenBufferSize");

	r.Left = r.Top = 0;
	r.Right = screen_width - 1;
	r.Bottom = screen_height - 1;
	ret = SetConsoleWindowInfo(__conio.houtput, TRUE, &r);
	assert(ret && "SetConsoleWindowInfo");
}

//
// ============================================================
// Funkcje kursorem (położenie, widzialnośc, itd)
//

static COORD get_cursor_position(void)
{
	__conio_init();

	CONSOLE_SCREEN_BUFFER_INFO csbi;

	BOOL ret = GetConsoleScreenBufferInfo(__conio.houtput, &csbi);
	assert(ret && "GetConsoleScreenBufferInfo");

	return csbi.dwCursorPosition;
}

static void set_cursor_position(COORD pos)
{
	__conio_init();

	BOOL ret = SetConsoleCursorPosition(__conio.houtput, pos);
	assert(ret && "SetConsoleCursorPosition");
}

static void gotoxy(int x, int y)
{
	__conio_init();

	COORD pos = { (SHORT)x, (SHORT)y };
	BOOL ret = SetConsoleCursorPosition(__conio.houtput, pos);
	assert(ret && "SetConsoleCursorPosition");
}

static void set_cursor_visibility(int visible)
{
	__conio_init();

	CONSOLE_CURSOR_INFO cci;
	BOOL ret = GetConsoleCursorInfo(__conio.houtput, &cci);
	assert(ret && "GetConsoleCursorInfo");
	
	cci.bVisible = visible;
	ret = SetConsoleCursorInfo(__conio.houtput, &cci);
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


//
// ============================================================
// Funkcje ustawiania koloru znaku i tła
//


static void set_background(int back_color)
{
	__conio_init();

	__conio.current_background = back_color & 0x0F;
	BOOL ret = SetConsoleTextAttribute(__conio.houtput, (__conio.current_background << 4) | __conio.current_foreground);
	assert(ret && "SetConsoleTextAttribute");
}

static void set_foreground(int fore_color)
{
	__conio_init();

	__conio.current_foreground = (fore_color & 0x0F);
	BOOL ret = SetConsoleTextAttribute(__conio.houtput, (__conio.current_background << 4) | __conio.current_foreground);
	assert(ret && "SetConsoleTextAttribute");
}


//
// ============================================================
// Funkcja czasu
//


static int get_time(void)
{
	return (int)GetTickCount();
}

static void sleep(int time_ms)
{
	Sleep(time_ms);
}

//
// ============================================================
// Funkcje wejścia/wyjścia
//


static int read_key(DWORD milliseconds)
{
	__conio_init();

	ULONGLONG start_time = GetTickCount64();

	INPUT_RECORD record;
	DWORD numRead;
	while (TRUE)
	{
		// czy minął czas?
		ULONGLONG now = GetTickCount64();
		if (now - start_time > milliseconds)
			return -1; // czas się skończył

		DWORD result = WaitForSingleObject(__conio.hinput, 10);
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



void wait(const wchar_t* msg)
{
	if (msg)
		wprintf(L"Nacisnij ENTER aby kontynuować (%s)...\n", msg);
	else
		wprintf(L"Nacisnij ENTER aby kontynuować...\n");
	DWORD a = GetTickCount();
	DWORD delta;
	do {
		getchar();
		delta = GetTickCount() - a;
	} while (delta < 10);
}


static int cwprintf(int color, const wchar_t* format, ...)
{
	__conio_init();

	va_list ap;
	va_start(ap, format);

	BOOL ret = SetConsoleTextAttribute(__conio.houtput, color);
	assert(ret && "SetConsoleTextAttribute");

	int chars = vwprintf(format, ap);

	ret = SetConsoleTextAttribute(__conio.houtput, (__conio.current_background << 4) | __conio.current_foreground);
	assert(ret && "SetConsoleTextAttribute");

	return chars;
}



static int cprintf(int color, const char* format, ...)
{
	__conio_init();

	va_list ap;
	va_start(ap, format);
	
	BOOL ret = SetConsoleTextAttribute(__conio.houtput, color);
	assert(ret && "SetConsoleTextAttribute");

	int chars = vprintf(format, ap);

	ret = SetConsoleTextAttribute(__conio.houtput, (__conio.current_background << 4) | __conio.current_foreground);
	assert(ret && "SetConsoleTextAttribute");

	return chars;
}

#endif // __NEW_CONIO_H_


