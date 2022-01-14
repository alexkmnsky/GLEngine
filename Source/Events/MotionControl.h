#pragma once

#include <GLM/glm.hpp>

class MotionControl
{
public:
	inline void Add(glm::ivec2 delta) { this->delta += delta; }
	inline void Add(int x, int y) { delta += glm::ivec2(x, y); }	
	inline void Set(glm::ivec2 delta) { this->delta = delta; }
	inline void Set(int x, int y) { delta = glm::ivec2(x, y); }
	inline glm::ivec2 Get() { return delta; }

private:
	glm::ivec2 delta;
};
