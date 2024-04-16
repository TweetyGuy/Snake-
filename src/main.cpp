#include <vector>
#include <ncurses.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>


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
	Vec2 head;
	std::vector<Vec2> body;
	Snake()
	{
		body.push_back(head);
	}
	bool should_move_back = true;

	void move(int x, int y)
	{
		Vec2 vec;
		for (int i = 0; i < body.size(); i++)
		{
			if (i == body.size() - 1 && !should_move_back)
			{
				should_move_back = true;
				continue;
			}
			vec.x += x;
			vec.y += y;

			if (i == 0)
			{
				body[i].x += vec.x;
				body[i].y += vec.y;
			}
			else
			{
				body[i].x = body[i-1].x;
				body[i].y = body[i-1].y;
			}
		}
	}

	void extend_snake()
	{
		Vec2 new_segment = body.back();
		body.push_back(new_segment);
		should_move_back = false;
	}

};

struct Apple : Vec2
{
	Apple() = default;
	void new_pos()
	{
		x = rand() % 20; // Use seeded random numbers $ man 3 rand
		y = rand() % 20;
	}
};

int main()
{
	WINDOW* win = initscr();
	keypad(win, true);
	nodelay(win, true);
	
	Apple apple;
	Snake snake;
	Vec2 dir;

	apple.new_pos();

	while (true)
	{
		int key = wgetch(win);
		if (snake.body.front().x == apple.x && snake.body.front().y == apple.y)
		{
			apple.new_pos();
			snake.extend_snake();
		}

		if (key == KEY_DOWN)
		{
			
		}
		if (key == KEY_UP)
		{
			dir.x = 0;
			dir.y = 1;
		}
		if (key == KEY_LEFT)
		{
			dir.x = -1;
			dir.y = 0;
		}
		if (key == KEY_RIGHT)
		{
			dir.x = 1;
			dir.y = 0;
		}
		snake.body.front().x += dir.x;
		snake.body.front().y += dir.y;
		erase();
		mvaddstr(snake.body.front().y, snake.body.front().x, "#");
		mvaddstr(apple.x, apple.y, "&");
		usleep(100000); // Sleep for 0.1s
	}

	endwin();
}
