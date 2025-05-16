#include "Renderer/OpenGL/ogl_program_object.h"
#include "Renderer/OpenGL/ogl_check.h"
#include "Renderer/OpenGL/ogl_uniform.h"

namespace volucris
{
    OGLShaderObject::OGLShaderObject(GLenum type, const std::string& source)
        : m_type(type)
        , m_id(0)
        , m_source(source)
    {
    }

    OGLShaderObject::~OGLShaderObject()
    {
        release();
    }

    bool volucris::OGLShaderObject::initialize()
    {
        m_id = glCreateShader(m_type);
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
        : m_id(0)
        , m_autoReleaseShader(false)
        , m_shaders()
    {
    }

    OGLProgramObject::~OGLProgramObject()
    {
        release();
    }

    bool OGLProgramObject::initialize()
    {
        for (const auto& shader : m_shaders)
        {
            if (shader->getID() == 0 && !shader->initialize())
            {
                V_LOG_WARN(Engine, "initialize program failed. some shader invalid.");
                return false;
            }
        }

        if (m_id == 0)
        {
            m_id = glCreateProgram();
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

        for (auto& desc : m_uniformDescriptions)
        {
            desc->location = glGetUniformLocation(m_id, desc->desc.name.c_str());
        }

        for (auto& desc : m_uniformBlockDescriptions)
        {
            desc.location = glGetUniformBlockIndex(m_id, desc.desc.name.c_str());
        }

        autoReleaseShaders();
        GL_CHECK();
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

    void OGLProgramObject::setUniformDescriptions(const std::vector<std::shared_ptr<UniformDescription>>& descritions, const std::vector<UniformDescription>& uniformBlockDescriptions)
    {
        m_uniformDescriptions = descritions;
        m_uniformBlockDescriptions = uniformBlockDescriptions;
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
