#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

int main() {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile("C:\\Users\\zanet\\Desktop\\Renderer\\assets\\models\\lowPolyG19\\source\\G19.blend", aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene) {
        return -1;
    }

    // Process the model data
    // ...

    return 0;
}
