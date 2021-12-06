#pragma once

#include <vector>
#include <unordered_map>
#include "Rendering/Mesh.h"
#include "AABB.h"
#include "Transform.h"

class Maze
{
public:
	/**
	 * Constructs the maze pseudo-randomly, using the seed specified.
	 * 
	 * @param width The width of the maze grid. Use an odd number for desirable results.
	 * @param height The height of the maze grid. Use an odd number for desirable results.
	 * @param seed The seed used for pseudo-random number generation.
	 */
	Maze(int width, int height, unsigned int seed);

	/**
	 * Generates a model with positions, texture coordinates, and normals.
	 * 
	 * Texture coordinates correspond to a texture atlas, where the top-left quarter of the texture.
	 * is reserved for the floor and the top-right quarter of the texture is reserved for the walls.
	 * 
	 * No unused geometry should be generated.
	 * 
	 * @return The model.
	 */
	IndexedModel GenerateModel();

	/**
	 * Generates the AABBs for each wall cell in the grid.
	 * 
	 * @param The transform to apply to each AABB.
	 * @return The AABB to use.
	 */
	AABB GenerateAABBs(std::vector<Transform>& transforms);

	static const float cellWidth;
	static const float floorHeight;
	static const float wallHeight;

private:
	enum class Direction
	{
		UP, 
		DOWN,
		LEFT,
		RIGHT
	};

	static const inline std::pair<int, int> DirectionToXY(Direction direction)
	{
		switch (direction)
		{
		case Maze::Direction::UP:
			return { 0, -1 };
		case Maze::Direction::DOWN:
			return { 0, 1 };
		case Maze::Direction::LEFT:
			return { -1, 0 };
		case Maze::Direction::RIGHT:
			return { 1, 0 };
		default:
			return { 0, 0 };
		}
	}

	/**
	 * Randomly generate the maze.
	 * 
	 * @param x Starting X position.
	 * @param y Starting Y position.
	 */
	void GenerateMaze(int x, int y);

	inline void SetEmpty(int x, int y) { grid[y][x] = false; }
	inline void SetWall(int x, int y) { grid[y][x] = true; }

	inline bool IsWall(int x, int y) const
	{
		// Check if the coordinates are within the maze grid
		if ((0 <= x && x < width) && (0 <= y && y < height))
			return grid[y][x];
		else
			return false;
	}

	int width;
	int height;
	unsigned int seed;

	// 2D grid, where true represents a wall cell and false represents an empty cell
	std::vector<std::vector<bool>> grid;
};

