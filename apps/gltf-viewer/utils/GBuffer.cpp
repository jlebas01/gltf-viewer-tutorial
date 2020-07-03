#include "GBuffer.hpp"
#include <iostream>

namespace buffer {

    GBuffer::GBuffer() {
        mFBO = 0;
        mDepthTexture = 0;
        mFinalTexture = 0;
        ZERO_MEM(mTextures);
    }

    GBuffer::~GBuffer() {
        if (mFBO != 0) {
            glDeleteFramebuffers(1, &mFBO);
        }

        if (mTextures[0] != 0) {
            glDeleteTextures(ARRAY_SIZE_IN_ELEMENTS(mTextures), mTextures);
        }

        if (mDepthTexture != 0) {
            glDeleteTextures(1, &mDepthTexture);
        }

        if (mFinalTexture != 0) {
            glDeleteTextures(1, &mFinalTexture);
        }
    }

    bool GBuffer::Init(unsigned int windowWidth, unsigned int windowHeight) {

        // FBO
        glGenFramebuffers(1, &mFBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);

        // Gbuffer tex
        glGenTextures(ARRAY_SIZE_IN_ELEMENTS(mTextures), mTextures);

        glGenTextures(1, &mDepthTexture);

        glGenTextures(1, &mFinalTexture);

        for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(mTextures); i++) {
            glBindTexture(GL_TEXTURE_2D, mTextures[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, mTextures[i], 0);
        }

        glBindTexture(GL_TEXTURE_2D, mDepthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                     NULL);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture, 0);

        glBindTexture(GL_TEXTURE_2D, mFinalTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, mFinalTexture, 0);

        GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

        if (Status != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << " Frame Buffer error, status : " << Status << std::endl;
            return false;
        }


        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        return true;
    }

    void GBuffer::StartFrame() const {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);
        glDrawBuffer(GL_COLOR_ATTACHMENT4);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void GBuffer::BindForGeomPass() const {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);

        GLenum DrawBuffers[] = {GL_COLOR_ATTACHMENT0,
                                GL_COLOR_ATTACHMENT1,
                                GL_COLOR_ATTACHMENT2};

        glDrawBuffers(ARRAY_SIZE_IN_ELEMENTS(DrawBuffers), DrawBuffers);
    }

    void GBuffer::BindForStencilPass() {
        glDrawBuffer(GL_NONE);
    }

    void GBuffer::BindForLightPass() {
        glDrawBuffer(GL_COLOR_ATTACHMENT4);

        for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(mTextures); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, mTextures[GBUFFER_TEXTURE_TYPE_POSITION + i]);
        }
    }


    void GBuffer::BindForFinalPass() const {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, mFBO);
        glReadBuffer(GL_COLOR_ATTACHMENT4);
    }


} // namespace buffer