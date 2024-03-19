#include "Model.h"

std::vector<Mesh> Model::BlendShapes;
Eigen::VectorXf Model::weights;
Eigen::MatrixXf Model::B_Delta;
Eigen::MatrixXf Model::B_w;
Eigen::MatrixXf Model::B_Bar;
Eigen::VectorXf Model::F0;
Eigen::VectorXf Model::F;
Mesh Model::Result;
GLuint Model::ResultVAO;

std::vector<GLuint> Model::constraints_m0;
std::vector<GLuint> Model::constraints_m;
Eigen::VectorXf Model::m0(3 * Model::constraints_m0.size(), 1);
Eigen::VectorXf Model::m(3 * Model::constraints_m.size(), 1);

double Model::animation_data[6000];
Mesh::Mesh(const char* fileName) {
    CreateMesh(fileName);
    model = glm::mat4(1.0f);
}

void Mesh::CreateMesh(const char* fileName) {
   
    const aiScene* scene = aiImportFile(
        fileName,
        aiProcess_Triangulate | aiProcess_PreTransformVertices | aiProcess_CalcTangentSpace | aiProcess_GenNormals
    );

    if (!scene) {
        std::cerr << "Error Reading Mesh : " << fileName << std::endl;
        return ;
    }

    std::cout << "Meshes : " << scene->mNumMeshes<< std::endl;

    for (unsigned int m_i = 0; m_i < scene->mNumMeshes; m_i++) {
        const aiMesh* mesh = scene->mMeshes[m_i];
        totalPoints += mesh->mNumVertices;
        for (unsigned int v_i = 0; v_i < mesh->mNumVertices; v_i++) {
            if (mesh->HasPositions()) {
                const aiVector3D* vertex = &(mesh->mVertices[v_i]);
                vertices.push_back(glm::vec3(vertex->x, vertex->y, vertex->z));
                VertexData.conservativeResize(vertices.size() * 3, 1);
                VertexData(3 * vertices.size() - 3) = vertex->x;
                VertexData(3 * vertices.size() - 2) = vertex->y;
                VertexData(3 * vertices.size() - 1) = vertex->z;
            }
            if (mesh->HasNormals()) {
                const aiVector3D* normal = &(mesh->mNormals[v_i]);
                normals.push_back(glm::vec3(normal->x, normal->y, normal->z));
            }
            if (mesh->HasTextureCoords(0)) {
                const aiVector3D* textureCoordinates = &(mesh->mTextureCoords[0][v_i]);
                textureCoords.push_back(glm::vec2(textureCoordinates->x, textureCoordinates->y));

            }
            if (mesh->HasTangentsAndBitangents()) {
                const aiVector3D* tangent = &(mesh->mTangents[v_i]);
                tangents.push_back(glm::vec3(tangent->x, tangent->y, tangent->z));

                const aiVector3D* biTangent = &(mesh->mBitangents[v_i]);
                biTangents.push_back(glm::vec3(biTangent->x, biTangent->y, biTangent->z));
            }
        }
    }
    
    if (std::strstr(fileName, "picker") == nullptr) { Model::BlendShapes.push_back(*this); }
    aiReleaseImport(scene);
    return;
}

