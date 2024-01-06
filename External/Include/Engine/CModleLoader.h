#pragma once

#include "CEntity.h"

class CModelLoader : public CEntity
{
private:
    // 절대경로/상대경로/파일이름
    std::string filePath; // 상대경로
    std::string basePath; // 절대경로 + 상대경로
    std::vector<tMeshData> meshes;
    bool m_isGLTF = false; // gltf or fbx
    bool m_revertNormals = false;

public:
    void Load(std::string filePath, std::string fileName, bool revertNormals);

    void ProcessNode(aiNode* node, const aiScene* scene, DirectX::SimpleMath::Matrix tr);

    tMeshData ProcessMesh(aiMesh* mesh, const aiScene* scene);

    std::string ReadFilename(aiMaterial* material, aiTextureType type);

    void UpdateTangents();

    friend class CAssetMgr;
};
