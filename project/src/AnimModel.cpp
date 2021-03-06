#include <opencv2/opencv.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

#include "MarkerDetector.h"
#include "AnimModel.h"
#include "Utils.h"


#define POSITION_LOCATION    0
#define TEX_COORD_LOCATION   2
#define NORMAL_LOCATION      1
#define BONE_ID_LOCATION     3
#define BONE_WEIGHT_LOCATION 4


static unsigned int texture_from_file(const char *path, const std::string &directory);


AnimModel::AnimModel(int id,
                     Camera *camera,
                     glm::vec3 translate,
                     glm::vec3 rotate,
                     float angle,
                     int width,
                     int height) {
    this->id = id;
    marker_detector = nullptr;
    this->state = nullptr;

    if (!get_pokemon_info(id, &scale, &anim_id)) {
        std::cout << "NEW_MARKER_FOUND" << std::endl;
    }

    model = glm::mat4(1.0f);
    model = glm::translate(model, translate);
    model = glm::scale(model, scale);
    model = glm::rotate(model, glm::radians(angle), rotate);
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    projection = glm::perspective(glm::radians(45.0f), (float) width / (float) height, 0.1f, 100.0f);
    view = camera->GetViewMatrix();


    m_VAO = 0;
    ZERO_MEM(m_Buffers);
    m_NumBones = 0;
    m_pScene = nullptr;

    shader = ShaderProgram("project/shaders/v_model_anim_pokedex_shader.txt",
                           "project/shaders/f_model_anim_shader.txt");

    bool has_path = get_dir_by_id(id, directory);
    if (!has_path) {
        directory = "project/models/Pikachu/";
    }

    anim_names = get_anims(directory);

    load_mesh(directory + anim_names[anim_id]);

}

AnimModel::AnimModel(int id,
                     GameState *state,
                     MarkerDetector *marker_detector,
                     std::function<void()> to_exec) {

    this->id = id;
    this->state = state;
    this->marker_detector = marker_detector;
    this->to_exec = std::move(to_exec);

    if (!get_pokemon_info(id, &scale, &anim_id)) {
        std::cout << "NEW_MARKER_FOUND" << std::endl;
    }


    projection = marker_detector->projection;
    view = glm::mat4(1.0f);
    model = glm::mat4(0.0f);

    m_VAO = 0;
    ZERO_MEM(m_Buffers);
    m_NumBones = 0;
    m_pScene = nullptr;

    shader = ShaderProgram("project/shaders/v_model_anim_shader.txt",
                           "project/shaders/f_model_anim_shader.txt");

    bool has_path = get_dir_by_id(id, directory);
    if (!has_path) {
        directory = "project/models/Pikachu/";
    }

    anim_names = get_anims(directory);


    load_mesh(directory + anim_names[anim_id]);
}


AnimModel::~AnimModel() {
    write_pokemon_info(id, scale, anim_id);
    Clear();
}


void AnimModel::Clear() {

    if (m_Buffers[0] != 0) {
        glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);
    }

    if (m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }
}


bool AnimModel::load_mesh(const string &Filename) {
    Clear();

    // Create the VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Create the buffers for the vertices attributes
    glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

    bool Ret = false;

    m_pScene = m_Importer.ReadFile(Filename.c_str(), ASSIMP_LOAD_FLAGS);

    if (m_pScene) {
        m_GlobalInverseTransform = m_pScene->mRootNode->mTransformation;
        m_GlobalInverseTransform.Inverse();
        Ret = InitFromScene(m_pScene, Filename);
    } else {
        printf("Error parsing '%s': '%s'\n", Filename.c_str(), m_Importer.GetErrorString());
    }

    // Make sure the VAO is not changed from the outside
    glBindVertexArray(0);

    return Ret;
}

void AnimModel::render() {
    glEnable(GL_DEPTH_TEST);

    shader.use();
    glBindVertexArray(m_VAO);

    for (uint i = 0; i < m_Entries.size(); i++) {
        const uint MaterialIndex = m_Entries[i].MaterialIndex;

        glUniform1i(glGetUniformLocation(shader.program, "texture_diffuse1"), 0);
        glBindTexture(GL_TEXTURE_2D, textures_loaded[i].id);


        glDrawElementsBaseVertex(GL_TRIANGLES,
                                 m_Entries[i].NumIndices,
                                 GL_UNSIGNED_INT,
                                 (void *) (sizeof(uint) * m_Entries[i].BaseIndex),
                                 m_Entries[i].BaseVertex);


        glBindTexture(GL_TEXTURE_2D, 0);
    }

    glBindVertexArray(0);

    glDisable(GL_DEPTH_TEST);
}