void Model::Load(Mesh mesh) {
    GLuint aPos = 0;
    GLuint aNormal = 1;
    GLuint aTexCords = 2;
    GLuint aTangent = 3;
    GLuint aBiTangent = 4;

    unsigned int aPosVBO = 0;
    unsigned int aTexCordsVBO = 0;
    unsigned int aNormalVBO = 0;
    unsigned int aTangentVBO = 0;
    unsigned int aBiTangentVBO = 0;

    glGenBuffers(1, &aPosVBO);
    glBindBuffer(GL_ARRAY_BUFFER, aPosVBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * mesh.totalPoints * sizeof(float), mesh.VertexData.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &aNormalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, aNormalVBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * mesh.totalPoints * sizeof(float), &mesh.normals[0], GL_STATIC_DRAW);

    if (mesh.textureCoords.size() > 0) {
        glGenBuffers(1, &aTexCordsVBO);
        glBindBuffer(GL_ARRAY_BUFFER, aTexCordsVBO);
        glBufferData(GL_ARRAY_BUFFER, 3 * mesh.totalPoints * sizeof(float), &mesh.textureCoords[0], GL_STATIC_DRAW);
    }
    if (mesh.tangents.size() > 0) {
        glGenBuffers(1, &aTangentVBO);
        glBindBuffer(GL_ARRAY_BUFFER, aTangentVBO);
        glBufferData(GL_ARRAY_BUFFER, 3 * mesh.totalPoints * sizeof(float), &mesh.tangents[0], GL_STATIC_DRAW);
    }
    
    if (mesh.biTangents.size() > 0) {
        glGenBuffers(1, &aBiTangentVBO);
        glBindBuffer(GL_ARRAY_BUFFER, aBiTangentVBO);
        glBufferData(GL_ARRAY_BUFFER, 3 * mesh.totalPoints * sizeof(float), &mesh.biTangents[0], GL_STATIC_DRAW);
    }

    

    glGenVertexArrays(1, &ResultVAO);
    glBindVertexArray(ResultVAO);

    glEnableVertexAttribArray(aPos);
    glBindBuffer(GL_ARRAY_BUFFER, aPosVBO);
    glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(aNormal);
    glBindBuffer(GL_ARRAY_BUFFER, aNormalVBO);
    glVertexAttribPointer(aNormal, 3, GL_FLOAT, GL_FALSE, 0, NULL);


    if (mesh.textureCoords.size() > 0) {
        glEnableVertexAttribArray(aTexCords);
        glBindBuffer(GL_ARRAY_BUFFER, aTexCordsVBO);
        glVertexAttribPointer(aTexCords, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    }
    if (mesh.tangents.size() > 0) {
        glEnableVertexAttribArray(aTangent);
        glBindBuffer(GL_ARRAY_BUFFER, aTangentVBO);
        glVertexAttribPointer(aTangent, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    }

    if (mesh.biTangents.size() > 0) {
        glEnableVertexAttribArray(aBiTangent);
        glBindBuffer(GL_ARRAY_BUFFER, aBiTangentVBO);
        glVertexAttribPointer(aBiTangent, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    }
}

void Model::Display(Camera camera,Shader shader,glm::mat4 model) {
    shader.Activate();

    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(camera.projection));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(camera.view));

    glBindVertexArray(ResultVAO);
    glDrawArrays(GL_TRIANGLES, 0, Result.totalPoints);
}

void Model::Initialize() {

    std::ifstream AnimationFile("./src/animation-data.txt");
    if (!AnimationFile.is_open()) {
        std::cerr << "Error opening Animation File!" << std::endl;
    }

    for (int i = 0; i < 6000; i++) {
        if (!(AnimationFile >> animation_data[i])) {
            std::cerr << "Error reading data from Animation file!" << std::endl;
        }
    }

    F0.conservativeResize(BlendShapes[0].VertexData.size(), 1);
    F0 = BlendShapes[0].VertexData;

    B_Delta.conservativeResize(BlendShapes[0].VertexData.size(), BlendShapes.size() - 1); 
    B_w.conservativeResize(BlendShapes[0].VertexData.size(), BlendShapes.size() - 1);

    weights.conservativeResize(BlendShapes.size() - 1, 1);
    for (int i = 0; i < BlendShapes.size() - 1; i++) {
        weights(i, 0) = 0.0;
    }

    for (int i = 2; i < BlendShapes.size(); i++) {
        for (int j = 0; j < BlendShapes[0].VertexData.size(); j++) {
            B_Delta(j, i - 1) = BlendShapes[i].VertexData(j, 0);
        }
    }

    Result = BlendShapes[0];
}

void Model::Blend() {

    for (int i = 1; i < BlendShapes.size(); i++) {
        for (int j = 0; j < BlendShapes[0].VertexData.rows(); j++) {
            int q = i - 1;
            B_Delta(j, q) = BlendShapes[i].VertexData(j, 0) - F0(j, 0);
            B_w(j, q) = B_Delta(j, q) * weights(q, 0); 
        }
    }

    for (int i = 0; i < BlendShapes[0].VertexData.rows(); i++) {
        float total = 0; 
        for (int j = 0; j < BlendShapes.size() - 1; j++) {
            total += B_w(i, j); 
        }
        Result.VertexData(i, 0) = F0(i, 0) + total; 
    }
}
