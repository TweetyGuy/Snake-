#include <vector>
#include <ncurses.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <memory>
#include <ctime>

#define SNAKE_CHAR "#"
#define APPLE_CHAR "&"

struct Vec2
{
	int x = 0;
	int y = 0;

	void set_pos(unsigned int new_x, unsigned int new_y)
	{
		x = new_x;
		y = new_y;
	}

};

struct Apple : Vec2
{
	Apple()
	{
		time_t t;
		srand((unsigned int) time(&t));
		set_pos(40, 15);
	}

	void new_pos(std::vector<std::shared_ptr<Vec2>>* body)
	{
		x = rand() % 60; // Use seeded random numbers $ man 3 rand
		y = rand() % 30;

		for (auto segment : *body)
		{
			if (segment->x == x && segment->y == y)
				new_pos(body);
		}
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

		body.front()->set_pos(20, 15);
	}
	
	void extend_snake()
	{
		std::shared_ptr<Vec2> new_segment = std::make_unique<Vec2>();
		body.push_back(std::move(new_segment));
	}

	void move(int xdir, int ydir)
	{
		if ((dir.x < 0 && xdir > 0) | (dir.x > 0 && xdir < 0) | (dir.y < 0 && ydir > 0) | (dir.y > 0 && ydir < 0))
		{
			return;
		}

		dir.x = xdir;
		dir.y = ydir;
	}

	void update(Apple* apple, WINDOW* win)
	{
		int key = wgetch(win);

		switch (key)
		{
			case 'S':
				move(0, 2);
				break;
			case 's':
			case KEY_DOWN:
				move(0, 1);
				break;
			case 'W':
				move(0, -2);
				break;
			case 'w':
			case KEY_UP:
				move(0, -1);
				break;
			case 'A':
				move(-2, 0);
				break;
			case 'a':
			case KEY_LEFT:
				move(-1, 0);
				break;
			case 'D':
				move(2, 0);
				break;
			case 'd':
			case KEY_RIGHT:
				move(1, 0);
				break;
		}

		if (body.front()->x == apple->x && body.front()->y == apple->y)
		{
			apple->new_pos(&body);
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
	curs_set(0);
	
	Apple apple;
	Snake snake;

	mvprintw(14, 2, "Welcome to Snake++, a terminal snake game written in C++.");
	mvprintw(15, 11, "To win, you must reach a score of 100.");
	mvprintw(30, 16, "Press any key to continue . . . ");

	wgetch(win);

	nodelay(win, true);
	while (snake.body.size() < 100)
	{
		mvprintw(30, 51, "Your score is: %ld", snake.body.size());

		usleep(100000);
		update(&snake, &apple, win);
		erase();
	}

	nodelay(win, false);
	mvprintw(15, 15, "Wow! You won! Congratulations!");
	mvprintw(30, 16, "Press any key to continue . . . ");
	wgetch(win);

	endwin();
}
