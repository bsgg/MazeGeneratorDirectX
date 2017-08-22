#pragma once

class Tableboard
{
private:
	struct Position
	{
		int x;
		int y;
		Position()
		{
			x = 0;
			y = 0;
		}
		Position(int pX, int pY)
		{
			x = pX;
			y = pY;
		}
	};
private:

	const static int width = 10;
	const static int height = 10;
	const static int obstacles = 4;

	int table[width][height];
	int manhattanTable[width][height];


	// Total distnace between end and start
	int totalDistance;
	Position endPosition;

private:
	int GetDirection(Position pos);
	void MoveTowards(Position pos, int nextDirection);

public:

	// Constructor de la clase
	Tableboard(Position pos, Position endpos);


};
