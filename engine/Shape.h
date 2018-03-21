#pragma once

#include "Definitions.h"

namespace RecursiveMapGenerator
{
	class Shape
	{
	public:

		Shape();
		Shape(unsigned int in_x, unsigned int in_y, EShapeType in_shapeType);

		unsigned int GetX()  const;
		void SetX(unsigned int in_x);

		unsigned int GetY() const;
		void SetY(unsigned int in_y);

		unsigned int GetSize() const;
		void SetSize(unsigned int in_size);

		EShapeType GetType() const;
		void SetType(EShapeType in_type);


	private:
		unsigned int x;
		unsigned int y;
		EShapeType shapeType;
		unsigned int size;
	};
}