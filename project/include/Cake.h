#ifndef PREP_POKEMON_RANCH_CAKE_H
#define PREP_POKEMON_RANCH_CAKE_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <opencv2/opencv.hpp>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Interfaces.h"

class Camera;


class Cake : public Renderable, public Updatable {
public:
    Cake(std::string const &path,
         Camera *camera,
         glm::vec3 translate,
         glm::vec3 scale,
         glm::vec3 rotate,
         float angle);

    ~Cake() override = default;

    void render() override;
    void update() override;

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
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;
    double y;
    double last_update_time;

    void load_model(std::string const &path);

    void process_node(aiNode *node, const aiScene *scene);

    Mesh process_mesh(aiMesh *mesh, const aiScene *scene);

    std::vector<Texture> load_material_textures(aiMaterial *mat, aiTextureType type, std::string type_name);
};


#endif //PREP_POKEMON_RANCH_CAKE_H
