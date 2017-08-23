#include <iostream>
#include "Tableboard.h"

Tableboard::Tableboard(Position pos, Position endpos)
	:
	endPosition(endpos.x, endpos.y)
{

	/*TILE_CLEAN = 1,
		TILE_INIT_POS = 2,
		TILE_OBSTACLE = 3,
		TILE_END_POS = 4,
		TILE_EXPLORED = 5*/

	for (int x = 0; x<width; x++)
	{
		for (int y = 0; y<height; y++)
		{
			// Clean cell
			table[x][y] = 1;

		}
	}

	table[2][2] = 3;
	table[3][2] = 3;
	table[3][3] = 3;
	table[6][3] = 3;
	table[7][3] = 3;
	table[9][2] = 3;

	// Set start, end position
	table[pos.x][pos.y] = 2;
	table[endpos.x][endpos.y] = 4;

	// Init manhatan board with the cost from each cell to end point
	// Manhattan [ D((a,b), (c,d)) = |c-a| + |d-b| ]
	// Inicial , final
	int distance = abs(endpos.x - pos.x) + abs(endpos.y - pos.y);

	for (int x = 0; x<width; x++)
	{
		for (int y = 0; y<height; y++)
		{
			// Set the distance from the current element to endpos
			manhattanTable[x][y] = abs(endpos.x - x) + abs(endpos.y - y);
		}
	}

	totalDistance = 0;
	int nextDirection = GetDirection(pos);
	if (nextDirection > -1)
	{
		MoveTowards(pos, nextDirection);
	}

}

int Tableboard::GetDirection(Position pos)
{
	int nextDirection = -1;

	int auxX = -1;
	int auxY = -1;

	// Distance to next cell
	int nextDistance = 0;

	int distEndPos = 0;

	int minimunCost = 0;
	int cost = 0;

	// Up: Check limits for this direction and if the cell is clean or end
	if ((pos.y - 1) >= 0)
	{
		int cell = table[pos.x][pos.y - 1];
		if ((cell == 1) || (cell == 4))
		{
			auxX = pos.x;
			auxY = pos.y - 1;

			nextDistance = abs(auxX - pos.x) + abs(auxY - pos.y);

			distEndPos = manhattanTable[auxX][auxY];
			cost = nextDistance + distEndPos;
			
			if ((minimunCost > cost) || (minimunCost == 0))
			{
				minimunCost = cost;
				nextDirection = 0;
			}
		}
	}

	// Rigth: Check limits for this direction and if the cell is clean or end
	if ((pos.x + 1) < width)
	{
		int cell = table[pos.x + 1][pos.y];
		if ((cell == 1) || (cell == 4))
		{
			auxX = pos.x + 1;
			auxY = pos.y;

			nextDistance = abs(auxX - pos.x) + abs(auxY - pos.y);

			distEndPos = manhattanTable[auxX][auxY];
			cost = nextDistance + distEndPos;

			if ((minimunCost > cost) || (minimunCost == 0))
			{
				minimunCost = cost;
				nextDirection = 1;
			}
		}
	}

	// Down: Check limits for this direction and if the cell is clean or end
	if ((pos.y + 1) < height)
	{
		int cell = table[pos.x][pos.y + 1];
		if ((cell == 1) || (cell == 4))
		{
			auxX = pos.x;
			auxY = pos.y + 1;

			nextDistance = abs(auxX - pos.x) + abs(auxY - pos.y);
			// Coste de esa posicion
			distEndPos = manhattanTable[auxX][auxY];
			cost = nextDistance + distEndPos;

			if ((minimunCost > cost) || (minimunCost == 0))
			{
				minimunCost = cost;
				nextDirection = 2;
			}
		}
	}

	// Left: Check limits for this direction and if the cell is clean or end
	if ((pos.x - 1) >= 0)
	{
		int cell = table[pos.x + 1][pos.y];
		if ((cell == 1) || (cell == 4))
		{
			auxX = pos.x - 1;
			auxY = pos.y;

			nextDistance = abs(auxX - pos.x) + abs(auxY - pos.y);
			// Cost
			distEndPos = manhattanTable[auxX][auxY];
			cost = nextDistance + distEndPos;
						

			if ((minimunCost > cost) || (minimunCost == 0))
			{
				minimunCost = cost;
				nextDirection = 3;
			}
		}
	}

	totalDistance += nextDistance;
	return nextDirection;

}
void Tableboard::MoveTowards(Position pos, int nextDirection)
{
	// Check if reaches end direction
	if ((pos.x == endPosition.x) && (pos.y == endPosition.y))
	{
		return;
	}


	switch (nextDirection)
	{

	case 0: // Up
		pos.y = pos.y - 1;

		break;

	case 1: // Right
		
		pos.x = pos.x + 1;
		break;


	case 2: // Down
		pos.y = pos.y + 1;
		break;

	case 3: // Left
		pos.x = pos.x - 1;		
		break;

	}

	// Check if the cell is clean and set as a explored
	if (table[pos.x][pos.y] == 1)
	{
		table[pos.x][pos.y] = 5;
	}

	// Get next direction and move if it's possible
	nextDirection = GetDirection(pos);
	if (nextDirection > -1)
	{
		MoveTowards(pos, nextDirection);
	}

}

void Tableboard::Draw(Graphics& gfx) const
{
	Vei2 topLeft(50, 50);

	int cellSize = 25;
	int border = 3;

	Color noWalkColor = { 59,59,59 };
	Color walkColor = { 110,206,238 };
	Color borderColor = { 200,200,200 };

	Color startNode = { 255,181,51 };
	Color endNode = { 166, 255,51 };

	for (int x = 0; x<width; x++)
	{
		for (int y = 0; y<height; y++)
		{	
			RectI cell(topLeft.x + (x * cellSize),
				topLeft.x + ((x * cellSize) + cellSize),
				topLeft.y + (y * cellSize),
				topLeft.y + ((y * cellSize) + cellSize));

			RectI border(cell.left - border, cell.right + border, cell.top - border, cell.bottom + border);

			/*TILE_CLEAN = 1,
			TILE_INIT_POS = 2,
			TILE_OBSTACLE = 3,
			TILE_END_POS = 4,
			TILE_EXPLORED = 5*/

			Color colorCell = { 0,0,0 };
			
			switch (table[x][y])
			{
				case 1: // Unexplored cell
					colorCell = { 59,59,59 };
				break;
				case 2: // Start position
					colorCell = { 255,181,51 };
				break;
				case 3: // Obstacle
					colorCell = { 255, 51,51 };
				break;
				case 4: // End Position
					colorCell = { 166, 255,51 };
				break;
				case 5: // Explored cell
					colorCell = { 110,206,238 };
				break;
			
			}	
			
			gfx.DrawRect(border, borderColor);
			gfx.DrawRect(cell, colorCell);

		}

	}
}