#include "renderer_stdafx.h"

#include "Systems/RenderSystem.h"

#include "Context/Window.h"

#include "Components/MeshComponent.h"
#include "Components/MaterialComponent.h"

#include "Debug/Log.h"
#include "Resources/Mesh.h"

namespace LowRenderer {
	class MaterialComponent;
}

using namespace Core;
using namespace Renderer;
using namespace Renderer;
using namespace Renderer;

RenderSystem::RenderSystem(GLFWwindow* p_window) : m_window(p_window)
{
	InitOpenGL();
}

void RenderSystem::InitOpenGL() const
{
	glfwMakeContextCurrent(m_window);

	const GLenum err = glewInit();
	if (err == GLEW_OK)
	{
		LOG(Info(), "OpenGL Version:" << glGetString(GL_VERSION));
		glfwSetFramebufferSizeCallback(m_window, Context::Window::FramebufferSize);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	else
	{
		LOG(Error(), "GLEW Error initializing glew :" << glewGetErrorString(err));
	}
}

void RenderSystem::PreUpdate(double dt)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderSystem::PostUpdate(double dt)
{
	glfwSwapBuffers(m_window);
}

void RenderSystem::SetCamera(CameraGameObject* p_camera)
{
	m_activeCamera = p_camera;
}

void RenderSystem::Update(double dt)
{
	if (m_activeCamera)
	{
		ECS::ComponentManager* Cm = ECS::ECS_Engine->GetComponentManager();
		for (auto renderableIt = Cm->GetComponentContainer<MeshComponent>()->GetTComponentContainer().begin();
			renderableIt != Cm->GetComponentContainer<MeshComponent>()->GetTComponentContainer().end();
			++renderableIt)
		{
			MeshComponent* renderable = reinterpret_cast<MeshComponent*>(renderableIt->second.get());

			if (renderable->IsActive() && ECS::ECS_Engine->GetEntityManager()->GetEntity(renderable->GetOwnerID())->IsActive())
			{
				Resources::Mesh* model = renderable->GetModel();
				if (model)
				{
					MaterialComponent* material = Cm->GetComponent<MaterialComponent>(renderable->GetOwnerID());
					if (material)
					{
						Resources::Texture* texture = material->GetTexture();
						Resources::Shader*	shader	= material->GetShader();

						if (texture)
							texture->Bind();

						if (shader)
						{
							SceneSystem* sceneSystem = ECS::ECS_Engine->GetSystemManager()->GetSystem<SceneSystem>();
							const glm::mat4 transform = sceneSystem->WorldTransform(renderable->GetOwnerID());
							shader->Bind();

							shader->SetUniform("u_model", transform);
							shader->SetUniform("u_view", m_activeCamera->GetView());
							shader->SetUniform("u_proj", m_activeCamera->GetProjection());

							shader->SetUniform("u_viewPos", m_activeCamera->GetComponent<TransformComponent>()->AsTransform().GetPosition());

							shader->SetUniform("u_light.position", Maths::vec3f(0.0f, 2.0f, 4.0f));
							shader->SetUniform("u_light.ambient", Maths::vec3f(0.2f, 0.2f ,0.2f));
							shader->SetUniform("u_light.diffuse", Maths::vec3f(0.5f, 0.5f, 0.5f));
							shader->SetUniform("u_light.specular", Maths::vec3f(1.0f, 1.0f, 1.0f));

							shader->SetUniform("u_material.ambient", material->GetAmbient());
							shader->SetUniform("u_material.diffuse", material->GetDiffuse());
							shader->SetUniform("u_material.specular", material->GetSpecular());
							shader->SetUniform("u_material.shininess", material->GetShininess());
						}
					}
					renderable->GetVertexArray().Bind();

					model->GetIndexBuffer().Bind();

					glPolygonMode(GL_FRONT, GL_FILL);
					glDrawElements(GL_TRIANGLES, model->GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, nullptr);
					Cm->GetComponent<TransformComponent>(renderable->GetOwnerID())->AsTransform().HasChanged(false);
				}
			}
		}
	}
	else
	{
		LOG(Error(), "No active camera set !");
	}
}