void AnimModel::update() {
    shader.use();

    BoneTransform((float) glfwGetTime());

    for (unsigned int i = 0; i < transforms.size(); ++i) {
        const std::string name = "gBones[" + std::to_string(i) + "]";
        auto boneTransform = (GLuint) glGetUniformLocation(shader.program, name.c_str());
        glUniformMatrix4fv(boneTransform, 1, GL_TRUE, (const GLfloat *) transforms[i]);
    }

    if (marker_detector != nullptr) {
        if (marker_detector->get_marker(id, &marker)) {
            is_deleted = false;
            //////

            cv::Mat rodrig;

            auto good_rvec = marker.Rvec;
            good_rvec.at<float>(0, 2) *= -1.0;


            Rodrigues(good_rvec, rodrig);


            GLfloat RTMat[16] = {rodrig.at<float>(0, 0), rodrig.at<float>(0, 1), rodrig.at<float>(0, 2), 0,
                                 rodrig.at<float>(1, 0), rodrig.at<float>(1, 1), rodrig.at<float>(1, 2), 0,
                                 rodrig.at<float>(2, 0), rodrig.at<float>(2, 1), rodrig.at<float>(2, 2), 0,
                                 10 * marker.Tvec.at<float>(0), 10 * marker.Tvec.at<float>(1),
                                 10 * -marker.Tvec.at<float>(2), 1};

            model = glm::make_mat4(RTMat);

            //////

            model = glm::scale(model, scale);

            last_update_time = glfwGetTime();

        } else {
            if (glfwGetTime() - last_update_time > 0.2) {
                is_deleted = true;
                model = glm::mat4(-1000.0f);
            }
        }
    }

    shader.set_mat4_uniform("projection", projection);
    shader.set_mat4_uniform("view", view);
    shader.set_mat4_uniform("model", model);
}


bool AnimModel::is_pointed_at() {
    if (is_deleted) {
        return false;
    }

    GLFWwindow *window = state->get_game()->get_window();

    /////////// Hitbox
    double hit_height = 1 / get_distance() * 4.5;
    double hit_width = hit_height / 2.25;

    auto center = marker.getCenter();
    // Перевод в нормированные координаты
    double marker_x = 2 * ((center.x) / 640 - .5);
    double marker_y = -2 * ((center.y) / 480 - .5);

    /////////// Cursor
    double cursor_x, cursor_y;
    glfwGetCursorPos(window, &cursor_x, &cursor_y);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    // Перевод из координат GLFW ([0;-1]) в координаты openGL ([-1;1])
    cursor_x = 2 * ((cursor_x) / width - .5);
    cursor_y = -2 * ((cursor_y) / height - .5);

    double left = marker_x - hit_width / 2;
    double right = marker_x + hit_width / 2;
    double up = marker_y + hit_height;
    double down = marker_y - hit_height * 0.1;

    return (left < cursor_x) && (cursor_x < right) && (down < cursor_y) && (cursor_y < up);
}

double AnimModel::get_distance() {
    auto tvec = model[3];
    auto distance = glm::length(tvec);
    return distance;
}

void AnimModel::exec() {
    if (to_exec != nullptr) {
        to_exec();
    }
}


bool AnimModel::InitFromScene(const aiScene *pScene, const string &Filename) {
    m_Entries.resize(pScene->mNumMeshes);

    vector<Vector3f> Positions;
    vector<Vector3f> Normals;
    vector<Vector2f> TexCoords;
    vector<VertexBoneData> Bones;
    vector<uint> Indices;

    uint NumVertices = 0;
    uint NumIndices = 0;

    // Count the number of vertices and indices
    for (uint i = 0; i < m_Entries.size(); i++) {
        m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
        m_Entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
        m_Entries[i].BaseVertex = NumVertices;
        m_Entries[i].BaseIndex = NumIndices;

        NumVertices += pScene->mMeshes[i]->mNumVertices;
        NumIndices += m_Entries[i].NumIndices;
    }

    // Reserve space in the vectors for the vertex attributes and indices
    Positions.reserve(NumVertices);
    Normals.reserve(NumVertices);
    TexCoords.reserve(NumVertices);
    Bones.resize(NumVertices);
    Indices.reserve(NumIndices);

    // Initialize the meshes in the scene one by one
    for (uint i = 0; i < m_Entries.size(); i++) {
        const aiMesh *paiMesh = pScene->mMeshes[i];
        InitMesh(i, pScene, paiMesh, Positions, Normals, TexCoords, Bones, Indices);
    }

    // Generate and populate the buffers with vertex attributes and the indices
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(TEX_COORD_LOCATION);
    glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(NORMAL_LOCATION);
    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[BONE_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Bones[0]) * Bones.size(), &Bones[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(BONE_ID_LOCATION);
    glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid *) 0);
    glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
    glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid *) 16);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

    return GLCheckError();
}


