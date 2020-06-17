#include "Snake.h"

Snake::Snake(vector<int> pos, vector<int> dir)
{
	this->pos = pos;
	this->dir = dir;
}

void Snake::setPos(vector<int> pos)
{
	this->pos = pos;
}

vector<int> Snake::getPos()
{
	return this->pos;
}

void Snake::setDir(vector<int> dir)
{
	this->dir = dir;
}

vector<int> Snake::getDir()
{
	return this->dir;
}

void Snake::move()
{
	this->pos[0] += this->dir[0];
	this->pos[1] += this->dir[1];
}

