//#include "TextRenderer.h"
//
//TextRenderer::TextRenderer(int width, int height, Shader& shader) : shader(shader)
//{
//	if (FT_Init_FreeType(&ft))
//		std::cerr << "FreeType failed to initialize!" << std::endl;
//
//	projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
//
//	// Create a vertex buffer object and vertex array object for rendering the quads
//	// We reserve enough memory so that we can later update the memory when rendering characters
//	glGenVertexArrays(1, &vertexArrayObject);
//	glGenBuffers(1, &vertexBufferObject);
//	glBindVertexArray(vertexArrayObject);
//	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
//	// 6 vertices to create a quad, with 4 floats each
//	// (2 for position, 2 for texture coordinates)
//	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//}
//
//void TextRenderer::RenderText(Font& font, const std::string& text, float x, float y, float scale,
//	const glm::vec3& color, bool isCentered)
//{
//	// Activate corresponding render state
//	shader.Bind();
//	shader.Update(projection);
//	glUniform3f(glGetUniformLocation(shader.GetProgram(), "textColor"), color.x, color.y, color.z);
//	glActiveTexture(GL_TEXTURE0);
//	glBindVertexArray(vertexArrayObject);
//
//	// Disable depth test; text should draw over other text
//	glDisable(GL_DEPTH_TEST);
//
//	// Find the width of the text for centering
//	float textWidth = 0.0f, textAdvance = 0.0f;
//	// Loop over all characters in the text to get the sizes
//	for (char c : text)
//	{
//		Font::Character character = font.GetCharacter(c);
//		float xPosition = textAdvance + character.bearing.x * scale;
//		float width = character.size.x * scale;
//
//		if (xPosition + width > textWidth)
//		{
//			textWidth = xPosition + width;
//		}
//
//		// Advance x position for next glyph
//		// Note that advance is the number of 1/64 pixels
//		textAdvance += (character.advance >> 6) * scale; // Bitshift by 6 (2^6 = 64)
//	}
//
//	// Loop over all characters in the text to render
//	for (char c : text)
//	{
//		Font::Character character = font.GetCharacter(c);
//
//		float xPosition = x + character.bearing.x * scale;
//		float yPosition = y - (character.size.y - character.bearing.y) * scale;
//
//		float width = character.size.x * scale;
//		float height = character.size.y * scale;
//
//		if (isCentered)
//			xPosition -= textWidth / 2;
//
//		// Update vertex buffer object for each character
//		// 6 vertices to create a quad, with 4 floats each
//		// (2 for position, 2 for texture coordinates)
//		float vertices[6][4] = {
//			{ xPosition,         yPosition + height, 0.0f, 0.0f },
//			{ xPosition,         yPosition,          0.0f, 1.0f },
//			{ xPosition + width, yPosition,          1.0f, 1.0f },
//
//			{ xPosition,         yPosition + height, 0.0f, 0.0f },
//			{ xPosition + width, yPosition,          1.0f, 1.0f },
//			{ xPosition + width, yPosition + height, 1.0f, 0.0f }
//		};
//
//		// Render glyph texture over quad
//		glBindTexture(GL_TEXTURE_2D, character.textureID);
//
//		// Update content of vertex buffer object memory
//		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
//		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//		// Render quad
//		glDrawArrays(GL_TRIANGLES, 0, 6);
//
//		// Advance x position for next glyph
//		// Note that advance is the number of 1/64 pixels
//		x += (character.advance >> 6) * scale; // Bitshift by 6 (2^6 = 64)
//	}
//
//	// Re-enable depth test
//	glEnable(GL_DEPTH_TEST);
//
//	glBindVertexArray(0);
//	glBindTexture(GL_TEXTURE_2D, 0);
//}
//
//TextRenderer::~TextRenderer()
//{
//	FT_Done_FreeType(ft);
//}