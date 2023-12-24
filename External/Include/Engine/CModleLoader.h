#pragma once

#include "CEntity.h"

class CModelLoader : public CEntity
{
public:
    void Load(std::string filePath, std::string fileName, bool revertNormals);

    void ProcessNode(aiNode* node, const aiScene* scene, DirectX::SimpleMath::Matrix tr);

    tMeshData ProcessMesh(aiMesh* mesh, const aiScene* scene);

    std::string ReadFilename(aiMaterial* material, aiTextureType type);

    void UpdateTangents();

public:
    std::string filePath;   // Load 에서 들어온 filePath 
    std::string basePath;   // fileName을 제외한 전체 파일 경로
    std::vector<tMeshData> meshes;
    bool m_isGLTF = false; // gltf or fbx
    bool m_revertNormals = false;
};
