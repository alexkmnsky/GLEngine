#include "TexturePacker.h"

TexturePacker::TexturePacker(glm::ivec2 initialSize)
{
	rootNode = std::shared_ptr<TextureNode>(new TextureNode(glm::ivec2(0, 0), initialSize));
	ResizeBuffer(initialSize);
}

TexturePacker::~TexturePacker()
{

}

TexturePacker::TextureNode* TexturePacker::Pack(TextureNode* node, const glm::ivec2& size)
{
	if (!node->empty)
	{
		// The node is filled, cannot fit anything else here
		assert(!node->left && !node->right);
		return NULL;
	}
	else if (node->left && node->right)
	{
		// Non-leaf, try inserting to the left and then to the right
		TextureNode* returnValue = Pack(node->left.get(), size);
		if (returnValue != NULL)
		{
			return returnValue;
		}
		return Pack(node->right.get(), size);
	}
	else
	{
		// This is an unfilled leaf; see if it can be filled
		glm::ivec2 realSize(node->size.x, node->size.y);

		// If we are along a boundary, calculate the actual size
		if (node->origin.x + node->size.x == INT_MAX)
		{
			realSize.x = textureSize.x - node->origin.x;
		}
		if (node->origin.y + node->size.y == INT_MAX)
		{
			realSize.y = textureSize.y - node->origin.y;
		}

		if (node->size.x == size.x && node->size.y == size.y)
		{
			// Perfect size; just pack into this node
			node->empty = false;
			return node;
		}
		else if (realSize.x < size.x || realSize.y < size.y)
		{
			// Not big enough
			return NULL;
		}
		else
		{
			// Large enough; split until we get a perfect fit
			TextureNode* left;
			TextureNode* right;

			// Determine how much space we will have left if we split each way
			int remainX = realSize.x - size.x;
			int remainY = realSize.y - size.y;

			// Split the way that will leave the most room
			bool verticalSplit = remainX < remainY;
			if (remainX == 0 && remainY == 0)
			{
				// Edge case; we are going to hit the border of the texture atlas perfectly, split 
				// at the border instead
				if (node->size.x > node->size.y)
				{
					verticalSplit = false;
				}
				else
				{
					verticalSplit = true;
				}
			}

			if (verticalSplit)
			{
				// Split vertically (left is top)
				left = new TextureNode(node->origin, glm::ivec2(node->size.x, size.y));
				right = new TextureNode(glm::ivec2(node->origin.x, node->origin.y + size.y),
					glm::ivec2(node->size.x, node->size.y - size.y));
			}
			else
			{
				// Split horizontally
				left = new TextureNode(node->origin, glm::ivec2(size.x, node->size.y));
				right = new TextureNode(glm::ivec2(node->origin.x + size.x, node->origin.y), glm::ivec2(node->size.x - size.x, node->size.y));
			}

			node->left = std::shared_ptr<TextureNode>(left);
			node->right = std::shared_ptr<TextureNode>(right);
			return Pack(node->left.get(), size);
		}
	}
}

glm::ivec2 TexturePacker::PackTexture(unsigned char* textureBuffer, const glm::ivec2& bufferSize)
{
	TextureNode* node = Pack(rootNode.get(), bufferSize);
	if (node == NULL)
	{
		glm::ivec2 newSize(textureSize.x * 2, textureSize.y * 2);
		ResizeRootNode(newSize);
		ResizeBuffer(newSize);

		node = Pack(rootNode.get(), bufferSize);

		// Note: this assert will be hit if we try to pack a texture larger
		// than the current size of the texture
		assert(node != NULL);
	}

	assert(bufferSize.x == node->size.x);
	assert(bufferSize.y == node->size.y);

	// Copy the texture to the texture atlas' buffer
	for (int ly = 0; ly < bufferSize.y; ly++)
	{
		for (int lx = 0; lx < bufferSize.x; lx++)
		{
			int y = node->origin.y + ly;
			int x = node->origin.x + lx;
			buffer[y * textureSize.x + x] = textureBuffer[ly * bufferSize.x + lx];
		}
	}

	return node->origin;
}

void TexturePacker::ResizeBuffer(const glm::ivec2& newSize)
{
	std::vector<unsigned char> newBuffer;
	newBuffer.resize(newSize.y * newSize.x);
	for (unsigned int y = 0; y < textureSize.y; y++)
	{
		for (unsigned int x = 0; x < textureSize.x; x++)
		{
			newBuffer[y * newSize.x + x] = buffer[y * textureSize.x + x];
		}
	}

	textureSize = newSize;
	buffer = std::move(newBuffer);
}

void TexturePacker::ResizeRootNode(const glm::ivec2& newSize)
{
	std::shared_ptr<TextureNode> newRootNode(new TextureNode(glm::ivec2(0, 0), newSize));

	TextureNode* node = Pack(newRootNode.get(), rootNode->size);
	node->left = rootNode->left;
	node->right = rootNode->right;
	node->empty = true;

	rootNode = newRootNode;
}
