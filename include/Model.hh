/**
 * @file Model.hh
 * @author Kate
 * @brief Defines the Model class
 * @version 1.0
 * @date 2023-04-30
 */

#ifndef MODEL_HH
#define MODEL_HH

// C++ Standard Library
#include <filesystem>
#include <string_view>
#include <stdexcept>
#include <cstdint>
#include <string>
#include <vector>

// Third Party Libraries
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Project Libraries
#include "Mesh.hh"
#include "Shader.hh"

namespace Kate {
    class Model {
    public:
        /**
         *
         * */
        explicit Model(const std::filesystem::path& path);

        /**
         *
         * */
        auto draw(const Shader& shader) -> void;

    private:
        /**
         *
         * */
        auto load(const std::filesystem::path& path) -> void;

        /**
         *
         * */
        auto processNode(aiNode* node, const aiScene* scene) -> void;

        /**
         *
         * */
        auto processMesh(aiMesh* node, const aiScene* scene) -> Mesh;

        auto loadMaterialTextures(aiMaterial *mat, aiTextureType type, Kate::Texture::TextureType tType) -> std::vector<Kate::Texture>;

        static auto getPosition(aiVector3D elem) -> glm::vec3;
        static auto getNormals(aiVector3D elem) -> glm::vec3;

        std::vector<Mesh> m_Meshes{};
        std::filesystem::path m_ModelPath{};

    };

}

#endif // MODEL_HH
