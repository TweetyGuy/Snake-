#include <vector>
#include <ncurses.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <memory>

#define SNAKE_CHAR "#"
#define APPLE_CHAR "&"

struct Vec2
{
	int x = 10;
	int y = 10;
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
	
	}

	void draw()
	{
		mvaddstr(y, x, APPLE_CHAR);
	}
};

struct Snake
{
	std::shared_ptr<Vec2> head = std::make_unique<Vec2>();
	std::vector<std::shared_ptr<Vec2>> body;
	Vec2 dir;
	Snake()
	{
		dir.x = 0;
		dir.y = 0;
	
		body.push_back(std::move(head));
	}
	
	void extend_snake()
	{
		std::shared_ptr<Vec2> new_segment = std::make_unique<Vec2>();
		body.push_back(std::move(new_segment));
	}

	void move(int xdir, int ydir)
	{
		dir.x = xdir;
		dir.y = ydir;
	}

	void update(Apple* apple, WINDOW* win)
	{
		int key = wgetch(win);

		switch (key)
		{
			case 'S':
			case 's':
			case KEY_DOWN:
				move(0, 1);
				break;
			case 'W':
			case 'w':
			case KEY_UP:
				move(0, -1);
				break;
			case 'A':
			case 'a':
			case KEY_LEFT:
				move(-1, 0);
				break;
			case 'D':
			case 'd':
			case KEY_RIGHT:
				move(1, 0);
				break;
		}

		if (body.front()->x == apple->x && body.front()->y == apple->y)
		{
			apple->new_pos();
			extend_snake();
		}
	}

	void draw()
	{
		for (int i = body.size()-1; i >= 0; i--)
		{
			
			if (i == 0)
			{
				body[i]->x += dir.x;
				body[i]->y += dir.y;
			}

			if (i > 0)
			{
				body[i]->x = body[i-1]->x;
				body[i]->y = body[i-1]->y;
			}


			mvaddstr(body[i]->y, body[i]->x, SNAKE_CHAR);
		}

	}

};

void update(Snake* snake, Apple* apple, WINDOW* win)
{
	snake->update(apple, win);
	apple->update();
	snake->draw();
	apple->draw();
	wrefresh(win);
}

int main()
{
	WINDOW* win = initscr();
	keypad(win, true);
	nodelay(win, true);
	curs_set(0);
	
	Apple apple;
	Snake snake;

	apple.new_pos();

	while (true)
	{
		mvprintw(30, 30, "Your score is: %ld", snake.body.size());

		usleep(100000);
		update(&snake, &apple, win);
		erase();
	}

	endwin();
}
