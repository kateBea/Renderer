//
// Created by zanet on 4/25/2023.
//

#ifndef MODEL_HH
#define MODEL_HH

// C++ Standard Library
#include <filesystem>
#include <vector>

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
        std::vector<Kate::Mesh> components{};

    };

}

#endif // MODEL_HH
