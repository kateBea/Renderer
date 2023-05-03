#include "../include/Model.hh"

#include <array>

namespace Kate {
    Model::Model(const std::filesystem::path& path)
        :   m_ModelPath{ path }
    {
        load(path);
    }

    auto Model::draw(const Shader &shader) -> void {
        for (const auto& mesh : m_Meshes)
            mesh.draw(shader);
    }

    auto Model::load(const std::filesystem::path& path) -> void {
        // 1. aiProcess_Triangulate we tell Assimp that if the model does not
        // (entirely) consist of triangles, it should transform all the
        // model's primitive shapes to triangles first
        //
        // 2. The aiProcess_FlipUVs flips the texture coordinates on the y-axis
        // where necessary during processing
        //
        // See more postprocessing options: https://assimp.sourceforge.net/lib_html/postprocess_8h.html

        std::array<char, 4096> fileDir{};
#ifdef WINDOWS
        // fileDir.size() will return the amount of elements of fileDir, since it contains char which are byte sized
        wcstombs_s(nullptr, fileDir.data(), fileDir.size(), path.c_str(), 4096);
#else

        std::copy(path.native().begin(), path.native().end(), fileDir.begin());
#endif

        Assimp::Importer importer{};
        const aiScene *scene = importer.ReadFile(fileDir.data(), aiProcess_Triangulate | aiProcess_FlipUVs);

        if((scene == nullptr) || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || scene->mRootNode == nullptr)
            throw std::runtime_error(importer.GetErrorString());


        processNode(scene->mRootNode, scene);
    }

    auto Model::processNode(aiNode* node, const aiScene* scene) -> void {

        // process all the node's meshes (if any)
        for(std::size_t i{}; i < node->mNumMeshes; i++)
            m_Meshes.push_back(std::move(processMesh(scene->mMeshes[node->mMeshes[i]], scene)));

        // then do the same for each of its children
        for(std::size_t i {}; i < node->mNumChildren; i++)
                processNode(node->mChildren[i], scene);
    }

    auto Model::processMesh(aiMesh* mesh, const aiScene* scene) -> Mesh {
        std::vector<Kate::Vertex> vertices;
        std::vector<std::uint32_t> indices;
        std::vector<Kate::Texture> textures;

        for(std::size_t i = 0; i < mesh->mNumVertices; i++) {
            Kate::Vertex vertex{};

            vertex.setPositions(getPosition(mesh->mVertices[i]));
            vertex.setNormals(getNormals(mesh->mVertices[i]));

            // does the mesh contain texture coordinates?
            if(mesh->mTextureCoords[0]) {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.setTextures(vec);
            }
            else
                vertex.setTextures(glm::vec2(0.0f, 0.0f));

            vertices.push_back(vertex);
        }

        // process indices
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        // process material
        if(mesh->mMaterialIndex >= 0) {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

            auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, Kate::Texture::TextureType::DIFFUSE);
            auto specularMaps = loadMaterialTextures(material,aiTextureType_SPECULAR, Kate::Texture::TextureType::SPECULAR);

            for (auto& item : diffuseMaps)
                textures.push_back(std::move(item));

            for (auto& item : specularMaps)
                textures.push_back(std::move(item));
        }

        return Mesh{ vertices, indices, textures };
    }

    auto Model::getPosition(aiVector3D elem) -> glm::vec3 {
        return { elem.x, elem.y, elem.y };
    }

    auto Model::getNormals(aiVector3D elem) -> glm::vec3 {
        return { elem.x, elem.y, elem.y };
    }

    auto Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, Kate::Texture::TextureType tType) -> std::vector<Kate::Texture> {
        std::vector<Kate::Texture> textures;

        for(std::size_t i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str{};
            mat->GetTexture(type, i, &str);
            textures.push_back(Kate::Texture::fromFile(str.C_Str(), tType));
        }

        return textures;
    }
}