#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <string>
#include <cstddef>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <ShaderProgram.h>

#define NUM_BONES_PER_VERTEX 4

struct Bone {
    glm::mat4 bone_offset;
    glm::mat4 final_transformation;
};

struct VertexBoneData {
    unsigned int bone_IDs[NUM_BONES_PER_VERTEX];
    float weights[NUM_BONES_PER_VERTEX];


    void VerteBoneData() {
        Reset();
    }

    void Reset() {
        for (unsigned int i = 0; i < NUM_BONES_PER_VERTEX; i++) {
            bone_IDs[i] = 0;
            weights[i] = 0;
        }
    }

    void add_bone_data(unsigned int bone_id, float weight);

};


struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
    
};

class Mesh {
 public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    std::vector<VertexBoneData> bones;

    Mesh(unsigned int mesh_index, unsigned int base_vertex_int, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, std::vector<VertexBoneData> bones);
    //Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, std::vector<VertexBoneData> bones);

    void draw_mesh(ShaderProgram shader);

 private:
    unsigned int VAO, VBO, EBO, BONE_VB;

    unsigned int mesh_id;
    unsigned int first_vertex_id;

    void setup_mesh();
};
