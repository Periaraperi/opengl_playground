#pragma once

#include <string>
#include <vector>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <glm/mat4x4.hpp>

#include "mesh.hpp"
#include "shader.hpp"

namespace peria::graphics {
class Model {
public:
    explicit Model(const std::string& path);
    //void draw(const Shader* const shader, const glm::mat4& projection, const Camera& camera);
    void draw(const Shader* const shader);
private:
    std::vector<std::unique_ptr<peria::graphics::Mesh>> meshes;
    std::string directory;
    std::vector<std::string> texture_paths_names;

    void process_node(aiNode* node, const aiScene* scene);

    [[nodiscard]]
    std::unique_ptr<Mesh> process_mesh(aiMesh* mesh, const aiScene* scene);

    void load_material_textures(aiMaterial* mat, aiTextureType type, std::string type_name);
};

}
