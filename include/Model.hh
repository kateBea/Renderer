//
// Created by zanet on 4/25/2023.
//

#ifndef MODEL_HH
#define MODEL_HH

// C++ Standard Library
#include <vector>

// Project Libraries
#include "Mesh.hh"

namespace Kate {
    class Model {
    public:
        enum class Axis {
            X_AXIS,
            Y_AXIS,
            Z_AXIS,
            LIMIT_AXIS,
        };

        explicit Model() = default;

        auto rotate(double degrees, Axis axis) -> void;
    private:
        std::vector<Kate::Mesh> components{};

    };

}

#endif // MODEL_HH
