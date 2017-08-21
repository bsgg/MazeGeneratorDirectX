#include "Maze.h"
#include "Colors.h"
#include <vector>
#include <cstdlib>  
#include <algorithm>
#include <iostream>

// CELL IMPLEMENTATION
Maze::Cell::Cell()
{
	typeCell = Cell::NOWALK;
	row = -1;
	col = -1;

}
Maze::Cell::Cell(const int r, const int c)
{
	row = r;
	col = c;
}


Maze::Cell::Cell::ETYPECELL Maze::Cell::TypeCell() const
{
	return typeCell;
}
void Maze::Cell::SetTypeCell(Maze::Cell::ETYPECELL tCell)
{
	typeCell = tCell;
}

int Maze::Cell::GetRow() const
{
	return row;
}
int Maze::Cell::GetColumn() const
{
	return col;
}

void Maze::Cell::SetPosition(const int r, const int c)
{
	row = r;
	col = c;
}
void Maze::Cell::SetLabel(const char l)
{
	label = l;
}
char Maze::Cell::Label() const
{
	return label;
}

void Maze::Cell::SetScore(const int s)
{
	score = s;
}
int Maze::Cell::Score() const
{
	return score;
}

void Maze::Cell::SetAcross(const bool s)
{
	isAcross = s;
}
bool Maze::Cell::Across() const
{
	return isAcross;
}

void Maze::Cell::SetDown(const bool s)
{
	isDown = s;
}
bool Maze::Cell::Down() const
{
	return isDown;
}

void Maze::Cell::SetIndexLetter(const int i)
{
	indexLetter = i;
}
int Maze::Cell::IndexLetter() const
{
	return indexLetter;
}


// CELL IMPLEMENTATION

