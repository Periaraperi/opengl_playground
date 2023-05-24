#pragma once

void log(const char* mesg);
void log_error(const char*, const char*, int, const char* mesg="");
void clear_gl_errors();
bool gl_check_errors();
int sdl_check(int, const char*, const char*, int);
void* sdl_check(void*, const char*, const char*, int);

#ifdef PERIA_PAINT_DEBUG
    #define PERIA_LOG(...) \
        log(__VA_ARGS__)
    #define PERIA_ASSERT(expr) \
        if (!(expr)) { \
            log_error(#expr,__FILE__,__LINE__); \
            exit(EXIT_FAILURE); \
        }
    #define PERIA_ASSERT_SDL(expr,expr_str,file,line,mesg) \
        if (!(expr)) { \
            log_error(expr_str,file,line,mesg); \
            exit(EXIT_FAILURE); \
        }
    #define PERIA_ASSERT_GL(expr,expr_str,file,line) \
        if (!(expr)) { \
            log_error(expr_str,file,line); \
            exit(EXIT_FAILURE); \
        }
#else
    #define PERIA_LOG(...)
    #define PERIA_ASSERT(expr) expr
    #define PERIA_ASSERT_SDL(expr,expr_str,file,line,mesg) 
    #define PERIA_ASSERT_GL(expr,expr_str,file,line) 
#endif

#define SDL_CALL(expr) sdl_check(expr,#expr,__FILE__,__LINE__)

#define GL_CALL(expr) clear_gl_errors(); \
    expr; \
    PERIA_ASSERT_GL(!gl_check_errors(),#expr,__FILE__,__LINE__)

