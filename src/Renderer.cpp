#include "OpenGL/Renderer.hh"
#include "OpenGL/Texture.hh"

namespace kT {
    auto Renderer::Init() -> void {
        s_VertexArray = std::make_shared<VertexArray>();
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    auto Renderer::ShutDown() -> void {

    }

    auto Renderer::EnableWireframeMode() -> void {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    auto Renderer::DisableWireframeMode() -> void {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    auto Renderer::DrawMesh(Shader& shader, const Mesh& mesh) -> void {
        const std::vector<Texture>& textures{ mesh.getTextures() };
        std::uint32_t diffuseCount{ 1 };
        std::uint32_t specularCount{ 1 };
        std::uint32_t normalCount{ 1 };

        for(std::int32_t i = 0; i < textures.size(); i++) {
            std::string number{};

            Texture::bindUnit(i);

            switch(textures[i].getType()) {

                case Texture::TextureType::DIFFUSE:
                    number = std::to_string(diffuseCount++);
                    break;
                case Texture::TextureType::SPECULAR:
                    number = std::to_string(specularCount++);
                    break;
                case Texture::TextureType::NORMAL:
                    number = std::to_string(normalCount++);
                    break;
            }

            shader.setUniformInt("material." + std::string(Texture::getStrType(textures[i].getType())), i);
            textures[i].bind();
        }

        shader.use();
        s_VertexArray->useVertexBuffer(mesh.getVertexBuffer());
        mesh.getIndexBuffer().bind();
        glDrawElements(GL_TRIANGLES, mesh.getIndexBuffer().getCount(), GL_UNSIGNED_INT, nullptr);
    }

    auto Renderer::DrawModel(Shader& shader, Model& model) -> void {
        for (const auto& mesh : model.getMeshes())
            Renderer::DrawMesh(shader, mesh);
    }

    auto Renderer::ClearColor(const glm::vec4 &color) -> void {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    auto Renderer::ClearColor(float red, float green, float blue, float alpha) -> void {
        glClearColor(red, green, blue, alpha);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    auto Renderer::DrawGeometry(Shader &shader, const VertexBuffer &vertexBuffer) -> void {
        glDrawArrays(GL_TRIANGLES, 0, vertexBuffer.getCount());
    }

    auto Renderer::DrawGeometry(Shader &shader, const VertexBuffer& vertexBuffer, const ElementBuffer &indexBuffer) -> void {
        s_VertexArray->useVertexBuffer(vertexBuffer);
        glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_INT, nullptr);
    }

    auto Renderer::ResetViewport(std::int32_t width, std::int32_t height) -> void {
        glViewport(0, 0, width, height);
    }

    auto Renderer::DrawGeometry(Shader &shader, std::initializer_list<float> &&vertexBuffer) -> void {
        // will use the default buffer layout
        VertexBuffer vertices{ std::forward<std::initializer_list<float>>(vertexBuffer) };
        DrawGeometry(shader, vertices);
    }

    auto Renderer::DrawGeometry(Shader &shader, std::initializer_list<float> &&vertexBuffer, std::initializer_list<std::uint32_t> &&indexBuffer) -> void {
        VertexBuffer vertices{ std::forward<std::initializer_list<float>>(vertexBuffer) };
        ElementBuffer indices{ std::forward<std::initializer_list<std::uint32_t>>(indexBuffer) };
        DrawGeometry(shader, vertices, indices);
    }

}