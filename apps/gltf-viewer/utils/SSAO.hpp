//
// Created by jeremie on 03/07/2020.
//

#ifndef GLTF_VIEWER_TUTORIAL_SSAO_HPP
#define GLTF_VIEWER_TUTORIAL_SSAO_HPP

#include <GL/glew.h>
#include "./GBuffer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>

class SSAO{
public:

    SSAO() = default;

    ~SSAO() = default;

private:

    static void GenKernel();

    const static uint KERNEL_SIZE = 64;

    buffer::GBuffer mGBuffer;

    static GLfloat Lerp(GLfloat a, GLfloat b, GLfloat f);
};

#endif //GLTF_VIEWER_TUTORIAL_SSAO_HPP
