#pragma once

#include "Graphics.h"

class Maze
{
private:
	class Cell
	{
	public:
		enum ETYPECELL
		{
			NONE = 0,
			WALK, 
			NOWALK, 
			START,
			END,
		};
	private:

		int row;
		int col;
		char label;

		int score;
		bool isAcross;
		bool isDown;
		int indexLetter;

		// 0 = Empty (White), 1 = Wall (Black), 2 Start, 3 End
		Cell::ETYPECELL typeCell;

	public:
		Cell();
		Cell(const int r, const int c);

		

		int GetRow() const;
		int GetColumn() const;

		Cell::ETYPECELL TypeCell() const;
		void SetTypeCell(Cell::ETYPECELL tCell);

		void SetPosition(const int r, const int c);

		void SetLabel(const char l);
		char Label() const;


		void SetScore(const int s);
		int Score() const;

		void SetAcross(const bool s);
		bool Across() const;

		void SetDown(const bool s);
		bool Down() const;

		void SetIndexLetter(const int i);
		int IndexLetter() const;
	};

public:
	Maze();

public:
	// Create an enum named DIR to keep track of the four directions (NORTH, EAST, SOUTH, WEST)
	enum DIRECTION { UP, RIGHT, DOWN, LEFT, NONE };
	void Draw(Graphics& gfx) const;
	

private:
	const static int ROWS = 18;
	const static int COLS = 18;
	int cellSize = 16;
	Cell grid[ROWS][COLS];

	void setDownWord(std::string word, int row, int col, int indexLetter);
	void setAcrossWord(std::string word, int row, int col, int indexLetter);

	int GetDownScore(const Cell currentCell, const std::string currentWord);
	int GetAcrossScore(const Cell currentCell, const std::string currentWord);

	bool Maze::CheckFirstDown(int index, int row, int col) const;
	bool Maze::CheckLastDown(int index, int lenght, int row, int col) const;

	bool Maze::CheckFirstAcross(int index, int row, int col) const;
	bool Maze::CheckLastAcross(int index, int lenght, int row, int col) const;

	bool Maze::CheckLeftRight(char label, char currentLabel, int row, int col, int& score);
	bool Maze::CheckTopBottom(char label, char currentLabel, int row, int col, int& score);
	
};