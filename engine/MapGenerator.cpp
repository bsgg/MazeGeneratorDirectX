#include "MapGenerator.h"


namespace RecursiveMapGenerator
{
	MapGenerator::MapGenerator() :
		currentGrid(new Matrix2D<EMapDefinitions::XCOLS, EMapDefinitions::YROWS, int>())

	{
		//currentGrid = new Matrix2D<Definitions::XCOLS, Definitions::YCOLS, int>();
		currentGrid->Initialize(0);
	}



	void MapGenerator::GenerateGrid(Matrix2D<EMapDefinitions::XCOLS, EMapDefinitions::YROWS, int> grid, Coords coord, EShapeType shapeType, int totalShapes)
	{
		// No shapes (first time), call GenerateGrid with all 6 posibilites
		if (shapeType == EShapeType::NONE)
		{

			GenerateGrid(grid, coord, EShapeType::ACROSS, totalShapes);
			GenerateGrid(grid, coord, EShapeType::DOWN, totalShapes);

			GenerateGrid(grid, coord, EShapeType::L, totalShapes);
			GenerateGrid(grid, coord, EShapeType::MIRROR_L, totalShapes);

			GenerateGrid(grid, coord, EShapeType::UPSIDEDOWN_MIRROR_L, totalShapes);
			GenerateGrid(grid, coord, EShapeType::UPSIDEDOWN_L, totalShapes);
		}
		else
		{
			if (totalShapes > 0)
			{
				// test if if possible to locate the figure 
				bool testShape = false;
				tryPlaceShape(&grid, coord, shapeType, EMapDefinitions::SIZESHAPE - 1, testShape);

				// The shapes is correct
				if (testShape)
				{
					Shape s(coord.x, coord.y, shapeType);

					// Add 1 to the right coord if the type of shape is INVERT_LEFT_L
					if (shapeType == EShapeType::UPSIDEDOWN_L)
					{
						coord.x = coord.x + 1;
						s.SetX(coord.x);
						s.SetY(coord.y);
					}

					// Place shape
					placeShape(&grid, &s, coord, EMapDefinitions::SIZESHAPE - 1);

					if (s.GetSize() == EMapDefinitions::SIZESHAPE)
					{
						// cover shape
						coverShape(&grid, s, coord, EMapDefinitions::SIZESHAPE - 1);

						// Add 1 to number of shapes in grid
						shapesNumber += 1;
						totalShapes -= 1;

						// Find next spawn location in the grid for the shape
						coord = findNextSpawnLocation(&grid, coord);

						if ((coord.x > -1) && (coord.y > -1))
						{
							GenerateGrid(grid, coord, EShapeType::ACROSS, totalShapes);
							GenerateGrid(grid, coord, EShapeType::DOWN, totalShapes);

							GenerateGrid(grid, coord, EShapeType::L, totalShapes);
							GenerateGrid(grid, coord, EShapeType::MIRROR_L, totalShapes);

							GenerateGrid(grid, coord, EShapeType::UPSIDEDOWN_MIRROR_L, totalShapes);
							GenerateGrid(grid, coord, EShapeType::UPSIDEDOWN_L, totalShapes);
						}
					}

				}
				else
				{
					// Unable to place shape, try with another shape

					EShapeType initShape = shapeType;
					bool exit = false;
					do
					{
						int tShape = shapeType;

						// Check if the shape is the last one
						if ((tShape + 1) > EShapeType::UPSIDEDOWN_L)
						{
							tShape = EShapeType::DOWN;
						}
						else
						{
							tShape += 1;
						}

						shapeType = (EShapeType)tShape;

						if (tShape == initShape)
						{
							exit = true;
						}
						else
						{
							bool auxF = false;
							tryPlaceShape(&grid, coord, shapeType, EMapDefinitions::SIZESHAPE - 1, auxF);

							if (auxF)
							{
								exit = true;
							}

						}
					} while (!exit);

					if (shapeType != initShape)
					{
						GenerateGrid(grid, coord, shapeType, totalShapes);
					}
				}
			}
		}

		if (totalShapes == 0)
		{
			bool isDifferent = true;

			int nDifferentSolutions = 0;

			/*if (solutionList.size() > 0)
			{
			for (int k = 0; k < solutionList.size(); k++)
			{
			bool checkSolution = true;
			for (int x = 0; x < EGridDefinitions::XCOLS; x++)
			{
			for (int y = 0; y < EGridDefinitions::YROWS; y++)
			{
			if ((grid.matrix[x][y]) != (solutionList[k].matrix[x][y]))
			{
			checkSolution = false;
			break;
			}
			}
			if (!checkSolution)
			{
			break;
			}
			}
			}
			}*/

			if (isDifferent)
			{
				solutionNumber++;
				SolutionList.push_back(grid);
				currentGrid = &grid;
			}
		}

		shapesNumber = 0;
	}

