/** Copyright (c) 2022-2023 Alexander Kaminsky and Maxwell Hunt */

#pragma once

#include "Texture.h"

#include <GLM/glm.hpp>
#include <memory>
#include <cassert>
#include <vector>

class TexturePacker
{
public:
	TexturePacker(glm::ivec2 initialSize);
	virtual ~TexturePacker();

	glm::ivec2 PackTexture(unsigned char* textureBuffer, const glm::ivec2& bufferSize);

	inline unsigned char* GetBuffer() { return buffer.data(); }
	inline glm::ivec2 GetTextureSize() { return textureSize; }

private:
	struct TextureNode
	{
		glm::ivec2 origin; // Top left of the rectangle this node represents
		glm::ivec2 size;   // Size of the rectangle this node represents
		bool empty;        // True if this node is a leaf and is filled

		std::shared_ptr<TextureNode> left;  // Left (or top) subdivision
		std::shared_ptr<TextureNode> right; // Right (or bottom) subdivision

		TextureNode(glm::ivec2 origin, glm::ivec2 size) : origin(origin), size(size), empty(true) {}
	};

	TextureNode* Pack(TextureNode* node, const glm::ivec2& size);
	void ResizeBuffer(const glm::ivec2& newSize);
	void ResizeRootNode(const glm::ivec2& newSize);

	glm::ivec2 textureSize;
	std::vector<unsigned char> buffer;
	std::shared_ptr<TextureNode> rootNode;
};
