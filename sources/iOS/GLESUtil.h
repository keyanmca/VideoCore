//
//  GLESUtil.h
//  videocore
//
//  Created by James Hurley on 1/15/14.
//  Copyright (c) 2014 Raster Multimedia. All rights reserved.
//

#ifndef videocore_GLESUtil_h
#define videocore_GLESUtil_h


#define BUFFER_OFFSET(i) ((void*)(i))
#ifdef DEBUG
#define GL_ERRORS(line) { GLenum glerr; while((glerr = glGetError())) {\
switch(glerr)\
{\
case GL_NO_ERROR:\
break;\
case GL_INVALID_ENUM:\
printf("OGL(" __FILE__ "):: %d: Invalid Enum\n", line );\
break;\
case GL_INVALID_VALUE:\
printf("OGL(" __FILE__ "):: %d: Invalid Value\n", line );\
break;\
case GL_INVALID_OPERATION:\
printf("OGL(" __FILE__ "):: %d: Invalid Operation\n", line );\
break;\
case GL_OUT_OF_MEMORY:\
printf("OGL(" __FILE__ "):: %d: Out of Memory\n", line );\
break;\
} } }

#define GL_FRAMEBUFFER_STATUS(line) { GLenum status; status = glCheckFramebufferStatus(GL_FRAMEBUFFER); {\
switch(status)\
{\
case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:\
printf("OGL(" __FILE__ "):: %d: Incomplete attachment\n", line);\
break;\
case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:\
printf("OGL(" __FILE__ "):: %d: Incomplete dimensions\n", line);\
break;\
case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:\
printf("OGL(" __FILE__ "):: %d: Incomplete missing attachment\n", line);\
break;\
case GL_FRAMEBUFFER_UNSUPPORTED:\
printf("OGL(" __FILE__ "):: %d: Framebuffer combination unsupported\n",line);\
break;\
} } }

#else
#define GL_ERRORS(line)
#define GL_FRAMEBUFFER_STATUS(line)
#endif

static float s_vbo [] =
{
    -1.f, -1.f,       0.f, 0.f, // 0
    1.f, -1.f,        1.f, 0.f, // 1
    -1.f, 1.f,        0.f, 1.f, // 2
    
    1.f, -1.f,        1.f, 0.f, // 1
    1.f, 1.f,         1.f, 1.f, // 3
    -1.f, 1.f,        0.f, 1.f, // 2
};


static const char s_vs [] =
"attribute vec2 aPos;"
"attribute vec2 aCoord;"
"varying vec2 vCoord;"
"void main(void) {"
"gl_Position = vec4(aPos,0.,1.);"
"vCoord = aCoord;"
"}";
static const char s_vs_mat [] =
"attribute vec2 aPos;"
"attribute vec2 aCoord;"
"varying vec2 vCoord;"
"uniform mat4 uMat;"
"void main(void) {"
"gl_Position = uMat * vec4(aPos,0.,1.);"
"vCoord = aCoord;"
"}"
;
static const char s_fs[] =
"precision mediump float;"
"varying vec2 vCoord;"
"uniform sampler2D uTex0;"
"void main(void) {"
"gl_FragData[0] = texture2D(uTex0, vCoord);"
"}";

static inline GLuint compile_shader(GLuint type, const char * source)
{
    
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
#ifdef DEBUG
    if (!compiled) {
        GLint length;
        char *log;
        
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        
        log = (char*)malloc((size_t)(length));
        glGetShaderInfoLog(shader, length, &length, &log[0]);
        printf("%s compilation error: %s\n", (type == GL_VERTEX_SHADER ? "GL_VERTEX_SHADER" : "GL_FRAGMENT_SHADER"), log);
        free(log);
        
        return 0;
    }
#endif
    return shader;
    
}

static inline GLuint build_program(const char * vertex, const char * fragment)
{
    GLuint  vshad,
    fshad,
    p;
    
    GLint   len;
#ifdef DEBUG
    char*   log;
#endif
    
    vshad = compile_shader(GL_VERTEX_SHADER, vertex);
    fshad = compile_shader(GL_FRAGMENT_SHADER, fragment);
    
    p = glCreateProgram();
    glAttachShader(p, vshad);
    glAttachShader(p, fshad);
    glLinkProgram(p);
    glGetProgramiv(p,GL_INFO_LOG_LENGTH, &len);
    
    
#ifdef DEBUG
    if(len) {
        log = (char*)malloc ( (size_t)(len) ) ;
        
        glGetProgramInfoLog(p, len, &len, log);
        
        printf("program log: %s\n", log);
        free(log);
    }
#endif
    
    
    glDeleteShader(vshad);
    glDeleteShader(fshad);
    return p;
}


#endif