	int MapGenerator::GetShapesNumber() const
	{
		return shapesNumber;
	}
	int MapGenerator::GetSolutionNumber() const
	{
		return solutionNumber;
	}

	void MapGenerator::tryPlaceShape(Matrix2D<EMapDefinitions::XCOLS, EMapDefinitions::YROWS, int> * grid, Coords coord, EShapeType shapeType, int totalSteps, bool & testShape)
	{

		if (totalSteps >= 0)
		{
			if ((grid->matrix[coord.x][coord.y] == ETileType::CLEAN) || (grid->matrix[coord.x][coord.y] == ETileType::SPAWN))
			{
				testShape = true;

				switch (shapeType)
				{
				case EShapeType::DOWN: // | Shape

									   // Check boundaries
					if ((coord.y + 1) < EMapDefinitions::YROWS)
					{
						coord.y = coord.y + 1;
					}
					else
					{
						// Check if more steps
						if (totalSteps > 0)
						{
							testShape = false;
						}
					}

					// If it's possible to place the shape, and we have more steps left, call the function again
					if (testShape && (totalSteps > 0))
					{
						// call test place shape
						tryPlaceShape(grid, coord, shapeType, totalSteps - 1, testShape);
					}

					break;
				case EShapeType::ACROSS: // -- Shape

										 // Check boundaries
					if ((coord.x + 1) < EMapDefinitions::XCOLS)
					{
						coord.x = coord.x + 1;
					}
					else
					{
						// Check if more steps
						if (totalSteps > 0)
						{
							testShape = false;
						}
					}

					// If it's possible to place the shape, and we have more steps left, call the function again
					if (testShape && (totalSteps > 0))
					{
						// call test place shape
						tryPlaceShape(grid, coord, shapeType, totalSteps - 1, testShape);
					}

					break;
				case EShapeType::L:  // L Shape (Down-Right direction)

					if (totalSteps == 2) // Step down
					{
						if ((coord.y + 1) < EMapDefinitions::YROWS)
						{
							coord.y = coord.y + 1;
						}
						else
						{
							testShape = false;
						}

					}
					else if (totalSteps == 1) // Step right
					{
						if ((coord.x + 1) < EMapDefinitions::XCOLS)
						{
							coord.x = coord.x + 1;
						}
						else
						{
							testShape = false;
						}
					}

					if (testShape && (totalSteps > 0))
					{
						// call test place shape
						tryPlaceShape(grid, coord, shapeType, totalSteps - 1, testShape);
					}

					break;
				case EShapeType::MIRROR_L:  // Mirror L Shape (Down-Left direction)

					if (totalSteps == 2) // Step down
					{
						if ((coord.y + 1) < EMapDefinitions::YROWS)
						{
							coord.y = coord.y + 1;
						}
						else
						{
							testShape = false;
						}

					}
					else if (totalSteps == 1) // Step left
					{
						if ((coord.x - 1) >= 0)
						{
							coord.x = coord.x - 1;
						}
						else
						{
							testShape = false;
						}
					}

					if (testShape && (totalSteps > 0))
					{
						// call test place shape
						tryPlaceShape(grid, coord, shapeType, totalSteps - 1, testShape);
					}


					break;
				case EShapeType::UPSIDEDOWN_MIRROR_L: // L Shape Upside down (Right - Down direction)

					if (totalSteps == 2) // Step right
					{
						if ((coord.x + 1) < EMapDefinitions::XCOLS)
						{
							coord.x = coord.x + 1;
						}
						else
						{
							testShape = false;
						}

					}
					else if (totalSteps == 1) // Step down
					{
						if ((coord.y + 1) < EMapDefinitions::YROWS)
						{
							coord.y = coord.y + 1;
						}
						else
						{
							testShape = false;
						}
					}

					if (testShape && (totalSteps > 0))
					{
						// call test place shape
						tryPlaceShape(grid, coord, shapeType, totalSteps - 1, testShape);
					}

					break;
				case EShapeType::UPSIDEDOWN_L: // Mirror L Shape Upside down (Left - Down direction)

					if (totalSteps == 2) // Step left
					{
						if ((coord.x - 1) >= 0)
						{
							coord.x = coord.x - 1;
						}
						else
						{
							testShape = false;
						}

					}
					else if (totalSteps == 1) // Step down
					{
						if ((coord.y + 1) < EMapDefinitions::YROWS)
						{
							coord.y = coord.y + 1;
						}
						else
						{
							testShape = false;
						}
					}

					if (testShape && (totalSteps > 0))
					{
						// call test place shape
						tryPlaceShape(grid, coord, shapeType, totalSteps - 1, testShape);
					}
					break;
				default: // Any other shape
					testShape = false;
					break;
				}
			}
			else // Any other type of tile
			{
				testShape = false;
			}
		}
	}

