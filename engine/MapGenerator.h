#pragma once
#include <vector>
#include "Definitions.h"
#include "Shape.h"

#include "Graphics.h"

using namespace std;

namespace RecursiveMapGenerator
{
	class MapGenerator
	{

	public:

		MapGenerator();

		void GenerateGrid(Matrix2D<EMapDefinitions::XCOLS, EMapDefinitions::YROWS, int> grid, Coords coord, EShapeType shapeType, int totalShapes);

		void Draw(Graphics& gfx) const;


		int GetShapesNumber() const;
		int GetSolutionNumber() const;


		vector<Matrix2D<EMapDefinitions::XCOLS, EMapDefinitions::YROWS, int>> SolutionList;

	private:
		// Test if a shape is suitable for a given coord
		void tryPlaceShape(Matrix2D<EMapDefinitions::XCOLS, EMapDefinitions::YROWS, int> * grid, Coords coord, EShapeType shapeType, int totalSteps, bool & testShape);
		void placeShape(Matrix2D<EMapDefinitions::XCOLS, EMapDefinitions::YROWS, int> * grid, Shape * shape, Coords coord, int totalSteps);
		void coverShape(Matrix2D<EMapDefinitions::XCOLS, EMapDefinitions::YROWS, int> * grid, const Shape shape, Coords coord, int totalSteps);
		void cover(Matrix2D<EMapDefinitions::XCOLS, EMapDefinitions::YROWS, int> * grid, int x, int y, int totalSteps, ETileType tileType);

		Coords findNextSpawnLocation(Matrix2D<EMapDefinitions::XCOLS, EMapDefinitions::YROWS, int> * grid, Coords coord);

	private:
		// Number shapes
		//const int sizeShape = 3;
		int shapesNumber = 0;
		int solutionNumber = 0;

		Matrix2D<EMapDefinitions::XCOLS, EMapDefinitions::YROWS, int> * currentGrid;

	};
}
