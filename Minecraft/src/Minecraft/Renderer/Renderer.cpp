#include "mcpch.h"
#include "Renderer.h"

namespace Minecraft
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(
		const std::shared_ptr<Shader> shader, 
		const std::shared_ptr<VertexArray>& vertexArray, 
		const glm::mat4& transform
	)
	{
		shader->Bind();
		shader->UploadUniformMat4("uViewProjection", m_SceneData->ViewProjectionMatrix);
		shader->UploadUniformMat4("uTransform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}