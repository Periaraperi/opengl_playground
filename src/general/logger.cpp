#include "logger.hpp"

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <iostream>

void log_error(const char* expr, const char* file, int line, const char* mesg)
{
    std::cerr << expr << " failed at\n"
              << "File: " << file << '\n'
              << "Line: " << line << '\n';
    if (std::string(mesg)!="") 
        std::cerr << mesg << '\n';
}

// ============================= OPENGL ====================================
std::string gl_description(int err)
{
    switch (err) {
        case 1280: return "GL_INVALID_ENUM";
        case 1281: return "GL_INVALID_VALUE";
        case 1282: return "GL_INVALID_OPERATION";
        case 1283: return "GL_STACK_OVERFLOW";
        case 1284: return "GL_STACK_UNDERFLOW";
        case 1285: return "GL_OUT_OF_MEMORY";
        case 1286: return "GL_INVALID_FRAMEBUFFER_OPERATION";
    }
    return "";
}

void clear_gl_errors()
{
    while (glGetError()!=GL_NO_ERROR);
}

bool gl_check_errors()
{
    while (auto err=glGetError()) {
        std::cerr << "[OPENGL Error]: " << err << " --> " << gl_description(err) << '\n';
        return true;
    }
    return false;
}
// =============================== SDL ================================ 
int sdl_check(int code,
              const char* expr,
              const char* file,
              int line)
{
    PERIA_ASSERT_SDL(code==0,expr,file,line,SDL_GetError());
    return code;
}

void* sdl_check(void* ptr,
                      const char* expr,
                      const char* file,
                      int line)
{
    PERIA_ASSERT_SDL(ptr!=nullptr,expr,file,line,SDL_GetError());
    return ptr;
}

