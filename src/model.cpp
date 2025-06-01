#include "model.hpp"

#include "simple_logger.hpp"
#include "vertex.hpp"
#include "peria_types.hpp"

namespace peria {

Model::Model(std::vector<Mesh>&& meshes_)
    :meshes{std::move(meshes_)},
     directory{""}
{

}

Model::Model(const std::string& path)
    :directory{path.substr(0, path.find_last_of('/'))}
{
    Assimp::Importer import;
    const aiScene* scene {import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs)};	
    peria::log(path);

    if(!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode) {
        peria::log("ERROR::ASSIMP::", import.GetErrorString());
        return;
    }

    peria::log(directory);
    peria::log("SCENE MESH COUNT:",     scene->mNumMeshes);
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

Mesh Model::process_mesh(aiMesh* mesh, [[maybe_unused]] const aiScene* scene)
{
    std::vector<Vertex<Pos3D, Normal, TexCoord>> vertices; vertices.reserve(mesh->mNumVertices);
    std::vector<u32> indices; indices.reserve(8192);

    for(u32 i{}; i<mesh->mNumVertices; ++i) {
        Pos3D pos;
        if (mesh->HasPositions()) {
            pos = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
        }
        Normal normal;
        if (mesh->HasNormals()) {
            normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
        }
        TexCoord tex_coord;
        if (mesh->HasTextureCoords(0)) {
            tex_coord = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
        }
        vertices.emplace_back(Vertex{pos, normal, tex_coord});
    }

    for(u32 i{}; i<mesh->mNumFaces; ++i) {
        auto face {mesh->mFaces[i]};
        for(u32 j{}; j<face.mNumIndices; ++j) {
            indices.emplace_back(face.mIndices[j]);
        }
    }

    //aiMaterial* material {scene->mMaterials[mesh->mMaterialIndex]};

    //load_material_textures(material, aiTextureType_DIFFUSE);
    //load_material_textures(material, aiTextureType_SPECULAR);

    return Mesh(std::move(vertices), std::move(indices));
}


//void Model::load_material_textures(aiMaterial* mat, aiTextureType type)
//{
//    for(u32 i{}; i<mat->GetTextureCount(type); ++i) {
//        aiString str;
//        mat->GetTexture(type, i, &str);
//
//        auto path {directory + "/" + std::string{str.C_Str()}};
//        auto skip {false};
//        for (u32 j{}; j<texture_paths_names.size(); ++j) {
//            if (texture_paths_names[j] == path) {
//                skip = true;
//                break;
//            }
//        }
//        if (!skip) {
//            Asset_Manager::instance()->load_texture(path.c_str(), get_type_name(type));
//            texture_paths_names.emplace_back(path);
//        }
//    }
//}

const std::vector<Mesh>& Model::get_meshes() const
{ return meshes; }

}