	void MapGenerator::placeShape(Matrix2D<EMapDefinitions::XCOLS, EMapDefinitions::YROWS, int> * grid, Shape * shape, Coords coord, int totalSteps)
	{
		if ((grid->matrix[coord.x][coord.y] == ETileType::CLEAN) || (grid->matrix[coord.x][coord.y] == ETileType::SPAWN))
		{
			// Set the tile as a occupied
			grid->matrix[coord.x][coord.y] = ETileType::OCCUPIED;
			// Increase the shape size
			shape->SetSize(shape->GetSize() + 1);
		}

		// Place shape according to the type of shape
		if (totalSteps > 0)
		{
			switch (shape->GetType())
			{
			case EShapeType::DOWN: // | Shape

								   // Check boundaries
				if ((coord.y + 1) < EMapDefinitions::YROWS)
				{
					coord.y = coord.y + 1;
					placeShape(grid, shape, coord, totalSteps - 1);
				}
				break;
			case EShapeType::ACROSS: // -- Shape

									 // Check boundaries
				if ((coord.x + 1) < EMapDefinitions::XCOLS)
				{
					coord.x = coord.x + 1;
					placeShape(grid, shape, coord, totalSteps - 1);
				}

				break;
			case EShapeType::L:  // L Shape (Down-Right direction)

								 // Down
				if (((coord.y + 1) < EMapDefinitions::YROWS) && (totalSteps > 1))
				{
					coord.y = coord.y + 1;
					placeShape(grid, shape, coord, totalSteps - 1);
				}

				// Rigth
				if (((coord.x + 1) < EMapDefinitions::XCOLS) && (totalSteps == 1))
				{
					coord.x = coord.x + 1;
					placeShape(grid, shape, coord, totalSteps - 1);
				}

				break;
			case EShapeType::MIRROR_L:  // Mirror L Shape (Down-Left direction)

										// Down
				if (((coord.y + 1) < EMapDefinitions::YROWS) && (totalSteps > 1))
				{
					coord.y = coord.y + 1;
					placeShape(grid, shape, coord, totalSteps - 1);
				}

				// Left
				if (((coord.x - 1) >= 0) && (totalSteps == 1))
				{
					coord.x = coord.x - 1;
					placeShape(grid, shape, coord, totalSteps - 1);
				}

				break;

			case EShapeType::UPSIDEDOWN_MIRROR_L: // L Shape Upside down (Right - Down direction)

												  // Rigth
				if (((coord.x + 1) < EMapDefinitions::XCOLS) && (totalSteps > 1))
				{
					coord.x = coord.x + 1;
					placeShape(grid, shape, coord, totalSteps - 1);
				}

				// Down
				if (((coord.y + 1) < EMapDefinitions::YROWS) && (totalSteps == 1))
				{
					coord.y = coord.y + 1;
					placeShape(grid, shape, coord, totalSteps - 1);
				}

				break;
			case EShapeType::UPSIDEDOWN_L: // Mirror L Shape Upside down (Left - Down direction)

										   // Left
				if (((coord.x - 1) >= 0) && (totalSteps > 1))
				{
					coord.x = coord.x - 1;
					placeShape(grid, shape, coord, totalSteps - 1);
				}

				// Down
				if (((coord.y + 1) < EMapDefinitions::YROWS) && (totalSteps == 1))
				{
					coord.y = coord.y + 1;
					placeShape(grid, shape, coord, totalSteps - 1);
				}
				break;
			}
		}
	}

