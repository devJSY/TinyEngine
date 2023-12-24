#pragma once

#include "CEntity.h"

class CModelLoader : public CEntity
{
public:
    void Load(std::string basePath, std::string filename, bool revertNormals);

    void ProcessNode(aiNode* node, const aiScene* scene, DirectX::SimpleMath::Matrix tr);

    tMeshData ProcessMesh(aiMesh* mesh, const aiScene* scene);

    std::string ReadFilename(aiMaterial* material, aiTextureType type);

    void UpdateTangents();

public:
    std::string basePath;
    std::vector<tMeshData> meshes;
    bool m_isGLTF = false; // gltf or fbx
    bool m_revertNormals = false;
};
