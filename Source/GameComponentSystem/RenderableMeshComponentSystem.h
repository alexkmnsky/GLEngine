/*  
 * Copyright (c) 2022-2023 Alexander Kaminsky and Maxwell Hunt
 *  
 * This file incorporates work covered by the following copyright and permission notice:  
 *  
 *     Copyright (c) 2018 Benny Bobaganoosh
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *     
 *     The above copyright notice and this permission notice shall be included in all
 *     copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *     SOFTWARE.
 */

#pragma once

#include "ECS/ECS.h"
#include "TransformComponent.h"
#include "GameRenderContext.h"

/** @brief Component which defines the visible mesh of an entity. */
struct RenderableMeshComponent : public ECSComponent<RenderableMeshComponent>
{
	// The mesh to use
	VertexArray* mesh = nullptr;

	// The texture to apply onto the mesh
	Texture* texture = nullptr;
};

/** @brief System which draws visible mesh of the entity every update. */
class RenderableMeshSystem : public BaseECSSystem
{
public:
	/**
	 * @param context The game render context, which bridges the gap between the high-level and
	 * low-level rendering commands
	 */
	RenderableMeshSystem(GameRenderContext& context) : BaseECSSystem(), context(context)
	{
		AddComponentType(TransformComponent::ID);
		AddComponentType(RenderableMeshComponent::ID);
	}

	virtual void UpdateComponents(float deltaTime, BaseECSComponent** components)
	{
		TransformComponent* transform = (TransformComponent*)components[0];
		RenderableMeshComponent* mesh = (RenderableMeshComponent*)components[1];

		context.RenderMesh(*mesh->mesh, *mesh->texture, transform->transform.GetModel());
	}
private:
	GameRenderContext& context;
};
