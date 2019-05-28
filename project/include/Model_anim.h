#include <Mesh_anim.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <opencv2/opencv.hpp>
#include <map>

glm::mat3 aiMatrix3x3_to_glm(const aiMatrix3x3 &from);
glm::mat4 aiMatrix4x4_to_glm(const aiMatrix4x4 &from);

class Model {
 public:
    Model(std::string const &path) {
        num_bones = 0;
        num_meshes = 0;
        num_vertices = 0;

        load_model(path);
    }

    void render(ShaderProgram shader);

    void bone_transform(float time_in_seconds, std::vector<glm::mat4> &transforms);

 private:
    std::vector<Mesh> meshes;
    std::vector<Texture> textures_loaded;
    std::string directory;

    const aiScene *scene;  // Нужно будет часто обращаться

    void load_model(std::string const &path);

    void process_node(aiNode *node, const aiScene *scene);
    Mesh process_mesh(unsigned int mesh_index, unsigned int base_vertex_index, aiMesh *mesh, const aiScene *scene);

    void read_node_heirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform);


    void load_bones(unsigned int mesh_index, const aiMesh *mesh, std::vector<VertexBoneData> &bones);

    const aiNodeAnim* find_node_anim(const aiAnimation *animation, const std::string node_name);

    //void bone_transform(float time_in_seconds, std::vector<glm::mat4> &transforms);

    std::vector<Texture> load_material_textures(aiMaterial *mat, aiTextureType type, std::string type_name);
    
    // для анимации
    glm::mat4 global_inverse_transform;
    std::map<std::string, unsigned int> bone_map;

    unsigned int num_bones;
    unsigned int num_meshes;
    unsigned int num_vertices;

    std::vector<Bone> bones_info;

    double anim_duration;

    void calc_interpolated_scaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void calc_interpolated_rotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void calc_interpolated_position(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int find_scaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int find_rotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int find_position(float AnimationTime, const aiNodeAnim* pNodeAnim);
};