	void MapGenerator::coverShape(Matrix2D<EMapDefinitions::XCOLS, EMapDefinitions::YROWS, int> * grid, const Shape shape, Coords coord, int totalSteps)
	{
		if (totalSteps >= 0)
		{
			// Cover the shape with obstacles
			cover(grid, coord.x, coord.y, 1, ETileType::OBSTACLE);

			switch (shape.GetType())
			{
			case EShapeType::DOWN:

				// Down
				if ((coord.y + 1) < EMapDefinitions::YROWS)
				{
					coord.y = coord.y + 1;
					coverShape(grid, shape, coord, totalSteps - 1);
				}

				break;

			case EShapeType::ACROSS: // -- Shape
									 // Rigth
				if ((coord.x + 1) < EMapDefinitions::XCOLS)
				{
					coord.x = coord.x + 1;
					coverShape(grid, shape, coord, totalSteps - 1);
				}
				break;

			case EShapeType::L:  // L Shape (Down-Right direction)

								 // Down
				if (((coord.y + 1) < EMapDefinitions::YROWS) && (totalSteps > 1))
				{
					coord.y = coord.y + 1;
					coverShape(grid, shape, coord, totalSteps - 1);
				}

				// Rigth
				if (((coord.x + 1) < EMapDefinitions::XCOLS) && (totalSteps == 1))
				{
					coord.x = coord.x + 1;
					coverShape(grid, shape, coord, totalSteps - 1);
				}

				break;
			case EShapeType::MIRROR_L:  // Mirror L Shape (Down-Left direction)

										// Down
				if (((coord.y + 1) < EMapDefinitions::YROWS) && (totalSteps > 1))
				{
					coord.y = coord.y + 1;
					coverShape(grid, shape, coord, totalSteps - 1);
				}

				// Left
				if (((coord.x - 1) >= 0) && (totalSteps == 1))
				{
					coord.x = coord.x - 1;
					coverShape(grid, shape, coord, totalSteps - 1);
				}

				break;

			case EShapeType::UPSIDEDOWN_MIRROR_L: // L Shape Upside down (Right - Down direction)

												  // Rigth
				if (((coord.x + 1) < EMapDefinitions::XCOLS) && (totalSteps > 1))
				{
					coord.x = coord.x + 1;
					coverShape(grid, shape, coord, totalSteps - 1);
				}

				// Down
				if (((coord.y + 1) < EMapDefinitions::YROWS) && (totalSteps == 1))
				{
					coord.y = coord.y + 1;
					coverShape(grid, shape, coord, totalSteps - 1);
				}

				break;
			case EShapeType::UPSIDEDOWN_L: // Mirror L Shape Upside down (Left - Down direction)

										   // Left
				if (((coord.x - 1) >= 0) && (totalSteps > 1))
				{
					coord.x = coord.x - 1;
					coverShape(grid, shape, coord, totalSteps - 1);
				}

				// Down
				if (((coord.y + 1) < EMapDefinitions::YROWS) && (totalSteps == 1))
				{
					coord.y = coord.y + 1;
					coverShape(grid, shape, coord, totalSteps - 1);
				}
				break;

			default:
				break;
			}
		}
	}

