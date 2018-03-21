/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	table({ 1,8 }, { 7, 2 })
{
	mazeA.Init();
	mazeA.Generate();


	// Recursive map generation
	Matrix2D<EMapDefinitions::XCOLS, EMapDefinitions::YROWS, int> auxMap;
	auxMap.Initialize(ETileType::CLEAN);

	Coords coord(0, 0);
	MapGenerator generatedMap;
	generatedMap.GenerateGrid(auxMap, coord, EShapeType::NONE, EMapDefinitions::TOTALSHAPES);

	int numSolutions = generatedMap.SolutionList.size();
	

	// Take first solution
	if (numSolutions > 0)
	{
		SolutionList = generatedMap.SolutionList;
	}

}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	if (wnd.kbd.KeyIsPressed(VK_UP))
	{
	}
	else if (wnd.kbd.KeyIsPressed(VK_DOWN))
	{

	}
	else if (wnd.kbd.KeyIsPressed(VK_LEFT))
	{
	}
	else if (wnd.kbd.KeyIsPressed(VK_RIGHT))
	{
		currentSolution += 1;
		if (currentSolution >= SolutionList.size())
		{
			currentSolution = 0;
		}
	}
}

void Game::ComposeFrame()
{
	//maze.Draw(gfx);

	//mazeA.Draw(gfx);

	//table.Draw(gfx);
	
	
	if (SolutionList.size() > 0)
	{
		Vei2 topLeft(50, 50);

		int cellSize = 25;
		int border = 3;

		Color noWalkColor = { 59,59,59 };
		Color walkColor = { 110,206,238 };
		Color borderColor = { 200,200,200 };

		Color startNode = { 255,181,51 };
		Color endNode = { 166, 255,51 };

		for (int x = 0; x < EMapDefinitions::XCOLS; x++)
		{
			for (int y = 0; y < EMapDefinitions::YROWS; y++)
			{
				RectI cell(topLeft.x + (x * cellSize),
					topLeft.x + ((x * cellSize) + cellSize),
					topLeft.y + (y * cellSize),
					topLeft.y + ((y * cellSize) + cellSize));

				RectI border(cell.left - border, cell.right + border, cell.top - border, cell.bottom + border);

				Color colorCell = { 0,0,0 };
				if (SolutionList[currentSolution].matrix[x][y] == ETileType::OCCUPIED)
				{
					colorCell = { 110,206,238 };

				}
				else
				{
					colorCell = { 59,59,59 };

				}


				gfx.DrawRect(border, borderColor);
				gfx.DrawRect(cell, colorCell);

			}
		}
	}

}
