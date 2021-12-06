#include "Maze.h"

#include <random>
#include <stack>

const float Maze::cellWidth = 2.0f;
const float Maze::floorHeight = 0.0f;
const float Maze::wallHeight = 2.0f;

Maze::Maze(int width, int height, unsigned int seed)
{
	this->width = width;
	this->height = height;
	this->seed = seed;

	grid = std::vector<std::vector<bool>>(height, std::vector<bool>(width, true));

	GenerateMaze(1, 1); // Start at the top left corner
}

IndexedModel Maze::GenerateModel()
{
	// Texture coordinates for the floor
	static const float textureFloorMinU = 0.0f;
	static const float textureFloorMaxU = 0.5f;
	static const float textureFloorMinV = 0.0f;
	static const float textureFloorMaxV = 0.5f;

	// Texture coordinates for the walls
	static const float textureWallMinU = 0.5f;
	static const float textureWallMaxU = 1.0f;
	static const float textureWallMinV = 0.0f;
	static const float textureWallMaxV = 0.5f;

	IndexedModel model;

	// Loop over all rows in the maze grid
	for (int row = 0; row < height; row++)
	{
		// Loop over all columns in the grid
		for (int column = 0; column < width; column++)
		{
			if (!IsWall(column, row))
			{
				unsigned int currentIndex;

				float minX = column * cellWidth;
				float maxX = (column + 1) * cellWidth;
				float minZ = -(row * cellWidth);
				float maxZ = -((row + 1) * cellWidth);

				currentIndex = model.positions.size();

				// Add 4 points of the floor
				model.positions.push_back(glm::vec3(minX, floorHeight, minZ));
				model.positions.push_back(glm::vec3(minX, floorHeight, maxZ));
				model.positions.push_back(glm::vec3(maxX, floorHeight, maxZ));
				model.positions.push_back(glm::vec3(maxX, floorHeight, minZ));

				// Set texture coordinates for the floor
				model.textureCoordinates.push_back(glm::vec2(textureFloorMinU, textureFloorMinV));
				model.textureCoordinates.push_back(glm::vec2(textureFloorMaxU, textureFloorMinV));
				model.textureCoordinates.push_back(glm::vec2(textureFloorMaxU, textureFloorMaxV));
				model.textureCoordinates.push_back(glm::vec2(textureFloorMinU, textureFloorMaxV));

				// Set normals to face up
				model.normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
				model.normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
				model.normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
				model.normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

				// Set indices for two triangles to form the quad
				// Ensure order is counter clockwise when facing the viewer for backface culling
				model.indices.push_back(currentIndex + 0);
				model.indices.push_back(currentIndex + 2);
				model.indices.push_back(currentIndex + 1);
				model.indices.push_back(currentIndex + 0);
				model.indices.push_back(currentIndex + 3);
				model.indices.push_back(currentIndex + 2);

				// North
				if (IsWall(column, row + 1))
				{
					currentIndex = model.positions.size();

					// Add North Wall
					model.positions.push_back(glm::vec3(minX, floorHeight, maxZ));
					model.positions.push_back(glm::vec3(minX, wallHeight, maxZ));
					model.positions.push_back(glm::vec3(maxX, wallHeight, maxZ));
					model.positions.push_back(glm::vec3(maxX, floorHeight, maxZ));

					// Set texture coordinates for the quad
					model.textureCoordinates.push_back(glm::vec2(textureWallMinU, textureWallMinV));
					model.textureCoordinates.push_back(glm::vec2(textureWallMaxU, textureWallMinV));
					model.textureCoordinates.push_back(glm::vec2(textureWallMaxU, textureWallMaxV));
					model.textureCoordinates.push_back(glm::vec2(textureWallMinU, textureWallMaxV));

					// Set normals to face south
					model.normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
					model.normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
					model.normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
					model.normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

					// Set indices for two triangles to form the quad
					// Ensure order is counter clockwise when facing the viewer for backface culling
					model.indices.push_back(currentIndex + 0);
					model.indices.push_back(currentIndex + 2);
					model.indices.push_back(currentIndex + 1);
					model.indices.push_back(currentIndex + 0);
					model.indices.push_back(currentIndex + 3);
					model.indices.push_back(currentIndex + 2);
				}

				// East
				if (IsWall(column + 1, row))
				{
					currentIndex = model.positions.size();

					// Add North Wall
					model.positions.push_back(glm::vec3(maxX, floorHeight, maxZ));
					model.positions.push_back(glm::vec3(maxX, wallHeight, maxZ));
					model.positions.push_back(glm::vec3(maxX, wallHeight, minZ));
					model.positions.push_back(glm::vec3(maxX, floorHeight, minZ));

					// Set texture coordinates for the quad
					model.textureCoordinates.push_back(glm::vec2(textureWallMinU, textureWallMinV));
					model.textureCoordinates.push_back(glm::vec2(textureWallMaxU, textureWallMinV));
					model.textureCoordinates.push_back(glm::vec2(textureWallMaxU, textureWallMaxV));
					model.textureCoordinates.push_back(glm::vec2(textureWallMinU, textureWallMaxV));

					// Set normals to face west
					model.normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
					model.normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
					model.normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
					model.normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));

					// Set indices for two triangles to form the quad
					// Ensure order is counter clockwise when facing the viewer for backface culling
					model.indices.push_back(currentIndex + 0);
					model.indices.push_back(currentIndex + 2);
					model.indices.push_back(currentIndex + 1);
					model.indices.push_back(currentIndex + 0);
					model.indices.push_back(currentIndex + 3);
					model.indices.push_back(currentIndex + 2);
				}

				// South
				if (IsWall(column, row - 1))
				{
					currentIndex = model.positions.size();

					// Add South Wall
					model.positions.push_back(glm::vec3(maxX, floorHeight, minZ));
					model.positions.push_back(glm::vec3(maxX, wallHeight, minZ));
					model.positions.push_back(glm::vec3(minX, wallHeight, minZ));
					model.positions.push_back(glm::vec3(minX, floorHeight, minZ));

					// Set texture coordinates for the quad
					model.textureCoordinates.push_back(glm::vec2(textureWallMinU, textureWallMinV));
					model.textureCoordinates.push_back(glm::vec2(textureWallMaxU, textureWallMinV));
					model.textureCoordinates.push_back(glm::vec2(textureWallMaxU, textureWallMaxV));
					model.textureCoordinates.push_back(glm::vec2(textureWallMinU, textureWallMaxV));

					// Set normals to face north
					model.normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
					model.normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
					model.normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
					model.normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));

					// Set indices for two triangles to form the quad
					// Ensure order is counter clockwise when facing the viewer for backface culling
					model.indices.push_back(currentIndex + 0);
					model.indices.push_back(currentIndex + 2);
					model.indices.push_back(currentIndex + 1);
					model.indices.push_back(currentIndex + 0);
					model.indices.push_back(currentIndex + 3);
					model.indices.push_back(currentIndex + 2);
				}

				// West
				if (IsWall(column - 1, row))
				{
					currentIndex = model.positions.size();

					// Add North Wall
					model.positions.push_back(glm::vec3(minX, floorHeight, minZ));
					model.positions.push_back(glm::vec3(minX, wallHeight, minZ));
					model.positions.push_back(glm::vec3(minX, wallHeight, maxZ));
					model.positions.push_back(glm::vec3(minX, floorHeight, maxZ));

					// Set texture coordinates for the quad
					model.textureCoordinates.push_back(glm::vec2(textureWallMinU, textureWallMinV));
					model.textureCoordinates.push_back(glm::vec2(textureWallMaxU, textureWallMinV));
					model.textureCoordinates.push_back(glm::vec2(textureWallMaxU, textureWallMaxV));
					model.textureCoordinates.push_back(glm::vec2(textureWallMinU, textureWallMaxV));

					// Set normals to face east
					model.normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
					model.normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
					model.normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
					model.normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

					// Set indices for two triangles to form the quad
					// Ensure order is counter clockwise when facing the viewer for backface culling
					model.indices.push_back(currentIndex + 0);
					model.indices.push_back(currentIndex + 2);
					model.indices.push_back(currentIndex + 1);
					model.indices.push_back(currentIndex + 0);
					model.indices.push_back(currentIndex + 3);
					model.indices.push_back(currentIndex + 2);
				}
			}
		}
	}

	return model;
}