	void MapGenerator::cover(Matrix2D<EMapDefinitions::XCOLS, EMapDefinitions::YROWS, int> * grid, int x, int y, int totalSteps, ETileType tileType)
	{
		// Left
		if (x - 1 >= 0)
		{
			if ((grid->matrix[x - 1][y] == ETileType::CLEAN) || (grid->matrix[x - 1][y] == ETileType::SPAWN))
			{
				grid->matrix[x - 1][y] = tileType;
			}

			if (totalSteps > 0)
			{
				cover(grid, x - 1, y, 0, ETileType::SPAWN);
			}
		}

		// Up left
		if ((x - 1 >= 0) && (y - 1 >= 0))
		{
			if ((grid->matrix[x - 1][y - 1] == ETileType::CLEAN) || (grid->matrix[x - 1][y - 1] == ETileType::SPAWN))
			{
				grid->matrix[x - 1][y - 1] = tileType;
			}

			if (totalSteps > 0)
			{
				cover(grid, x - 1, y - 1, 0, ETileType::SPAWN);
			}
		}

		// Up
		if ((y - 1) >= 0)
		{
			if ((grid->matrix[x][y - 1] == ETileType::CLEAN) || (grid->matrix[x][y - 1] == ETileType::SPAWN))
			{
				grid->matrix[x][y - 1] = tileType;
			}

			if (totalSteps > 0)
			{
				cover(grid, x, y - 1, 0, ETileType::SPAWN);
			}
		}

		// Up right
		if (((x + 1) < EMapDefinitions::XCOLS) && ((y - 1) >= 0))
		{
			if ((grid->matrix[x + 1][y - 1] == ETileType::CLEAN) || (grid->matrix[x + 1][y - 1] == ETileType::SPAWN))
			{
				grid->matrix[x + 1][y - 1] = tileType;
			}

			if (totalSteps > 0)
			{
				cover(grid, x + 1, y - 1, 0, ETileType::SPAWN);
			}
		}

		// Right
		if ((x + 1) < EMapDefinitions::XCOLS)
		{
			if ((grid->matrix[x + 1][y] == ETileType::CLEAN) || (grid->matrix[x + 1][y] == ETileType::SPAWN))
			{
				grid->matrix[x + 1][y] = tileType;
			}

			if (totalSteps > 0)
			{
				cover(grid, x + 1, y, 0, ETileType::SPAWN);
			}
		}

		// Down right
		if (((y + 1) < EMapDefinitions::YROWS) && ((x + 1) < EMapDefinitions::XCOLS))
		{
			if ((grid->matrix[x + 1][y + 1] == ETileType::CLEAN) || (grid->matrix[x + 1][y + 1] == ETileType::SPAWN))
			{
				grid->matrix[x + 1][y + 1] = tileType;
			}

			if (totalSteps > 0)
			{
				cover(grid, x + 1, y + 1, 0, ETileType::SPAWN);
			}

		}

		// Down
		if ((y + 1) < EMapDefinitions::YROWS)
		{
			if ((grid->matrix[x][y + 1] == ETileType::CLEAN) || (grid->matrix[x][y + 1] == ETileType::SPAWN))
			{
				grid->matrix[x][y + 1] = tileType;
			}

			if (totalSteps > 0)
			{
				cover(grid, x, y + 1, 0, ETileType::SPAWN);
			}
		}

		// Down left
		if (((y + 1) < EMapDefinitions::YROWS) && ((x - 1) >= 0))
		{
			if ((grid->matrix[x - 1][y + 1] == ETileType::CLEAN) || (grid->matrix[x - 1][y + 1] == ETileType::SPAWN))
			{
				grid->matrix[x - 1][y + 1] = tileType;
			}

			if (totalSteps > 0)
			{
				cover(grid, x - 1, y + 1, 0, ETileType::SPAWN);
			}
		}
	}

	Coords MapGenerator::findNextSpawnLocation(Matrix2D<EMapDefinitions::XCOLS, EMapDefinitions::YROWS, int> * grid, Coords coord)
	{
		if (grid->matrix[coord.x][coord.y] == ETileType::SPAWN)
		{
			return coord;
		}
		else
		{
			if ((coord.x + 1) < EMapDefinitions::XCOLS)
			{
				coord.x = coord.x + 1;
				return findNextSpawnLocation(grid, coord);
			}
			else
			{
				if ((coord.y + 1) < EMapDefinitions::YROWS)
				{
					coord.x = 0;
					coord.y = coord.y + 1;
					return findNextSpawnLocation(grid, coord);
				}
				else
				{
					Coords c(-1, -1);
					return c;
				}
			}
		}
	}
}