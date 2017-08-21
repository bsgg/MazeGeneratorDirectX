#pragma once

#include "Graphics.h"
#include <vector>

class MazeA
{
private:
	class Node 
	{

	public:

		Node();

		int X() const
		{
			return x;
		}
		void SetX(int ix)
		{
			x = ix;
		}

		int Y() const
		{
			return y;
		}
		void SetY(int iy)
		{
			y = iy;
		}

		Node* Parent() const
		{
			return parent;
		}		
		void SetParent(Node* src)
		{
			parent = src;
		}

		char Label() const
		{
			return label;
		}
		void SetLabel(char il)
		{
			label = il;
		}
		char Directions() const
		{
			return dir;
		}
		void SetDirections(char id)
		{
			dir = id;
		}

		int GetDirection() const;

		bool AllDirectionsExplored() const;

		bool CheckDirection(int dir) const;

		void SetExploredDirection(int dir);
		

	private:
		int x = -1;
		int y = -1;
		char label = '#'; // label to indicate that there is a wall or a hole
		char dir = 15; //Directions that stil haven't been explored
		Node* parent = nullptr; //Pointer to parent node

		// Direction
		int iDirection = 0; // Current direction to check
		int directions[4] = { 0,1,2,3 }; // UP = 0; RIGHT = 1, DOWN = 2, LEFT = 3
		bool exploredDirections[4] = { false,false,false,false }; // Explored already explored
	};	

public:
	void Init();
	void Generate();
	void Draw(Graphics& gfx) const;
	~MazeA();

private:
	Node * LinkNode(Node * node);
	void GetCoordFromQuadrant(int quadrant, int& x, int& y);

	
	std::vector<int> listPickups;

private:
	bool init = false;

	// Only odd numbers
	int width = 21;
	int height = 21;

	Node * listNodes = nullptr;

	Node StartNode;
	Node EndNode;


};