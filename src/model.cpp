#include "model.hpp"

#include "vertex.hpp"
#include "peria_types.hpp"

namespace peria::graphics {

Model::Model(const std::string& path)
{
    Assimp::Importer import;
    const aiScene* scene {import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs)};	
    peria::log(path);
    if(!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode) {
        peria::log("ERROR::ASSIMP::", import.GetErrorString());
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    process_node(scene->mRootNode, scene);
    peria::log("deda movtyaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaan");
}

void Model::process_node(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for(u32 i{}; i<node->mNumMeshes; ++i) {
        auto mesh {scene->mMeshes[node->mMeshes[i]]};
        meshes.emplace_back(process_mesh(mesh, scene));
    }

    // then do the same for each of its children
    peria::log(node->mNumChildren);
    for(u32 i{}; i<node->mNumChildren; ++i) {
        process_node(node->mChildren[i], scene);
    }
    peria::log(meshes.size());
}

std::unique_ptr<Mesh> Model::process_mesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<vertex::Vertex3d> vertices; vertices.reserve(512);
    std::vector<u32> indices; indices.reserve(512);

    for(u32 i{}; i<mesh->mNumVertices; ++i) {
        vertex::Vertex3d vertex {
            {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z},
            {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z},
            {}
        };

        if (mesh->mTextureCoords[0]) {
            vertex.tex_coordinates = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
        }
        vertices.emplace_back(vertex);
    }

    for(u32 i{}; i<mesh->mNumFaces; ++i) {
        auto face {mesh->mFaces[i]};

        for(u32 j{}; j<face.mNumIndices; ++j) {
            indices.emplace_back(face.mIndices[j]);
        }
    }

    aiMaterial* material {scene->mMaterials[mesh->mMaterialIndex]};
    auto diffuse_texture_indices {load_material_textures(material, aiTextureType_DIFFUSE, "u_texture_diffuse")};
    auto specular_texture_indices {load_material_textures(material, aiTextureType_SPECULAR, "u_texture_specular")};
    std::vector<u32> texture_inds; texture_inds.reserve(diffuse_texture_indices.size() + specular_texture_indices.size());
    for (auto t:diffuse_texture_indices) {
        texture_inds.emplace_back(t);
    }
    for (auto t:specular_texture_indices) {
        texture_inds.emplace_back(t);
    }

    // 1. diffuse maps
    //auto diffuse_maps {load_material_textures(material, aiTextureType_DIFFUSE, "u_texture_diffuse")};
    //for (auto&& t:diffuse_maps) {
    //    textures.emplace_back(std::move(t));
    //}

    //// 2. specular maps
    //auto specular_maps {load_material_textures(material, aiTextureType_SPECULAR, "u_texture_specular")};
    //for (auto&& t:specular_maps) {
    //    textures.emplace_back(std::move(t));
    //}
    return std::make_unique<Mesh>(std::move(vertices), std::move(indices), std::move(texture_inds));
}


std::vector<u32>
Model::load_material_textures(aiMaterial* mat, aiTextureType type, std::string type_name)
{
    std::vector<u32> texture_indices; texture_indices.reserve(4);
    for(u32 i{}; i<mat->GetTextureCount(type); ++i) {
        aiString str;
        mat->GetTexture(type, i, &str);

        auto path {directory + "/" + std::string{str.C_Str()}};
        auto skip {false};
        u32 index {0};
        for (u32 j{}; j<textures.size(); ++j) {
            if (textures[j]->get_path() == path) {
                skip = true;
                index = j;
                break;
            }
        }
        if (!skip) {
            textures.emplace_back(std::make_unique<Texture>(path.c_str(), type_name));
            index = textures.size()-1;
        }
        texture_indices.emplace_back(index);
    }
    return texture_indices;
}

void Model::draw(const Shader* const shader, const glm::mat4& projection, const peria::graphics::Camera& camera)
{
    std::vector<Texture*> ts;
    for (auto& x:textures) {
        ts.emplace_back(x.get());
    }
    for (const auto& m:meshes) {
        m->draw(shader, projection, camera, ts);
    }
}

}
