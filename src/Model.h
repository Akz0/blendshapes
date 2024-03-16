#pragma once
#ifndef MODEL_CLASS
#define MODEL_CLASS

#include <vector>
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include <assimp/cimport.h> 
#include <assimp/scene.h>
#include <assimp/postprocess.h> 

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Utilities.h"

#define NEUTRAL "./src/model/neutral.obj"                               // FaceMesh Index: 1        
// Middle blendshape models                                                
#define JAW_OPEN "./src/model/Mery_jaw_open.obj"                        // FaceMesh Index: 2             Weight Index: 0
#define KISS "./src/model/Mery_kiss.obj"                                // FaceMesh Index: 3             Weight Index: 1
// Delta Blendshape models                                                                                                
#define L_BROW_LOWER "./src/model/Mery_l_brow_lower.obj"                // FaceMesh Index: 4             Weight Index: 2
#define L_BROW_NARROW "./src/model/Mery_l_brow_narrow.obj"              // FaceMesh Index: 5             Weight Index: 3
#define L_BROW_RAISE "./src/model/Mery_l_brow_raise.obj"                // FaceMesh Index: 6             Weight Index: 4
#define LEFT_CLOSED "./src/model/Mery_l_eye_closed.obj"                 // FaceMesh Index: 7             Weight Index: 5
#define L_LOWER_O "./src/model/Mery_l_eye_lower_open.obj"               // FaceMesh Index: 8             Weight Index: 6
#define L_UPPER_O "./src/model/Mery_l_eye_upper_open.obj"               // FaceMesh Index: 9             Weight Index: 7
#define L_NOSE_WRINKLE "./src/model/Mery_l_nose_wrinkle.obj"            // FaceMesh Index: 10            Weight Index: 8
#define L_PUFF "./src/model/Mery_l_puff.obj"                            // FaceMesh Index: 11            Weight Index: 9
#define L_SAD "./src/model/Mery_l_sad.obj"                              // FaceMesh Index: 12            Weight Index: 10
#define L_SMILE "./src/model/Mery_l_smile.obj"                          // FaceMesh Index: 13            Weight Index: 11
#define L_SUCK "./src/model/Mery_l_suck.obj"                            // FaceMesh Index: 14            Weight Index: 12
// Right Blendshape Models                                                                                                
#define R_BROW_LOWER "./src/model/Mery_r_brow_lower.obj"                // FaceMesh Index: 15            Weight Index: 13
#define R_BROW_NARROW "./src/model/Mery_r_brow_narrow.obj"              // FaceMesh Index: 16            Weight Index: 14
#define R_BROW_RAISE "./src/model/Mery_r_brow_raise.obj"                // FaceMesh Index: 17            Weight Index: 15
#define REFT_CLOSED "./src/model/Mery_r_eye_closed.obj"                 // FaceMesh Index: 18            Weight Index: 16
#define R_LOWER_O "./src/model/Mery_r_eye_lower_open.obj"               // FaceMesh Index: 19            Weight Index: 17
#define R_UPPER_O "./src/model/Mery_r_eye_upper_open.obj"               // FaceMesh Index: 20            Weight Index: 18
#define R_NOSE_WRINKLE "./src/model/Mery_r_nose_wrinkle.obj"            // FaceMesh Index: 21            Weight Index: 19
#define R_PUFF "./src/model/Mery_r_puff.obj"                            // FaceMesh Index: 22            Weight Index: 20
#define R_SAD "./src/model/Mery_r_sad.obj"                              // FaceMesh Index: 23            Weight Index: 21
#define R_SMILE "./src/model/Mery_r_smile.obj"                          // FaceMesh Index: 24            Weight Index: 22
#define R_SUCK "./src/model/Mery_r_suck.obj"                            // FaceMesh Index: 25            Weight Index: 23

class Mesh {
public:
    size_t totalPoints = 0;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> textureCoords;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> biTangents;

    Eigen::VectorXf VertexData;

    glm::mat4 model;

    Mesh() {};
    Mesh(const char* fileName);
    void CreateMesh(const char* fileName);
};

class Model
{
public:
    static std::vector<Mesh> BlendShapes;
    static Eigen::VectorXf weights;
    static Eigen::MatrixXf B_DeltaBlendShapes;
    static Eigen::MatrixXf B_w;
    static Eigen::MatrixXf B_Bar;
    static Eigen::VectorXf f0;
    static Eigen::VectorXf F;
    static Mesh Result;
    static GLuint ResultVAO;

    static double animation_data[6000];
    
    static void Load(Mesh mesh,Shader shader);
    static void Display(Camera camera,Shader shader, glm::mat4 model);
};

#endif // !MODEL_CLASS
