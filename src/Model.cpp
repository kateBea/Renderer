// C++ Standard Library
#include <array>
#include <utility>
#include <cstddef>

// Project Libraries
#include "OpenGL/Model.hh"

namespace kT {
    Model::Model(const std::filesystem::path& path)
        :   m_ModelPath{ path.string().substr(0,  path.string().find_last_of('/')) }
    {
        load(path);
    }

    auto Model::LoadFromFile(const std::string path) -> void {
        m_ModelPath = path.substr(0, path.find_last_of('/'));
        load(path);
    }

    auto Model::load(const std::filesystem::path& path) -> void {
        if (!path.has_filename())
            throw std::runtime_error("Not valid path for model object");

        std::array<char, 4096> fileDir{};
#if  defined(_WIN64) || defined(WIN32)
        wcstombs_s(nullptr, fileDir.data(), fileDir.size(), path.c_str(), 4096);
#else
        std::copy(path.native().begin(), path.native().end(), fileDir.begin());
#endif

        Assimp::Importer importer{};

        // See more postprocessing options: https://assimp.sourceforge.net/lib_html/postprocess_8h.html
        auto scene = importer.ReadFile(fileDir.data(),
                                                 aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
        if((scene == nullptr) || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || (scene->mRootNode == nullptr))
            throw std::runtime_error(importer.GetErrorString());

        m_Meshes.reserve(scene->mRootNode->mNumMeshes);
        processNode(scene->mRootNode, scene);
    }

    auto Model::processNode(aiNode* root, const aiScene* scene) -> void {
        // Process all the meshes from this node
        for(std::size_t i{}; i < root->mNumMeshes; i++)
            m_Meshes.push_back(std::move(processMesh(scene->mMeshes[root->mMeshes[i]], scene)));

        // then do the same for each of its children
        for(std::size_t i {}; i < root->mNumChildren; i++)
                processNode(root->mChildren[i], scene);
    }

    auto Model::processMesh(aiMesh* mesh, const aiScene* scene) -> kT::Mesh {
        std::vector<float> vertices{};
        std::vector<std::uint32_t> indices{};
        std::vector<kT::Texture> textures{};

        for(std::size_t i = 0; i < mesh->mNumVertices; i++) {
            vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });

            if (mesh->HasNormals())
                vertices.insert(vertices.end(), { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z });
            if (mesh->mTextureCoords[0] != nullptr)
                vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
        }

        // Retrieve mesh indices
        for(std::size_t i{}; i < mesh->mNumFaces; i++) {
            auto face{ mesh->mFaces[i] };

            for(std::size_t index{}; index < face.mNumIndices; index++)
                indices.push_back(face.mIndices[index]);
        }

        // process material
        if(mesh->mMaterialIndex >= 0) {
            auto material { scene->mMaterials[mesh->mMaterialIndex] };

            auto diffuseMaps { loadMaterialTextures(material, aiTextureType_DIFFUSE, kT::Texture::TextureType::DIFFUSE, scene) };
            auto specularMaps { loadMaterialTextures(material, aiTextureType_SPECULAR, kT::Texture::TextureType::SPECULAR, scene) };
            auto normalMaps { loadMaterialTextures(material, aiTextureType_NORMALS, kT::Texture::TextureType::NORMAL, scene) };

            for (auto& item : diffuseMaps)
                textures.push_back(std::move(item));

            for (auto& item : specularMaps)
                textures.push_back(std::move(item));

            for (auto& item : normalMaps)
                textures.push_back(std::move(item));
        }

        return Mesh{ vertices, indices, std::move(textures) };
    }

    auto Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, kT::Texture::TextureType tType, const aiScene* scene) -> std::vector<kT::Texture> {
        std::vector<kT::Texture> textures{};
        for(std::uint32_t i{}; i < mat->GetTextureCount(type); i++) {
            aiString str{};
            if (mat->GetTexture(type, i, &str) == AI_SUCCESS)
                // TODO: str might not be the name of a texture and instead hold the texture index. Right we assume textures are in same directory as the model
                textures.push_back(kT::Texture::fromFile(m_ModelPath.string() + '/' + str.C_Str(), tType));
        }

        return textures;
    }

    Model::Model(Model &&other) noexcept
        :   m_Meshes{ std::move(other.m_Meshes) }, m_ModelPath{ std::move(other.m_ModelPath) }
    {}

    auto Model::operator=(Model&& other) noexcept -> Model& {
        m_Meshes = std::move(other.m_Meshes);
        m_ModelPath = std::move(other.m_ModelPath);

        return *this;
    }

    auto Model::getVertexCount() const -> std::size_t {
        std::size_t total{ 0 };

        for (const auto& it : m_Meshes)
            total += it.getVertexCount();
        return total;
    }

    auto Model::getIndexCount() const -> std::size_t {
        std::size_t total{ 0 };
        for (const auto& it : m_Meshes)
            total += it.getIndexCount();
        return total;
    }

    auto Model::getTextureCount() const -> std::size_t {
        std::size_t total{ 0 };
        for (const auto& it : m_Meshes)
            total += it.getTextureCount();
        return total;
    }
}