void AnimModel::InitMesh(uint MeshIndex,
                         const aiScene *pScene,
                         const aiMesh *paiMesh,
                         vector<Vector3f> &Positions,
                         vector<Vector3f> &Normals,
                         vector<Vector2f> &TexCoords,
                         vector<VertexBoneData> &Bones,
                         vector<uint> &Indices) {
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    // Populate the vertex attribute vectors
    for (uint i = 0; i < paiMesh->mNumVertices; i++) {
        const aiVector3D *pPos = &(paiMesh->mVertices[i]);
        const aiVector3D *pNormal = &(paiMesh->mNormals[i]);
        const aiVector3D *pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

        Positions.push_back(Vector3f(pPos->x, pPos->y, pPos->z));
        Normals.push_back(Vector3f(pNormal->x, pNormal->y, pNormal->z));
        TexCoords.push_back(Vector2f(pTexCoord->x, pTexCoord->y));
    }

    LoadBones(MeshIndex, paiMesh, Bones);

    // Populate the index buffer
    for (uint i = 0; i < paiMesh->mNumFaces; i++) {
        const aiFace &Face = paiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }

    aiMaterial *material = pScene->mMaterials[paiMesh->mMaterialIndex];
    load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
}


void AnimModel::LoadBones(uint MeshIndex, const aiMesh *pMesh, vector<VertexBoneData> &Bones) {
    for (uint i = 0; i < pMesh->mNumBones; i++) {
        uint BoneIndex = 0;
        string BoneName(pMesh->mBones[i]->mName.data);

        if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
            // Allocate an index for a new bone
            BoneIndex = m_NumBones;
            m_NumBones++;
            BoneInfo bi;
            m_BoneInfo.push_back(bi);
            m_BoneInfo[BoneIndex].BoneOffset = pMesh->mBones[i]->mOffsetMatrix;
            m_BoneMapping[BoneName] = BoneIndex;
        } else {
            BoneIndex = m_BoneMapping[BoneName];
        }

        for (uint j = 0; j < pMesh->mBones[i]->mNumWeights; j++) {
            uint VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
            float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
            Bones[VertexID].AddBoneData(BoneIndex, Weight);
        }
    }
}


uint AnimModel::FindPosition(float AnimationTime, const aiNodeAnim *pNodeAnim) {
    for (uint i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
        if (AnimationTime < (float) pNodeAnim->mPositionKeys[i + 1].mTime) {
            return i;
        }
    }

    assert(0);

    return 0;
}


uint AnimModel::FindRotation(float AnimationTime, const aiNodeAnim *pNodeAnim) {
    assert(pNodeAnim->mNumRotationKeys > 0);

    for (uint i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
        if (AnimationTime < (float) pNodeAnim->mRotationKeys[i + 1].mTime) {
            return i;
        }
    }

    assert(0);

    return 0;
}


uint AnimModel::FindScaling(float AnimationTime, const aiNodeAnim *pNodeAnim) {
    assert(pNodeAnim->mNumScalingKeys > 0);

    for (uint i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
        if (AnimationTime < (float) pNodeAnim->mScalingKeys[i + 1].mTime) {
            return i;
        }
    }

    assert(0);

    return 0;
}


