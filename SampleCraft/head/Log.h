#pragma once
#include <cstdio>
#include <glad/glad.h>

class Log
{
public:
    static void GLAPIENTRY
        messageCallback(GLenum source,
            GLenum type,
            GLuint id,
            GLenum severity,
            GLsizei length,
            const GLchar* message,
            const void* userParam)
    {
        fprintf(stderr, "LINE: %d GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            __LINE__,(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);
    }

    Log()
    {
	    
    }
    ~Log()
    {
	    
    }
};
