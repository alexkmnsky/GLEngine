#pragma once

#include <GLM/glm.hpp>

class MotionControl
{
public:
	void Add(glm::vec2 delta) { this->delta += delta; }
	void Add(float x, float y) { delta += glm::vec2(x, y); }
	void Set(glm::vec2 delta) { this->delta = delta; }
	void Set(float x, float y) { delta = glm::vec2(x, y); }
	glm::vec2 Get() { return delta; }

private:
	glm::vec2 delta;
};
