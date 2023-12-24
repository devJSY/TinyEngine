#include "pch.h"
#include "CModleLoader.h"
#include "CPathMgr.h"

void UpdateNormals(vector<tMeshData>& meshes)
{
    // 노멀 벡터가 없는 경우를 대비하여 다시 계산
    // 한 위치에는 한 버텍스만 있어야 연결 관계를 찾을 수 있음

    // DirectXMesh의 ComputeNormals()과 비슷합니다.
    // https://github.com/microsoft/DirectXMesh/wiki/ComputeNormals

    for (auto& m : meshes)
    {
        vector<Vector3> normalsTemp(m.vertices.size(), Vector3(0.0f));
        vector<float> weightsTemp(m.vertices.size(), 0.0f);

        for (int i = 0; i < m.indices.size(); i += 3)
        {

            int idx0 = m.indices[i];
            int idx1 = m.indices[i + 1];
            int idx2 = m.indices[i + 2];

            auto v0 = m.vertices[idx0];
            auto v1 = m.vertices[idx1];
            auto v2 = m.vertices[idx2];

            auto faceNormal = (v1.vPos - v0.vPos).Cross(v2.vPos - v0.vPos);

            normalsTemp[idx0] += faceNormal;
            normalsTemp[idx1] += faceNormal;
            normalsTemp[idx2] += faceNormal;
            weightsTemp[idx0] += 1.0f;
            weightsTemp[idx1] += 1.0f;
            weightsTemp[idx2] += 1.0f;
        }

        for (int i = 0; i < m.vertices.size(); i++)
        {
            if (weightsTemp[i] > 0.0f)
            {
                m.vertices[i].vNormal = normalsTemp[i] / weightsTemp[i];
                m.vertices[i].vNormal.Normalize();
            }
        }
    }
}

string GetExtension(const string filename)
{
    string ext(std::filesystem::path(filename).extension().string());
    transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext;
}

