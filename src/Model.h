#pragma once
#ifndef MODEL_CLASS
#define MODEL_CLASS

#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>

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

#define NEUTRAL "./src/model/neutral.obj"                                     
                                            
#define JAW_OPEN "./src/model/Mery_jaw_open.obj"                        //              Weight Index: 0
#define KISS "./src/model/Mery_kiss.obj"                                //             Weight Index: 1
                                                                                             
#define L_BROW_LOWER "./src/model/Mery_l_brow_lower.obj"                //             Weight Index: 2
#define L_BROW_NARROW "./src/model/Mery_l_brow_narrow.obj"              //             Weight Index: 3
#define L_BROW_RAISE "./src/model/Mery_l_brow_raise.obj"                //          Weight Index: 4
#define LEFT_CLOSED "./src/model/Mery_l_eye_closed.obj"                 //         Weight Index: 5
#define L_LOWER_O "./src/model/Mery_l_eye_lower_open.obj"               //             Weight Index: 6
#define L_UPPER_O "./src/model/Mery_l_eye_upper_open.obj"               //             Weight Index: 7
#define L_NOSE_WRINKLE "./src/model/Mery_l_nose_wrinkle.obj"            //            Weight Index: 8
#define L_PUFF "./src/model/Mery_l_puff.obj"                            //          Weight Index: 9
#define L_SAD "./src/model/Mery_l_sad.obj"                              //         Weight Index: 10
#define L_SMILE "./src/model/Mery_l_smile.obj"                          //             Weight Index: 11
#define L_SUCK "./src/model/Mery_l_suck.obj"                            //            Weight Index: 12
// Right Blendshape Models                                                                                                
#define R_BROW_LOWER "./src/model/Mery_r_brow_lower.obj"                //            Weight Index: 13
#define R_BROW_NARROW "./src/model/Mery_r_brow_narrow.obj"              //            Weight Index: 14
#define R_BROW_RAISE "./src/model/Mery_r_brow_raise.obj"                //            Weight Index: 15
#define REFT_CLOSED "./src/model/Mery_r_eye_closed.obj"                 //             Weight Index: 16
#define R_LOWER_O "./src/model/Mery_r_eye_lower_open.obj"               //           Weight Index: 17
#define R_UPPER_O "./src/model/Mery_r_eye_upper_open.obj"               //           Weight Index: 18
#define R_NOSE_WRINKLE "./src/model/Mery_r_nose_wrinkle.obj"            //           Weight Index: 19
#define R_PUFF "./src/model/Mery_r_puff.obj"                            //             Weight Index: 20
#define R_SAD "./src/model/Mery_r_sad.obj"                              //            Weight Index: 21
#define R_SMILE "./src/model/Mery_r_smile.obj"                          //            Weight Index: 22
#define R_SUCK "./src/model/Mery_r_suck.obj"





#define NEUTRAL_LOW_RES "./src/low-res-model/neutral.obj"                                      
                                              
#define JAW_OPEN_LOW_RES "./src/low-res-model/Mery_jaw_open.obj"                        //             Weight Index: 0
#define KISS_LOW_RES "./src/low-res-model/Mery_kiss.obj"                                //             Weight Index: 1
                                                                                              
#define L_BROW_LOWER_LOW_RES "./src/low-res-model/Mery_l_brow_lower.obj"                //             Weight Index: 2
#define L_BROW_NARROW_LOW_RES "./src/low-res-model/Mery_l_brow_narrow.obj"              //             Weight Index: 3
#define L_BROW_RAISE_LOW_RES "./src/low-res-model/Mery_l_brow_raise.obj"                //              Weight Index: 4
#define LEFT_CLOSED_LOW_RES "./src/low-res-model/Mery_l_eye_closed.obj"                 //             Weight Index: 5
#define L_LOWER_O_LOW_RES "./src/low-res-model/Mery_l_eye_lower_open.obj"               //              Weight Index: 6
#define L_UPPER_O_LOW_RES "./src/low-res-model/Mery_l_eye_upper_open.obj"               //             Weight Index: 7
#define L_NOSE_WRINKLE_LOW_RES "./src/low-res-model/Mery_l_nose_wrinkle.obj"            //             Weight Index: 8
#define L_PUFF_LOW_RES "./src/low-res-model/Mery_l_puff.obj"                            //            Weight Index: 9
#define L_SAD_LOW_RES "./src/low-res-model/Mery_l_sad.obj"                              //             Weight Index: 10
#define L_SMILE_LOW_RES "./src/low-res-model/Mery_l_smile.obj"                          //           Weight Index: 11
#define L_SUCK_LOW_RES "./src/low-res-model/Mery_l_suck.obj"                            //            Weight Index: 12
// Right Blendshape Models                                                                                                
#define R_BROW_LOWER_LOW_RES "./src/low-res-model/Mery_r_brow_lower.obj"                //             Weight Index: 13
#define R_BROW_NARROW_LOW_RES "./src/low-res-model/Mery_r_brow_narrow.obj"              //            Weight Index: 14
#define R_BROW_RAISE_LOW_RES "./src/low-res-model/Mery_r_brow_raise.obj"                //             Weight Index: 15
#define R_CLOSED_LOW_RES "./src/low-res-model/Mery_r_eye_closed.obj"                     //             Weight Index: 16
#define R_LOWER_O_LOW_RES "./src/low-res-model/Mery_r_eye_lower_open.obj"               //             Weight Index: 17
#define R_UPPER_O_LOW_RES "./src/low-res-model/Mery_r_eye_upper_open.obj"               //           Weight Index: 18
#define R_NOSE_WRINKLE_LOW_RES "./src/low-res-model/Mery_r_nose_wrinkle.obj"            //             Weight Index: 19
#define R_PUFF_LOW_RES "./src/low-res-model/Mery_r_puff.obj"                            //           Weight Index: 20
#define R_SAD_LOW_RES "./src/low-res-model/Mery_r_sad.obj"                              //             Weight Index: 21
#define R_SMILE_LOW_RES "./src/low-res-model/Mery_r_smile.obj"                          //            Weight Index: 22
#define R_SUCK_LOW_RES "./src/low-res-model/Mery_r_suck.obj"                            //             Weight Index: 23

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
    static Eigen::MatrixXf B_Delta;
    static Eigen::MatrixXf B_w;
    static Eigen::MatrixXf B_Bar;
    static Eigen::VectorXf F0;
    static Eigen::VectorXf F;
    static Mesh Result;
    static GLuint ResultVAO;

    static std::vector<GLuint> constraints_m0;
    static std::vector<GLuint> constraints_m;
    static Eigen::VectorXf m0;
    static Eigen::VectorXf m;


    static double animation_data[6000];
    
    static void Initialize();
    static void Load(Mesh mesh);
    static void Display(Camera camera,Shader shader, glm::mat4 model);
    static void Blend();
    static void Animate();
};

#endif // !MODEL_CLASS