#include "Cake.h"

#include "Cake.h"
#include "Camera.h"

unsigned int texture_from_file(const char *path, const std::string &directory);

Cake::Cake(std::string const &path,
             Camera *camera,
             glm::vec3 translate,
             glm::vec3 scale,
             glm::vec3 rotate,
             float angle) {
    load_model(path);
    this->translate = translate;
    this->scale = scale;
    this->rotate = rotate;
    this->angle = angle;
    this->camera = camera;
    y = 2;
    last_update_time = glfwGetTime();
    shader = ShaderProgram("project/shaders/v_model_shader.txt", "project/shaders/f_model_shader.txt");
}

void Cake::load_model(std::string const &path) {
    Assimp::Importer import;

    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "Assimp import error" << std::endl;
        return;
    }


    directory = path.substr(0, path.find_last_of('/'));

    process_node(scene->mRootNode, scene);
}


void Cake::process_node(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(process_mesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        process_node(node->mChildren[i], scene);
    }
}


Mesh Cake::process_mesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        glm::vec3 vector;

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;

            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.tex_coords = vec;
        } else {
            vertex.tex_coords = glm::vec2(0.0f, 0.0f);
        }


        vertices.push_back(vertex);

    }


    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }


    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Texture> diffuse_maps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

    std::vector<Texture> specularMaps = load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture> normalMaps = load_material_textures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture> heightMaps = load_material_textures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());


    return Mesh(vertices, indices, textures);

}

std::vector<Texture> Cake::load_material_textures(aiMaterial *mat, aiTextureType type, std::string type_name) {
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {

        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++) {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }

        if (!skip) {
            Texture texture;
            texture.id = texture_from_file(str.C_Str(), this->directory);
            texture.type = type_name;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }

    }

    return textures;
}


void Cake::render() {

    glEnable(GL_DEPTH_TEST);
    shader.use();
    projection = glm::perspective(glm::radians(45.0f), (float) 800 / (float) 600, 0.1f, 100.0f);

    view = camera->GetViewMatrix();

    shader.set_mat4_uniform("projection", projection);
    shader.set_mat4_uniform("view", view);

    model = glm::mat4(1.0f);
    model = glm::translate(model, translate);
    model = glm::scale(model, scale);

    shader.set_mat4_uniform("model", model);


    for (auto mesh: meshes) {
        mesh.draw_mesh(shader);
    }
    glDisable(GL_DEPTH_TEST);

}

void Cake::update() {
    double velocity = .1;
    y -= velocity*(glfwGetTime() - last_update_time);
    model = glm::translate(model, glm::vec3(0.0f, y, 0.0f));
}


unsigned int texture_from_file(const char *path, const std::string &directory) {
    std::string filename = std::string(path);

    filename = directory + '/' + filename;

    unsigned int texture_id;
    glGenTextures(1, &texture_id);

    cv::Mat image = cv::imread(filename);

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, image.ptr());
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    cv::flip(image, image, 0);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture_id;
}
