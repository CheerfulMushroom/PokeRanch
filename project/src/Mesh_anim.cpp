#include <Mesh_anim.h>

void VertexBoneData::add_bone_data(unsigned int bone_id, float weight) {
    for (unsigned int i = 0; i < NUM_BONES_PER_VERTEX; i++) {
        if (weights[i] == 0.0) {
            bone_IDs[i] = bone_id;
            weights[i] = weight;
            return;
        }
    }
}


Mesh::Mesh(unsigned int mesh_id, unsigned int base_vertex_id, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, std::vector<VertexBoneData> bones) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->mesh_id = mesh_id;
    this->first_vertex_id = base_vertex_id;
    this->bones = bones;

    setup_mesh();

    //std::cout << "Mesh_id: " << this->mesh_id << std::endl;
    //std::cout << "Base vertex number: " << this->first_vertex_id << std::endl;
}

void Mesh::setup_mesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &BONE_VB);


    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, tex_coords));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, BONE_VB);
    glBufferData(GL_ARRAY_BUFFER, bones.size() * sizeof(VertexBoneData), &bones[0], GL_STATIC_DRAW);
    
    glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexBoneData), (void*) 0);
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (void *) 16);
    glEnableVertexAttribArray(4);
    
    glBindVertexArray(0);

}

void Mesh::draw_mesh(ShaderProgram shader) {
    // bind appropriate textures
        unsigned int diffuseNr  = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr   = 1;
        unsigned int heightNr   = 1;
        for(unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            std::string number;
            std::string name = textures[i].type;
            if(name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if(name == "texture_specular")
				number = std::to_string(specularNr++); // transfer unsigned int to stream
            else if(name == "texture_normal")
				number = std::to_string(normalNr++); // transfer unsigned int to stream
             else if(name == "texture_height")
			    number = std::to_string(heightNr++); // transfer unsigned int to stream

													 // now set the sampler to the correct texture unit
            glUniform1i(glGetUniformLocation(shader.program, (name + number).c_str()), i);
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        
        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);

}
