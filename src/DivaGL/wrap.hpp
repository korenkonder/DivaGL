/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "gl.hpp"
#include <Windows.h>

//#define USE_STUB

struct gl_func_struct {
    size_t address;
    size_t func;
};

typedef void (GLAPIENTRY * PFNGLBLENDFUNCDLLPROC) (GLenum sfactor, GLenum dfactor);
typedef void (GLAPIENTRY * PFNGLCOLORMASKDLLPROC) (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
typedef void (GLAPIENTRY * PFNGLDELETETEXTURESDLLPROC) (GLsizei n, GLuint const * textures);
typedef void (GLAPIENTRY * PFNGLDEPTHMASKDLLPROC) (GLboolean flag);
typedef void (GLAPIENTRY * PFNGLFINISHDLLPROC) ();
typedef void (GLAPIENTRY * PFNGLGENTEXTURESDLLPROC) (GLsizei n, GLuint * textures);
typedef void (GLAPIENTRY * PFNGLSCISSORDLLPROC) (GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLVIEWPORTDLLPROC) (GLint x, GLint y, GLsizei width, GLsizei height);
typedef PROC (GLAPIENTRY * PFNWGLGETPROCADDRESSDLLPROC) (LPCSTR lpszProc);

typedef void (GLAPIENTRY * PFNGLACTIVESTENCILFACEEXTPROC) (GLenum face);
typedef void (GLAPIENTRY * PFNGLACTIVETEXTUREARBPROC) (GLenum texture);
typedef void (GLAPIENTRY * PFNGLACTIVEVARYINGNVPROC) (GLuint program, GLchar const * name);
typedef GLboolean (GLAPIENTRY * PFNGLAREPROGRAMSRESIDENTNVPROC) (GLsizei n, GLuint const * programs, GLboolean * residences);
typedef GLboolean (GLAPIENTRY * PFNGLARETEXTURESRESIDENTEXTPROC) (GLsizei n, GLuint const * textures, GLboolean * residences);
typedef void (GLAPIENTRY * PFNGLARRAYELEMENTEXTPROC) (GLint i);
typedef void (GLAPIENTRY * PFNGLATTACHOBJECTARBPROC) (GLhandleARB containerObj, GLhandleARB obj);
typedef void (GLAPIENTRY * PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (GLAPIENTRY * PFNGLBEGINCONDITIONALRENDERPROC) (GLuint id, GLenum mode);
typedef void (GLAPIENTRY * PFNGLBEGINCONDITIONALRENDERNVPROC) (GLuint id, GLenum mode);
typedef void (GLAPIENTRY * PFNGLBEGINOCCLUSIONQUERYNVPROC) (GLuint id);
typedef void (GLAPIENTRY * PFNGLBEGINQUERYPROC) (GLenum target, GLuint id);
typedef void (GLAPIENTRY * PFNGLBEGINQUERYARBPROC) (GLenum target, GLuint id);
typedef void (GLAPIENTRY * PFNGLBEGINTRANSFORMFEEDBACKPROC) (GLenum primitiveMode);
typedef void (GLAPIENTRY * PFNGLBEGINTRANSFORMFEEDBACKNVPROC) (GLenum primitiveMode);
typedef void (GLAPIENTRY * PFNGLBINDATTRIBLOCATIONPROC) (GLuint program, GLuint index, GLchar const * name);
typedef void (GLAPIENTRY * PFNGLBINDATTRIBLOCATIONARBPROC) (GLhandleARB programObj, GLuint index, GLcharARB const * name);
typedef void (GLAPIENTRY * PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLBINDBUFFERBASEPROC) (GLenum target, GLuint index, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLBINDBUFFEROFFSETNVPROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLBINDBUFFERRANGEPROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (GLAPIENTRY * PFNGLBINDBUFFERRANGENVPROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (GLAPIENTRY * PFNGLBINDFRAGDATALOCATIONPROC) (GLuint program, GLuint color, GLchar const * name);
typedef void (GLAPIENTRY * PFNGLBINDFRAGDATALOCATIONEXTPROC) (GLuint program, GLuint color, GLchar const * name);
typedef void (GLAPIENTRY * PFNGLBINDFRAMEBUFFERPROC) (GLenum target, GLuint framebuffer);
typedef void (GLAPIENTRY * PFNGLBINDPROGRAMARBPROC) (GLenum target, GLuint program);
typedef void (GLAPIENTRY * PFNGLBINDPROGRAMNVPROC) (GLenum target, GLuint id);
typedef void (GLAPIENTRY * PFNGLBINDRENDERBUFFERPROC) (GLenum target, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLBINDSAMPLERPROC) (GLuint unit, GLuint sampler);
typedef void (GLAPIENTRY * PFNGLBINDTEXTUREEXTPROC) (GLenum target, GLuint texture);
typedef void (GLAPIENTRY * PFNGLBINDVERTEXARRAYPROC) (GLuint array);
typedef void (GLAPIENTRY * PFNGLBLENDCOLORPROC) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (GLAPIENTRY * PFNGLBLENDCOLOREXTPROC) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONPROC) (GLenum mode);
typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONEXTPROC) (GLenum mode);
typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONSEPARATEPROC) (GLenum modeRGB, GLenum modeAlpha);
typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONSEPARATEEXTPROC) (GLenum modeRGB, GLenum modeAlpha);
typedef void (GLAPIENTRY * PFNGLBLENDFUNCSEPARATEPROC) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
typedef void (GLAPIENTRY * PFNGLBLENDFUNCSEPARATEEXTPROC) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
typedef void (GLAPIENTRY * PFNGLBLITFRAMEBUFFERPROC) (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef void (GLAPIENTRY * PFNGLBLITFRAMEBUFFEREXTPROC) (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef void (GLAPIENTRY * PFNGLBUFFERDATAPROC) (GLenum target, GLsizeiptr size, const void * data, GLenum usage);
typedef void (GLAPIENTRY * PFNGLBUFFERSUBDATAPROC) (GLenum target, GLintptr offset, GLsizeiptr size, const void * data);
typedef void (GLAPIENTRY * PFNGLBUFFERSUBDATAARBPROC) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, const void * data);
typedef GLenum (GLAPIENTRY * PFNGLCHECKFRAMEBUFFERSTATUSPROC) (GLenum target);
typedef GLenum (GLAPIENTRY * PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC) (GLenum target);
typedef GLenum (GLAPIENTRY * PFNGLCHECKNAMEDFRAMEBUFFERSTATUSEXTPROC) (GLuint framebuffer, GLenum target);
typedef void (GLAPIENTRY * PFNGLCLEARBUFFERFIPROC) (GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
typedef void (GLAPIENTRY * PFNGLCLEARBUFFERFVPROC) (GLenum buffer, GLint drawbuffer, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLCLEARBUFFERIVPROC) (GLenum buffer, GLint drawbuffer, GLint const * value);
typedef void (GLAPIENTRY * PFNGLCLEARBUFFERUIVPROC) (GLenum buffer, GLint drawbuffer, GLuint const * value);
typedef void (GLAPIENTRY * PFNGLCLEARCOLORIIEXTPROC) (GLint red, GLint green, GLint blue, GLint alpha);
typedef void (GLAPIENTRY * PFNGLCLEARCOLORIUIEXTPROC) (GLuint red, GLuint green, GLuint blue, GLuint alpha);
typedef void (GLAPIENTRY * PFNGLCLEARDEPTHDNVPROC) (GLdouble depth);
typedef void (GLAPIENTRY * PFNGLCLIENTACTIVETEXTUREARBPROC) (GLenum texture);
typedef void (GLAPIENTRY * PFNGLCLIENTATTRIBDEFAULTEXTPROC) (GLbitfield mask);
typedef void (GLAPIENTRY * PFNGLCOLOR3HNVPROC) (GLhalfNV red, GLhalfNV green, GLhalfNV blue);
typedef void (GLAPIENTRY * PFNGLCOLOR3HVNVPROC) (GLhalfNV const * v);
typedef void (GLAPIENTRY * PFNGLCOLOR4HNVPROC) (GLhalfNV red, GLhalfNV green, GLhalfNV blue, GLhalfNV alpha);
typedef void (GLAPIENTRY * PFNGLCOLOR4HVNVPROC) (GLhalfNV const * v);
typedef void (GLAPIENTRY * PFNGLCOLORMASKIPROC) (GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
typedef void (GLAPIENTRY * PFNGLCOLORMASKINDEXEDEXTPROC) (GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
typedef void (GLAPIENTRY * PFNGLCOLORPOINTEREXTPROC) (GLint size, GLenum type, GLsizei stride, GLsizei count, const void * pointer);
typedef void (GLAPIENTRY * PFNGLCOLORSUBTABLEPROC) (GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const void * data);
typedef void (GLAPIENTRY * PFNGLCOLORSUBTABLEEXTPROC) (GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const void * data);
typedef void (GLAPIENTRY * PFNGLCOLORTABLEPROC) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void * table);
typedef void (GLAPIENTRY * PFNGLCOLORTABLEEXTPROC) (GLenum target, GLenum internalFormat, GLsizei width, GLenum format, GLenum type, const void * table);
typedef void (GLAPIENTRY * PFNGLCOLORTABLEPARAMETERFVPROC) (GLenum target, GLenum pname, GLfloat const * params);
typedef void (GLAPIENTRY * PFNGLCOLORTABLEPARAMETERIVPROC) (GLenum target, GLenum pname, GLint const * params);
typedef void (GLAPIENTRY * PFNGLCOMBINERINPUTNVPROC) (GLenum stage, GLenum portion, GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
typedef void (GLAPIENTRY * PFNGLCOMBINEROUTPUTNVPROC) (GLenum stage, GLenum portion, GLenum abOutput, GLenum cdOutput, GLenum sumOutput, GLenum scale, GLenum bias, GLboolean abDotProduct, GLboolean cdDotProduct, GLboolean muxSum);
typedef void (GLAPIENTRY * PFNGLCOMBINERPARAMETERFNVPROC) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLCOMBINERPARAMETERFVNVPROC) (GLenum pname, GLfloat const * params);
typedef void (GLAPIENTRY * PFNGLCOMBINERPARAMETERINVPROC) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLCOMBINERPARAMETERIVNVPROC) (GLenum pname, GLint const * params);
typedef void (GLAPIENTRY * PFNGLCOMBINERSTAGEPARAMETERFVNVPROC) (GLenum stage, GLenum pname, GLfloat const * params);
typedef void (GLAPIENTRY * PFNGLCOMPILESHADERPROC) (GLuint shader);
typedef void (GLAPIENTRY * PFNGLCOMPILESHADERARBPROC) (GLhandleARB shaderObj);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void * bits);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void * bits);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void * bits);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXSUBIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void * bits);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXSUBIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void * bits);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXSUBIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void * bits);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXIMAGE1DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void * data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXIMAGE1DARBPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void * data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXIMAGE2DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void * data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXIMAGE2DARBPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void * data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXIMAGE3DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void * data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXIMAGE3DARBPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void * data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void * data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void * data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void * data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void * data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void * data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void * data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTUREIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void * bits);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTUREIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void * bits);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTUREIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void * bits);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTURESUBIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void * bits);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTURESUBIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void * bits);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTURESUBIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void * bits);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONFILTER1DPROC) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void * image);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONFILTER2DPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * image);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONPARAMETERFPROC) (GLenum target, GLenum pname, GLfloat params);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONPARAMETERFVPROC) (GLenum target, GLenum pname, GLfloat const * params);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONPARAMETERIPROC) (GLenum target, GLenum pname, GLint params);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONPARAMETERIVPROC) (GLenum target, GLenum pname, GLint const * params);
typedef void (GLAPIENTRY * PFNGLCOPYCOLORSUBTABLEPROC) (GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYCOLORTABLEPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYCONVOLUTIONFILTER1DPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYCONVOLUTIONFILTER2DPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLCOPYIMAGESUBDATAPROC) (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);
typedef void (GLAPIENTRY * PFNGLCOPYMULTITEXIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
typedef void (GLAPIENTRY * PFNGLCOPYMULTITEXIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void (GLAPIENTRY * PFNGLCOPYMULTITEXSUBIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYMULTITEXSUBIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLCOPYTEXSUBIMAGE3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLCOPYTEXSUBIMAGE3DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLCOPYTEXTUREIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
typedef void (GLAPIENTRY * PFNGLCOPYTEXTUREIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void (GLAPIENTRY * PFNGLCOPYTEXTURESUBIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYTEXTURESUBIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLCOPYTEXTURESUBIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef GLuint (GLAPIENTRY * PFNGLCREATEPROGRAMPROC) ();
typedef GLhandleARB (GLAPIENTRY * PFNGLCREATEPROGRAMOBJECTARBPROC) ();
typedef GLuint (GLAPIENTRY * PFNGLCREATESHADERPROC) (GLenum type);
typedef GLhandleARB (GLAPIENTRY * PFNGLCREATESHADEROBJECTARBPROC) (GLenum shaderType);
typedef void (GLAPIENTRY * PFNGLDELETEBUFFERSPROC) (GLsizei n, GLuint const * buffers);
typedef void (GLAPIENTRY * PFNGLDELETEFENCESNVPROC) (GLsizei n, GLuint const * fences);
typedef void (GLAPIENTRY * PFNGLDELETEFRAMEBUFFERSPROC) (GLsizei n, GLuint const * framebuffers);
typedef void (GLAPIENTRY * PFNGLDELETEOBJECTARBPROC) (GLhandleARB obj);
typedef void (GLAPIENTRY * PFNGLDELETEOCCLUSIONQUERIESNVPROC) (GLsizei n, GLuint const * ids);
typedef void (GLAPIENTRY * PFNGLDELETEPROGRAMPROC) (GLuint program);
typedef void (GLAPIENTRY * PFNGLDELETEPROGRAMSARBPROC) (GLsizei n, GLuint const * programs);
typedef void (GLAPIENTRY * PFNGLDELETEPROGRAMSNVPROC) (GLsizei n, GLuint const * programs);
typedef void (GLAPIENTRY * PFNGLDELETEQUERIESPROC) (GLsizei n, GLuint const * ids);
typedef void (GLAPIENTRY * PFNGLDELETEQUERIESARBPROC) (GLsizei n, GLuint const * ids);
typedef void (GLAPIENTRY * PFNGLDELETERENDERBUFFERSPROC) (GLsizei n, GLuint const * renderbuffers);
typedef void (GLAPIENTRY * PFNGLDELETESAMPLERSPROC) (GLsizei count, GLuint const * samplers);
typedef void (GLAPIENTRY * PFNGLDELETESHADERPROC) (GLuint shader);
typedef void (GLAPIENTRY * PFNGLDELETETEXTURESEXTPROC) (GLsizei n, GLuint const * textures);
typedef void (GLAPIENTRY * PFNGLDELETEVERTEXARRAYSPROC) (GLsizei n, GLuint const * arrays);
typedef void (GLAPIENTRY * PFNGLDEPTHBOUNDSDNVPROC) (GLdouble zmin, GLdouble zmax);
typedef void (GLAPIENTRY * PFNGLDEPTHBOUNDSEXTPROC) (GLclampd zmin, GLclampd zmax);
typedef void (GLAPIENTRY * PFNGLDEPTHRANGEDNVPROC) (GLdouble zNear, GLdouble zFar);
typedef void (GLAPIENTRY * PFNGLDETACHOBJECTARBPROC) (GLhandleARB containerObj, GLhandleARB attachedObj);
typedef void (GLAPIENTRY * PFNGLDETACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (GLAPIENTRY * PFNGLDISABLECLIENTSTATEINDEXEDEXTPROC) (GLenum array, GLuint index);
typedef void (GLAPIENTRY * PFNGLDISABLEIPROC) (GLenum target, GLuint index);
typedef void (GLAPIENTRY * PFNGLDISABLEINDEXEDEXTPROC) (GLenum target, GLuint index);
typedef void (GLAPIENTRY * PFNGLDISABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (GLAPIENTRY * PFNGLDRAWARRAYSEXTPROC) (GLenum mode, GLint first, GLsizei count);
typedef void (GLAPIENTRY * PFNGLDRAWARRAYSINSTANCEDARBPROC) (GLenum mode, GLint first, GLsizei count, GLsizei primcount);
typedef void (GLAPIENTRY * PFNGLDRAWARRAYSINSTANCEDEXTPROC) (GLenum mode, GLint start, GLsizei count, GLsizei primcount);
typedef void (GLAPIENTRY * PFNGLDRAWBUFFERSPROC) (GLsizei n, GLenum const * bufs);
typedef void (GLAPIENTRY * PFNGLDRAWBUFFERSARBPROC) (GLsizei n, GLenum const * bufs);
typedef void (GLAPIENTRY * PFNGLDRAWBUFFERSATIPROC) (GLsizei n, GLenum const * bufs);
typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSINSTANCEDARBPROC) (GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei primcount);
typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSINSTANCEDEXTPROC) (GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei primcount);
typedef void (GLAPIENTRY * PFNGLDRAWRANGEELEMENTSEXTPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void * indices);
typedef void (GLAPIENTRY * PFNGLEDGEFLAGPOINTEREXTPROC) (GLsizei stride, GLsizei count, GLboolean const * pointer);
typedef void (GLAPIENTRY * PFNGLENABLECLIENTSTATEINDEXEDEXTPROC) (GLenum array, GLuint index);
typedef void (GLAPIENTRY * PFNGLENABLEIPROC) (GLenum target, GLuint index);
typedef void (GLAPIENTRY * PFNGLENABLEINDEXEDEXTPROC) (GLenum target, GLuint index);
typedef void (GLAPIENTRY * PFNGLENDCONDITIONALRENDERPROC) ();
typedef void (GLAPIENTRY * PFNGLENDCONDITIONALRENDERNVPROC) ();
typedef void (GLAPIENTRY * PFNGLENDOCCLUSIONQUERYNVPROC) ();
typedef void (GLAPIENTRY * PFNGLENDQUERYPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLENDQUERYARBPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLENDTRANSFORMFEEDBACKPROC) ();
typedef void (GLAPIENTRY * PFNGLENDTRANSFORMFEEDBACKNVPROC) ();
typedef void (GLAPIENTRY * PFNGLEXECUTEPROGRAMNVPROC) (GLenum target, GLuint id, GLfloat const * params);
typedef void (GLAPIENTRY * PFNGLFINALCOMBINERINPUTNVPROC) (GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
typedef void (GLAPIENTRY * PFNGLFINISHFENCENVPROC) (GLuint fence);
typedef void (GLAPIENTRY * PFNGLFLUSHMAPPEDBUFFERRANGEPROC) (GLenum target, GLintptr offset, GLsizeiptr length);
typedef void (GLAPIENTRY * PFNGLFLUSHPIXELDATARANGENVPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLFLUSHVERTEXARRAYRANGENVPROC) ();
typedef void (GLAPIENTRY * PFNGLFOGCOORDDPROC) (GLdouble coord);
typedef void (GLAPIENTRY * PFNGLFOGCOORDDEXTPROC) (GLdouble coord);
typedef void (GLAPIENTRY * PFNGLFOGCOORDDVPROC) (GLdouble const * coord);
typedef void (GLAPIENTRY * PFNGLFOGCOORDDVEXTPROC) (GLdouble const * coord);
typedef void (GLAPIENTRY * PFNGLFOGCOORDFPROC) (GLfloat coord);
typedef void (GLAPIENTRY * PFNGLFOGCOORDFEXTPROC) (GLfloat coord);
typedef void (GLAPIENTRY * PFNGLFOGCOORDFVPROC) (GLfloat const * coord);
typedef void (GLAPIENTRY * PFNGLFOGCOORDFVEXTPROC) (GLfloat const * coord);
typedef void (GLAPIENTRY * PFNGLFOGCOORDHNVPROC) (GLhalfNV fog);
typedef void (GLAPIENTRY * PFNGLFOGCOORDHVNVPROC) (GLhalfNV const * fog);
typedef void (GLAPIENTRY * PFNGLFOGCOORDPOINTERPROC) (GLenum type, GLsizei stride, const void * pointer);
typedef void (GLAPIENTRY * PFNGLFOGCOORDPOINTEREXTPROC) (GLenum type, GLsizei stride, const void * pointer);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERDRAWBUFFEREXTPROC) (GLuint framebuffer, GLenum mode);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERDRAWBUFFERSEXTPROC) (GLuint framebuffer, GLsizei n, GLenum const * bufs);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERREADBUFFEREXTPROC) (GLuint framebuffer, GLenum mode);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERRENDERBUFFERPROC) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTUREPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURE1DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURE1DEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURE2DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURE3DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURE3DEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTUREARBPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTUREEXTPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTUREFACEARBPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTUREFACEEXTPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURELAYERPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURELAYERARBPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURELAYEREXTPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
typedef void (GLAPIENTRY * PFNGLGENBUFFERSPROC) (GLsizei n, GLuint * buffers);
typedef void (GLAPIENTRY * PFNGLGENBUFFERSARBPROC) (GLsizei n, GLuint * buffers);
typedef void (GLAPIENTRY * PFNGLGENERATEMIPMAPPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLGENERATEMIPMAPEXTPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLGENERATEMULTITEXMIPMAPEXTPROC) (GLenum texunit, GLenum target);
typedef void (GLAPIENTRY * PFNGLGENERATETEXTUREMIPMAPEXTPROC) (GLuint texture, GLenum target);
typedef void (GLAPIENTRY * PFNGLGENFENCESNVPROC) (GLsizei n, GLuint * fences);
typedef void (GLAPIENTRY * PFNGLGENFRAMEBUFFERSPROC) (GLsizei n, GLuint * framebuffers);
typedef void (GLAPIENTRY * PFNGLGENOCCLUSIONQUERIESNVPROC) (GLsizei n, GLuint * ids);
typedef void (GLAPIENTRY * PFNGLGENPROGRAMSARBPROC) (GLsizei n, GLuint * programs);
typedef void (GLAPIENTRY * PFNGLGENPROGRAMSNVPROC) (GLsizei n, GLuint * programs);
typedef void (GLAPIENTRY * PFNGLGENQUERIESPROC) (GLsizei n, GLuint * ids);
typedef void (GLAPIENTRY * PFNGLGENQUERIESARBPROC) (GLsizei n, GLuint * ids);
typedef void (GLAPIENTRY * PFNGLGENRENDERBUFFERSPROC) (GLsizei n, GLuint * renderbuffers);
typedef void (GLAPIENTRY * PFNGLGENSAMPLERSPROC) (GLsizei count, GLuint * samplers);
typedef void (GLAPIENTRY * PFNGLGENTEXTURESEXTPROC) (GLsizei n, GLuint * textures);
typedef void (GLAPIENTRY * PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint * arrays);
typedef void (GLAPIENTRY * PFNGLGETACTIVEATTRIBPROC) (GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name);
typedef void (GLAPIENTRY * PFNGLGETACTIVEATTRIBARBPROC) (GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei * length, GLint * size, GLenum * type, GLcharARB * name);
typedef void (GLAPIENTRY * PFNGLGETACTIVEUNIFORMPROC) (GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name);
typedef void (GLAPIENTRY * PFNGLGETACTIVEUNIFORMARBPROC) (GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei * length, GLint * size, GLenum * type, GLcharARB * name);
typedef void (GLAPIENTRY * PFNGLGETACTIVEVARYINGNVPROC) (GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLsizei * size, GLenum * type, GLchar * name);
typedef void (GLAPIENTRY * PFNGLGETATTACHEDOBJECTSARBPROC) (GLhandleARB containerObj, GLsizei maxCount, GLsizei * count, GLhandleARB * obj);
typedef void (GLAPIENTRY * PFNGLGETATTACHEDSHADERSPROC) (GLuint program, GLsizei maxCount, GLsizei * count, GLuint * shaders);
typedef GLint (GLAPIENTRY * PFNGLGETATTRIBLOCATIONPROC) (GLuint program, GLchar const * name);
typedef GLint (GLAPIENTRY * PFNGLGETATTRIBLOCATIONARBPROC) (GLhandleARB programObj, GLcharARB const * name);
typedef void (GLAPIENTRY * PFNGLGETBOOLEANI_VPROC) (GLenum target, GLuint index, GLboolean * data);
typedef void (GLAPIENTRY * PFNGLGETBOOLEANINDEXEDVEXTPROC) (GLenum target, GLuint index, GLboolean * data);
typedef void (GLAPIENTRY * PFNGLGETBUFFERPARAMETERIVPROC) (GLenum target, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETBUFFERPARAMETERIVARBPROC) (GLenum target, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETBUFFERPOINTERVPROC) (GLenum target, GLenum pname, void ** params);
typedef void (GLAPIENTRY * PFNGLGETBUFFERPOINTERVARBPROC) (GLenum target, GLenum pname, void ** params);
typedef void (GLAPIENTRY * PFNGLGETBUFFERSUBDATAPROC) (GLenum target, GLintptr offset, GLsizeiptr size, void * data);
typedef void (GLAPIENTRY * PFNGLGETBUFFERSUBDATAARBPROC) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, void * data);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLEPROC) (GLenum target, GLenum format, GLenum type, void * table);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLEEXTPROC) (GLenum target, GLenum format, GLenum type, void * data);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLEPARAMETERFVPROC) (GLenum target, GLenum pname, GLfloat * params);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLEPARAMETERFVEXTPROC) (GLenum target, GLenum pname, GLfloat * params);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLEPARAMETERIVPROC) (GLenum target, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLEPARAMETERIVEXTPROC) (GLenum target, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETCOMBINERINPUTPARAMETERFVNVPROC) (GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLfloat * params);
typedef void (GLAPIENTRY * PFNGLGETCOMBINERINPUTPARAMETERIVNVPROC) (GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETCOMBINEROUTPUTPARAMETERFVNVPROC) (GLenum stage, GLenum portion, GLenum pname, GLfloat * params);
typedef void (GLAPIENTRY * PFNGLGETCOMBINEROUTPUTPARAMETERIVNVPROC) (GLenum stage, GLenum portion, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETCOMBINERSTAGEPARAMETERFVNVPROC) (GLenum stage, GLenum pname, GLfloat * params);
typedef void (GLAPIENTRY * PFNGLGETCOMPRESSEDMULTITEXIMAGEEXTPROC) (GLenum texunit, GLenum target, GLint lod, void * img);
typedef void (GLAPIENTRY * PFNGLGETCOMPRESSEDTEXIMAGEPROC) (GLenum target, GLint level, void * img);
typedef void (GLAPIENTRY * PFNGLGETCOMPRESSEDTEXIMAGEARBPROC) (GLenum target, GLint level, void * img);
typedef void (GLAPIENTRY * PFNGLGETCOMPRESSEDTEXTUREIMAGEEXTPROC) (GLuint texture, GLenum target, GLint lod, void * img);
typedef void (GLAPIENTRY * PFNGLGETCONVOLUTIONFILTERPROC) (GLenum target, GLenum format, GLenum type, void * image);
typedef void (GLAPIENTRY * PFNGLGETCONVOLUTIONPARAMETERFVPROC) (GLenum target, GLenum pname, GLfloat * params);
typedef void (GLAPIENTRY * PFNGLGETCONVOLUTIONPARAMETERIVPROC) (GLenum target, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETDOUBLEINDEXEDVEXTPROC) (GLenum target, GLuint index, GLdouble * data);
typedef void (GLAPIENTRY * PFNGLGETFENCEIVNVPROC) (GLuint fence, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETFINALCOMBINERINPUTPARAMETERFVNVPROC) (GLenum variable, GLenum pname, GLfloat * params);
typedef void (GLAPIENTRY * PFNGLGETFINALCOMBINERINPUTPARAMETERIVNVPROC) (GLenum variable, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETFLOATINDEXEDVEXTPROC) (GLenum target, GLuint index, GLfloat * data);
typedef GLint (GLAPIENTRY * PFNGLGETFRAGDATALOCATIONPROC) (GLuint program, GLchar const * name);
typedef GLint (GLAPIENTRY * PFNGLGETFRAGDATALOCATIONEXTPROC) (GLuint program, GLchar const * name);
typedef void (GLAPIENTRY * PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) (GLenum target, GLenum attachment, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC) (GLenum target, GLenum attachment, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETFRAMEBUFFERPARAMETERIVEXTPROC) (GLuint framebuffer, GLenum pname, GLint * params);
typedef GLhandleARB (GLAPIENTRY * PFNGLGETHANDLEARBPROC) (GLenum pname);
typedef void (GLAPIENTRY * PFNGLGETHISTOGRAMPROC) (GLenum target, GLboolean reset, GLenum format, GLenum type, void * values);
typedef void (GLAPIENTRY * PFNGLGETHISTOGRAMPARAMETERFVPROC) (GLenum target, GLenum pname, GLfloat * params);
typedef void (GLAPIENTRY * PFNGLGETHISTOGRAMPARAMETERIVPROC) (GLenum target, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETINFOLOGARBPROC) (GLhandleARB obj, GLsizei maxLength, GLsizei * length, GLcharARB * infoLog);
typedef void (GLAPIENTRY * PFNGLGETINTEGERI_VPROC) (GLenum target, GLuint index, GLint * data);
typedef void (GLAPIENTRY * PFNGLGETINTEGERINDEXEDVEXTPROC) (GLenum target, GLuint index, GLint * data);
typedef void (GLAPIENTRY * PFNGLGETMINMAXPROC) (GLenum target, GLboolean reset, GLenum format, GLenum type, void * values);
typedef void (GLAPIENTRY * PFNGLGETMINMAXPARAMETERFVPROC) (GLenum target, GLenum pname, GLfloat * params);
typedef void (GLAPIENTRY * PFNGLGETMINMAXPARAMETERIVPROC) (GLenum target, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETMULTISAMPLEFVNVPROC) (GLenum pname, GLuint index, GLfloat * val);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXENVFVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat * params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXENVIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXGENDVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLdouble * params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXGENFVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLfloat * params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXGENIVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXIMAGEEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum format, GLenum type, void * pixels);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXLEVELPARAMETERFVEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum pname, GLfloat * params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXLEVELPARAMETERIVEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXPARAMETERFVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat * params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXPARAMETERIIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXPARAMETERIUIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLuint * params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXPARAMETERIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDBUFFERPARAMETERIVEXTPROC) (GLuint buffer, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDBUFFERPOINTERVEXTPROC) (GLuint buffer, GLenum pname, void ** params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDBUFFERSUBDATAEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr size, void * data);
typedef void (GLAPIENTRY * PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC) (GLuint framebuffer, GLenum attachment, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMIVEXTPROC) (GLuint program, GLenum target, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMLOCALPARAMETERDVEXTPROC) (GLuint program, GLenum target, GLuint index, GLdouble * params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMLOCALPARAMETERFVEXTPROC) (GLuint program, GLenum target, GLuint index, GLfloat * params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMLOCALPARAMETERIIVEXTPROC) (GLuint program, GLenum target, GLuint index, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMLOCALPARAMETERIUIVEXTPROC) (GLuint program, GLenum target, GLuint index, GLuint * params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMSTRINGEXTPROC) (GLuint program, GLenum target, GLenum pname, void * string);
typedef void (GLAPIENTRY * PFNGLGETNAMEDRENDERBUFFERPARAMETERIVEXTPROC) (GLuint renderbuffer, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETOBJECTPARAMETERFVARBPROC) (GLhandleARB obj, GLenum pname, GLfloat * params);
typedef void (GLAPIENTRY * PFNGLGETOBJECTPARAMETERIVARBPROC) (GLhandleARB obj, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETOCCLUSIONQUERYIVNVPROC) (GLuint id, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETOCCLUSIONQUERYUIVNVPROC) (GLuint id, GLenum pname, GLuint * params);
typedef void (GLAPIENTRY * PFNGLGETPOINTERINDEXEDVEXTPROC) (GLenum target, GLuint index, void ** data);
typedef void (GLAPIENTRY * PFNGLGETPOINTERVEXTPROC) (GLenum pname, void ** params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMENVPARAMETERDVARBPROC) (GLenum target, GLuint index, GLdouble * params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMENVPARAMETERIIVNVPROC) (GLenum target, GLuint index, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMENVPARAMETERIUIVNVPROC) (GLenum target, GLuint index, GLuint * params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMIVNVPROC) (GLuint id, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC) (GLenum target, GLuint index, GLdouble * params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC) (GLenum target, GLuint index, GLfloat * params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMLOCALPARAMETERIIVNVPROC) (GLenum target, GLuint index, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMLOCALPARAMETERIUIVNVPROC) (GLenum target, GLuint index, GLuint * params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMNAMEDPARAMETERDVNVPROC) (GLuint id, GLsizei len, GLubyte const * name, GLdouble * params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMNAMEDPARAMETERFVNVPROC) (GLuint id, GLsizei len, GLubyte const * name, GLfloat * params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMPARAMETERDVNVPROC) (GLenum target, GLuint index, GLenum pname, GLdouble * params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMPARAMETERFVNVPROC) (GLenum target, GLuint index, GLenum pname, GLfloat * params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMSTRINGARBPROC) (GLenum target, GLenum pname, void * string);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMSTRINGNVPROC) (GLuint id, GLenum pname, GLubyte * program);
typedef void (GLAPIENTRY * PFNGLGETQUERYIVPROC) (GLenum target, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETQUERYIVARBPROC) (GLenum target, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTI64VEXTPROC) (GLuint id, GLenum pname, GLint64 * params);
typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTIVPROC) (GLuint id, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTIVARBPROC) (GLuint id, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTUI64VPROC) (GLuint id, GLenum pname, GLuint64 * params);
typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTUI64VEXTPROC) (GLuint id, GLenum pname, GLuint64 * params);
typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTUIVPROC) (GLuint id, GLenum pname, GLuint * params);
typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTUIVARBPROC) (GLuint id, GLenum pname, GLuint * params);
typedef void (GLAPIENTRY * PFNGLGETRENDERBUFFERPARAMETERIVPROC) (GLenum target, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC) (GLenum target, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETSEPARABLEFILTERPROC) (GLenum target, GLenum format, GLenum type, void * row, void * column, void * span);
typedef void (GLAPIENTRY * PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog);
typedef void (GLAPIENTRY * PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETSHADERSOURCEPROC) (GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * source);
typedef void (GLAPIENTRY * PFNGLGETSHADERSOURCEARBPROC) (GLhandleARB obj, GLsizei maxLength, GLsizei * length, GLcharARB * source);
typedef GLubyte const *  (GLAPIENTRY * PFNGLGETSTRINGIPROC) (GLenum name, GLuint index);
typedef void (GLAPIENTRY * PFNGLGETTEXPARAMETERIIVPROC) (GLenum target, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETTEXPARAMETERIIVEXTPROC) (GLenum target, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETTEXPARAMETERIUIVPROC) (GLenum target, GLenum pname, GLuint * params);
typedef void (GLAPIENTRY * PFNGLGETTEXPARAMETERIUIVEXTPROC) (GLenum target, GLenum pname, GLuint * params);
typedef void (GLAPIENTRY * PFNGLGETTEXTURELEVELPARAMETERFVEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum pname, GLfloat * params);
typedef void (GLAPIENTRY * PFNGLGETTEXTURELEVELPARAMETERIVEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETTEXTUREPARAMETERFVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLfloat * params);
typedef void (GLAPIENTRY * PFNGLGETTEXTUREPARAMETERIIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETTEXTUREPARAMETERIUIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLuint * params);
typedef void (GLAPIENTRY * PFNGLGETTEXTUREPARAMETERIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETTRACKMATRIXIVNVPROC) (GLenum target, GLuint address, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETTRANSFORMFEEDBACKVARYINGPROC) (GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLsizei * size, GLenum * type, GLchar * name);
typedef void (GLAPIENTRY * PFNGLGETTRANSFORMFEEDBACKVARYINGNVPROC) (GLuint program, GLuint index, GLint * location);
typedef GLint (GLAPIENTRY * PFNGLGETUNIFORMBUFFERSIZEEXTPROC) (GLuint program, GLint location);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMFVPROC) (GLuint program, GLint location, GLfloat * params);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMFVARBPROC) (GLhandleARB programObj, GLint location, GLfloat * params);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMIVPROC) (GLuint program, GLint location, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMIVARBPROC) (GLhandleARB programObj, GLint location, GLint * params);
typedef GLint (GLAPIENTRY * PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, GLchar const * name);
typedef GLint (GLAPIENTRY * PFNGLGETUNIFORMLOCATIONARBPROC) (GLhandleARB programObj, GLcharARB const * name);
typedef GLintptr (GLAPIENTRY * PFNGLGETUNIFORMOFFSETEXTPROC) (GLuint program, GLint location);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMUIVPROC) (GLuint program, GLint location, GLuint * params);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMUIVEXTPROC) (GLuint program, GLint location, GLuint * params);
typedef GLint (GLAPIENTRY * PFNGLGETVARYINGLOCATIONNVPROC) (GLuint program, GLchar const * name);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBDVPROC) (GLuint index, GLenum pname, GLdouble * params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBDVARBPROC) (GLuint index, GLenum pname, GLdouble * params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBDVNVPROC) (GLuint index, GLenum pname, GLdouble * params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBFVPROC) (GLuint index, GLenum pname, GLfloat * params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBFVARBPROC) (GLuint index, GLenum pname, GLfloat * params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBFVNVPROC) (GLuint index, GLenum pname, GLfloat * params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBIIVPROC) (GLuint index, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBIIVEXTPROC) (GLuint index, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBIUIVPROC) (GLuint index, GLenum pname, GLuint * params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBIUIVEXTPROC) (GLuint index, GLenum pname, GLuint * params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBIVPROC) (GLuint index, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBIVARBPROC) (GLuint index, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBIVNVPROC) (GLuint index, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBPOINTERVPROC) (GLuint index, GLenum pname, void ** pointer);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBPOINTERVARBPROC) (GLuint index, GLenum pname, void ** pointer);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBPOINTERVNVPROC) (GLuint index, GLenum pname, void ** pointer);
typedef void (GLAPIENTRY * PFNGLGETVIDEOI64VNVPROC) (GLuint video_slot, GLenum pname, GLint64EXT * params);
typedef void (GLAPIENTRY * PFNGLGETVIDEOIVNVPROC) (GLuint video_slot, GLenum pname, GLint * params);
typedef void (GLAPIENTRY * PFNGLGETVIDEOUI64VNVPROC) (GLuint video_slot, GLenum pname, GLuint64EXT * params);
typedef void (GLAPIENTRY * PFNGLGETVIDEOUIVNVPROC) (GLuint video_slot, GLenum pname, GLuint * params);
typedef void (GLAPIENTRY * PFNGLHISTOGRAMPROC) (GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
typedef void (GLAPIENTRY * PFNGLINDEXPOINTEREXTPROC) (GLenum type, GLsizei stride, GLsizei count, const void * pointer);
typedef GLboolean (GLAPIENTRY * PFNGLISBUFFERPROC) (GLuint buffer);
typedef GLboolean (GLAPIENTRY * PFNGLISENABLEDIPROC) (GLenum target, GLuint index);
typedef GLboolean (GLAPIENTRY * PFNGLISENABLEDINDEXEDEXTPROC) (GLenum target, GLuint index);
typedef GLboolean (GLAPIENTRY * PFNGLISFENCENVPROC) (GLuint fence);
typedef GLboolean (GLAPIENTRY * PFNGLISFRAMEBUFFERPROC) (GLuint framebuffer);
typedef GLboolean (GLAPIENTRY * PFNGLISFRAMEBUFFEREXTPROC) (GLuint framebuffer);
typedef GLboolean (GLAPIENTRY * PFNGLISOCCLUSIONQUERYNVPROC) (GLuint id);
typedef GLboolean (GLAPIENTRY * PFNGLISPROGRAMPROC) (GLuint program);
typedef GLboolean (GLAPIENTRY * PFNGLISPROGRAMARBPROC) (GLuint program);
typedef GLboolean (GLAPIENTRY * PFNGLISPROGRAMNVPROC) (GLuint id);
typedef GLboolean (GLAPIENTRY * PFNGLISQUERYPROC) (GLuint id);
typedef GLboolean (GLAPIENTRY * PFNGLISQUERYARBPROC) (GLuint id);
typedef GLboolean (GLAPIENTRY * PFNGLISRENDERBUFFERPROC) (GLuint renderbuffer);
typedef GLboolean (GLAPIENTRY * PFNGLISRENDERBUFFEREXTPROC) (GLuint renderbuffer);
typedef GLboolean (GLAPIENTRY * PFNGLISSHADERPROC) (GLuint shader);
typedef GLboolean (GLAPIENTRY * PFNGLISTEXTUREEXTPROC) (GLuint texture);
typedef GLboolean (GLAPIENTRY * PFNGLISVERTEXARRAYPROC) (GLuint array);
typedef void (GLAPIENTRY * PFNGLLINKPROGRAMPROC) (GLuint program);
typedef void (GLAPIENTRY * PFNGLLINKPROGRAMARBPROC) (GLhandleARB programObj);
typedef void (GLAPIENTRY * PFNGLLOADPROGRAMNVPROC) (GLenum target, GLuint id, GLsizei len, GLubyte const * program);
typedef void (GLAPIENTRY * PFNGLLOADTRANSPOSEMATRIXDPROC) (GLdouble const * m);
typedef void (GLAPIENTRY * PFNGLLOADTRANSPOSEMATRIXDARBPROC) (GLdouble const * m);
typedef void (GLAPIENTRY * PFNGLLOADTRANSPOSEMATRIXFARBPROC) (GLfloat const * m);
typedef void (GLAPIENTRY * PFNGLLOCKARRAYSEXTPROC) (GLint first, GLsizei count);
typedef void * (GLAPIENTRY * PFNGLMAPBUFFERPROC) (GLenum target, GLenum access);
typedef void * (GLAPIENTRY * PFNGLMAPBUFFERARBPROC) (GLenum target, GLenum access);
typedef void * (GLAPIENTRY * PFNGLMAPBUFFERRANGEPROC) (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef void (GLAPIENTRY * PFNGLMATRIXFRUSTUMEXTPROC) (GLenum mode, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
typedef void (GLAPIENTRY * PFNGLMATRIXLOADDEXTPROC) (GLenum mode, GLdouble const * m);
typedef void (GLAPIENTRY * PFNGLMATRIXLOADFEXTPROC) (GLenum mode, GLfloat const * m);
typedef void (GLAPIENTRY * PFNGLMATRIXLOADIDENTITYEXTPROC) (GLenum mode);
typedef void (GLAPIENTRY * PFNGLMATRIXLOADTRANSPOSEDEXTPROC) (GLenum mode, GLdouble const * m);
typedef void (GLAPIENTRY * PFNGLMATRIXLOADTRANSPOSEFEXTPROC) (GLenum mode, GLfloat const * m);
typedef void (GLAPIENTRY * PFNGLMATRIXMULTDEXTPROC) (GLenum mode, GLdouble const * m);
typedef void (GLAPIENTRY * PFNGLMATRIXMULTFEXTPROC) (GLenum mode, GLfloat const * m);
typedef void (GLAPIENTRY * PFNGLMATRIXMULTTRANSPOSEDEXTPROC) (GLenum mode, GLdouble const * m);
typedef void (GLAPIENTRY * PFNGLMATRIXMULTTRANSPOSEFEXTPROC) (GLenum mode, GLfloat const * m);
typedef void (GLAPIENTRY * PFNGLMATRIXORTHOEXTPROC) (GLenum mode, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
typedef void (GLAPIENTRY * PFNGLMATRIXPOPEXTPROC) (GLenum mode);
typedef void (GLAPIENTRY * PFNGLMATRIXPUSHEXTPROC) (GLenum mode);
typedef void (GLAPIENTRY * PFNGLMATRIXROTATEDEXTPROC) (GLenum mode, GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLMATRIXROTATEFEXTPROC) (GLenum mode, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLMATRIXSCALEDEXTPROC) (GLenum mode, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLMATRIXSCALEFEXTPROC) (GLenum mode, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLMATRIXTRANSLATEDEXTPROC) (GLenum mode, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLMATRIXTRANSLATEFEXTPROC) (GLenum mode, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLMINMAXPROC) (GLenum target, GLenum internalformat, GLboolean sink);
typedef void (GLAPIENTRY * PFNGLMULTIDRAWARRAYSPROC) (GLenum mode, GLint const * first, GLsizei const * count, GLsizei drawcount);
typedef void (GLAPIENTRY * PFNGLMULTIDRAWARRAYSEXTPROC) (GLenum mode, GLint const * first, GLsizei const * count, GLsizei primcount);
typedef void (GLAPIENTRY * PFNGLMULTIDRAWELEMENTSPROC) (GLenum mode, GLsizei const * count, GLenum type, const void *const* indices, GLsizei drawcount);
typedef void (GLAPIENTRY * PFNGLMULTIDRAWELEMENTSEXTPROC) (GLenum mode, GLsizei const * count, GLenum type, const void *const* indices, GLsizei primcount);
typedef void (GLAPIENTRY * PFNGLMULTITEXBUFFEREXTPROC) (GLenum texunit, GLenum target, GLenum internalformat, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1DPROC) (GLenum target, GLdouble s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1DARBPROC) (GLenum target, GLdouble s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1DVPROC) (GLenum target, GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1DVARBPROC) (GLenum target, GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1FPROC) (GLenum target, GLfloat s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1FARBPROC) (GLenum target, GLfloat s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1FVPROC) (GLenum target, GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1FVARBPROC) (GLenum target, GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1HNVPROC) (GLenum target, GLhalfNV s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1HVNVPROC) (GLenum target, GLhalfNV const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1IPROC) (GLenum target, GLint s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1IARBPROC) (GLenum target, GLint s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1IVPROC) (GLenum target, GLint const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1IVARBPROC) (GLenum target, GLint const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1SPROC) (GLenum target, GLshort s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1SARBPROC) (GLenum target, GLshort s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1SVPROC) (GLenum target, GLshort const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1SVARBPROC) (GLenum target, GLshort const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2DPROC) (GLenum target, GLdouble s, GLdouble t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2DARBPROC) (GLenum target, GLdouble s, GLdouble t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2DVPROC) (GLenum target, GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2DVARBPROC) (GLenum target, GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2FARBPROC) (GLenum target, GLfloat s, GLfloat t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2FVARBPROC) (GLenum target, GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2HNVPROC) (GLenum target, GLhalfNV s, GLhalfNV t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2HVNVPROC) (GLenum target, GLhalfNV const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2IPROC) (GLenum target, GLint s, GLint t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2IARBPROC) (GLenum target, GLint s, GLint t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2IVPROC) (GLenum target, GLint const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2IVARBPROC) (GLenum target, GLint const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2SPROC) (GLenum target, GLshort s, GLshort t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2SARBPROC) (GLenum target, GLshort s, GLshort t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2SVPROC) (GLenum target, GLshort const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2SVARBPROC) (GLenum target, GLshort const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3DPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3DARBPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3DVPROC) (GLenum target, GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3DVARBPROC) (GLenum target, GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3FPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3FARBPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3FVPROC) (GLenum target, GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3FVARBPROC) (GLenum target, GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3HNVPROC) (GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3HVNVPROC) (GLenum target, GLhalfNV const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3IPROC) (GLenum target, GLint s, GLint t, GLint r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3IARBPROC) (GLenum target, GLint s, GLint t, GLint r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3IVPROC) (GLenum target, GLint const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3IVARBPROC) (GLenum target, GLint const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3SPROC) (GLenum target, GLshort s, GLshort t, GLshort r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3SARBPROC) (GLenum target, GLshort s, GLshort t, GLshort r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3SVPROC) (GLenum target, GLshort const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3SVARBPROC) (GLenum target, GLshort const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4DPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4DARBPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4DVPROC) (GLenum target, GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4DVARBPROC) (GLenum target, GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4FPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4FARBPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4FVPROC) (GLenum target, GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4FVARBPROC) (GLenum target, GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4HNVPROC) (GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4HVNVPROC) (GLenum target, GLhalfNV const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4IPROC) (GLenum target, GLint s, GLint t, GLint r, GLint q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4IARBPROC) (GLenum target, GLint s, GLint t, GLint r, GLint q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4IVPROC) (GLenum target, GLint const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4IVARBPROC) (GLenum target, GLint const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4SPROC) (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4SARBPROC) (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4SVPROC) (GLenum target, GLshort const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4SVARBPROC) (GLenum target, GLshort const * v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORDPOINTEREXTPROC) (GLenum texunit, GLint size, GLenum type, GLsizei stride, const void * pointer);
typedef void (GLAPIENTRY * PFNGLMULTITEXENVFEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLMULTITEXENVFVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat const * params);
typedef void (GLAPIENTRY * PFNGLMULTITEXENVIEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLMULTITEXENVIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint const * params);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENDEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLdouble param);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENDVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLdouble const * params);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENFEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENFVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLfloat const * params);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENIEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENIVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLint const * params);
typedef void (GLAPIENTRY * PFNGLMULTITEXIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void * pixels);
typedef void (GLAPIENTRY * PFNGLMULTITEXIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void * pixels);
typedef void (GLAPIENTRY * PFNGLMULTITEXIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void * pixels);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERFEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERFVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat const * params);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERIEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERIIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint const * params);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERIUIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLuint const * params);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint const * params);
typedef void (GLAPIENTRY * PFNGLMULTITEXRENDERBUFFEREXTPROC) (GLenum texunit, GLenum target, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLMULTITEXSUBIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void * pixels);
typedef void (GLAPIENTRY * PFNGLMULTITEXSUBIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * pixels);
typedef void (GLAPIENTRY * PFNGLMULTITEXSUBIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void * pixels);
typedef void (GLAPIENTRY * PFNGLMULTTRANSPOSEMATRIXDPROC) (GLdouble const * m);
typedef void (GLAPIENTRY * PFNGLMULTTRANSPOSEMATRIXDARBPROC) (GLdouble const * m);
typedef void (GLAPIENTRY * PFNGLMULTTRANSPOSEMATRIXFARBPROC) (GLfloat const * m);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERRENDERBUFFEREXTPROC) (GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTURE1DEXTPROC) (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTURE2DEXTPROC) (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTURE3DEXTPROC) (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTUREEXTPROC) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTUREFACEEXTPROC) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLenum face);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTURELAYEREXTPROC) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETER4DEXTPROC) (GLuint program, GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETER4DVEXTPROC) (GLuint program, GLenum target, GLuint index, GLdouble const * params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETER4FEXTPROC) (GLuint program, GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETER4FVEXTPROC) (GLuint program, GLenum target, GLuint index, GLfloat const * params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERI4IEXTPROC) (GLuint program, GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERI4IVEXTPROC) (GLuint program, GLenum target, GLuint index, GLint const * params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIEXTPROC) (GLuint program, GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIVEXTPROC) (GLuint program, GLenum target, GLuint index, GLuint const * params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERS4FVEXTPROC) (GLuint program, GLenum target, GLuint index, GLsizei count, GLfloat const * params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERSI4IVEXTPROC) (GLuint program, GLenum target, GLuint index, GLsizei count, GLint const * params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERSI4UIVEXTPROC) (GLuint program, GLenum target, GLuint index, GLsizei count, GLuint const * params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMSTRINGEXTPROC) (GLuint program, GLenum target, GLenum format, GLsizei len, const void * string);
typedef void (GLAPIENTRY * PFNGLNAMEDRENDERBUFFERSTORAGEEXTPROC) (GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLECOVERAGEEXTPROC) (GLuint renderbuffer, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC) (GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLNORMAL3HNVPROC) (GLhalfNV nx, GLhalfNV ny, GLhalfNV nz);
typedef void (GLAPIENTRY * PFNGLNORMAL3HVNVPROC) (GLhalfNV const * v);
typedef void (GLAPIENTRY * PFNGLNORMALPOINTEREXTPROC) (GLenum type, GLsizei stride, GLsizei count, const void * pointer);
typedef void (GLAPIENTRY * PFNGLPIXELDATARANGENVPROC) (GLenum target, GLsizei length, const void * pointer);
typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERFARBPROC) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERFEXTPROC) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERFVARBPROC) (GLenum pname, GLfloat const * params);
typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERFVEXTPROC) (GLenum pname, GLfloat const * params);
typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERINVPROC) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERIVPROC) (GLenum pname, GLint const * params);
typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERIVNVPROC) (GLenum pname, GLint const * params);
typedef void (GLAPIENTRY * PFNGLPRESENTFRAMEDUALFILLNVPROC) (GLuint video_slot, GLuint64EXT minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum type, GLenum target0, GLuint fill0, GLenum target1, GLuint fill1, GLenum target2, GLuint fill2, GLenum target3, GLuint fill3);
typedef void (GLAPIENTRY * PFNGLPRESENTFRAMEKEYEDNVPROC) (GLuint video_slot, GLuint64EXT minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum type, GLenum target0, GLuint fill0, GLuint key0, GLenum target1, GLuint fill1, GLuint key1);
typedef void (GLAPIENTRY * PFNGLPRIMITIVERESTARTNVPROC) ();
typedef void (GLAPIENTRY * PFNGLPRIORITIZETEXTURESEXTPROC) (GLsizei n, GLuint const * textures, GLclampf const * priorities);
typedef void (GLAPIENTRY * PFNGLPROGRAMBUFFERPARAMETERSIIVNVPROC) (GLenum target, GLuint bindingIndex, GLuint wordIndex, GLsizei count, GLint const * params);
typedef void (GLAPIENTRY * PFNGLPROGRAMBUFFERPARAMETERSIUIVNVPROC) (GLenum target, GLuint bindingIndex, GLuint wordIndex, GLsizei count, GLuint const * params);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETER4DARBPROC) (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETER4DVARBPROC) (GLenum target, GLuint index, GLdouble const * params);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETERI4INVPROC) (GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETERI4IVNVPROC) (GLenum target, GLuint index, GLint const * params);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETERI4UINVPROC) (GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETERI4UIVNVPROC) (GLenum target, GLuint index, GLuint const * params);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETERSI4IVNVPROC) (GLenum target, GLuint index, GLsizei count, GLint const * params);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETERSI4UIVNVPROC) (GLenum target, GLuint index, GLsizei count, GLuint const * params);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETER4DARBPROC) (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETER4DVARBPROC) (GLenum target, GLuint index, GLdouble const * params);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETERI4INVPROC) (GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETERI4IVNVPROC) (GLenum target, GLuint index, GLint const * params);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETERI4UINVPROC) (GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETERI4UIVNVPROC) (GLenum target, GLuint index, GLuint const * params);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETERSI4IVNVPROC) (GLenum target, GLuint index, GLsizei count, GLint const * params);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETERSI4UIVNVPROC) (GLenum target, GLuint index, GLsizei count, GLuint const * params);
typedef void (GLAPIENTRY * PFNGLPROGRAMNAMEDPARAMETER4DNVPROC) (GLuint id, GLsizei len, GLubyte const * name, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLPROGRAMNAMEDPARAMETER4DVNVPROC) (GLuint id, GLsizei len, GLubyte const * name, GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLPROGRAMNAMEDPARAMETER4FNVPROC) (GLuint id, GLsizei len, GLubyte const * name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLPROGRAMNAMEDPARAMETER4FVNVPROC) (GLuint id, GLsizei len, GLubyte const * name, GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLPROGRAMPARAMETER4DNVPROC) (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLPROGRAMPARAMETER4DVNVPROC) (GLenum target, GLuint index, GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLPROGRAMPARAMETER4FNVPROC) (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLPROGRAMPARAMETER4FVNVPROC) (GLenum target, GLuint index, GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLPROGRAMPARAMETERIARBPROC) (GLuint program, GLenum pname, GLint value);
typedef void (GLAPIENTRY * PFNGLPROGRAMPARAMETERIEXTPROC) (GLuint program, GLenum pname, GLint value);
typedef void (GLAPIENTRY * PFNGLPROGRAMPARAMETERS4DVNVPROC) (GLenum target, GLuint index, GLsizei count, GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLPROGRAMPARAMETERS4FVNVPROC) (GLenum target, GLuint index, GLsizei count, GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLPROGRAMSTRINGARBPROC) (GLenum target, GLenum format, GLsizei len, const void * string);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1FEXTPROC) (GLuint program, GLint location, GLfloat v0);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1IEXTPROC) (GLuint program, GLint location, GLint v0);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1IVEXTPROC) (GLuint program, GLint location, GLsizei count, GLint const * value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1UIEXTPROC) (GLuint program, GLint location, GLuint v0);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1UIVEXTPROC) (GLuint program, GLint location, GLsizei count, GLuint const * value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2FEXTPROC) (GLuint program, GLint location, GLfloat v0, GLfloat v1);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2IEXTPROC) (GLuint program, GLint location, GLint v0, GLint v1);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2IVEXTPROC) (GLuint program, GLint location, GLsizei count, GLint const * value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2UIEXTPROC) (GLuint program, GLint location, GLuint v0, GLuint v1);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2UIVEXTPROC) (GLuint program, GLint location, GLsizei count, GLuint const * value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3FEXTPROC) (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3IEXTPROC) (GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3IVEXTPROC) (GLuint program, GLint location, GLsizei count, GLint const * value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3UIEXTPROC) (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3UIVEXTPROC) (GLuint program, GLint location, GLsizei count, GLuint const * value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4FEXTPROC) (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4IEXTPROC) (GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4IVEXTPROC) (GLuint program, GLint location, GLsizei count, GLint const * value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4UIEXTPROC) (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4UIVEXTPROC) (GLuint program, GLint location, GLsizei count, GLuint const * value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2X3FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2X4FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3X2FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3X4FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4X2FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4X3FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLPROGRAMVERTEXLIMITNVPROC) (GLenum target, GLint limit);
typedef void (GLAPIENTRY * PFNGLPUSHCLIENTATTRIBDEFAULTEXTPROC) (GLbitfield mask);
typedef void (GLAPIENTRY * PFNGLQUERYCOUNTERPROC) (GLuint id, GLenum target);
typedef void (GLAPIENTRY * PFNGLRENDERBUFFERSTORAGEPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLREQUESTRESIDENTPROGRAMSNVPROC) (GLsizei n, GLuint const * programs);
typedef void (GLAPIENTRY * PFNGLRESETHISTOGRAMPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLRESETMINMAXPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLSAMPLECOVERAGEPROC) (GLfloat value, GLboolean invert);
typedef void (GLAPIENTRY * PFNGLSAMPLECOVERAGEARBPROC) (GLfloat value, GLboolean invert);
typedef void (GLAPIENTRY * PFNGLSAMPLEMASKINDEXEDNVPROC) (GLuint index, GLbitfield mask);
typedef void (GLAPIENTRY * PFNGLSAMPLERPARAMETERFPROC) (GLuint sampler, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLSAMPLERPARAMETERFVPROC) (GLuint sampler, GLenum pname, GLfloat const * param);
typedef void (GLAPIENTRY * PFNGLSAMPLERPARAMETERIPROC) (GLuint sampler, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3BPROC) (GLbyte red, GLbyte green, GLbyte blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3BEXTPROC) (GLbyte red, GLbyte green, GLbyte blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3BVPROC) (GLbyte const * v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3BVEXTPROC) (GLbyte const * v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3DPROC) (GLdouble red, GLdouble green, GLdouble blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3DEXTPROC) (GLdouble red, GLdouble green, GLdouble blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3DVPROC) (GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3DVEXTPROC) (GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3FPROC) (GLfloat red, GLfloat green, GLfloat blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3FEXTPROC) (GLfloat red, GLfloat green, GLfloat blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3FVPROC) (GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3FVEXTPROC) (GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3HNVPROC) (GLhalfNV red, GLhalfNV green, GLhalfNV blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3HVNVPROC) (GLhalfNV const * v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3IPROC) (GLint red, GLint green, GLint blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3IEXTPROC) (GLint red, GLint green, GLint blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3IVPROC) (GLint const * v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3IVEXTPROC) (GLint const * v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3SPROC) (GLshort red, GLshort green, GLshort blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3SEXTPROC) (GLshort red, GLshort green, GLshort blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3SVPROC) (GLshort const * v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3SVEXTPROC) (GLshort const * v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UBPROC) (GLubyte red, GLubyte green, GLubyte blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UBEXTPROC) (GLubyte red, GLubyte green, GLubyte blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UBVPROC) (GLubyte const * v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UBVEXTPROC) (GLubyte const * v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UIPROC) (GLuint red, GLuint green, GLuint blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UIEXTPROC) (GLuint red, GLuint green, GLuint blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UIVPROC) (GLuint const * v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UIVEXTPROC) (GLuint const * v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3USPROC) (GLushort red, GLushort green, GLushort blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3USEXTPROC) (GLushort red, GLushort green, GLushort blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3USVPROC) (GLushort const * v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3USVEXTPROC) (GLushort const * v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLORPOINTERPROC) (GLint size, GLenum type, GLsizei stride, const void * pointer);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLORPOINTEREXTPROC) (GLint size, GLenum type, GLsizei stride, const void * pointer);
typedef void (GLAPIENTRY * PFNGLSEPARABLEFILTER2DPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * row, const void * column);
typedef void (GLAPIENTRY * PFNGLSETFENCENVPROC) (GLuint fence, GLenum condition);
typedef void (GLAPIENTRY * PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, GLchar const *const* string, GLint const * length);
typedef void (GLAPIENTRY * PFNGLSHADERSOURCEARBPROC) (GLhandleARB shaderObj, GLsizei count, GLcharARB const ** string, GLint const * length);
typedef void (GLAPIENTRY * PFNGLSTENCILFUNCSEPARATEPROC) (GLenum face, GLenum func, GLint ref, GLuint mask);
typedef void (GLAPIENTRY * PFNGLSTENCILMASKSEPARATEPROC) (GLenum face, GLuint mask);
typedef void (GLAPIENTRY * PFNGLSTENCILOPSEPARATEPROC) (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
typedef GLboolean (GLAPIENTRY * PFNGLTESTFENCENVPROC) (GLuint fence);
typedef void (GLAPIENTRY * PFNGLTEXBUFFERARBPROC) (GLenum target, GLenum internalformat, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLTEXBUFFEREXTPROC) (GLenum target, GLenum internalformat, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLTEXCOORD1HNVPROC) (GLhalfNV s);
typedef void (GLAPIENTRY * PFNGLTEXCOORD1HVNVPROC) (GLhalfNV const * v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2HNVPROC) (GLhalfNV s, GLhalfNV t);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2HVNVPROC) (GLhalfNV const * v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD3HNVPROC) (GLhalfNV s, GLhalfNV t, GLhalfNV r);
typedef void (GLAPIENTRY * PFNGLTEXCOORD3HVNVPROC) (GLhalfNV const * v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD4HNVPROC) (GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q);
typedef void (GLAPIENTRY * PFNGLTEXCOORD4HVNVPROC) (GLhalfNV const * v);
typedef void (GLAPIENTRY * PFNGLTEXCOORDPOINTEREXTPROC) (GLint size, GLenum type, GLsizei stride, GLsizei count, const void * pointer);
typedef void (GLAPIENTRY * PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void * pixels);
typedef void (GLAPIENTRY * PFNGLTEXIMAGE3DEXTPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void * pixels);
typedef void (GLAPIENTRY * PFNGLTEXPARAMETERIIVPROC) (GLenum target, GLenum pname, GLint const * params);
typedef void (GLAPIENTRY * PFNGLTEXPARAMETERIIVEXTPROC) (GLenum target, GLenum pname, GLint const * params);
typedef void (GLAPIENTRY * PFNGLTEXPARAMETERIUIVPROC) (GLenum target, GLenum pname, GLuint const * params);
typedef void (GLAPIENTRY * PFNGLTEXPARAMETERIUIVEXTPROC) (GLenum target, GLenum pname, GLuint const * params);
typedef void (GLAPIENTRY * PFNGLTEXRENDERBUFFERNVPROC) (GLenum target, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLTEXSTORAGE2DPROC) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLTEXSUBIMAGE3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void * pixels);
typedef void (GLAPIENTRY * PFNGLTEXSUBIMAGE3DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void * pixels);
typedef void (GLAPIENTRY * PFNGLTEXTUREBARRIERNVPROC) ();
typedef void (GLAPIENTRY * PFNGLTEXTUREBUFFEREXTPROC) (GLuint texture, GLenum target, GLenum internalformat, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLTEXTUREIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void * pixels);
typedef void (GLAPIENTRY * PFNGLTEXTUREIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void * pixels);
typedef void (GLAPIENTRY * PFNGLTEXTUREIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void * pixels);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERFEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERFVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLfloat const * params);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERIEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERIIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLint const * params);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERIUIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLuint const * params);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLint const * params);
typedef void (GLAPIENTRY * PFNGLTEXTURERENDERBUFFEREXTPROC) (GLuint texture, GLenum target, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGE2DEXTPROC) (GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLTEXTURESUBIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void * pixels);
typedef void (GLAPIENTRY * PFNGLTEXTURESUBIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * pixels);
typedef void (GLAPIENTRY * PFNGLTEXTURESUBIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void * pixels);
typedef void (GLAPIENTRY * PFNGLTRACKMATRIXNVPROC) (GLenum target, GLuint address, GLenum matrix, GLenum transform);
typedef void (GLAPIENTRY * PFNGLTRANSFORMFEEDBACKATTRIBSNVPROC) (GLsizei count, GLint const * attribs, GLenum bufferMode);
typedef void (GLAPIENTRY * PFNGLTRANSFORMFEEDBACKVARYINGSPROC) (GLuint program, GLsizei count, GLchar const *const* varyings, GLenum bufferMode);
typedef void (GLAPIENTRY * PFNGLTRANSFORMFEEDBACKVARYINGSNVPROC) (GLuint program, GLsizei count, GLint const * locations, GLenum bufferMode);
typedef void (GLAPIENTRY * PFNGLUNIFORM1FPROC) (GLint location, GLfloat v0);
typedef void (GLAPIENTRY * PFNGLUNIFORM1FARBPROC) (GLint location, GLfloat v0);
typedef void (GLAPIENTRY * PFNGLUNIFORM1FVPROC) (GLint location, GLsizei count, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORM1FVARBPROC) (GLint location, GLsizei count, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORM1IPROC) (GLint location, GLint v0);
typedef void (GLAPIENTRY * PFNGLUNIFORM1IARBPROC) (GLint location, GLint v0);
typedef void (GLAPIENTRY * PFNGLUNIFORM1IVPROC) (GLint location, GLsizei count, GLint const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORM1IVARBPROC) (GLint location, GLsizei count, GLint const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORM1UIPROC) (GLint location, GLuint v0);
typedef void (GLAPIENTRY * PFNGLUNIFORM1UIEXTPROC) (GLint location, GLuint v0);
typedef void (GLAPIENTRY * PFNGLUNIFORM1UIVPROC) (GLint location, GLsizei count, GLuint const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORM1UIVEXTPROC) (GLint location, GLsizei count, GLuint const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2FPROC) (GLint location, GLfloat v0, GLfloat v1);
typedef void (GLAPIENTRY * PFNGLUNIFORM2FARBPROC) (GLint location, GLfloat v0, GLfloat v1);
typedef void (GLAPIENTRY * PFNGLUNIFORM2FVPROC) (GLint location, GLsizei count, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2FVARBPROC) (GLint location, GLsizei count, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2IPROC) (GLint location, GLint v0, GLint v1);
typedef void (GLAPIENTRY * PFNGLUNIFORM2IARBPROC) (GLint location, GLint v0, GLint v1);
typedef void (GLAPIENTRY * PFNGLUNIFORM2IVPROC) (GLint location, GLsizei count, GLint const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2IVARBPROC) (GLint location, GLsizei count, GLint const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2UIPROC) (GLint location, GLuint v0, GLuint v1);
typedef void (GLAPIENTRY * PFNGLUNIFORM2UIEXTPROC) (GLint location, GLuint v0, GLuint v1);
typedef void (GLAPIENTRY * PFNGLUNIFORM2UIVPROC) (GLint location, GLsizei count, GLuint const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2UIVEXTPROC) (GLint location, GLsizei count, GLuint const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (GLAPIENTRY * PFNGLUNIFORM3FARBPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (GLAPIENTRY * PFNGLUNIFORM3FVPROC) (GLint location, GLsizei count, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3FVARBPROC) (GLint location, GLsizei count, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3IPROC) (GLint location, GLint v0, GLint v1, GLint v2);
typedef void (GLAPIENTRY * PFNGLUNIFORM3IARBPROC) (GLint location, GLint v0, GLint v1, GLint v2);
typedef void (GLAPIENTRY * PFNGLUNIFORM3IVPROC) (GLint location, GLsizei count, GLint const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3IVARBPROC) (GLint location, GLsizei count, GLint const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3UIPROC) (GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void (GLAPIENTRY * PFNGLUNIFORM3UIEXTPROC) (GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void (GLAPIENTRY * PFNGLUNIFORM3UIVPROC) (GLint location, GLsizei count, GLuint const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3UIVEXTPROC) (GLint location, GLsizei count, GLuint const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (GLAPIENTRY * PFNGLUNIFORM4FARBPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (GLAPIENTRY * PFNGLUNIFORM4FVPROC) (GLint location, GLsizei count, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4FVARBPROC) (GLint location, GLsizei count, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4IPROC) (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (GLAPIENTRY * PFNGLUNIFORM4IARBPROC) (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (GLAPIENTRY * PFNGLUNIFORM4IVPROC) (GLint location, GLsizei count, GLint const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4IVARBPROC) (GLint location, GLsizei count, GLint const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4UIPROC) (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void (GLAPIENTRY * PFNGLUNIFORM4UIEXTPROC) (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void (GLAPIENTRY * PFNGLUNIFORM4UIVPROC) (GLint location, GLsizei count, GLuint const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4UIVEXTPROC) (GLint location, GLsizei count, GLuint const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORMBUFFEREXTPROC) (GLuint program, GLint location, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX2FVPROC) (GLint location, GLsizei count, GLboolean transpose, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX2FVARBPROC) (GLint location, GLsizei count, GLboolean transpose, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX2X3FVPROC) (GLint location, GLsizei count, GLboolean transpose, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX2X4FVPROC) (GLint location, GLsizei count, GLboolean transpose, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX3FVPROC) (GLint location, GLsizei count, GLboolean transpose, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX3FVARBPROC) (GLint location, GLsizei count, GLboolean transpose, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX3X2FVPROC) (GLint location, GLsizei count, GLboolean transpose, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX3X4FVPROC) (GLint location, GLsizei count, GLboolean transpose, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX4FVPROC) (GLint location, GLsizei count, GLboolean transpose, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX4FVARBPROC) (GLint location, GLsizei count, GLboolean transpose, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX4X2FVPROC) (GLint location, GLsizei count, GLboolean transpose, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX4X3FVPROC) (GLint location, GLsizei count, GLboolean transpose, GLfloat const * value);
typedef void (GLAPIENTRY * PFNGLUNLOCKARRAYSEXTPROC) ();
typedef GLboolean (GLAPIENTRY * PFNGLUNMAPBUFFERPROC) (GLenum target);
typedef GLboolean (GLAPIENTRY * PFNGLUNMAPBUFFERARBPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLUSEPROGRAMOBJECTARBPROC) (GLhandleARB programObj);
typedef void (GLAPIENTRY * PFNGLVALIDATEPROGRAMPROC) (GLuint program);
typedef void (GLAPIENTRY * PFNGLVALIDATEPROGRAMARBPROC) (GLhandleARB programObj);
typedef void (GLAPIENTRY * PFNGLVERTEX2HNVPROC) (GLhalfNV x, GLhalfNV y);
typedef void (GLAPIENTRY * PFNGLVERTEX2HVNVPROC) (GLhalfNV const * v);
typedef void (GLAPIENTRY * PFNGLVERTEX3HNVPROC) (GLhalfNV x, GLhalfNV y, GLhalfNV z);
typedef void (GLAPIENTRY * PFNGLVERTEX3HVNVPROC) (GLhalfNV const * v);
typedef void (GLAPIENTRY * PFNGLVERTEX4HNVPROC) (GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w);
typedef void (GLAPIENTRY * PFNGLVERTEX4HVNVPROC) (GLhalfNV const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYRANGENVPROC) (GLsizei length, const void * pointer);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1DPROC) (GLuint index, GLdouble x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1DARBPROC) (GLuint index, GLdouble x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1DNVPROC) (GLuint index, GLdouble x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1DVPROC) (GLuint index, GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1DVARBPROC) (GLuint index, GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1DVNVPROC) (GLuint index, GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1FPROC) (GLuint index, GLfloat x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1FARBPROC) (GLuint index, GLfloat x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1FNVPROC) (GLuint index, GLfloat x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1FVPROC) (GLuint index, GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1FVARBPROC) (GLuint index, GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1FVNVPROC) (GLuint index, GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1HNVPROC) (GLuint index, GLhalfNV x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1HVNVPROC) (GLuint index, GLhalfNV const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1SPROC) (GLuint index, GLshort x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1SARBPROC) (GLuint index, GLshort x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1SNVPROC) (GLuint index, GLshort x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1SVPROC) (GLuint index, GLshort const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1SVARBPROC) (GLuint index, GLshort const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1SVNVPROC) (GLuint index, GLshort const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2DPROC) (GLuint index, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2DARBPROC) (GLuint index, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2DNVPROC) (GLuint index, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2DVPROC) (GLuint index, GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2DVARBPROC) (GLuint index, GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2DVNVPROC) (GLuint index, GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2FPROC) (GLuint index, GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2FARBPROC) (GLuint index, GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2FNVPROC) (GLuint index, GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2FVPROC) (GLuint index, GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2FVARBPROC) (GLuint index, GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2FVNVPROC) (GLuint index, GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2HNVPROC) (GLuint index, GLhalfNV x, GLhalfNV y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2HVNVPROC) (GLuint index, GLhalfNV const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2SPROC) (GLuint index, GLshort x, GLshort y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2SARBPROC) (GLuint index, GLshort x, GLshort y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2SNVPROC) (GLuint index, GLshort x, GLshort y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2SVPROC) (GLuint index, GLshort const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2SVARBPROC) (GLuint index, GLshort const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2SVNVPROC) (GLuint index, GLshort const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3DPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3DARBPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3DNVPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3DVPROC) (GLuint index, GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3DVARBPROC) (GLuint index, GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3DVNVPROC) (GLuint index, GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3FPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3FARBPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3FNVPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3FVPROC) (GLuint index, GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3FVARBPROC) (GLuint index, GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3FVNVPROC) (GLuint index, GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3HNVPROC) (GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3HVNVPROC) (GLuint index, GLhalfNV const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3SPROC) (GLuint index, GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3SARBPROC) (GLuint index, GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3SNVPROC) (GLuint index, GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3SVPROC) (GLuint index, GLshort const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3SVARBPROC) (GLuint index, GLshort const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3SVNVPROC) (GLuint index, GLshort const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4BVPROC) (GLuint index, GLbyte const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4BVARBPROC) (GLuint index, GLbyte const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4DPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4DARBPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4DNVPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4DVPROC) (GLuint index, GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4DVARBPROC) (GLuint index, GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4DVNVPROC) (GLuint index, GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4FPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4FARBPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4FNVPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4FVPROC) (GLuint index, GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4FVARBPROC) (GLuint index, GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4FVNVPROC) (GLuint index, GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4HNVPROC) (GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4HVNVPROC) (GLuint index, GLhalfNV const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4IVPROC) (GLuint index, GLint const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4IVARBPROC) (GLuint index, GLint const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NBVPROC) (GLuint index, GLbyte const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NBVARBPROC) (GLuint index, GLbyte const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NIVPROC) (GLuint index, GLint const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NIVARBPROC) (GLuint index, GLint const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NSVPROC) (GLuint index, GLshort const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NSVARBPROC) (GLuint index, GLshort const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUBPROC) (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUBARBPROC) (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUBVPROC) (GLuint index, GLubyte const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUBVARBPROC) (GLuint index, GLubyte const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUIVPROC) (GLuint index, GLuint const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUIVARBPROC) (GLuint index, GLuint const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUSVPROC) (GLuint index, GLushort const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUSVARBPROC) (GLuint index, GLushort const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4SPROC) (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4SARBPROC) (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4SNVPROC) (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4SVPROC) (GLuint index, GLshort const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4SVARBPROC) (GLuint index, GLshort const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4SVNVPROC) (GLuint index, GLshort const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4UBNVPROC) (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4UBVPROC) (GLuint index, GLubyte const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4UBVARBPROC) (GLuint index, GLubyte const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4UBVNVPROC) (GLuint index, GLubyte const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4UIVPROC) (GLuint index, GLuint const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4UIVARBPROC) (GLuint index, GLuint const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4USVPROC) (GLuint index, GLushort const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4USVARBPROC) (GLuint index, GLushort const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1IPROC) (GLuint index, GLint x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1IEXTPROC) (GLuint index, GLint x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1IVPROC) (GLuint index, GLint const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1IVEXTPROC) (GLuint index, GLint const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1UIPROC) (GLuint index, GLuint x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1UIEXTPROC) (GLuint index, GLuint x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1UIVPROC) (GLuint index, GLuint const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1UIVEXTPROC) (GLuint index, GLuint const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2IPROC) (GLuint index, GLint x, GLint y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2IEXTPROC) (GLuint index, GLint x, GLint y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2IVPROC) (GLuint index, GLint const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2IVEXTPROC) (GLuint index, GLint const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2UIPROC) (GLuint index, GLuint x, GLuint y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2UIEXTPROC) (GLuint index, GLuint x, GLuint y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2UIVPROC) (GLuint index, GLuint const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2UIVEXTPROC) (GLuint index, GLuint const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3IPROC) (GLuint index, GLint x, GLint y, GLint z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3IEXTPROC) (GLuint index, GLint x, GLint y, GLint z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3IVPROC) (GLuint index, GLint const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3IVEXTPROC) (GLuint index, GLint const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3UIPROC) (GLuint index, GLuint x, GLuint y, GLuint z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3UIEXTPROC) (GLuint index, GLuint x, GLuint y, GLuint z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3UIVPROC) (GLuint index, GLuint const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3UIVEXTPROC) (GLuint index, GLuint const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4BVPROC) (GLuint index, GLbyte const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4BVEXTPROC) (GLuint index, GLbyte const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4IPROC) (GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4IEXTPROC) (GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4IVPROC) (GLuint index, GLint const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4IVEXTPROC) (GLuint index, GLint const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4SVPROC) (GLuint index, GLshort const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4SVEXTPROC) (GLuint index, GLshort const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4UBVPROC) (GLuint index, GLubyte const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4UBVEXTPROC) (GLuint index, GLubyte const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4UIPROC) (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4UIEXTPROC) (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4UIVPROC) (GLuint index, GLuint const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4UIVEXTPROC) (GLuint index, GLuint const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4USVPROC) (GLuint index, GLushort const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4USVEXTPROC) (GLuint index, GLushort const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBIPOINTERPROC) (GLuint index, GLint size, GLenum type, GLsizei stride, const void * pointer);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBIPOINTEREXTPROC) (GLuint index, GLint size, GLenum type, GLsizei stride, const void * pointer);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBPOINTERNVPROC) (GLuint index, GLint fsize, GLenum type, GLsizei stride, const void * pointer);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS1DVNVPROC) (GLuint index, GLsizei count, GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS1FVNVPROC) (GLuint index, GLsizei count, GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS1HVNVPROC) (GLuint index, GLsizei n, GLhalfNV const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS1SVNVPROC) (GLuint index, GLsizei count, GLshort const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS2DVNVPROC) (GLuint index, GLsizei count, GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS2FVNVPROC) (GLuint index, GLsizei count, GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS2HVNVPROC) (GLuint index, GLsizei n, GLhalfNV const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS2SVNVPROC) (GLuint index, GLsizei count, GLshort const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS3DVNVPROC) (GLuint index, GLsizei count, GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS3FVNVPROC) (GLuint index, GLsizei count, GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS3HVNVPROC) (GLuint index, GLsizei n, GLhalfNV const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS3SVNVPROC) (GLuint index, GLsizei count, GLshort const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS4DVNVPROC) (GLuint index, GLsizei count, GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS4FVNVPROC) (GLuint index, GLsizei count, GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS4HVNVPROC) (GLuint index, GLsizei n, GLhalfNV const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS4SVNVPROC) (GLuint index, GLsizei count, GLshort const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS4UBVNVPROC) (GLuint index, GLsizei count, GLubyte const * v);
typedef void (GLAPIENTRY * PFNGLVERTEXPOINTEREXTPROC) (GLint size, GLenum type, GLsizei stride, GLsizei count, const void * pointer);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2DPROC) (GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2DARBPROC) (GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2DVPROC) (GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2DVARBPROC) (GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2FPROC) (GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2FARBPROC) (GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2FVPROC) (GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2FVARBPROC) (GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2IPROC) (GLint x, GLint y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2IARBPROC) (GLint x, GLint y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2IVPROC) (GLint const * v);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2IVARBPROC) (GLint const * v);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2SPROC) (GLshort x, GLshort y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2SARBPROC) (GLshort x, GLshort y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2SVPROC) (GLshort const * v);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2SVARBPROC) (GLshort const * v);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3DPROC) (GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3DARBPROC) (GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3DVPROC) (GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3DVARBPROC) (GLdouble const * v);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3FPROC) (GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3FARBPROC) (GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3FVPROC) (GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3FVARBPROC) (GLfloat const * v);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3IPROC) (GLint x, GLint y, GLint z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3IARBPROC) (GLint x, GLint y, GLint z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3IVPROC) (GLint const * v);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3IVARBPROC) (GLint const * v);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3SPROC) (GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3SARBPROC) (GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3SVPROC) (GLshort const * v);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3SVARBPROC) (GLshort const * v);
typedef void * (GLAPIENTRY * PFNWGLALLOCATEMEMORYNVPROC) (GLsizei size, GLfloat readfreq, GLfloat writefreq, GLfloat priority);
typedef BOOL (GLAPIENTRY * PFNWGLBINDSWAPBARRIERNVPROC) (GLuint group, GLuint barrier);
typedef BOOL (GLAPIENTRY * PFNWGLBINDTEXIMAGEARBPROC) (HPBUFFERARB hPbuffer, int iBuffer);
typedef BOOL (GLAPIENTRY * PFNWGLBINDVIDEOIMAGENVPROC) (HPVIDEODEV hVideoDevice, HPBUFFERARB hPbuffer, int iVideoBuffer);
typedef BOOL (GLAPIENTRY * PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int * piAttribIList, FLOAT const * pfAttribFList, UINT nMaxFormats, int * piFormats, UINT * nNumFormats);
typedef HANDLE (GLAPIENTRY * PFNWGLCREATEBUFFERREGIONARBPROC) (HDC hDC, int iLayerPlane, UINT uType);
typedef HGLRC (GLAPIENTRY * PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int * attribList);
typedef HPBUFFERARB (GLAPIENTRY * PFNWGLCREATEPBUFFERARBPROC) (HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int * piAttribList);
typedef VOID (GLAPIENTRY * PFNWGLDELETEBUFFERREGIONARBPROC) (HANDLE hRegion);
typedef BOOL (GLAPIENTRY * PFNWGLDESTROYPBUFFERARBPROC) (HPBUFFERARB hPbuffer);
typedef BOOL (GLAPIENTRY * PFNWGLDXCLOSEDEVICENVPROC) (HANDLE hDevice);
typedef BOOL (GLAPIENTRY * PFNWGLDXLOCKOBJECTSNVPROC) (HANDLE hDevice, GLint count, HANDLE * hObjects);
typedef HANDLE (GLAPIENTRY * PFNWGLDXOPENDEVICENVPROC) (void * dxDevice);
typedef HANDLE (GLAPIENTRY * PFNWGLDXREGISTEROBJECTNVPROC) (HANDLE hDevice, void * dxObject, GLuint name, GLenum type, GLenum access);
typedef BOOL (GLAPIENTRY * PFNWGLDXSETRESOURCESHAREHANDLENVPROC) (void * dxObject, HANDLE shareHandle);
typedef BOOL (GLAPIENTRY * PFNWGLDXUNLOCKOBJECTSNVPROC) (HANDLE hDevice, GLint count, HANDLE * hObjects);
typedef BOOL (GLAPIENTRY * PFNWGLDXUNREGISTEROBJECTNVPROC) (HANDLE hDevice, HANDLE hObject);
typedef void (GLAPIENTRY * PFNWGLFREEMEMORYNVPROC) (void * pointer);
typedef HGLRC (GLAPIENTRY * PFNWGLGETCURRENTCONTEXTPROC) ();
typedef HDC (GLAPIENTRY * PFNWGLGETCURRENTREADDCARBPROC) ();
typedef const char * (GLAPIENTRY * PFNWGLGETEXTENSIONSSTRINGARBPROC) (HDC hdc);
typedef const char * (GLAPIENTRY * PFNWGLGETEXTENSIONSSTRINGEXTPROC) ();
typedef HDC (GLAPIENTRY * PFNWGLGETPBUFFERDCARBPROC) (HPBUFFERARB hPbuffer);
typedef BOOL (GLAPIENTRY * PFNWGLGETPIXELFORMATATTRIBFVARBPROC) (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int * piAttributes, FLOAT * pfValues);
typedef BOOL (GLAPIENTRY * PFNWGLGETPIXELFORMATATTRIBIVARBPROC) (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int * piAttributes, int * piValues);
typedef PROC (GLAPIENTRY * PFNWGLGETPROCADDRESSPROC) (LPCSTR lpszProc);
typedef int (GLAPIENTRY * PFNWGLGETSWAPINTERVALEXTPROC) ();
typedef BOOL (GLAPIENTRY * PFNWGLGETVIDEODEVICENVPROC) (HDC hDC, int numDevices, HPVIDEODEV * hVideoDevice);
typedef BOOL (GLAPIENTRY * PFNWGLGETVIDEOINFONVPROC) (HPVIDEODEV hpVideoDevice, unsigned long * pulCounterOutputPbuffer, unsigned long * pulCounterOutputVideo);
typedef BOOL (GLAPIENTRY * PFNWGLJOINSWAPGROUPNVPROC) (HDC hDC, GLuint group);
typedef BOOL (GLAPIENTRY * PFNWGLMAKECONTEXTCURRENTARBPROC) (HDC hDrawDC, HDC hReadDC, HGLRC hglrc);
typedef BOOL (GLAPIENTRY * PFNWGLQUERYFRAMECOUNTNVPROC) (HDC hDC, GLuint * count);
typedef BOOL (GLAPIENTRY * PFNWGLQUERYMAXSWAPGROUPSNVPROC) (HDC hDC, GLuint * maxGroups, GLuint * maxBarriers);
typedef BOOL (GLAPIENTRY * PFNWGLQUERYPBUFFERARBPROC) (HPBUFFERARB hPbuffer, int iAttribute, int * piValue);
typedef BOOL (GLAPIENTRY * PFNWGLQUERYSWAPGROUPNVPROC) (HDC hDC, GLuint * group, GLuint * barrier);
typedef int (GLAPIENTRY * PFNWGLRELEASEPBUFFERDCARBPROC) (HPBUFFERARB hPbuffer, HDC hDC);
typedef BOOL (GLAPIENTRY * PFNWGLRELEASETEXIMAGEARBPROC) (HPBUFFERARB hPbuffer, int iBuffer);
typedef BOOL (GLAPIENTRY * PFNWGLRELEASEVIDEODEVICENVPROC) (HPVIDEODEV hVideoDevice);
typedef BOOL (GLAPIENTRY * PFNWGLRELEASEVIDEOIMAGENVPROC) (HPBUFFERARB hPbuffer, int iVideoBuffer);
typedef BOOL (GLAPIENTRY * PFNWGLRESETFRAMECOUNTNVPROC) (HDC hDC);
typedef BOOL (GLAPIENTRY * PFNWGLRESTOREBUFFERREGIONARBPROC) (HANDLE hRegion, int x, int y, int width, int height, int xSrc, int ySrc);
typedef BOOL (GLAPIENTRY * PFNWGLSAVEBUFFERREGIONARBPROC) (HANDLE hRegion, int x, int y, int width, int height);
typedef BOOL (GLAPIENTRY * PFNWGLSENDPBUFFERTOVIDEONVPROC) (HPBUFFERARB hPbuffer, int iBufferType, unsigned long * pulCounterPbuffer, BOOL bBlock);
typedef BOOL (GLAPIENTRY * PFNWGLSETPBUFFERATTRIBARBPROC) (HPBUFFERARB hPbuffer, const int * piAttribList);
typedef BOOL (GLAPIENTRY * PFNWGLSWAPINTERVALEXTPROC) (int interval);

typedef void (GLAPIENTRY * PFNGLFINISHGLUTPROC) ();
typedef void (GLAPIENTRY * PFNGLVIEWPORTGLUTPROC) (GLint x, GLint y, GLsizei width, GLsizei height);
typedef BOOL (GLAPIENTRY * PFNWGLDELETECONTEXTGLUTPROC) (HGLRC oldContext);
typedef HGLRC (GLAPIENTRY * PFNWGLGETCURRENTCONTEXTGLUTPROC) ();
typedef HDC (GLAPIENTRY * PFNWGLGETCURRENTDCGLUTPROC) ();
typedef PROC (GLAPIENTRY * PFNWGLGETPROCADDRESSGLUTPROC) (LPCSTR lpszProc);
typedef BOOL (GLAPIENTRY * PFNWGLMAKECURRENTGLUTPROC) (HDC hDc, HGLRC newContext);

#define GLBLENDFUNCDLL 0
#define GLCOLORMASKDLL 1
#define GLDELETETEXTURESDLL 2
#define GLDEPTHMASKDLL 3
#define GLFINISHDLL 4
#define GLGENTEXTURESDLL 5
#define GLSCISSORDLL 6
#define GLVIEWPORTDLL 7
#define WGLGETPROCADDRESSDLL 8

#define GLACTIVESTENCILFACEEXT 0
#define GLACTIVETEXTUREARB 1
#define GLACTIVEVARYINGNV 2
#define GLAREPROGRAMSRESIDENTNV 3
#define GLARETEXTURESRESIDENTEXT 4
#define GLARRAYELEMENTEXT 5
#define GLATTACHOBJECTARB 6
#define GLATTACHSHADER 7
#define GLBEGINCONDITIONALRENDER 8
#define GLBEGINCONDITIONALRENDERNV 9
#define GLBEGINOCCLUSIONQUERYNV 10
#define GLBEGINQUERY 11
#define GLBEGINQUERYARB 12
#define GLBEGINTRANSFORMFEEDBACK 13
#define GLBEGINTRANSFORMFEEDBACKNV 14
#define GLBINDATTRIBLOCATION 15
#define GLBINDATTRIBLOCATIONARB 16
#define GLBINDBUFFER 17
#define GLBINDBUFFERBASE 18
#define GLBINDBUFFEROFFSETNV 19
#define GLBINDBUFFERRANGE 20
#define GLBINDBUFFERRANGENV 21
#define GLBINDFRAGDATALOCATION 22
#define GLBINDFRAGDATALOCATIONEXT 23
#define GLBINDFRAMEBUFFER 24
#define GLBINDPROGRAMARB 25
#define GLBINDPROGRAMNV 26
#define GLBINDRENDERBUFFER 27
#define GLBINDSAMPLER 28
#define GLBINDTEXTUREEXT 29
#define GLBINDVERTEXARRAY 30
#define GLBLENDCOLOR 31
#define GLBLENDCOLOREXT 32
#define GLBLENDEQUATION 33
#define GLBLENDEQUATIONEXT 34
#define GLBLENDEQUATIONSEPARATE 35
#define GLBLENDEQUATIONSEPARATEEXT 36
#define GLBLENDFUNCSEPARATE 37
#define GLBLENDFUNCSEPARATEEXT 38
#define GLBLITFRAMEBUFFER 39
#define GLBLITFRAMEBUFFEREXT 40
#define GLBUFFERDATA 41
#define GLBUFFERSUBDATA 42
#define GLBUFFERSUBDATAARB 43
#define GLCHECKFRAMEBUFFERSTATUS 44
#define GLCHECKFRAMEBUFFERSTATUSEXT 45
#define GLCHECKNAMEDFRAMEBUFFERSTATUSEXT 46
#define GLCLEARBUFFERFI 47
#define GLCLEARBUFFERFV 48
#define GLCLEARBUFFERIV 49
#define GLCLEARBUFFERUIV 50
#define GLCLEARCOLORIIEXT 51
#define GLCLEARCOLORIUIEXT 52
#define GLCLEARDEPTHDNV 53
#define GLCLIENTACTIVETEXTUREARB 54
#define GLCLIENTATTRIBDEFAULTEXT 55
#define GLCOLOR3HNV 56
#define GLCOLOR3HVNV 57
#define GLCOLOR4HNV 58
#define GLCOLOR4HVNV 59
#define GLCOLORMASKI 60
#define GLCOLORMASKINDEXEDEXT 61
#define GLCOLORPOINTEREXT 62
#define GLCOLORSUBTABLE 63
#define GLCOLORSUBTABLEEXT 64
#define GLCOLORTABLE 65
#define GLCOLORTABLEEXT 66
#define GLCOLORTABLEPARAMETERFV 67
#define GLCOLORTABLEPARAMETERIV 68
#define GLCOMBINERINPUTNV 69
#define GLCOMBINEROUTPUTNV 70
#define GLCOMBINERPARAMETERFNV 71
#define GLCOMBINERPARAMETERFVNV 72
#define GLCOMBINERPARAMETERINV 73
#define GLCOMBINERPARAMETERIVNV 74
#define GLCOMBINERSTAGEPARAMETERFVNV 75
#define GLCOMPILESHADER 76
#define GLCOMPILESHADERARB 77
#define GLCOMPRESSEDMULTITEXIMAGE1DEXT 78
#define GLCOMPRESSEDMULTITEXIMAGE2DEXT 79
#define GLCOMPRESSEDMULTITEXIMAGE3DEXT 80
#define GLCOMPRESSEDMULTITEXSUBIMAGE1DEXT 81
#define GLCOMPRESSEDMULTITEXSUBIMAGE2DEXT 82
#define GLCOMPRESSEDMULTITEXSUBIMAGE3DEXT 83
#define GLCOMPRESSEDTEXIMAGE1D 84
#define GLCOMPRESSEDTEXIMAGE1DARB 85
#define GLCOMPRESSEDTEXIMAGE2D 86
#define GLCOMPRESSEDTEXIMAGE2DARB 87
#define GLCOMPRESSEDTEXIMAGE3D 88
#define GLCOMPRESSEDTEXIMAGE3DARB 89
#define GLCOMPRESSEDTEXSUBIMAGE1D 90
#define GLCOMPRESSEDTEXSUBIMAGE1DARB 91
#define GLCOMPRESSEDTEXSUBIMAGE2D 92
#define GLCOMPRESSEDTEXSUBIMAGE2DARB 93
#define GLCOMPRESSEDTEXSUBIMAGE3D 94
#define GLCOMPRESSEDTEXSUBIMAGE3DARB 95
#define GLCOMPRESSEDTEXTUREIMAGE1DEXT 96
#define GLCOMPRESSEDTEXTUREIMAGE2DEXT 97
#define GLCOMPRESSEDTEXTUREIMAGE3DEXT 98
#define GLCOMPRESSEDTEXTURESUBIMAGE1DEXT 99
#define GLCOMPRESSEDTEXTURESUBIMAGE2DEXT 100
#define GLCOMPRESSEDTEXTURESUBIMAGE3DEXT 101
#define GLCONVOLUTIONFILTER1D 102
#define GLCONVOLUTIONFILTER2D 103
#define GLCONVOLUTIONPARAMETERF 104
#define GLCONVOLUTIONPARAMETERFV 105
#define GLCONVOLUTIONPARAMETERI 106
#define GLCONVOLUTIONPARAMETERIV 107
#define GLCOPYCOLORSUBTABLE 108
#define GLCOPYCOLORTABLE 109
#define GLCOPYCONVOLUTIONFILTER1D 110
#define GLCOPYCONVOLUTIONFILTER2D 111
#define GLCOPYIMAGESUBDATA 112
#define GLCOPYMULTITEXIMAGE1DEXT 113
#define GLCOPYMULTITEXIMAGE2DEXT 114
#define GLCOPYMULTITEXSUBIMAGE1DEXT 115
#define GLCOPYMULTITEXSUBIMAGE3DEXT 116
#define GLCOPYTEXSUBIMAGE3D 117
#define GLCOPYTEXSUBIMAGE3DEXT 118
#define GLCOPYTEXTUREIMAGE1DEXT 119
#define GLCOPYTEXTUREIMAGE2DEXT 120
#define GLCOPYTEXTURESUBIMAGE1DEXT 121
#define GLCOPYTEXTURESUBIMAGE2DEXT 122
#define GLCOPYTEXTURESUBIMAGE3DEXT 123
#define GLCREATEPROGRAM 124
#define GLCREATEPROGRAMOBJECTARB 125
#define GLCREATESHADER 126
#define GLCREATESHADEROBJECTARB 127
#define GLDELETEBUFFERS 128
#define GLDELETEFENCESNV 129
#define GLDELETEFRAMEBUFFERS 130
#define GLDELETEOBJECTARB 131
#define GLDELETEOCCLUSIONQUERIESNV 132
#define GLDELETEPROGRAM 133
#define GLDELETEPROGRAMSARB 134
#define GLDELETEPROGRAMSNV 135
#define GLDELETEQUERIES 136
#define GLDELETEQUERIESARB 137
#define GLDELETERENDERBUFFERS 138
#define GLDELETESAMPLERS 139
#define GLDELETESHADER 140
#define GLDELETETEXTURESEXT 141
#define GLDELETEVERTEXARRAYS 142
#define GLDEPTHBOUNDSDNV 143
#define GLDEPTHBOUNDSEXT 144
#define GLDEPTHRANGEDNV 145
#define GLDETACHOBJECTARB 146
#define GLDETACHSHADER 147
#define GLDISABLECLIENTSTATEINDEXEDEXT 148
#define GLDISABLEI 149
#define GLDISABLEINDEXEDEXT 150
#define GLDISABLEVERTEXATTRIBARRAY 151
#define GLDRAWARRAYSEXT 152
#define GLDRAWARRAYSINSTANCEDARB 153
#define GLDRAWARRAYSINSTANCEDEXT 154
#define GLDRAWBUFFERS 155
#define GLDRAWBUFFERSARB 156
#define GLDRAWBUFFERSATI 157
#define GLDRAWELEMENTSINSTANCEDARB 158
#define GLDRAWELEMENTSINSTANCEDEXT 159
#define GLDRAWRANGEELEMENTSEXT 160
#define GLEDGEFLAGPOINTEREXT 161
#define GLENABLECLIENTSTATEINDEXEDEXT 162
#define GLENABLEI 163
#define GLENABLEINDEXEDEXT 164
#define GLENDCONDITIONALRENDER 165
#define GLENDCONDITIONALRENDERNV 166
#define GLENDOCCLUSIONQUERYNV 167
#define GLENDQUERY 168
#define GLENDQUERYARB 169
#define GLENDTRANSFORMFEEDBACK 170
#define GLENDTRANSFORMFEEDBACKNV 171
#define GLEXECUTEPROGRAMNV 172
#define GLFINALCOMBINERINPUTNV 173
#define GLFINISHFENCENV 174
#define GLFLUSHMAPPEDBUFFERRANGE 175
#define GLFLUSHPIXELDATARANGENV 176
#define GLFLUSHVERTEXARRAYRANGENV 177
#define GLFOGCOORDD 178
#define GLFOGCOORDDEXT 179
#define GLFOGCOORDDV 180
#define GLFOGCOORDDVEXT 181
#define GLFOGCOORDF 182
#define GLFOGCOORDFEXT 183
#define GLFOGCOORDFV 184
#define GLFOGCOORDFVEXT 185
#define GLFOGCOORDHNV 186
#define GLFOGCOORDHVNV 187
#define GLFOGCOORDPOINTER 188
#define GLFOGCOORDPOINTEREXT 189
#define GLFRAMEBUFFERDRAWBUFFEREXT 190
#define GLFRAMEBUFFERDRAWBUFFERSEXT 191
#define GLFRAMEBUFFERREADBUFFEREXT 192
#define GLFRAMEBUFFERRENDERBUFFER 193
#define GLFRAMEBUFFERTEXTURE 194
#define GLFRAMEBUFFERTEXTURE1D 195
#define GLFRAMEBUFFERTEXTURE1DEXT 196
#define GLFRAMEBUFFERTEXTURE2D 197
#define GLFRAMEBUFFERTEXTURE3D 198
#define GLFRAMEBUFFERTEXTURE3DEXT 199
#define GLFRAMEBUFFERTEXTUREARB 200
#define GLFRAMEBUFFERTEXTUREEXT 201
#define GLFRAMEBUFFERTEXTUREFACEARB 202
#define GLFRAMEBUFFERTEXTUREFACEEXT 203
#define GLFRAMEBUFFERTEXTURELAYER 204
#define GLFRAMEBUFFERTEXTURELAYERARB 205
#define GLFRAMEBUFFERTEXTURELAYEREXT 206
#define GLGENBUFFERS 207
#define GLGENBUFFERSARB 208
#define GLGENERATEMIPMAP 209
#define GLGENERATEMIPMAPEXT 210
#define GLGENERATEMULTITEXMIPMAPEXT 211
#define GLGENERATETEXTUREMIPMAPEXT 212
#define GLGENFENCESNV 213
#define GLGENFRAMEBUFFERS 214
#define GLGENOCCLUSIONQUERIESNV 215
#define GLGENPROGRAMSARB 216
#define GLGENPROGRAMSNV 217
#define GLGENQUERIES 218
#define GLGENQUERIESARB 219
#define GLGENRENDERBUFFERS 220
#define GLGENSAMPLERS 221
#define GLGENTEXTURESEXT 222
#define GLGENVERTEXARRAYS 223
#define GLGETACTIVEATTRIB 224
#define GLGETACTIVEATTRIBARB 225
#define GLGETACTIVEUNIFORM 226
#define GLGETACTIVEUNIFORMARB 227
#define GLGETACTIVEVARYINGNV 228
#define GLGETATTACHEDOBJECTSARB 229
#define GLGETATTACHEDSHADERS 230
#define GLGETATTRIBLOCATION 231
#define GLGETATTRIBLOCATIONARB 232
#define GLGETBOOLEANI_V 233
#define GLGETBOOLEANINDEXEDVEXT 234
#define GLGETBUFFERPARAMETERIV 235
#define GLGETBUFFERPARAMETERIVARB 236
#define GLGETBUFFERPOINTERV 237
#define GLGETBUFFERPOINTERVARB 238
#define GLGETBUFFERSUBDATA 239
#define GLGETBUFFERSUBDATAARB 240
#define GLGETCOLORTABLE 241
#define GLGETCOLORTABLEEXT 242
#define GLGETCOLORTABLEPARAMETERFV 243
#define GLGETCOLORTABLEPARAMETERFVEXT 244
#define GLGETCOLORTABLEPARAMETERIV 245
#define GLGETCOLORTABLEPARAMETERIVEXT 246
#define GLGETCOMBINERINPUTPARAMETERFVNV 247
#define GLGETCOMBINERINPUTPARAMETERIVNV 248
#define GLGETCOMBINEROUTPUTPARAMETERFVNV 249
#define GLGETCOMBINEROUTPUTPARAMETERIVNV 250
#define GLGETCOMBINERSTAGEPARAMETERFVNV 251
#define GLGETCOMPRESSEDMULTITEXIMAGEEXT 252
#define GLGETCOMPRESSEDTEXIMAGE 253
#define GLGETCOMPRESSEDTEXIMAGEARB 254
#define GLGETCOMPRESSEDTEXTUREIMAGEEXT 255
#define GLGETCONVOLUTIONFILTER 256
#define GLGETCONVOLUTIONPARAMETERFV 257
#define GLGETCONVOLUTIONPARAMETERIV 258
#define GLGETDOUBLEINDEXEDVEXT 259
#define GLGETFENCEIVNV 260
#define GLGETFINALCOMBINERINPUTPARAMETERFVNV 261
#define GLGETFINALCOMBINERINPUTPARAMETERIVNV 262
#define GLGETFLOATINDEXEDVEXT 263
#define GLGETFRAGDATALOCATION 264
#define GLGETFRAGDATALOCATIONEXT 265
#define GLGETFRAMEBUFFERATTACHMENTPARAMETERIV 266
#define GLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXT 267
#define GLGETFRAMEBUFFERPARAMETERIVEXT 268
#define GLGETHANDLEARB 269
#define GLGETHISTOGRAM 270
#define GLGETHISTOGRAMPARAMETERFV 271
#define GLGETHISTOGRAMPARAMETERIV 272
#define GLGETINFOLOGARB 273
#define GLGETINTEGERI_V 274
#define GLGETINTEGERINDEXEDVEXT 275
#define GLGETMINMAX 276
#define GLGETMINMAXPARAMETERFV 277
#define GLGETMINMAXPARAMETERIV 278
#define GLGETMULTISAMPLEFVNV 279
#define GLGETMULTITEXENVFVEXT 280
#define GLGETMULTITEXENVIVEXT 281
#define GLGETMULTITEXGENDVEXT 282
#define GLGETMULTITEXGENFVEXT 283
#define GLGETMULTITEXGENIVEXT 284
#define GLGETMULTITEXIMAGEEXT 285
#define GLGETMULTITEXLEVELPARAMETERFVEXT 286
#define GLGETMULTITEXLEVELPARAMETERIVEXT 287
#define GLGETMULTITEXPARAMETERFVEXT 288
#define GLGETMULTITEXPARAMETERIIVEXT 289
#define GLGETMULTITEXPARAMETERIUIVEXT 290
#define GLGETMULTITEXPARAMETERIVEXT 291
#define GLGETNAMEDBUFFERPARAMETERIVEXT 292
#define GLGETNAMEDBUFFERPOINTERVEXT 293
#define GLGETNAMEDBUFFERSUBDATAEXT 294
#define GLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVEXT 295
#define GLGETNAMEDPROGRAMIVEXT 296
#define GLGETNAMEDPROGRAMLOCALPARAMETERDVEXT 297
#define GLGETNAMEDPROGRAMLOCALPARAMETERFVEXT 298
#define GLGETNAMEDPROGRAMLOCALPARAMETERIIVEXT 299
#define GLGETNAMEDPROGRAMLOCALPARAMETERIUIVEXT 300
#define GLGETNAMEDPROGRAMSTRINGEXT 301
#define GLGETNAMEDRENDERBUFFERPARAMETERIVEXT 302
#define GLGETOBJECTPARAMETERFVARB 303
#define GLGETOBJECTPARAMETERIVARB 304
#define GLGETOCCLUSIONQUERYIVNV 305
#define GLGETOCCLUSIONQUERYUIVNV 306
#define GLGETPOINTERINDEXEDVEXT 307
#define GLGETPOINTERVEXT 308
#define GLGETPROGRAMENVPARAMETERDVARB 309
#define GLGETPROGRAMENVPARAMETERIIVNV 310
#define GLGETPROGRAMENVPARAMETERIUIVNV 311
#define GLGETPROGRAMINFOLOG 312
#define GLGETPROGRAMIV 313
#define GLGETPROGRAMIVNV 314
#define GLGETPROGRAMLOCALPARAMETERDVARB 315
#define GLGETPROGRAMLOCALPARAMETERFVARB 316
#define GLGETPROGRAMLOCALPARAMETERIIVNV 317
#define GLGETPROGRAMLOCALPARAMETERIUIVNV 318
#define GLGETPROGRAMNAMEDPARAMETERDVNV 319
#define GLGETPROGRAMNAMEDPARAMETERFVNV 320
#define GLGETPROGRAMPARAMETERDVNV 321
#define GLGETPROGRAMPARAMETERFVNV 322
#define GLGETPROGRAMSTRINGARB 323
#define GLGETPROGRAMSTRINGNV 324
#define GLGETQUERYIV 325
#define GLGETQUERYIVARB 326
#define GLGETQUERYOBJECTI64VEXT 327
#define GLGETQUERYOBJECTIV 328
#define GLGETQUERYOBJECTIVARB 329
#define GLGETQUERYOBJECTUI64V 330
#define GLGETQUERYOBJECTUI64VEXT 331
#define GLGETQUERYOBJECTUIV 332
#define GLGETQUERYOBJECTUIVARB 333
#define GLGETRENDERBUFFERPARAMETERIV 334
#define GLGETRENDERBUFFERPARAMETERIVEXT 335
#define GLGETSEPARABLEFILTER 336
#define GLGETSHADERINFOLOG 337
#define GLGETSHADERIV 338
#define GLGETSHADERSOURCE 339
#define GLGETSHADERSOURCEARB 340
#define GLGETSTRINGI 341
#define GLGETTEXPARAMETERIIV 342
#define GLGETTEXPARAMETERIIVEXT 343
#define GLGETTEXPARAMETERIUIV 344
#define GLGETTEXPARAMETERIUIVEXT 345
#define GLGETTEXTURELEVELPARAMETERFVEXT 346
#define GLGETTEXTURELEVELPARAMETERIVEXT 347
#define GLGETTEXTUREPARAMETERFVEXT 348
#define GLGETTEXTUREPARAMETERIIVEXT 349
#define GLGETTEXTUREPARAMETERIUIVEXT 350
#define GLGETTEXTUREPARAMETERIVEXT 351
#define GLGETTRACKMATRIXIVNV 352
#define GLGETTRANSFORMFEEDBACKVARYING 353
#define GLGETTRANSFORMFEEDBACKVARYINGNV 354
#define GLGETUNIFORMBUFFERSIZEEXT 355
#define GLGETUNIFORMFV 356
#define GLGETUNIFORMFVARB 357
#define GLGETUNIFORMIV 358
#define GLGETUNIFORMIVARB 359
#define GLGETUNIFORMLOCATION 360
#define GLGETUNIFORMLOCATIONARB 361
#define GLGETUNIFORMOFFSETEXT 362
#define GLGETUNIFORMUIV 363
#define GLGETUNIFORMUIVEXT 364
#define GLGETVARYINGLOCATIONNV 365
#define GLGETVERTEXATTRIBDV 366
#define GLGETVERTEXATTRIBDVARB 367
#define GLGETVERTEXATTRIBDVNV 368
#define GLGETVERTEXATTRIBFV 369
#define GLGETVERTEXATTRIBFVARB 370
#define GLGETVERTEXATTRIBFVNV 371
#define GLGETVERTEXATTRIBIIV 372
#define GLGETVERTEXATTRIBIIVEXT 373
#define GLGETVERTEXATTRIBIUIV 374
#define GLGETVERTEXATTRIBIUIVEXT 375
#define GLGETVERTEXATTRIBIV 376
#define GLGETVERTEXATTRIBIVARB 377
#define GLGETVERTEXATTRIBIVNV 378
#define GLGETVERTEXATTRIBPOINTERV 379
#define GLGETVERTEXATTRIBPOINTERVARB 380
#define GLGETVERTEXATTRIBPOINTERVNV 381
#define GLGETVIDEOI64VNV 382
#define GLGETVIDEOIVNV 383
#define GLGETVIDEOUI64VNV 384
#define GLGETVIDEOUIVNV 385
#define GLHISTOGRAM 386
#define GLINDEXPOINTEREXT 387
#define GLISBUFFER 388
#define GLISENABLEDI 389
#define GLISENABLEDINDEXEDEXT 390
#define GLISFENCENV 391
#define GLISFRAMEBUFFER 392
#define GLISFRAMEBUFFEREXT 393
#define GLISOCCLUSIONQUERYNV 394
#define GLISPROGRAM 395
#define GLISPROGRAMARB 396
#define GLISPROGRAMNV 397
#define GLISQUERY 398
#define GLISQUERYARB 399
#define GLISRENDERBUFFER 400
#define GLISRENDERBUFFEREXT 401
#define GLISSHADER 402
#define GLISTEXTUREEXT 403
#define GLISVERTEXARRAY 404
#define GLLINKPROGRAM 405
#define GLLINKPROGRAMARB 406
#define GLLOADPROGRAMNV 407
#define GLLOADTRANSPOSEMATRIXD 408
#define GLLOADTRANSPOSEMATRIXDARB 409
#define GLLOADTRANSPOSEMATRIXFARB 410
#define GLLOCKARRAYSEXT 411
#define GLMAPBUFFER 412
#define GLMAPBUFFERARB 413
#define GLMAPBUFFERRANGE 414
#define GLMATRIXFRUSTUMEXT 415
#define GLMATRIXLOADDEXT 416
#define GLMATRIXLOADFEXT 417
#define GLMATRIXLOADIDENTITYEXT 418
#define GLMATRIXLOADTRANSPOSEDEXT 419
#define GLMATRIXLOADTRANSPOSEFEXT 420
#define GLMATRIXMULTDEXT 421
#define GLMATRIXMULTFEXT 422
#define GLMATRIXMULTTRANSPOSEDEXT 423
#define GLMATRIXMULTTRANSPOSEFEXT 424
#define GLMATRIXORTHOEXT 425
#define GLMATRIXPOPEXT 426
#define GLMATRIXPUSHEXT 427
#define GLMATRIXROTATEDEXT 428
#define GLMATRIXROTATEFEXT 429
#define GLMATRIXSCALEDEXT 430
#define GLMATRIXSCALEFEXT 431
#define GLMATRIXTRANSLATEDEXT 432
#define GLMATRIXTRANSLATEFEXT 433
#define GLMINMAX 434
#define GLMULTIDRAWARRAYS 435
#define GLMULTIDRAWARRAYSEXT 436
#define GLMULTIDRAWELEMENTS 437
#define GLMULTIDRAWELEMENTSEXT 438
#define GLMULTITEXBUFFEREXT 439
#define GLMULTITEXCOORD1D 440
#define GLMULTITEXCOORD1DARB 441
#define GLMULTITEXCOORD1DV 442
#define GLMULTITEXCOORD1DVARB 443
#define GLMULTITEXCOORD1F 444
#define GLMULTITEXCOORD1FARB 445
#define GLMULTITEXCOORD1FV 446
#define GLMULTITEXCOORD1FVARB 447
#define GLMULTITEXCOORD1HNV 448
#define GLMULTITEXCOORD1HVNV 449
#define GLMULTITEXCOORD1I 450
#define GLMULTITEXCOORD1IARB 451
#define GLMULTITEXCOORD1IV 452
#define GLMULTITEXCOORD1IVARB 453
#define GLMULTITEXCOORD1S 454
#define GLMULTITEXCOORD1SARB 455
#define GLMULTITEXCOORD1SV 456
#define GLMULTITEXCOORD1SVARB 457
#define GLMULTITEXCOORD2D 458
#define GLMULTITEXCOORD2DARB 459
#define GLMULTITEXCOORD2DV 460
#define GLMULTITEXCOORD2DVARB 461
#define GLMULTITEXCOORD2FARB 462
#define GLMULTITEXCOORD2FVARB 463
#define GLMULTITEXCOORD2HNV 464
#define GLMULTITEXCOORD2HVNV 465
#define GLMULTITEXCOORD2I 466
#define GLMULTITEXCOORD2IARB 467
#define GLMULTITEXCOORD2IV 468
#define GLMULTITEXCOORD2IVARB 469
#define GLMULTITEXCOORD2S 470
#define GLMULTITEXCOORD2SARB 471
#define GLMULTITEXCOORD2SV 472
#define GLMULTITEXCOORD2SVARB 473
#define GLMULTITEXCOORD3D 474
#define GLMULTITEXCOORD3DARB 475
#define GLMULTITEXCOORD3DV 476
#define GLMULTITEXCOORD3DVARB 477
#define GLMULTITEXCOORD3F 478
#define GLMULTITEXCOORD3FARB 479
#define GLMULTITEXCOORD3FV 480
#define GLMULTITEXCOORD3FVARB 481
#define GLMULTITEXCOORD3HNV 482
#define GLMULTITEXCOORD3HVNV 483
#define GLMULTITEXCOORD3I 484
#define GLMULTITEXCOORD3IARB 485
#define GLMULTITEXCOORD3IV 486
#define GLMULTITEXCOORD3IVARB 487
#define GLMULTITEXCOORD3S 488
#define GLMULTITEXCOORD3SARB 489
#define GLMULTITEXCOORD3SV 490
#define GLMULTITEXCOORD3SVARB 491
#define GLMULTITEXCOORD4D 492
#define GLMULTITEXCOORD4DARB 493
#define GLMULTITEXCOORD4DV 494
#define GLMULTITEXCOORD4DVARB 495
#define GLMULTITEXCOORD4F 496
#define GLMULTITEXCOORD4FARB 497
#define GLMULTITEXCOORD4FV 498
#define GLMULTITEXCOORD4FVARB 499
#define GLMULTITEXCOORD4HNV 500
#define GLMULTITEXCOORD4HVNV 501
#define GLMULTITEXCOORD4I 502
#define GLMULTITEXCOORD4IARB 503
#define GLMULTITEXCOORD4IV 504
#define GLMULTITEXCOORD4IVARB 505
#define GLMULTITEXCOORD4S 506
#define GLMULTITEXCOORD4SARB 507
#define GLMULTITEXCOORD4SV 508
#define GLMULTITEXCOORD4SVARB 509
#define GLMULTITEXCOORDPOINTEREXT 510
#define GLMULTITEXENVFEXT 511
#define GLMULTITEXENVFVEXT 512
#define GLMULTITEXENVIEXT 513
#define GLMULTITEXENVIVEXT 514
#define GLMULTITEXGENDEXT 515
#define GLMULTITEXGENDVEXT 516
#define GLMULTITEXGENFEXT 517
#define GLMULTITEXGENFVEXT 518
#define GLMULTITEXGENIEXT 519
#define GLMULTITEXGENIVEXT 520
#define GLMULTITEXIMAGE1DEXT 521
#define GLMULTITEXIMAGE2DEXT 522
#define GLMULTITEXIMAGE3DEXT 523
#define GLMULTITEXPARAMETERFEXT 524
#define GLMULTITEXPARAMETERFVEXT 525
#define GLMULTITEXPARAMETERIEXT 526
#define GLMULTITEXPARAMETERIIVEXT 527
#define GLMULTITEXPARAMETERIUIVEXT 528
#define GLMULTITEXPARAMETERIVEXT 529
#define GLMULTITEXRENDERBUFFEREXT 530
#define GLMULTITEXSUBIMAGE1DEXT 531
#define GLMULTITEXSUBIMAGE2DEXT 532
#define GLMULTITEXSUBIMAGE3DEXT 533
#define GLMULTTRANSPOSEMATRIXD 534
#define GLMULTTRANSPOSEMATRIXDARB 535
#define GLMULTTRANSPOSEMATRIXFARB 536
#define GLNAMEDFRAMEBUFFERRENDERBUFFEREXT 537
#define GLNAMEDFRAMEBUFFERTEXTURE1DEXT 538
#define GLNAMEDFRAMEBUFFERTEXTURE2DEXT 539
#define GLNAMEDFRAMEBUFFERTEXTURE3DEXT 540
#define GLNAMEDFRAMEBUFFERTEXTUREEXT 541
#define GLNAMEDFRAMEBUFFERTEXTUREFACEEXT 542
#define GLNAMEDFRAMEBUFFERTEXTURELAYEREXT 543
#define GLNAMEDPROGRAMLOCALPARAMETER4DEXT 544
#define GLNAMEDPROGRAMLOCALPARAMETER4DVEXT 545
#define GLNAMEDPROGRAMLOCALPARAMETER4FEXT 546
#define GLNAMEDPROGRAMLOCALPARAMETER4FVEXT 547
#define GLNAMEDPROGRAMLOCALPARAMETERI4IEXT 548
#define GLNAMEDPROGRAMLOCALPARAMETERI4IVEXT 549
#define GLNAMEDPROGRAMLOCALPARAMETERI4UIEXT 550
#define GLNAMEDPROGRAMLOCALPARAMETERI4UIVEXT 551
#define GLNAMEDPROGRAMLOCALPARAMETERS4FVEXT 552
#define GLNAMEDPROGRAMLOCALPARAMETERSI4IVEXT 553
#define GLNAMEDPROGRAMLOCALPARAMETERSI4UIVEXT 554
#define GLNAMEDPROGRAMSTRINGEXT 555
#define GLNAMEDRENDERBUFFERSTORAGEEXT 556
#define GLNAMEDRENDERBUFFERSTORAGEMULTISAMPLECOVERAGEEXT 557
#define GLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEEXT 558
#define GLNORMAL3HNV 559
#define GLNORMAL3HVNV 560
#define GLNORMALPOINTEREXT 561
#define GLPIXELDATARANGENV 562
#define GLPOINTPARAMETERFARB 563
#define GLPOINTPARAMETERFEXT 564
#define GLPOINTPARAMETERFVARB 565
#define GLPOINTPARAMETERFVEXT 566
#define GLPOINTPARAMETERINV 567
#define GLPOINTPARAMETERIV 568
#define GLPOINTPARAMETERIVNV 569
#define GLPRESENTFRAMEDUALFILLNV 570
#define GLPRESENTFRAMEKEYEDNV 571
#define GLPRIMITIVERESTARTNV 572
#define GLPRIORITIZETEXTURESEXT 573
#define GLPROGRAMBUFFERPARAMETERSIIVNV 574
#define GLPROGRAMBUFFERPARAMETERSIUIVNV 575
#define GLPROGRAMENVPARAMETER4DARB 576
#define GLPROGRAMENVPARAMETER4DVARB 577
#define GLPROGRAMENVPARAMETERI4INV 578
#define GLPROGRAMENVPARAMETERI4IVNV 579
#define GLPROGRAMENVPARAMETERI4UINV 580
#define GLPROGRAMENVPARAMETERI4UIVNV 581
#define GLPROGRAMENVPARAMETERSI4IVNV 582
#define GLPROGRAMENVPARAMETERSI4UIVNV 583
#define GLPROGRAMLOCALPARAMETER4DARB 584
#define GLPROGRAMLOCALPARAMETER4DVARB 585
#define GLPROGRAMLOCALPARAMETERI4INV 586
#define GLPROGRAMLOCALPARAMETERI4IVNV 587
#define GLPROGRAMLOCALPARAMETERI4UINV 588
#define GLPROGRAMLOCALPARAMETERI4UIVNV 589
#define GLPROGRAMLOCALPARAMETERSI4IVNV 590
#define GLPROGRAMLOCALPARAMETERSI4UIVNV 591
#define GLPROGRAMNAMEDPARAMETER4DNV 592
#define GLPROGRAMNAMEDPARAMETER4DVNV 593
#define GLPROGRAMNAMEDPARAMETER4FNV 594
#define GLPROGRAMNAMEDPARAMETER4FVNV 595
#define GLPROGRAMPARAMETER4DNV 596
#define GLPROGRAMPARAMETER4DVNV 597
#define GLPROGRAMPARAMETER4FNV 598
#define GLPROGRAMPARAMETER4FVNV 599
#define GLPROGRAMPARAMETERIARB 600
#define GLPROGRAMPARAMETERIEXT 601
#define GLPROGRAMPARAMETERS4DVNV 602
#define GLPROGRAMPARAMETERS4FVNV 603
#define GLPROGRAMSTRINGARB 604
#define GLPROGRAMUNIFORM1FEXT 605
#define GLPROGRAMUNIFORM1FVEXT 606
#define GLPROGRAMUNIFORM1IEXT 607
#define GLPROGRAMUNIFORM1IVEXT 608
#define GLPROGRAMUNIFORM1UIEXT 609
#define GLPROGRAMUNIFORM1UIVEXT 610
#define GLPROGRAMUNIFORM2FEXT 611
#define GLPROGRAMUNIFORM2FVEXT 612
#define GLPROGRAMUNIFORM2IEXT 613
#define GLPROGRAMUNIFORM2IVEXT 614
#define GLPROGRAMUNIFORM2UIEXT 615
#define GLPROGRAMUNIFORM2UIVEXT 616
#define GLPROGRAMUNIFORM3FEXT 617
#define GLPROGRAMUNIFORM3FVEXT 618
#define GLPROGRAMUNIFORM3IEXT 619
#define GLPROGRAMUNIFORM3IVEXT 620
#define GLPROGRAMUNIFORM3UIEXT 621
#define GLPROGRAMUNIFORM3UIVEXT 622
#define GLPROGRAMUNIFORM4FEXT 623
#define GLPROGRAMUNIFORM4FVEXT 624
#define GLPROGRAMUNIFORM4IEXT 625
#define GLPROGRAMUNIFORM4IVEXT 626
#define GLPROGRAMUNIFORM4UIEXT 627
#define GLPROGRAMUNIFORM4UIVEXT 628
#define GLPROGRAMUNIFORMMATRIX2FVEXT 629
#define GLPROGRAMUNIFORMMATRIX2X3FVEXT 630
#define GLPROGRAMUNIFORMMATRIX2X4FVEXT 631
#define GLPROGRAMUNIFORMMATRIX3FVEXT 632
#define GLPROGRAMUNIFORMMATRIX3X2FVEXT 633
#define GLPROGRAMUNIFORMMATRIX3X4FVEXT 634
#define GLPROGRAMUNIFORMMATRIX4FVEXT 635
#define GLPROGRAMUNIFORMMATRIX4X2FVEXT 636
#define GLPROGRAMUNIFORMMATRIX4X3FVEXT 637
#define GLPROGRAMVERTEXLIMITNV 638
#define GLPUSHCLIENTATTRIBDEFAULTEXT 639
#define GLQUERYCOUNTER 640
#define GLRENDERBUFFERSTORAGE 641
#define GLRENDERBUFFERSTORAGEMULTISAMPLE 642
#define GLRENDERBUFFERSTORAGEMULTISAMPLEEXT 643
#define GLREQUESTRESIDENTPROGRAMSNV 644
#define GLRESETHISTOGRAM 645
#define GLRESETMINMAX 646
#define GLSAMPLECOVERAGE 647
#define GLSAMPLECOVERAGEARB 648
#define GLSAMPLEMASKINDEXEDNV 649
#define GLSAMPLERPARAMETERF 650
#define GLSAMPLERPARAMETERFV 651
#define GLSAMPLERPARAMETERI 652
#define GLSECONDARYCOLOR3B 653
#define GLSECONDARYCOLOR3BEXT 654
#define GLSECONDARYCOLOR3BV 655
#define GLSECONDARYCOLOR3BVEXT 656
#define GLSECONDARYCOLOR3D 657
#define GLSECONDARYCOLOR3DEXT 658
#define GLSECONDARYCOLOR3DV 659
#define GLSECONDARYCOLOR3DVEXT 660
#define GLSECONDARYCOLOR3F 661
#define GLSECONDARYCOLOR3FEXT 662
#define GLSECONDARYCOLOR3FV 663
#define GLSECONDARYCOLOR3FVEXT 664
#define GLSECONDARYCOLOR3HNV 665
#define GLSECONDARYCOLOR3HVNV 666
#define GLSECONDARYCOLOR3I 667
#define GLSECONDARYCOLOR3IEXT 668
#define GLSECONDARYCOLOR3IV 669
#define GLSECONDARYCOLOR3IVEXT 670
#define GLSECONDARYCOLOR3S 671
#define GLSECONDARYCOLOR3SEXT 672
#define GLSECONDARYCOLOR3SV 673
#define GLSECONDARYCOLOR3SVEXT 674
#define GLSECONDARYCOLOR3UB 675
#define GLSECONDARYCOLOR3UBEXT 676
#define GLSECONDARYCOLOR3UBV 677
#define GLSECONDARYCOLOR3UBVEXT 678
#define GLSECONDARYCOLOR3UI 679
#define GLSECONDARYCOLOR3UIEXT 680
#define GLSECONDARYCOLOR3UIV 681
#define GLSECONDARYCOLOR3UIVEXT 682
#define GLSECONDARYCOLOR3US 683
#define GLSECONDARYCOLOR3USEXT 684
#define GLSECONDARYCOLOR3USV 685
#define GLSECONDARYCOLOR3USVEXT 686
#define GLSECONDARYCOLORPOINTER 687
#define GLSECONDARYCOLORPOINTEREXT 688
#define GLSEPARABLEFILTER2D 689
#define GLSETFENCENV 690
#define GLSHADERSOURCE 691
#define GLSHADERSOURCEARB 692
#define GLSTENCILFUNCSEPARATE 693
#define GLSTENCILMASKSEPARATE 694
#define GLSTENCILOPSEPARATE 695
#define GLTESTFENCENV 696
#define GLTEXBUFFERARB 697
#define GLTEXBUFFEREXT 698
#define GLTEXCOORD1HNV 699
#define GLTEXCOORD1HVNV 700
#define GLTEXCOORD2HNV 701
#define GLTEXCOORD2HVNV 702
#define GLTEXCOORD3HNV 703
#define GLTEXCOORD3HVNV 704
#define GLTEXCOORD4HNV 705
#define GLTEXCOORD4HVNV 706
#define GLTEXCOORDPOINTEREXT 707
#define GLTEXIMAGE3D 708
#define GLTEXIMAGE3DEXT 709
#define GLTEXPARAMETERIIV 710
#define GLTEXPARAMETERIIVEXT 711
#define GLTEXPARAMETERIUIV 712
#define GLTEXPARAMETERIUIVEXT 713
#define GLTEXRENDERBUFFERNV 714
#define GLTEXSTORAGE2D 715
#define GLTEXSUBIMAGE3D 716
#define GLTEXSUBIMAGE3DEXT 717
#define GLTEXTUREBARRIERNV 718
#define GLTEXTUREBUFFEREXT 719
#define GLTEXTUREIMAGE1DEXT 720
#define GLTEXTUREIMAGE2DEXT 721
#define GLTEXTUREIMAGE3DEXT 722
#define GLTEXTUREPARAMETERFEXT 723
#define GLTEXTUREPARAMETERFVEXT 724
#define GLTEXTUREPARAMETERIEXT 725
#define GLTEXTUREPARAMETERIIVEXT 726
#define GLTEXTUREPARAMETERIUIVEXT 727
#define GLTEXTUREPARAMETERIVEXT 728
#define GLTEXTURERENDERBUFFEREXT 729
#define GLTEXTURESTORAGE2DEXT 730
#define GLTEXTURESUBIMAGE1DEXT 731
#define GLTEXTURESUBIMAGE2DEXT 732
#define GLTEXTURESUBIMAGE3DEXT 733
#define GLTRACKMATRIXNV 734
#define GLTRANSFORMFEEDBACKATTRIBSNV 735
#define GLTRANSFORMFEEDBACKVARYINGS 736
#define GLTRANSFORMFEEDBACKVARYINGSNV 737
#define GLUNIFORM1F 738
#define GLUNIFORM1FARB 739
#define GLUNIFORM1FV 740
#define GLUNIFORM1FVARB 741
#define GLUNIFORM1I 742
#define GLUNIFORM1IARB 743
#define GLUNIFORM1IV 744
#define GLUNIFORM1IVARB 745
#define GLUNIFORM1UI 746
#define GLUNIFORM1UIEXT 747
#define GLUNIFORM1UIV 748
#define GLUNIFORM1UIVEXT 749
#define GLUNIFORM2F 750
#define GLUNIFORM2FARB 751
#define GLUNIFORM2FV 752
#define GLUNIFORM2FVARB 753
#define GLUNIFORM2I 754
#define GLUNIFORM2IARB 755
#define GLUNIFORM2IV 756
#define GLUNIFORM2IVARB 757
#define GLUNIFORM2UI 758
#define GLUNIFORM2UIEXT 759
#define GLUNIFORM2UIV 760
#define GLUNIFORM2UIVEXT 761
#define GLUNIFORM3F 762
#define GLUNIFORM3FARB 763
#define GLUNIFORM3FV 764
#define GLUNIFORM3FVARB 765
#define GLUNIFORM3I 766
#define GLUNIFORM3IARB 767
#define GLUNIFORM3IV 768
#define GLUNIFORM3IVARB 769
#define GLUNIFORM3UI 770
#define GLUNIFORM3UIEXT 771
#define GLUNIFORM3UIV 772
#define GLUNIFORM3UIVEXT 773
#define GLUNIFORM4F 774
#define GLUNIFORM4FARB 775
#define GLUNIFORM4FV 776
#define GLUNIFORM4FVARB 777
#define GLUNIFORM4I 778
#define GLUNIFORM4IARB 779
#define GLUNIFORM4IV 780
#define GLUNIFORM4IVARB 781
#define GLUNIFORM4UI 782
#define GLUNIFORM4UIEXT 783
#define GLUNIFORM4UIV 784
#define GLUNIFORM4UIVEXT 785
#define GLUNIFORMBUFFEREXT 786
#define GLUNIFORMMATRIX2FV 787
#define GLUNIFORMMATRIX2FVARB 788
#define GLUNIFORMMATRIX2X3FV 789
#define GLUNIFORMMATRIX2X4FV 790
#define GLUNIFORMMATRIX3FV 791
#define GLUNIFORMMATRIX3FVARB 792
#define GLUNIFORMMATRIX3X2FV 793
#define GLUNIFORMMATRIX3X4FV 794
#define GLUNIFORMMATRIX4FV 795
#define GLUNIFORMMATRIX4FVARB 796
#define GLUNIFORMMATRIX4X2FV 797
#define GLUNIFORMMATRIX4X3FV 798
#define GLUNLOCKARRAYSEXT 799
#define GLUNMAPBUFFER 800
#define GLUNMAPBUFFERARB 801
#define GLUSEPROGRAMOBJECTARB 802
#define GLVALIDATEPROGRAM 803
#define GLVALIDATEPROGRAMARB 804
#define GLVERTEX2HNV 805
#define GLVERTEX2HVNV 806
#define GLVERTEX3HNV 807
#define GLVERTEX3HVNV 808
#define GLVERTEX4HNV 809
#define GLVERTEX4HVNV 810
#define GLVERTEXARRAYRANGENV 811
#define GLVERTEXATTRIB1D 812
#define GLVERTEXATTRIB1DARB 813
#define GLVERTEXATTRIB1DNV 814
#define GLVERTEXATTRIB1DV 815
#define GLVERTEXATTRIB1DVARB 816
#define GLVERTEXATTRIB1DVNV 817
#define GLVERTEXATTRIB1F 818
#define GLVERTEXATTRIB1FARB 819
#define GLVERTEXATTRIB1FNV 820
#define GLVERTEXATTRIB1FV 821
#define GLVERTEXATTRIB1FVARB 822
#define GLVERTEXATTRIB1FVNV 823
#define GLVERTEXATTRIB1HNV 824
#define GLVERTEXATTRIB1HVNV 825
#define GLVERTEXATTRIB1S 826
#define GLVERTEXATTRIB1SARB 827
#define GLVERTEXATTRIB1SNV 828
#define GLVERTEXATTRIB1SV 829
#define GLVERTEXATTRIB1SVARB 830
#define GLVERTEXATTRIB1SVNV 831
#define GLVERTEXATTRIB2D 832
#define GLVERTEXATTRIB2DARB 833
#define GLVERTEXATTRIB2DNV 834
#define GLVERTEXATTRIB2DV 835
#define GLVERTEXATTRIB2DVARB 836
#define GLVERTEXATTRIB2DVNV 837
#define GLVERTEXATTRIB2F 838
#define GLVERTEXATTRIB2FARB 839
#define GLVERTEXATTRIB2FNV 840
#define GLVERTEXATTRIB2FV 841
#define GLVERTEXATTRIB2FVARB 842
#define GLVERTEXATTRIB2FVNV 843
#define GLVERTEXATTRIB2HNV 844
#define GLVERTEXATTRIB2HVNV 845
#define GLVERTEXATTRIB2S 846
#define GLVERTEXATTRIB2SARB 847
#define GLVERTEXATTRIB2SNV 848
#define GLVERTEXATTRIB2SV 849
#define GLVERTEXATTRIB2SVARB 850
#define GLVERTEXATTRIB2SVNV 851
#define GLVERTEXATTRIB3D 852
#define GLVERTEXATTRIB3DARB 853
#define GLVERTEXATTRIB3DNV 854
#define GLVERTEXATTRIB3DV 855
#define GLVERTEXATTRIB3DVARB 856
#define GLVERTEXATTRIB3DVNV 857
#define GLVERTEXATTRIB3F 858
#define GLVERTEXATTRIB3FARB 859
#define GLVERTEXATTRIB3FNV 860
#define GLVERTEXATTRIB3FV 861
#define GLVERTEXATTRIB3FVARB 862
#define GLVERTEXATTRIB3FVNV 863
#define GLVERTEXATTRIB3HNV 864
#define GLVERTEXATTRIB3HVNV 865
#define GLVERTEXATTRIB3S 866
#define GLVERTEXATTRIB3SARB 867
#define GLVERTEXATTRIB3SNV 868
#define GLVERTEXATTRIB3SV 869
#define GLVERTEXATTRIB3SVARB 870
#define GLVERTEXATTRIB3SVNV 871
#define GLVERTEXATTRIB4BV 872
#define GLVERTEXATTRIB4BVARB 873
#define GLVERTEXATTRIB4D 874
#define GLVERTEXATTRIB4DARB 875
#define GLVERTEXATTRIB4DNV 876
#define GLVERTEXATTRIB4DV 877
#define GLVERTEXATTRIB4DVARB 878
#define GLVERTEXATTRIB4DVNV 879
#define GLVERTEXATTRIB4F 880
#define GLVERTEXATTRIB4FARB 881
#define GLVERTEXATTRIB4FNV 882
#define GLVERTEXATTRIB4FV 883
#define GLVERTEXATTRIB4FVARB 884
#define GLVERTEXATTRIB4FVNV 885
#define GLVERTEXATTRIB4HNV 886
#define GLVERTEXATTRIB4HVNV 887
#define GLVERTEXATTRIB4IV 888
#define GLVERTEXATTRIB4IVARB 889
#define GLVERTEXATTRIB4NBV 890
#define GLVERTEXATTRIB4NBVARB 891
#define GLVERTEXATTRIB4NIV 892
#define GLVERTEXATTRIB4NIVARB 893
#define GLVERTEXATTRIB4NSV 894
#define GLVERTEXATTRIB4NSVARB 895
#define GLVERTEXATTRIB4NUB 896
#define GLVERTEXATTRIB4NUBARB 897
#define GLVERTEXATTRIB4NUBV 898
#define GLVERTEXATTRIB4NUBVARB 899
#define GLVERTEXATTRIB4NUIV 900
#define GLVERTEXATTRIB4NUIVARB 901
#define GLVERTEXATTRIB4NUSV 902
#define GLVERTEXATTRIB4NUSVARB 903
#define GLVERTEXATTRIB4S 904
#define GLVERTEXATTRIB4SARB 905
#define GLVERTEXATTRIB4SNV 906
#define GLVERTEXATTRIB4SV 907
#define GLVERTEXATTRIB4SVARB 908
#define GLVERTEXATTRIB4SVNV 909
#define GLVERTEXATTRIB4UBNV 910
#define GLVERTEXATTRIB4UBV 911
#define GLVERTEXATTRIB4UBVARB 912
#define GLVERTEXATTRIB4UBVNV 913
#define GLVERTEXATTRIB4UIV 914
#define GLVERTEXATTRIB4UIVARB 915
#define GLVERTEXATTRIB4USV 916
#define GLVERTEXATTRIB4USVARB 917
#define GLVERTEXATTRIBI1I 918
#define GLVERTEXATTRIBI1IEXT 919
#define GLVERTEXATTRIBI1IV 920
#define GLVERTEXATTRIBI1IVEXT 921
#define GLVERTEXATTRIBI1UI 922
#define GLVERTEXATTRIBI1UIEXT 923
#define GLVERTEXATTRIBI1UIV 924
#define GLVERTEXATTRIBI1UIVEXT 925
#define GLVERTEXATTRIBI2I 926
#define GLVERTEXATTRIBI2IEXT 927
#define GLVERTEXATTRIBI2IV 928
#define GLVERTEXATTRIBI2IVEXT 929
#define GLVERTEXATTRIBI2UI 930
#define GLVERTEXATTRIBI2UIEXT 931
#define GLVERTEXATTRIBI2UIV 932
#define GLVERTEXATTRIBI2UIVEXT 933
#define GLVERTEXATTRIBI3I 934
#define GLVERTEXATTRIBI3IEXT 935
#define GLVERTEXATTRIBI3IV 936
#define GLVERTEXATTRIBI3IVEXT 937
#define GLVERTEXATTRIBI3UI 938
#define GLVERTEXATTRIBI3UIEXT 939
#define GLVERTEXATTRIBI3UIV 940
#define GLVERTEXATTRIBI3UIVEXT 941
#define GLVERTEXATTRIBI4BV 942
#define GLVERTEXATTRIBI4BVEXT 943
#define GLVERTEXATTRIBI4I 944
#define GLVERTEXATTRIBI4IEXT 945
#define GLVERTEXATTRIBI4IV 946
#define GLVERTEXATTRIBI4IVEXT 947
#define GLVERTEXATTRIBI4SV 948
#define GLVERTEXATTRIBI4SVEXT 949
#define GLVERTEXATTRIBI4UBV 950
#define GLVERTEXATTRIBI4UBVEXT 951
#define GLVERTEXATTRIBI4UI 952
#define GLVERTEXATTRIBI4UIEXT 953
#define GLVERTEXATTRIBI4UIV 954
#define GLVERTEXATTRIBI4UIVEXT 955
#define GLVERTEXATTRIBI4USV 956
#define GLVERTEXATTRIBI4USVEXT 957
#define GLVERTEXATTRIBIPOINTER 958
#define GLVERTEXATTRIBIPOINTEREXT 959
#define GLVERTEXATTRIBPOINTER 960
#define GLVERTEXATTRIBPOINTERNV 961
#define GLVERTEXATTRIBS1DVNV 962
#define GLVERTEXATTRIBS1FVNV 963
#define GLVERTEXATTRIBS1HVNV 964
#define GLVERTEXATTRIBS1SVNV 965
#define GLVERTEXATTRIBS2DVNV 966
#define GLVERTEXATTRIBS2FVNV 967
#define GLVERTEXATTRIBS2HVNV 968
#define GLVERTEXATTRIBS2SVNV 969
#define GLVERTEXATTRIBS3DVNV 970
#define GLVERTEXATTRIBS3FVNV 971
#define GLVERTEXATTRIBS3HVNV 972
#define GLVERTEXATTRIBS3SVNV 973
#define GLVERTEXATTRIBS4DVNV 974
#define GLVERTEXATTRIBS4FVNV 975
#define GLVERTEXATTRIBS4HVNV 976
#define GLVERTEXATTRIBS4SVNV 977
#define GLVERTEXATTRIBS4UBVNV 978
#define GLVERTEXPOINTEREXT 979
#define GLWINDOWPOS2D 980
#define GLWINDOWPOS2DARB 981
#define GLWINDOWPOS2DV 982
#define GLWINDOWPOS2DVARB 983
#define GLWINDOWPOS2F 984
#define GLWINDOWPOS2FARB 985
#define GLWINDOWPOS2FV 986
#define GLWINDOWPOS2FVARB 987
#define GLWINDOWPOS2I 988
#define GLWINDOWPOS2IARB 989
#define GLWINDOWPOS2IV 990
#define GLWINDOWPOS2IVARB 991
#define GLWINDOWPOS2S 992
#define GLWINDOWPOS2SARB 993
#define GLWINDOWPOS2SV 994
#define GLWINDOWPOS2SVARB 995
#define GLWINDOWPOS3D 996
#define GLWINDOWPOS3DARB 997
#define GLWINDOWPOS3DV 998
#define GLWINDOWPOS3DVARB 999
#define GLWINDOWPOS3F 1000
#define GLWINDOWPOS3FARB 1001
#define GLWINDOWPOS3FV 1002
#define GLWINDOWPOS3FVARB 1003
#define GLWINDOWPOS3I 1004
#define GLWINDOWPOS3IARB 1005
#define GLWINDOWPOS3IV 1006
#define GLWINDOWPOS3IVARB 1007
#define GLWINDOWPOS3S 1008
#define GLWINDOWPOS3SARB 1009
#define GLWINDOWPOS3SV 1010
#define GLWINDOWPOS3SVARB 1011
#define WGLALLOCATEMEMORYNV 1012
#define WGLBINDSWAPBARRIERNV 1013
#define WGLBINDTEXIMAGEARB 1014
#define WGLBINDVIDEOIMAGENV 1015
#define WGLCHOOSEPIXELFORMATARB 1016
#define WGLCREATEBUFFERREGIONARB 1017
#define WGLCREATECONTEXTATTRIBSARB 1018
#define WGLCREATEPBUFFERARB 1019
#define WGLDELETEBUFFERREGIONARB 1020
#define WGLDESTROYPBUFFERARB 1021
#define WGLDXCLOSEDEVICENV 1022
#define WGLDXLOCKOBJECTSNV 1023
#define WGLDXOPENDEVICENV 1024
#define WGLDXREGISTEROBJECTNV 1025
#define WGLDXSETRESOURCESHAREHANDLENV 1026
#define WGLDXUNLOCKOBJECTSNV 1027
#define WGLDXUNREGISTEROBJECTNV 1028
#define WGLFREEMEMORYNV 1029
#define WGLGETCURRENTCONTEXT 1030
#define WGLGETCURRENTREADDCARB 1031
#define WGLGETEXTENSIONSSTRINGARB 1032
#define WGLGETEXTENSIONSSTRINGEXT 1033
#define WGLGETPBUFFERDCARB 1034
#define WGLGETPIXELFORMATATTRIBFVARB 1035
#define WGLGETPIXELFORMATATTRIBIVARB 1036
#define WGLGETPROCADDRESS 1037
#define WGLGETSWAPINTERVALEXT 1038
#define WGLGETVIDEODEVICENV 1039
#define WGLGETVIDEOINFONV 1040
#define WGLJOINSWAPGROUPNV 1041
#define WGLMAKECONTEXTCURRENTARB 1042
#define WGLQUERYFRAMECOUNTNV 1043
#define WGLQUERYMAXSWAPGROUPSNV 1044
#define WGLQUERYPBUFFERARB 1045
#define WGLQUERYSWAPGROUPNV 1046
#define WGLRELEASEPBUFFERDCARB 1047
#define WGLRELEASETEXIMAGEARB 1048
#define WGLRELEASEVIDEODEVICENV 1049
#define WGLRELEASEVIDEOIMAGENV 1050
#define WGLRESETFRAMECOUNTNV 1051
#define WGLRESTOREBUFFERREGIONARB 1052
#define WGLSAVEBUFFERREGIONARB 1053
#define WGLSENDPBUFFERTOVIDEONV 1054
#define WGLSETPBUFFERATTRIBARB 1055
#define WGLSWAPINTERVALEXT 1056

#define GLFINISHGLUT 0
#define GLVIEWPORTGLUT 1
#define WGLDELETECONTEXTGLUT 2
#define WGLGETCURRENTCONTEXTGLUT 3
#define WGLGETCURRENTDCGLUT 4
#define WGLGETPROCADDRESSGLUT 5
#define WGLMAKECURRENTGLUT 6

extern gl_func_struct gl_addr_dll[9];
extern gl_func_struct gl_addr[1057];
extern gl_func_struct gl_addr_glut[7];

#define glBlendFuncDLL ((PFNGLBLENDFUNCDLLPROC)gl_addr_dll[GLBLENDFUNCDLL].func)
#define glColorMaskDLL ((PFNGLCOLORMASKDLLPROC)gl_addr_dll[GLCOLORMASKDLL].func)
#define glDeleteTexturesDLL ((PFNGLDELETETEXTURESDLLPROC)gl_addr_dll[GLDELETETEXTURESDLL].func)
#define glDepthMaskDLL ((PFNGLDEPTHMASKDLLPROC)gl_addr_dll[GLDEPTHMASKDLL].func)
#define glFinishDLL ((PFNGLFINISHDLLPROC)gl_addr_dll[GLFINISHDLL].func)
#define glGenTexturesDLL ((PFNGLGENTEXTURESDLLPROC)gl_addr_dll[GLGENTEXTURESDLL].func)
#define glScissorDLL ((PFNGLSCISSORDLLPROC)gl_addr_dll[GLSCISSORDLL].func)
#define glViewportDLL ((PFNGLVIEWPORTDLLPROC)gl_addr_dll[GLVIEWPORTDLL].func)
#define wglGetProcAddressDLL ((PFNWGLGETPROCADDRESSDLLPROC)gl_addr_dll[WGLGETPROCADDRESSDLL].func)

#define glActiveStencilFaceEXT ((PFNGLACTIVESTENCILFACEEXTPROC)gl_addr[GLACTIVESTENCILFACEEXT].func)
#define glActiveTextureARB ((PFNGLACTIVETEXTUREARBPROC)gl_addr[GLACTIVETEXTUREARB].func)
#define glActiveVaryingNV ((PFNGLACTIVEVARYINGNVPROC)gl_addr[GLACTIVEVARYINGNV].func)
#define glAreProgramsResidentNV ((PFNGLAREPROGRAMSRESIDENTNVPROC)gl_addr[GLAREPROGRAMSRESIDENTNV].func)
#define glAreTexturesResidentEXT ((PFNGLARETEXTURESRESIDENTEXTPROC)gl_addr[GLARETEXTURESRESIDENTEXT].func)
#define glArrayElementEXT ((PFNGLARRAYELEMENTEXTPROC)gl_addr[GLARRAYELEMENTEXT].func)
#define glAttachObjectARB ((PFNGLATTACHOBJECTARBPROC)gl_addr[GLATTACHOBJECTARB].func)
#define glAttachShader ((PFNGLATTACHSHADERPROC)gl_addr[GLATTACHSHADER].func)
#define glBeginConditionalRender ((PFNGLBEGINCONDITIONALRENDERPROC)gl_addr[GLBEGINCONDITIONALRENDER].func)
#define glBeginConditionalRenderNV ((PFNGLBEGINCONDITIONALRENDERNVPROC)gl_addr[GLBEGINCONDITIONALRENDERNV].func)
#define glBeginOcclusionQueryNV ((PFNGLBEGINOCCLUSIONQUERYNVPROC)gl_addr[GLBEGINOCCLUSIONQUERYNV].func)
#define glBeginQuery ((PFNGLBEGINQUERYPROC)gl_addr[GLBEGINQUERY].func)
#define glBeginQueryARB ((PFNGLBEGINQUERYARBPROC)gl_addr[GLBEGINQUERYARB].func)
#define glBeginTransformFeedback ((PFNGLBEGINTRANSFORMFEEDBACKPROC)gl_addr[GLBEGINTRANSFORMFEEDBACK].func)
#define glBeginTransformFeedbackNV ((PFNGLBEGINTRANSFORMFEEDBACKNVPROC)gl_addr[GLBEGINTRANSFORMFEEDBACKNV].func)
#define glBindAttribLocation ((PFNGLBINDATTRIBLOCATIONPROC)gl_addr[GLBINDATTRIBLOCATION].func)
#define glBindAttribLocationARB ((PFNGLBINDATTRIBLOCATIONARBPROC)gl_addr[GLBINDATTRIBLOCATIONARB].func)
#define glBindBuffer ((PFNGLBINDBUFFERPROC)gl_addr[GLBINDBUFFER].func)
#define glBindBufferBase ((PFNGLBINDBUFFERBASEPROC)gl_addr[GLBINDBUFFERBASE].func)
#define glBindBufferOffsetNV ((PFNGLBINDBUFFEROFFSETNVPROC)gl_addr[GLBINDBUFFEROFFSETNV].func)
#define glBindBufferRange ((PFNGLBINDBUFFERRANGEPROC)gl_addr[GLBINDBUFFERRANGE].func)
#define glBindBufferRangeNV ((PFNGLBINDBUFFERRANGENVPROC)gl_addr[GLBINDBUFFERRANGENV].func)
#define glBindFragDataLocation ((PFNGLBINDFRAGDATALOCATIONPROC)gl_addr[GLBINDFRAGDATALOCATION].func)
#define glBindFragDataLocationEXT ((PFNGLBINDFRAGDATALOCATIONEXTPROC)gl_addr[GLBINDFRAGDATALOCATIONEXT].func)
#define glBindFramebuffer ((PFNGLBINDFRAMEBUFFERPROC)gl_addr[GLBINDFRAMEBUFFER].func)
#define glBindProgramARB ((PFNGLBINDPROGRAMARBPROC)gl_addr[GLBINDPROGRAMARB].func)
#define glBindProgramNV ((PFNGLBINDPROGRAMNVPROC)gl_addr[GLBINDPROGRAMNV].func)
#define glBindRenderbuffer ((PFNGLBINDRENDERBUFFERPROC)gl_addr[GLBINDRENDERBUFFER].func)
#define glBindSampler ((PFNGLBINDSAMPLERPROC)gl_addr[GLBINDSAMPLER].func)
#define glBindTextureEXT ((PFNGLBINDTEXTUREEXTPROC)gl_addr[GLBINDTEXTUREEXT].func)
#define glBindVertexArray ((PFNGLBINDVERTEXARRAYPROC)gl_addr[GLBINDVERTEXARRAY].func)
#define glBlendColor ((PFNGLBLENDCOLORPROC)gl_addr[GLBLENDCOLOR].func)
#define glBlendColorEXT ((PFNGLBLENDCOLOREXTPROC)gl_addr[GLBLENDCOLOREXT].func)
#define glBlendEquation ((PFNGLBLENDEQUATIONPROC)gl_addr[GLBLENDEQUATION].func)
#define glBlendEquationEXT ((PFNGLBLENDEQUATIONEXTPROC)gl_addr[GLBLENDEQUATIONEXT].func)
#define glBlendEquationSeparate ((PFNGLBLENDEQUATIONSEPARATEPROC)gl_addr[GLBLENDEQUATIONSEPARATE].func)
#define glBlendEquationSeparateEXT ((PFNGLBLENDEQUATIONSEPARATEEXTPROC)gl_addr[GLBLENDEQUATIONSEPARATEEXT].func)
#define glBlendFuncSeparate ((PFNGLBLENDFUNCSEPARATEPROC)gl_addr[GLBLENDFUNCSEPARATE].func)
#define glBlendFuncSeparateEXT ((PFNGLBLENDFUNCSEPARATEEXTPROC)gl_addr[GLBLENDFUNCSEPARATEEXT].func)
#define glBlitFramebuffer ((PFNGLBLITFRAMEBUFFERPROC)gl_addr[GLBLITFRAMEBUFFER].func)
#define glBlitFramebufferEXT ((PFNGLBLITFRAMEBUFFEREXTPROC)gl_addr[GLBLITFRAMEBUFFEREXT].func)
#define glBufferData ((PFNGLBUFFERDATAPROC)gl_addr[GLBUFFERDATA].func)
#define glBufferSubData ((PFNGLBUFFERSUBDATAPROC)gl_addr[GLBUFFERSUBDATA].func)
#define glBufferSubDataARB ((PFNGLBUFFERSUBDATAARBPROC)gl_addr[GLBUFFERSUBDATAARB].func)
#define glCheckFramebufferStatus ((PFNGLCHECKFRAMEBUFFERSTATUSPROC)gl_addr[GLCHECKFRAMEBUFFERSTATUS].func)
#define glCheckFramebufferStatusEXT ((PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)gl_addr[GLCHECKFRAMEBUFFERSTATUSEXT].func)
#define glCheckNamedFramebufferStatusEXT ((PFNGLCHECKNAMEDFRAMEBUFFERSTATUSEXTPROC)gl_addr[GLCHECKNAMEDFRAMEBUFFERSTATUSEXT].func)
#define glClearBufferfi ((PFNGLCLEARBUFFERFIPROC)gl_addr[GLCLEARBUFFERFI].func)
#define glClearBufferfv ((PFNGLCLEARBUFFERFVPROC)gl_addr[GLCLEARBUFFERFV].func)
#define glClearBufferiv ((PFNGLCLEARBUFFERIVPROC)gl_addr[GLCLEARBUFFERIV].func)
#define glClearBufferuiv ((PFNGLCLEARBUFFERUIVPROC)gl_addr[GLCLEARBUFFERUIV].func)
#define glClearColorIiEXT ((PFNGLCLEARCOLORIIEXTPROC)gl_addr[GLCLEARCOLORIIEXT].func)
#define glClearColorIuiEXT ((PFNGLCLEARCOLORIUIEXTPROC)gl_addr[GLCLEARCOLORIUIEXT].func)
#define glClearDepthdNV ((PFNGLCLEARDEPTHDNVPROC)gl_addr[GLCLEARDEPTHDNV].func)
#define glClientActiveTextureARB ((PFNGLCLIENTACTIVETEXTUREARBPROC)gl_addr[GLCLIENTACTIVETEXTUREARB].func)
#define glClientAttribDefaultEXT ((PFNGLCLIENTATTRIBDEFAULTEXTPROC)gl_addr[GLCLIENTATTRIBDEFAULTEXT].func)
#define glColor3hNV ((PFNGLCOLOR3HNVPROC)gl_addr[GLCOLOR3HNV].func)
#define glColor3hvNV ((PFNGLCOLOR3HVNVPROC)gl_addr[GLCOLOR3HVNV].func)
#define glColor4hNV ((PFNGLCOLOR4HNVPROC)gl_addr[GLCOLOR4HNV].func)
#define glColor4hvNV ((PFNGLCOLOR4HVNVPROC)gl_addr[GLCOLOR4HVNV].func)
#define glColorMaski ((PFNGLCOLORMASKIPROC)gl_addr[GLCOLORMASKI].func)
#define glColorMaskIndexedEXT ((PFNGLCOLORMASKINDEXEDEXTPROC)gl_addr[GLCOLORMASKINDEXEDEXT].func)
#define glColorPointerEXT ((PFNGLCOLORPOINTEREXTPROC)gl_addr[GLCOLORPOINTEREXT].func)
#define glColorSubTable ((PFNGLCOLORSUBTABLEPROC)gl_addr[GLCOLORSUBTABLE].func)
#define glColorSubTableEXT ((PFNGLCOLORSUBTABLEEXTPROC)gl_addr[GLCOLORSUBTABLEEXT].func)
#define glColorTable ((PFNGLCOLORTABLEPROC)gl_addr[GLCOLORTABLE].func)
#define glColorTableEXT ((PFNGLCOLORTABLEEXTPROC)gl_addr[GLCOLORTABLEEXT].func)
#define glColorTableParameterfv ((PFNGLCOLORTABLEPARAMETERFVPROC)gl_addr[GLCOLORTABLEPARAMETERFV].func)
#define glColorTableParameteriv ((PFNGLCOLORTABLEPARAMETERIVPROC)gl_addr[GLCOLORTABLEPARAMETERIV].func)
#define glCombinerInputNV ((PFNGLCOMBINERINPUTNVPROC)gl_addr[GLCOMBINERINPUTNV].func)
#define glCombinerOutputNV ((PFNGLCOMBINEROUTPUTNVPROC)gl_addr[GLCOMBINEROUTPUTNV].func)
#define glCombinerParameterfNV ((PFNGLCOMBINERPARAMETERFNVPROC)gl_addr[GLCOMBINERPARAMETERFNV].func)
#define glCombinerParameterfvNV ((PFNGLCOMBINERPARAMETERFVNVPROC)gl_addr[GLCOMBINERPARAMETERFVNV].func)
#define glCombinerParameteriNV ((PFNGLCOMBINERPARAMETERINVPROC)gl_addr[GLCOMBINERPARAMETERINV].func)
#define glCombinerParameterivNV ((PFNGLCOMBINERPARAMETERIVNVPROC)gl_addr[GLCOMBINERPARAMETERIVNV].func)
#define glCombinerStageParameterfvNV ((PFNGLCOMBINERSTAGEPARAMETERFVNVPROC)gl_addr[GLCOMBINERSTAGEPARAMETERFVNV].func)
#define glCompileShader ((PFNGLCOMPILESHADERPROC)gl_addr[GLCOMPILESHADER].func)
#define glCompileShaderARB ((PFNGLCOMPILESHADERARBPROC)gl_addr[GLCOMPILESHADERARB].func)
#define glCompressedMultiTexImage1DEXT ((PFNGLCOMPRESSEDMULTITEXIMAGE1DEXTPROC)gl_addr[GLCOMPRESSEDMULTITEXIMAGE1DEXT].func)
#define glCompressedMultiTexImage2DEXT ((PFNGLCOMPRESSEDMULTITEXIMAGE2DEXTPROC)gl_addr[GLCOMPRESSEDMULTITEXIMAGE2DEXT].func)
#define glCompressedMultiTexImage3DEXT ((PFNGLCOMPRESSEDMULTITEXIMAGE3DEXTPROC)gl_addr[GLCOMPRESSEDMULTITEXIMAGE3DEXT].func)
#define glCompressedMultiTexSubImage1DEXT ((PFNGLCOMPRESSEDMULTITEXSUBIMAGE1DEXTPROC)gl_addr[GLCOMPRESSEDMULTITEXSUBIMAGE1DEXT].func)
#define glCompressedMultiTexSubImage2DEXT ((PFNGLCOMPRESSEDMULTITEXSUBIMAGE2DEXTPROC)gl_addr[GLCOMPRESSEDMULTITEXSUBIMAGE2DEXT].func)
#define glCompressedMultiTexSubImage3DEXT ((PFNGLCOMPRESSEDMULTITEXSUBIMAGE3DEXTPROC)gl_addr[GLCOMPRESSEDMULTITEXSUBIMAGE3DEXT].func)
#define glCompressedTexImage1D ((PFNGLCOMPRESSEDTEXIMAGE1DPROC)gl_addr[GLCOMPRESSEDTEXIMAGE1D].func)
#define glCompressedTexImage1DARB ((PFNGLCOMPRESSEDTEXIMAGE1DARBPROC)gl_addr[GLCOMPRESSEDTEXIMAGE1DARB].func)
#define glCompressedTexImage2D ((PFNGLCOMPRESSEDTEXIMAGE2DPROC)gl_addr[GLCOMPRESSEDTEXIMAGE2D].func)
#define glCompressedTexImage2DARB ((PFNGLCOMPRESSEDTEXIMAGE2DARBPROC)gl_addr[GLCOMPRESSEDTEXIMAGE2DARB].func)
#define glCompressedTexImage3D ((PFNGLCOMPRESSEDTEXIMAGE3DPROC)gl_addr[GLCOMPRESSEDTEXIMAGE3D].func)
#define glCompressedTexImage3DARB ((PFNGLCOMPRESSEDTEXIMAGE3DARBPROC)gl_addr[GLCOMPRESSEDTEXIMAGE3DARB].func)
#define glCompressedTexSubImage1D ((PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC)gl_addr[GLCOMPRESSEDTEXSUBIMAGE1D].func)
#define glCompressedTexSubImage1DARB ((PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC)gl_addr[GLCOMPRESSEDTEXSUBIMAGE1DARB].func)
#define glCompressedTexSubImage2D ((PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)gl_addr[GLCOMPRESSEDTEXSUBIMAGE2D].func)
#define glCompressedTexSubImage2DARB ((PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC)gl_addr[GLCOMPRESSEDTEXSUBIMAGE2DARB].func)
#define glCompressedTexSubImage3D ((PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC)gl_addr[GLCOMPRESSEDTEXSUBIMAGE3D].func)
#define glCompressedTexSubImage3DARB ((PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC)gl_addr[GLCOMPRESSEDTEXSUBIMAGE3DARB].func)
#define glCompressedTextureImage1DEXT ((PFNGLCOMPRESSEDTEXTUREIMAGE1DEXTPROC)gl_addr[GLCOMPRESSEDTEXTUREIMAGE1DEXT].func)
#define glCompressedTextureImage2DEXT ((PFNGLCOMPRESSEDTEXTUREIMAGE2DEXTPROC)gl_addr[GLCOMPRESSEDTEXTUREIMAGE2DEXT].func)
#define glCompressedTextureImage3DEXT ((PFNGLCOMPRESSEDTEXTUREIMAGE3DEXTPROC)gl_addr[GLCOMPRESSEDTEXTUREIMAGE3DEXT].func)
#define glCompressedTextureSubImage1DEXT ((PFNGLCOMPRESSEDTEXTURESUBIMAGE1DEXTPROC)gl_addr[GLCOMPRESSEDTEXTURESUBIMAGE1DEXT].func)
#define glCompressedTextureSubImage2DEXT ((PFNGLCOMPRESSEDTEXTURESUBIMAGE2DEXTPROC)gl_addr[GLCOMPRESSEDTEXTURESUBIMAGE2DEXT].func)
#define glCompressedTextureSubImage3DEXT ((PFNGLCOMPRESSEDTEXTURESUBIMAGE3DEXTPROC)gl_addr[GLCOMPRESSEDTEXTURESUBIMAGE3DEXT].func)
#define glConvolutionFilter1D ((PFNGLCONVOLUTIONFILTER1DPROC)gl_addr[GLCONVOLUTIONFILTER1D].func)
#define glConvolutionFilter2D ((PFNGLCONVOLUTIONFILTER2DPROC)gl_addr[GLCONVOLUTIONFILTER2D].func)
#define glConvolutionParameterf ((PFNGLCONVOLUTIONPARAMETERFPROC)gl_addr[GLCONVOLUTIONPARAMETERF].func)
#define glConvolutionParameterfv ((PFNGLCONVOLUTIONPARAMETERFVPROC)gl_addr[GLCONVOLUTIONPARAMETERFV].func)
#define glConvolutionParameteri ((PFNGLCONVOLUTIONPARAMETERIPROC)gl_addr[GLCONVOLUTIONPARAMETERI].func)
#define glConvolutionParameteriv ((PFNGLCONVOLUTIONPARAMETERIVPROC)gl_addr[GLCONVOLUTIONPARAMETERIV].func)
#define glCopyColorSubTable ((PFNGLCOPYCOLORSUBTABLEPROC)gl_addr[GLCOPYCOLORSUBTABLE].func)
#define glCopyColorTable ((PFNGLCOPYCOLORTABLEPROC)gl_addr[GLCOPYCOLORTABLE].func)
#define glCopyConvolutionFilter1D ((PFNGLCOPYCONVOLUTIONFILTER1DPROC)gl_addr[GLCOPYCONVOLUTIONFILTER1D].func)
#define glCopyConvolutionFilter2D ((PFNGLCOPYCONVOLUTIONFILTER2DPROC)gl_addr[GLCOPYCONVOLUTIONFILTER2D].func)
#define glCopyImageSubData ((PFNGLCOPYIMAGESUBDATAPROC)gl_addr[GLCOPYIMAGESUBDATA].func)
#define glCopyMultiTexImage1DEXT ((PFNGLCOPYMULTITEXIMAGE1DEXTPROC)gl_addr[GLCOPYMULTITEXIMAGE1DEXT].func)
#define glCopyMultiTexImage2DEXT ((PFNGLCOPYMULTITEXIMAGE2DEXTPROC)gl_addr[GLCOPYMULTITEXIMAGE2DEXT].func)
#define glCopyMultiTexSubImage1DEXT ((PFNGLCOPYMULTITEXSUBIMAGE1DEXTPROC)gl_addr[GLCOPYMULTITEXSUBIMAGE1DEXT].func)
#define glCopyMultiTexSubImage3DEXT ((PFNGLCOPYMULTITEXSUBIMAGE3DEXTPROC)gl_addr[GLCOPYMULTITEXSUBIMAGE3DEXT].func)
#define glCopyTexSubImage3D ((PFNGLCOPYTEXSUBIMAGE3DPROC)gl_addr[GLCOPYTEXSUBIMAGE3D].func)
#define glCopyTexSubImage3DEXT ((PFNGLCOPYTEXSUBIMAGE3DEXTPROC)gl_addr[GLCOPYTEXSUBIMAGE3DEXT].func)
#define glCopyTextureImage1DEXT ((PFNGLCOPYTEXTUREIMAGE1DEXTPROC)gl_addr[GLCOPYTEXTUREIMAGE1DEXT].func)
#define glCopyTextureImage2DEXT ((PFNGLCOPYTEXTUREIMAGE2DEXTPROC)gl_addr[GLCOPYTEXTUREIMAGE2DEXT].func)
#define glCopyTextureSubImage1DEXT ((PFNGLCOPYTEXTURESUBIMAGE1DEXTPROC)gl_addr[GLCOPYTEXTURESUBIMAGE1DEXT].func)
#define glCopyTextureSubImage2DEXT ((PFNGLCOPYTEXTURESUBIMAGE2DEXTPROC)gl_addr[GLCOPYTEXTURESUBIMAGE2DEXT].func)
#define glCopyTextureSubImage3DEXT ((PFNGLCOPYTEXTURESUBIMAGE3DEXTPROC)gl_addr[GLCOPYTEXTURESUBIMAGE3DEXT].func)
#define glCreateProgram ((PFNGLCREATEPROGRAMPROC)gl_addr[GLCREATEPROGRAM].func)
#define glCreateProgramObjectARB ((PFNGLCREATEPROGRAMOBJECTARBPROC)gl_addr[GLCREATEPROGRAMOBJECTARB].func)
#define glCreateShader ((PFNGLCREATESHADERPROC)gl_addr[GLCREATESHADER].func)
#define glCreateShaderObjectARB ((PFNGLCREATESHADEROBJECTARBPROC)gl_addr[GLCREATESHADEROBJECTARB].func)
#define glDeleteBuffers ((PFNGLDELETEBUFFERSPROC)gl_addr[GLDELETEBUFFERS].func)
#define glDeleteFencesNV ((PFNGLDELETEFENCESNVPROC)gl_addr[GLDELETEFENCESNV].func)
#define glDeleteFramebuffers ((PFNGLDELETEFRAMEBUFFERSPROC)gl_addr[GLDELETEFRAMEBUFFERS].func)
#define glDeleteObjectARB ((PFNGLDELETEOBJECTARBPROC)gl_addr[GLDELETEOBJECTARB].func)
#define glDeleteOcclusionQueriesNV ((PFNGLDELETEOCCLUSIONQUERIESNVPROC)gl_addr[GLDELETEOCCLUSIONQUERIESNV].func)
#define glDeleteProgram ((PFNGLDELETEPROGRAMPROC)gl_addr[GLDELETEPROGRAM].func)
#define glDeleteProgramsARB ((PFNGLDELETEPROGRAMSARBPROC)gl_addr[GLDELETEPROGRAMSARB].func)
#define glDeleteProgramsNV ((PFNGLDELETEPROGRAMSNVPROC)gl_addr[GLDELETEPROGRAMSNV].func)
#define glDeleteQueries ((PFNGLDELETEQUERIESPROC)gl_addr[GLDELETEQUERIES].func)
#define glDeleteQueriesARB ((PFNGLDELETEQUERIESARBPROC)gl_addr[GLDELETEQUERIESARB].func)
#define glDeleteRenderbuffers ((PFNGLDELETERENDERBUFFERSPROC)gl_addr[GLDELETERENDERBUFFERS].func)
#define glDeleteSamplers ((PFNGLDELETESAMPLERSPROC)gl_addr[GLDELETESAMPLERS].func)
#define glDeleteShader ((PFNGLDELETESHADERPROC)gl_addr[GLDELETESHADER].func)
#define glDeleteTexturesEXT ((PFNGLDELETETEXTURESEXTPROC)gl_addr[GLDELETETEXTURESEXT].func)
#define glDeleteVertexArrays ((PFNGLDELETEVERTEXARRAYSPROC)gl_addr[GLDELETEVERTEXARRAYS].func)
#define glDepthBoundsdNV ((PFNGLDEPTHBOUNDSDNVPROC)gl_addr[GLDEPTHBOUNDSDNV].func)
#define glDepthBoundsEXT ((PFNGLDEPTHBOUNDSEXTPROC)gl_addr[GLDEPTHBOUNDSEXT].func)
#define glDepthRangedNV ((PFNGLDEPTHRANGEDNVPROC)gl_addr[GLDEPTHRANGEDNV].func)
#define glDetachObjectARB ((PFNGLDETACHOBJECTARBPROC)gl_addr[GLDETACHOBJECTARB].func)
#define glDetachShader ((PFNGLDETACHSHADERPROC)gl_addr[GLDETACHSHADER].func)
#define glDisableClientStateIndexedEXT ((PFNGLDISABLECLIENTSTATEINDEXEDEXTPROC)gl_addr[GLDISABLECLIENTSTATEINDEXEDEXT].func)
#define glDisablei ((PFNGLDISABLEIPROC)gl_addr[GLDISABLEI].func)
#define glDisableIndexedEXT ((PFNGLDISABLEINDEXEDEXTPROC)gl_addr[GLDISABLEINDEXEDEXT].func)
#define glDisableVertexAttribArray ((PFNGLDISABLEVERTEXATTRIBARRAYPROC)gl_addr[GLDISABLEVERTEXATTRIBARRAY].func)
#define glDrawArraysEXT ((PFNGLDRAWARRAYSEXTPROC)gl_addr[GLDRAWARRAYSEXT].func)
#define glDrawArraysInstancedARB ((PFNGLDRAWARRAYSINSTANCEDARBPROC)gl_addr[GLDRAWARRAYSINSTANCEDARB].func)
#define glDrawArraysInstancedEXT ((PFNGLDRAWARRAYSINSTANCEDEXTPROC)gl_addr[GLDRAWARRAYSINSTANCEDEXT].func)
#define glDrawBuffers ((PFNGLDRAWBUFFERSPROC)gl_addr[GLDRAWBUFFERS].func)
#define glDrawBuffersARB ((PFNGLDRAWBUFFERSARBPROC)gl_addr[GLDRAWBUFFERSARB].func)
#define glDrawBuffersATI ((PFNGLDRAWBUFFERSATIPROC)gl_addr[GLDRAWBUFFERSATI].func)
#define glDrawElementsInstancedARB ((PFNGLDRAWELEMENTSINSTANCEDARBPROC)gl_addr[GLDRAWELEMENTSINSTANCEDARB].func)
#define glDrawElementsInstancedEXT ((PFNGLDRAWELEMENTSINSTANCEDEXTPROC)gl_addr[GLDRAWELEMENTSINSTANCEDEXT].func)
#define glDrawRangeElementsEXT ((PFNGLDRAWRANGEELEMENTSEXTPROC)gl_addr[GLDRAWRANGEELEMENTSEXT].func)
#define glEdgeFlagPointerEXT ((PFNGLEDGEFLAGPOINTEREXTPROC)gl_addr[GLEDGEFLAGPOINTEREXT].func)
#define glEnableClientStateIndexedEXT ((PFNGLENABLECLIENTSTATEINDEXEDEXTPROC)gl_addr[GLENABLECLIENTSTATEINDEXEDEXT].func)
#define glEnablei ((PFNGLENABLEIPROC)gl_addr[GLENABLEI].func)
#define glEnableIndexedEXT ((PFNGLENABLEINDEXEDEXTPROC)gl_addr[GLENABLEINDEXEDEXT].func)
#define glEndConditionalRender ((PFNGLENDCONDITIONALRENDERPROC)gl_addr[GLENDCONDITIONALRENDER].func)
#define glEndConditionalRenderNV ((PFNGLENDCONDITIONALRENDERNVPROC)gl_addr[GLENDCONDITIONALRENDERNV].func)
#define glEndOcclusionQueryNV ((PFNGLENDOCCLUSIONQUERYNVPROC)gl_addr[GLENDOCCLUSIONQUERYNV].func)
#define glEndQuery ((PFNGLENDQUERYPROC)gl_addr[GLENDQUERY].func)
#define glEndQueryARB ((PFNGLENDQUERYARBPROC)gl_addr[GLENDQUERYARB].func)
#define glEndTransformFeedback ((PFNGLENDTRANSFORMFEEDBACKPROC)gl_addr[GLENDTRANSFORMFEEDBACK].func)
#define glEndTransformFeedbackNV ((PFNGLENDTRANSFORMFEEDBACKNVPROC)gl_addr[GLENDTRANSFORMFEEDBACKNV].func)
#define glExecuteProgramNV ((PFNGLEXECUTEPROGRAMNVPROC)gl_addr[GLEXECUTEPROGRAMNV].func)
#define glFinalCombinerInputNV ((PFNGLFINALCOMBINERINPUTNVPROC)gl_addr[GLFINALCOMBINERINPUTNV].func)
#define glFinishFenceNV ((PFNGLFINISHFENCENVPROC)gl_addr[GLFINISHFENCENV].func)
#define glFlushMappedBufferRange ((PFNGLFLUSHMAPPEDBUFFERRANGEPROC)gl_addr[GLFLUSHMAPPEDBUFFERRANGE].func)
#define glFlushPixelDataRangeNV ((PFNGLFLUSHPIXELDATARANGENVPROC)gl_addr[GLFLUSHPIXELDATARANGENV].func)
#define glFlushVertexArrayRangeNV ((PFNGLFLUSHVERTEXARRAYRANGENVPROC)gl_addr[GLFLUSHVERTEXARRAYRANGENV].func)
#define glFogCoordd ((PFNGLFOGCOORDDPROC)gl_addr[GLFOGCOORDD].func)
#define glFogCoorddEXT ((PFNGLFOGCOORDDEXTPROC)gl_addr[GLFOGCOORDDEXT].func)
#define glFogCoorddv ((PFNGLFOGCOORDDVPROC)gl_addr[GLFOGCOORDDV].func)
#define glFogCoorddvEXT ((PFNGLFOGCOORDDVEXTPROC)gl_addr[GLFOGCOORDDVEXT].func)
#define glFogCoordf ((PFNGLFOGCOORDFPROC)gl_addr[GLFOGCOORDF].func)
#define glFogCoordfEXT ((PFNGLFOGCOORDFEXTPROC)gl_addr[GLFOGCOORDFEXT].func)
#define glFogCoordfv ((PFNGLFOGCOORDFVPROC)gl_addr[GLFOGCOORDFV].func)
#define glFogCoordfvEXT ((PFNGLFOGCOORDFVEXTPROC)gl_addr[GLFOGCOORDFVEXT].func)
#define glFogCoordhNV ((PFNGLFOGCOORDHNVPROC)gl_addr[GLFOGCOORDHNV].func)
#define glFogCoordhvNV ((PFNGLFOGCOORDHVNVPROC)gl_addr[GLFOGCOORDHVNV].func)
#define glFogCoordPointer ((PFNGLFOGCOORDPOINTERPROC)gl_addr[GLFOGCOORDPOINTER].func)
#define glFogCoordPointerEXT ((PFNGLFOGCOORDPOINTEREXTPROC)gl_addr[GLFOGCOORDPOINTEREXT].func)
#define glFramebufferDrawBufferEXT ((PFNGLFRAMEBUFFERDRAWBUFFEREXTPROC)gl_addr[GLFRAMEBUFFERDRAWBUFFEREXT].func)
#define glFramebufferDrawBuffersEXT ((PFNGLFRAMEBUFFERDRAWBUFFERSEXTPROC)gl_addr[GLFRAMEBUFFERDRAWBUFFERSEXT].func)
#define glFramebufferReadBufferEXT ((PFNGLFRAMEBUFFERREADBUFFEREXTPROC)gl_addr[GLFRAMEBUFFERREADBUFFEREXT].func)
#define glFramebufferRenderbuffer ((PFNGLFRAMEBUFFERRENDERBUFFERPROC)gl_addr[GLFRAMEBUFFERRENDERBUFFER].func)
#define glFramebufferTexture ((PFNGLFRAMEBUFFERTEXTUREPROC)gl_addr[GLFRAMEBUFFERTEXTURE].func)
#define glFramebufferTexture1D ((PFNGLFRAMEBUFFERTEXTURE1DPROC)gl_addr[GLFRAMEBUFFERTEXTURE1D].func)
#define glFramebufferTexture1DEXT ((PFNGLFRAMEBUFFERTEXTURE1DEXTPROC)gl_addr[GLFRAMEBUFFERTEXTURE1DEXT].func)
#define glFramebufferTexture2D ((PFNGLFRAMEBUFFERTEXTURE2DPROC)gl_addr[GLFRAMEBUFFERTEXTURE2D].func)
#define glFramebufferTexture3D ((PFNGLFRAMEBUFFERTEXTURE3DPROC)gl_addr[GLFRAMEBUFFERTEXTURE3D].func)
#define glFramebufferTexture3DEXT ((PFNGLFRAMEBUFFERTEXTURE3DEXTPROC)gl_addr[GLFRAMEBUFFERTEXTURE3DEXT].func)
#define glFramebufferTextureARB ((PFNGLFRAMEBUFFERTEXTUREARBPROC)gl_addr[GLFRAMEBUFFERTEXTUREARB].func)
#define glFramebufferTextureEXT ((PFNGLFRAMEBUFFERTEXTUREEXTPROC)gl_addr[GLFRAMEBUFFERTEXTUREEXT].func)
#define glFramebufferTextureFaceARB ((PFNGLFRAMEBUFFERTEXTUREFACEARBPROC)gl_addr[GLFRAMEBUFFERTEXTUREFACEARB].func)
#define glFramebufferTextureFaceEXT ((PFNGLFRAMEBUFFERTEXTUREFACEEXTPROC)gl_addr[GLFRAMEBUFFERTEXTUREFACEEXT].func)
#define glFramebufferTextureLayer ((PFNGLFRAMEBUFFERTEXTURELAYERPROC)gl_addr[GLFRAMEBUFFERTEXTURELAYER].func)
#define glFramebufferTextureLayerARB ((PFNGLFRAMEBUFFERTEXTURELAYERARBPROC)gl_addr[GLFRAMEBUFFERTEXTURELAYERARB].func)
#define glFramebufferTextureLayerEXT ((PFNGLFRAMEBUFFERTEXTURELAYEREXTPROC)gl_addr[GLFRAMEBUFFERTEXTURELAYEREXT].func)
#define glGenBuffers ((PFNGLGENBUFFERSPROC)gl_addr[GLGENBUFFERS].func)
#define glGenBuffersARB ((PFNGLGENBUFFERSARBPROC)gl_addr[GLGENBUFFERSARB].func)
#define glGenerateMipmap ((PFNGLGENERATEMIPMAPPROC)gl_addr[GLGENERATEMIPMAP].func)
#define glGenerateMipmapEXT ((PFNGLGENERATEMIPMAPEXTPROC)gl_addr[GLGENERATEMIPMAPEXT].func)
#define glGenerateMultiTexMipmapEXT ((PFNGLGENERATEMULTITEXMIPMAPEXTPROC)gl_addr[GLGENERATEMULTITEXMIPMAPEXT].func)
#define glGenerateTextureMipmapEXT ((PFNGLGENERATETEXTUREMIPMAPEXTPROC)gl_addr[GLGENERATETEXTUREMIPMAPEXT].func)
#define glGenFencesNV ((PFNGLGENFENCESNVPROC)gl_addr[GLGENFENCESNV].func)
#define glGenFramebuffers ((PFNGLGENFRAMEBUFFERSPROC)gl_addr[GLGENFRAMEBUFFERS].func)
#define glGenOcclusionQueriesNV ((PFNGLGENOCCLUSIONQUERIESNVPROC)gl_addr[GLGENOCCLUSIONQUERIESNV].func)
#define glGenProgramsARB ((PFNGLGENPROGRAMSARBPROC)gl_addr[GLGENPROGRAMSARB].func)
#define glGenProgramsNV ((PFNGLGENPROGRAMSNVPROC)gl_addr[GLGENPROGRAMSNV].func)
#define glGenQueries ((PFNGLGENQUERIESPROC)gl_addr[GLGENQUERIES].func)
#define glGenQueriesARB ((PFNGLGENQUERIESARBPROC)gl_addr[GLGENQUERIESARB].func)
#define glGenRenderbuffers ((PFNGLGENRENDERBUFFERSPROC)gl_addr[GLGENRENDERBUFFERS].func)
#define glGenSamplers ((PFNGLGENSAMPLERSPROC)gl_addr[GLGENSAMPLERS].func)
#define glGenTexturesEXT ((PFNGLGENTEXTURESEXTPROC)gl_addr[GLGENTEXTURESEXT].func)
#define glGenVertexArrays ((PFNGLGENVERTEXARRAYSPROC)gl_addr[GLGENVERTEXARRAYS].func)
#define glGetActiveAttrib ((PFNGLGETACTIVEATTRIBPROC)gl_addr[GLGETACTIVEATTRIB].func)
#define glGetActiveAttribARB ((PFNGLGETACTIVEATTRIBARBPROC)gl_addr[GLGETACTIVEATTRIBARB].func)
#define glGetActiveUniform ((PFNGLGETACTIVEUNIFORMPROC)gl_addr[GLGETACTIVEUNIFORM].func)
#define glGetActiveUniformARB ((PFNGLGETACTIVEUNIFORMARBPROC)gl_addr[GLGETACTIVEUNIFORMARB].func)
#define glGetActiveVaryingNV ((PFNGLGETACTIVEVARYINGNVPROC)gl_addr[GLGETACTIVEVARYINGNV].func)
#define glGetAttachedObjectsARB ((PFNGLGETATTACHEDOBJECTSARBPROC)gl_addr[GLGETATTACHEDOBJECTSARB].func)
#define glGetAttachedShaders ((PFNGLGETATTACHEDSHADERSPROC)gl_addr[GLGETATTACHEDSHADERS].func)
#define glGetAttribLocation ((PFNGLGETATTRIBLOCATIONPROC)gl_addr[GLGETATTRIBLOCATION].func)
#define glGetAttribLocationARB ((PFNGLGETATTRIBLOCATIONARBPROC)gl_addr[GLGETATTRIBLOCATIONARB].func)
#define glGetBooleani_v ((PFNGLGETBOOLEANI_VPROC)gl_addr[GLGETBOOLEANI_V].func)
#define glGetBooleanIndexedvEXT ((PFNGLGETBOOLEANINDEXEDVEXTPROC)gl_addr[GLGETBOOLEANINDEXEDVEXT].func)
#define glGetBufferParameteriv ((PFNGLGETBUFFERPARAMETERIVPROC)gl_addr[GLGETBUFFERPARAMETERIV].func)
#define glGetBufferParameterivARB ((PFNGLGETBUFFERPARAMETERIVARBPROC)gl_addr[GLGETBUFFERPARAMETERIVARB].func)
#define glGetBufferPointerv ((PFNGLGETBUFFERPOINTERVPROC)gl_addr[GLGETBUFFERPOINTERV].func)
#define glGetBufferPointervARB ((PFNGLGETBUFFERPOINTERVARBPROC)gl_addr[GLGETBUFFERPOINTERVARB].func)
#define glGetBufferSubData ((PFNGLGETBUFFERSUBDATAPROC)gl_addr[GLGETBUFFERSUBDATA].func)
#define glGetBufferSubDataARB ((PFNGLGETBUFFERSUBDATAARBPROC)gl_addr[GLGETBUFFERSUBDATAARB].func)
#define glGetColorTable ((PFNGLGETCOLORTABLEPROC)gl_addr[GLGETCOLORTABLE].func)
#define glGetColorTableEXT ((PFNGLGETCOLORTABLEEXTPROC)gl_addr[GLGETCOLORTABLEEXT].func)
#define glGetColorTableParameterfv ((PFNGLGETCOLORTABLEPARAMETERFVPROC)gl_addr[GLGETCOLORTABLEPARAMETERFV].func)
#define glGetColorTableParameterfvEXT ((PFNGLGETCOLORTABLEPARAMETERFVEXTPROC)gl_addr[GLGETCOLORTABLEPARAMETERFVEXT].func)
#define glGetColorTableParameteriv ((PFNGLGETCOLORTABLEPARAMETERIVPROC)gl_addr[GLGETCOLORTABLEPARAMETERIV].func)
#define glGetColorTableParameterivEXT ((PFNGLGETCOLORTABLEPARAMETERIVEXTPROC)gl_addr[GLGETCOLORTABLEPARAMETERIVEXT].func)
#define glGetCombinerInputParameterfvNV ((PFNGLGETCOMBINERINPUTPARAMETERFVNVPROC)gl_addr[GLGETCOMBINERINPUTPARAMETERFVNV].func)
#define glGetCombinerInputParameterivNV ((PFNGLGETCOMBINERINPUTPARAMETERIVNVPROC)gl_addr[GLGETCOMBINERINPUTPARAMETERIVNV].func)
#define glGetCombinerOutputParameterfvNV ((PFNGLGETCOMBINEROUTPUTPARAMETERFVNVPROC)gl_addr[GLGETCOMBINEROUTPUTPARAMETERFVNV].func)
#define glGetCombinerOutputParameterivNV ((PFNGLGETCOMBINEROUTPUTPARAMETERIVNVPROC)gl_addr[GLGETCOMBINEROUTPUTPARAMETERIVNV].func)
#define glGetCombinerStageParameterfvNV ((PFNGLGETCOMBINERSTAGEPARAMETERFVNVPROC)gl_addr[GLGETCOMBINERSTAGEPARAMETERFVNV].func)
#define glGetCompressedMultiTexImageEXT ((PFNGLGETCOMPRESSEDMULTITEXIMAGEEXTPROC)gl_addr[GLGETCOMPRESSEDMULTITEXIMAGEEXT].func)
#define glGetCompressedTexImage ((PFNGLGETCOMPRESSEDTEXIMAGEPROC)gl_addr[GLGETCOMPRESSEDTEXIMAGE].func)
#define glGetCompressedTexImageARB ((PFNGLGETCOMPRESSEDTEXIMAGEARBPROC)gl_addr[GLGETCOMPRESSEDTEXIMAGEARB].func)
#define glGetCompressedTextureImageEXT ((PFNGLGETCOMPRESSEDTEXTUREIMAGEEXTPROC)gl_addr[GLGETCOMPRESSEDTEXTUREIMAGEEXT].func)
#define glGetConvolutionFilter ((PFNGLGETCONVOLUTIONFILTERPROC)gl_addr[GLGETCONVOLUTIONFILTER].func)
#define glGetConvolutionParameterfv ((PFNGLGETCONVOLUTIONPARAMETERFVPROC)gl_addr[GLGETCONVOLUTIONPARAMETERFV].func)
#define glGetConvolutionParameteriv ((PFNGLGETCONVOLUTIONPARAMETERIVPROC)gl_addr[GLGETCONVOLUTIONPARAMETERIV].func)
#define glGetDoubleIndexedvEXT ((PFNGLGETDOUBLEINDEXEDVEXTPROC)gl_addr[GLGETDOUBLEINDEXEDVEXT].func)
#define glGetFenceivNV ((PFNGLGETFENCEIVNVPROC)gl_addr[GLGETFENCEIVNV].func)
#define glGetFinalCombinerInputParameterfvNV ((PFNGLGETFINALCOMBINERINPUTPARAMETERFVNVPROC)gl_addr[GLGETFINALCOMBINERINPUTPARAMETERFVNV].func)
#define glGetFinalCombinerInputParameterivNV ((PFNGLGETFINALCOMBINERINPUTPARAMETERIVNVPROC)gl_addr[GLGETFINALCOMBINERINPUTPARAMETERIVNV].func)
#define glGetFloatIndexedvEXT ((PFNGLGETFLOATINDEXEDVEXTPROC)gl_addr[GLGETFLOATINDEXEDVEXT].func)
#define glGetFragDataLocation ((PFNGLGETFRAGDATALOCATIONPROC)gl_addr[GLGETFRAGDATALOCATION].func)
#define glGetFragDataLocationEXT ((PFNGLGETFRAGDATALOCATIONEXTPROC)gl_addr[GLGETFRAGDATALOCATIONEXT].func)
#define glGetFramebufferAttachmentParameteriv ((PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)gl_addr[GLGETFRAMEBUFFERATTACHMENTPARAMETERIV].func)
#define glGetFramebufferAttachmentParameterivEXT ((PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC)gl_addr[GLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXT].func)
#define glGetFramebufferParameterivEXT ((PFNGLGETFRAMEBUFFERPARAMETERIVEXTPROC)gl_addr[GLGETFRAMEBUFFERPARAMETERIVEXT].func)
#define glGetHandleARB ((PFNGLGETHANDLEARBPROC)gl_addr[GLGETHANDLEARB].func)
#define glGetHistogram ((PFNGLGETHISTOGRAMPROC)gl_addr[GLGETHISTOGRAM].func)
#define glGetHistogramParameterfv ((PFNGLGETHISTOGRAMPARAMETERFVPROC)gl_addr[GLGETHISTOGRAMPARAMETERFV].func)
#define glGetHistogramParameteriv ((PFNGLGETHISTOGRAMPARAMETERIVPROC)gl_addr[GLGETHISTOGRAMPARAMETERIV].func)
#define glGetInfoLogARB ((PFNGLGETINFOLOGARBPROC)gl_addr[GLGETINFOLOGARB].func)
#define glGetIntegeri_v ((PFNGLGETINTEGERI_VPROC)gl_addr[GLGETINTEGERI_V].func)
#define glGetIntegerIndexedvEXT ((PFNGLGETINTEGERINDEXEDVEXTPROC)gl_addr[GLGETINTEGERINDEXEDVEXT].func)
#define glGetMinmax ((PFNGLGETMINMAXPROC)gl_addr[GLGETMINMAX].func)
#define glGetMinmaxParameterfv ((PFNGLGETMINMAXPARAMETERFVPROC)gl_addr[GLGETMINMAXPARAMETERFV].func)
#define glGetMinmaxParameteriv ((PFNGLGETMINMAXPARAMETERIVPROC)gl_addr[GLGETMINMAXPARAMETERIV].func)
#define glGetMultisamplefvNV ((PFNGLGETMULTISAMPLEFVNVPROC)gl_addr[GLGETMULTISAMPLEFVNV].func)
#define glGetMultiTexEnvfvEXT ((PFNGLGETMULTITEXENVFVEXTPROC)gl_addr[GLGETMULTITEXENVFVEXT].func)
#define glGetMultiTexEnvivEXT ((PFNGLGETMULTITEXENVIVEXTPROC)gl_addr[GLGETMULTITEXENVIVEXT].func)
#define glGetMultiTexGendvEXT ((PFNGLGETMULTITEXGENDVEXTPROC)gl_addr[GLGETMULTITEXGENDVEXT].func)
#define glGetMultiTexGenfvEXT ((PFNGLGETMULTITEXGENFVEXTPROC)gl_addr[GLGETMULTITEXGENFVEXT].func)
#define glGetMultiTexGenivEXT ((PFNGLGETMULTITEXGENIVEXTPROC)gl_addr[GLGETMULTITEXGENIVEXT].func)
#define glGetMultiTexImageEXT ((PFNGLGETMULTITEXIMAGEEXTPROC)gl_addr[GLGETMULTITEXIMAGEEXT].func)
#define glGetMultiTexLevelParameterfvEXT ((PFNGLGETMULTITEXLEVELPARAMETERFVEXTPROC)gl_addr[GLGETMULTITEXLEVELPARAMETERFVEXT].func)
#define glGetMultiTexLevelParameterivEXT ((PFNGLGETMULTITEXLEVELPARAMETERIVEXTPROC)gl_addr[GLGETMULTITEXLEVELPARAMETERIVEXT].func)
#define glGetMultiTexParameterfvEXT ((PFNGLGETMULTITEXPARAMETERFVEXTPROC)gl_addr[GLGETMULTITEXPARAMETERFVEXT].func)
#define glGetMultiTexParameterIivEXT ((PFNGLGETMULTITEXPARAMETERIIVEXTPROC)gl_addr[GLGETMULTITEXPARAMETERIIVEXT].func)
#define glGetMultiTexParameterIuivEXT ((PFNGLGETMULTITEXPARAMETERIUIVEXTPROC)gl_addr[GLGETMULTITEXPARAMETERIUIVEXT].func)
#define glGetMultiTexParameterivEXT ((PFNGLGETMULTITEXPARAMETERIVEXTPROC)gl_addr[GLGETMULTITEXPARAMETERIVEXT].func)
#define glGetNamedBufferParameterivEXT ((PFNGLGETNAMEDBUFFERPARAMETERIVEXTPROC)gl_addr[GLGETNAMEDBUFFERPARAMETERIVEXT].func)
#define glGetNamedBufferPointervEXT ((PFNGLGETNAMEDBUFFERPOINTERVEXTPROC)gl_addr[GLGETNAMEDBUFFERPOINTERVEXT].func)
#define glGetNamedBufferSubDataEXT ((PFNGLGETNAMEDBUFFERSUBDATAEXTPROC)gl_addr[GLGETNAMEDBUFFERSUBDATAEXT].func)
#define glGetNamedFramebufferAttachmentParameterivEXT ((PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC)gl_addr[GLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVEXT].func)
#define glGetNamedProgramivEXT ((PFNGLGETNAMEDPROGRAMIVEXTPROC)gl_addr[GLGETNAMEDPROGRAMIVEXT].func)
#define glGetNamedProgramLocalParameterdvEXT ((PFNGLGETNAMEDPROGRAMLOCALPARAMETERDVEXTPROC)gl_addr[GLGETNAMEDPROGRAMLOCALPARAMETERDVEXT].func)
#define glGetNamedProgramLocalParameterfvEXT ((PFNGLGETNAMEDPROGRAMLOCALPARAMETERFVEXTPROC)gl_addr[GLGETNAMEDPROGRAMLOCALPARAMETERFVEXT].func)
#define glGetNamedProgramLocalParameterIivEXT ((PFNGLGETNAMEDPROGRAMLOCALPARAMETERIIVEXTPROC)gl_addr[GLGETNAMEDPROGRAMLOCALPARAMETERIIVEXT].func)
#define glGetNamedProgramLocalParameterIuivEXT ((PFNGLGETNAMEDPROGRAMLOCALPARAMETERIUIVEXTPROC)gl_addr[GLGETNAMEDPROGRAMLOCALPARAMETERIUIVEXT].func)
#define glGetNamedProgramStringEXT ((PFNGLGETNAMEDPROGRAMSTRINGEXTPROC)gl_addr[GLGETNAMEDPROGRAMSTRINGEXT].func)
#define glGetNamedRenderbufferParameterivEXT ((PFNGLGETNAMEDRENDERBUFFERPARAMETERIVEXTPROC)gl_addr[GLGETNAMEDRENDERBUFFERPARAMETERIVEXT].func)
#define glGetObjectParameterfvARB ((PFNGLGETOBJECTPARAMETERFVARBPROC)gl_addr[GLGETOBJECTPARAMETERFVARB].func)
#define glGetObjectParameterivARB ((PFNGLGETOBJECTPARAMETERIVARBPROC)gl_addr[GLGETOBJECTPARAMETERIVARB].func)
#define glGetOcclusionQueryivNV ((PFNGLGETOCCLUSIONQUERYIVNVPROC)gl_addr[GLGETOCCLUSIONQUERYIVNV].func)
#define glGetOcclusionQueryuivNV ((PFNGLGETOCCLUSIONQUERYUIVNVPROC)gl_addr[GLGETOCCLUSIONQUERYUIVNV].func)
#define glGetPointerIndexedvEXT ((PFNGLGETPOINTERINDEXEDVEXTPROC)gl_addr[GLGETPOINTERINDEXEDVEXT].func)
#define glGetPointervEXT ((PFNGLGETPOINTERVEXTPROC)gl_addr[GLGETPOINTERVEXT].func)
#define glGetProgramEnvParameterdvARB ((PFNGLGETPROGRAMENVPARAMETERDVARBPROC)gl_addr[GLGETPROGRAMENVPARAMETERDVARB].func)
#define glGetProgramEnvParameterIivNV ((PFNGLGETPROGRAMENVPARAMETERIIVNVPROC)gl_addr[GLGETPROGRAMENVPARAMETERIIVNV].func)
#define glGetProgramEnvParameterIuivNV ((PFNGLGETPROGRAMENVPARAMETERIUIVNVPROC)gl_addr[GLGETPROGRAMENVPARAMETERIUIVNV].func)
#define glGetProgramInfoLog ((PFNGLGETPROGRAMINFOLOGPROC)gl_addr[GLGETPROGRAMINFOLOG].func)
#define glGetProgramiv ((PFNGLGETPROGRAMIVPROC)gl_addr[GLGETPROGRAMIV].func)
#define glGetProgramivNV ((PFNGLGETPROGRAMIVNVPROC)gl_addr[GLGETPROGRAMIVNV].func)
#define glGetProgramLocalParameterdvARB ((PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC)gl_addr[GLGETPROGRAMLOCALPARAMETERDVARB].func)
#define glGetProgramLocalParameterfvARB ((PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC)gl_addr[GLGETPROGRAMLOCALPARAMETERFVARB].func)
#define glGetProgramLocalParameterIivNV ((PFNGLGETPROGRAMLOCALPARAMETERIIVNVPROC)gl_addr[GLGETPROGRAMLOCALPARAMETERIIVNV].func)
#define glGetProgramLocalParameterIuivNV ((PFNGLGETPROGRAMLOCALPARAMETERIUIVNVPROC)gl_addr[GLGETPROGRAMLOCALPARAMETERIUIVNV].func)
#define glGetProgramNamedParameterdvNV ((PFNGLGETPROGRAMNAMEDPARAMETERDVNVPROC)gl_addr[GLGETPROGRAMNAMEDPARAMETERDVNV].func)
#define glGetProgramNamedParameterfvNV ((PFNGLGETPROGRAMNAMEDPARAMETERFVNVPROC)gl_addr[GLGETPROGRAMNAMEDPARAMETERFVNV].func)
#define glGetProgramParameterdvNV ((PFNGLGETPROGRAMPARAMETERDVNVPROC)gl_addr[GLGETPROGRAMPARAMETERDVNV].func)
#define glGetProgramParameterfvNV ((PFNGLGETPROGRAMPARAMETERFVNVPROC)gl_addr[GLGETPROGRAMPARAMETERFVNV].func)
#define glGetProgramStringARB ((PFNGLGETPROGRAMSTRINGARBPROC)gl_addr[GLGETPROGRAMSTRINGARB].func)
#define glGetProgramStringNV ((PFNGLGETPROGRAMSTRINGNVPROC)gl_addr[GLGETPROGRAMSTRINGNV].func)
#define glGetQueryiv ((PFNGLGETQUERYIVPROC)gl_addr[GLGETQUERYIV].func)
#define glGetQueryivARB ((PFNGLGETQUERYIVARBPROC)gl_addr[GLGETQUERYIVARB].func)
#define glGetQueryObjecti64vEXT ((PFNGLGETQUERYOBJECTI64VEXTPROC)gl_addr[GLGETQUERYOBJECTI64VEXT].func)
#define glGetQueryObjectiv ((PFNGLGETQUERYOBJECTIVPROC)gl_addr[GLGETQUERYOBJECTIV].func)
#define glGetQueryObjectivARB ((PFNGLGETQUERYOBJECTIVARBPROC)gl_addr[GLGETQUERYOBJECTIVARB].func)
#define glGetQueryObjectui64v ((PFNGLGETQUERYOBJECTUI64VPROC)gl_addr[GLGETQUERYOBJECTUI64V].func)
#define glGetQueryObjectui64vEXT ((PFNGLGETQUERYOBJECTUI64VEXTPROC)gl_addr[GLGETQUERYOBJECTUI64VEXT].func)
#define glGetQueryObjectuiv ((PFNGLGETQUERYOBJECTUIVPROC)gl_addr[GLGETQUERYOBJECTUIV].func)
#define glGetQueryObjectuivARB ((PFNGLGETQUERYOBJECTUIVARBPROC)gl_addr[GLGETQUERYOBJECTUIVARB].func)
#define glGetRenderbufferParameteriv ((PFNGLGETRENDERBUFFERPARAMETERIVPROC)gl_addr[GLGETRENDERBUFFERPARAMETERIV].func)
#define glGetRenderbufferParameterivEXT ((PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC)gl_addr[GLGETRENDERBUFFERPARAMETERIVEXT].func)
#define glGetSeparableFilter ((PFNGLGETSEPARABLEFILTERPROC)gl_addr[GLGETSEPARABLEFILTER].func)
#define glGetShaderInfoLog ((PFNGLGETSHADERINFOLOGPROC)gl_addr[GLGETSHADERINFOLOG].func)
#define glGetShaderiv ((PFNGLGETSHADERIVPROC)gl_addr[GLGETSHADERIV].func)
#define glGetShaderSource ((PFNGLGETSHADERSOURCEPROC)gl_addr[GLGETSHADERSOURCE].func)
#define glGetShaderSourceARB ((PFNGLGETSHADERSOURCEARBPROC)gl_addr[GLGETSHADERSOURCEARB].func)
#define glGetStringi ((PFNGLGETSTRINGIPROC)gl_addr[GLGETSTRINGI].func)
#define glGetTexParameterIiv ((PFNGLGETTEXPARAMETERIIVPROC)gl_addr[GLGETTEXPARAMETERIIV].func)
#define glGetTexParameterIivEXT ((PFNGLGETTEXPARAMETERIIVEXTPROC)gl_addr[GLGETTEXPARAMETERIIVEXT].func)
#define glGetTexParameterIuiv ((PFNGLGETTEXPARAMETERIUIVPROC)gl_addr[GLGETTEXPARAMETERIUIV].func)
#define glGetTexParameterIuivEXT ((PFNGLGETTEXPARAMETERIUIVEXTPROC)gl_addr[GLGETTEXPARAMETERIUIVEXT].func)
#define glGetTextureLevelParameterfvEXT ((PFNGLGETTEXTURELEVELPARAMETERFVEXTPROC)gl_addr[GLGETTEXTURELEVELPARAMETERFVEXT].func)
#define glGetTextureLevelParameterivEXT ((PFNGLGETTEXTURELEVELPARAMETERIVEXTPROC)gl_addr[GLGETTEXTURELEVELPARAMETERIVEXT].func)
#define glGetTextureParameterfvEXT ((PFNGLGETTEXTUREPARAMETERFVEXTPROC)gl_addr[GLGETTEXTUREPARAMETERFVEXT].func)
#define glGetTextureParameterIivEXT ((PFNGLGETTEXTUREPARAMETERIIVEXTPROC)gl_addr[GLGETTEXTUREPARAMETERIIVEXT].func)
#define glGetTextureParameterIuivEXT ((PFNGLGETTEXTUREPARAMETERIUIVEXTPROC)gl_addr[GLGETTEXTUREPARAMETERIUIVEXT].func)
#define glGetTextureParameterivEXT ((PFNGLGETTEXTUREPARAMETERIVEXTPROC)gl_addr[GLGETTEXTUREPARAMETERIVEXT].func)
#define glGetTrackMatrixivNV ((PFNGLGETTRACKMATRIXIVNVPROC)gl_addr[GLGETTRACKMATRIXIVNV].func)
#define glGetTransformFeedbackVarying ((PFNGLGETTRANSFORMFEEDBACKVARYINGPROC)gl_addr[GLGETTRANSFORMFEEDBACKVARYING].func)
#define glGetTransformFeedbackVaryingNV ((PFNGLGETTRANSFORMFEEDBACKVARYINGNVPROC)gl_addr[GLGETTRANSFORMFEEDBACKVARYINGNV].func)
#define glGetUniformBufferSizeEXT ((PFNGLGETUNIFORMBUFFERSIZEEXTPROC)gl_addr[GLGETUNIFORMBUFFERSIZEEXT].func)
#define glGetUniformfv ((PFNGLGETUNIFORMFVPROC)gl_addr[GLGETUNIFORMFV].func)
#define glGetUniformfvARB ((PFNGLGETUNIFORMFVARBPROC)gl_addr[GLGETUNIFORMFVARB].func)
#define glGetUniformiv ((PFNGLGETUNIFORMIVPROC)gl_addr[GLGETUNIFORMIV].func)
#define glGetUniformivARB ((PFNGLGETUNIFORMIVARBPROC)gl_addr[GLGETUNIFORMIVARB].func)
#define glGetUniformLocation ((PFNGLGETUNIFORMLOCATIONPROC)gl_addr[GLGETUNIFORMLOCATION].func)
#define glGetUniformLocationARB ((PFNGLGETUNIFORMLOCATIONARBPROC)gl_addr[GLGETUNIFORMLOCATIONARB].func)
#define glGetUniformOffsetEXT ((PFNGLGETUNIFORMOFFSETEXTPROC)gl_addr[GLGETUNIFORMOFFSETEXT].func)
#define glGetUniformuiv ((PFNGLGETUNIFORMUIVPROC)gl_addr[GLGETUNIFORMUIV].func)
#define glGetUniformuivEXT ((PFNGLGETUNIFORMUIVEXTPROC)gl_addr[GLGETUNIFORMUIVEXT].func)
#define glGetVaryingLocationNV ((PFNGLGETVARYINGLOCATIONNVPROC)gl_addr[GLGETVARYINGLOCATIONNV].func)
#define glGetVertexAttribdv ((PFNGLGETVERTEXATTRIBDVPROC)gl_addr[GLGETVERTEXATTRIBDV].func)
#define glGetVertexAttribdvARB ((PFNGLGETVERTEXATTRIBDVARBPROC)gl_addr[GLGETVERTEXATTRIBDVARB].func)
#define glGetVertexAttribdvNV ((PFNGLGETVERTEXATTRIBDVNVPROC)gl_addr[GLGETVERTEXATTRIBDVNV].func)
#define glGetVertexAttribfv ((PFNGLGETVERTEXATTRIBFVPROC)gl_addr[GLGETVERTEXATTRIBFV].func)
#define glGetVertexAttribfvARB ((PFNGLGETVERTEXATTRIBFVARBPROC)gl_addr[GLGETVERTEXATTRIBFVARB].func)
#define glGetVertexAttribfvNV ((PFNGLGETVERTEXATTRIBFVNVPROC)gl_addr[GLGETVERTEXATTRIBFVNV].func)
#define glGetVertexAttribIiv ((PFNGLGETVERTEXATTRIBIIVPROC)gl_addr[GLGETVERTEXATTRIBIIV].func)
#define glGetVertexAttribIivEXT ((PFNGLGETVERTEXATTRIBIIVEXTPROC)gl_addr[GLGETVERTEXATTRIBIIVEXT].func)
#define glGetVertexAttribIuiv ((PFNGLGETVERTEXATTRIBIUIVPROC)gl_addr[GLGETVERTEXATTRIBIUIV].func)
#define glGetVertexAttribIuivEXT ((PFNGLGETVERTEXATTRIBIUIVEXTPROC)gl_addr[GLGETVERTEXATTRIBIUIVEXT].func)
#define glGetVertexAttribiv ((PFNGLGETVERTEXATTRIBIVPROC)gl_addr[GLGETVERTEXATTRIBIV].func)
#define glGetVertexAttribivARB ((PFNGLGETVERTEXATTRIBIVARBPROC)gl_addr[GLGETVERTEXATTRIBIVARB].func)
#define glGetVertexAttribivNV ((PFNGLGETVERTEXATTRIBIVNVPROC)gl_addr[GLGETVERTEXATTRIBIVNV].func)
#define glGetVertexAttribPointerv ((PFNGLGETVERTEXATTRIBPOINTERVPROC)gl_addr[GLGETVERTEXATTRIBPOINTERV].func)
#define glGetVertexAttribPointervARB ((PFNGLGETVERTEXATTRIBPOINTERVARBPROC)gl_addr[GLGETVERTEXATTRIBPOINTERVARB].func)
#define glGetVertexAttribPointervNV ((PFNGLGETVERTEXATTRIBPOINTERVNVPROC)gl_addr[GLGETVERTEXATTRIBPOINTERVNV].func)
#define glGetVideoi64vNV ((PFNGLGETVIDEOI64VNVPROC)gl_addr[GLGETVIDEOI64VNV].func)
#define glGetVideoivNV ((PFNGLGETVIDEOIVNVPROC)gl_addr[GLGETVIDEOIVNV].func)
#define glGetVideoui64vNV ((PFNGLGETVIDEOUI64VNVPROC)gl_addr[GLGETVIDEOUI64VNV].func)
#define glGetVideouivNV ((PFNGLGETVIDEOUIVNVPROC)gl_addr[GLGETVIDEOUIVNV].func)
#define glHistogram ((PFNGLHISTOGRAMPROC)gl_addr[GLHISTOGRAM].func)
#define glIndexPointerEXT ((PFNGLINDEXPOINTEREXTPROC)gl_addr[GLINDEXPOINTEREXT].func)
#define glIsBuffer ((PFNGLISBUFFERPROC)gl_addr[GLISBUFFER].func)
#define glIsEnabledi ((PFNGLISENABLEDIPROC)gl_addr[GLISENABLEDI].func)
#define glIsEnabledIndexedEXT ((PFNGLISENABLEDINDEXEDEXTPROC)gl_addr[GLISENABLEDINDEXEDEXT].func)
#define glIsFenceNV ((PFNGLISFENCENVPROC)gl_addr[GLISFENCENV].func)
#define glIsFramebuffer ((PFNGLISFRAMEBUFFERPROC)gl_addr[GLISFRAMEBUFFER].func)
#define glIsFramebufferEXT ((PFNGLISFRAMEBUFFEREXTPROC)gl_addr[GLISFRAMEBUFFEREXT].func)
#define glIsOcclusionQueryNV ((PFNGLISOCCLUSIONQUERYNVPROC)gl_addr[GLISOCCLUSIONQUERYNV].func)
#define glIsProgram ((PFNGLISPROGRAMPROC)gl_addr[GLISPROGRAM].func)
#define glIsProgramARB ((PFNGLISPROGRAMARBPROC)gl_addr[GLISPROGRAMARB].func)
#define glIsProgramNV ((PFNGLISPROGRAMNVPROC)gl_addr[GLISPROGRAMNV].func)
#define glIsQuery ((PFNGLISQUERYPROC)gl_addr[GLISQUERY].func)
#define glIsQueryARB ((PFNGLISQUERYARBPROC)gl_addr[GLISQUERYARB].func)
#define glIsRenderbuffer ((PFNGLISRENDERBUFFERPROC)gl_addr[GLISRENDERBUFFER].func)
#define glIsRenderbufferEXT ((PFNGLISRENDERBUFFEREXTPROC)gl_addr[GLISRENDERBUFFEREXT].func)
#define glIsShader ((PFNGLISSHADERPROC)gl_addr[GLISSHADER].func)
#define glIsTextureEXT ((PFNGLISTEXTUREEXTPROC)gl_addr[GLISTEXTUREEXT].func)
#define glIsVertexArray ((PFNGLISVERTEXARRAYPROC)gl_addr[GLISVERTEXARRAY].func)
#define glLinkProgram ((PFNGLLINKPROGRAMPROC)gl_addr[GLLINKPROGRAM].func)
#define glLinkProgramARB ((PFNGLLINKPROGRAMARBPROC)gl_addr[GLLINKPROGRAMARB].func)
#define glLoadProgramNV ((PFNGLLOADPROGRAMNVPROC)gl_addr[GLLOADPROGRAMNV].func)
#define glLoadTransposeMatrixd ((PFNGLLOADTRANSPOSEMATRIXDPROC)gl_addr[GLLOADTRANSPOSEMATRIXD].func)
#define glLoadTransposeMatrixdARB ((PFNGLLOADTRANSPOSEMATRIXDARBPROC)gl_addr[GLLOADTRANSPOSEMATRIXDARB].func)
#define glLoadTransposeMatrixfARB ((PFNGLLOADTRANSPOSEMATRIXFARBPROC)gl_addr[GLLOADTRANSPOSEMATRIXFARB].func)
#define glLockArraysEXT ((PFNGLLOCKARRAYSEXTPROC)gl_addr[GLLOCKARRAYSEXT].func)
#define glMapBuffer ((PFNGLMAPBUFFERPROC)gl_addr[GLMAPBUFFER].func)
#define glMapBufferARB ((PFNGLMAPBUFFERARBPROC)gl_addr[GLMAPBUFFERARB].func)
#define glMapBufferRange ((PFNGLMAPBUFFERRANGEPROC)gl_addr[GLMAPBUFFERRANGE].func)
#define glMatrixFrustumEXT ((PFNGLMATRIXFRUSTUMEXTPROC)gl_addr[GLMATRIXFRUSTUMEXT].func)
#define glMatrixLoaddEXT ((PFNGLMATRIXLOADDEXTPROC)gl_addr[GLMATRIXLOADDEXT].func)
#define glMatrixLoadfEXT ((PFNGLMATRIXLOADFEXTPROC)gl_addr[GLMATRIXLOADFEXT].func)
#define glMatrixLoadIdentityEXT ((PFNGLMATRIXLOADIDENTITYEXTPROC)gl_addr[GLMATRIXLOADIDENTITYEXT].func)
#define glMatrixLoadTransposedEXT ((PFNGLMATRIXLOADTRANSPOSEDEXTPROC)gl_addr[GLMATRIXLOADTRANSPOSEDEXT].func)
#define glMatrixLoadTransposefEXT ((PFNGLMATRIXLOADTRANSPOSEFEXTPROC)gl_addr[GLMATRIXLOADTRANSPOSEFEXT].func)
#define glMatrixMultdEXT ((PFNGLMATRIXMULTDEXTPROC)gl_addr[GLMATRIXMULTDEXT].func)
#define glMatrixMultfEXT ((PFNGLMATRIXMULTFEXTPROC)gl_addr[GLMATRIXMULTFEXT].func)
#define glMatrixMultTransposedEXT ((PFNGLMATRIXMULTTRANSPOSEDEXTPROC)gl_addr[GLMATRIXMULTTRANSPOSEDEXT].func)
#define glMatrixMultTransposefEXT ((PFNGLMATRIXMULTTRANSPOSEFEXTPROC)gl_addr[GLMATRIXMULTTRANSPOSEFEXT].func)
#define glMatrixOrthoEXT ((PFNGLMATRIXORTHOEXTPROC)gl_addr[GLMATRIXORTHOEXT].func)
#define glMatrixPopEXT ((PFNGLMATRIXPOPEXTPROC)gl_addr[GLMATRIXPOPEXT].func)
#define glMatrixPushEXT ((PFNGLMATRIXPUSHEXTPROC)gl_addr[GLMATRIXPUSHEXT].func)
#define glMatrixRotatedEXT ((PFNGLMATRIXROTATEDEXTPROC)gl_addr[GLMATRIXROTATEDEXT].func)
#define glMatrixRotatefEXT ((PFNGLMATRIXROTATEFEXTPROC)gl_addr[GLMATRIXROTATEFEXT].func)
#define glMatrixScaledEXT ((PFNGLMATRIXSCALEDEXTPROC)gl_addr[GLMATRIXSCALEDEXT].func)
#define glMatrixScalefEXT ((PFNGLMATRIXSCALEFEXTPROC)gl_addr[GLMATRIXSCALEFEXT].func)
#define glMatrixTranslatedEXT ((PFNGLMATRIXTRANSLATEDEXTPROC)gl_addr[GLMATRIXTRANSLATEDEXT].func)
#define glMatrixTranslatefEXT ((PFNGLMATRIXTRANSLATEFEXTPROC)gl_addr[GLMATRIXTRANSLATEFEXT].func)
#define glMinmax ((PFNGLMINMAXPROC)gl_addr[GLMINMAX].func)
#define glMultiDrawArrays ((PFNGLMULTIDRAWARRAYSPROC)gl_addr[GLMULTIDRAWARRAYS].func)
#define glMultiDrawArraysEXT ((PFNGLMULTIDRAWARRAYSEXTPROC)gl_addr[GLMULTIDRAWARRAYSEXT].func)
#define glMultiDrawElements ((PFNGLMULTIDRAWELEMENTSPROC)gl_addr[GLMULTIDRAWELEMENTS].func)
#define glMultiDrawElementsEXT ((PFNGLMULTIDRAWELEMENTSEXTPROC)gl_addr[GLMULTIDRAWELEMENTSEXT].func)
#define glMultiTexBufferEXT ((PFNGLMULTITEXBUFFEREXTPROC)gl_addr[GLMULTITEXBUFFEREXT].func)
#define glMultiTexCoord1d ((PFNGLMULTITEXCOORD1DPROC)gl_addr[GLMULTITEXCOORD1D].func)
#define glMultiTexCoord1dARB ((PFNGLMULTITEXCOORD1DARBPROC)gl_addr[GLMULTITEXCOORD1DARB].func)
#define glMultiTexCoord1dv ((PFNGLMULTITEXCOORD1DVPROC)gl_addr[GLMULTITEXCOORD1DV].func)
#define glMultiTexCoord1dvARB ((PFNGLMULTITEXCOORD1DVARBPROC)gl_addr[GLMULTITEXCOORD1DVARB].func)
#define glMultiTexCoord1f ((PFNGLMULTITEXCOORD1FPROC)gl_addr[GLMULTITEXCOORD1F].func)
#define glMultiTexCoord1fARB ((PFNGLMULTITEXCOORD1FARBPROC)gl_addr[GLMULTITEXCOORD1FARB].func)
#define glMultiTexCoord1fv ((PFNGLMULTITEXCOORD1FVPROC)gl_addr[GLMULTITEXCOORD1FV].func)
#define glMultiTexCoord1fvARB ((PFNGLMULTITEXCOORD1FVARBPROC)gl_addr[GLMULTITEXCOORD1FVARB].func)
#define glMultiTexCoord1hNV ((PFNGLMULTITEXCOORD1HNVPROC)gl_addr[GLMULTITEXCOORD1HNV].func)
#define glMultiTexCoord1hvNV ((PFNGLMULTITEXCOORD1HVNVPROC)gl_addr[GLMULTITEXCOORD1HVNV].func)
#define glMultiTexCoord1i ((PFNGLMULTITEXCOORD1IPROC)gl_addr[GLMULTITEXCOORD1I].func)
#define glMultiTexCoord1iARB ((PFNGLMULTITEXCOORD1IARBPROC)gl_addr[GLMULTITEXCOORD1IARB].func)
#define glMultiTexCoord1iv ((PFNGLMULTITEXCOORD1IVPROC)gl_addr[GLMULTITEXCOORD1IV].func)
#define glMultiTexCoord1ivARB ((PFNGLMULTITEXCOORD1IVARBPROC)gl_addr[GLMULTITEXCOORD1IVARB].func)
#define glMultiTexCoord1s ((PFNGLMULTITEXCOORD1SPROC)gl_addr[GLMULTITEXCOORD1S].func)
#define glMultiTexCoord1sARB ((PFNGLMULTITEXCOORD1SARBPROC)gl_addr[GLMULTITEXCOORD1SARB].func)
#define glMultiTexCoord1sv ((PFNGLMULTITEXCOORD1SVPROC)gl_addr[GLMULTITEXCOORD1SV].func)
#define glMultiTexCoord1svARB ((PFNGLMULTITEXCOORD1SVARBPROC)gl_addr[GLMULTITEXCOORD1SVARB].func)
#define glMultiTexCoord2d ((PFNGLMULTITEXCOORD2DPROC)gl_addr[GLMULTITEXCOORD2D].func)
#define glMultiTexCoord2dARB ((PFNGLMULTITEXCOORD2DARBPROC)gl_addr[GLMULTITEXCOORD2DARB].func)
#define glMultiTexCoord2dv ((PFNGLMULTITEXCOORD2DVPROC)gl_addr[GLMULTITEXCOORD2DV].func)
#define glMultiTexCoord2dvARB ((PFNGLMULTITEXCOORD2DVARBPROC)gl_addr[GLMULTITEXCOORD2DVARB].func)
#define glMultiTexCoord2fARB ((PFNGLMULTITEXCOORD2FARBPROC)gl_addr[GLMULTITEXCOORD2FARB].func)
#define glMultiTexCoord2fvARB ((PFNGLMULTITEXCOORD2FVARBPROC)gl_addr[GLMULTITEXCOORD2FVARB].func)
#define glMultiTexCoord2hNV ((PFNGLMULTITEXCOORD2HNVPROC)gl_addr[GLMULTITEXCOORD2HNV].func)
#define glMultiTexCoord2hvNV ((PFNGLMULTITEXCOORD2HVNVPROC)gl_addr[GLMULTITEXCOORD2HVNV].func)
#define glMultiTexCoord2i ((PFNGLMULTITEXCOORD2IPROC)gl_addr[GLMULTITEXCOORD2I].func)
#define glMultiTexCoord2iARB ((PFNGLMULTITEXCOORD2IARBPROC)gl_addr[GLMULTITEXCOORD2IARB].func)
#define glMultiTexCoord2iv ((PFNGLMULTITEXCOORD2IVPROC)gl_addr[GLMULTITEXCOORD2IV].func)
#define glMultiTexCoord2ivARB ((PFNGLMULTITEXCOORD2IVARBPROC)gl_addr[GLMULTITEXCOORD2IVARB].func)
#define glMultiTexCoord2s ((PFNGLMULTITEXCOORD2SPROC)gl_addr[GLMULTITEXCOORD2S].func)
#define glMultiTexCoord2sARB ((PFNGLMULTITEXCOORD2SARBPROC)gl_addr[GLMULTITEXCOORD2SARB].func)
#define glMultiTexCoord2sv ((PFNGLMULTITEXCOORD2SVPROC)gl_addr[GLMULTITEXCOORD2SV].func)
#define glMultiTexCoord2svARB ((PFNGLMULTITEXCOORD2SVARBPROC)gl_addr[GLMULTITEXCOORD2SVARB].func)
#define glMultiTexCoord3d ((PFNGLMULTITEXCOORD3DPROC)gl_addr[GLMULTITEXCOORD3D].func)
#define glMultiTexCoord3dARB ((PFNGLMULTITEXCOORD3DARBPROC)gl_addr[GLMULTITEXCOORD3DARB].func)
#define glMultiTexCoord3dv ((PFNGLMULTITEXCOORD3DVPROC)gl_addr[GLMULTITEXCOORD3DV].func)
#define glMultiTexCoord3dvARB ((PFNGLMULTITEXCOORD3DVARBPROC)gl_addr[GLMULTITEXCOORD3DVARB].func)
#define glMultiTexCoord3f ((PFNGLMULTITEXCOORD3FPROC)gl_addr[GLMULTITEXCOORD3F].func)
#define glMultiTexCoord3fARB ((PFNGLMULTITEXCOORD3FARBPROC)gl_addr[GLMULTITEXCOORD3FARB].func)
#define glMultiTexCoord3fv ((PFNGLMULTITEXCOORD3FVPROC)gl_addr[GLMULTITEXCOORD3FV].func)
#define glMultiTexCoord3fvARB ((PFNGLMULTITEXCOORD3FVARBPROC)gl_addr[GLMULTITEXCOORD3FVARB].func)
#define glMultiTexCoord3hNV ((PFNGLMULTITEXCOORD3HNVPROC)gl_addr[GLMULTITEXCOORD3HNV].func)
#define glMultiTexCoord3hvNV ((PFNGLMULTITEXCOORD3HVNVPROC)gl_addr[GLMULTITEXCOORD3HVNV].func)
#define glMultiTexCoord3i ((PFNGLMULTITEXCOORD3IPROC)gl_addr[GLMULTITEXCOORD3I].func)
#define glMultiTexCoord3iARB ((PFNGLMULTITEXCOORD3IARBPROC)gl_addr[GLMULTITEXCOORD3IARB].func)
#define glMultiTexCoord3iv ((PFNGLMULTITEXCOORD3IVPROC)gl_addr[GLMULTITEXCOORD3IV].func)
#define glMultiTexCoord3ivARB ((PFNGLMULTITEXCOORD3IVARBPROC)gl_addr[GLMULTITEXCOORD3IVARB].func)
#define glMultiTexCoord3s ((PFNGLMULTITEXCOORD3SPROC)gl_addr[GLMULTITEXCOORD3S].func)
#define glMultiTexCoord3sARB ((PFNGLMULTITEXCOORD3SARBPROC)gl_addr[GLMULTITEXCOORD3SARB].func)
#define glMultiTexCoord3sv ((PFNGLMULTITEXCOORD3SVPROC)gl_addr[GLMULTITEXCOORD3SV].func)
#define glMultiTexCoord3svARB ((PFNGLMULTITEXCOORD3SVARBPROC)gl_addr[GLMULTITEXCOORD3SVARB].func)
#define glMultiTexCoord4d ((PFNGLMULTITEXCOORD4DPROC)gl_addr[GLMULTITEXCOORD4D].func)
#define glMultiTexCoord4dARB ((PFNGLMULTITEXCOORD4DARBPROC)gl_addr[GLMULTITEXCOORD4DARB].func)
#define glMultiTexCoord4dv ((PFNGLMULTITEXCOORD4DVPROC)gl_addr[GLMULTITEXCOORD4DV].func)
#define glMultiTexCoord4dvARB ((PFNGLMULTITEXCOORD4DVARBPROC)gl_addr[GLMULTITEXCOORD4DVARB].func)
#define glMultiTexCoord4f ((PFNGLMULTITEXCOORD4FPROC)gl_addr[GLMULTITEXCOORD4F].func)
#define glMultiTexCoord4fARB ((PFNGLMULTITEXCOORD4FARBPROC)gl_addr[GLMULTITEXCOORD4FARB].func)
#define glMultiTexCoord4fv ((PFNGLMULTITEXCOORD4FVPROC)gl_addr[GLMULTITEXCOORD4FV].func)
#define glMultiTexCoord4fvARB ((PFNGLMULTITEXCOORD4FVARBPROC)gl_addr[GLMULTITEXCOORD4FVARB].func)
#define glMultiTexCoord4hNV ((PFNGLMULTITEXCOORD4HNVPROC)gl_addr[GLMULTITEXCOORD4HNV].func)
#define glMultiTexCoord4hvNV ((PFNGLMULTITEXCOORD4HVNVPROC)gl_addr[GLMULTITEXCOORD4HVNV].func)
#define glMultiTexCoord4i ((PFNGLMULTITEXCOORD4IPROC)gl_addr[GLMULTITEXCOORD4I].func)
#define glMultiTexCoord4iARB ((PFNGLMULTITEXCOORD4IARBPROC)gl_addr[GLMULTITEXCOORD4IARB].func)
#define glMultiTexCoord4iv ((PFNGLMULTITEXCOORD4IVPROC)gl_addr[GLMULTITEXCOORD4IV].func)
#define glMultiTexCoord4ivARB ((PFNGLMULTITEXCOORD4IVARBPROC)gl_addr[GLMULTITEXCOORD4IVARB].func)
#define glMultiTexCoord4s ((PFNGLMULTITEXCOORD4SPROC)gl_addr[GLMULTITEXCOORD4S].func)
#define glMultiTexCoord4sARB ((PFNGLMULTITEXCOORD4SARBPROC)gl_addr[GLMULTITEXCOORD4SARB].func)
#define glMultiTexCoord4sv ((PFNGLMULTITEXCOORD4SVPROC)gl_addr[GLMULTITEXCOORD4SV].func)
#define glMultiTexCoord4svARB ((PFNGLMULTITEXCOORD4SVARBPROC)gl_addr[GLMULTITEXCOORD4SVARB].func)
#define glMultiTexCoordPointerEXT ((PFNGLMULTITEXCOORDPOINTEREXTPROC)gl_addr[GLMULTITEXCOORDPOINTEREXT].func)
#define glMultiTexEnvfEXT ((PFNGLMULTITEXENVFEXTPROC)gl_addr[GLMULTITEXENVFEXT].func)
#define glMultiTexEnvfvEXT ((PFNGLMULTITEXENVFVEXTPROC)gl_addr[GLMULTITEXENVFVEXT].func)
#define glMultiTexEnviEXT ((PFNGLMULTITEXENVIEXTPROC)gl_addr[GLMULTITEXENVIEXT].func)
#define glMultiTexEnvivEXT ((PFNGLMULTITEXENVIVEXTPROC)gl_addr[GLMULTITEXENVIVEXT].func)
#define glMultiTexGendEXT ((PFNGLMULTITEXGENDEXTPROC)gl_addr[GLMULTITEXGENDEXT].func)
#define glMultiTexGendvEXT ((PFNGLMULTITEXGENDVEXTPROC)gl_addr[GLMULTITEXGENDVEXT].func)
#define glMultiTexGenfEXT ((PFNGLMULTITEXGENFEXTPROC)gl_addr[GLMULTITEXGENFEXT].func)
#define glMultiTexGenfvEXT ((PFNGLMULTITEXGENFVEXTPROC)gl_addr[GLMULTITEXGENFVEXT].func)
#define glMultiTexGeniEXT ((PFNGLMULTITEXGENIEXTPROC)gl_addr[GLMULTITEXGENIEXT].func)
#define glMultiTexGenivEXT ((PFNGLMULTITEXGENIVEXTPROC)gl_addr[GLMULTITEXGENIVEXT].func)
#define glMultiTexImage1DEXT ((PFNGLMULTITEXIMAGE1DEXTPROC)gl_addr[GLMULTITEXIMAGE1DEXT].func)
#define glMultiTexImage2DEXT ((PFNGLMULTITEXIMAGE2DEXTPROC)gl_addr[GLMULTITEXIMAGE2DEXT].func)
#define glMultiTexImage3DEXT ((PFNGLMULTITEXIMAGE3DEXTPROC)gl_addr[GLMULTITEXIMAGE3DEXT].func)
#define glMultiTexParameterfEXT ((PFNGLMULTITEXPARAMETERFEXTPROC)gl_addr[GLMULTITEXPARAMETERFEXT].func)
#define glMultiTexParameterfvEXT ((PFNGLMULTITEXPARAMETERFVEXTPROC)gl_addr[GLMULTITEXPARAMETERFVEXT].func)
#define glMultiTexParameteriEXT ((PFNGLMULTITEXPARAMETERIEXTPROC)gl_addr[GLMULTITEXPARAMETERIEXT].func)
#define glMultiTexParameterIivEXT ((PFNGLMULTITEXPARAMETERIIVEXTPROC)gl_addr[GLMULTITEXPARAMETERIIVEXT].func)
#define glMultiTexParameterIuivEXT ((PFNGLMULTITEXPARAMETERIUIVEXTPROC)gl_addr[GLMULTITEXPARAMETERIUIVEXT].func)
#define glMultiTexParameterivEXT ((PFNGLMULTITEXPARAMETERIVEXTPROC)gl_addr[GLMULTITEXPARAMETERIVEXT].func)
#define glMultiTexRenderbufferEXT ((PFNGLMULTITEXRENDERBUFFEREXTPROC)gl_addr[GLMULTITEXRENDERBUFFEREXT].func)
#define glMultiTexSubImage1DEXT ((PFNGLMULTITEXSUBIMAGE1DEXTPROC)gl_addr[GLMULTITEXSUBIMAGE1DEXT].func)
#define glMultiTexSubImage2DEXT ((PFNGLMULTITEXSUBIMAGE2DEXTPROC)gl_addr[GLMULTITEXSUBIMAGE2DEXT].func)
#define glMultiTexSubImage3DEXT ((PFNGLMULTITEXSUBIMAGE3DEXTPROC)gl_addr[GLMULTITEXSUBIMAGE3DEXT].func)
#define glMultTransposeMatrixd ((PFNGLMULTTRANSPOSEMATRIXDPROC)gl_addr[GLMULTTRANSPOSEMATRIXD].func)
#define glMultTransposeMatrixdARB ((PFNGLMULTTRANSPOSEMATRIXDARBPROC)gl_addr[GLMULTTRANSPOSEMATRIXDARB].func)
#define glMultTransposeMatrixfARB ((PFNGLMULTTRANSPOSEMATRIXFARBPROC)gl_addr[GLMULTTRANSPOSEMATRIXFARB].func)
#define glNamedFramebufferRenderbufferEXT ((PFNGLNAMEDFRAMEBUFFERRENDERBUFFEREXTPROC)gl_addr[GLNAMEDFRAMEBUFFERRENDERBUFFEREXT].func)
#define glNamedFramebufferTexture1DEXT ((PFNGLNAMEDFRAMEBUFFERTEXTURE1DEXTPROC)gl_addr[GLNAMEDFRAMEBUFFERTEXTURE1DEXT].func)
#define glNamedFramebufferTexture2DEXT ((PFNGLNAMEDFRAMEBUFFERTEXTURE2DEXTPROC)gl_addr[GLNAMEDFRAMEBUFFERTEXTURE2DEXT].func)
#define glNamedFramebufferTexture3DEXT ((PFNGLNAMEDFRAMEBUFFERTEXTURE3DEXTPROC)gl_addr[GLNAMEDFRAMEBUFFERTEXTURE3DEXT].func)
#define glNamedFramebufferTextureEXT ((PFNGLNAMEDFRAMEBUFFERTEXTUREEXTPROC)gl_addr[GLNAMEDFRAMEBUFFERTEXTUREEXT].func)
#define glNamedFramebufferTextureFaceEXT ((PFNGLNAMEDFRAMEBUFFERTEXTUREFACEEXTPROC)gl_addr[GLNAMEDFRAMEBUFFERTEXTUREFACEEXT].func)
#define glNamedFramebufferTextureLayerEXT ((PFNGLNAMEDFRAMEBUFFERTEXTURELAYEREXTPROC)gl_addr[GLNAMEDFRAMEBUFFERTEXTURELAYEREXT].func)
#define glNamedProgramLocalParameter4dEXT ((PFNGLNAMEDPROGRAMLOCALPARAMETER4DEXTPROC)gl_addr[GLNAMEDPROGRAMLOCALPARAMETER4DEXT].func)
#define glNamedProgramLocalParameter4dvEXT ((PFNGLNAMEDPROGRAMLOCALPARAMETER4DVEXTPROC)gl_addr[GLNAMEDPROGRAMLOCALPARAMETER4DVEXT].func)
#define glNamedProgramLocalParameter4fEXT ((PFNGLNAMEDPROGRAMLOCALPARAMETER4FEXTPROC)gl_addr[GLNAMEDPROGRAMLOCALPARAMETER4FEXT].func)
#define glNamedProgramLocalParameter4fvEXT ((PFNGLNAMEDPROGRAMLOCALPARAMETER4FVEXTPROC)gl_addr[GLNAMEDPROGRAMLOCALPARAMETER4FVEXT].func)
#define glNamedProgramLocalParameterI4iEXT ((PFNGLNAMEDPROGRAMLOCALPARAMETERI4IEXTPROC)gl_addr[GLNAMEDPROGRAMLOCALPARAMETERI4IEXT].func)
#define glNamedProgramLocalParameterI4ivEXT ((PFNGLNAMEDPROGRAMLOCALPARAMETERI4IVEXTPROC)gl_addr[GLNAMEDPROGRAMLOCALPARAMETERI4IVEXT].func)
#define glNamedProgramLocalParameterI4uiEXT ((PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIEXTPROC)gl_addr[GLNAMEDPROGRAMLOCALPARAMETERI4UIEXT].func)
#define glNamedProgramLocalParameterI4uivEXT ((PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIVEXTPROC)gl_addr[GLNAMEDPROGRAMLOCALPARAMETERI4UIVEXT].func)
#define glNamedProgramLocalParameters4fvEXT ((PFNGLNAMEDPROGRAMLOCALPARAMETERS4FVEXTPROC)gl_addr[GLNAMEDPROGRAMLOCALPARAMETERS4FVEXT].func)
#define glNamedProgramLocalParametersI4ivEXT ((PFNGLNAMEDPROGRAMLOCALPARAMETERSI4IVEXTPROC)gl_addr[GLNAMEDPROGRAMLOCALPARAMETERSI4IVEXT].func)
#define glNamedProgramLocalParametersI4uivEXT ((PFNGLNAMEDPROGRAMLOCALPARAMETERSI4UIVEXTPROC)gl_addr[GLNAMEDPROGRAMLOCALPARAMETERSI4UIVEXT].func)
#define glNamedProgramStringEXT ((PFNGLNAMEDPROGRAMSTRINGEXTPROC)gl_addr[GLNAMEDPROGRAMSTRINGEXT].func)
#define glNamedRenderbufferStorageEXT ((PFNGLNAMEDRENDERBUFFERSTORAGEEXTPROC)gl_addr[GLNAMEDRENDERBUFFERSTORAGEEXT].func)
#define glNamedRenderbufferStorageMultisampleCoverageEXT ((PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLECOVERAGEEXTPROC)gl_addr[GLNAMEDRENDERBUFFERSTORAGEMULTISAMPLECOVERAGEEXT].func)
#define glNamedRenderbufferStorageMultisampleEXT ((PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC)gl_addr[GLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEEXT].func)
#define glNormal3hNV ((PFNGLNORMAL3HNVPROC)gl_addr[GLNORMAL3HNV].func)
#define glNormal3hvNV ((PFNGLNORMAL3HVNVPROC)gl_addr[GLNORMAL3HVNV].func)
#define glNormalPointerEXT ((PFNGLNORMALPOINTEREXTPROC)gl_addr[GLNORMALPOINTEREXT].func)
#define glPixelDataRangeNV ((PFNGLPIXELDATARANGENVPROC)gl_addr[GLPIXELDATARANGENV].func)
#define glPointParameterfARB ((PFNGLPOINTPARAMETERFARBPROC)gl_addr[GLPOINTPARAMETERFARB].func)
#define glPointParameterfEXT ((PFNGLPOINTPARAMETERFEXTPROC)gl_addr[GLPOINTPARAMETERFEXT].func)
#define glPointParameterfvARB ((PFNGLPOINTPARAMETERFVARBPROC)gl_addr[GLPOINTPARAMETERFVARB].func)
#define glPointParameterfvEXT ((PFNGLPOINTPARAMETERFVEXTPROC)gl_addr[GLPOINTPARAMETERFVEXT].func)
#define glPointParameteriNV ((PFNGLPOINTPARAMETERINVPROC)gl_addr[GLPOINTPARAMETERINV].func)
#define glPointParameteriv ((PFNGLPOINTPARAMETERIVPROC)gl_addr[GLPOINTPARAMETERIV].func)
#define glPointParameterivNV ((PFNGLPOINTPARAMETERIVNVPROC)gl_addr[GLPOINTPARAMETERIVNV].func)
#define glPresentFrameDualFillNV ((PFNGLPRESENTFRAMEDUALFILLNVPROC)gl_addr[GLPRESENTFRAMEDUALFILLNV].func)
#define glPresentFrameKeyedNV ((PFNGLPRESENTFRAMEKEYEDNVPROC)gl_addr[GLPRESENTFRAMEKEYEDNV].func)
#define glPrimitiveRestartNV ((PFNGLPRIMITIVERESTARTNVPROC)gl_addr[GLPRIMITIVERESTARTNV].func)
#define glPrioritizeTexturesEXT ((PFNGLPRIORITIZETEXTURESEXTPROC)gl_addr[GLPRIORITIZETEXTURESEXT].func)
#define glProgramBufferParametersIivNV ((PFNGLPROGRAMBUFFERPARAMETERSIIVNVPROC)gl_addr[GLPROGRAMBUFFERPARAMETERSIIVNV].func)
#define glProgramBufferParametersIuivNV ((PFNGLPROGRAMBUFFERPARAMETERSIUIVNVPROC)gl_addr[GLPROGRAMBUFFERPARAMETERSIUIVNV].func)
#define glProgramEnvParameter4dARB ((PFNGLPROGRAMENVPARAMETER4DARBPROC)gl_addr[GLPROGRAMENVPARAMETER4DARB].func)
#define glProgramEnvParameter4dvARB ((PFNGLPROGRAMENVPARAMETER4DVARBPROC)gl_addr[GLPROGRAMENVPARAMETER4DVARB].func)
#define glProgramEnvParameterI4iNV ((PFNGLPROGRAMENVPARAMETERI4INVPROC)gl_addr[GLPROGRAMENVPARAMETERI4INV].func)
#define glProgramEnvParameterI4ivNV ((PFNGLPROGRAMENVPARAMETERI4IVNVPROC)gl_addr[GLPROGRAMENVPARAMETERI4IVNV].func)
#define glProgramEnvParameterI4uiNV ((PFNGLPROGRAMENVPARAMETERI4UINVPROC)gl_addr[GLPROGRAMENVPARAMETERI4UINV].func)
#define glProgramEnvParameterI4uivNV ((PFNGLPROGRAMENVPARAMETERI4UIVNVPROC)gl_addr[GLPROGRAMENVPARAMETERI4UIVNV].func)
#define glProgramEnvParametersI4ivNV ((PFNGLPROGRAMENVPARAMETERSI4IVNVPROC)gl_addr[GLPROGRAMENVPARAMETERSI4IVNV].func)
#define glProgramEnvParametersI4uivNV ((PFNGLPROGRAMENVPARAMETERSI4UIVNVPROC)gl_addr[GLPROGRAMENVPARAMETERSI4UIVNV].func)
#define glProgramLocalParameter4dARB ((PFNGLPROGRAMLOCALPARAMETER4DARBPROC)gl_addr[GLPROGRAMLOCALPARAMETER4DARB].func)
#define glProgramLocalParameter4dvARB ((PFNGLPROGRAMLOCALPARAMETER4DVARBPROC)gl_addr[GLPROGRAMLOCALPARAMETER4DVARB].func)
#define glProgramLocalParameterI4iNV ((PFNGLPROGRAMLOCALPARAMETERI4INVPROC)gl_addr[GLPROGRAMLOCALPARAMETERI4INV].func)
#define glProgramLocalParameterI4ivNV ((PFNGLPROGRAMLOCALPARAMETERI4IVNVPROC)gl_addr[GLPROGRAMLOCALPARAMETERI4IVNV].func)
#define glProgramLocalParameterI4uiNV ((PFNGLPROGRAMLOCALPARAMETERI4UINVPROC)gl_addr[GLPROGRAMLOCALPARAMETERI4UINV].func)
#define glProgramLocalParameterI4uivNV ((PFNGLPROGRAMLOCALPARAMETERI4UIVNVPROC)gl_addr[GLPROGRAMLOCALPARAMETERI4UIVNV].func)
#define glProgramLocalParametersI4ivNV ((PFNGLPROGRAMLOCALPARAMETERSI4IVNVPROC)gl_addr[GLPROGRAMLOCALPARAMETERSI4IVNV].func)
#define glProgramLocalParametersI4uivNV ((PFNGLPROGRAMLOCALPARAMETERSI4UIVNVPROC)gl_addr[GLPROGRAMLOCALPARAMETERSI4UIVNV].func)
#define glProgramNamedParameter4dNV ((PFNGLPROGRAMNAMEDPARAMETER4DNVPROC)gl_addr[GLPROGRAMNAMEDPARAMETER4DNV].func)
#define glProgramNamedParameter4dvNV ((PFNGLPROGRAMNAMEDPARAMETER4DVNVPROC)gl_addr[GLPROGRAMNAMEDPARAMETER4DVNV].func)
#define glProgramNamedParameter4fNV ((PFNGLPROGRAMNAMEDPARAMETER4FNVPROC)gl_addr[GLPROGRAMNAMEDPARAMETER4FNV].func)
#define glProgramNamedParameter4fvNV ((PFNGLPROGRAMNAMEDPARAMETER4FVNVPROC)gl_addr[GLPROGRAMNAMEDPARAMETER4FVNV].func)
#define glProgramParameter4dNV ((PFNGLPROGRAMPARAMETER4DNVPROC)gl_addr[GLPROGRAMPARAMETER4DNV].func)
#define glProgramParameter4dvNV ((PFNGLPROGRAMPARAMETER4DVNVPROC)gl_addr[GLPROGRAMPARAMETER4DVNV].func)
#define glProgramParameter4fNV ((PFNGLPROGRAMPARAMETER4FNVPROC)gl_addr[GLPROGRAMPARAMETER4FNV].func)
#define glProgramParameter4fvNV ((PFNGLPROGRAMPARAMETER4FVNVPROC)gl_addr[GLPROGRAMPARAMETER4FVNV].func)
#define glProgramParameteriARB ((PFNGLPROGRAMPARAMETERIARBPROC)gl_addr[GLPROGRAMPARAMETERIARB].func)
#define glProgramParameteriEXT ((PFNGLPROGRAMPARAMETERIEXTPROC)gl_addr[GLPROGRAMPARAMETERIEXT].func)
#define glProgramParameters4dvNV ((PFNGLPROGRAMPARAMETERS4DVNVPROC)gl_addr[GLPROGRAMPARAMETERS4DVNV].func)
#define glProgramParameters4fvNV ((PFNGLPROGRAMPARAMETERS4FVNVPROC)gl_addr[GLPROGRAMPARAMETERS4FVNV].func)
#define glProgramStringARB ((PFNGLPROGRAMSTRINGARBPROC)gl_addr[GLPROGRAMSTRINGARB].func)
#define glProgramUniform1fEXT ((PFNGLPROGRAMUNIFORM1FEXTPROC)gl_addr[GLPROGRAMUNIFORM1FEXT].func)
#define glProgramUniform1fvEXT ((PFNGLPROGRAMUNIFORM1FVEXTPROC)gl_addr[GLPROGRAMUNIFORM1FVEXT].func)
#define glProgramUniform1iEXT ((PFNGLPROGRAMUNIFORM1IEXTPROC)gl_addr[GLPROGRAMUNIFORM1IEXT].func)
#define glProgramUniform1ivEXT ((PFNGLPROGRAMUNIFORM1IVEXTPROC)gl_addr[GLPROGRAMUNIFORM1IVEXT].func)
#define glProgramUniform1uiEXT ((PFNGLPROGRAMUNIFORM1UIEXTPROC)gl_addr[GLPROGRAMUNIFORM1UIEXT].func)
#define glProgramUniform1uivEXT ((PFNGLPROGRAMUNIFORM1UIVEXTPROC)gl_addr[GLPROGRAMUNIFORM1UIVEXT].func)
#define glProgramUniform2fEXT ((PFNGLPROGRAMUNIFORM2FEXTPROC)gl_addr[GLPROGRAMUNIFORM2FEXT].func)
#define glProgramUniform2fvEXT ((PFNGLPROGRAMUNIFORM2FVEXTPROC)gl_addr[GLPROGRAMUNIFORM2FVEXT].func)
#define glProgramUniform2iEXT ((PFNGLPROGRAMUNIFORM2IEXTPROC)gl_addr[GLPROGRAMUNIFORM2IEXT].func)
#define glProgramUniform2ivEXT ((PFNGLPROGRAMUNIFORM2IVEXTPROC)gl_addr[GLPROGRAMUNIFORM2IVEXT].func)
#define glProgramUniform2uiEXT ((PFNGLPROGRAMUNIFORM2UIEXTPROC)gl_addr[GLPROGRAMUNIFORM2UIEXT].func)
#define glProgramUniform2uivEXT ((PFNGLPROGRAMUNIFORM2UIVEXTPROC)gl_addr[GLPROGRAMUNIFORM2UIVEXT].func)
#define glProgramUniform3fEXT ((PFNGLPROGRAMUNIFORM3FEXTPROC)gl_addr[GLPROGRAMUNIFORM3FEXT].func)
#define glProgramUniform3fvEXT ((PFNGLPROGRAMUNIFORM3FVEXTPROC)gl_addr[GLPROGRAMUNIFORM3FVEXT].func)
#define glProgramUniform3iEXT ((PFNGLPROGRAMUNIFORM3IEXTPROC)gl_addr[GLPROGRAMUNIFORM3IEXT].func)
#define glProgramUniform3ivEXT ((PFNGLPROGRAMUNIFORM3IVEXTPROC)gl_addr[GLPROGRAMUNIFORM3IVEXT].func)
#define glProgramUniform3uiEXT ((PFNGLPROGRAMUNIFORM3UIEXTPROC)gl_addr[GLPROGRAMUNIFORM3UIEXT].func)
#define glProgramUniform3uivEXT ((PFNGLPROGRAMUNIFORM3UIVEXTPROC)gl_addr[GLPROGRAMUNIFORM3UIVEXT].func)
#define glProgramUniform4fEXT ((PFNGLPROGRAMUNIFORM4FEXTPROC)gl_addr[GLPROGRAMUNIFORM4FEXT].func)
#define glProgramUniform4fvEXT ((PFNGLPROGRAMUNIFORM4FVEXTPROC)gl_addr[GLPROGRAMUNIFORM4FVEXT].func)
#define glProgramUniform4iEXT ((PFNGLPROGRAMUNIFORM4IEXTPROC)gl_addr[GLPROGRAMUNIFORM4IEXT].func)
#define glProgramUniform4ivEXT ((PFNGLPROGRAMUNIFORM4IVEXTPROC)gl_addr[GLPROGRAMUNIFORM4IVEXT].func)
#define glProgramUniform4uiEXT ((PFNGLPROGRAMUNIFORM4UIEXTPROC)gl_addr[GLPROGRAMUNIFORM4UIEXT].func)
#define glProgramUniform4uivEXT ((PFNGLPROGRAMUNIFORM4UIVEXTPROC)gl_addr[GLPROGRAMUNIFORM4UIVEXT].func)
#define glProgramUniformMatrix2fvEXT ((PFNGLPROGRAMUNIFORMMATRIX2FVEXTPROC)gl_addr[GLPROGRAMUNIFORMMATRIX2FVEXT].func)
#define glProgramUniformMatrix2x3fvEXT ((PFNGLPROGRAMUNIFORMMATRIX2X3FVEXTPROC)gl_addr[GLPROGRAMUNIFORMMATRIX2X3FVEXT].func)
#define glProgramUniformMatrix2x4fvEXT ((PFNGLPROGRAMUNIFORMMATRIX2X4FVEXTPROC)gl_addr[GLPROGRAMUNIFORMMATRIX2X4FVEXT].func)
#define glProgramUniformMatrix3fvEXT ((PFNGLPROGRAMUNIFORMMATRIX3FVEXTPROC)gl_addr[GLPROGRAMUNIFORMMATRIX3FVEXT].func)
#define glProgramUniformMatrix3x2fvEXT ((PFNGLPROGRAMUNIFORMMATRIX3X2FVEXTPROC)gl_addr[GLPROGRAMUNIFORMMATRIX3X2FVEXT].func)
#define glProgramUniformMatrix3x4fvEXT ((PFNGLPROGRAMUNIFORMMATRIX3X4FVEXTPROC)gl_addr[GLPROGRAMUNIFORMMATRIX3X4FVEXT].func)
#define glProgramUniformMatrix4fvEXT ((PFNGLPROGRAMUNIFORMMATRIX4FVEXTPROC)gl_addr[GLPROGRAMUNIFORMMATRIX4FVEXT].func)
#define glProgramUniformMatrix4x2fvEXT ((PFNGLPROGRAMUNIFORMMATRIX4X2FVEXTPROC)gl_addr[GLPROGRAMUNIFORMMATRIX4X2FVEXT].func)
#define glProgramUniformMatrix4x3fvEXT ((PFNGLPROGRAMUNIFORMMATRIX4X3FVEXTPROC)gl_addr[GLPROGRAMUNIFORMMATRIX4X3FVEXT].func)
#define glProgramVertexLimitNV ((PFNGLPROGRAMVERTEXLIMITNVPROC)gl_addr[GLPROGRAMVERTEXLIMITNV].func)
#define glPushClientAttribDefaultEXT ((PFNGLPUSHCLIENTATTRIBDEFAULTEXTPROC)gl_addr[GLPUSHCLIENTATTRIBDEFAULTEXT].func)
#define glQueryCounter ((PFNGLQUERYCOUNTERPROC)gl_addr[GLQUERYCOUNTER].func)
#define glRenderbufferStorage ((PFNGLRENDERBUFFERSTORAGEPROC)gl_addr[GLRENDERBUFFERSTORAGE].func)
#define glRenderbufferStorageMultisample ((PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)gl_addr[GLRENDERBUFFERSTORAGEMULTISAMPLE].func)
#define glRenderbufferStorageMultisampleEXT ((PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC)gl_addr[GLRENDERBUFFERSTORAGEMULTISAMPLEEXT].func)
#define glRequestResidentProgramsNV ((PFNGLREQUESTRESIDENTPROGRAMSNVPROC)gl_addr[GLREQUESTRESIDENTPROGRAMSNV].func)
#define glResetHistogram ((PFNGLRESETHISTOGRAMPROC)gl_addr[GLRESETHISTOGRAM].func)
#define glResetMinmax ((PFNGLRESETMINMAXPROC)gl_addr[GLRESETMINMAX].func)
#define glSampleCoverage ((PFNGLSAMPLECOVERAGEPROC)gl_addr[GLSAMPLECOVERAGE].func)
#define glSampleCoverageARB ((PFNGLSAMPLECOVERAGEARBPROC)gl_addr[GLSAMPLECOVERAGEARB].func)
#define glSampleMaskIndexedNV ((PFNGLSAMPLEMASKINDEXEDNVPROC)gl_addr[GLSAMPLEMASKINDEXEDNV].func)
#define glSamplerParameterf ((PFNGLSAMPLERPARAMETERFPROC)gl_addr[GLSAMPLERPARAMETERF].func)
#define glSamplerParameterfv ((PFNGLSAMPLERPARAMETERFVPROC)gl_addr[GLSAMPLERPARAMETERFV].func)
#define glSamplerParameteri ((PFNGLSAMPLERPARAMETERIPROC)gl_addr[GLSAMPLERPARAMETERI].func)
#define glSecondaryColor3b ((PFNGLSECONDARYCOLOR3BPROC)gl_addr[GLSECONDARYCOLOR3B].func)
#define glSecondaryColor3bEXT ((PFNGLSECONDARYCOLOR3BEXTPROC)gl_addr[GLSECONDARYCOLOR3BEXT].func)
#define glSecondaryColor3bv ((PFNGLSECONDARYCOLOR3BVPROC)gl_addr[GLSECONDARYCOLOR3BV].func)
#define glSecondaryColor3bvEXT ((PFNGLSECONDARYCOLOR3BVEXTPROC)gl_addr[GLSECONDARYCOLOR3BVEXT].func)
#define glSecondaryColor3d ((PFNGLSECONDARYCOLOR3DPROC)gl_addr[GLSECONDARYCOLOR3D].func)
#define glSecondaryColor3dEXT ((PFNGLSECONDARYCOLOR3DEXTPROC)gl_addr[GLSECONDARYCOLOR3DEXT].func)
#define glSecondaryColor3dv ((PFNGLSECONDARYCOLOR3DVPROC)gl_addr[GLSECONDARYCOLOR3DV].func)
#define glSecondaryColor3dvEXT ((PFNGLSECONDARYCOLOR3DVEXTPROC)gl_addr[GLSECONDARYCOLOR3DVEXT].func)
#define glSecondaryColor3f ((PFNGLSECONDARYCOLOR3FPROC)gl_addr[GLSECONDARYCOLOR3F].func)
#define glSecondaryColor3fEXT ((PFNGLSECONDARYCOLOR3FEXTPROC)gl_addr[GLSECONDARYCOLOR3FEXT].func)
#define glSecondaryColor3fv ((PFNGLSECONDARYCOLOR3FVPROC)gl_addr[GLSECONDARYCOLOR3FV].func)
#define glSecondaryColor3fvEXT ((PFNGLSECONDARYCOLOR3FVEXTPROC)gl_addr[GLSECONDARYCOLOR3FVEXT].func)
#define glSecondaryColor3hNV ((PFNGLSECONDARYCOLOR3HNVPROC)gl_addr[GLSECONDARYCOLOR3HNV].func)
#define glSecondaryColor3hvNV ((PFNGLSECONDARYCOLOR3HVNVPROC)gl_addr[GLSECONDARYCOLOR3HVNV].func)
#define glSecondaryColor3i ((PFNGLSECONDARYCOLOR3IPROC)gl_addr[GLSECONDARYCOLOR3I].func)
#define glSecondaryColor3iEXT ((PFNGLSECONDARYCOLOR3IEXTPROC)gl_addr[GLSECONDARYCOLOR3IEXT].func)
#define glSecondaryColor3iv ((PFNGLSECONDARYCOLOR3IVPROC)gl_addr[GLSECONDARYCOLOR3IV].func)
#define glSecondaryColor3ivEXT ((PFNGLSECONDARYCOLOR3IVEXTPROC)gl_addr[GLSECONDARYCOLOR3IVEXT].func)
#define glSecondaryColor3s ((PFNGLSECONDARYCOLOR3SPROC)gl_addr[GLSECONDARYCOLOR3S].func)
#define glSecondaryColor3sEXT ((PFNGLSECONDARYCOLOR3SEXTPROC)gl_addr[GLSECONDARYCOLOR3SEXT].func)
#define glSecondaryColor3sv ((PFNGLSECONDARYCOLOR3SVPROC)gl_addr[GLSECONDARYCOLOR3SV].func)
#define glSecondaryColor3svEXT ((PFNGLSECONDARYCOLOR3SVEXTPROC)gl_addr[GLSECONDARYCOLOR3SVEXT].func)
#define glSecondaryColor3ub ((PFNGLSECONDARYCOLOR3UBPROC)gl_addr[GLSECONDARYCOLOR3UB].func)
#define glSecondaryColor3ubEXT ((PFNGLSECONDARYCOLOR3UBEXTPROC)gl_addr[GLSECONDARYCOLOR3UBEXT].func)
#define glSecondaryColor3ubv ((PFNGLSECONDARYCOLOR3UBVPROC)gl_addr[GLSECONDARYCOLOR3UBV].func)
#define glSecondaryColor3ubvEXT ((PFNGLSECONDARYCOLOR3UBVEXTPROC)gl_addr[GLSECONDARYCOLOR3UBVEXT].func)
#define glSecondaryColor3ui ((PFNGLSECONDARYCOLOR3UIPROC)gl_addr[GLSECONDARYCOLOR3UI].func)
#define glSecondaryColor3uiEXT ((PFNGLSECONDARYCOLOR3UIEXTPROC)gl_addr[GLSECONDARYCOLOR3UIEXT].func)
#define glSecondaryColor3uiv ((PFNGLSECONDARYCOLOR3UIVPROC)gl_addr[GLSECONDARYCOLOR3UIV].func)
#define glSecondaryColor3uivEXT ((PFNGLSECONDARYCOLOR3UIVEXTPROC)gl_addr[GLSECONDARYCOLOR3UIVEXT].func)
#define glSecondaryColor3us ((PFNGLSECONDARYCOLOR3USPROC)gl_addr[GLSECONDARYCOLOR3US].func)
#define glSecondaryColor3usEXT ((PFNGLSECONDARYCOLOR3USEXTPROC)gl_addr[GLSECONDARYCOLOR3USEXT].func)
#define glSecondaryColor3usv ((PFNGLSECONDARYCOLOR3USVPROC)gl_addr[GLSECONDARYCOLOR3USV].func)
#define glSecondaryColor3usvEXT ((PFNGLSECONDARYCOLOR3USVEXTPROC)gl_addr[GLSECONDARYCOLOR3USVEXT].func)
#define glSecondaryColorPointer ((PFNGLSECONDARYCOLORPOINTERPROC)gl_addr[GLSECONDARYCOLORPOINTER].func)
#define glSecondaryColorPointerEXT ((PFNGLSECONDARYCOLORPOINTEREXTPROC)gl_addr[GLSECONDARYCOLORPOINTEREXT].func)
#define glSeparableFilter2D ((PFNGLSEPARABLEFILTER2DPROC)gl_addr[GLSEPARABLEFILTER2D].func)
#define glSetFenceNV ((PFNGLSETFENCENVPROC)gl_addr[GLSETFENCENV].func)
#define glShaderSource ((PFNGLSHADERSOURCEPROC)gl_addr[GLSHADERSOURCE].func)
#define glShaderSourceARB ((PFNGLSHADERSOURCEARBPROC)gl_addr[GLSHADERSOURCEARB].func)
#define glStencilFuncSeparate ((PFNGLSTENCILFUNCSEPARATEPROC)gl_addr[GLSTENCILFUNCSEPARATE].func)
#define glStencilMaskSeparate ((PFNGLSTENCILMASKSEPARATEPROC)gl_addr[GLSTENCILMASKSEPARATE].func)
#define glStencilOpSeparate ((PFNGLSTENCILOPSEPARATEPROC)gl_addr[GLSTENCILOPSEPARATE].func)
#define glTestFenceNV ((PFNGLTESTFENCENVPROC)gl_addr[GLTESTFENCENV].func)
#define glTexBufferARB ((PFNGLTEXBUFFERARBPROC)gl_addr[GLTEXBUFFERARB].func)
#define glTexBufferEXT ((PFNGLTEXBUFFEREXTPROC)gl_addr[GLTEXBUFFEREXT].func)
#define glTexCoord1hNV ((PFNGLTEXCOORD1HNVPROC)gl_addr[GLTEXCOORD1HNV].func)
#define glTexCoord1hvNV ((PFNGLTEXCOORD1HVNVPROC)gl_addr[GLTEXCOORD1HVNV].func)
#define glTexCoord2hNV ((PFNGLTEXCOORD2HNVPROC)gl_addr[GLTEXCOORD2HNV].func)
#define glTexCoord2hvNV ((PFNGLTEXCOORD2HVNVPROC)gl_addr[GLTEXCOORD2HVNV].func)
#define glTexCoord3hNV ((PFNGLTEXCOORD3HNVPROC)gl_addr[GLTEXCOORD3HNV].func)
#define glTexCoord3hvNV ((PFNGLTEXCOORD3HVNVPROC)gl_addr[GLTEXCOORD3HVNV].func)
#define glTexCoord4hNV ((PFNGLTEXCOORD4HNVPROC)gl_addr[GLTEXCOORD4HNV].func)
#define glTexCoord4hvNV ((PFNGLTEXCOORD4HVNVPROC)gl_addr[GLTEXCOORD4HVNV].func)
#define glTexCoordPointerEXT ((PFNGLTEXCOORDPOINTEREXTPROC)gl_addr[GLTEXCOORDPOINTEREXT].func)
#define glTexImage3D ((PFNGLTEXIMAGE3DPROC)gl_addr[GLTEXIMAGE3D].func)
#define glTexImage3DEXT ((PFNGLTEXIMAGE3DEXTPROC)gl_addr[GLTEXIMAGE3DEXT].func)
#define glTexParameterIiv ((PFNGLTEXPARAMETERIIVPROC)gl_addr[GLTEXPARAMETERIIV].func)
#define glTexParameterIivEXT ((PFNGLTEXPARAMETERIIVEXTPROC)gl_addr[GLTEXPARAMETERIIVEXT].func)
#define glTexParameterIuiv ((PFNGLTEXPARAMETERIUIVPROC)gl_addr[GLTEXPARAMETERIUIV].func)
#define glTexParameterIuivEXT ((PFNGLTEXPARAMETERIUIVEXTPROC)gl_addr[GLTEXPARAMETERIUIVEXT].func)
#define glTexRenderbufferNV ((PFNGLTEXRENDERBUFFERNVPROC)gl_addr[GLTEXRENDERBUFFERNV].func)
#define glTexStorage2D ((PFNGLTEXSTORAGE2DPROC)gl_addr[GLTEXSTORAGE2D].func)
#define glTexSubImage3D ((PFNGLTEXSUBIMAGE3DPROC)gl_addr[GLTEXSUBIMAGE3D].func)
#define glTexSubImage3DEXT ((PFNGLTEXSUBIMAGE3DEXTPROC)gl_addr[GLTEXSUBIMAGE3DEXT].func)
#define glTextureBarrierNV ((PFNGLTEXTUREBARRIERNVPROC)gl_addr[GLTEXTUREBARRIERNV].func)
#define glTextureBufferEXT ((PFNGLTEXTUREBUFFEREXTPROC)gl_addr[GLTEXTUREBUFFEREXT].func)
#define glTextureImage1DEXT ((PFNGLTEXTUREIMAGE1DEXTPROC)gl_addr[GLTEXTUREIMAGE1DEXT].func)
#define glTextureImage2DEXT ((PFNGLTEXTUREIMAGE2DEXTPROC)gl_addr[GLTEXTUREIMAGE2DEXT].func)
#define glTextureImage3DEXT ((PFNGLTEXTUREIMAGE3DEXTPROC)gl_addr[GLTEXTUREIMAGE3DEXT].func)
#define glTextureParameterfEXT ((PFNGLTEXTUREPARAMETERFEXTPROC)gl_addr[GLTEXTUREPARAMETERFEXT].func)
#define glTextureParameterfvEXT ((PFNGLTEXTUREPARAMETERFVEXTPROC)gl_addr[GLTEXTUREPARAMETERFVEXT].func)
#define glTextureParameteriEXT ((PFNGLTEXTUREPARAMETERIEXTPROC)gl_addr[GLTEXTUREPARAMETERIEXT].func)
#define glTextureParameterIivEXT ((PFNGLTEXTUREPARAMETERIIVEXTPROC)gl_addr[GLTEXTUREPARAMETERIIVEXT].func)
#define glTextureParameterIuivEXT ((PFNGLTEXTUREPARAMETERIUIVEXTPROC)gl_addr[GLTEXTUREPARAMETERIUIVEXT].func)
#define glTextureParameterivEXT ((PFNGLTEXTUREPARAMETERIVEXTPROC)gl_addr[GLTEXTUREPARAMETERIVEXT].func)
#define glTextureRenderbufferEXT ((PFNGLTEXTURERENDERBUFFEREXTPROC)gl_addr[GLTEXTURERENDERBUFFEREXT].func)
#define glTextureStorage2DEXT ((PFNGLTEXTURESTORAGE2DEXTPROC)gl_addr[GLTEXTURESTORAGE2DEXT].func)
#define glTextureSubImage1DEXT ((PFNGLTEXTURESUBIMAGE1DEXTPROC)gl_addr[GLTEXTURESUBIMAGE1DEXT].func)
#define glTextureSubImage2DEXT ((PFNGLTEXTURESUBIMAGE2DEXTPROC)gl_addr[GLTEXTURESUBIMAGE2DEXT].func)
#define glTextureSubImage3DEXT ((PFNGLTEXTURESUBIMAGE3DEXTPROC)gl_addr[GLTEXTURESUBIMAGE3DEXT].func)
#define glTrackMatrixNV ((PFNGLTRACKMATRIXNVPROC)gl_addr[GLTRACKMATRIXNV].func)
#define glTransformFeedbackAttribsNV ((PFNGLTRANSFORMFEEDBACKATTRIBSNVPROC)gl_addr[GLTRANSFORMFEEDBACKATTRIBSNV].func)
#define glTransformFeedbackVaryings ((PFNGLTRANSFORMFEEDBACKVARYINGSPROC)gl_addr[GLTRANSFORMFEEDBACKVARYINGS].func)
#define glTransformFeedbackVaryingsNV ((PFNGLTRANSFORMFEEDBACKVARYINGSNVPROC)gl_addr[GLTRANSFORMFEEDBACKVARYINGSNV].func)
#define glUniform1f ((PFNGLUNIFORM1FPROC)gl_addr[GLUNIFORM1F].func)
#define glUniform1fARB ((PFNGLUNIFORM1FARBPROC)gl_addr[GLUNIFORM1FARB].func)
#define glUniform1fv ((PFNGLUNIFORM1FVPROC)gl_addr[GLUNIFORM1FV].func)
#define glUniform1fvARB ((PFNGLUNIFORM1FVARBPROC)gl_addr[GLUNIFORM1FVARB].func)
#define glUniform1i ((PFNGLUNIFORM1IPROC)gl_addr[GLUNIFORM1I].func)
#define glUniform1iARB ((PFNGLUNIFORM1IARBPROC)gl_addr[GLUNIFORM1IARB].func)
#define glUniform1iv ((PFNGLUNIFORM1IVPROC)gl_addr[GLUNIFORM1IV].func)
#define glUniform1ivARB ((PFNGLUNIFORM1IVARBPROC)gl_addr[GLUNIFORM1IVARB].func)
#define glUniform1ui ((PFNGLUNIFORM1UIPROC)gl_addr[GLUNIFORM1UI].func)
#define glUniform1uiEXT ((PFNGLUNIFORM1UIEXTPROC)gl_addr[GLUNIFORM1UIEXT].func)
#define glUniform1uiv ((PFNGLUNIFORM1UIVPROC)gl_addr[GLUNIFORM1UIV].func)
#define glUniform1uivEXT ((PFNGLUNIFORM1UIVEXTPROC)gl_addr[GLUNIFORM1UIVEXT].func)
#define glUniform2f ((PFNGLUNIFORM2FPROC)gl_addr[GLUNIFORM2F].func)
#define glUniform2fARB ((PFNGLUNIFORM2FARBPROC)gl_addr[GLUNIFORM2FARB].func)
#define glUniform2fv ((PFNGLUNIFORM2FVPROC)gl_addr[GLUNIFORM2FV].func)
#define glUniform2fvARB ((PFNGLUNIFORM2FVARBPROC)gl_addr[GLUNIFORM2FVARB].func)
#define glUniform2i ((PFNGLUNIFORM2IPROC)gl_addr[GLUNIFORM2I].func)
#define glUniform2iARB ((PFNGLUNIFORM2IARBPROC)gl_addr[GLUNIFORM2IARB].func)
#define glUniform2iv ((PFNGLUNIFORM2IVPROC)gl_addr[GLUNIFORM2IV].func)
#define glUniform2ivARB ((PFNGLUNIFORM2IVARBPROC)gl_addr[GLUNIFORM2IVARB].func)
#define glUniform2ui ((PFNGLUNIFORM2UIPROC)gl_addr[GLUNIFORM2UI].func)
#define glUniform2uiEXT ((PFNGLUNIFORM2UIEXTPROC)gl_addr[GLUNIFORM2UIEXT].func)
#define glUniform2uiv ((PFNGLUNIFORM2UIVPROC)gl_addr[GLUNIFORM2UIV].func)
#define glUniform2uivEXT ((PFNGLUNIFORM2UIVEXTPROC)gl_addr[GLUNIFORM2UIVEXT].func)
#define glUniform3f ((PFNGLUNIFORM3FPROC)gl_addr[GLUNIFORM3F].func)
#define glUniform3fARB ((PFNGLUNIFORM3FARBPROC)gl_addr[GLUNIFORM3FARB].func)
#define glUniform3fv ((PFNGLUNIFORM3FVPROC)gl_addr[GLUNIFORM3FV].func)
#define glUniform3fvARB ((PFNGLUNIFORM3FVARBPROC)gl_addr[GLUNIFORM3FVARB].func)
#define glUniform3i ((PFNGLUNIFORM3IPROC)gl_addr[GLUNIFORM3I].func)
#define glUniform3iARB ((PFNGLUNIFORM3IARBPROC)gl_addr[GLUNIFORM3IARB].func)
#define glUniform3iv ((PFNGLUNIFORM3IVPROC)gl_addr[GLUNIFORM3IV].func)
#define glUniform3ivARB ((PFNGLUNIFORM3IVARBPROC)gl_addr[GLUNIFORM3IVARB].func)
#define glUniform3ui ((PFNGLUNIFORM3UIPROC)gl_addr[GLUNIFORM3UI].func)
#define glUniform3uiEXT ((PFNGLUNIFORM3UIEXTPROC)gl_addr[GLUNIFORM3UIEXT].func)
#define glUniform3uiv ((PFNGLUNIFORM3UIVPROC)gl_addr[GLUNIFORM3UIV].func)
#define glUniform3uivEXT ((PFNGLUNIFORM3UIVEXTPROC)gl_addr[GLUNIFORM3UIVEXT].func)
#define glUniform4f ((PFNGLUNIFORM4FPROC)gl_addr[GLUNIFORM4F].func)
#define glUniform4fARB ((PFNGLUNIFORM4FARBPROC)gl_addr[GLUNIFORM4FARB].func)
#define glUniform4fv ((PFNGLUNIFORM4FVPROC)gl_addr[GLUNIFORM4FV].func)
#define glUniform4fvARB ((PFNGLUNIFORM4FVARBPROC)gl_addr[GLUNIFORM4FVARB].func)
#define glUniform4i ((PFNGLUNIFORM4IPROC)gl_addr[GLUNIFORM4I].func)
#define glUniform4iARB ((PFNGLUNIFORM4IARBPROC)gl_addr[GLUNIFORM4IARB].func)
#define glUniform4iv ((PFNGLUNIFORM4IVPROC)gl_addr[GLUNIFORM4IV].func)
#define glUniform4ivARB ((PFNGLUNIFORM4IVARBPROC)gl_addr[GLUNIFORM4IVARB].func)
#define glUniform4ui ((PFNGLUNIFORM4UIPROC)gl_addr[GLUNIFORM4UI].func)
#define glUniform4uiEXT ((PFNGLUNIFORM4UIEXTPROC)gl_addr[GLUNIFORM4UIEXT].func)
#define glUniform4uiv ((PFNGLUNIFORM4UIVPROC)gl_addr[GLUNIFORM4UIV].func)
#define glUniform4uivEXT ((PFNGLUNIFORM4UIVEXTPROC)gl_addr[GLUNIFORM4UIVEXT].func)
#define glUniformBufferEXT ((PFNGLUNIFORMBUFFEREXTPROC)gl_addr[GLUNIFORMBUFFEREXT].func)
#define glUniformMatrix2fv ((PFNGLUNIFORMMATRIX2FVPROC)gl_addr[GLUNIFORMMATRIX2FV].func)
#define glUniformMatrix2fvARB ((PFNGLUNIFORMMATRIX2FVARBPROC)gl_addr[GLUNIFORMMATRIX2FVARB].func)
#define glUniformMatrix2x3fv ((PFNGLUNIFORMMATRIX2X3FVPROC)gl_addr[GLUNIFORMMATRIX2X3FV].func)
#define glUniformMatrix2x4fv ((PFNGLUNIFORMMATRIX2X4FVPROC)gl_addr[GLUNIFORMMATRIX2X4FV].func)
#define glUniformMatrix3fv ((PFNGLUNIFORMMATRIX3FVPROC)gl_addr[GLUNIFORMMATRIX3FV].func)
#define glUniformMatrix3fvARB ((PFNGLUNIFORMMATRIX3FVARBPROC)gl_addr[GLUNIFORMMATRIX3FVARB].func)
#define glUniformMatrix3x2fv ((PFNGLUNIFORMMATRIX3X2FVPROC)gl_addr[GLUNIFORMMATRIX3X2FV].func)
#define glUniformMatrix3x4fv ((PFNGLUNIFORMMATRIX3X4FVPROC)gl_addr[GLUNIFORMMATRIX3X4FV].func)
#define glUniformMatrix4fv ((PFNGLUNIFORMMATRIX4FVPROC)gl_addr[GLUNIFORMMATRIX4FV].func)
#define glUniformMatrix4fvARB ((PFNGLUNIFORMMATRIX4FVARBPROC)gl_addr[GLUNIFORMMATRIX4FVARB].func)
#define glUniformMatrix4x2fv ((PFNGLUNIFORMMATRIX4X2FVPROC)gl_addr[GLUNIFORMMATRIX4X2FV].func)
#define glUniformMatrix4x3fv ((PFNGLUNIFORMMATRIX4X3FVPROC)gl_addr[GLUNIFORMMATRIX4X3FV].func)
#define glUnlockArraysEXT ((PFNGLUNLOCKARRAYSEXTPROC)gl_addr[GLUNLOCKARRAYSEXT].func)
#define glUnmapBuffer ((PFNGLUNMAPBUFFERPROC)gl_addr[GLUNMAPBUFFER].func)
#define glUnmapBufferARB ((PFNGLUNMAPBUFFERARBPROC)gl_addr[GLUNMAPBUFFERARB].func)
#define glUseProgramObjectARB ((PFNGLUSEPROGRAMOBJECTARBPROC)gl_addr[GLUSEPROGRAMOBJECTARB].func)
#define glValidateProgram ((PFNGLVALIDATEPROGRAMPROC)gl_addr[GLVALIDATEPROGRAM].func)
#define glValidateProgramARB ((PFNGLVALIDATEPROGRAMARBPROC)gl_addr[GLVALIDATEPROGRAMARB].func)
#define glVertex2hNV ((PFNGLVERTEX2HNVPROC)gl_addr[GLVERTEX2HNV].func)
#define glVertex2hvNV ((PFNGLVERTEX2HVNVPROC)gl_addr[GLVERTEX2HVNV].func)
#define glVertex3hNV ((PFNGLVERTEX3HNVPROC)gl_addr[GLVERTEX3HNV].func)
#define glVertex3hvNV ((PFNGLVERTEX3HVNVPROC)gl_addr[GLVERTEX3HVNV].func)
#define glVertex4hNV ((PFNGLVERTEX4HNVPROC)gl_addr[GLVERTEX4HNV].func)
#define glVertex4hvNV ((PFNGLVERTEX4HVNVPROC)gl_addr[GLVERTEX4HVNV].func)
#define glVertexArrayRangeNV ((PFNGLVERTEXARRAYRANGENVPROC)gl_addr[GLVERTEXARRAYRANGENV].func)
#define glVertexAttrib1d ((PFNGLVERTEXATTRIB1DPROC)gl_addr[GLVERTEXATTRIB1D].func)
#define glVertexAttrib1dARB ((PFNGLVERTEXATTRIB1DARBPROC)gl_addr[GLVERTEXATTRIB1DARB].func)
#define glVertexAttrib1dNV ((PFNGLVERTEXATTRIB1DNVPROC)gl_addr[GLVERTEXATTRIB1DNV].func)
#define glVertexAttrib1dv ((PFNGLVERTEXATTRIB1DVPROC)gl_addr[GLVERTEXATTRIB1DV].func)
#define glVertexAttrib1dvARB ((PFNGLVERTEXATTRIB1DVARBPROC)gl_addr[GLVERTEXATTRIB1DVARB].func)
#define glVertexAttrib1dvNV ((PFNGLVERTEXATTRIB1DVNVPROC)gl_addr[GLVERTEXATTRIB1DVNV].func)
#define glVertexAttrib1f ((PFNGLVERTEXATTRIB1FPROC)gl_addr[GLVERTEXATTRIB1F].func)
#define glVertexAttrib1fARB ((PFNGLVERTEXATTRIB1FARBPROC)gl_addr[GLVERTEXATTRIB1FARB].func)
#define glVertexAttrib1fNV ((PFNGLVERTEXATTRIB1FNVPROC)gl_addr[GLVERTEXATTRIB1FNV].func)
#define glVertexAttrib1fv ((PFNGLVERTEXATTRIB1FVPROC)gl_addr[GLVERTEXATTRIB1FV].func)
#define glVertexAttrib1fvARB ((PFNGLVERTEXATTRIB1FVARBPROC)gl_addr[GLVERTEXATTRIB1FVARB].func)
#define glVertexAttrib1fvNV ((PFNGLVERTEXATTRIB1FVNVPROC)gl_addr[GLVERTEXATTRIB1FVNV].func)
#define glVertexAttrib1hNV ((PFNGLVERTEXATTRIB1HNVPROC)gl_addr[GLVERTEXATTRIB1HNV].func)
#define glVertexAttrib1hvNV ((PFNGLVERTEXATTRIB1HVNVPROC)gl_addr[GLVERTEXATTRIB1HVNV].func)
#define glVertexAttrib1s ((PFNGLVERTEXATTRIB1SPROC)gl_addr[GLVERTEXATTRIB1S].func)
#define glVertexAttrib1sARB ((PFNGLVERTEXATTRIB1SARBPROC)gl_addr[GLVERTEXATTRIB1SARB].func)
#define glVertexAttrib1sNV ((PFNGLVERTEXATTRIB1SNVPROC)gl_addr[GLVERTEXATTRIB1SNV].func)
#define glVertexAttrib1sv ((PFNGLVERTEXATTRIB1SVPROC)gl_addr[GLVERTEXATTRIB1SV].func)
#define glVertexAttrib1svARB ((PFNGLVERTEXATTRIB1SVARBPROC)gl_addr[GLVERTEXATTRIB1SVARB].func)
#define glVertexAttrib1svNV ((PFNGLVERTEXATTRIB1SVNVPROC)gl_addr[GLVERTEXATTRIB1SVNV].func)
#define glVertexAttrib2d ((PFNGLVERTEXATTRIB2DPROC)gl_addr[GLVERTEXATTRIB2D].func)
#define glVertexAttrib2dARB ((PFNGLVERTEXATTRIB2DARBPROC)gl_addr[GLVERTEXATTRIB2DARB].func)
#define glVertexAttrib2dNV ((PFNGLVERTEXATTRIB2DNVPROC)gl_addr[GLVERTEXATTRIB2DNV].func)
#define glVertexAttrib2dv ((PFNGLVERTEXATTRIB2DVPROC)gl_addr[GLVERTEXATTRIB2DV].func)
#define glVertexAttrib2dvARB ((PFNGLVERTEXATTRIB2DVARBPROC)gl_addr[GLVERTEXATTRIB2DVARB].func)
#define glVertexAttrib2dvNV ((PFNGLVERTEXATTRIB2DVNVPROC)gl_addr[GLVERTEXATTRIB2DVNV].func)
#define glVertexAttrib2f ((PFNGLVERTEXATTRIB2FPROC)gl_addr[GLVERTEXATTRIB2F].func)
#define glVertexAttrib2fARB ((PFNGLVERTEXATTRIB2FARBPROC)gl_addr[GLVERTEXATTRIB2FARB].func)
#define glVertexAttrib2fNV ((PFNGLVERTEXATTRIB2FNVPROC)gl_addr[GLVERTEXATTRIB2FNV].func)
#define glVertexAttrib2fv ((PFNGLVERTEXATTRIB2FVPROC)gl_addr[GLVERTEXATTRIB2FV].func)
#define glVertexAttrib2fvARB ((PFNGLVERTEXATTRIB2FVARBPROC)gl_addr[GLVERTEXATTRIB2FVARB].func)
#define glVertexAttrib2fvNV ((PFNGLVERTEXATTRIB2FVNVPROC)gl_addr[GLVERTEXATTRIB2FVNV].func)
#define glVertexAttrib2hNV ((PFNGLVERTEXATTRIB2HNVPROC)gl_addr[GLVERTEXATTRIB2HNV].func)
#define glVertexAttrib2hvNV ((PFNGLVERTEXATTRIB2HVNVPROC)gl_addr[GLVERTEXATTRIB2HVNV].func)
#define glVertexAttrib2s ((PFNGLVERTEXATTRIB2SPROC)gl_addr[GLVERTEXATTRIB2S].func)
#define glVertexAttrib2sARB ((PFNGLVERTEXATTRIB2SARBPROC)gl_addr[GLVERTEXATTRIB2SARB].func)
#define glVertexAttrib2sNV ((PFNGLVERTEXATTRIB2SNVPROC)gl_addr[GLVERTEXATTRIB2SNV].func)
#define glVertexAttrib2sv ((PFNGLVERTEXATTRIB2SVPROC)gl_addr[GLVERTEXATTRIB2SV].func)
#define glVertexAttrib2svARB ((PFNGLVERTEXATTRIB2SVARBPROC)gl_addr[GLVERTEXATTRIB2SVARB].func)
#define glVertexAttrib2svNV ((PFNGLVERTEXATTRIB2SVNVPROC)gl_addr[GLVERTEXATTRIB2SVNV].func)
#define glVertexAttrib3d ((PFNGLVERTEXATTRIB3DPROC)gl_addr[GLVERTEXATTRIB3D].func)
#define glVertexAttrib3dARB ((PFNGLVERTEXATTRIB3DARBPROC)gl_addr[GLVERTEXATTRIB3DARB].func)
#define glVertexAttrib3dNV ((PFNGLVERTEXATTRIB3DNVPROC)gl_addr[GLVERTEXATTRIB3DNV].func)
#define glVertexAttrib3dv ((PFNGLVERTEXATTRIB3DVPROC)gl_addr[GLVERTEXATTRIB3DV].func)
#define glVertexAttrib3dvARB ((PFNGLVERTEXATTRIB3DVARBPROC)gl_addr[GLVERTEXATTRIB3DVARB].func)
#define glVertexAttrib3dvNV ((PFNGLVERTEXATTRIB3DVNVPROC)gl_addr[GLVERTEXATTRIB3DVNV].func)
#define glVertexAttrib3f ((PFNGLVERTEXATTRIB3FPROC)gl_addr[GLVERTEXATTRIB3F].func)
#define glVertexAttrib3fARB ((PFNGLVERTEXATTRIB3FARBPROC)gl_addr[GLVERTEXATTRIB3FARB].func)
#define glVertexAttrib3fNV ((PFNGLVERTEXATTRIB3FNVPROC)gl_addr[GLVERTEXATTRIB3FNV].func)
#define glVertexAttrib3fv ((PFNGLVERTEXATTRIB3FVPROC)gl_addr[GLVERTEXATTRIB3FV].func)
#define glVertexAttrib3fvARB ((PFNGLVERTEXATTRIB3FVARBPROC)gl_addr[GLVERTEXATTRIB3FVARB].func)
#define glVertexAttrib3fvNV ((PFNGLVERTEXATTRIB3FVNVPROC)gl_addr[GLVERTEXATTRIB3FVNV].func)
#define glVertexAttrib3hNV ((PFNGLVERTEXATTRIB3HNVPROC)gl_addr[GLVERTEXATTRIB3HNV].func)
#define glVertexAttrib3hvNV ((PFNGLVERTEXATTRIB3HVNVPROC)gl_addr[GLVERTEXATTRIB3HVNV].func)
#define glVertexAttrib3s ((PFNGLVERTEXATTRIB3SPROC)gl_addr[GLVERTEXATTRIB3S].func)
#define glVertexAttrib3sARB ((PFNGLVERTEXATTRIB3SARBPROC)gl_addr[GLVERTEXATTRIB3SARB].func)
#define glVertexAttrib3sNV ((PFNGLVERTEXATTRIB3SNVPROC)gl_addr[GLVERTEXATTRIB3SNV].func)
#define glVertexAttrib3sv ((PFNGLVERTEXATTRIB3SVPROC)gl_addr[GLVERTEXATTRIB3SV].func)
#define glVertexAttrib3svARB ((PFNGLVERTEXATTRIB3SVARBPROC)gl_addr[GLVERTEXATTRIB3SVARB].func)
#define glVertexAttrib3svNV ((PFNGLVERTEXATTRIB3SVNVPROC)gl_addr[GLVERTEXATTRIB3SVNV].func)
#define glVertexAttrib4bv ((PFNGLVERTEXATTRIB4BVPROC)gl_addr[GLVERTEXATTRIB4BV].func)
#define glVertexAttrib4bvARB ((PFNGLVERTEXATTRIB4BVARBPROC)gl_addr[GLVERTEXATTRIB4BVARB].func)
#define glVertexAttrib4d ((PFNGLVERTEXATTRIB4DPROC)gl_addr[GLVERTEXATTRIB4D].func)
#define glVertexAttrib4dARB ((PFNGLVERTEXATTRIB4DARBPROC)gl_addr[GLVERTEXATTRIB4DARB].func)
#define glVertexAttrib4dNV ((PFNGLVERTEXATTRIB4DNVPROC)gl_addr[GLVERTEXATTRIB4DNV].func)
#define glVertexAttrib4dv ((PFNGLVERTEXATTRIB4DVPROC)gl_addr[GLVERTEXATTRIB4DV].func)
#define glVertexAttrib4dvARB ((PFNGLVERTEXATTRIB4DVARBPROC)gl_addr[GLVERTEXATTRIB4DVARB].func)
#define glVertexAttrib4dvNV ((PFNGLVERTEXATTRIB4DVNVPROC)gl_addr[GLVERTEXATTRIB4DVNV].func)
#define glVertexAttrib4f ((PFNGLVERTEXATTRIB4FPROC)gl_addr[GLVERTEXATTRIB4F].func)
#define glVertexAttrib4fARB ((PFNGLVERTEXATTRIB4FARBPROC)gl_addr[GLVERTEXATTRIB4FARB].func)
#define glVertexAttrib4fNV ((PFNGLVERTEXATTRIB4FNVPROC)gl_addr[GLVERTEXATTRIB4FNV].func)
#define glVertexAttrib4fv ((PFNGLVERTEXATTRIB4FVPROC)gl_addr[GLVERTEXATTRIB4FV].func)
#define glVertexAttrib4fvARB ((PFNGLVERTEXATTRIB4FVARBPROC)gl_addr[GLVERTEXATTRIB4FVARB].func)
#define glVertexAttrib4fvNV ((PFNGLVERTEXATTRIB4FVNVPROC)gl_addr[GLVERTEXATTRIB4FVNV].func)
#define glVertexAttrib4hNV ((PFNGLVERTEXATTRIB4HNVPROC)gl_addr[GLVERTEXATTRIB4HNV].func)
#define glVertexAttrib4hvNV ((PFNGLVERTEXATTRIB4HVNVPROC)gl_addr[GLVERTEXATTRIB4HVNV].func)
#define glVertexAttrib4iv ((PFNGLVERTEXATTRIB4IVPROC)gl_addr[GLVERTEXATTRIB4IV].func)
#define glVertexAttrib4ivARB ((PFNGLVERTEXATTRIB4IVARBPROC)gl_addr[GLVERTEXATTRIB4IVARB].func)
#define glVertexAttrib4Nbv ((PFNGLVERTEXATTRIB4NBVPROC)gl_addr[GLVERTEXATTRIB4NBV].func)
#define glVertexAttrib4NbvARB ((PFNGLVERTEXATTRIB4NBVARBPROC)gl_addr[GLVERTEXATTRIB4NBVARB].func)
#define glVertexAttrib4Niv ((PFNGLVERTEXATTRIB4NIVPROC)gl_addr[GLVERTEXATTRIB4NIV].func)
#define glVertexAttrib4NivARB ((PFNGLVERTEXATTRIB4NIVARBPROC)gl_addr[GLVERTEXATTRIB4NIVARB].func)
#define glVertexAttrib4Nsv ((PFNGLVERTEXATTRIB4NSVPROC)gl_addr[GLVERTEXATTRIB4NSV].func)
#define glVertexAttrib4NsvARB ((PFNGLVERTEXATTRIB4NSVARBPROC)gl_addr[GLVERTEXATTRIB4NSVARB].func)
#define glVertexAttrib4Nub ((PFNGLVERTEXATTRIB4NUBPROC)gl_addr[GLVERTEXATTRIB4NUB].func)
#define glVertexAttrib4NubARB ((PFNGLVERTEXATTRIB4NUBARBPROC)gl_addr[GLVERTEXATTRIB4NUBARB].func)
#define glVertexAttrib4Nubv ((PFNGLVERTEXATTRIB4NUBVPROC)gl_addr[GLVERTEXATTRIB4NUBV].func)
#define glVertexAttrib4NubvARB ((PFNGLVERTEXATTRIB4NUBVARBPROC)gl_addr[GLVERTEXATTRIB4NUBVARB].func)
#define glVertexAttrib4Nuiv ((PFNGLVERTEXATTRIB4NUIVPROC)gl_addr[GLVERTEXATTRIB4NUIV].func)
#define glVertexAttrib4NuivARB ((PFNGLVERTEXATTRIB4NUIVARBPROC)gl_addr[GLVERTEXATTRIB4NUIVARB].func)
#define glVertexAttrib4Nusv ((PFNGLVERTEXATTRIB4NUSVPROC)gl_addr[GLVERTEXATTRIB4NUSV].func)
#define glVertexAttrib4NusvARB ((PFNGLVERTEXATTRIB4NUSVARBPROC)gl_addr[GLVERTEXATTRIB4NUSVARB].func)
#define glVertexAttrib4s ((PFNGLVERTEXATTRIB4SPROC)gl_addr[GLVERTEXATTRIB4S].func)
#define glVertexAttrib4sARB ((PFNGLVERTEXATTRIB4SARBPROC)gl_addr[GLVERTEXATTRIB4SARB].func)
#define glVertexAttrib4sNV ((PFNGLVERTEXATTRIB4SNVPROC)gl_addr[GLVERTEXATTRIB4SNV].func)
#define glVertexAttrib4sv ((PFNGLVERTEXATTRIB4SVPROC)gl_addr[GLVERTEXATTRIB4SV].func)
#define glVertexAttrib4svARB ((PFNGLVERTEXATTRIB4SVARBPROC)gl_addr[GLVERTEXATTRIB4SVARB].func)
#define glVertexAttrib4svNV ((PFNGLVERTEXATTRIB4SVNVPROC)gl_addr[GLVERTEXATTRIB4SVNV].func)
#define glVertexAttrib4ubNV ((PFNGLVERTEXATTRIB4UBNVPROC)gl_addr[GLVERTEXATTRIB4UBNV].func)
#define glVertexAttrib4ubv ((PFNGLVERTEXATTRIB4UBVPROC)gl_addr[GLVERTEXATTRIB4UBV].func)
#define glVertexAttrib4ubvARB ((PFNGLVERTEXATTRIB4UBVARBPROC)gl_addr[GLVERTEXATTRIB4UBVARB].func)
#define glVertexAttrib4ubvNV ((PFNGLVERTEXATTRIB4UBVNVPROC)gl_addr[GLVERTEXATTRIB4UBVNV].func)
#define glVertexAttrib4uiv ((PFNGLVERTEXATTRIB4UIVPROC)gl_addr[GLVERTEXATTRIB4UIV].func)
#define glVertexAttrib4uivARB ((PFNGLVERTEXATTRIB4UIVARBPROC)gl_addr[GLVERTEXATTRIB4UIVARB].func)
#define glVertexAttrib4usv ((PFNGLVERTEXATTRIB4USVPROC)gl_addr[GLVERTEXATTRIB4USV].func)
#define glVertexAttrib4usvARB ((PFNGLVERTEXATTRIB4USVARBPROC)gl_addr[GLVERTEXATTRIB4USVARB].func)
#define glVertexAttribI1i ((PFNGLVERTEXATTRIBI1IPROC)gl_addr[GLVERTEXATTRIBI1I].func)
#define glVertexAttribI1iEXT ((PFNGLVERTEXATTRIBI1IEXTPROC)gl_addr[GLVERTEXATTRIBI1IEXT].func)
#define glVertexAttribI1iv ((PFNGLVERTEXATTRIBI1IVPROC)gl_addr[GLVERTEXATTRIBI1IV].func)
#define glVertexAttribI1ivEXT ((PFNGLVERTEXATTRIBI1IVEXTPROC)gl_addr[GLVERTEXATTRIBI1IVEXT].func)
#define glVertexAttribI1ui ((PFNGLVERTEXATTRIBI1UIPROC)gl_addr[GLVERTEXATTRIBI1UI].func)
#define glVertexAttribI1uiEXT ((PFNGLVERTEXATTRIBI1UIEXTPROC)gl_addr[GLVERTEXATTRIBI1UIEXT].func)
#define glVertexAttribI1uiv ((PFNGLVERTEXATTRIBI1UIVPROC)gl_addr[GLVERTEXATTRIBI1UIV].func)
#define glVertexAttribI1uivEXT ((PFNGLVERTEXATTRIBI1UIVEXTPROC)gl_addr[GLVERTEXATTRIBI1UIVEXT].func)
#define glVertexAttribI2i ((PFNGLVERTEXATTRIBI2IPROC)gl_addr[GLVERTEXATTRIBI2I].func)
#define glVertexAttribI2iEXT ((PFNGLVERTEXATTRIBI2IEXTPROC)gl_addr[GLVERTEXATTRIBI2IEXT].func)
#define glVertexAttribI2iv ((PFNGLVERTEXATTRIBI2IVPROC)gl_addr[GLVERTEXATTRIBI2IV].func)
#define glVertexAttribI2ivEXT ((PFNGLVERTEXATTRIBI2IVEXTPROC)gl_addr[GLVERTEXATTRIBI2IVEXT].func)
#define glVertexAttribI2ui ((PFNGLVERTEXATTRIBI2UIPROC)gl_addr[GLVERTEXATTRIBI2UI].func)
#define glVertexAttribI2uiEXT ((PFNGLVERTEXATTRIBI2UIEXTPROC)gl_addr[GLVERTEXATTRIBI2UIEXT].func)
#define glVertexAttribI2uiv ((PFNGLVERTEXATTRIBI2UIVPROC)gl_addr[GLVERTEXATTRIBI2UIV].func)
#define glVertexAttribI2uivEXT ((PFNGLVERTEXATTRIBI2UIVEXTPROC)gl_addr[GLVERTEXATTRIBI2UIVEXT].func)
#define glVertexAttribI3i ((PFNGLVERTEXATTRIBI3IPROC)gl_addr[GLVERTEXATTRIBI3I].func)
#define glVertexAttribI3iEXT ((PFNGLVERTEXATTRIBI3IEXTPROC)gl_addr[GLVERTEXATTRIBI3IEXT].func)
#define glVertexAttribI3iv ((PFNGLVERTEXATTRIBI3IVPROC)gl_addr[GLVERTEXATTRIBI3IV].func)
#define glVertexAttribI3ivEXT ((PFNGLVERTEXATTRIBI3IVEXTPROC)gl_addr[GLVERTEXATTRIBI3IVEXT].func)
#define glVertexAttribI3ui ((PFNGLVERTEXATTRIBI3UIPROC)gl_addr[GLVERTEXATTRIBI3UI].func)
#define glVertexAttribI3uiEXT ((PFNGLVERTEXATTRIBI3UIEXTPROC)gl_addr[GLVERTEXATTRIBI3UIEXT].func)
#define glVertexAttribI3uiv ((PFNGLVERTEXATTRIBI3UIVPROC)gl_addr[GLVERTEXATTRIBI3UIV].func)
#define glVertexAttribI3uivEXT ((PFNGLVERTEXATTRIBI3UIVEXTPROC)gl_addr[GLVERTEXATTRIBI3UIVEXT].func)
#define glVertexAttribI4bv ((PFNGLVERTEXATTRIBI4BVPROC)gl_addr[GLVERTEXATTRIBI4BV].func)
#define glVertexAttribI4bvEXT ((PFNGLVERTEXATTRIBI4BVEXTPROC)gl_addr[GLVERTEXATTRIBI4BVEXT].func)
#define glVertexAttribI4i ((PFNGLVERTEXATTRIBI4IPROC)gl_addr[GLVERTEXATTRIBI4I].func)
#define glVertexAttribI4iEXT ((PFNGLVERTEXATTRIBI4IEXTPROC)gl_addr[GLVERTEXATTRIBI4IEXT].func)
#define glVertexAttribI4iv ((PFNGLVERTEXATTRIBI4IVPROC)gl_addr[GLVERTEXATTRIBI4IV].func)
#define glVertexAttribI4ivEXT ((PFNGLVERTEXATTRIBI4IVEXTPROC)gl_addr[GLVERTEXATTRIBI4IVEXT].func)
#define glVertexAttribI4sv ((PFNGLVERTEXATTRIBI4SVPROC)gl_addr[GLVERTEXATTRIBI4SV].func)
#define glVertexAttribI4svEXT ((PFNGLVERTEXATTRIBI4SVEXTPROC)gl_addr[GLVERTEXATTRIBI4SVEXT].func)
#define glVertexAttribI4ubv ((PFNGLVERTEXATTRIBI4UBVPROC)gl_addr[GLVERTEXATTRIBI4UBV].func)
#define glVertexAttribI4ubvEXT ((PFNGLVERTEXATTRIBI4UBVEXTPROC)gl_addr[GLVERTEXATTRIBI4UBVEXT].func)
#define glVertexAttribI4ui ((PFNGLVERTEXATTRIBI4UIPROC)gl_addr[GLVERTEXATTRIBI4UI].func)
#define glVertexAttribI4uiEXT ((PFNGLVERTEXATTRIBI4UIEXTPROC)gl_addr[GLVERTEXATTRIBI4UIEXT].func)
#define glVertexAttribI4uiv ((PFNGLVERTEXATTRIBI4UIVPROC)gl_addr[GLVERTEXATTRIBI4UIV].func)
#define glVertexAttribI4uivEXT ((PFNGLVERTEXATTRIBI4UIVEXTPROC)gl_addr[GLVERTEXATTRIBI4UIVEXT].func)
#define glVertexAttribI4usv ((PFNGLVERTEXATTRIBI4USVPROC)gl_addr[GLVERTEXATTRIBI4USV].func)
#define glVertexAttribI4usvEXT ((PFNGLVERTEXATTRIBI4USVEXTPROC)gl_addr[GLVERTEXATTRIBI4USVEXT].func)
#define glVertexAttribIPointer ((PFNGLVERTEXATTRIBIPOINTERPROC)gl_addr[GLVERTEXATTRIBIPOINTER].func)
#define glVertexAttribIPointerEXT ((PFNGLVERTEXATTRIBIPOINTEREXTPROC)gl_addr[GLVERTEXATTRIBIPOINTEREXT].func)
#define glVertexAttribPointer ((PFNGLVERTEXATTRIBPOINTERPROC)gl_addr[GLVERTEXATTRIBPOINTER].func)
#define glVertexAttribPointerNV ((PFNGLVERTEXATTRIBPOINTERNVPROC)gl_addr[GLVERTEXATTRIBPOINTERNV].func)
#define glVertexAttribs1dvNV ((PFNGLVERTEXATTRIBS1DVNVPROC)gl_addr[GLVERTEXATTRIBS1DVNV].func)
#define glVertexAttribs1fvNV ((PFNGLVERTEXATTRIBS1FVNVPROC)gl_addr[GLVERTEXATTRIBS1FVNV].func)
#define glVertexAttribs1hvNV ((PFNGLVERTEXATTRIBS1HVNVPROC)gl_addr[GLVERTEXATTRIBS1HVNV].func)
#define glVertexAttribs1svNV ((PFNGLVERTEXATTRIBS1SVNVPROC)gl_addr[GLVERTEXATTRIBS1SVNV].func)
#define glVertexAttribs2dvNV ((PFNGLVERTEXATTRIBS2DVNVPROC)gl_addr[GLVERTEXATTRIBS2DVNV].func)
#define glVertexAttribs2fvNV ((PFNGLVERTEXATTRIBS2FVNVPROC)gl_addr[GLVERTEXATTRIBS2FVNV].func)
#define glVertexAttribs2hvNV ((PFNGLVERTEXATTRIBS2HVNVPROC)gl_addr[GLVERTEXATTRIBS2HVNV].func)
#define glVertexAttribs2svNV ((PFNGLVERTEXATTRIBS2SVNVPROC)gl_addr[GLVERTEXATTRIBS2SVNV].func)
#define glVertexAttribs3dvNV ((PFNGLVERTEXATTRIBS3DVNVPROC)gl_addr[GLVERTEXATTRIBS3DVNV].func)
#define glVertexAttribs3fvNV ((PFNGLVERTEXATTRIBS3FVNVPROC)gl_addr[GLVERTEXATTRIBS3FVNV].func)
#define glVertexAttribs3hvNV ((PFNGLVERTEXATTRIBS3HVNVPROC)gl_addr[GLVERTEXATTRIBS3HVNV].func)
#define glVertexAttribs3svNV ((PFNGLVERTEXATTRIBS3SVNVPROC)gl_addr[GLVERTEXATTRIBS3SVNV].func)
#define glVertexAttribs4dvNV ((PFNGLVERTEXATTRIBS4DVNVPROC)gl_addr[GLVERTEXATTRIBS4DVNV].func)
#define glVertexAttribs4fvNV ((PFNGLVERTEXATTRIBS4FVNVPROC)gl_addr[GLVERTEXATTRIBS4FVNV].func)
#define glVertexAttribs4hvNV ((PFNGLVERTEXATTRIBS4HVNVPROC)gl_addr[GLVERTEXATTRIBS4HVNV].func)
#define glVertexAttribs4svNV ((PFNGLVERTEXATTRIBS4SVNVPROC)gl_addr[GLVERTEXATTRIBS4SVNV].func)
#define glVertexAttribs4ubvNV ((PFNGLVERTEXATTRIBS4UBVNVPROC)gl_addr[GLVERTEXATTRIBS4UBVNV].func)
#define glVertexPointerEXT ((PFNGLVERTEXPOINTEREXTPROC)gl_addr[GLVERTEXPOINTEREXT].func)
#define glWindowPos2d ((PFNGLWINDOWPOS2DPROC)gl_addr[GLWINDOWPOS2D].func)
#define glWindowPos2dARB ((PFNGLWINDOWPOS2DARBPROC)gl_addr[GLWINDOWPOS2DARB].func)
#define glWindowPos2dv ((PFNGLWINDOWPOS2DVPROC)gl_addr[GLWINDOWPOS2DV].func)
#define glWindowPos2dvARB ((PFNGLWINDOWPOS2DVARBPROC)gl_addr[GLWINDOWPOS2DVARB].func)
#define glWindowPos2f ((PFNGLWINDOWPOS2FPROC)gl_addr[GLWINDOWPOS2F].func)
#define glWindowPos2fARB ((PFNGLWINDOWPOS2FARBPROC)gl_addr[GLWINDOWPOS2FARB].func)
#define glWindowPos2fv ((PFNGLWINDOWPOS2FVPROC)gl_addr[GLWINDOWPOS2FV].func)
#define glWindowPos2fvARB ((PFNGLWINDOWPOS2FVARBPROC)gl_addr[GLWINDOWPOS2FVARB].func)
#define glWindowPos2i ((PFNGLWINDOWPOS2IPROC)gl_addr[GLWINDOWPOS2I].func)
#define glWindowPos2iARB ((PFNGLWINDOWPOS2IARBPROC)gl_addr[GLWINDOWPOS2IARB].func)
#define glWindowPos2iv ((PFNGLWINDOWPOS2IVPROC)gl_addr[GLWINDOWPOS2IV].func)
#define glWindowPos2ivARB ((PFNGLWINDOWPOS2IVARBPROC)gl_addr[GLWINDOWPOS2IVARB].func)
#define glWindowPos2s ((PFNGLWINDOWPOS2SPROC)gl_addr[GLWINDOWPOS2S].func)
#define glWindowPos2sARB ((PFNGLWINDOWPOS2SARBPROC)gl_addr[GLWINDOWPOS2SARB].func)
#define glWindowPos2sv ((PFNGLWINDOWPOS2SVPROC)gl_addr[GLWINDOWPOS2SV].func)
#define glWindowPos2svARB ((PFNGLWINDOWPOS2SVARBPROC)gl_addr[GLWINDOWPOS2SVARB].func)
#define glWindowPos3d ((PFNGLWINDOWPOS3DPROC)gl_addr[GLWINDOWPOS3D].func)
#define glWindowPos3dARB ((PFNGLWINDOWPOS3DARBPROC)gl_addr[GLWINDOWPOS3DARB].func)
#define glWindowPos3dv ((PFNGLWINDOWPOS3DVPROC)gl_addr[GLWINDOWPOS3DV].func)
#define glWindowPos3dvARB ((PFNGLWINDOWPOS3DVARBPROC)gl_addr[GLWINDOWPOS3DVARB].func)
#define glWindowPos3f ((PFNGLWINDOWPOS3FPROC)gl_addr[GLWINDOWPOS3F].func)
#define glWindowPos3fARB ((PFNGLWINDOWPOS3FARBPROC)gl_addr[GLWINDOWPOS3FARB].func)
#define glWindowPos3fv ((PFNGLWINDOWPOS3FVPROC)gl_addr[GLWINDOWPOS3FV].func)
#define glWindowPos3fvARB ((PFNGLWINDOWPOS3FVARBPROC)gl_addr[GLWINDOWPOS3FVARB].func)
#define glWindowPos3i ((PFNGLWINDOWPOS3IPROC)gl_addr[GLWINDOWPOS3I].func)
#define glWindowPos3iARB ((PFNGLWINDOWPOS3IARBPROC)gl_addr[GLWINDOWPOS3IARB].func)
#define glWindowPos3iv ((PFNGLWINDOWPOS3IVPROC)gl_addr[GLWINDOWPOS3IV].func)
#define glWindowPos3ivARB ((PFNGLWINDOWPOS3IVARBPROC)gl_addr[GLWINDOWPOS3IVARB].func)
#define glWindowPos3s ((PFNGLWINDOWPOS3SPROC)gl_addr[GLWINDOWPOS3S].func)
#define glWindowPos3sARB ((PFNGLWINDOWPOS3SARBPROC)gl_addr[GLWINDOWPOS3SARB].func)
#define glWindowPos3sv ((PFNGLWINDOWPOS3SVPROC)gl_addr[GLWINDOWPOS3SV].func)
#define glWindowPos3svARB ((PFNGLWINDOWPOS3SVARBPROC)gl_addr[GLWINDOWPOS3SVARB].func)
#define wglAllocateMemoryNV ((PFNWGLALLOCATEMEMORYNVPROC)gl_addr[WGLALLOCATEMEMORYNV].func)
#define wglBindSwapBarrierNV ((PFNWGLBINDSWAPBARRIERNVPROC)gl_addr[WGLBINDSWAPBARRIERNV].func)
#define wglBindTexImageARB ((PFNWGLBINDTEXIMAGEARBPROC)gl_addr[WGLBINDTEXIMAGEARB].func)
#define wglBindVideoImageNV ((PFNWGLBINDVIDEOIMAGENVPROC)gl_addr[WGLBINDVIDEOIMAGENV].func)
#define wglChoosePixelFormatARB ((PFNWGLCHOOSEPIXELFORMATARBPROC)gl_addr[WGLCHOOSEPIXELFORMATARB].func)
#define wglCreateBufferRegionARB ((PFNWGLCREATEBUFFERREGIONARBPROC)gl_addr[WGLCREATEBUFFERREGIONARB].func)
#define wglCreateContextAttribsARB ((PFNWGLCREATECONTEXTATTRIBSARBPROC)gl_addr[WGLCREATECONTEXTATTRIBSARB].func)
#define wglCreatePbufferARB ((PFNWGLCREATEPBUFFERARBPROC)gl_addr[WGLCREATEPBUFFERARB].func)
#define wglDeleteBufferRegionARB ((PFNWGLDELETEBUFFERREGIONARBPROC)gl_addr[WGLDELETEBUFFERREGIONARB].func)
#define wglDestroyPbufferARB ((PFNWGLDESTROYPBUFFERARBPROC)gl_addr[WGLDESTROYPBUFFERARB].func)
#define wglDXCloseDeviceNV ((PFNWGLDXCLOSEDEVICENVPROC)gl_addr[WGLDXCLOSEDEVICENV].func)
#define wglDXLockObjectsNV ((PFNWGLDXLOCKOBJECTSNVPROC)gl_addr[WGLDXLOCKOBJECTSNV].func)
#define wglDXOpenDeviceNV ((PFNWGLDXOPENDEVICENVPROC)gl_addr[WGLDXOPENDEVICENV].func)
#define wglDXRegisterObjectNV ((PFNWGLDXREGISTEROBJECTNVPROC)gl_addr[WGLDXREGISTEROBJECTNV].func)
#define wglDXSetResourceShareHandleNV ((PFNWGLDXSETRESOURCESHAREHANDLENVPROC)gl_addr[WGLDXSETRESOURCESHAREHANDLENV].func)
#define wglDXUnlockObjectsNV ((PFNWGLDXUNLOCKOBJECTSNVPROC)gl_addr[WGLDXUNLOCKOBJECTSNV].func)
#define wglDXUnregisterObjectNV ((PFNWGLDXUNREGISTEROBJECTNVPROC)gl_addr[WGLDXUNREGISTEROBJECTNV].func)
#define wglFreeMemoryNV ((PFNWGLFREEMEMORYNVPROC)gl_addr[WGLFREEMEMORYNV].func)
#define wglGetCurrentContext ((PFNWGLGETCURRENTCONTEXTPROC)gl_addr[WGLGETCURRENTCONTEXT].func)
#define wglGetCurrentReadDCARB ((PFNWGLGETCURRENTREADDCARBPROC)gl_addr[WGLGETCURRENTREADDCARB].func)
#define wglGetExtensionsStringARB ((PFNWGLGETEXTENSIONSSTRINGARBPROC)gl_addr[WGLGETEXTENSIONSSTRINGARB].func)
#define wglGetExtensionsStringEXT ((PFNWGLGETEXTENSIONSSTRINGEXTPROC)gl_addr[WGLGETEXTENSIONSSTRINGEXT].func)
#define wglGetPbufferDCARB ((PFNWGLGETPBUFFERDCARBPROC)gl_addr[WGLGETPBUFFERDCARB].func)
#define wglGetPixelFormatAttribfvARB ((PFNWGLGETPIXELFORMATATTRIBFVARBPROC)gl_addr[WGLGETPIXELFORMATATTRIBFVARB].func)
#define wglGetPixelFormatAttribivARB ((PFNWGLGETPIXELFORMATATTRIBIVARBPROC)gl_addr[WGLGETPIXELFORMATATTRIBIVARB].func)
#define wglGetProcAddress ((PFNWGLGETPROCADDRESSPROC)gl_addr[WGLGETPROCADDRESS].func)
#define wglGetSwapIntervalEXT ((PFNWGLGETSWAPINTERVALEXTPROC)gl_addr[WGLGETSWAPINTERVALEXT].func)
#define wglGetVideoDeviceNV ((PFNWGLGETVIDEODEVICENVPROC)gl_addr[WGLGETVIDEODEVICENV].func)
#define wglGetVideoInfoNV ((PFNWGLGETVIDEOINFONVPROC)gl_addr[WGLGETVIDEOINFONV].func)
#define wglJoinSwapGroupNV ((PFNWGLJOINSWAPGROUPNVPROC)gl_addr[WGLJOINSWAPGROUPNV].func)
#define wglMakeContextCurrentARB ((PFNWGLMAKECONTEXTCURRENTARBPROC)gl_addr[WGLMAKECONTEXTCURRENTARB].func)
#define wglQueryFrameCountNV ((PFNWGLQUERYFRAMECOUNTNVPROC)gl_addr[WGLQUERYFRAMECOUNTNV].func)
#define wglQueryMaxSwapGroupsNV ((PFNWGLQUERYMAXSWAPGROUPSNVPROC)gl_addr[WGLQUERYMAXSWAPGROUPSNV].func)
#define wglQueryPbufferARB ((PFNWGLQUERYPBUFFERARBPROC)gl_addr[WGLQUERYPBUFFERARB].func)
#define wglQuerySwapGroupNV ((PFNWGLQUERYSWAPGROUPNVPROC)gl_addr[WGLQUERYSWAPGROUPNV].func)
#define wglReleasePbufferDCARB ((PFNWGLRELEASEPBUFFERDCARBPROC)gl_addr[WGLRELEASEPBUFFERDCARB].func)
#define wglReleaseTexImageARB ((PFNWGLRELEASETEXIMAGEARBPROC)gl_addr[WGLRELEASETEXIMAGEARB].func)
#define wglReleaseVideoDeviceNV ((PFNWGLRELEASEVIDEODEVICENVPROC)gl_addr[WGLRELEASEVIDEODEVICENV].func)
#define wglReleaseVideoImageNV ((PFNWGLRELEASEVIDEOIMAGENVPROC)gl_addr[WGLRELEASEVIDEOIMAGENV].func)
#define wglResetFrameCountNV ((PFNWGLRESETFRAMECOUNTNVPROC)gl_addr[WGLRESETFRAMECOUNTNV].func)
#define wglRestoreBufferRegionARB ((PFNWGLRESTOREBUFFERREGIONARBPROC)gl_addr[WGLRESTOREBUFFERREGIONARB].func)
#define wglSaveBufferRegionARB ((PFNWGLSAVEBUFFERREGIONARBPROC)gl_addr[WGLSAVEBUFFERREGIONARB].func)
#define wglSendPbufferToVideoNV ((PFNWGLSENDPBUFFERTOVIDEONVPROC)gl_addr[WGLSENDPBUFFERTOVIDEONV].func)
#define wglSetPbufferAttribARB ((PFNWGLSETPBUFFERATTRIBARBPROC)gl_addr[WGLSETPBUFFERATTRIBARB].func)
#define wglSwapIntervalEXT ((PFNWGLSWAPINTERVALEXTPROC)gl_addr[WGLSWAPINTERVALEXT].func)

#define glFinishGLUT ((PFNGLFINISHGLUTPROC)gl_addr_glut[GLFINISHGLUT].func)
#define glViewportGLUT ((PFNGLVIEWPORTGLUTPROC)gl_addr_glut[GLVIEWPORTGLUT].func)
#define wglDeleteContextGLUT ((PFNWGLDELETECONTEXTGLUTPROC)gl_addr_glut[WGLDELETECONTEXTGLUT].func)
#define wglGetCurrentContextGLUT ((PFNWGLGETCURRENTCONTEXTGLUTPROC)gl_addr_glut[WGLGETCURRENTCONTEXTGLUT].func)
#define wglGetCurrentDCGLUT ((PFNWGLGETCURRENTDCGLUTPROC)gl_addr_glut[WGLGETCURRENTDCGLUT].func)
#define wglGetProcAddressGLUT ((PFNWGLGETPROCADDRESSGLUTPROC)gl_addr_glut[WGLGETPROCADDRESSGLUT].func)
#define wglMakeCurrentGLUT ((PFNWGLMAKECURRENTGLUTPROC)gl_addr_glut[WGLMAKECURRENTGLUT].func)

extern bool GL_VERSION_4_3;
extern bool GL_VERSION_4_4;
extern bool GL_VERSION_4_5;
extern bool GL_VERSION_4_6;

#ifdef USE_STUB
extern void wrap_addresses(bool stub = false);
#else
extern void wrap_addresses();
#endif
extern void wrap_patch();

extern void GLAPIENTRY glAlphaFuncDLL(GLenum func, GLfloat ref);
extern void GLAPIENTRY glBeginDLL(GLenum mode);
extern void GLAPIENTRY glBindTextureDLL(GLenum target, GLuint texture);
extern void GLAPIENTRY glClearDLL(GLbitfield mask);
extern void GLAPIENTRY glClearColorDLL(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern void GLAPIENTRY glClearDepthDLL(GLdouble depth);
extern void GLAPIENTRY glClearStencilDLL(GLint s);
extern void GLAPIENTRY glColor3fDLL(GLfloat red, GLfloat green, GLfloat blue);
extern void GLAPIENTRY glColor4fDLL(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern void GLAPIENTRY glColor4ubDLL(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
extern void GLAPIENTRY glColor4ubvDLL(GLubyte const * v);
extern void GLAPIENTRY glColorPointerDLL(GLint size, GLenum type, GLsizei stride, const void * pointer);
extern void GLAPIENTRY glCopyTexImage2DDLL(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
extern void GLAPIENTRY glCopyTexSubImage2DDLL(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern void GLAPIENTRY glCullFaceDLL(GLenum mode);
extern void GLAPIENTRY glDepthFuncDLL(GLenum func);
extern void GLAPIENTRY glDisableDLL(GLenum cap);
extern void GLAPIENTRY glDisableClientStateDLL(GLenum array);
extern void GLAPIENTRY glDrawArraysDLL(GLenum mode, GLint first, GLsizei count);
extern void GLAPIENTRY glDrawBufferDLL(GLenum buf);
extern void GLAPIENTRY glDrawElementsDLL(GLenum mode, GLsizei count, GLenum type, const void * indices);
extern void GLAPIENTRY glEnableDLL(GLenum cap);
extern void GLAPIENTRY glEnableClientStateDLL(GLenum array);
extern void GLAPIENTRY glEndDLL();
extern void GLAPIENTRY glFogfDLL(GLenum pname, GLfloat param);
extern void GLAPIENTRY glFogfvDLL(GLenum pname, GLfloat const * params);
extern void GLAPIENTRY glFogiDLL(GLenum pname, GLint param);
extern void GLAPIENTRY glFrontFaceDLL(GLenum mode);
extern void GLAPIENTRY glFrustumDLL(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
extern void GLAPIENTRY glGetBooleanvDLL(GLenum pname, GLboolean * data);
extern GLenum GLAPIENTRY glGetErrorDLL();
extern void GLAPIENTRY glGetFloatvDLL(GLenum pname, GLfloat * data);
extern void GLAPIENTRY glGetIntegervDLL(GLenum pname, GLint * data);
extern void GLAPIENTRY glGetMaterialfvDLL(GLenum face, GLenum pname, GLfloat * params);
extern void GLAPIENTRY glGetTexImageDLL(GLenum target, GLint level, GLenum format, GLenum type, void * pixels);
extern void GLAPIENTRY glGetTexLevelParameterivDLL(GLenum target, GLint level, GLenum pname, GLint * params);
extern void GLAPIENTRY glGetTexParameterivDLL(GLenum target, GLenum pname, GLint * params);
extern GLboolean GLAPIENTRY glIsEnabledDLL(GLenum cap);
extern void GLAPIENTRY glLightfvDLL(GLenum light, GLenum pname, GLfloat const * params);
extern void GLAPIENTRY glLightModelfvDLL(GLenum pname, GLfloat const * params);
extern void GLAPIENTRY glLineWidthDLL(GLfloat width);
extern void GLAPIENTRY glLoadIdentityDLL();
extern void GLAPIENTRY glLoadMatrixfDLL(GLfloat const * m);
extern void GLAPIENTRY glMaterialfDLL(GLenum face, GLenum pname, GLfloat param);
extern void GLAPIENTRY glMaterialfvDLL(GLenum face, GLenum pname, GLfloat const * params);
extern void GLAPIENTRY glMatrixModeDLL(GLenum mode);
extern void GLAPIENTRY glMultMatrixdDLL(GLdouble const * m);
extern void GLAPIENTRY glNormal3fDLL(GLfloat nx, GLfloat ny, GLfloat nz);
extern void GLAPIENTRY glNormalPointerDLL(GLenum type, GLsizei stride, const void * pointer);
extern void GLAPIENTRY glOrthoDLL(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
extern void GLAPIENTRY glPixelStoreiDLL(GLenum pname, GLint param);
extern void GLAPIENTRY glPointSizeDLL(GLfloat size);
extern void GLAPIENTRY glPolygonModeDLL(GLenum face, GLenum mode);
extern void GLAPIENTRY glPopAttribDLL();
extern void GLAPIENTRY glPopClientAttribDLL();
extern void GLAPIENTRY glPopMatrixDLL();
extern void GLAPIENTRY glPushAttribDLL(GLbitfield mask);
extern void GLAPIENTRY glPushClientAttribDLL(GLbitfield mask);
extern void GLAPIENTRY glPushMatrixDLL();
extern void GLAPIENTRY glReadBufferDLL(GLenum src);
extern void GLAPIENTRY glReadPixelsDLL(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void * pixels);
extern void GLAPIENTRY glRotatefDLL(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
extern void GLAPIENTRY glScalefDLL(GLfloat x, GLfloat y, GLfloat z);
extern void GLAPIENTRY glShadeModelDLL(GLenum mode);
extern void GLAPIENTRY glTexCoord2fDLL(GLfloat s, GLfloat t);
extern void GLAPIENTRY glTexCoordPointerDLL(GLint size, GLenum type, GLsizei stride, const void * pointer);
extern void GLAPIENTRY glTexEnvfDLL(GLenum target, GLenum pname, GLfloat param);
extern void GLAPIENTRY glTexEnviDLL(GLenum target, GLenum pname, GLint param);
extern void GLAPIENTRY glTexImage1DDLL(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void * pixels);
extern void GLAPIENTRY glTexImage2DDLL(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void * pixels);
extern void GLAPIENTRY glTexParameterfDLL(GLenum target, GLenum pname, GLfloat param);
extern void GLAPIENTRY glTexParameterfvDLL(GLenum target, GLenum pname, GLfloat const * params);
extern void GLAPIENTRY glTexParameteriDLL(GLenum target, GLenum pname, GLint param);
extern void GLAPIENTRY glTexSubImage2DDLL(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * pixels);
extern void GLAPIENTRY glTranslatefDLL(GLfloat x, GLfloat y, GLfloat z);
extern void GLAPIENTRY glVertex3fDLL(GLfloat x, GLfloat y, GLfloat z);
extern void GLAPIENTRY glVertex3fvDLL(GLfloat const * v);
extern void GLAPIENTRY glVertexPointerDLL(GLint size, GLenum type, GLsizei stride, const void * pointer);
extern void GLAPIENTRY glActiveTexture(GLenum texture);
extern void GLAPIENTRY glBindBufferARB(GLenum target, GLuint buffer);
extern void GLAPIENTRY glBindBufferBaseNV(GLenum target, GLuint index, GLuint buffer);
extern void GLAPIENTRY glBindFramebufferEXT(GLenum target, GLuint framebuffer);
extern void GLAPIENTRY glBindMultiTextureEXT(GLenum texunit, GLenum target, GLuint texture);
extern void GLAPIENTRY glBindRenderbufferEXT(GLenum target, GLuint renderbuffer);
extern void GLAPIENTRY glBufferDataARB(GLenum target, GLsizeiptrARB size, const void * data, GLenum usage);
extern void GLAPIENTRY glClampColorARB(GLenum target, GLenum clamp);
extern void GLAPIENTRY glClientActiveTexture(GLenum texture);
extern void GLAPIENTRY glCopyMultiTexSubImage2DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern void GLAPIENTRY glDeleteBuffersARB(GLsizei n, GLuint const * buffers);
extern void GLAPIENTRY glDeleteFramebuffersEXT(GLsizei n, GLuint const * framebuffers);
extern void GLAPIENTRY glDeleteRenderbuffersEXT(GLsizei n, GLuint const * renderbuffers);
extern void GLAPIENTRY glDeleteTextures(GLsizei n, GLuint const * textures);
extern void GLAPIENTRY glDisableVertexAttribArrayARB(GLuint index);
extern void GLAPIENTRY glDrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void * indices);
extern void GLAPIENTRY glEnableVertexAttribArray(GLuint index);
extern void GLAPIENTRY glEnableVertexAttribArrayARB(GLuint index);
extern void GLAPIENTRY glFramebufferRenderbufferEXT(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
extern void GLAPIENTRY glFramebufferTexture2DEXT(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
extern void GLAPIENTRY glGenFramebuffersEXT(GLsizei n, GLuint * framebuffers);
extern void GLAPIENTRY glGenRenderbuffersEXT(GLsizei n, GLuint * renderbuffers);
extern void GLAPIENTRY glGenTextures(GLsizei n, GLuint * textures);
extern void GLAPIENTRY glGetProgramEnvParameterfvARB(GLenum target, GLuint index, GLfloat * params);
extern void GLAPIENTRY glGetProgramivARB(GLenum target, GLenum pname, GLint * params);
extern void GLAPIENTRY glGetTextureImageEXT(GLuint texture, GLenum target, GLint level, GLenum format, GLenum type, void * pixels);
extern GLboolean GLAPIENTRY glIsBufferARB(GLuint buffer);
extern void GLAPIENTRY glLoadTransposeMatrixf(GLfloat const * m);
extern void * GLAPIENTRY glMapNamedBufferEXT(GLuint buffer, GLenum access);
extern void GLAPIENTRY glMultiTexCoord2f(GLenum target, GLfloat s, GLfloat t);
extern void GLAPIENTRY glMultiTexCoord2fv(GLenum target, GLfloat const * v);
extern void GLAPIENTRY glMultTransposeMatrixf(GLfloat const * m);
extern void GLAPIENTRY glNamedBufferDataEXT(GLuint buffer, GLsizeiptr size, const void * data, GLenum usage);
extern void GLAPIENTRY glNamedBufferSubDataEXT(GLuint buffer, GLintptr offset, GLsizeiptr size, const void * data);
extern void GLAPIENTRY glPointParameterf(GLenum pname, GLfloat param);
extern void GLAPIENTRY glPointParameterfv(GLenum pname, GLfloat const * params);
extern void GLAPIENTRY glPointParameteri(GLenum pname, GLint param);
extern void GLAPIENTRY glPrimitiveRestartIndexNV(GLuint index);
extern void GLAPIENTRY glProgramBufferParametersfvNV(GLenum target, GLuint bindingIndex, GLuint wordIndex, GLsizei count, GLfloat const * params);
extern void GLAPIENTRY glProgramEnvParameter4fARB(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void GLAPIENTRY glProgramEnvParameter4fvARB(GLenum target, GLuint index, GLfloat const * params);
extern void GLAPIENTRY glProgramEnvParameters4fvEXT(GLenum target, GLuint index, GLsizei count, GLfloat const * params);
extern void GLAPIENTRY glProgramLocalParameter4fARB(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void GLAPIENTRY glProgramLocalParameter4fvARB(GLenum target, GLuint index, GLfloat const * params);
extern void GLAPIENTRY glProgramLocalParameters4fvEXT(GLenum target, GLuint index, GLsizei count, GLfloat const * params);
extern void GLAPIENTRY glRenderbufferStorageEXT(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
extern void GLAPIENTRY glRenderbufferStorageMultisampleCoverageNV(GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height);
extern GLboolean GLAPIENTRY glUnmapNamedBufferEXT(GLuint buffer);
extern void GLAPIENTRY glUseProgram(GLuint program);
extern void GLAPIENTRY glVertexAttribPointerARB(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer);
extern void GLAPIENTRY glBeginGLUT(GLenum mode);
extern void GLAPIENTRY glBitmapGLUT(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, GLubyte const * bitmap);
extern void GLAPIENTRY glDrawBufferGLUT(GLenum buf);
extern void GLAPIENTRY glEnableGLUT(GLenum cap);
extern void GLAPIENTRY glEndGLUT();
extern void GLAPIENTRY glEvalMesh2GLUT(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
extern GLenum GLAPIENTRY glGetErrorGLUT();
extern void GLAPIENTRY glGetIntegervGLUT(GLenum pname, GLint * data);
extern GLubyte const *  GLAPIENTRY glGetStringGLUT(GLenum name);
extern void GLAPIENTRY glMap2fGLUT(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, GLfloat const * points);
extern void GLAPIENTRY glMapGrid2fGLUT(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
extern void GLAPIENTRY glNormal3fGLUT(GLfloat nx, GLfloat ny, GLfloat nz);
extern void GLAPIENTRY glNormal3fvGLUT(GLfloat const * v);
extern void GLAPIENTRY glPixelStoreiGLUT(GLenum pname, GLint param);
extern void GLAPIENTRY glPolygonModeGLUT(GLenum face, GLenum mode);
extern void GLAPIENTRY glPopAttribGLUT();
extern void GLAPIENTRY glPopMatrixGLUT();
extern void GLAPIENTRY glPushAttribGLUT(GLbitfield mask);
extern void GLAPIENTRY glPushMatrixGLUT();
extern void GLAPIENTRY glReadBufferGLUT(GLenum src);
extern void GLAPIENTRY glRotatefGLUT(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
extern void GLAPIENTRY glScalefGLUT(GLfloat x, GLfloat y, GLfloat z);
extern void GLAPIENTRY glTranslatefGLUT(GLfloat x, GLfloat y, GLfloat z);
extern void GLAPIENTRY glVertex2fGLUT(GLfloat x, GLfloat y);
extern void GLAPIENTRY glVertex3fGLUT(GLfloat x, GLfloat y, GLfloat z);
extern void GLAPIENTRY glVertex3fvGLUT(GLfloat const * v);
extern HGLRC GLAPIENTRY wglCreateContextGLUT(HDC hDc);

extern void GLAPIENTRY glTexParameteriv(GLenum target, GLenum pname, const GLint* params);
extern void GLAPIENTRY glStencilMask(GLuint mask);
extern void GLAPIENTRY glPrimitiveRestartIndex(GLuint index);
extern void GLAPIENTRY glClearDepthf(GLfloat d);
extern void GLAPIENTRY glGetProgramBinary(GLuint program, GLsizei bufSize, GLsizei* length, GLenum* binaryFormat, void* binary);
extern void GLAPIENTRY glProgramBinary(GLuint program, GLenum binaryFormat, const void* binary, GLsizei length);
extern void GLAPIENTRY glBufferStorage(GLenum target, GLsizeiptr size, const void* data, GLbitfield flags);
extern void GLAPIENTRY glCreateBuffers(GLsizei n, GLuint* buffers);
extern void GLAPIENTRY glNamedBufferStorage(GLuint buffer, GLsizeiptr size, const void* data, GLbitfield flags);
extern void GLAPIENTRY glNamedBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr size, const void* data);
extern void* GLAPIENTRY glMapNamedBuffer(GLuint buffer, GLenum access);
extern void* GLAPIENTRY glMapNamedBufferRange(GLuint buffer, GLintptr offset, GLsizeiptr length, GLenum access);
extern GLboolean GLAPIENTRY glUnmapNamedBuffer(GLuint buffer);
extern void GLAPIENTRY glTextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);

extern constexpr void GLAPIENTRY glPushDebugGroup(GLenum source, GLuint id, GLsizei length, const GLchar* message);
extern constexpr void GLAPIENTRY glPopDebugGroup();