AABB Maze::GenerateAABBs(std::vector<Transform>& transforms)
{
	// Loop over all rows in the maze grid
	for (int row = 0; row < height; row++)
	{
		// Loop over all columns in the grid
		for (int column = 0; column < width; column++)
		{
			if (IsWall(column, row))
			{
				glm::vec3 min(column * cellWidth, floorHeight, -row * cellWidth);
				glm::vec3 max((column + 1) * cellWidth, wallHeight, -(row + 1) * cellWidth);

				transforms.push_back(Transform((min + max) / 2.0f));
			}
		}
	}

	return AABB(
		glm::vec3(-cellWidth / 2, -(floorHeight + wallHeight)/2, -cellWidth / 2),
		glm::vec3(cellWidth / 2, (floorHeight + wallHeight) / 2, cellWidth / 2));
}

void Maze::GenerateMaze(int x, int y)
{
	// Using a stack allows us to perform a DFS iteratively
	// If performed recursively, large maze sizes will result in a stack overflow
	std::stack<std::pair<int, int>> stack;

	// Add the starting cell to the stack
	stack.push({ x, y });

	// Once the stack is empty, we have exhasuted all possible paths; the maze is done generating
	while (!stack.empty())
	{
		// Get the last element from the stack
		std::pair<int, int> current = stack.top();
		stack.pop();

		int x = current.first;
		int y = current.second;

		// Set the current cell to empty, so that we do not return here later
		SetEmpty(x, y);

		std::vector<Direction> directions = { Direction::UP, Direction::DOWN,
		Direction::LEFT, Direction::RIGHT };

		// Randomize the directions we try
		std::shuffle(directions.begin(), directions.end(), std::default_random_engine(seed * x + y));

		while (!directions.empty())
		{
			// Get and remove the next direction
			std::pair<int, int> nextDirection = DirectionToXY(
				directions[directions.size() - 1]);
			directions.pop_back();

			// Multiply by two as we are moving two cells in each direction to the next node
			int newX = x + nextDirection.first * 2;
			int newY = y + nextDirection.second * 2;

			// Check if the new node is a wall (it has not been visited)
			if (IsWall(newX, newY))
			{
				// Push the current cell to the stack
				stack.push({ x, y });

				// Set the cell between the two nodes to empty
				SetEmpty(x + nextDirection.first, y + nextDirection.second);

				// Continue from our new node
				stack.push({ newX, newY });

				break;
			}
		}
	}
}