void AnimModel::CalcInterpolatedPosition(aiVector3D &Out, float AnimationTime, const aiNodeAnim *pNodeAnim) {
    if (pNodeAnim->mNumPositionKeys == 1) {
        Out = pNodeAnim->mPositionKeys[0].mValue;
        return;
    }

    uint PositionIndex = FindPosition(AnimationTime, pNodeAnim);
    uint NextPositionIndex = (PositionIndex + 1);
    assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
    float DeltaTime = (float) (pNodeAnim->mPositionKeys[NextPositionIndex].mTime -
                               pNodeAnim->mPositionKeys[PositionIndex].mTime);
    float Factor = (AnimationTime - (float) pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D &Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
    const aiVector3D &End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}


void AnimModel::CalcInterpolatedRotation(aiQuaternion &Out, float AnimationTime, const aiNodeAnim *pNodeAnim) {
    // we need at least two values to interpolate...
    if (pNodeAnim->mNumRotationKeys == 1) {
        Out = pNodeAnim->mRotationKeys[0].mValue;
        return;
    }

    uint RotationIndex = FindRotation(AnimationTime, pNodeAnim);
    uint NextRotationIndex = (RotationIndex + 1);
    assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
    float DeltaTime = (float) (pNodeAnim->mRotationKeys[NextRotationIndex].mTime -
                               pNodeAnim->mRotationKeys[RotationIndex].mTime);
    float Factor = (AnimationTime - (float) pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiQuaternion &StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
    const aiQuaternion &EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
    aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
    Out = Out.Normalize();
}


void AnimModel::CalcInterpolatedScaling(aiVector3D &Out, float AnimationTime, const aiNodeAnim *pNodeAnim) {
    if (pNodeAnim->mNumScalingKeys == 1) {
        Out = pNodeAnim->mScalingKeys[0].mValue;
        return;
    }

    uint ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
    uint NextScalingIndex = (ScalingIndex + 1);
    assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
    float DeltaTime = (float) (pNodeAnim->mScalingKeys[NextScalingIndex].mTime -
                               pNodeAnim->mScalingKeys[ScalingIndex].mTime);
    float Factor = (AnimationTime - (float) pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D &Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
    const aiVector3D &End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}


void AnimModel::ReadNodeHeirarchy(float AnimationTime, const aiNode *pNode, const Matrix4f &ParentTransform) {
    string NodeName(pNode->mName.data);

    const aiAnimation *pAnimation = m_pScene->mAnimations[0];

    Matrix4f NodeTransformation(pNode->mTransformation);

    const aiNodeAnim *pNodeAnim = FindNodeAnim(pAnimation, NodeName);

    if (pNodeAnim) {
        aiVector3D Scaling;
        CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
        Matrix4f ScalingM;
        ScalingM.InitScaleTransform(Scaling.x, Scaling.y, Scaling.z);

        // Interpolate rotation and generate rotation transformation matrix
        aiQuaternion RotationQ;
        CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
        Matrix4f RotationM = Matrix4f(RotationQ.GetMatrix());

        // Interpolate translation and generate translation transformation matrix
        aiVector3D Translation;
        CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
        Matrix4f TranslationM;
        TranslationM.InitTranslationTransform(Translation.x, Translation.y, Translation.z);

        // Combine the above transformations
        NodeTransformation = TranslationM * RotationM * ScalingM;
    }

    Matrix4f GlobalTransformation = ParentTransform * NodeTransformation;

    if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) {
        uint BoneIndex = m_BoneMapping[NodeName];
        m_BoneInfo[BoneIndex].FinalTransformation =
                m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
    }

    for (uint i = 0; i < pNode->mNumChildren; i++) {
        ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
    }
}


void AnimModel::BoneTransform(float TimeInSeconds) {
    Matrix4f Identity;
    Identity.InitIdentity();

    auto TicksPerSecond = (float) (m_pScene->mAnimations[0]->mTicksPerSecond != 0
                                   ? m_pScene->mAnimations[0]->mTicksPerSecond : 25.0f);
    float TimeInTicks = TimeInSeconds * TicksPerSecond;
    float AnimationTime = fmod(TimeInTicks, (float) m_pScene->mAnimations[0]->mDuration);

    ReadNodeHeirarchy(AnimationTime, m_pScene->mRootNode, Identity);

    transforms.resize(m_NumBones);

    for (uint i = 0; i < m_NumBones; i++) {
        transforms[i] = m_BoneInfo[i].FinalTransformation;
    }
}


void AnimModel::swap_animation() {
    anim_id = (anim_id + 1) % anim_names.size();
    change_animation(directory + anim_names[anim_id]);
}

void AnimModel::change_animation(std::string path) {
    Clear();
    load_mesh(path);
}

void AnimModel::rotate(float delta) {
    model = glm::rotate(model, glm::radians(delta), glm::vec3(0.0, 0.0, 1.0));
}

void AnimModel::feed(float k) {
    scale *= k;
    model = glm::scale(model, glm::vec3(k));
}

void AnimModel::run() {

}

const aiNodeAnim *AnimModel::FindNodeAnim(const aiAnimation *pAnimation, const string NodeName) {
    for (uint i = 0; i < pAnimation->mNumChannels; i++) {
        const aiNodeAnim *pNodeAnim = pAnimation->mChannels[i];

        if (string(pNodeAnim->mNodeName.data) == NodeName) {
            return pNodeAnim;
        }
    }

    return nullptr;
}


std::vector<AnimTexture> AnimModel::load_material_textures(aiMaterial *mat, aiTextureType type, std::string type_name) {
    std::vector<AnimTexture> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {

        aiString str;
        mat->GetTexture(type, i, &str);

        AnimTexture texture;
        texture.id = texture_from_file(str.C_Str(), this->directory);
        texture.type = type_name;
        texture.path = str.C_Str();

        textures.push_back(texture);
        textures_loaded.push_back(texture);
    }

    return textures;
}

static unsigned int texture_from_file(const char *path, const std::string &directory) {
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


void AnimModel::VertexBoneData::AddBoneData(uint BoneID, float Weight) {
    for (uint i = 0; i < ARRAY_SIZE_IN_ELEMENTS(IDs); i++) {
        if (Weights[i] == 0.0) {
            IDs[i] = BoneID;
            Weights[i] = Weight;
            return;
        }
    }

    assert(0);
}