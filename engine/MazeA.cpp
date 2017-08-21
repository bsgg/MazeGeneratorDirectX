#include "MazeA.h"
#include <time.h>

MazeA::Node::Node()
{
	// Shuffle directions
	for (int i = 0; i<4; i++)
	{
		int r = rand() % 4;  // generate a random position
		int temp = directions[i];
		directions[i] = directions[r];
		directions[r] = temp;
	}
}

int MazeA::Node::GetDirection() const
{
	return directions[iDirection];
}

bool MazeA::Node::AllDirectionsExplored() const
{
	// Gets if all directions have been already explored
	return (exploredDirections[0] && exploredDirections[1] && exploredDirections[2] && exploredDirections[3]);

}

bool MazeA::Node::CheckDirection(int dir) const
{
	if ((dir >= 0) && (dir < 4)) return exploredDirections[dir];
	
	return false;
}

void MazeA::Node::SetExploredDirection(int dir)
{
	if ((dir >= 0) && (dir < 4))
	{
		exploredDirections[dir] = true;
		iDirection++;
	}
}


void MazeA::Init()
{
	if (!init)
	{
		// Only positive numbers
		if ((width <  0) || (height < 0))
		{
			return;
		}

		// Width and height has to be odd numbers
		if ((width % 2 == 0) || (height % 2 == 0))
		{
			return;
		}

		// Init array nodes
		listNodes = new Node[width * height];

		Node * current;
		// Setup crucial nodes
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				// Select a node
				current  = listNodes + i + j * width;
				if (i * j % 2)
				{
					current->SetX(i);
					current->SetY(j);
					current->SetDirections(15); //Assume that all directions can be explored (4 youngest bits set)
					current->SetLabel(' ');
				}
				else
				{
					current->SetLabel('#'); //Add walls between nodes
				}
			}
		}

		init = true;

		//Seed random generator
		srand(time(NULL));
	}
}
void MazeA::Generate()
{
	if (!init) return;

	Node *start;
	Node *last;

	start = listNodes + 1 + width;
	start->SetParent(start);
	last = start;

	//Connect nodes until start node is reached and can't be left	
	do 
	{
		last = LinkNode(last);

	} while (last != start);

	// Set Start, End position

	// Get a random quadrant for start position // 0: LeftTop, 1: RightTop, 2: RigthBottom, 3: LeftBottom
	int startQuadrant = rand() % 4;
	int xStart = -1;
	int yStart = -1;

	GetCoordFromQuadrant(startQuadrant, xStart, yStart);
	StartNode.SetX(xStart);
	StartNode.SetY(yStart);

	int endQuadrant = -1;
	// Set end quadrant
	switch (startQuadrant)
	{
		// LeftTop
	case 0:
		endQuadrant = 2;
		break;
		// RightTop
	case 1:
		endQuadrant = 3;
		break;

		// RigthBottom
	case 2:
		endQuadrant = 0;
		break;

		// LeftBottom
	case 3:
		endQuadrant = 1;
		break;
	}

	GetCoordFromQuadrant(endQuadrant, xStart, yStart);
	EndNode.SetX(xStart);
	EndNode.SetY(yStart);

}

void MazeA::GetCoordFromQuadrant(int quadrant, int& x, int& y)
{
	char label = '#';
	int halfH = (height / 2);

	while (label == '#')
	{
		switch (quadrant)
		{
			// LeftTop
			case 0:
			x = 1;
			y = (rand() % halfH) + 1;
			break;

			// RightTop
			case 1:
			x = width - 2;
			y = (rand() % halfH) + 1;
			break;

			// RigthBottom
			case 2:
			x = width - 2;
			y = (rand() % halfH) + halfH;
			break;

			// LeftBottom
			case 3:
			x = 1;
			y = (rand() % halfH) + halfH;
			break;
			
		}

		label = listNodes[x + y * width].Label();

	};

}

