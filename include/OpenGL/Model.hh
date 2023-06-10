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
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

// Project Libraries
#include "Mesh.hh"
#include "Shader.hh"

namespace kT {
    class Model {
    public:
        explicit Model() = default;
        /**
         * Loads an object model from the given path. If the path is not valid
         * this function raises an exception
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
        Model(const Model& other) = delete;

        auto LoadFromFile(const std::string path) -> void;

        /**
         * Copy assigment disabled. Use the default constructor
         * to load a new Model
         * */
        auto operator=(const Model& other) -> Model& = delete;

        /**
         * Moves <code>other</code> model to the implicit parameter
         * @param other moved from Model
         * */
        Model(Model&& other) noexcept;

        /**
         * Moves other model to the implicit parameter
         * @param other moved from Model
         * @returns *this
         * */
        auto operator=(Model&& other) noexcept -> Model&;

        auto getVertexCount() const -> std::size_t;
        auto getIndexCount() const -> std::size_t;
        auto getTextureCount() const -> std::size_t;

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
         * @param root contains components of the given scene
         * @param scene represents a complete scene, which contains aiNodes and the associated meshes, materials, etc.
         * */
        auto processNode(aiNode* root, const aiScene* scene) -> void;

        /**
         * Retrieves the components of the Mesh contained within
         * the given node from the given scene
         * @param node contains components of the Mesh
         * @param scene represents a complete scene, which contains aiNodes and the associated meshes, materials, etc.
         * @returns mesh containing the retrieved data
         * */
        auto processMesh(aiMesh* node, const aiScene* scene) -> Mesh;

        /**
         * Retrieves texture materials from the given aiMaterial
         * @param mat container of the materials
         * @param type type of texture to be processed
         * @param tType specifies the type of texture for the <b>kT::Texture</b> object
         * @param scene represents a complete scene, which contains aiNodes and the associated meshes, materials, etc.
         * @returns a list of textures from the given material of type <code>type</code>
         * */
        auto loadMaterialTextures(aiMaterial* mat, aiTextureType type, kT::Texture::TextureType tType,
                                  const aiScene* scene) -> std::vector<kT::Texture>;


        /**
         * Helper functions to construct a 3D vector from a aiVector3D object. This function
         * is more of a helper mainly used in <b>processNode(aiNode*, const aiScene*)</b> to construct
         * the vertices of each mesh
         * @param elem data structure from which the components are retrieved
         * @return 3D vector containing contents of elem
         * @returns a three dimensional vector
         * */
        static auto constructVec3(aiVector3D elem) -> glm::vec3;


        std::vector<Mesh>       m_Meshes{};
        std::filesystem::path   m_ModelPath{};
    };

}

#endif // MODEL_HH
