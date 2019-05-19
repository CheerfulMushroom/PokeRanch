#include <Model_anim.h>
#include <string>

unsigned int texture_from_file(const char *path, const std::string &directory);


//------------------------вспомогательные функции---------------------------

glm::mat3 aiMatrix3x3_to_glm(const aiMatrix3x3 &from) {
    glm::mat3 to;

    to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3;
    to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3;
    to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3;

    return to;
}

glm::mat4 aiMatrix4x4_to_glm(const aiMatrix4x4 &from) {
    glm::mat4 to;

    to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
    to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
    to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
    to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;

    return to;
}

//------------------------вспомогательные функции-------------------------------


//--------------------------------функции для работы с матрицами в aiNodeAnim---------------------------

unsigned int Model::find_position(float animation_time, const aiNodeAnim *node_anim) {
    for (unsigned int i = 0; i < node_anim->mNumPositionKeys - 1; i++) {
        if (animation_time < (float) node_anim->mPositionKeys[i + 1].mTime) {
            return i;
        }
    }

    return 0;
}

unsigned int Model::find_rotation(float animation_time, const aiNodeAnim *node_anim) {
    for (unsigned int i = 0; i < node_anim->mNumRotationKeys - 1; i++) {
        if (animation_time < (float) node_anim->mRotationKeys[i + 1].mTime) {
            return i;
        }
    }

    return 0;
}

unsigned int Model::find_scaling(float animation_time, const aiNodeAnim *node_anim) {
    for (unsigned int i = 0; i < node_anim->mNumScalingKeys - 1; i++) {
        if (animation_time < (float) node_anim->mScalingKeys[i + 1].mTime) {
            return i;
        }
    }

    return 0;
}

void Model::calc_interpolated_position(aiVector3D& out, float animation_time, const aiNodeAnim *node_anim) {
	if (node_anim->mNumPositionKeys == 1) {
		out = node_anim->mPositionKeys[0].mValue;
		return;
	}

	unsigned int position_index = find_position(animation_time, node_anim);
	unsigned int next_position_index = position_index + 1;

	//assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);

	float delta_time = (float) (node_anim->mPositionKeys[next_position_index].mTime - node_anim->mPositionKeys[position_index].mTime);
	float factor = (animation_time - (float) node_anim->mPositionKeys[position_index].mTime) / delta_time;

	//assert(Factor >= 0.0f && Factor <= 1.0f);

	const aiVector3D& start = node_anim->mPositionKeys[position_index].mValue;
	const aiVector3D& end = node_anim->mPositionKeys[next_position_index].mValue;

	aiVector3D delta = end - start;
	out = start + factor * delta;
}


void Model::calc_interpolated_rotation(aiQuaternion& out, float animation_time, const aiNodeAnim *node_anim) {
	if (node_anim->mNumRotationKeys == 1) {
		out = node_anim->mRotationKeys[0].mValue;
		return;
	}

	unsigned int rotation_index = find_rotation(animation_time, node_anim);
	unsigned int next_rotation_index = rotation_index + 1;

	//assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);

	float delta_time = (float) (node_anim->mRotationKeys[next_rotation_index].mTime - node_anim->mRotationKeys[rotation_index].mTime);
	float factor = (animation_time - (float) node_anim->mRotationKeys[rotation_index].mTime) / delta_time;

	//assert(Factor >= 0.0f && Factor <= 1.0f);

	const aiQuaternion& start_rotation = node_anim->mRotationKeys[rotation_index].mValue;
	const aiQuaternion& end_rotation = node_anim->mRotationKeys[next_rotation_index].mValue;

    aiQuaternion::Interpolate(out, start_rotation, end_rotation, factor);
	out = out.Normalize();
}

void Model::calc_interpolated_scaling(aiVector3D& out, float animation_time, const aiNodeAnim* node_anim)
{
	if (node_anim->mNumScalingKeys == 1) {
		out = node_anim->mScalingKeys[0].mValue;
		return;
	}

	unsigned int scaling_index = find_scaling(animation_time, node_anim);
    unsigned int next_scaling_index = (scaling_index + 1);

	//assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);

	float delta_time = (float)(node_anim->mScalingKeys[next_scaling_index].mTime - node_anim->mScalingKeys[scaling_index].mTime);
	float factor = (animation_time - (float) node_anim->mScalingKeys[scaling_index].mTime) / delta_time;

	//assert(Factor >= 0.0f && Factor <= 1.0f);

	const aiVector3D& start = node_anim->mScalingKeys[scaling_index].mValue;
	const aiVector3D& end = node_anim->mScalingKeys[next_scaling_index].mValue;
	aiVector3D delta = end - start;
	out = start + factor * delta;
}

//--------------------------------функции для работы с матрицами в aiNodeAnim---------------------------


//
void Model::load_model(std::string const &path) {
    Assimp::Importer import;

    scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "Assimp import error" << std::endl;
        return;
    }

    aiMatrix4x4 matrix_for_nodes = scene->mRootNode->mTransformation;
    global_inverse_transform = aiMatrix4x4_to_glm(matrix_for_nodes);
    global_inverse_transform = glm::inverse(global_inverse_transform);

    //InitFromScene ???
    
    directory = path.substr(0, path.find_last_of('/'));

    unsigned int mesh_index = 0;
    unsigned int base_vertex_index = 0;

    process_node(scene->mRootNode, scene);
}


void Model::process_node(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(process_mesh(mesh_index, mesh, scene));
        
    }

    

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        process_node(mesh_index, node->mChildren[i], scene);
    }
}


Mesh Model::process_mesh(unsigned int mesh_index, unsigned int base_vertex_index, aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    //std::vector<VertexBoneData>



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


    //load_bones

    //return Mesh(vertices, indices, textures);
    return Mesh(mesh_index, base_vertex_index, vertices, indices, textures);

}


void Model::load_bones(unsigned int mesh_index, const aiMesh *mesh, std::vector<VertexBoneData> &bones) {
    for (unsigned int i = 0; i < mesh->nNumBones; i++) {
        unsigned int bone_index = 0;
        std::string bone_name(mesh->mBones[i]->mName.data);

        if (bone_map.find(bone_name) == bone_map.end()) {
            bone_index = num_bones;
            num_bones++;
            Bone single_bone;
            bones_info.push_back(single_bone);

            aiMatrix4x4 offset = mesh->mBones[i]->mOffsetMatrix;
            bones_info[bone_index].bone_offset = aiMatrix4x4_to_glm(offset);
            bone_map[bone_name] = bone_index;
        } else {
            bone_index = bone_map[bone_name];
        }

    
        for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
            float weight = mesh->mBones[i]->mWeights[j].mWeight;
            unsigned int vertex_id = 



std::vector<Texture> Model::load_material_textures(aiMaterial *mat, aiTextureType type, std::string type_name) {
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


void Model::render(ShaderProgram shader) {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].draw_mesh(shader);
    }
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