// Maze implementation
Maze::Maze()
{
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			grid[row][col].SetPosition(row, col);
			// Set cell as empty
			grid[row][col].SetTypeCell(Cell::NOWALK);
			grid[row][col].SetLabel('0');
		}
	}


	

	// Generate list of "words" minimum 3, max number rows and columsn
	
	int label = 0;

	// Create array of lenghs for words
	int auxLenghtWords[20];
	for (int i = 0; i < 20; i++)
	{
		// Maximun nRows, Minimun 3
		if (ROWS < COLS)
		{
			auxLenghtWords[i] = rand() % (ROWS -3) + 2;
		}
		else
		{
			auxLenghtWords[i] = rand() % (COLS -3) + 2;
		}
		
	}

	// Sort array of lenghs for words
	std::sort(auxLenghtWords, auxLenghtWords + sizeof(auxLenghtWords) / sizeof(int));

	// Create list of words according to the sizes (Descending size of words)
	std::vector<std::string> lWords;
	std::string alphabet = { "ABCDEFGHIJKLMNOPQRSTUVW" };
	for (int i = 19; i >= 0; i--)
	{
		std::string word = "";
		for (int l = 0; l < auxLenghtWords[i]; l++)
		{
			int rWord = rand() % alphabet.length();
			word += alphabet[rWord];
		}
		lWords.push_back(word);
	}
	

	// FOR TESTING PUSH CORRECT WORDS
	/*lWords.clear();
	lWords.push_back("COOPERATE");
	lWords.push_back("BURGLARY");
	lWords.push_back("SAFFRON");
	lWords.push_back("DENTIST");
	lWords.push_back("RENEW");
	lWords.push_back("CREW");
	lWords.push_back("RUST");
	lWords.push_back("EWE");*/

	/*lWords.push_back("RUST");
	lWords.push_back("TUS");
	lWords.push_back("TO");*/



	int indexW = 0;
	std::string word = lWords[indexW];


		
	// Put first word in random position
	// Check if this word fits down direction
	if (word.size() < ROWS)
	{
		int row = rand() % (ROWS - word.size());
		int col = rand() % COLS;

		setDownWord(word, row, col, 0);
	}
	else if (word.size() < COLS)
	{
		int row = rand() % ROWS;
		int col = rand() % (COLS - word.size());
		
		// DEBUG
		//row = 6;
		//col = 1;

		setAcrossWord(word, row, col, 0);
	}

	// Remove word from the grid
	lWords.erase(lWords.begin());
	//indexW++;
	
	while (lWords.size() > 0)
	{
		word = lWords[0];

		// Remove word from the list
		lWords.erase(lWords.begin());

		std::vector<Cell> lSugestedCoords;
		// Cycle each letter in the given word
		for (int iLetter = 0; iLetter < word.size(); iLetter++)
		{
			for (int iRow = 0; iRow < ROWS; iRow++)
			{
				for (int iCol = 0; iCol < COLS; iCol++)
				{
					// Find a match ind grid
					char gridLetter = grid[iRow][iCol].Label();
					char auxLetter = word[iLetter];
					if (gridLetter == auxLetter)
					{
						// Filter this match according its size
						Cell auxC(grid[iRow][iCol].GetRow(), grid[iRow][iCol].GetColumn());
						
						auxC.SetIndexLetter(iLetter);
						auxC.SetScore(-1);
						auxC.SetAcross(false);
						auxC.SetDown(false);


						// Check across
						if (((iCol + (word.size() - iLetter)) < COLS) &&
							((iCol - iLetter) >= 0))
						{
							auxC.SetAcross(true);
						}

						// Check down
						if (((iRow + (word.size() - iLetter)) < ROWS) &&
							((iRow - iLetter) >= 0))
						{
							auxC.SetDown(true);
						}

						if (auxC.Across() || auxC.Down())
						{
							lSugestedCoords.push_back(auxC);
						}
					}

				}
			}

		}
		

		if (lSugestedCoords.size() > 0)
		{
			// Get final score
			Cell finalCell;
			//int iFinalCell = -1;
			int maxScore = -1;
			//int iLetter = -1;

			for (int i = 0; i < lSugestedCoords.size(); i++)
			{
				// Check each coord in down or across, and get the score for that one
				// acording with the number of matches in grid
				if (lSugestedCoords[i].Down())
				{
					int score = GetDownScore(lSugestedCoords[i], word);
					if (score > maxScore)
					{
						//iFinalCell = i;

						maxScore = score;

						finalCell = lSugestedCoords[i];
						/*finalCell.SetPosition(lSugestedCoords[i].GetRow(), lSugestedCoords[i].GetColumn());
						finalCell.SetLabel(lSugestedCoords[i].Label);*/

						finalCell.SetDown(true);
						finalCell.SetAcross(false);
					}

				}
				else if (lSugestedCoords[i].Across())
				{
					int score = GetAcrossScore(lSugestedCoords[i], word);
					if (score > maxScore)
					{
						maxScore = score;
						finalCell = lSugestedCoords[i];

						/*iFinalCell = i;
						maxScore = score;*/

						finalCell.SetDown(false);
						finalCell.SetAcross(true);
					}
					
				}
			}

			if (maxScore > -1)
			{
				// Get Cell from lSugestedCoords with iFinalCell
				//finalCell				

				// Put this labels for this coords
				if (finalCell.Down())
				{
					setDownWord(word, finalCell.GetRow(), finalCell.GetColumn(), finalCell.IndexLetter());
				}
				else if (finalCell.Across())
				{
					setAcrossWord(word, finalCell.GetRow(), finalCell.GetColumn(), finalCell.IndexLetter());
				}

			}

		}

		
	}

}


void Maze::setDownWord(std::string word, int row, int col, int indexLetter)
{
	// First letter
	grid[row][col].SetLabel(word[indexLetter]);
	grid[row][col].SetTypeCell(Cell::WALK);

	// Rest of the letters
	int auxRow = row + 1;
	for (int i = (indexLetter + 1); i < word.size(); i++)
	{
		grid[auxRow][col].SetLabel(word[i]);
		grid[auxRow][col].SetTypeCell(Cell::WALK);
		auxRow += 1;
	}

	auxRow = row - 1;
	for (int i = (indexLetter - 1); i >= 0; i--)
	{
		grid[auxRow][col].SetLabel(word[i]);
		grid[auxRow][col].SetTypeCell(Cell::WALK);
		auxRow -= 1;
	}
}