MazeA::Node * MazeA::LinkNode(Node * node)
{
	if (!init) return nullptr;

	if (node == nullptr) return nullptr;

	int x, y;
	//char dir;
	Node *dest;

	//While there are directions still unexplored
	//while (node->Directions())
	while (!node->AllDirectionsExplored())
	{
		// Get random direction

		//Randomly pick one direction
		//dir = (1 << (rand() % 4));

		//If it has already been explored - try again
		
		//if (~node->Directions() & dir) continue;
		int dir = node->GetDirection();

		if (node->CheckDirection(dir)) continue;


		//Mark direction as explored
		//char cDir = node->Directions();
		//char newDir = cDir &= ~dir;
		//node->SetDirections(newDir);

		node->SetExploredDirection(dir);

		//Depending on chosen direction
		/*switch (dir)
		{
			//Check if it's possible to go right
		case 1:
			if (node->X() + 2 < width)
			{
				x = node->X() + 2;
				y = node->Y();
			}
			else continue;
			break;

			//Check if it's possible to go down
		case 2:
			if (node->Y() + 2 < height)
			{
				x = node->X();
				y = node->Y() + 2;
			}
			else continue;
			break;

			//Check if it's possible to go left	
		case 4:
			if (node->X() - 2 >= 0)
			{
				x = node->X() - 2;
				y = node->Y();
			}
			else continue;
			break;

			//Check if it's possible to go up
		case 8:
			if (node->Y() - 2 >= 0)
			{
				x = node->X();
				y = node->Y() - 2;
			}
			else continue;
			break;
		}*/

		// Check each direction
		switch (dir)
		{
			
		case 0: // Up
			if (node->Y() - 2 >= 0)
			{
				x = node->X();
				y = node->Y() - 2;
			}
			else continue;
			break;

		case 1:
			// Rigth
			if (node->X() + 2 < width)
			{
				x = node->X() + 2;
				y = node->Y();
			}
			else continue;
			break;

			
		case 2: // Down
			if (node->Y() + 2 < height)
			{
				x = node->X();
				y = node->Y() + 2;
			}
			else continue;
			break;

			//Check if it's possible to go left	
		case 3:
			if (node->X() - 2 >= 0)
			{
				x = node->X() - 2;
				y = node->Y();
			}
			else continue;
			break;
		}

		//Get destination node into pointer (makes things a tiny bit faster)
		dest = listNodes + x + y * width;

		//Make sure that destination node is not a wall
		if (dest->Label() == ' ')
		{
			//If destination is a linked node already - abort
			if (dest->Parent() != NULL) continue;

			//Otherwise, adopt node
			dest->SetParent(node);

			//Remove wall between nodes
			listNodes[node->X() + (x - node->X()) / 2 + (node->Y() + (y - node->Y()) / 2) * width].SetLabel(' ');
			//Return address of the child node
			return dest;
		}
	}

	//If nothing more can be done here - return parent's address
	return node->Parent();

}

void MazeA::Draw(Graphics& gfx) const
{
	if (!init) return;

	Vei2 topLeft(50, 50);

	int cellSize = 25;
	int border = 3;

	Color noWalkColor = { 59,59,59 };
	Color walkColor = { 110,206,238 };
	Color borderColor = { 200,200,200 };

	Color startNode = { 255,181,51 };
	Color endNode = { 166, 255,51 };

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			RectI cell(topLeft.x + (x * cellSize),
				topLeft.x + ((x * cellSize) + cellSize),
				topLeft.y + (y * cellSize),
				topLeft.y + ((y * cellSize) + cellSize));

			RectI border(cell.left - border, cell.right + border, cell.top - border, cell.bottom + border);

			char c = listNodes[x + y * width].Label();

			Color colorCell = { 0,0,0 };
			if (c == ' ')
			{
				colorCell = { 59,59,59 };
			}
			else
			{
				colorCell = { 110,206,238 };
			}

			// Draw Start node
			if (x == StartNode.X() && y == StartNode.Y())
			{
				colorCell = { 255,181,51 };
			}
			

			// Draw End node
			if (x == EndNode.X() && y == EndNode.Y())
			{
				colorCell = { 166, 255,51 };
			}
			

			gfx.DrawRect(border, borderColor);
			gfx.DrawRect(cell, colorCell);
			
		}

	}
}

MazeA::~MazeA()
{
	delete[] listNodes;
	listNodes = nullptr;
}

