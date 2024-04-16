#include <ncurses.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <vector>


struct Vec2
{
	Vec2() = default;

	int x = 0;
	int y = 0;

	void set_pos(int npos_x, int npos_y)
	{
		x = npos_x;
		y = npos_y;
	}

	void set_x(int new_x)
	{
		x = new_x;
	}

	void set_y(int new_y)
	{
		y = new_y;
	}
};

struct Snake
{
	std::vector<struct Vec2> body{};

	struct Vec2 head{};
	body.push_back(head);

	Snake()
	{
		body.push_back(head);
	}

	bool should_move_back = true;

	void move(int x, int y)
	{
		for (auto segment : body)
		{
			if (segment.x == body.back().x && segment.y == body.back().y && !should_move_back)
			{
				should_move_back = true;
				continue;
			}
			segment.x += x;
			segment.y += y;
		}
	}

	void extend_snake()
	{
		Vec2 new_segment = body.back();
		should_move_back = false;
	}

};

struct Apple
{
	Apple() = default;
	Vec2 apple;
	void new_pos()
	{
		apple.x = rand() % 20; // Use seeded random numbers $ man 3 rand
		apple.y = rand() % 20;
	}
};

int main()
{
	WINDOW* win = initscr();
	keypad(win, true);
	nodelay(win, true);
	
	Apple apple;
	Snake snake;

	apple.new_pos();

	while (true)
	{
		int key = wgetch(win);
		
		apple.new_pos();

		if (key == KEY_DOWN)
			snake.move(0, -1);
		if (key == KEY_UP)
			snake.move(0, 1);
		if (key == KEY_LEFT)
			snake.move(-1, 0);
		if (key == KEY_RIGHT)
			snake.move(1, 0);

		erase();
		mvaddstr(snake.body.front().x, snake.body.front().y, "*");
		mvaddstr(apple.apple.x, apple.apple.y, "&");
		usleep(1000000);
	}

	endwin();
}