void Maze::setAcrossWord(std::string word, int row, int col, int indexLetter)
{
	// First letter
	grid[row][col].SetLabel(word[indexLetter]);
	grid[row][col].SetTypeCell(Cell::WALK);

	// Rest of the letters
	int auxCol = col + 1;
	for (int i = (indexLetter + 1); i < word.size(); i++)
	{		grid[row][auxCol].SetLabel(word[i]);
		grid[row][auxCol].SetTypeCell(Cell::WALK);
		auxCol += 1;
	}

	auxCol = col - 1;
	for (int i = (indexLetter - 1); i >= 0; i--)
	{
		grid[row][auxCol].SetLabel(word[i]);
		grid[row][auxCol].SetTypeCell(Cell::WALK);
		auxCol -= 1;
	}
}
int Maze::GetDownScore(const Cell currentCell, const std::string currentWord)
{
	int scoreCell = -1;
	bool test = true;
	bool first = CheckFirstDown(currentCell.IndexLetter(), currentCell.GetRow(), currentCell.GetColumn());
	bool last = CheckLastDown(currentCell.IndexLetter(), currentWord.size(), currentCell.GetRow(), currentCell.GetColumn());

	if (!first || !last) test = false;

	if (test)
	{
		// Go down from this position until  reach last row or las index
		int auxILetter = currentCell.IndexLetter() + 1;
		int auxRow = currentCell.GetRow() + 1;
		scoreCell = 0;

		while ((auxILetter < currentWord.size()) && (auxRow < ROWS) && test)
		{
			char gridLetter = grid[auxRow] [currentCell.GetColumn()].Label();
			char currentLetter = currentWord[auxILetter];
			int auxScore = 0;

			// Check if is the last letter, if an empty letter
			bool empty = CheckFirstDown(auxILetter, auxRow, currentCell.GetColumn());
			if (empty)
			{
				int auxScore = 0;
				test = CheckLeftRight(gridLetter, currentLetter, auxRow, currentCell.GetColumn(), auxScore);
				if (test)
				{
					scoreCell += auxScore;
					auxILetter++;
					auxRow++;
				}
			}
			else
			{
				test = false;
			}

		}
	}

	if (test)
	{
		// Go up from this position until reach first row or first index
		int auxILetter = currentCell.IndexLetter() - 1;
		int auxRow = currentCell.GetRow() - 1;
		while ((auxILetter >=0) && (auxRow >=0) && test)
		{
			char gridLetter = grid[auxRow][currentCell.GetColumn()].Label();
			char currentLetter = currentWord[auxILetter];
			int auxScore = 0;

			bool empty = CheckFirstDown(currentCell.IndexLetter(), auxRow, currentCell.GetColumn());
			if (empty)
			{
				test = CheckLeftRight(gridLetter, currentLetter, auxRow, currentCell.GetColumn(), auxScore);
				if (test)
				{
					scoreCell += auxScore;
					auxILetter--;
					auxRow--;
				}
			}
			else
			{
				test = false;
			}
		}
	}

	if (test)
	{
		return scoreCell;
	}

	return -1;

}

int  Maze::GetAcrossScore(const Cell currentCell, const std::string currentWord)
{
	int scoreCell = -1;
	bool test = true;


	bool first = CheckFirstAcross(currentCell.IndexLetter(), currentCell.GetRow(), currentCell.GetColumn());
	bool last = CheckLastAcross(currentCell.IndexLetter(), currentWord.size(), currentCell.GetRow(), currentCell.GetColumn());

	if (!first || !last) test = false;


	if (test)
	{
		// Go down from this position until  reach last row or las index
		int auxILetter = currentCell.IndexLetter() + 1;

		int auxCol = currentCell.GetColumn() + 1;
		scoreCell = 0;

		while ((auxILetter < currentWord.size()) && (auxCol < COLS) && test)
		{
			char gridLetter = grid[currentCell.GetRow()][auxCol].Label();
			char currentLetter = currentWord[auxILetter];
			int auxScore = 0;

			// Check if is the last letter, if an empty letter
			bool empty = CheckLastAcross(auxILetter, currentWord.size(),currentCell.GetRow(), currentCell.GetColumn());
			if (empty)
			{
				int auxScore = 0;

				test = CheckTopBottom(gridLetter, currentLetter, currentCell.GetRow(), auxCol, auxScore);

				if (test)
				{
					scoreCell += auxScore;
					auxILetter++;
					auxCol++;
				}
			}
			else
			{
				test = false;
			}
		}
	}

	if (test)
	{
		// Go up from this position until reach first row or first index
		int auxILetter = currentCell.IndexLetter() - 1;
		int auxCol = currentCell.GetColumn() - 1;
		while ((auxILetter >= 0) && (auxCol >= 0) && test)
		{
			char gridLetter = grid[currentCell.GetRow()][auxCol].Label();
			char currentLetter = currentWord[auxILetter];
			int auxScore = 0;

			bool empty = CheckFirstAcross(auxILetter, currentCell.GetRow(), auxCol);
			if (empty)
			{
				int auxScore = 0;

				test = CheckTopBottom(gridLetter, currentLetter, currentCell.GetRow(), auxCol, auxScore);
				if (test)
				{
					scoreCell += auxScore;
					auxILetter--;
					auxCol--;
				}
			}
			else
			{
				test = false;
			}

		}
	}

	if (test)
	{
		return scoreCell;
	}

	return -1;

}

