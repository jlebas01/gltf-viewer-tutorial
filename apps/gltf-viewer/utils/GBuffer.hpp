#ifndef GLTF_VIEWER_TUTORIAL_GBUFFER_HPP
#define GLTF_VIEWER_TUTORIAL_GBUFFER_HPP

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>

namespace buffer {

#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

#define GBUFFER_POSITION_TEXTURE_UNIT 0
#define GBUFFER_DIFFUSE_TEXTURE_UNIT  1
#define GBUFFER_NORMAL_TEXTURE_UNIT   2

    class GBuffer {

    public:
        enum GBUFFER_TEXTURE_TYPE {
            GBUFFER_TEXTURE_TYPE_POSITION,
            GBUFFER_TEXTURE_TYPE_DIFFUSE,
            GBUFFER_TEXTURE_TYPE_NORMAL,
            GBUFFER_TEXTURE_TYPE_TEXCOORD,
            GBUFFER_NUM_TEXTURES
        };

        GBuffer();

        ~GBuffer();

        bool Init(unsigned int windowWidth, unsigned int windowHeight);

        void StartFrame() const;

        void BindForGeomPass() const;

        static void BindForStencilPass();

        void BindForLightPass();

        void BindForFinalPass() const;

    private:

        GLuint mFBO;
        GLuint mTextures[GBUFFER_NUM_TEXTURES]{};
        GLuint mDepthTexture;
        GLuint mFinalTexture;

    };
} // namespace buffer
#endif // GLTF_VIEWER_TUTORIAL_GBUFFER_HPP
