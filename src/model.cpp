#include "model.hpp"

#include "vertex.hpp"
#include "peria_types.hpp"
#include "asset_manager.hpp"

namespace {

[[nodiscard]]
peria::graphics::Texture_Type get_type_name(aiTextureType type)
{
    if (type == aiTextureType_DIFFUSE) {
        return peria::graphics::Texture_Type::DIFFUSE;
    }
    if (type == aiTextureType_SPECULAR) {
        return peria::graphics::Texture_Type::SPECULAR;
    }
    return peria::graphics::Texture_Type::NONE;
}

}

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
    peria::log(directory);
    peria::log("SCENE MESH COUNT:", scene->mNumMeshes);
    peria::log("SCENE Material COUNT:", scene->mNumMaterials);
    meshes.reserve(scene->mNumMeshes);

    process_node(scene->mRootNode, scene);
}

void Model::process_node(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for(u32 i{}; i<node->mNumMeshes; ++i) {
        auto mesh {scene->mMeshes[node->mMeshes[i]]};
        meshes.emplace_back(process_mesh(mesh, scene));
    }

    // then do the same for each of its children
    for(u32 i{}; i<node->mNumChildren; ++i) {
        process_node(node->mChildren[i], scene);
    }
}

std::unique_ptr<Mesh> Model::process_mesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<vertex::Vertex3d> vertices; vertices.reserve(mesh->mNumVertices);
    std::vector<u32> indices; indices.reserve(4096);

    for(u32 i{}; i<mesh->mNumVertices; ++i) {
        vertex::Vertex3d vertex {};

        if (mesh->HasPositions()) {
            vertex.pos.x = mesh->mVertices[i].x;
            vertex.pos.y = mesh->mVertices[i].y;
            vertex.pos.z = mesh->mVertices[i].z;
        }
        if (mesh->HasNormals()) {
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
        }
        if (mesh->HasTextureCoords(0)) {
            vertex.tex_coordinates.x = mesh->mTextureCoords[0][i].x;
            vertex.tex_coordinates.y = mesh->mTextureCoords[0][i].y;
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

    load_material_textures(material, aiTextureType_DIFFUSE);
    load_material_textures(material, aiTextureType_SPECULAR);

    return std::make_unique<Mesh>(std::move(vertices), std::move(indices), std::move(texture_paths_names));
}


void Model::load_material_textures(aiMaterial* mat, aiTextureType type)
{
    for(u32 i{}; i<mat->GetTextureCount(type); ++i) {
        aiString str;
        mat->GetTexture(type, i, &str);

        auto path {directory + "/" + std::string{str.C_Str()}};
        auto skip {false};
        for (u32 j{}; j<texture_paths_names.size(); ++j) {
            if (texture_paths_names[j] == path) {
                skip = true;
                break;
            }
        }
        if (!skip) {
            Asset_Manager::instance()->load_texture(path.c_str(), get_type_name(type));
            texture_paths_names.emplace_back(path);
        }
    }
}

void Model::draw(const Shader* const shader)
{
    for (const auto& m:meshes) {
        //m->draw(shader);
    }
}


const std::vector<Mesh*> Model::get_meshes() const
{ 
    std::vector<Mesh*> v; v.reserve(meshes.size());
    for (std::size_t i{}; i<meshes.size(); ++i) {
        v.emplace_back(meshes[i].get());
    }
    return v;
}

}
