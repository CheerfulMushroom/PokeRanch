#ifndef OGLDEV_SKINNED_MESH_H
#define	OGLDEV_SKINNED_MESH_H

#include <map>
#include <vector>

#include <GL/glew.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags

#include <ogldev_util.h>
#include <ogldev_math_3d.h>
#include <ShaderProgram.h> 

using namespace std;


struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};


class SkinnedMesh
{
public:
    SkinnedMesh(const std::string &path);

    ~SkinnedMesh();

    bool LoadMesh(const string& Filename);

    void Render();

    void update(float running_time, glm::mat4 projection, glm::mat4 view, glm::mat4 model);

    uint NumBones() const
    {
        return m_NumBones;
    }
    
    void BoneTransform(float TimeInSeconds);
    
private:
    #define NUM_BONES_PER_VEREX 4


    ShaderProgram shader;

    std::string directory;
    std::vector<Texture> textures_loaded;

    struct BoneInfo
    {
        Matrix4f BoneOffset;
        Matrix4f FinalTransformation;        

        BoneInfo()
        {
            BoneOffset.SetZero();
            FinalTransformation.SetZero();            
        }
    };
    
    struct VertexBoneData
    {        
        uint IDs[NUM_BONES_PER_VEREX];
        float Weights[NUM_BONES_PER_VEREX];

        VertexBoneData()
        {
            Reset();
        };
        
        void Reset()
        {
            ZERO_MEM(IDs);
            ZERO_MEM(Weights);        
        }
        
        void AddBoneData(uint BoneID, float Weight);
    };

    void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);    
    uint FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
    uint FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
    uint FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
    const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string NodeName);
    void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix4f& ParentTransform);
    bool InitFromScene(const aiScene* pScene, const string& Filename);
    void InitMesh(uint MeshIndex,
                  const aiScene *pScene,
                  const aiMesh* paiMesh,
                  vector<Vector3f>& Positions,
                  vector<Vector3f>& Normals,
                  vector<Vector2f>& TexCoords,
                  vector<VertexBoneData>& Bones,
                  vector<unsigned int>& Indices);
    void LoadBones(uint MeshIndex, const aiMesh* paiMesh, vector<VertexBoneData>& Bones);

    std::vector<Texture> load_material_textures(aiMaterial *mat, aiTextureType type, std::string type_name);

    void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF
  
enum VB_TYPES {
    INDEX_BUFFER,
    POS_VB,
    NORMAL_VB,
    TEXCOORD_VB,
    BONE_VB,
    NUM_VBs            
};

    GLuint m_VAO;
    GLuint m_Buffers[NUM_VBs];

    struct MeshEntry {
        MeshEntry()
        {
            NumIndices    = 0;
            BaseVertex    = 0;
            BaseIndex     = 0;
            MaterialIndex = INVALID_MATERIAL;
        }
        
        unsigned int NumIndices;
        unsigned int BaseVertex;
        unsigned int BaseIndex;
        unsigned int MaterialIndex;
    };
    
    vector<MeshEntry> m_Entries;
     
    map<string,uint> m_BoneMapping; // maps a bone name to its index
    uint m_NumBones;
    vector<BoneInfo> m_BoneInfo;
    Matrix4f m_GlobalInverseTransform;

    vector<Matrix4f> transforms;

    const aiScene* m_pScene;
    Assimp::Importer m_Importer;
};


#endif	/* OGLDEV_SKINNED_MESH_H */
