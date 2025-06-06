#include "Renderer/OpenGL/ogl_program_object.h"
#include "Renderer/OpenGL/ogl_check.h"
#include "Renderer/OpenGL/ogl_uniform.h"
#include <Core/material_global.h>

namespace volucris
{
    OGLShaderObject::OGLShaderObject(GLenum type, const std::string& source)
        : m_type(type)
        , m_id(0)
        , m_source(source)
        , m_dirty(true)
    {
    }

    OGLShaderObject::~OGLShaderObject()
    {
        release();
    }

    bool OGLShaderObject::create()
    {
        if (m_id == 0)
        {
            m_id = glCreateShader(m_type);
        }
        return m_id > 0;
    }

    bool volucris::OGLShaderObject::initialize()
    {
        if (!m_dirty)
        {
            return true;
        }

        if (m_id == 0)
        {
            return false;
        }

        const auto ss = m_source.c_str();
        glShaderSource(m_id, 1, &ss, nullptr);
        glCompileShader(m_id);
        char infoLog[512];
        int success;
        glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(m_id, 512, NULL, infoLog);
            V_LOG_WARN(Engine, "shader compile failed.");
            V_LOG_WARN(Engine, "{}", infoLog);
            release();
            return false;
        }
        GL_CHECK();
        m_dirty = false;
        return true;
    }

    void OGLShaderObject::release()
    {
        if (m_id > 0)
        {
            glDeleteShader(m_id);
            m_id = 0;
        }
    }

    OGLProgramObject::OGLProgramObject()
        : m_dirty(true)
        , m_autoReleaseShader(false)
        , m_id(0)
        , m_shaders()
        , m_parameterUniforms()
        , m_blockUniforms()
    {
    }

    OGLProgramObject::~OGLProgramObject()
    {
        release();
    }

    bool OGLProgramObject::create()
    {
        if (m_id == 0)
        {
            m_id = glCreateProgram();
        }
        return m_id > 0;
    }

    bool OGLProgramObject::initialize()
    {
        if (!m_dirty)
        {
            return true;
        }
        
        for (const auto& shader : m_shaders)
        {
            if (!(shader->isDirty() && shader->create() && shader->initialize()))
            {
                V_LOG_WARN(Engine, "initialize program failed. some shader invalid.");
                return false;
            }
        }

        if (m_id == 0)
        {
            V_LOG_WARN(Engine, "initialize program failed. program not generated.");
            return false;
        }

        for (const auto& shader : m_shaders)
        {
            glAttachShader(m_id, shader->getID());
        }

        glLinkProgram(m_id);

        int success;
        char infoLog[512];

        glGetProgramiv(m_id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(m_id, 512, NULL, infoLog);
            V_LOG_WARN(Engine, "program link failed.");
            V_LOG_WARN(Engine, "{}", infoLog);
            release();
            return false;
        }

        for (const auto& uniform : m_parameterUniforms)
        {
            auto location = glGetUniformLocation(m_id, uniform->getName().c_str());
            uniform->setLocation(location);
        }

        for (const auto& uniform : m_blockUniforms)
        {
            auto location = glGetUniformBlockIndex(m_id, uniform->getName().c_str());
            uniform->setLocation(location);
        }

        autoReleaseShaders();
        GL_CHECK();
        m_dirty = false;
        return true;
    }

    void OGLProgramObject::release()
    {
        autoReleaseShaders();
        if (m_id > 0)
        {
            glDeleteProgram(m_id);
        }
    }

    void OGLProgramObject::autoReleaseShaders()
    {
        if (m_autoReleaseShader)
        {
            for (const auto& shader : m_shaders)
            {
                if (shader.use_count() == 1)
                {
                    shader->release();
                }
            }
        }
    }
}
