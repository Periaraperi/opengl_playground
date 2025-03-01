#pragma once

#include <string>
#include <vector>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "mesh.hpp"

namespace peria {

class Model {
public:
    explicit Model(const std::string& path);

    [[nodiscard]]
    const std::vector<Mesh>& get_meshes() const;
private:
    std::vector<Mesh> meshes;
    std::string directory;

    void process_node(aiNode* node, const aiScene* scene);

    [[nodiscard]]
    Mesh process_mesh(aiMesh* mesh, const aiScene* scene);

    //void load_material_textures(aiMaterial* mat, aiTextureType type);
};

}
