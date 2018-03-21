#include "Shape.h"

namespace RecursiveMapGenerator
{
	Shape::Shape()
		:
		x(0),
		y(0),
		size(0),
		shapeType(EShapeType::NONE)
	{}

	Shape::Shape(unsigned int in_x, unsigned int in_y, EShapeType in_shapeType)
		:
		x(in_x),
		y(in_y),
		shapeType(in_shapeType),
		size(0)
	{}

	unsigned int Shape::GetX() const
	{
		return x;
	}
	void Shape::SetX(unsigned int in_x)
	{
		x = in_x;
	}
	unsigned int Shape::GetY() const
	{
		return y;
	}
	void Shape::SetY(unsigned int in_y)
	{
		y = in_y;
	}

	unsigned int Shape::GetSize() const
	{
		return size;
	}
	void Shape::SetSize(unsigned int in_size)
	{
		size = in_size;
	}

	EShapeType Shape::GetType() const
	{
		return shapeType;
	}
	void Shape::SetType(EShapeType in_type)
	{
		shapeType = in_type;
	}
}