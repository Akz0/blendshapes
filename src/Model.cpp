#include "Model.h"

std::vector<Mesh> Model::BlendShapes;
Eigen::VectorXf Model::weights;
Eigen::MatrixXf Model::B_DeltaBlendShapes;
Eigen::MatrixXf Model::B_w;
Eigen::MatrixXf Model::B_Bar;
Eigen::VectorXf Model::f0;
Eigen::VectorXf Model::F;
Mesh Model::Result;
GLuint Model::ResultVAO;

static double animation_data[6000];
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

    Model::BlendShapes.push_back(*this);

    aiReleaseImport(scene);
    return;
}


void Model::Load(Mesh mesh, Shader shader) {
    unsigned int aPosVBO = 0;
    GLuint aPos = 0;
    GLuint aNormal = 1;

    glGenBuffers(1, &aPosVBO);
    glBindBuffer(GL_ARRAY_BUFFER, aPosVBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * mesh.totalPoints * sizeof(float), mesh.VertexData.data(), GL_STATIC_DRAW);

    unsigned int aNormalVBO = 0;
    glGenBuffers(1, &aNormalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, aNormalVBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * mesh.totalPoints * sizeof(float), &mesh.normals[0], GL_STATIC_DRAW);

    glGenVertexArrays(1, &Model::ResultVAO);
    glBindVertexArray(Model::ResultVAO);

    glEnableVertexAttribArray(aPos);
    glBindBuffer(GL_ARRAY_BUFFER, aPosVBO);
    glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(aNormal);
    glBindBuffer(GL_ARRAY_BUFFER, aNormalVBO);
    glVertexAttribPointer(aNormal, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    Model::Result = mesh;
}


void Model::Display(Camera camera,Shader shader,glm::mat4 model) {
    shader.Activate();

    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(camera.projection));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(camera.view));

    glBindVertexArray(Model::ResultVAO);
    glDrawArrays(GL_TRIANGLES, 0, Model::Result.totalPoints);
}