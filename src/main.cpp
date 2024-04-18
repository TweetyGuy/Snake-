#include <vector>
#include <ncurses.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>


struct Vec2
{
	Vec2() = default;

	int x = 10;
	int y = 10;

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

struct Apple : Vec2
{
	Apple() = default;
	void new_pos()
	{
		x = rand() % 20; // Use seeded random numbers $ man 3 rand
		y = rand() % 20;
	}

	void update()
	{
		mvaddstr(y, x, "&");
	}
};

struct Snake
{
	Vec2 head;
	std::vector<Vec2> body;
	Vec2 dir;
	Snake()
	{
		dir.x = 0;
		dir.y = 0;
	
		body.push_back(head);
	}
	bool should_move_back = true;


	void extend_snake() // Silly ahh function. new_segment gets deleted at end of scope lmaooo gotta make it an std::vector<std::shared_ptr<Vec2>> and push_back an std::shared_ptr<Vec2>;
	{
		Vec2 new_segment = body.back();
		body.push_back(new_segment);
		should_move_back = false;
	}

	void move(int xdir, int ydir)
	{
		dir.x = xdir;
		dir.y = ydir;
	}

	void update(Apple* apple, WINDOW* win)
	{
		int key = wgetch(win);

		if (key == KEY_DOWN)
		{
			move(0, 1);
		}
		if (key == KEY_UP)
		{
			move(0, -1);
		}
		if (key == KEY_LEFT)
		{
			move(-1, 0);
		}
		if (key == KEY_RIGHT)
		{
			move(1, 0);
		}

		if (body.front().x == apple->x && body.front().y == apple->y)
		{
			apple->new_pos();
			//extend_snake();
		}

		for (int i = 0; i < body.size(); i++)
		{
			if (i == 0)
			{
				body[i].x += dir.x;
				body[i].y += dir.y;
			}
			else
			{
				body[i].x = body[i-1].x;
				body[i].y = body[i-1].y;
			}
		}
		mvaddstr(body.front().y, body.front().x, "#");

	}

	


};

void update(Snake* snake, Apple* apple, WINDOW* win)
{
	snake->update(apple, win);
	apple->update();
	refresh();
}

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
		update(&snake, &apple, win);
		erase();
		usleep(100000); // Sleep for 0.1s
	}

	endwin();
}
