#include <Mesh.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <opencv2/opencv.hpp>

class Model {
 public:
    Model(std::string const &path) {
        load_model(path);
    }

    void render(ShaderProgram shader);

 private:
    std::vector<Mesh> meshes;
    std::vector<Mesh_Texture> textures_loaded;
    std::string directory;

    void load_model(std::string const &path);

    void process_node(aiNode *node, const aiScene *scene);
    Mesh process_mesh(aiMesh *mesh, const aiScene *scene);

    std::vector<Mesh_Texture> load_material_textures(aiMaterial *mat, aiTextureType type, std::string type_name);
};
