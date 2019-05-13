#include <Mesh.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <opencv2/opencv.hpp>

#include "Interfaces.h"

class Model :public Renderable{
 public:
    explicit Model(std::string const &path);

    void render() override;

 private:
    std::vector<Mesh> meshes;
    std::vector<Texture> textures_loaded;
    std::string directory;
    ShaderProgram shader;

    void load_model(std::string const &path);

    void process_node(aiNode *node, const aiScene *scene);
    Mesh process_mesh(aiMesh *mesh, const aiScene *scene);

    std::vector<Texture> load_material_textures(aiMaterial *mat, aiTextureType type, std::string type_name);
};
