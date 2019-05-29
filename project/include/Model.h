#ifndef PREP_POKEMON_RANCH_MODEL_H
#define PREP_POKEMON_RANCH_MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <opencv2/opencv.hpp>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Interfaces.h"

class Camera;

class Model : public Renderable{
public:
    Model(std::string const &path,
          Camera *camera,
          glm::vec3 translate,
          glm::vec3 scale,
          glm::vec3 rotate,
          float angle,
          std::function<void()> to_exec = nullptr);
    ~Model() override = default;

    void render() override;

private:
    std::vector<Mesh> meshes;
    std::vector<Texture> textures_loaded;
    std::string directory;
    ShaderProgram shader;
    Camera *camera;
    glm::vec3 translate;
    glm::vec3 scale;
    glm::vec3 rotate;
    float angle;
    std::function<void()> to_exec;

    void load_model(std::string const &path);
    void process_node(aiNode *node, const aiScene *scene);
    Mesh process_mesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> load_material_textures(aiMaterial *mat, aiTextureType type, std::string type_name);
};

#endif // PREP_POKEMON_RANCH_MODEL_H