bool Maze::CheckFirstDown(int index, int row, int col) const
{
	// Check if last index and coord inside boundaries
	if ((index == 0) && ((row - 1) >= 0))
	{
		if (grid[row - 1][col].Label() != '0')
		{
			return false;
		}
	}
	return true;
}

bool Maze::CheckLastDown(int index, int lenght, int row, int col) const
{
	// Check if last index and coord inside boundaries
	if ((index == (lenght - 1)) && ((row + 1) < ROWS))
	{
		if (grid[row + 1][col].Label() != '0')
		{
			return false;
		}
	}
	return true;
}

bool Maze::CheckFirstAcross(int index, int row, int col) const
{
	// Check if last index and coord inside boundaries
	if ((index == 0) && ((col - 1) >= 0))
	{
		if (grid[row ][col - 1].Label() != '0')
		{
			return false;
		}
	}
	return true;
}

bool Maze::CheckLastAcross(int index, int lenght, int row, int col) const
{
	// Check if last index and coord inside boundaries
	if ((index == (lenght - 1)) && ((col + 1) < COLS))
	{
		if (grid[row][col + 1].Label() != '0')
		{
			return false;
		}
	}
	return true;
}



bool Maze::CheckLeftRight(char label, char currentLabel, int row, int col,int& score)
{
	score = 0;

	if (label != '0')
	{
		if (label == currentLabel)
		{
			score = 1;
			return true;
		}
		return false;
	}
	else
	{
		// Empty char, check if on left and righ col there is an empty letter
		// left
		if ((col - 1) >= 0)
		{
  			if (grid[row][col - 1].Label() != '0')
			{
				return false;
			}
		}

		// Rigth
		if ((col + 1) < COLS)
		{
			if (grid[row][col + 1].Label() != '0')
			{
				return false;
			}
		}
	}

	return true;

}

bool Maze::CheckTopBottom(char label, char currentLabel, int row, int col, int& score)
{
	score = 0;

	if (label != '0')
	{
		if (label == currentLabel)
		{
			score = 1;
			return true;
		}
		return false;
	}
	else
	{
		// Empty char, check if on left and righ col there is an empty letter
		// top
		if ((row - 1) >= 0)
		{
			if (grid[row - 1][col].Label() != '0')
			{
				return false;
			}
		}

		// Rigth
		if ((row + 1) < ROWS)
		{
			if (grid[row + 1][col].Label() != '0')
			{
				return false;
			}
		}
	}

	return true;

}


void Maze::Draw(Graphics& gfx) const
{
	Vei2 topLeft(50,50);

	int cellSize = 30;
	int border = 3;

	Color noWalkColor = { 59,59,59 };
	Color walkColor = { 110,206,238 };
	Color borderColor = { 200,200,200 };
	

	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			RectI cell(topLeft.x + (col * cellSize),
				topLeft.x + ((col * cellSize) + cellSize),
				topLeft.y + (row * cellSize),
				topLeft.y + ((row * cellSize) + cellSize));

			RectI border(cell.left - border, cell.right + border, cell.top - border, cell.bottom + border);

			/*if (grid[row][col].TypeCell() == Cell::NOWALK)
			{
				gfx.DrawRect(border, borderColor);
				gfx.DrawRect(cell, noWalkColor);
			}
			else if (grid[row][col].TypeCell() == Cell::WALK)
			{
				gfx.DrawRect(border, borderColor);
				gfx.DrawRect(cell, walkColor);
			}*/


			if (grid[row][col].Label() == '0')
			{
				gfx.DrawRect(border, borderColor);
				gfx.DrawRect(cell, noWalkColor);
			}
			else if (grid[row][col].Label() != '0')
			{
				gfx.DrawRect(border, borderColor);
				gfx.DrawRect(cell, walkColor);
			}			
			
		}
	}

}