void CModelLoader::Load(std::string filePath, std::string fileName, bool revertNormals)
{
    if (GetExtension(fileName) == ".gltf")
    {
        m_isGLTF = true;
        m_revertNormals = revertNormals;
    }

    // wstring To string
    wstring strFilePath = CPathMgr::GetContentPath();
    std::string ContentPath(strFilePath.length(), 0);
    std::transform(strFilePath.begin(), strFilePath.end(), ContentPath.begin(), [](wchar_t c) { return (char)c; });

    this->filePath = filePath;
    this->basePath = ContentPath + filePath;

    Assimp::Importer importer;

    const aiScene* pScene =
        importer.ReadFile(this->basePath + fileName, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

    if (!pScene)
    {
        std::cout << "Failed to read file: " << this->basePath + fileName << std::endl;
    }
    else
    {
        Matrix tr; // Initial transformation
        ProcessNode(pScene->mRootNode, pScene, tr);
    }

    // UpdateNormals(this->meshes); // Vtx Normal을 직접 계산 (참고용)

    UpdateTangents();
}

void CModelLoader::UpdateTangents()
{
    using namespace std;
    using namespace DirectX;

    // https://github.com/microsoft/DirectXMesh/wiki/ComputeTangentFrame

    for (auto& m : this->meshes)
    {
        vector<XMFLOAT3> positions(m.vertices.size());
        vector<XMFLOAT3> normals(m.vertices.size());
        vector<XMFLOAT2> texcoords(m.vertices.size());
        vector<XMFLOAT3> tangents(m.vertices.size());
        vector<XMFLOAT3> bitangents(m.vertices.size());

        for (size_t i = 0; i < m.vertices.size(); i++)
        {
            auto& v = m.vertices[i];
            positions[i] = v.vPos;
            normals[i] = v.vNormal;
            texcoords[i] = v.vUV;
        }

        // DirectXMesh
        // ComputeTangentFrame(m.indices.data(), m.indices.size() / 3, positions.data(), normals.data(),
        // texcoords.data(),
        //                    m.vertices.size(), tangents.data(), bitangents.data());

        // for (size_t i = 0; i < m.vertices.size(); i++)
        //{
        //     m.vertices[i].tangentModel = tangents[i];
        // }
    }
}

void CModelLoader::ProcessNode(aiNode* node, const aiScene* scene, Matrix tr)
{
    // std::cout << node->mName.C_Str() << " : " << node->mNumMeshes << " "
    //           << node->mNumChildren << std::endl;

    Matrix m;
    ai_real* temp = &node->mTransformation.a1;
    float* mTemp = &m._11;
    for (int t = 0; t < 16; t++)
    {
        mTemp[t] = float(temp[t]);
    }
    m = m.Transpose() * tr;

    for (UINT i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        auto newMesh = this->ProcessMesh(mesh, scene);

        for (auto& v : newMesh.vertices)
        {
            v.vPos = DirectX::SimpleMath::Vector3::Transform(v.vPos, m);
        }

        meshes.push_back(newMesh);
    }

    for (UINT i = 0; i < node->mNumChildren; i++)
    {
        this->ProcessNode(node->mChildren[i], scene, m);
    }
}

string CModelLoader::ReadFilename(aiMaterial* material, aiTextureType type)
{
    if (material->GetTextureCount(type) > 0)
    {
        aiString filepath;
        material->GetTexture(type, 0, &filepath);

        std::string fileName = std::string(std::filesystem::path(filepath.C_Str()).filename().string());

        return fileName;
    }
    else
    {
        return "";
    }
}

tMeshData CModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    // Data to fill
    std::vector<Vtx> vertices;
    std::vector<UINT> indices;

    // Walk through each of the mesh's vertices
    for (UINT i = 0; i < mesh->mNumVertices; i++)
    {
        Vtx vertex;

        vertex.vPos.x = mesh->mVertices[i].x;
        vertex.vPos.y = mesh->mVertices[i].y;
        vertex.vPos.z = mesh->mVertices[i].z;

        vertex.vNormal.x = mesh->mNormals[i].x;
        if (m_isGLTF)
        {
            vertex.vNormal.y = mesh->mNormals[i].z;
            vertex.vNormal.z = -mesh->mNormals[i].y;
        }
        else
        {
            vertex.vNormal.y = mesh->mNormals[i].y;
            vertex.vNormal.z = mesh->mNormals[i].z;
        }

        if (m_revertNormals)
        {
            vertex.vNormal *= -1.0f;
        }

        vertex.vNormal.Normalize();

        if (mesh->mTextureCoords[0])
        {
            vertex.vUV.x = (float)mesh->mTextureCoords[0][i].x;
            vertex.vUV.y = (float)mesh->mTextureCoords[0][i].y;
        }

        vertices.push_back(vertex);
    }

    for (UINT i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (UINT j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    tMeshData newMesh;
    newMesh.vertices = vertices;
    newMesh.indices = indices;

    newMesh.RelativeTextureFilePath = filePath;

    // http://assimp.sourceforge.net/lib_html/materials.html
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        newMesh.AlbedoTextureFilename = ReadFilename(material, aiTextureType_BASE_COLOR);
        if (newMesh.AlbedoTextureFilename.empty())
        {
            newMesh.AlbedoTextureFilename = ReadFilename(material, aiTextureType_DIFFUSE);
        }

        newMesh.EmissiveTextureFilename = ReadFilename(material, aiTextureType_EMISSIVE);
        newMesh.HeightTextureFilename = ReadFilename(material, aiTextureType_HEIGHT);
        newMesh.NormalTextureFilename = ReadFilename(material, aiTextureType_NORMALS);
        newMesh.MetallicTextureFilename = ReadFilename(material, aiTextureType_METALNESS);
        newMesh.RoughnessTextureFilename = ReadFilename(material, aiTextureType_DIFFUSE_ROUGHNESS);

        newMesh.AoTextureFilename = ReadFilename(material, aiTextureType_AMBIENT_OCCLUSION);
        if (newMesh.AoTextureFilename.empty())
        {
            newMesh.AoTextureFilename = ReadFilename(material, aiTextureType_LIGHTMAP);
        }

        // 디버깅용
        // for (size_t i = 0; i < 22; i++) {
        //    cout << i << " " << ReadFilename(material, aiTextureType(i))
        //         << endl;
        //}
    }

    return newMesh;
}
