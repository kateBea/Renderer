//
// Created by kate on 3/17/2023.
//

#ifndef RENDERER_OBJ_PARSER_H
#define RENDERER_OBJ_PARSER_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace kate {
    inline auto get_vertices_data(const std::filesystem::path& path) -> std::vector<float> {

        // parses files with vertices in the format:
        /**
         *    //   POSITIONS        TEXTURES
         *   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         *    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         *    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         *    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         *   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         *   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         *
         *  see assets/vertices.txt for an example
         * */

        // This function will only return the values as floats contained with in
        // a vector. User must ensure to specify buffer layout with appropriate function calls
        std::ifstream file{ path };
        std::string temp{};
        std::vector<float> retVal{};

        if (file.is_open()) {
            while (std::getline(file, temp, ','))
                if (not temp.starts_with("//"))
                    retVal.push_back(std::stof(temp));
        }
        else
            std::cerr << "Could not open file containing vertices data...\n";

        return retVal;
    }
}
#endif //RENDERER_OBJ_PARSER_H
