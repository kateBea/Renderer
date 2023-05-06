/**
 * @file Model.hh
 * @author kT
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

namespace kT {
    class Model {
    public:
        /**
         * Loads an object model from the given path. If the path is not valid
         * @param path path to the model to be loaded
         * @throws std::runtime_error if the file does not exist or the path is invalid
         * */
        explicit Model(const std::filesystem::path& path);

        /**
         * Renders Model mesh using the provided shader
         * @param shader shader used to render this Model
         * */
        auto draw(const Shader& shader) -> void;

        /**
         * Copy constructor disabled. Use the default constructor
         * to load a new Model
         * */
        Model(const Model& other)           = delete;

        /**
         * Copy assigment disabled. Use the default constructor
         * to load a new Model
         * */
        auto operator=(const Model& other)  = delete;

        /**
         * Moves other model to the implicit parameter
         * @param other moved from Model
         * */
        Model(Model&& other) noexcept;

        /**
         * Moves other model to the implicit parameter
         * @param other moved from Model
         * @return *this
         * */
        auto operator=(Model&& other) noexcept -> Model&;

    private:
        /**
         * Helper function to load model resources from given path
         * @param path path to the model to be loaded
         * @throws std::runtime_error if the file does not exist or the path is invalid
         * */
        auto load(const std::filesystem::path& path) -> void;


        // ASSIMP INTERFACE HELPER FUNCTIONS

        /**
         * Retrieves each one of the meshes contained within the scene
         * into this Model. This process starts from the given node
         * traversing all of its children nodes
         * */
        auto processNode(aiNode* node, const aiScene* scene) -> void;

        /**
         * Helper functions to construct a kT::Mesh from the data
         * contained within the node
         * */
        auto processMesh(aiMesh* node, const aiScene* scene) -> Mesh;

        /**
         * Retrieves texture materials from the given aiMaterial
         * */
        auto loadMaterialTextures(aiMaterial *mat, aiTextureType type, kT::Texture::TextureType tType,
                                  const aiScene* scene) -> std::vector<kT::Texture>;


        /**
         * Helper functions to construct a 3D vector from a aiVector3D object
         * */
        static auto constructVec3(aiVector3D elem) -> glm::vec3;


        std::vector<Mesh>       m_Meshes{};
        std::filesystem::path   m_ModelPath{};

    };

}

#endif // MODEL_HH
