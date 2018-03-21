#pragma once


namespace RecursiveMapGenerator
{
	enum EShapeType 
	{
		NONE = -1,
		DOWN = 0,  // | Shape
		ACROSS, // -- Shape
		L, // L Shape (Down-Right direction)
		MIRROR_L, // Mirror L Shape (Down-Left direction)
		UPSIDEDOWN_L, // Upside down L  (Left - Down direction)
		UPSIDEDOWN_MIRROR_L // Upside down Mirror L Shape  (Right - Down direction)

	};

	// Definitions for the solutions recursive map
	enum EMapDefinitions { XCOLS = 8, YROWS = 17, TOTALSHAPES = 14, SIZESHAPE = 3 };

	// Tile types that we can find in the map
	enum ETileType { 
		CLEAN,  // Free tile
		OCCUPIED, // Tile occupied
		OBSTACLE, // Obstacle
		SPAWN // Spawn position
	};

	struct Coords
	{
		int x;
		int y;

		Coords()
		{
			x = 0;
			y = 0;
		}
		Coords(int inX, int inY)
		{
			x = inX;
			y = inY;
		}
	};

	template< unsigned W, unsigned H, typename TYPE = int>
	class Matrix2D
	{
	public:

		TYPE matrix[W][H];

		Matrix2D() {}

		Matrix2D(const Matrix2D & other)
		{
			*this = other;
		}

		Matrix2D(const TYPE matrix)
		{
			*this = matrix;
		}

		// Assign operator
		Matrix2D & operator = (const Matrix2D & other)
		{
			const TYPE * src = &(other.matrix[0][0]);
			TYPE * dst = &(this->matrix[0][0]);

			for (unsigned i = 0; i < W*H; i++)
			{
				*dst++ = *src++;
			}

			return (*this);
		}

		Matrix2D & operator = (const TYPE matrix)
		{
			TYPE * dst = &this->matrix[0];

			for (unsigned i = 0; i < W*H; i++)
			{
				*dst++ = matrix;
			}

			return (*this);
		}

		void Initialize(int value)
		{
			for (int x = 0; x<W; x++)
			{
				for (int y = 0; y<H; y++)
				{
					matrix[x][y] = value;
				}
			}
		}
	};


}