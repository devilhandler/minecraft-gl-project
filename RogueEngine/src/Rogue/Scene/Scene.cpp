#include "repch.h"

#include <glm/glm.hpp>

#include "Rogue/Scene/Scene.h"
#include "Rogue/Scene/Components.h"
#include "Rogue/Scene/Entity.h"

#include "Rogue/Renderer/Renderer2D.h"


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

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity{ m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag{ entity.AddComponent<TagComponent>() };
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}


	void Scene::OnUpdate(Timestep ts)
	{	
		// Update scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
				{
					if (!nsc.Instance)
					{
						nsc.InstantiateFunction();
						nsc.Instance->m_Entity = Entity{ entity, this };
						nsc.OnCreateFunction(nsc.Instance);
					}

					nsc.OnUpdateFunction(nsc.Instance, ts);
				});
		}

		// Render 2D
		Camera* mainCamera{ nullptr };
		glm::mat4* cameraTransform{ nullptr };
		{
			auto view{ m_Registry.view<TransformComponent, CameraComponent>() };
			for (auto entity : view)
			{
				auto&& [transform, camera] {view.get<TransformComponent, CameraComponent>(entity)};

				if (camera.Primary)
				{
					if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
					{
						camera.Camera.SetViewportSize(
							m_ViewportWidth,
							m_ViewportHeight
						);
					}

					mainCamera = &camera.Camera;
					cameraTransform = &transform.Transform;
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, *cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto&& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transform, sprite.Color);
			}

			Renderer2D::EndScene();
		}


	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0)
		{
			RE_CORE_WARN("Scene::OnViewportResize: Width or height is 0");
			return;
		}


		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view{ m_Registry.view<CameraComponent>() };
		for (auto entity : view)
		{
			auto& camera{ view.get<CameraComponent>(entity) };
			if (!camera.FixedAspectRatio)
				camera.Camera.SetViewportSize(width, height);
		}
	}

}