#include "repch.h"

#include "Rogue/Scene/Scene.h"
#include "Rogue/Scene/Components.h"

#include "Rogue/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

namespace Rogue
{

	static void DoMath(const glm::mat4& transform)
	{

	}

	static void OnTransformConstruct(entt::registry& registry, entt::entity entity)
	{

	}

	Scene::Scene()
	{
#if ENTT_EXAMPLE
		entt::entity entity{ m_Registry.create() };
		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();

		if (m_Registry.any_of<TransformComponent>(entity))
			TransformComponent& transform{ m_Registry.get<TransformComponent>(entity) };

		auto view{ m_Registry.view<TransformComponent>() };
		for (auto entity : view)
		{
			auto& transform{ m_Registry.get<TransformComponent>(entity) };
		}

		auto group{ m_Registry.group<TransformComponent>(entt::get<MeshComponent>) };
		for (auto entity : group)
		{
			auto&[transform, mesh]{ m_Registry.get<TransformComponent, MeshComponent>(entity) };

			// Renderer::Submit(mesh, transform);
		}
#endif
	}

	Scene::~Scene()
	{

	}

	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto&& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}
}