/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "wrap.hpp"
#include "../KKdLib/half_t.hpp"
#include "../KKdLib/mat.hpp"
#include "../KKdLib/vec.hpp"
#include "inject.hpp"
#include "print.hpp"
#include "uniform.hpp"
#include <Helpers.h>
#include <time.h>

struct func_struct {
    size_t address;
    size_t orig_func;
    size_t func;
};

#define GLALPHAFUNCDLL 0
#define GLBEGINDLL 1
#define GLBINDTEXTUREDLL 2
#define GLCLEARDLL 3
#define GLCLEARCOLORDLL 4
#define GLCLEARDEPTHDLL 5
#define GLCLEARSTENCILDLL 6
#define GLCOLOR3FDLL 7
#define GLCOLOR4FDLL 8
#define GLCOLOR4UBDLL 9
#define GLCOLOR4UBVDLL 10
#define GLCOLORPOINTERDLL 11
#define GLCOPYTEXIMAGE2DDLL 12
#define GLCOPYTEXSUBIMAGE2DDLL 13
#define GLCULLFACEDLL 14
#define GLDEPTHFUNCDLL 15
#define GLDISABLEDLL 16
#define GLDISABLECLIENTSTATEDLL 17
#define GLDRAWARRAYSDLL 18
#define GLDRAWBUFFERDLL 19
#define GLDRAWELEMENTSDLL 20
#define GLENABLEDLL 21
#define GLENABLECLIENTSTATEDLL 22
#define GLENDDLL 23
#define GLFOGFDLL 24
#define GLFOGFVDLL 25
#define GLFOGIDLL 26
#define GLFRONTFACEDLL 27
#define GLFRUSTUMDLL 28
#define GLGETBOOLEANVDLL 29
#define GLGETERRORDLL 30
#define GLGETFLOATVDLL 31
#define GLGETINTEGERVDLL 32
#define GLGETMATERIALFVDLL 33
#define GLGETTEXIMAGEDLL 34
#define GLGETTEXLEVELPARAMETERIVDLL 35
#define GLGETTEXPARAMETERIVDLL 36
#define GLISENABLEDDLL 37
#define GLLIGHTFVDLL 38
#define GLLIGHTMODELFVDLL 39
#define GLLINEWIDTHDLL 40
#define GLLOADIDENTITYDLL 41
#define GLLOADMATRIXFDLL 42
#define GLMATERIALFDLL 43
#define GLMATERIALFVDLL 44
#define GLMATRIXMODEDLL 45
#define GLMULTMATRIXDDLL 46
#define GLNORMAL3FDLL 47
#define GLNORMALPOINTERDLL 48
#define GLORTHODLL 49
#define GLPIXELSTOREIDLL 50
#define GLPOINTSIZEDLL 51
#define GLPOLYGONMODEDLL 52
#define GLPOPATTRIBDLL 53
#define GLPOPCLIENTATTRIBDLL 54
#define GLPOPMATRIXDLL 55
#define GLPUSHATTRIBDLL 56
#define GLPUSHCLIENTATTRIBDLL 57
#define GLPUSHMATRIXDLL 58
#define GLREADBUFFERDLL 59
#define GLREADPIXELSDLL 60
#define GLROTATEFDLL 61
#define GLSCALEFDLL 62
#define GLSHADEMODELDLL 63
#define GLTEXCOORD2FDLL 64
#define GLTEXCOORDPOINTERDLL 65
#define GLTEXENVFDLL 66
#define GLTEXENVIDLL 67
#define GLTEXIMAGE1DDLL 68
#define GLTEXIMAGE2DDLL 69
#define GLTEXPARAMETERFDLL 70
#define GLTEXPARAMETERFVDLL 71
#define GLTEXPARAMETERIDLL 72
#define GLTEXSUBIMAGE2DDLL 73
#define GLTRANSLATEFDLL 74
#define GLVERTEX3FDLL 75
#define GLVERTEX3FVDLL 76
#define GLVERTEXPOINTERDLL 77

#define GLACTIVETEXTURE 0
#define GLBINDBUFFERARB 1
#define GLBINDBUFFERBASENV 2
#define GLBINDFRAMEBUFFEREXT 3
#define GLBINDMULTITEXTUREEXT 4
#define GLBINDRENDERBUFFEREXT 5
#define GLBUFFERDATAARB 6
#define GLCLAMPCOLORARB 7
#define GLCLIENTACTIVETEXTURE 8
#define GLCOPYMULTITEXSUBIMAGE2DEXT 9
#define GLDELETEBUFFERSARB 10
#define GLDELETEFRAMEBUFFERSEXT 11
#define GLDELETERENDERBUFFERSEXT 12
#define GLDELETETEXTURES 13
#define GLDISABLEVERTEXATTRIBARRAYARB 14
#define GLDRAWRANGEELEMENTS 15
#define GLENABLEVERTEXATTRIBARRAY 16
#define GLENABLEVERTEXATTRIBARRAYARB 17
#define GLFRAMEBUFFERRENDERBUFFEREXT 18
#define GLFRAMEBUFFERTEXTURE2DEXT 19
#define GLGENFRAMEBUFFERSEXT 20
#define GLGENRENDERBUFFERSEXT 21
#define GLGENTEXTURES 22
#define GLGETPROGRAMENVPARAMETERFVARB 23
#define GLGETPROGRAMIVARB 24
#define GLGETTEXTUREIMAGEEXT 25
#define GLISBUFFERARB 26
#define GLLOADTRANSPOSEMATRIXF 27
#define GLMAPNAMEDBUFFEREXT 28
#define GLMULTITEXCOORD2F 29
#define GLMULTITEXCOORD2FV 30
#define GLMULTTRANSPOSEMATRIXF 31
#define GLNAMEDBUFFERDATAEXT 32
#define GLNAMEDBUFFERSUBDATAEXT 33
#define GLPOINTPARAMETERF 34
#define GLPOINTPARAMETERFV 35
#define GLPOINTPARAMETERI 36
#define GLPRIMITIVERESTARTINDEXNV 37
#define GLPROGRAMBUFFERPARAMETERSFVNV 38
#define GLPROGRAMENVPARAMETER4FARB 39
#define GLPROGRAMENVPARAMETER4FVARB 40
#define GLPROGRAMENVPARAMETERS4FVEXT 41
#define GLPROGRAMLOCALPARAMETER4FARB 42
#define GLPROGRAMLOCALPARAMETER4FVARB 43
#define GLPROGRAMLOCALPARAMETERS4FVEXT 44
#define GLRENDERBUFFERSTORAGEEXT 45
#define GLRENDERBUFFERSTORAGEMULTISAMPLECOVERAGENV 46
#define GLUNMAPNAMEDBUFFEREXT 47
#define GLUSEPROGRAM 48
#define GLVERTEXATTRIBPOINTERARB 49

#define GLBEGINGLUT 0
#define GLBITMAPGLUT 1
#define GLDRAWBUFFERGLUT 2
#define GLENABLEGLUT 3
#define GLENDGLUT 4
#define GLEVALMESH2GLUT 5
#define GLGETERRORGLUT 6
#define GLGETINTEGERVGLUT 7
#define GLGETSTRINGGLUT 8
#define GLMAP2FGLUT 9
#define GLMAPGRID2FGLUT 10
#define GLNORMAL3FGLUT 11
#define GLNORMAL3FVGLUT 12
#define GLPIXELSTOREIGLUT 13
#define GLPOLYGONMODEGLUT 14
#define GLPOPATTRIBGLUT 15
#define GLPOPMATRIXGLUT 16
#define GLPUSHATTRIBGLUT 17
#define GLPUSHMATRIXGLUT 18
#define GLREADBUFFERGLUT 19
#define GLROTATEFGLUT 20
#define GLSCALEFGLUT 21
#define GLTRANSLATEFGLUT 22
#define GLVERTEX2FGLUT 23
#define GLVERTEX3FGLUT 24
#define GLVERTEX3FVGLUT 25
#define WGLCREATECONTEXTGLUT 26

typedef void (GLAPIENTRY* PFNGLALPHAFUNCDLLPROC)(GLenum func, GLfloat ref);
typedef void (GLAPIENTRY* PFNGLBEGINDLLPROC)(GLenum mode);
typedef void (GLAPIENTRY* PFNGLBINDTEXTUREDLLPROC)(GLenum target, GLuint texture);
typedef void (GLAPIENTRY* PFNGLCLEARDLLPROC)(GLbitfield mask);
typedef void (GLAPIENTRY* PFNGLCLEARCOLORDLLPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (GLAPIENTRY* PFNGLCLEARDEPTHDLLPROC)(GLdouble depth);
typedef void (GLAPIENTRY* PFNGLCLEARSTENCILDLLPROC)(GLint s);
typedef void (GLAPIENTRY* PFNGLCOLOR3FDLLPROC)(GLfloat red, GLfloat green, GLfloat blue);
typedef void (GLAPIENTRY* PFNGLCOLOR4FDLLPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (GLAPIENTRY* PFNGLCOLOR4UBDLLPROC)(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
typedef void (GLAPIENTRY* PFNGLCOLOR4UBVDLLPROC)(GLubyte const * v);
typedef void (GLAPIENTRY* PFNGLCOLORPOINTERDLLPROC)(GLint size, GLenum type, GLsizei stride, const void * pointer);
typedef void (GLAPIENTRY* PFNGLCOPYTEXIMAGE2DDLLPROC)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void (GLAPIENTRY* PFNGLCOPYTEXSUBIMAGE2DDLLPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY* PFNGLCULLFACEDLLPROC)(GLenum mode);
typedef void (GLAPIENTRY* PFNGLDEPTHFUNCDLLPROC)(GLenum func);
typedef void (GLAPIENTRY* PFNGLDISABLEDLLPROC)(GLenum cap);
typedef void (GLAPIENTRY* PFNGLDISABLECLIENTSTATEDLLPROC)(GLenum array);
typedef void (GLAPIENTRY* PFNGLDRAWARRAYSDLLPROC)(GLenum mode, GLint first, GLsizei count);
typedef void (GLAPIENTRY* PFNGLDRAWBUFFERDLLPROC)(GLenum buf);
typedef void (GLAPIENTRY* PFNGLDRAWELEMENTSDLLPROC)(GLenum mode, GLsizei count, GLenum type, const void * indices);
typedef void (GLAPIENTRY* PFNGLENABLEDLLPROC)(GLenum cap);
typedef void (GLAPIENTRY* PFNGLENABLECLIENTSTATEDLLPROC)(GLenum array);
typedef void (GLAPIENTRY* PFNGLENDDLLPROC)();
typedef void (GLAPIENTRY* PFNGLFOGFDLLPROC)(GLenum pname, GLfloat param);
typedef void (GLAPIENTRY* PFNGLFOGFVDLLPROC)(GLenum pname, GLfloat const * params);
typedef void (GLAPIENTRY* PFNGLFOGIDLLPROC)(GLenum pname, GLint param);
typedef void (GLAPIENTRY* PFNGLFRONTFACEDLLPROC)(GLenum mode);
typedef void (GLAPIENTRY* PFNGLFRUSTUMDLLPROC)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
typedef void (GLAPIENTRY* PFNGLGETBOOLEANVDLLPROC)(GLenum pname, GLboolean * data);
typedef GLenum (GLAPIENTRY* PFNGLGETERRORDLLPROC)();
typedef void (GLAPIENTRY* PFNGLGETFLOATVDLLPROC)(GLenum pname, GLfloat * data);
typedef void (GLAPIENTRY* PFNGLGETINTEGERVDLLPROC)(GLenum pname, GLint * data);
typedef void (GLAPIENTRY* PFNGLGETMATERIALFVDLLPROC)(GLenum face, GLenum pname, GLfloat * params);
typedef void (GLAPIENTRY* PFNGLGETTEXIMAGEDLLPROC)(GLenum target, GLint level, GLenum format, GLenum type, void * pixels);
typedef void (GLAPIENTRY* PFNGLGETTEXLEVELPARAMETERIVDLLPROC)(GLenum target, GLint level, GLenum pname, GLint * params);
typedef void (GLAPIENTRY* PFNGLGETTEXPARAMETERIVDLLPROC)(GLenum target, GLenum pname, GLint * params);
typedef GLboolean (GLAPIENTRY* PFNGLISENABLEDDLLPROC)(GLenum cap);
typedef void (GLAPIENTRY* PFNGLLIGHTFVDLLPROC)(GLenum light, GLenum pname, GLfloat const * params);
typedef void (GLAPIENTRY* PFNGLLIGHTMODELFVDLLPROC)(GLenum pname, GLfloat const * params);
typedef void (GLAPIENTRY* PFNGLLINEWIDTHDLLPROC)(GLfloat width);
typedef void (GLAPIENTRY* PFNGLLOADIDENTITYDLLPROC)();
typedef void (GLAPIENTRY* PFNGLLOADMATRIXFDLLPROC)(GLfloat const * m);
typedef void (GLAPIENTRY* PFNGLMATERIALFDLLPROC)(GLenum face, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY* PFNGLMATERIALFVDLLPROC)(GLenum face, GLenum pname, GLfloat const * params);
typedef void (GLAPIENTRY* PFNGLMATRIXMODEDLLPROC)(GLenum mode);
typedef void (GLAPIENTRY* PFNGLMULTMATRIXDDLLPROC)(GLdouble const * m);
typedef void (GLAPIENTRY* PFNGLNORMAL3FDLLPROC)(GLfloat nx, GLfloat ny, GLfloat nz);
typedef void (GLAPIENTRY* PFNGLNORMALPOINTERDLLPROC)(GLenum type, GLsizei stride, const void * pointer);
typedef void (GLAPIENTRY* PFNGLORTHODLLPROC)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
typedef void (GLAPIENTRY* PFNGLPIXELSTOREIDLLPROC)(GLenum pname, GLint param);
typedef void (GLAPIENTRY* PFNGLPOINTSIZEDLLPROC)(GLfloat size);
typedef void (GLAPIENTRY* PFNGLPOLYGONMODEDLLPROC)(GLenum face, GLenum mode);
typedef void (GLAPIENTRY* PFNGLPOPATTRIBDLLPROC)();
typedef void (GLAPIENTRY* PFNGLPOPCLIENTATTRIBDLLPROC)();
typedef void (GLAPIENTRY* PFNGLPOPMATRIXDLLPROC)();
typedef void (GLAPIENTRY* PFNGLPUSHATTRIBDLLPROC)(GLbitfield mask);
typedef void (GLAPIENTRY* PFNGLPUSHCLIENTATTRIBDLLPROC)(GLbitfield mask);
typedef void (GLAPIENTRY* PFNGLPUSHMATRIXDLLPROC)();
typedef void (GLAPIENTRY* PFNGLREADBUFFERDLLPROC)(GLenum src);
typedef void (GLAPIENTRY* PFNGLREADPIXELSDLLPROC)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void * pixels);
typedef void (GLAPIENTRY* PFNGLROTATEFDLLPROC)(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY* PFNGLSCALEFDLLPROC)(GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY* PFNGLSHADEMODELDLLPROC)(GLenum mode);
typedef void (GLAPIENTRY* PFNGLTEXCOORD2FDLLPROC)(GLfloat s, GLfloat t);
typedef void (GLAPIENTRY* PFNGLTEXCOORDPOINTERDLLPROC)(GLint size, GLenum type, GLsizei stride, const void * pointer);
typedef void (GLAPIENTRY* PFNGLTEXENVFDLLPROC)(GLenum target, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY* PFNGLTEXENVIDLLPROC)(GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY* PFNGLTEXIMAGE1DDLLPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void * pixels);
typedef void (GLAPIENTRY* PFNGLTEXIMAGE2DDLLPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void * pixels);
typedef void (GLAPIENTRY* PFNGLTEXPARAMETERFDLLPROC)(GLenum target, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY* PFNGLTEXPARAMETERFVDLLPROC)(GLenum target, GLenum pname, GLfloat const * params);
typedef void (GLAPIENTRY* PFNGLTEXPARAMETERIDLLPROC)(GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY* PFNGLTEXSUBIMAGE2DDLLPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * pixels);
typedef void (GLAPIENTRY* PFNGLTRANSLATEFDLLPROC)(GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY* PFNGLVERTEX3FDLLPROC)(GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY* PFNGLVERTEX3FVDLLPROC)(GLfloat const * v);
typedef void (GLAPIENTRY* PFNGLVERTEXPOINTERDLLPROC)(GLint size, GLenum type, GLsizei stride, const void * pointer);

typedef void (GLAPIENTRY* PFNGLACTIVETEXTUREPROC)(GLenum texture);
typedef void (GLAPIENTRY* PFNGLBINDBUFFERARBPROC)(GLenum target, GLuint buffer);
typedef void (GLAPIENTRY* PFNGLBINDBUFFERBASENVPROC)(GLenum target, GLuint index, GLuint buffer);
typedef void (GLAPIENTRY* PFNGLBINDFRAMEBUFFEREXTPROC)(GLenum target, GLuint framebuffer);
typedef void (GLAPIENTRY* PFNGLBINDMULTITEXTUREEXTPROC)(GLenum texunit, GLenum target, GLuint texture);
typedef void (GLAPIENTRY* PFNGLBINDRENDERBUFFEREXTPROC)(GLenum target, GLuint renderbuffer);
typedef void (GLAPIENTRY* PFNGLBUFFERDATAARBPROC)(GLenum target, GLsizeiptrARB size, const void * data, GLenum usage);
typedef void (GLAPIENTRY* PFNGLCLAMPCOLORARBPROC)(GLenum target, GLenum clamp);
typedef void (GLAPIENTRY* PFNGLCLIENTACTIVETEXTUREPROC)(GLenum texture);
typedef void (GLAPIENTRY* PFNGLCOPYMULTITEXSUBIMAGE2DEXTPROC)(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY* PFNGLDELETEBUFFERSARBPROC)(GLsizei n, GLuint const * buffers);
typedef void (GLAPIENTRY* PFNGLDELETEFRAMEBUFFERSEXTPROC)(GLsizei n, GLuint const * framebuffers);
typedef void (GLAPIENTRY* PFNGLDELETERENDERBUFFERSEXTPROC)(GLsizei n, GLuint const * renderbuffers);
typedef void (GLAPIENTRY* PFNGLDELETETEXTURESPROC)(GLsizei n, GLuint const * textures);
typedef void (GLAPIENTRY* PFNGLDISABLEVERTEXATTRIBARRAYARBPROC)(GLuint index);
typedef void (GLAPIENTRY* PFNGLDRAWRANGEELEMENTSPROC)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void * indices);
typedef void (GLAPIENTRY* PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void (GLAPIENTRY* PFNGLENABLEVERTEXATTRIBARRAYARBPROC)(GLuint index);
typedef void (GLAPIENTRY* PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (GLAPIENTRY* PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY* PFNGLGENFRAMEBUFFERSEXTPROC)(GLsizei n, GLuint * framebuffers);
typedef void (GLAPIENTRY* PFNGLGENRENDERBUFFERSEXTPROC)(GLsizei n, GLuint * renderbuffers);
typedef void (GLAPIENTRY* PFNGLGENTEXTURESPROC)(GLsizei n, GLuint * textures);
typedef void (GLAPIENTRY* PFNGLGETPROGRAMENVPARAMETERFVARBPROC)(GLenum target, GLuint index, GLfloat * params);
typedef void (GLAPIENTRY* PFNGLGETPROGRAMIVARBPROC)(GLenum target, GLenum pname, GLint * params);
typedef void (GLAPIENTRY* PFNGLGETTEXTUREIMAGEEXTPROC)(GLuint texture, GLenum target, GLint level, GLenum format, GLenum type, void * pixels);
typedef GLboolean (GLAPIENTRY* PFNGLISBUFFERARBPROC)(GLuint buffer);
typedef void (GLAPIENTRY* PFNGLLOADTRANSPOSEMATRIXFPROC)(GLfloat const * m);
typedef void * (GLAPIENTRY* PFNGLMAPNAMEDBUFFEREXTPROC)(GLuint buffer, GLenum access);
typedef void (GLAPIENTRY* PFNGLMULTITEXCOORD2FPROC)(GLenum target, GLfloat s, GLfloat t);
typedef void (GLAPIENTRY* PFNGLMULTITEXCOORD2FVPROC)(GLenum target, GLfloat const * v);
typedef void (GLAPIENTRY* PFNGLMULTTRANSPOSEMATRIXFPROC)(GLfloat const * m);
typedef void (GLAPIENTRY* PFNGLNAMEDBUFFERDATAEXTPROC)(GLuint buffer, GLsizeiptr size, const void * data, GLenum usage);
typedef void (GLAPIENTRY* PFNGLNAMEDBUFFERSUBDATAEXTPROC)(GLuint buffer, GLintptr offset, GLsizeiptr size, const void * data);
typedef void (GLAPIENTRY* PFNGLPOINTPARAMETERFPROC)(GLenum pname, GLfloat param);
typedef void (GLAPIENTRY* PFNGLPOINTPARAMETERFVPROC)(GLenum pname, GLfloat const * params);
typedef void (GLAPIENTRY* PFNGLPOINTPARAMETERIPROC)(GLenum pname, GLint param);
typedef void (GLAPIENTRY* PFNGLPRIMITIVERESTARTINDEXNVPROC)(GLuint index);
typedef void (GLAPIENTRY* PFNGLPROGRAMBUFFERPARAMETERSFVNVPROC)(GLenum target, GLuint bindingIndex, GLuint wordIndex, GLsizei count, GLfloat const * params);
typedef void (GLAPIENTRY* PFNGLPROGRAMENVPARAMETER4FARBPROC)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY* PFNGLPROGRAMENVPARAMETER4FVARBPROC)(GLenum target, GLuint index, GLfloat const * params);
typedef void (GLAPIENTRY* PFNGLPROGRAMENVPARAMETERS4FVEXTPROC)(GLenum target, GLuint index, GLsizei count, GLfloat const * params);
typedef void (GLAPIENTRY* PFNGLPROGRAMLOCALPARAMETER4FARBPROC)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY* PFNGLPROGRAMLOCALPARAMETER4FVARBPROC)(GLenum target, GLuint index, GLfloat const * params);
typedef void (GLAPIENTRY* PFNGLPROGRAMLOCALPARAMETERS4FVEXTPROC)(GLenum target, GLuint index, GLsizei count, GLfloat const * params);
typedef void (GLAPIENTRY* PFNGLRENDERBUFFERSTORAGEEXTPROC)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY* PFNGLRENDERBUFFERSTORAGEMULTISAMPLECOVERAGENVPROC)(GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height);
typedef GLboolean (GLAPIENTRY* PFNGLUNMAPNAMEDBUFFEREXTPROC)(GLuint buffer);
typedef void (GLAPIENTRY* PFNGLUSEPROGRAMPROC)(GLuint program);
typedef void (GLAPIENTRY* PFNGLVERTEXATTRIBPOINTERARBPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer);

typedef void (GLAPIENTRY* PFNGLBEGINGLUTPROC)(GLenum mode);
typedef void (GLAPIENTRY* PFNGLBITMAPGLUTPROC)(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, GLubyte const * bitmap);
typedef void (GLAPIENTRY* PFNGLDRAWBUFFERGLUTPROC)(GLenum buf);
typedef void (GLAPIENTRY* PFNGLENABLEGLUTPROC)(GLenum cap);
typedef void (GLAPIENTRY* PFNGLENDGLUTPROC)();
typedef void (GLAPIENTRY* PFNGLEVALMESH2GLUTPROC)(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
typedef GLenum (GLAPIENTRY* PFNGLGETERRORGLUTPROC)();
typedef void (GLAPIENTRY* PFNGLGETINTEGERVGLUTPROC)(GLenum pname, GLint * data);
typedef GLubyte const *  (GLAPIENTRY* PFNGLGETSTRINGGLUTPROC)(GLenum name);
typedef void (GLAPIENTRY* PFNGLMAP2FGLUTPROC)(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, GLfloat const * points);
typedef void (GLAPIENTRY* PFNGLMAPGRID2FGLUTPROC)(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
typedef void (GLAPIENTRY* PFNGLNORMAL3FGLUTPROC)(GLfloat nx, GLfloat ny, GLfloat nz);
typedef void (GLAPIENTRY* PFNGLNORMAL3FVGLUTPROC)(GLfloat const * v);
typedef void (GLAPIENTRY* PFNGLPIXELSTOREIGLUTPROC)(GLenum pname, GLint param);
typedef void (GLAPIENTRY* PFNGLPOLYGONMODEGLUTPROC)(GLenum face, GLenum mode);
typedef void (GLAPIENTRY* PFNGLPOPATTRIBGLUTPROC)();
typedef void (GLAPIENTRY* PFNGLPOPMATRIXGLUTPROC)();
typedef void (GLAPIENTRY* PFNGLPUSHATTRIBGLUTPROC)(GLbitfield mask);
typedef void (GLAPIENTRY* PFNGLPUSHMATRIXGLUTPROC)();
typedef void (GLAPIENTRY* PFNGLREADBUFFERGLUTPROC)(GLenum src);
typedef void (GLAPIENTRY* PFNGLROTATEFGLUTPROC)(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY* PFNGLSCALEFGLUTPROC)(GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY* PFNGLTRANSLATEFGLUTPROC)(GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY* PFNGLVERTEX2FGLUTPROC)(GLfloat x, GLfloat y);
typedef void (GLAPIENTRY* PFNGLVERTEX3FGLUTPROC)(GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY* PFNGLVERTEX3FVGLUTPROC)(GLfloat const * v);
typedef HGLRC (GLAPIENTRY* PFNWGLCREATECONTEXTGLUTPROC)(HDC hDc);

extern size_t glut_handle;

func_struct wrap_addr_dll[78] = {
    { 0x0000000140965A00, 0, (size_t)&glAlphaFuncDLL, }, // glAlphaFunc
    { 0x0000000140965A70, 0, (size_t)&glBeginDLL, }, // glBegin
    { 0x0000000140965BF8, 0, (size_t)&glBindTextureDLL, }, // glBindTexture
    { 0x0000000140965C58, 0, (size_t)&glClearDLL, }, // glClear
    { 0x0000000140965C38, 0, (size_t)&glClearColorDLL, }, // glClearColor
    { 0x0000000140965C08, 0, (size_t)&glClearDepthDLL, }, // glClearDepth
    { 0x0000000140965C10, 0, (size_t)&glClearStencilDLL, }, // glClearStencil
    { 0x0000000140965A88, 0, (size_t)&glColor3fDLL, }, // glColor3f
    { 0x0000000140965A90, 0, (size_t)&glColor4fDLL, }, // glColor4f
    { 0x0000000140965B88, 0, (size_t)&glColor4ubDLL, }, // glColor4ub
    { 0x0000000140965B90, 0, (size_t)&glColor4ubvDLL, }, // glColor4ubv
    { 0x0000000140965C30, 0, (size_t)&glColorPointerDLL, }, // glColorPointer
    { 0x0000000140965B40, 0, (size_t)&glCopyTexImage2DDLL, }, // glCopyTexImage2D
    { 0x0000000140965A98, 0, (size_t)&glCopyTexSubImage2DDLL, }, // glCopyTexSubImage2D
    { 0x0000000140965C18, 0, (size_t)&glCullFaceDLL, }, // glCullFace
    { 0x0000000140965C40, 0, (size_t)&glDepthFuncDLL, }, // glDepthFunc
    { 0x0000000140965C48, 0, (size_t)&glDisableDLL, }, // glDisable
    { 0x0000000140965C28, 0, (size_t)&glDisableClientStateDLL, }, // glDisableClientState
    { 0x0000000140965BF0, 0, (size_t)&glDrawArraysDLL, }, // glDrawArrays
    { 0x0000000140965B60, 0, (size_t)&glDrawBufferDLL, }, // glDrawBuffer
    { 0x0000000140965A48, 0, (size_t)&glDrawElementsDLL, }, // glDrawElements
    { 0x0000000140965C50, 0, (size_t)&glEnableDLL, }, // glEnable
    { 0x0000000140965BE8, 0, (size_t)&glEnableClientStateDLL, }, // glEnableClientState
    { 0x0000000140965A78, 0, (size_t)&glEndDLL, }, // glEnd
    { 0x0000000140965A58, 0, (size_t)&glFogfDLL, }, // glFogf
    { 0x0000000140965A60, 0, (size_t)&glFogfvDLL, }, // glFogfv
    { 0x0000000140965A68, 0, (size_t)&glFogiDLL, }, // glFogi
    { 0x0000000140965BB0, 0, (size_t)&glFrontFaceDLL, }, // glFrontFace
    { 0x0000000140965C98, 0, (size_t)&glFrustumDLL, }, // glFrustum
    { 0x0000000140965AF0, 0, (size_t)&glGetBooleanvDLL, }, // glGetBooleanv
    { 0x0000000140965BA8, 0, (size_t)&glGetErrorDLL, }, // glGetError
    { 0x0000000140965C90, 0, (size_t)&glGetFloatvDLL, }, // glGetFloatv
    { 0x0000000140965AF8, 0, (size_t)&glGetIntegervDLL, }, // glGetIntegerv
    { 0x0000000140965CB0, 0, (size_t)&glGetMaterialfvDLL, }, // glGetMaterialfv
    { 0x0000000140965C00, 0, (size_t)&glGetTexImageDLL, }, // glGetTexImage
    { 0x0000000140965AA0, 0, (size_t)&glGetTexLevelParameterivDLL, }, // glGetTexLevelParameteriv
    { 0x0000000140965AA8, 0, (size_t)&glGetTexParameterivDLL, }, // glGetTexParameteriv
    { 0x0000000140965B08, 0, (size_t)&glIsEnabledDLL, }, // glIsEnabled
    { 0x0000000140965AC8, 0, (size_t)&glLightfvDLL, }, // glLightfv
    { 0x0000000140965AC0, 0, (size_t)&glLightModelfvDLL, }, // glLightModelfv
    { 0x0000000140965B10, 0, (size_t)&glLineWidthDLL, }, // glLineWidth
    { 0x0000000140965C88, 0, (size_t)&glLoadIdentityDLL, }, // glLoadIdentity
    { 0x0000000140965A50, 0, (size_t)&glLoadMatrixfDLL, }, // glLoadMatrixf
    { 0x0000000140965A08, 0, (size_t)&glMaterialfDLL, }, // glMaterialf
    { 0x0000000140965A80, 0, (size_t)&glMaterialfvDLL, }, // glMaterialfv
    { 0x0000000140965C80, 0, (size_t)&glMatrixModeDLL, }, // glMatrixMode
    { 0x0000000140965B48, 0, (size_t)&glMultMatrixdDLL, }, // glMultMatrixd
    { 0x0000000140965B18, 0, (size_t)&glNormal3fDLL, }, // glNormal3f
    { 0x0000000140965B20, 0, (size_t)&glNormalPointerDLL, }, // glNormalPointer
    { 0x0000000140965B50, 0, (size_t)&glOrthoDLL, }, // glOrtho
    { 0x0000000140965BA0, 0, (size_t)&glPixelStoreiDLL, }, // glPixelStorei
    { 0x0000000140965BE0, 0, (size_t)&glPointSizeDLL, }, // glPointSize
    { 0x0000000140965B98, 0, (size_t)&glPolygonModeDLL, }, // glPolygonMode
    { 0x0000000140965BD8, 0, (size_t)&glPopAttribDLL, }, // glPopAttrib
    { 0x0000000140965BD0, 0, (size_t)&glPopClientAttribDLL, }, // glPopClientAttrib
    { 0x0000000140965CA8, 0, (size_t)&glPopMatrixDLL, }, // glPopMatrix
    { 0x0000000140965BC8, 0, (size_t)&glPushAttribDLL, }, // glPushAttrib
    { 0x0000000140965BC0, 0, (size_t)&glPushClientAttribDLL, }, // glPushClientAttrib
    { 0x0000000140965CA0, 0, (size_t)&glPushMatrixDLL, }, // glPushMatrix
    { 0x0000000140965B70, 0, (size_t)&glReadBufferDLL, }, // glReadBuffer
    { 0x0000000140965B78, 0, (size_t)&glReadPixelsDLL, }, // glReadPixels
    { 0x0000000140965C70, 0, (size_t)&glRotatefDLL, }, // glRotatef
    { 0x0000000140965C68, 0, (size_t)&glScalefDLL, }, // glScalef
    { 0x0000000140965C20, 0, (size_t)&glShadeModelDLL, }, // glShadeModel
    { 0x0000000140965B80, 0, (size_t)&glTexCoord2fDLL, }, // glTexCoord2f
    { 0x0000000140965A30, 0, (size_t)&glTexCoordPointerDLL, }, // glTexCoordPointer
    { 0x0000000140965B28, 0, (size_t)&glTexEnvfDLL, }, // glTexEnvf
    { 0x0000000140965BB8, 0, (size_t)&glTexEnviDLL, }, // glTexEnvi
    { 0x0000000140965B58, 0, (size_t)&glTexImage1DDLL, }, // glTexImage1D
    { 0x0000000140965AE0, 0, (size_t)&glTexImage2DDLL, }, // glTexImage2D
    { 0x0000000140965AE8, 0, (size_t)&glTexParameterfDLL, }, // glTexParameterf
    { 0x0000000140965A10, 0, (size_t)&glTexParameterfvDLL, }, // glTexParameterfv
    { 0x0000000140965A18, 0, (size_t)&glTexParameteriDLL, }, // glTexParameteri
    { 0x0000000140965AB0, 0, (size_t)&glTexSubImage2DDLL, }, // glTexSubImage2D
    { 0x0000000140965C60, 0, (size_t)&glTranslatefDLL, }, // glTranslatef
    { 0x0000000140965AB8, 0, (size_t)&glVertex3fDLL, }, // glVertex3f
    { 0x0000000140965B30, 0, (size_t)&glVertex3fvDLL, }, // glVertex3fv
    { 0x0000000140965B00, 0, (size_t)&glVertexPointerDLL, }, // glVertexPointer
};

func_struct wrap_addr[50] = {
    { 0x00000001411A3DB8, 0, (size_t)&glActiveTexture, }, // glActiveTexture
    { 0x00000001411A4B10, 0, (size_t)&glBindBufferARB, }, // glBindBufferARB
    { 0x00000001411A3578, 0, (size_t)&glBindBufferBaseNV, }, // glBindBufferBaseNV
    { 0x00000001411A54D8, 0, (size_t)&glBindFramebufferEXT, }, // glBindFramebufferEXT
    { 0x00000001411A3818, 0, (size_t)&glBindMultiTextureEXT, }, // glBindMultiTextureEXT
    { 0x00000001411A54A8, 0, (size_t)&glBindRenderbufferEXT, }, // glBindRenderbufferEXT
    { 0x00000001411A4B30, 0, (size_t)&glBufferDataARB, }, // glBufferDataARB
    { 0x00000001411A4D00, 0, (size_t)&glClampColorARB, }, // glClampColorARB
    { 0x00000001411A3DC8, 0, (size_t)&glClientActiveTexture, }, // glClientActiveTexture
    { 0x00000001411A37D0, 0, (size_t)&glCopyMultiTexSubImage2DEXT, }, // glCopyMultiTexSubImage2DEXT
    { 0x00000001411A4B18, 0, (size_t)&glDeleteBuffersARB, }, // glDeleteBuffersARB
    { 0x00000001411A54E0, 0, (size_t)&glDeleteFramebuffersEXT, }, // glDeleteFramebuffersEXT
    { 0x00000001411A54B0, 0, (size_t)&glDeleteRenderbuffersEXT, }, // glDeleteRenderbuffersEXT
    { 0x00000001411A1A30, 0, (size_t)&glDeleteTextures, }, // glDeleteTextures
    { 0x00000001411A4A50, 0, (size_t)&glDisableVertexAttribArrayARB, }, // glDisableVertexAttribArrayARB
    { 0x00000001411A3B78, 0, (size_t)&glDrawRangeElements, }, // glDrawRangeElements
    { 0x00000001411A42A8, 0, (size_t)&glEnableVertexAttribArray, }, // glEnableVertexAttribArray
    { 0x00000001411A4A48, 0, (size_t)&glEnableVertexAttribArrayARB, }, // glEnableVertexAttribArrayARB
    { 0x00000001411A5510, 0, (size_t)&glFramebufferRenderbufferEXT, }, // glFramebufferRenderbufferEXT
    { 0x00000001411A5500, 0, (size_t)&glFramebufferTexture2DEXT, }, // glFramebufferTexture2DEXT
    { 0x00000001411A54E8, 0, (size_t)&glGenFramebuffersEXT, }, // glGenFramebuffersEXT
    { 0x00000001411A54B8, 0, (size_t)&glGenRenderbuffersEXT, }, // glGenRenderbuffersEXT
    { 0x00000001411A1A38, 0, (size_t)&glGenTextures, }, // glGenTextures
    { 0x00000001411A4AC0, 0, (size_t)&glGetProgramEnvParameterfvARB, }, // glGetProgramEnvParameterfvARB
    { 0x00000001411A4AD8, 0, (size_t)&glGetProgramivARB, }, // glGetProgramivARB
    { 0x00000001411A3738, 0, (size_t)&glGetTextureImageEXT, }, // glGetTextureImageEXT
    { 0x00000001411A4B28, 0, (size_t)&glIsBufferARB, }, // glIsBufferARB
    { 0x00000001411A3FD8, 0, (size_t)&glLoadTransposeMatrixf, }, // glLoadTransposeMatrixf
    { 0x00000001411A3B20, 0, (size_t)&glMapNamedBufferEXT, }, // glMapNamedBufferEXT
    { 0x00000001411A3E78, 0, (size_t)&glMultiTexCoord2f, }, // glMultiTexCoord2f
    { 0x00000001411A3E88, 0, (size_t)&glMultiTexCoord2fv, }, // glMultiTexCoord2fv
    { 0x00000001411A3FE8, 0, (size_t)&glMultTransposeMatrixf, }, // glMultTransposeMatrixf
    { 0x00000001411A3B10, 0, (size_t)&glNamedBufferDataEXT, }, // glNamedBufferDataEXT
    { 0x00000001411A3B18, 0, (size_t)&glNamedBufferSubDataEXT, }, // glNamedBufferSubDataEXT
    { 0x00000001411A4078, 0, (size_t)&glPointParameterf, }, // glPointParameterf
    { 0x00000001411A4080, 0, (size_t)&glPointParameterfv, }, // glPointParameterfv
    { 0x00000001411A4088, 0, (size_t)&glPointParameteri, }, // glPointParameteri
    { 0x00000001411A5488, 0, (size_t)&glPrimitiveRestartIndexNV, }, // glPrimitiveRestartIndexNV
    { 0x00000001411A3508, 0, (size_t)&glProgramBufferParametersfvNV, }, // glProgramBufferParametersfvNV
    { 0x00000001411A4A88, 0, (size_t)&glProgramEnvParameter4fARB, }, // glProgramEnvParameter4fARB
    { 0x00000001411A4A90, 0, (size_t)&glProgramEnvParameter4fvARB, }, // glProgramEnvParameter4fvARB
    { 0x00000001411A5548, 0, (size_t)&glProgramEnvParameters4fvEXT, }, // glProgramEnvParameters4fvEXT
    { 0x00000001411A4AA8, 0, (size_t)&glProgramLocalParameter4fARB, }, // glProgramLocalParameter4fARB
    { 0x00000001411A4AB0, 0, (size_t)&glProgramLocalParameter4fvARB, }, // glProgramLocalParameter4fvARB
    { 0x00000001411A5550, 0, (size_t)&glProgramLocalParameters4fvEXT, }, // glProgramLocalParameters4fvEXT
    { 0x00000001411A54C0, 0, (size_t)&glRenderbufferStorageEXT, }, // glRenderbufferStorageEXT
    { 0x00000001411A3500, 0, (size_t)&glRenderbufferStorageMultisampleCoverageNV, }, // glRenderbufferStorageMultisampleCoverageNV
    { 0x00000001411A3B28, 0, (size_t)&glUnmapNamedBufferEXT, }, // glUnmapNamedBufferEXT
    { 0x00000001411A4350, 0, (size_t)&glUseProgram, }, // glUseProgram
    { 0x00000001411A4A40, 0, (size_t)&glVertexAttribPointerARB, }, // glVertexAttribPointerARB
};

func_struct wrap_addr_glut[27] = {
    { 0x000000000002A380, 0, (size_t)&glBeginGLUT, }, // glBegin
    { 0x000000000002A348, 0, (size_t)&glBitmapGLUT, }, // glBitmap
    { 0x000000000002A370, 0, (size_t)&glDrawBufferGLUT, }, // glDrawBuffer
    { 0x000000000002A3D8, 0, (size_t)&glEnableGLUT, }, // glEnable
    { 0x000000000002A388, 0, (size_t)&glEndGLUT, }, // glEnd
    { 0x000000000002A3E0, 0, (size_t)&glEvalMesh2GLUT, }, // glEvalMesh2
    { 0x000000000002A418, 0, (size_t)&glGetErrorGLUT, }, // glGetError
    { 0x000000000002A450, 0, (size_t)&glGetIntegervGLUT, }, // glGetIntegerv
    { 0x000000000002A360, 0, (size_t)&glGetStringGLUT, }, // glGetString
    { 0x000000000002A3E8, 0, (size_t)&glMap2fGLUT, }, // glMap2f
    { 0x000000000002A3F0, 0, (size_t)&glMapGrid2fGLUT, }, // glMapGrid2f
    { 0x000000000002A390, 0, (size_t)&glNormal3fGLUT, }, // glNormal3f
    { 0x000000000002A398, 0, (size_t)&glNormal3fvGLUT, }, // glNormal3fv
    { 0x000000000002A350, 0, (size_t)&glPixelStoreiGLUT, }, // glPixelStorei
    { 0x000000000002A3A0, 0, (size_t)&glPolygonModeGLUT, }, // glPolygonMode
    { 0x000000000002A3A8, 0, (size_t)&glPopAttribGLUT, }, // glPopAttrib
    { 0x000000000002A3F8, 0, (size_t)&glPopMatrixGLUT, }, // glPopMatrix
    { 0x000000000002A3B0, 0, (size_t)&glPushAttribGLUT, }, // glPushAttrib
    { 0x000000000002A400, 0, (size_t)&glPushMatrixGLUT, }, // glPushMatrix
    { 0x000000000002A378, 0, (size_t)&glReadBufferGLUT, }, // glReadBuffer
    { 0x000000000002A408, 0, (size_t)&glRotatefGLUT, }, // glRotatef
    { 0x000000000002A410, 0, (size_t)&glScalefGLUT, }, // glScalef
    { 0x000000000002A3C8, 0, (size_t)&glTranslatefGLUT, }, // glTranslatef
    { 0x000000000002A3D0, 0, (size_t)&glVertex2fGLUT, }, // glVertex2f
    { 0x000000000002A3B8, 0, (size_t)&glVertex3fGLUT, }, // glVertex3f
    { 0x000000000002A3C0, 0, (size_t)&glVertex3fvGLUT, }, // glVertex3fv
    { 0x000000000002A440, 0, (size_t)&wglCreateContextGLUT, }, // wglCreateContext
};

gl_func_struct gl_addr_dll[9] = {
    { 0x0000000140965A20, 0, }, // glBlendFunc
    { 0x0000000140965A38, 0, }, // glColorMask
    { 0x0000000140965AD0, 0, }, // glDeleteTextures
    { 0x0000000140965A28, 0, }, // glDepthMask
    { 0x0000000140965B68, 0, }, // glFinish
    { 0x0000000140965AD8, 0, }, // glGenTextures
    { 0x0000000140965A40, 0, }, // glScissor
    { 0x0000000140965C78, 0, }, // glViewport
    { 0x0000000140965B38, 0, }, // wglGetProcAddress
};

gl_func_struct gl_addr[1057] = {
    { 0x00000001411A52D0, 0, }, // glActiveStencilFaceEXT
    { 0x00000001411A4720, 0, }, // glActiveTextureARB
    { 0x00000001411A3588, 0, }, // glActiveVaryingNV
    { 0x00000001411A5088, 0, }, // glAreProgramsResidentNV
    { 0x00000001411A4E30, 0, }, // glAreTexturesResidentEXT
    { 0x00000001411A4E60, 0, }, // glArrayElementEXT
    { 0x00000001411A4BE0, 0, }, // glAttachObjectARB
    { 0x00000001411A4260, 0, }, // glAttachShader
    { 0x00000001411A45B0, 0, }, // glBeginConditionalRender
    { 0x00000001411A35F0, 0, }, // glBeginConditionalRenderNV
    { 0x00000001411A52A0, 0, }, // glBeginOcclusionQueryNV
    { 0x00000001411A41B8, 0, }, // glBeginQuery
    { 0x00000001411A4B80, 0, }, // glBeginQueryARB
    { 0x00000001411A4580, 0, }, // glBeginTransformFeedback
    { 0x00000001411A3550, 0, }, // glBeginTransformFeedbackNV
    { 0x00000001411A4268, 0, }, // glBindAttribLocation
    { 0x00000001411A4CE0, 0, }, // glBindAttribLocationARB
    { 0x00000001411A41E0, 0, }, // glBindBuffer
    { 0x0000001411A4598, 0, }, // glBindBufferBase
    { 0x00000001411A3570, 0, }, // glBindBufferOffsetNV
    { 0x00000001411A4590, 0, }, // glBindBufferRange
    { 0x00000001411A3568, 0, }, // glBindBufferRangeNV
    { 0x00000001411A4680, 0, }, // glBindFragDataLocation
    { 0x00000001411A56C0, 0, }, // glBindFragDataLocationEXT
    { 0x00000001411A4D50, 0, }, // glBindFramebuffer
    { 0x00000001411A4A60, 0, }, // glBindProgramARB
    { 0x00000001411A5090, 0, }, // glBindProgramNV
    { 0x00000001411A4D20, 0, }, // glBindRenderbuffer
    { 0x00000001411A3FA0, 0, }, // glBindSampler
    { 0x00000001411A4E38, 0, }, // glBindTextureEXT
    { 0x00000001411A4DF0, 0, }, // glBindVertexArray
    { 0x00000001411A3B58, 0, }, // glBlendColor
    { 0x00000001411A4E10, 0, }, // glBlendColorEXT
    { 0x00000001411A3B68, 0, }, // glBlendEquation
    { 0x00000001411A4EA8, 0, }, // glBlendEquationEXT
    { 0x00000001411A4238, 0, }, // glBlendEquationSeparate
    { 0x00000001411A5498, 0, }, // glBlendEquationSeparateEXT
    { 0x00000001411A4038, 0, }, // glBlendFuncSeparate
    { 0x00000001411A4FC0, 0, }, // glBlendFuncSeparateEXT
    { 0x00000001411A4DA0, 0, }, // glBlitFramebuffer
    { 0x00000001411A5528, 0, }, // glBlitFramebufferEXT
    { 0x00000001411A4200, 0, }, // glBufferData
    { 0x00000001411A4208, 0, }, // glBufferSubData
    { 0x00000001411A4B38, 0, }, // glBufferSubDataARB
    { 0x00000001411A4D68, 0, }, // glCheckFramebufferStatus
    { 0x00000001411A54F0, 0, }, // glCheckFramebufferStatusEXT
    { 0x00000001411A3B80, 0, }, // glCheckNamedFramebufferStatusEXT
    { 0x00000001411A4708, 0, }, // glClearBufferfi
    { 0x00000001411A4700, 0, }, // glClearBufferfv
    { 0x00000001411A46F0, 0, }, // glClearBufferiv
    { 0x00000001411A46F8, 0, }, // glClearBufferuiv
    { 0x00000001411A35E0, 0, }, // glClearColorIiEXT
    { 0x00000001411A35E8, 0, }, // glClearColorIuiEXT
    { 0x00000001411A34F0, 0, }, // glClearDepthdNV
    { 0x00000001411A4728, 0, }, // glClientActiveTextureARB
    { 0x00000001411A3630, 0, }, // glClientAttribDefaultEXT
    { 0x00000001411A5350, 0, }, // glColor3hNV
    { 0x00000001411A5358, 0, }, // glColor3hvNV
    { 0x00000001411A5360, 0, }, // glColor4hNV
    { 0x00000001411A5368, 0, }, // glColor4hvNV
    { 0x00000001411A4550, 0, }, // glColorMaski
    { 0x00000001411A3520, 0, }, // glColorMaskIndexedEXT
    { 0x00000001411A4E68, 0, }, // glColorPointerEXT
    { 0x00000001411A3BF8, 0, }, // glColorSubTable
    { 0x00000001411A4EC0, 0, }, // glColorSubTableEXT
    { 0x00000001411A3B88, 0, }, // glColorTable
    { 0x00000001411A4EC8, 0, }, // glColorTableEXT
    { 0x00000001411A3B98, 0, }, // glColorTableParameterfv
    { 0x00000001411A3BA8, 0, }, // glColorTableParameteriv
    { 0x00000001411A4FF8, 0, }, // glCombinerInputNV
    { 0x00000001411A5000, 0, }, // glCombinerOutputNV
    { 0x00000001411A4FE0, 0, }, // glCombinerParameterfNV
    { 0x00000001411A4FD8, 0, }, // glCombinerParameterfvNV
    { 0x00000001411A4FF0, 0, }, // glCombinerParameteriNV
    { 0x00000001411A4FE8, 0, }, // glCombinerParameterivNV
    { 0x00000001411A5078, 0, }, // glCombinerStageParameterfvNV
    { 0x00000001411A4270, 0, }, // glCompileShader
    { 0x00000001411A4BD0, 0, }, // glCompileShaderARB
    { 0x00000001411A3910, 0, }, // glCompressedMultiTexImage1DEXT
    { 0x00000001411A3908, 0, }, // glCompressedMultiTexImage2DEXT
    { 0x00000001411A3900, 0, }, // glCompressedMultiTexImage3DEXT
    { 0x00000001411A3928, 0, }, // glCompressedMultiTexSubImage1DEXT
    { 0x00000001411A3920, 0, }, // glCompressedMultiTexSubImage2DEXT
    { 0x00000001411A3918, 0, }, // glCompressedMultiTexSubImage3DEXT
    { 0x00000001411A4010, 0, }, // glCompressedTexImage1D
    { 0x00000001411A4868, 0, }, // glCompressedTexImage1DARB
    { 0x00000001411A4008, 0, }, // glCompressedTexImage2D
    { 0x00000001411A4860, 0, }, // glCompressedTexImage2DARB
    { 0x00000001411A4000, 0, }, // glCompressedTexImage3D
    { 0x00000001411A4858, 0, }, // glCompressedTexImage3DARB
    { 0x00000001411A4028, 0, }, // glCompressedTexSubImage1D
    { 0x00000001411A4880, 0, }, // glCompressedTexSubImage1DARB
    { 0x00000001411A4020, 0, }, // glCompressedTexSubImage2D
    { 0x00000001411A4878, 0, }, // glCompressedTexSubImage2DARB
    { 0x00000001411A4018, 0, }, // glCompressedTexSubImage3D
    { 0x00000001411A4870, 0, }, // glCompressedTexSubImage3DARB
    { 0x00000001411A38D8, 0, }, // glCompressedTextureImage1DEXT
    { 0x00000001411A38D0, 0, }, // glCompressedTextureImage2DEXT
    { 0x00000001411A38C8, 0, }, // glCompressedTextureImage3DEXT
    { 0x00000001411A38F0, 0, }, // glCompressedTextureSubImage1DEXT
    { 0x00000001411A38E8, 0, }, // glCompressedTextureSubImage2DEXT
    { 0x00000001411A38E0, 0, }, // glCompressedTextureSubImage3DEXT
    { 0x00000001411A3C18, 0, }, // glConvolutionFilter1D
    { 0x00000001411A3C28, 0, }, // glConvolutionFilter2D
    { 0x00000001411A3C38, 0, }, // glConvolutionParameterf
    { 0x00000001411A3C48, 0, }, // glConvolutionParameterfv
    { 0x00000001411A3C58, 0, }, // glConvolutionParameteri
    { 0x00000001411A3C68, 0, }, // glConvolutionParameteriv
    { 0x00000001411A3C08, 0, }, // glCopyColorSubTable
    { 0x00000001411A3BB8, 0, }, // glCopyColorTable
    { 0x00000001411A3C78, 0, }, // glCopyConvolutionFilter1D
    { 0x00000001411A3C88, 0, }, // glCopyConvolutionFilter2D
    { 0x00000001411A4718, 0, }, // glCopyImageSubData
    { 0x00000001411A37B8, 0, }, // glCopyMultiTexImage1DEXT
    { 0x00000001411A37C0, 0, }, // glCopyMultiTexImage2DEXT
    { 0x00000001411A37C8, 0, }, // glCopyMultiTexSubImage1DEXT
    { 0x00000001411A3810, 0, }, // glCopyMultiTexSubImage3DEXT
    { 0x00000001411A3DA8, 0, }, // glCopyTexSubImage3D
    { 0x00000001411A4E28, 0, }, // glCopyTexSubImage3DEXT
    { 0x00000001411A3718, 0, }, // glCopyTextureImage1DEXT
    { 0x00000001411A3720, 0, }, // glCopyTextureImage2DEXT
    { 0x00000001411A3728, 0, }, // glCopyTextureSubImage1DEXT
    { 0x00000001411A3730, 0, }, // glCopyTextureSubImage2DEXT
    { 0x00000001411A3770, 0, }, // glCopyTextureSubImage3DEXT
    { 0x00000001411A4278, 0, }, // glCreateProgram
    { 0x00000001411A4BD8, 0, }, // glCreateProgramObjectARB
    { 0x00000001411A4280, 0, }, // glCreateShader
    { 0x00000001411A4BC0, 0, }, // glCreateShaderObjectARB
    { 0x00000001411A41E8, 0, }, // glDeleteBuffers
    { 0x00000001411A5040, 0, }, // glDeleteFencesNV
    { 0x00000001411A4D58, 0, }, // glDeleteFramebuffers
    { 0x00000001411A4BA8, 0, }, // glDeleteObjectARB
    { 0x00000001411A5290, 0, }, // glDeleteOcclusionQueriesNV
    { 0x00000001411A4288, 0, }, // glDeleteProgram
    { 0x00000001411A4A68, 0, }, // glDeleteProgramsARB
    { 0x00000001411A5098, 0, }, // glDeleteProgramsNV
    { 0x00000001411A41A8, 0, }, // glDeleteQueries
    { 0x00000001411A4B70, 0, }, // glDeleteQueriesARB
    { 0x00000001411A4D28, 0, }, // glDeleteRenderbuffers
    { 0x00000001411A3F90, 0, }, // glDeleteSamplers
    { 0x00000001411A4290, 0, }, // glDeleteShader
    { 0x00000001411A4E40, 0, }, // glDeleteTexturesEXT
    { 0x00000001411A4DF8, 0, }, // glDeleteVertexArrays
    { 0x00000001411A34F8, 0, }, // glDepthBoundsdNV
    { 0x00000001411A5490, 0, }, // glDepthBoundsEXT
    { 0x00000001411A34E8, 0, }, // glDepthRangedNV
    { 0x00000001411A4BB8, 0, }, // glDetachObjectARB
    { 0x00000001411A4298, 0, }, // glDetachShader
    { 0x00000001411A3828, 0, }, // glDisableClientStateIndexedEXT
    { 0x00000001411A4570, 0, }, // glDisablei
    { 0x00000001411A3540, 0, }, // glDisableIndexedEXT
    { 0x00000001411A42A0, 0, }, // glDisableVertexAttribArray
    { 0x00000001411A4E70, 0, }, // glDrawArraysEXT
    { 0x00000001411A4D08, 0, }, // glDrawArraysInstancedARB
    { 0x00000001411A5710, 0, }, // glDrawArraysInstancedEXT
    { 0x00000001411A4240, 0, }, // glDrawBuffers
    { 0x00000001411A4CF8, 0, }, // glDrawBuffersARB
    { 0x00000001411A52D8, 0, }, // glDrawBuffersATI
    { 0x00000001411A4D10, 0, }, // glDrawElementsInstancedARB
    { 0x00000001411A5718, 0, }, // glDrawElementsInstancedEXT
    { 0x00000001411A4EF8, 0, }, // glDrawRangeElementsEXT
    { 0x00000001411A4E78, 0, }, // glEdgeFlagPointerEXT
    { 0x00000001411A3820, 0, }, // glEnableClientStateIndexedEXT
    { 0x00000001411A4568, 0, }, // glEnablei
    { 0x00000001411A3538, 0, }, // glEnableIndexedEXT
    { 0x00000001411A45B8, 0, }, // glEndConditionalRender
    { 0x00000001411A35F8, 0, }, // glEndConditionalRenderNV
    { 0x00000001411A52A8, 0, }, // glEndOcclusionQueryNV
    { 0x00000001411A41C0, 0, }, // glEndQuery
    { 0x00000001411A4B88, 0, }, // glEndQueryARB
    { 0x00000001411A4588, 0, }, // glEndTransformFeedback
    { 0x00000001411A3558, 0, }, // glEndTransformFeedbackNV
    { 0x00000001411A50A0, 0, }, // glExecuteProgramNV
    { 0x00000001411A5008, 0, }, // glFinalCombinerInputNV
    { 0x00000001411A5068, 0, }, // glFinishFenceNV
    { 0x00000001411A4DE0, 0, }, // glFlushMappedBufferRange
    { 0x00000001411A5478, 0, }, // glFlushPixelDataRangeNV
    { 0x00000001411A4FC8, 0, }, // glFlushVertexArrayRangeNV
    { 0x00000001411A4050, 0, }, // glFogCoordd
    { 0x00000001411A4FA8, 0, }, // glFogCoorddEXT
    { 0x00000001411A4058, 0, }, // glFogCoorddv
    { 0x00000001411A4FB0, 0, }, // glFogCoorddvEXT
    { 0x00000001411A4040, 0, }, // glFogCoordf
    { 0x00000001411A4F98, 0, }, // glFogCoordfEXT
    { 0x00000001411A4048, 0, }, // glFogCoordfv
    { 0x00000001411A4FA0, 0, }, // glFogCoordfvEXT
    { 0x00000001411A53F0, 0, }, // glFogCoordhNV
    { 0x00000001411A53F8, 0, }, // glFogCoordhvNV
    { 0x00000001411A4060, 0, }, // glFogCoordPointer
    { 0x00000001411A4FB8, 0, }, // glFogCoordPointerEXT
    { 0x00000001411A3C00, 0, }, // glFramebufferDrawBufferEXT
    { 0x00000001411A3C10, 0, }, // glFramebufferDrawBuffersEXT
    { 0x00000001411A3C20, 0, }, // glFramebufferReadBufferEXT
    { 0x00000001411A4D88, 0, }, // glFramebufferRenderbuffer
    { 0x00000001411A3F50, 0, }, // glFramebufferTexture
    { 0x00000001411A4D70, 0, }, // glFramebufferTexture1D
    { 0x00000001411A54F8, 0, }, // glFramebufferTexture1DEXT
    { 0x00000001411A4D78, 0, }, // glFramebufferTexture2D
    { 0x00000001411A4D80, 0, }, // glFramebufferTexture3D
    { 0x00000001411A5508, 0, }, // glFramebufferTexture3DEXT
    { 0x00000001411A4DC0, 0, }, // glFramebufferTextureARB
    { 0x00000001411A55E0, 0, }, // glFramebufferTextureEXT
    { 0x00000001411A4DD0, 0, }, // glFramebufferTextureFaceARB
    { 0x00000001411A55F0, 0, }, // glFramebufferTextureFaceEXT
    { 0x00000001411A4DB0, 0, }, // glFramebufferTextureLayer
    { 0x00000001411A4DC8, 0, }, // glFramebufferTextureLayerARB
    { 0x00000001411A55E8, 0, }, // glFramebufferTextureLayerEXT
    { 0x00000001411A41F0, 0, }, // glGenBuffers
    { 0x00000001411A4B20, 0, }, // glGenBuffersARB
    { 0x00000001411A4D98, 0, }, // glGenerateMipmap
    { 0x00000001411A5520, 0, }, // glGenerateMipmapEXT
    { 0x00000001411A3BF0, 0, }, // glGenerateMultiTexMipmapEXT
    { 0x00000001411A3BE0, 0, }, // glGenerateTextureMipmapEXT
    { 0x00000001411A5048, 0, }, // glGenFencesNV
    { 0x00000001411A4D60, 0, }, // glGenFramebuffers
    { 0x00000001411A5288, 0, }, // glGenOcclusionQueriesNV
    { 0x00000001411A4A70, 0, }, // glGenProgramsARB
    { 0x00000001411A50A8, 0, }, // glGenProgramsNV
    { 0x00000001411A41A0, 0, }, // glGenQueries
    { 0x00000001411A4B68, 0, }, // glGenQueriesARB
    { 0x00000001411A4D30, 0, }, // glGenRenderbuffers
    { 0x00000001411A3F80, 0, }, // glGenSamplers
    { 0x00000001411A4E48, 0, }, // glGenTexturesEXT
    { 0x00000001411A4E00, 0, }, // glGenVertexArrays
    { 0x00000001411A42B0, 0, }, // glGetActiveAttrib
    { 0x00000001411A4CE8, 0, }, // glGetActiveAttribARB
    { 0x00000001411A42B8, 0, }, // glGetActiveUniform
    { 0x00000001411A4CC0, 0, }, // glGetActiveUniformARB
    { 0x00000001411A3598, 0, }, // glGetActiveVaryingNV
    { 0x00000001411A4CB0, 0, }, // glGetAttachedObjectsARB
    { 0x00000001411A42C0, 0, }, // glGetAttachedShaders
    { 0x00000001411A42C8, 0, }, // glGetAttribLocation
    { 0x00000001411A4CF0, 0, }, // glGetAttribLocationARB
    { 0x00000001411A4558, 0, }, // glGetBooleani_v
    { 0x00000001411A3528, 0, }, // glGetBooleanIndexedvEXT
    { 0x00000001411A4228, 0, }, // glGetBufferParameteriv
    { 0x00000001411A4B58, 0, }, // glGetBufferParameterivARB
    { 0x00000001411A4230, 0, }, // glGetBufferPointerv
    { 0x00000001411A4B60, 0, }, // glGetBufferPointervARB
    { 0x00000001411A4210, 0, }, // glGetBufferSubData
    { 0x00000001411A4B40, 0, }, // glGetBufferSubDataARB
    { 0x00000001411A3BC8, 0, }, // glGetColorTable
    { 0x00000001411A4ED0, 0, }, // glGetColorTableEXT
    { 0x00000001411A3BD8, 0, }, // glGetColorTableParameterfv
    { 0x00000001411A4EE0, 0, }, // glGetColorTableParameterfvEXT
    { 0x00000001411A3BE8, 0, }, // glGetColorTableParameteriv
    { 0x00000001411A4ED8, 0, }, // glGetColorTableParameterivEXT
    { 0x00000001411A5010, 0, }, // glGetCombinerInputParameterfvNV
    { 0x00000001411A5018, 0, }, // glGetCombinerInputParameterivNV
    { 0x00000001411A5020, 0, }, // glGetCombinerOutputParameterfvNV
    { 0x00000001411A5028, 0, }, // glGetCombinerOutputParameterivNV
    { 0x00000001411A5080, 0, }, // glGetCombinerStageParameterfvNV
    { 0x00000001411A3930, 0, }, // glGetCompressedMultiTexImageEXT
    { 0x00000001411A4030, 0, }, // glGetCompressedTexImage
    { 0x00000001411A4888, 0, }, // glGetCompressedTexImageARB
    { 0x00000001411A38F8, 0, }, // glGetCompressedTextureImageEXT
    { 0x00000001411A3C98, 0, }, // glGetConvolutionFilter
    { 0x00000001411A3CA8, 0, }, // glGetConvolutionParameterfv
    { 0x00000001411A3CB8, 0, }, // glGetConvolutionParameteriv
    { 0x00000001411A38B8, 0, }, // glGetDoubleIndexedvEXT
    { 0x00000001411A5060, 0, }, // glGetFenceivNV
    { 0x00000001411A5030, 0, }, // glGetFinalCombinerInputParameterfvNV
    { 0x00000001411A5038, 0, }, // glGetFinalCombinerInputParameterivNV
    { 0x00000001411A38B0, 0, }, // glGetFloatIndexedvEXT
    { 0x00000001411A4688, 0, }, // glGetFragDataLocation
    { 0x00000001411A56C8, 0, }, // glGetFragDataLocationEXT
    { 0x00000001411A4D90, 0, }, // glGetFramebufferAttachmentParameteriv
    { 0x00000001411A5518, 0, }, // glGetFramebufferAttachmentParameterivEXT
    { 0x00000001411A3C30, 0, }, // glGetFramebufferParameterivEXT
    { 0x00000001411A4BB0, 0, }, // glGetHandleARB
    { 0x00000001411A3CE8, 0, }, // glGetHistogram
    { 0x00000001411A3CF8, 0, }, // glGetHistogramParameterfv
    { 0x00000001411A3D08, 0, }, // glGetHistogramParameteriv
    { 0x00000001411A4CA8, 0, }, // glGetInfoLogARB
    { 0x00000001411A4560, 0, }, // glGetIntegeri_v
    { 0x00000001411A3530, 0, }, // glGetIntegerIndexedvEXT
    { 0x00000001411A3D18, 0, }, // glGetMinmax
    { 0x00000001411A3D28, 0, }, // glGetMinmaxParameterfv
    { 0x00000001411A3D38, 0, }, // glGetMinmaxParameteriv
    { 0x00000001411A3CB0, 0, }, // glGetMultisamplefvNV
    { 0x00000001411A3888, 0, }, // glGetMultiTexEnvfvEXT
    { 0x00000001411A3890, 0, }, // glGetMultiTexEnvivEXT
    { 0x00000001411A3898, 0, }, // glGetMultiTexGendvEXT
    { 0x00000001411A38A0, 0, }, // glGetMultiTexGenfvEXT
    { 0x00000001411A38A8, 0, }, // glGetMultiTexGenivEXT
    { 0x00000001411A37D8, 0, }, // glGetMultiTexImageEXT
    { 0x00000001411A37F0, 0, }, // glGetMultiTexLevelParameterfvEXT
    { 0x00000001411A37F8, 0, }, // glGetMultiTexLevelParameterivEXT
    { 0x00000001411A37E0, 0, }, // glGetMultiTexParameterfvEXT
    { 0x00000001411A39F8, 0, }, // glGetMultiTexParameterIivEXT
    { 0x00000001411A3A00, 0, }, // glGetMultiTexParameterIuivEXT
    { 0x00000001411A37E8, 0, }, // glGetMultiTexParameterivEXT
    { 0x00000001411A3B30, 0, }, // glGetNamedBufferParameterivEXT
    { 0x00000001411A3B38, 0, }, // glGetNamedBufferPointervEXT
    { 0x00000001411A3B40, 0, }, // glGetNamedBufferSubDataEXT
    { 0x00000001411A3BD0, 0, }, // glGetNamedFramebufferAttachmentParameterivEXT
    { 0x00000001411A3970, 0, }, // glGetNamedProgramivEXT
    { 0x00000001411A3960, 0, }, // glGetNamedProgramLocalParameterdvEXT
    { 0x00000001411A3968, 0, }, // glGetNamedProgramLocalParameterfvEXT
    { 0x00000001411A39B8, 0, }, // glGetNamedProgramLocalParameterIivEXT
    { 0x00000001411A39C0, 0, }, // glGetNamedProgramLocalParameterIuivEXT
    { 0x00000001411A3978, 0, }, // glGetNamedProgramStringEXT
    { 0x00000001411A3B70, 0, }, // glGetNamedRenderbufferParameterivEXT
    { 0x00000001411A4C98, 0, }, // glGetObjectParameterfvARB
    { 0x00000001411A4CA0, 0, }, // glGetObjectParameterivARB
    { 0x00000001411A52B0, 0, }, // glGetOcclusionQueryivNV
    { 0x00000001411A52B8, 0, }, // glGetOcclusionQueryuivNV
    { 0x00000001411A38C0, 0, }, // glGetPointerIndexedvEXT
    { 0x00000001411A4E80, 0, }, // glGetPointervEXT
    { 0x00000001411A4AB8, 0, }, // glGetProgramEnvParameterdvARB
    { 0x00000001411A55C8, 0, }, // glGetProgramEnvParameterIivNV
    { 0x00000001411A55D0, 0, }, // glGetProgramEnvParameterIuivNV
    { 0x00000001411A42D8, 0, }, // glGetProgramInfoLog
    { 0x00000001411A42D0, 0, }, // glGetProgramiv
    { 0x00000001411A50C0, 0, }, // glGetProgramivNV
    { 0x00000001411A4AC8, 0, }, // glGetProgramLocalParameterdvARB
    { 0x00000001411A4AD0, 0, }, // glGetProgramLocalParameterfvARB
    { 0x00000001411A55B8, 0, }, // glGetProgramLocalParameterIivNV
    { 0x00000001411A55C0, 0, }, // glGetProgramLocalParameterIuivNV
    { 0x00000001411A5308, 0, }, // glGetProgramNamedParameterdvNV
    { 0x00000001411A5300, 0, }, // glGetProgramNamedParameterfvNV
    { 0x00000001411A50B0, 0, }, // glGetProgramParameterdvNV
    { 0x00000001411A50B8, 0, }, // glGetProgramParameterfvNV
    { 0x00000001411A4AE0, 0, }, // glGetProgramStringARB
    { 0x00000001411A50C8, 0, }, // glGetProgramStringNV
    { 0x00000001411A41C8, 0, }, // glGetQueryiv
    { 0x00000001411A4B90, 0, }, // glGetQueryivARB
    { 0x00000001411A5538, 0, }, // glGetQueryObjecti64vEXT
    { 0x00000001411A41D0, 0, }, // glGetQueryObjectiv
    { 0x00000001411A4B98, 0, }, // glGetQueryObjectivARB
    { 0x00000001411A3F40, 0, }, // glGetQueryObjectui64v
    { 0x00000001411A5540, 0, }, // glGetQueryObjectui64vEXT
    { 0x00000001411A41D8, 0, }, // glGetQueryObjectuiv
    { 0x00000001411A4BA0, 0, }, // glGetQueryObjectuivARB
    { 0x00000001411A4D40, 0, }, // glGetRenderbufferParameteriv
    { 0x00000001411A54C8, 0, }, // glGetRenderbufferParameterivEXT
    { 0x00000001411A3CC8, 0, }, // glGetSeparableFilter
    { 0x00000001411A42E8, 0, }, // glGetShaderInfoLog
    { 0x00000001411A42E0, 0, }, // glGetShaderiv
    { 0x00000001411A42F0, 0, }, // glGetShaderSource
    { 0x00000001411A4CD8, 0, }, // glGetShaderSourceARB
    { 0x00000001411A4710, 0, }, // glGetStringi
    { 0x00000001411A46E0, 0, }, // glGetTexParameterIiv
    { 0x00000001411A35D0, 0, }, // glGetTexParameterIivEXT
    { 0x00000001411A46E8, 0, }, // glGetTexParameterIuiv
    { 0x00000001411A35D8, 0, }, // glGetTexParameterIuivEXT
    { 0x00000001411A3750, 0, }, // glGetTextureLevelParameterfvEXT
    { 0x00000001411A3758, 0, }, // glGetTextureLevelParameterivEXT
    { 0x00000001411A3740, 0, }, // glGetTextureParameterfvEXT
    { 0x00000001411A39D8, 0, }, // glGetTextureParameterIivEXT
    { 0x00000001411A39E0, 0, }, // glGetTextureParameterIuivEXT
    { 0x00000001411A3748, 0, }, // glGetTextureParameterivEXT
    { 0x00000001411A50D0, 0, }, // glGetTrackMatrixivNV
    { 0x00000001411A45A8, 0, }, // glGetTransformFeedbackVarying
    { 0x00000001411A35A0, 0, }, // glGetTransformFeedbackVaryingNV
    { 0x00000001411A35B0, 0, }, // glGetUniformBufferSizeEXT
    { 0x00000001411A4300, 0, }, // glGetUniformfv
    { 0x00000001411A4CC8, 0, }, // glGetUniformfvARB
    { 0x00000001411A4308, 0, }, // glGetUniformiv
    { 0x00000001411A4CD0, 0, }, // glGetUniformivARB
    { 0x00000001411A42F8, 0, }, // glGetUniformLocation
    { 0x00000001411A4CB8, 0, }, // glGetUniformLocationARB
    { 0x00000001411A35B8, 0, }, // glGetUniformOffsetEXT
    { 0x00000001411A4678, 0, }, // glGetUniformuiv
    { 0x00000001411A56B8, 0, }, // glGetUniformuivEXT
    { 0x00000001411A3590, 0, }, // glGetVaryingLocationNV
    { 0x00000001411A4310, 0, }, // glGetVertexAttribdv
    { 0x00000001411A4AE8, 0, }, // glGetVertexAttribdvARB
    { 0x00000001411A50D8, 0, }, // glGetVertexAttribdvNV
    { 0x00000001411A4318, 0, }, // glGetVertexAttribfv
    { 0x00000001411A4AF0, 0, }, // glGetVertexAttribfvARB
    { 0x00000001411A50E0, 0, }, // glGetVertexAttribfvNV
    { 0x00000001411A4668, 0, }, // glGetVertexAttribIiv
    { 0x00000001411A56A8, 0, }, // glGetVertexAttribIivEXT
    { 0x00000001411A4670, 0, }, // glGetVertexAttribIuiv
    { 0x00000001411A56B0, 0, }, // glGetVertexAttribIuivEXT
    { 0x00000001411A4320, 0, }, // glGetVertexAttribiv
    { 0x00000001411A4AF8, 0, }, // glGetVertexAttribivARB
    { 0x00000001411A50E8, 0, }, // glGetVertexAttribivNV
    { 0x00000001411A4328, 0, }, // glGetVertexAttribPointerv
    { 0x00000001411A4B00, 0, }, // glGetVertexAttribPointervARB
    { 0x00000001411A50F0, 0, }, // glGetVertexAttribPointervNV
    { 0x00000001411A3620, 0, }, // glGetVideoi64vNV
    { 0x00000001411A3610, 0, }, // glGetVideoivNV
    { 0x00000001411A3628, 0, }, // glGetVideoui64vNV
    { 0x00000001411A3618, 0, }, // glGetVideouivNV
    { 0x00000001411A3D48, 0, }, // glHistogram
    { 0x00000001411A4E88, 0, }, // glIndexPointerEXT
    { 0x00000001411A41F8, 0, }, // glIsBuffer
    { 0x00000001411A4578, 0, }, // glIsEnabledi
    { 0x00000001411A3548, 0, }, // glIsEnabledIndexedEXT
    { 0x00000001411A5050, 0, }, // glIsFenceNV
    { 0x00000001411A4D48, 0, }, // glIsFramebuffer
    { 0x00000001411A54D0, 0, }, // glIsFramebufferEXT
    { 0x00000001411A5298, 0, }, // glIsOcclusionQueryNV
    { 0x00000001411A4330, 0, }, // glIsProgram
    { 0x00000001411A4B08, 0, }, // glIsProgramARB
    { 0x00000001411A50F8, 0, }, // glIsProgramNV
    { 0x00000001411A41B0, 0, }, // glIsQuery
    { 0x00000001411A4B78, 0, }, // glIsQueryARB
    { 0x00000001411A4D18, 0, }, // glIsRenderbuffer
    { 0x00000001411A54A0, 0, }, // glIsRenderbufferEXT
    { 0x00000001411A4338, 0, }, // glIsShader
    { 0x00000001411A4E50, 0, }, // glIsTextureEXT
    { 0x00000001411A4E08, 0, }, // glIsVertexArray
    { 0x00000001411A4340, 0, }, // glLinkProgram
    { 0x00000001411A4BE8, 0, }, // glLinkProgramARB
    { 0x00000001411A5100, 0, }, // glLoadProgramNV
    { 0x00000001411A3FE0, 0, }, // glLoadTransposeMatrixd
    { 0x00000001411A4838, 0, }, // glLoadTransposeMatrixdARB
    { 0x00000001411A4830, 0, }, // glLoadTransposeMatrixfARB
    { 0x00000001411A4EE8, 0, }, // glLockArraysEXT
    { 0x00000001411A4218, 0, }, // glMapBuffer
    { 0x00000001411A4B48, 0, }, // glMapBufferARB
    { 0x00000001411A4DD8, 0, }, // glMapBufferRange
    { 0x00000001411A3698, 0, }, // glMatrixFrustumEXT
    { 0x00000001411A3648, 0, }, // glMatrixLoaddEXT
    { 0x00000001411A3640, 0, }, // glMatrixLoadfEXT
    { 0x00000001411A3660, 0, }, // glMatrixLoadIdentityEXT
    { 0x00000001411A36C0, 0, }, // glMatrixLoadTransposedEXT
    { 0x00000001411A36B8, 0, }, // glMatrixLoadTransposefEXT
    { 0x00000001411A3658, 0, }, // glMatrixMultdEXT
    { 0x00000001411A3650, 0, }, // glMatrixMultfEXT
    { 0x00000001411A36D0, 0, }, // glMatrixMultTransposedEXT
    { 0x00000001411A36C8, 0, }, // glMatrixMultTransposefEXT
    { 0x00000001411A36A0, 0, }, // glMatrixOrthoEXT
    { 0x00000001411A36A8, 0, }, // glMatrixPopEXT
    { 0x00000001411A36B0, 0, }, // glMatrixPushEXT
    { 0x00000001411A3670, 0, }, // glMatrixRotatedEXT
    { 0x00000001411A3668, 0, }, // glMatrixRotatefEXT
    { 0x00000001411A3680, 0, }, // glMatrixScaledEXT
    { 0x00000001411A3678, 0, }, // glMatrixScalefEXT
    { 0x00000001411A3690, 0, }, // glMatrixTranslatedEXT
    { 0x00000001411A3688, 0, }, // glMatrixTranslatefEXT
    { 0x00000001411A3D58, 0, }, // glMinmax
    { 0x00000001411A4068, 0, }, // glMultiDrawArrays
    { 0x00000001411A4F88, 0, }, // glMultiDrawArraysEXT
    { 0x00000001411A4070, 0, }, // glMultiDrawElements
    { 0x00000001411A4F90, 0, }, // glMultiDrawElementsEXT
    { 0x00000001411A3B50, 0, }, // glMultiTexBufferEXT
    { 0x00000001411A3DD8, 0, }, // glMultiTexCoord1d
    { 0x00000001411A4730, 0, }, // glMultiTexCoord1dARB
    { 0x00000001411A3DE8, 0, }, // glMultiTexCoord1dv
    { 0x00000001411A4738, 0, }, // glMultiTexCoord1dvARB
    { 0x00000001411A3DF8, 0, }, // glMultiTexCoord1f
    { 0x00000001411A4740, 0, }, // glMultiTexCoord1fARB
    { 0x00000001411A3E08, 0, }, // glMultiTexCoord1fv
    { 0x00000001411A4748, 0, }, // glMultiTexCoord1fvARB
    { 0x00000001411A53B0, 0, }, // glMultiTexCoord1hNV
    { 0x00000001411A53B8, 0, }, // glMultiTexCoord1hvNV
    { 0x00000001411A3E18, 0, }, // glMultiTexCoord1i
    { 0x00000001411A4750, 0, }, // glMultiTexCoord1iARB
    { 0x00000001411A3E28, 0, }, // glMultiTexCoord1iv
    { 0x00000001411A4758, 0, }, // glMultiTexCoord1ivARB
    { 0x00000001411A3E38, 0, }, // glMultiTexCoord1s
    { 0x00000001411A4760, 0, }, // glMultiTexCoord1sARB
    { 0x00000001411A3E48, 0, }, // glMultiTexCoord1sv
    { 0x00000001411A4768, 0, }, // glMultiTexCoord1svARB
    { 0x00000001411A3E58, 0, }, // glMultiTexCoord2d
    { 0x00000001411A4770, 0, }, // glMultiTexCoord2dARB
    { 0x00000001411A3E68, 0, }, // glMultiTexCoord2dv
    { 0x00000001411A4778, 0, }, // glMultiTexCoord2dvARB
    { 0x00000001411A4780, 0, }, // glMultiTexCoord2fARB
    { 0x00000001411A4788, 0, }, // glMultiTexCoord2fvARB
    { 0x00000001411A53C0, 0, }, // glMultiTexCoord2hNV
    { 0x00000001411A53C8, 0, }, // glMultiTexCoord2hvNV
    { 0x00000001411A3E98, 0, }, // glMultiTexCoord2i
    { 0x00000001411A4790, 0, }, // glMultiTexCoord2iARB
    { 0x00000001411A3EA8, 0, }, // glMultiTexCoord2iv
    { 0x00000001411A4798, 0, }, // glMultiTexCoord2ivARB
    { 0x00000001411A3EB8, 0, }, // glMultiTexCoord2s
    { 0x00000001411A47A0, 0, }, // glMultiTexCoord2sARB
    { 0x00000001411A3EC8, 0, }, // glMultiTexCoord2sv
    { 0x00000001411A47A8, 0, }, // glMultiTexCoord2svARB
    { 0x00000001411A3ED8, 0, }, // glMultiTexCoord3d
    { 0x00000001411A47B0, 0, }, // glMultiTexCoord3dARB
    { 0x00000001411A3EE8, 0, }, // glMultiTexCoord3dv
    { 0x00000001411A47B8, 0, }, // glMultiTexCoord3dvARB
    { 0x00000001411A3EF8, 0, }, // glMultiTexCoord3f
    { 0x00000001411A47C0, 0, }, // glMultiTexCoord3fARB
    { 0x00000001411A3F08, 0, }, // glMultiTexCoord3fv
    { 0x00000001411A47C8, 0, }, // glMultiTexCoord3fvARB
    { 0x00000001411A53D0, 0, }, // glMultiTexCoord3hNV
    { 0x00000001411A53D8, 0, }, // glMultiTexCoord3hvNV
    { 0x00000001411A3F18, 0, }, // glMultiTexCoord3i
    { 0x00000001411A47D0, 0, }, // glMultiTexCoord3iARB
    { 0x00000001411A3F28, 0, }, // glMultiTexCoord3iv
    { 0x00000001411A47D8, 0, }, // glMultiTexCoord3ivARB
    { 0x00000001411A3F38, 0, }, // glMultiTexCoord3s
    { 0x00000001411A47E0, 0, }, // glMultiTexCoord3sARB
    { 0x00000001411A3F48, 0, }, // glMultiTexCoord3sv
    { 0x00000001411A47E8, 0, }, // glMultiTexCoord3svARB
    { 0x00000001411A3F58, 0, }, // glMultiTexCoord4d
    { 0x00000001411A47F0, 0, }, // glMultiTexCoord4dARB
    { 0x00000001411A3F68, 0, }, // glMultiTexCoord4dv
    { 0x00000001411A47F8, 0, }, // glMultiTexCoord4dvARB
    { 0x00000001411A3F78, 0, }, // glMultiTexCoord4f
    { 0x00000001411A4800, 0, }, // glMultiTexCoord4fARB
    { 0x00000001411A3F88, 0, }, // glMultiTexCoord4fv
    { 0x00000001411A4808, 0, }, // glMultiTexCoord4fvARB
    { 0x00000001411A53E0, 0, }, // glMultiTexCoord4hNV
    { 0x00000001411A53E8, 0, }, // glMultiTexCoord4hvNV
    { 0x00000001411A3F98, 0, }, // glMultiTexCoord4i
    { 0x00000001411A4810, 0, }, // glMultiTexCoord4iARB
    { 0x00000001411A3FA8, 0, }, // glMultiTexCoord4iv
    { 0x00000001411A4818, 0, }, // glMultiTexCoord4ivARB
    { 0x00000001411A3FB8, 0, }, // glMultiTexCoord4s
    { 0x00000001411A4820, 0, }, // glMultiTexCoord4sARB
    { 0x00000001411A3FC8, 0, }, // glMultiTexCoord4sv
    { 0x00000001411A4828, 0, }, // glMultiTexCoord4svARB
    { 0x00000001411A3830, 0, }, // glMultiTexCoordPointerEXT
    { 0x00000001411A3838, 0, }, // glMultiTexEnvfEXT
    { 0x00000001411A3840, 0, }, // glMultiTexEnvfvEXT
    { 0x00000001411A3848, 0, }, // glMultiTexEnviEXT
    { 0x00000001411A3850, 0, }, // glMultiTexEnvivEXT
    { 0x00000001411A3858, 0, }, // glMultiTexGendEXT
    { 0x00000001411A3860, 0, }, // glMultiTexGendvEXT
    { 0x00000001411A3868, 0, }, // glMultiTexGenfEXT
    { 0x00000001411A3870, 0, }, // glMultiTexGenfvEXT
    { 0x00000001411A3878, 0, }, // glMultiTexGeniEXT
    { 0x00000001411A3880, 0, }, // glMultiTexGenivEXT
    { 0x00000001411A3798, 0, }, // glMultiTexImage1DEXT
    { 0x00000001411A37A0, 0, }, // glMultiTexImage2DEXT
    { 0x00000001411A3800, 0, }, // glMultiTexImage3DEXT
    { 0x00000001411A3778, 0, }, // glMultiTexParameterfEXT
    { 0x00000001411A3780, 0, }, // glMultiTexParameterfvEXT
    { 0x00000001411A3788, 0, }, // glMultiTexParameteriEXT
    { 0x00000001411A39E8, 0, }, // glMultiTexParameterIivEXT
    { 0x00000001411A39F0, 0, }, // glMultiTexParameterIuivEXT
    { 0x00000001411A3790, 0, }, // glMultiTexParameterivEXT
    { 0x00000001411A3CA0, 0, }, // glMultiTexRenderbufferEXT
    { 0x00000001411A37A8, 0, }, // glMultiTexSubImage1DEXT
    { 0x00000001411A37B0, 0, }, // glMultiTexSubImage2DEXT
    { 0x00000001411A3808, 0, }, // glMultiTexSubImage3DEXT
    { 0x00000001411A3FF0, 0, }, // glMultTransposeMatrixd
    { 0x00000001411A4848, 0, }, // glMultTransposeMatrixdARB
    { 0x00000001411A4840, 0, }, // glMultTransposeMatrixfARB
    { 0x00000001411A3BC0, 0, }, // glNamedFramebufferRenderbufferEXT
    { 0x00000001411A3B90, 0, }, // glNamedFramebufferTexture1DEXT
    { 0x00000001411A3BA0, 0, }, // glNamedFramebufferTexture2DEXT
    { 0x00000001411A3BB0, 0, }, // glNamedFramebufferTexture3DEXT
    { 0x00000001411A3C60, 0, }, // glNamedFramebufferTextureEXT
    { 0x00000001411A3C80, 0, }, // glNamedFramebufferTextureFaceEXT
    { 0x00000001411A3C70, 0, }, // glNamedFramebufferTextureLayerEXT
    { 0x00000001411A3940, 0, }, // glNamedProgramLocalParameter4dEXT
    { 0x00000001411A3948, 0, }, // glNamedProgramLocalParameter4dvEXT
    { 0x00000001411A3950, 0, }, // glNamedProgramLocalParameter4fEXT
    { 0x00000001411A3958, 0, }, // glNamedProgramLocalParameter4fvEXT
    { 0x00000001411A3988, 0, }, // glNamedProgramLocalParameterI4iEXT
    { 0x00000001411A3990, 0, }, // glNamedProgramLocalParameterI4ivEXT
    { 0x00000001411A39A0, 0, }, // glNamedProgramLocalParameterI4uiEXT
    { 0x00000001411A39A8, 0, }, // glNamedProgramLocalParameterI4uivEXT
    { 0x00000001411A3980, 0, }, // glNamedProgramLocalParameters4fvEXT
    { 0x00000001411A3998, 0, }, // glNamedProgramLocalParametersI4ivEXT
    { 0x00000001411A39B0, 0, }, // glNamedProgramLocalParametersI4uivEXT
    { 0x00000001411A3938, 0, }, // glNamedProgramStringEXT
    { 0x00000001411A3B60, 0, }, // glNamedRenderbufferStorageEXT
    { 0x00000001411A3C50, 0, }, // glNamedRenderbufferStorageMultisampleCoverageEXT
    { 0x00000001411A3C40, 0, }, // glNamedRenderbufferStorageMultisampleEXT
    { 0x00000001411A5340, 0, }, // glNormal3hNV
    { 0x00000001411A5348, 0, }, // glNormal3hvNV
    { 0x00000001411A4E90, 0, }, // glNormalPointerEXT
    { 0x00000001411A5470, 0, }, // glPixelDataRangeNV
    { 0x00000001411A4890, 0, }, // glPointParameterfARB
    { 0x00000001411A4EB0, 0, }, // glPointParameterfEXT
    { 0x00000001411A4898, 0, }, // glPointParameterfvARB
    { 0x00000001411A4EB8, 0, }, // glPointParameterfvEXT
    { 0x00000001411A52C0, 0, }, // glPointParameteriNV
    { 0x00000001411A4090, 0, }, // glPointParameteriv
    { 0x00000001411A52C8, 0, }, // glPointParameterivNV
    { 0x00000001411A3608, 0, }, // glPresentFrameDualFillNV
    { 0x00000001411A3600, 0, }, // glPresentFrameKeyedNV
    { 0x00000001411A5480, 0, }, // glPrimitiveRestartNV
    { 0x00000001411A4E58, 0, }, // glPrioritizeTexturesEXT
    { 0x00000001411A3510, 0, }, // glProgramBufferParametersIivNV
    { 0x00000001411A3518, 0, }, // glProgramBufferParametersIuivNV
    { 0x00000001411A4A78, 0, }, // glProgramEnvParameter4dARB
    { 0x00000001411A4A80, 0, }, // glProgramEnvParameter4dvARB
    { 0x00000001411A5588, 0, }, // glProgramEnvParameterI4iNV
    { 0x00000001411A5590, 0, }, // glProgramEnvParameterI4ivNV
    { 0x00000001411A55A0, 0, }, // glProgramEnvParameterI4uiNV
    { 0x00000001411A55A8, 0, }, // glProgramEnvParameterI4uivNV
    { 0x00000001411A5598, 0, }, // glProgramEnvParametersI4ivNV
    { 0x00000001411A55B0, 0, }, // glProgramEnvParametersI4uivNV
    { 0x00000001411A4A98, 0, }, // glProgramLocalParameter4dARB
    { 0x00000001411A4AA0, 0, }, // glProgramLocalParameter4dvARB
    { 0x00000001411A5558, 0, }, // glProgramLocalParameterI4iNV
    { 0x00000001411A5560, 0, }, // glProgramLocalParameterI4ivNV
    { 0x00000001411A5570, 0, }, // glProgramLocalParameterI4uiNV
    { 0x00000001411A5578, 0, }, // glProgramLocalParameterI4uivNV
    { 0x00000001411A5568, 0, }, // glProgramLocalParametersI4ivNV
    { 0x00000001411A5580, 0, }, // glProgramLocalParametersI4uivNV
    { 0x00000001411A52E8, 0, }, // glProgramNamedParameter4dNV
    { 0x00000001411A52F8, 0, }, // glProgramNamedParameter4dvNV
    { 0x00000001411A52E0, 0, }, // glProgramNamedParameter4fNV
    { 0x00000001411A52F0, 0, }, // glProgramNamedParameter4fvNV
    { 0x00000001411A5108, 0, }, // glProgramParameter4dNV
    { 0x00000001411A5110, 0, }, // glProgramParameter4dvNV
    { 0x00000001411A5118, 0, }, // glProgramParameter4fNV
    { 0x00000001411A5120, 0, }, // glProgramParameter4fvNV
    { 0x00000001411A4DB8, 0, }, // glProgramParameteriARB
    { 0x00000001411A55F8, 0, }, // glProgramParameteriEXT
    { 0x00000001411A5128, 0, }, // glProgramParameters4dvNV
    { 0x00000001411A5130, 0, }, // glProgramParameters4fvNV
    { 0x00000001411A4A58, 0, }, // glProgramStringARB
    { 0x00000001411A3A08, 0, }, // glProgramUniform1fEXT
    { 0x00000001411A3A48, 0, }, // glProgramUniform1fvEXT
    { 0x00000001411A3A28, 0, }, // glProgramUniform1iEXT
    { 0x00000001411A3A68, 0, }, // glProgramUniform1ivEXT
    { 0x00000001411A3AD0, 0, }, // glProgramUniform1uiEXT
    { 0x00000001411A3AF0, 0, }, // glProgramUniform1uivEXT
    { 0x00000001411A3A10, 0, }, // glProgramUniform2fEXT
    { 0x00000001411A3A50, 0, }, // glProgramUniform2fvEXT
    { 0x00000001411A3A30, 0, }, // glProgramUniform2iEXT
    { 0x00000001411A3A70, 0, }, // glProgramUniform2ivEXT
    { 0x00000001411A3AD8, 0, }, // glProgramUniform2uiEXT
    { 0x00000001411A3AF8, 0, }, // glProgramUniform2uivEXT
    { 0x00000001411A3A18, 0, }, // glProgramUniform3fEXT
    { 0x00000001411A3A58, 0, }, // glProgramUniform3fvEXT
    { 0x00000001411A3A38, 0, }, // glProgramUniform3iEXT
    { 0x00000001411A3A78, 0, }, // glProgramUniform3ivEXT
    { 0x00000001411A3AE0, 0, }, // glProgramUniform3uiEXT
    { 0x00000001411A3B00, 0, }, // glProgramUniform3uivEXT
    { 0x00000001411A3A20, 0, }, // glProgramUniform4fEXT
    { 0x00000001411A3A60, 0, }, // glProgramUniform4fvEXT
    { 0x00000001411A3A40, 0, }, // glProgramUniform4iEXT
    { 0x00000001411A3A80, 0, }, // glProgramUniform4ivEXT
    { 0x00000001411A3AE8, 0, }, // glProgramUniform4uiEXT
    { 0x00000001411A3B08, 0, }, // glProgramUniform4uivEXT
    { 0x00000001411A3A88, 0, }, // glProgramUniformMatrix2fvEXT
    { 0x00000001411A3AA0, 0, }, // glProgramUniformMatrix2x3fvEXT
    { 0x00000001411A3AB0, 0, }, // glProgramUniformMatrix2x4fvEXT
    { 0x00000001411A3A90, 0, }, // glProgramUniformMatrix3fvEXT
    { 0x00000001411A3AA8, 0, }, // glProgramUniformMatrix3x2fvEXT
    { 0x00000001411A3AC0, 0, }, // glProgramUniformMatrix3x4fvEXT
    { 0x00000001411A3A98, 0, }, // glProgramUniformMatrix4fvEXT
    { 0x00000001411A3AB8, 0, }, // glProgramUniformMatrix4x2fvEXT
    { 0x00000001411A3AC8, 0, }, // glProgramUniformMatrix4x3fvEXT
    { 0x00000001411A55D8, 0, }, // glProgramVertexLimitNV
    { 0x00000001411A3638, 0, }, // glPushClientAttribDefaultEXT
    { 0x00000001411A3F30, 0, }, // glQueryCounter
    { 0x00000001411A4D38, 0, }, // glRenderbufferStorage
    { 0x00000001411A4DA8, 0, }, // glRenderbufferStorageMultisample
    { 0x00000001411A5530, 0, }, // glRenderbufferStorageMultisampleEXT
    { 0x00000001411A5138, 0, }, // glRequestResidentProgramsNV
    { 0x00000001411A3D68, 0, }, // glResetHistogram
    { 0x00000001411A3D78, 0, }, // glResetMinmax
    { 0x00000001411A3FF8, 0, }, // glSampleCoverage
    { 0x00000001411A4850, 0, }, // glSampleCoverageARB
    { 0x00000001411A3CC0, 0, }, // glSampleMaskIndexedNV
    { 0x00000001411A3FC0, 0, }, // glSamplerParameterf
    { 0x00000001411A3FD0, 0, }, // glSamplerParameterfv
    { 0x00000001411A3FB0, 0, }, // glSamplerParameteri
    { 0x00000001411A4098, 0, }, // glSecondaryColor3b
    { 0x00000001411A4F00, 0, }, // glSecondaryColor3bEXT
    { 0x00000001411A40A0, 0, }, // glSecondaryColor3bv
    { 0x00000001411A4F08, 0, }, // glSecondaryColor3bvEXT
    { 0x00000001411A40A8, 0, }, // glSecondaryColor3d
    { 0x00000001411A4F10, 0, }, // glSecondaryColor3dEXT
    { 0x00000001411A40B0, 0, }, // glSecondaryColor3dv
    { 0x00000001411A4F18, 0, }, // glSecondaryColor3dvEXT
    { 0x00000001411A40B8, 0, }, // glSecondaryColor3f
    { 0x00000001411A4F20, 0, }, // glSecondaryColor3fEXT
    { 0x00000001411A40C0, 0, }, // glSecondaryColor3fv
    { 0x00000001411A4F28, 0, }, // glSecondaryColor3fvEXT
    { 0x00000001411A5400, 0, }, // glSecondaryColor3hNV
    { 0x00000001411A5408, 0, }, // glSecondaryColor3hvNV
    { 0x00000001411A40C8, 0, }, // glSecondaryColor3i
    { 0x00000001411A4F30, 0, }, // glSecondaryColor3iEXT
    { 0x00000001411A40D0, 0, }, // glSecondaryColor3iv
    { 0x00000001411A4F38, 0, }, // glSecondaryColor3ivEXT
    { 0x00000001411A40D8, 0, }, // glSecondaryColor3s
    { 0x00000001411A4F40, 0, }, // glSecondaryColor3sEXT
    { 0x00000001411A40E0, 0, }, // glSecondaryColor3sv
    { 0x00000001411A4F48, 0, }, // glSecondaryColor3svEXT
    { 0x00000001411A40E8, 0, }, // glSecondaryColor3ub
    { 0x00000001411A4F50, 0, }, // glSecondaryColor3ubEXT
    { 0x00000001411A40F0, 0, }, // glSecondaryColor3ubv
    { 0x00000001411A4F58, 0, }, // glSecondaryColor3ubvEXT
    { 0x00000001411A40F8, 0, }, // glSecondaryColor3ui
    { 0x00000001411A4F60, 0, }, // glSecondaryColor3uiEXT
    { 0x00000001411A4100, 0, }, // glSecondaryColor3uiv
    { 0x00000001411A4F68, 0, }, // glSecondaryColor3uivEXT
    { 0x00000001411A4108, 0, }, // glSecondaryColor3us
    { 0x00000001411A4F70, 0, }, // glSecondaryColor3usEXT
    { 0x00000001411A4110, 0, }, // glSecondaryColor3usv
    { 0x00000001411A4F78, 0, }, // glSecondaryColor3usvEXT
    { 0x00000001411A4118, 0, }, // glSecondaryColorPointer
    { 0x00000001411A4F80, 0, }, // glSecondaryColorPointerEXT
    { 0x00000001411A3CD8, 0, }, // glSeparableFilter2D
    { 0x00000001411A5070, 0, }, // glSetFenceNV
    { 0x00000001411A4348, 0, }, // glShaderSource
    { 0x00000001411A4BC8, 0, }, // glShaderSourceARB
    { 0x00000001411A4250, 0, }, // glStencilFuncSeparate
    { 0x00000001411A4258, 0, }, // glStencilMaskSeparate
    { 0x00000001411A4248, 0, }, // glStencilOpSeparate
    { 0x00000001411A5058, 0, }, // glTestFenceNV
    { 0x00000001411A4DE8, 0, }, // glTexBufferARB
    { 0x00000001411A34E0, 0, }, // glTexBufferEXT
    { 0x00000001411A5370, 0, }, // glTexCoord1hNV
    { 0x00000001411A5378, 0, }, // glTexCoord1hvNV
    { 0x00000001411A5380, 0, }, // glTexCoord2hNV
    { 0x00000001411A5388, 0, }, // glTexCoord2hvNV
    { 0x00000001411A5390, 0, }, // glTexCoord3hNV
    { 0x00000001411A5398, 0, }, // glTexCoord3hvNV
    { 0x00000001411A53A0, 0, }, // glTexCoord4hNV
    { 0x00000001411A53A8, 0, }, // glTexCoord4hvNV
    { 0x00000001411A4E98, 0, }, // glTexCoordPointerEXT
    { 0x00000001411A3D88, 0, }, // glTexImage3D
    { 0x00000001411A4E18, 0, }, // glTexImage3DEXT
    { 0x00000001411A46D0, 0, }, // glTexParameterIiv
    { 0x00000001411A35C0, 0, }, // glTexParameterIivEXT
    { 0x00000001411A46D8, 0, }, // glTexParameterIuiv
    { 0x00000001411A35C8, 0, }, // glTexParameterIuivEXT
    { 0x00000001411A3CD0, 0, }, // glTexRenderbufferNV
    { 0x00000001411A3F60, 0, }, // glTexStorage2D
    { 0x00000001411A3D98, 0, }, // glTexSubImage3D
    { 0x00000001411A4E20, 0, }, // glTexSubImage3DEXT
    { 0x00000001411A3CE0, 0, }, // glTextureBarrierNV
    { 0x00000001411A3B48, 0, }, // glTextureBufferEXT
    { 0x00000001411A36F8, 0, }, // glTextureImage1DEXT
    { 0x00000001411A3700, 0, }, // glTextureImage2DEXT
    { 0x00000001411A3760, 0, }, // glTextureImage3DEXT
    { 0x00000001411A36D8, 0, }, // glTextureParameterfEXT
    { 0x00000001411A36E0, 0, }, // glTextureParameterfvEXT
    { 0x00000001411A36E8, 0, }, // glTextureParameteriEXT
    { 0x00000001411A39C8, 0, }, // glTextureParameterIivEXT
    { 0x00000001411A39D0, 0, }, // glTextureParameterIuivEXT
    { 0x00000001411A36F0, 0, }, // glTextureParameterivEXT
    { 0x00000001411A3C90, 0, }, // glTextureRenderbufferEXT
    { 0x00000001411A3F70, 0, }, // glTextureStorage2DEXT
    { 0x00000001411A3708, 0, }, // glTextureSubImage1DEXT
    { 0x00000001411A3710, 0, }, // glTextureSubImage2DEXT
    { 0x00000001411A3768, 0, }, // glTextureSubImage3DEXT
    { 0x00000001411A5140, 0, }, // glTrackMatrixNV
    { 0x00000001411A3560, 0, }, // glTransformFeedbackAttribsNV
    { 0x00000001411A45A0, 0, }, // glTransformFeedbackVaryings
    { 0x00000001411A3580, 0, }, // glTransformFeedbackVaryingsNV
    { 0x00000001411A4358, 0, }, // glUniform1f
    { 0x00000001411A4C00, 0, }, // glUniform1fARB
    { 0x00000001411A4398, 0, }, // glUniform1fv
    { 0x00000001411A4C40, 0, }, // glUniform1fvARB
    { 0x00000001411A4378, 0, }, // glUniform1i
    { 0x00000001411A4C20, 0, }, // glUniform1iARB
    { 0x00000001411A43B8, 0, }, // glUniform1iv
    { 0x00000001411A4C60, 0, }, // glUniform1ivARB
    { 0x00000001411A4690, 0, }, // glUniform1ui
    { 0x00000001411A56D0, 0, }, // glUniform1uiEXT
    { 0x00000001411A46B0, 0, }, // glUniform1uiv
    { 0x00000001411A56F0, 0, }, // glUniform1uivEXT
    { 0x00000001411A4360, 0, }, // glUniform2f
    { 0x00000001411A4C08, 0, }, // glUniform2fARB
    { 0x00000001411A43A0, 0, }, // glUniform2fv
    { 0x00000001411A4C48, 0, }, // glUniform2fvARB
    { 0x00000001411A4380, 0, }, // glUniform2i
    { 0x00000001411A4C28, 0, }, // glUniform2iARB
    { 0x00000001411A43C0, 0, }, // glUniform2iv
    { 0x00000001411A4C68, 0, }, // glUniform2ivARB
    { 0x00000001411A4698, 0, }, // glUniform2ui
    { 0x00000001411A56D8, 0, }, // glUniform2uiEXT
    { 0x00000001411A46B8, 0, }, // glUniform2uiv
    { 0x00000001411A56F8, 0, }, // glUniform2uivEXT
    { 0x00000001411A4368, 0, }, // glUniform3f
    { 0x00000001411A4C10, 0, }, // glUniform3fARB
    { 0x00000001411A43A8, 0, }, // glUniform3fv
    { 0x00000001411A4C50, 0, }, // glUniform3fvARB
    { 0x00000001411A4388, 0, }, // glUniform3i
    { 0x00000001411A4C30, 0, }, // glUniform3iARB
    { 0x00000001411A43C8, 0, }, // glUniform3iv
    { 0x00000001411A4C70, 0, }, // glUniform3ivARB
    { 0x00000001411A46A0, 0, }, // glUniform3ui
    { 0x00000001411A56E0, 0, }, // glUniform3uiEXT
    { 0x00000001411A46C0, 0, }, // glUniform3uiv
    { 0x00000001411A5700, 0, }, // glUniform3uivEXT
    { 0x00000001411A4370, 0, }, // glUniform4f
    { 0x00000001411A4C18, 0, }, // glUniform4fARB
    { 0x00000001411A43B0, 0, }, // glUniform4fv
    { 0x00000001411A4C58, 0, }, // glUniform4fvARB
    { 0x00000001411A4390, 0, }, // glUniform4i
    { 0x00000001411A4C38, 0, }, // glUniform4iARB
    { 0x00000001411A43D0, 0, }, // glUniform4iv
    { 0x00000001411A4C78, 0, }, // glUniform4ivARB
    { 0x00000001411A46A8, 0, }, // glUniform4ui
    { 0x00000001411A56E8, 0, }, // glUniform4uiEXT
    { 0x00000001411A46C8, 0, }, // glUniform4uiv
    { 0x00000001411A5708, 0, }, // glUniform4uivEXT
    { 0x00000001411A35A8, 0, }, // glUniformBufferEXT
    { 0x00000001411A43D8, 0, }, // glUniformMatrix2fv
    { 0x00000001411A4C80, 0, }, // glUniformMatrix2fvARB
    { 0x00000001411A4520, 0, }, // glUniformMatrix2x3fv
    { 0x00000001411A4530, 0, }, // glUniformMatrix2x4fv
    { 0x00000001411A43E0, 0, }, // glUniformMatrix3fv
    { 0x00000001411A4C88, 0, }, // glUniformMatrix3fvARB
    { 0x00000001411A4528, 0, }, // glUniformMatrix3x2fv
    { 0x00000001411A4540, 0, }, // glUniformMatrix3x4fv
    { 0x00000001411A43E8, 0, }, // glUniformMatrix4fv
    { 0x00000001411A4C90, 0, }, // glUniformMatrix4fvARB
    { 0x00000001411A4538, 0, }, // glUniformMatrix4x2fv
    { 0x00000001411A4548, 0, }, // glUniformMatrix4x3fv
    { 0x00000001411A4EF0, 0, }, // glUnlockArraysEXT
    { 0x00000001411A4220, 0, }, // glUnmapBuffer
    { 0x00000001411A4B50, 0, }, // glUnmapBufferARB
    { 0x00000001411A4BF0, 0, }, // glUseProgramObjectARB
    { 0x00000001411A43F0, 0, }, // glValidateProgram
    { 0x00000001411A4BF8, 0, }, // glValidateProgramARB
    { 0x00000001411A5310, 0, }, // glVertex2hNV
    { 0x00000001411A5318, 0, }, // glVertex2hvNV
    { 0x00000001411A5320, 0, }, // glVertex3hNV
    { 0x00000001411A5328, 0, }, // glVertex3hvNV
    { 0x00000001411A5330, 0, }, // glVertex4hNV
    { 0x00000001411A5338, 0, }, // glVertex4hvNV
    { 0x00000001411A4FD0, 0, }, // glVertexArrayRangeNV
    { 0x00000001411A43F8, 0, }, // glVertexAttrib1d
    { 0x00000001411A4920, 0, }, // glVertexAttrib1dARB
    { 0x00000001411A5150, 0, }, // glVertexAttrib1dNV
    { 0x00000001411A4400, 0, }, // glVertexAttrib1dv
    { 0x00000001411A4928, 0, }, // glVertexAttrib1dvARB
    { 0x00000001411A5158, 0, }, // glVertexAttrib1dvNV
    { 0x00000001411A4408, 0, }, // glVertexAttrib1f
    { 0x00000001411A4930, 0, }, // glVertexAttrib1fARB
    { 0x00000001411A5160, 0, }, // glVertexAttrib1fNV
    { 0x00000001411A4410, 0, }, // glVertexAttrib1fv
    { 0x00000001411A4938, 0, }, // glVertexAttrib1fvARB
    { 0x00000001411A5168, 0, }, // glVertexAttrib1fvNV
    { 0x00000001411A5410, 0, }, // glVertexAttrib1hNV
    { 0x00000001411A5418, 0, }, // glVertexAttrib1hvNV
    { 0x00000001411A4418, 0, }, // glVertexAttrib1s
    { 0x00000001411A4940, 0, }, // glVertexAttrib1sARB
    { 0x00000001411A5170, 0, }, // glVertexAttrib1sNV
    { 0x00000001411A4420, 0, }, // glVertexAttrib1sv
    { 0x00000001411A4948, 0, }, // glVertexAttrib1svARB
    { 0x00000001411A5178, 0, }, // glVertexAttrib1svNV
    { 0x00000001411A4428, 0, }, // glVertexAttrib2d
    { 0x00000001411A4950, 0, }, // glVertexAttrib2dARB
    { 0x00000001411A5180, 0, }, // glVertexAttrib2dNV
    { 0x00000001411A4430, 0, }, // glVertexAttrib2dv
    { 0x00000001411A4958, 0, }, // glVertexAttrib2dvARB
    { 0x00000001411A5188, 0, }, // glVertexAttrib2dvNV
    { 0x00000001411A4438, 0, }, // glVertexAttrib2f
    { 0x00000001411A4960, 0, }, // glVertexAttrib2fARB
    { 0x00000001411A5190, 0, }, // glVertexAttrib2fNV
    { 0x00000001411A4440, 0, }, // glVertexAttrib2fv
    { 0x00000001411A4968, 0, }, // glVertexAttrib2fvARB
    { 0x00000001411A5198, 0, }, // glVertexAttrib2fvNV
    { 0x00000001411A5420, 0, }, // glVertexAttrib2hNV
    { 0x00000001411A5428, 0, }, // glVertexAttrib2hvNV
    { 0x00000001411A4448, 0, }, // glVertexAttrib2s
    { 0x00000001411A4970, 0, }, // glVertexAttrib2sARB
    { 0x00000001411A51A0, 0, }, // glVertexAttrib2sNV
    { 0x00000001411A4450, 0, }, // glVertexAttrib2sv
    { 0x00000001411A4978, 0, }, // glVertexAttrib2svARB
    { 0x00000001411A51A8, 0, }, // glVertexAttrib2svNV
    { 0x00000001411A4458, 0, }, // glVertexAttrib3d
    { 0x00000001411A4980, 0, }, // glVertexAttrib3dARB
    { 0x00000001411A51B0, 0, }, // glVertexAttrib3dNV
    { 0x00000001411A4460, 0, }, // glVertexAttrib3dv
    { 0x00000001411A4988, 0, }, // glVertexAttrib3dvARB
    { 0x00000001411A51B8, 0, }, // glVertexAttrib3dvNV
    { 0x00000001411A4468, 0, }, // glVertexAttrib3f
    { 0x00000001411A4990, 0, }, // glVertexAttrib3fARB
    { 0x00000001411A51C0, 0, }, // glVertexAttrib3fNV
    { 0x00000001411A4470, 0, }, // glVertexAttrib3fv
    { 0x00000001411A4998, 0, }, // glVertexAttrib3fvARB
    { 0x00000001411A51C8, 0, }, // glVertexAttrib3fvNV
    { 0x00000001411A5430, 0, }, // glVertexAttrib3hNV
    { 0x00000001411A5438, 0, }, // glVertexAttrib3hvNV
    { 0x00000001411A4478, 0, }, // glVertexAttrib3s
    { 0x00000001411A49A0, 0, }, // glVertexAttrib3sARB
    { 0x00000001411A51D0, 0, }, // glVertexAttrib3sNV
    { 0x00000001411A4480, 0, }, // glVertexAttrib3sv
    { 0x00000001411A49A8, 0, }, // glVertexAttrib3svARB
    { 0x00000001411A51D8, 0, }, // glVertexAttrib3svNV
    { 0x00000001411A44C0, 0, }, // glVertexAttrib4bv
    { 0x00000001411A49E8, 0, }, // glVertexAttrib4bvARB
    { 0x00000001411A44C8, 0, }, // glVertexAttrib4d
    { 0x00000001411A49F0, 0, }, // glVertexAttrib4dARB
    { 0x00000001411A51E0, 0, }, // glVertexAttrib4dNV
    { 0x00000001411A44D0, 0, }, // glVertexAttrib4dv
    { 0x00000001411A49F8, 0, }, // glVertexAttrib4dvARB
    { 0x00000001411A51E8, 0, }, // glVertexAttrib4dvNV
    { 0x00000001411A44D8, 0, }, // glVertexAttrib4f
    { 0x00000001411A4A00, 0, }, // glVertexAttrib4fARB
    { 0x00000001411A51F0, 0, }, // glVertexAttrib4fNV
    { 0x00000001411A44E0, 0, }, // glVertexAttrib4fv
    { 0x00000001411A4A08, 0, }, // glVertexAttrib4fvARB
    { 0x00000001411A51F8, 0, }, // glVertexAttrib4fvNV
    { 0x00000001411A5440, 0, }, // glVertexAttrib4hNV
    { 0x00000001411A5448, 0, }, // glVertexAttrib4hvNV
    { 0x00000001411A44E8, 0, }, // glVertexAttrib4iv
    { 0x00000001411A4A10, 0, }, // glVertexAttrib4ivARB
    { 0x00000001411A4488, 0, }, // glVertexAttrib4Nbv
    { 0x00000001411A49B0, 0, }, // glVertexAttrib4NbvARB
    { 0x00000001411A4490, 0, }, // glVertexAttrib4Niv
    { 0x00000001411A49B8, 0, }, // glVertexAttrib4NivARB
    { 0x00000001411A4498, 0, }, // glVertexAttrib4Nsv
    { 0x00000001411A49C0, 0, }, // glVertexAttrib4NsvARB
    { 0x00000001411A44A0, 0, }, // glVertexAttrib4Nub
    { 0x00000001411A49C8, 0, }, // glVertexAttrib4NubARB
    { 0x00000001411A44A8, 0, }, // glVertexAttrib4Nubv
    { 0x00000001411A49D0, 0, }, // glVertexAttrib4NubvARB
    { 0x00000001411A44B0, 0, }, // glVertexAttrib4Nuiv
    { 0x00000001411A49D8, 0, }, // glVertexAttrib4NuivARB
    { 0x00000001411A44B8, 0, }, // glVertexAttrib4Nusv
    { 0x00000001411A49E0, 0, }, // glVertexAttrib4NusvARB
    { 0x00000001411A44F0, 0, }, // glVertexAttrib4s
    { 0x00000001411A4A18, 0, }, // glVertexAttrib4sARB
    { 0x00000001411A5200, 0, }, // glVertexAttrib4sNV
    { 0x00000001411A44F8, 0, }, // glVertexAttrib4sv
    { 0x00000001411A4A20, 0, }, // glVertexAttrib4svARB
    { 0x00000001411A5208, 0, }, // glVertexAttrib4svNV
    { 0x00000001411A5210, 0, }, // glVertexAttrib4ubNV
    { 0x00000001411A4500, 0, }, // glVertexAttrib4ubv
    { 0x00000001411A4A28, 0, }, // glVertexAttrib4ubvARB
    { 0x00000001411A5218, 0, }, // glVertexAttrib4ubvNV
    { 0x00000001411A4508, 0, }, // glVertexAttrib4uiv
    { 0x00000001411A4A30, 0, }, // glVertexAttrib4uivARB
    { 0x00000001411A4510, 0, }, // glVertexAttrib4usv
    { 0x00000001411A4A38, 0, }, // glVertexAttrib4usvARB
    { 0x00000001411A45C0, 0, }, // glVertexAttribI1i
    { 0x00000001411A5600, 0, }, // glVertexAttribI1iEXT
    { 0x00000001411A4600, 0, }, // glVertexAttribI1iv
    { 0x00000001411A5640, 0, }, // glVertexAttribI1ivEXT
    { 0x00000001411A45E0, 0, }, // glVertexAttribI1ui
    { 0x00000001411A5620, 0, }, // glVertexAttribI1uiEXT
    { 0x00000001411A4620, 0, }, // glVertexAttribI1uiv
    { 0x00000001411A5660, 0, }, // glVertexAttribI1uivEXT
    { 0x00000001411A45C8, 0, }, // glVertexAttribI2i
    { 0x00000001411A5608, 0, }, // glVertexAttribI2iEXT
    { 0x00000001411A4608, 0, }, // glVertexAttribI2iv
    { 0x00000001411A5648, 0, }, // glVertexAttribI2ivEXT
    { 0x00000001411A45E8, 0, }, // glVertexAttribI2ui
    { 0x00000001411A5628, 0, }, // glVertexAttribI2uiEXT
    { 0x00000001411A4628, 0, }, // glVertexAttribI2uiv
    { 0x00000001411A5668, 0, }, // glVertexAttribI2uivEXT
    { 0x00000001411A45D0, 0, }, // glVertexAttribI3i
    { 0x00000001411A5610, 0, }, // glVertexAttribI3iEXT
    { 0x00000001411A4610, 0, }, // glVertexAttribI3iv
    { 0x00000001411A5650, 0, }, // glVertexAttribI3ivEXT
    { 0x00000001411A45F0, 0, }, // glVertexAttribI3ui
    { 0x00000001411A5630, 0, }, // glVertexAttribI3uiEXT
    { 0x00000001411A4630, 0, }, // glVertexAttribI3uiv
    { 0x00000001411A5670, 0, }, // glVertexAttribI3uivEXT
    { 0x00000001411A4640, 0, }, // glVertexAttribI4bv
    { 0x00000001411A5680, 0, }, // glVertexAttribI4bvEXT
    { 0x00000001411A45D8, 0, }, // glVertexAttribI4i
    { 0x00000001411A5618, 0, }, // glVertexAttribI4iEXT
    { 0x00000001411A4618, 0, }, // glVertexAttribI4iv
    { 0x00000001411A5658, 0, }, // glVertexAttribI4ivEXT
    { 0x00000001411A4648, 0, }, // glVertexAttribI4sv
    { 0x00000001411A5688, 0, }, // glVertexAttribI4svEXT
    { 0x00000001411A4650, 0, }, // glVertexAttribI4ubv
    { 0x00000001411A5690, 0, }, // glVertexAttribI4ubvEXT
    { 0x00000001411A45F8, 0, }, // glVertexAttribI4ui
    { 0x00000001411A5638, 0, }, // glVertexAttribI4uiEXT
    { 0x00000001411A4638, 0, }, // glVertexAttribI4uiv
    { 0x00000001411A5678, 0, }, // glVertexAttribI4uivEXT
    { 0x00000001411A4658, 0, }, // glVertexAttribI4usv
    { 0x00000001411A5698, 0, }, // glVertexAttribI4usvEXT
    { 0x00000001411A4660, 0, }, // glVertexAttribIPointer
    { 0x00000001411A56A0, 0, }, // glVertexAttribIPointerEXT
    { 0x00000001411A4518, 0, }, // glVertexAttribPointer
    { 0x00000001411A5148, 0, }, // glVertexAttribPointerNV
    { 0x00000001411A5220, 0, }, // glVertexAttribs1dvNV
    { 0x00000001411A5228, 0, }, // glVertexAttribs1fvNV
    { 0x00000001411A5450, 0, }, // glVertexAttribs1hvNV
    { 0x00000001411A5230, 0, }, // glVertexAttribs1svNV
    { 0x00000001411A5238, 0, }, // glVertexAttribs2dvNV
    { 0x00000001411A5240, 0, }, // glVertexAttribs2fvNV
    { 0x00000001411A5458, 0, }, // glVertexAttribs2hvNV
    { 0x00000001411A5248, 0, }, // glVertexAttribs2svNV
    { 0x00000001411A5250, 0, }, // glVertexAttribs3dvNV
    { 0x00000001411A5258, 0, }, // glVertexAttribs3fvNV
    { 0x00000001411A5460, 0, }, // glVertexAttribs3hvNV
    { 0x00000001411A5260, 0, }, // glVertexAttribs3svNV
    { 0x00000001411A5268, 0, }, // glVertexAttribs4dvNV
    { 0x00000001411A5270, 0, }, // glVertexAttribs4fvNV
    { 0x00000001411A5468, 0, }, // glVertexAttribs4hvNV
    { 0x00000001411A5278, 0, }, // glVertexAttribs4svNV
    { 0x00000001411A5280, 0, }, // glVertexAttribs4ubvNV
    { 0x00000001411A4EA0, 0, }, // glVertexPointerEXT
    { 0x00000001411A4120, 0, }, // glWindowPos2d
    { 0x00000001411A48A0, 0, }, // glWindowPos2dARB
    { 0x00000001411A4128, 0, }, // glWindowPos2dv
    { 0x00000001411A48A8, 0, }, // glWindowPos2dvARB
    { 0x00000001411A4130, 0, }, // glWindowPos2f
    { 0x00000001411A48B0, 0, }, // glWindowPos2fARB
    { 0x00000001411A4138, 0, }, // glWindowPos2fv
    { 0x00000001411A48B8, 0, }, // glWindowPos2fvARB
    { 0x00000001411A4140, 0, }, // glWindowPos2i
    { 0x00000001411A48C0, 0, }, // glWindowPos2iARB
    { 0x00000001411A4148, 0, }, // glWindowPos2iv
    { 0x00000001411A48C8, 0, }, // glWindowPos2ivARB
    { 0x00000001411A4150, 0, }, // glWindowPos2s
    { 0x00000001411A48D0, 0, }, // glWindowPos2sARB
    { 0x00000001411A4158, 0, }, // glWindowPos2sv
    { 0x00000001411A48D8, 0, }, // glWindowPos2svARB
    { 0x00000001411A4160, 0, }, // glWindowPos3d
    { 0x00000001411A48E0, 0, }, // glWindowPos3dARB
    { 0x00000001411A4168, 0, }, // glWindowPos3dv
    { 0x00000001411A48E8, 0, }, // glWindowPos3dvARB
    { 0x00000001411A4170, 0, }, // glWindowPos3f
    { 0x00000001411A48F0, 0, }, // glWindowPos3fARB
    { 0x00000001411A4178, 0, }, // glWindowPos3fv
    { 0x00000001411A48F8, 0, }, // glWindowPos3fvARB
    { 0x00000001411A4180, 0, }, // glWindowPos3i
    { 0x00000001411A4900, 0, }, // glWindowPos3iARB
    { 0x00000001411A4188, 0, }, // glWindowPos3iv
    { 0x00000001411A4908, 0, }, // glWindowPos3ivARB
    { 0x00000001411A4190, 0, }, // glWindowPos3s
    { 0x00000001411A4910, 0, }, // glWindowPos3sARB
    { 0x00000001411A4198, 0, }, // glWindowPos3sv
    { 0x00000001411A4918, 0, }, // glWindowPos3svARB
    { 0x00000001411A3E50, 0, }, // wglAllocateMemoryNV
    { 0x00000001411A3EE0, 0, }, // wglBindSwapBarrierNV
    { 0x00000001411A3DE0, 0, }, // wglBindTexImageARB
    { 0x00000001411A3E90, 0, }, // wglBindVideoImageNV
    { 0x00000001411A3D60, 0, }, // wglChoosePixelFormatARB
    { 0x00000001411A3CF0, 0, }, // wglCreateBufferRegionARB
    { 0x00000001411A3E10, 0, }, // wglCreateContextAttribsARB
    { 0x00000001411A3D90, 0, }, // wglCreatePbufferARB
    { 0x00000001411A3D00, 0, }, // wglDeleteBufferRegionARB
    { 0x00000001411A3DC0, 0, }, // wglDestroyPbufferARB
    { 0x00000001411A1A48, 0, }, // wglDXCloseDeviceNV
    { 0x00000001411A1A68, 0, }, // wglDXLockObjectsNV
    { 0x00000001411A1A40, 0, }, // wglDXOpenDeviceNV
    { 0x00000001411A1A58, 0, }, // wglDXRegisterObjectNV
    { 0x00000001411A1A50, 0, }, // wglDXSetResourceShareHandleNV
    { 0x00000001411A1A70, 0, }, // wglDXUnlockObjectsNV
    { 0x00000001411A1A60, 0, }, // wglDXUnregisterObjectNV
    { 0x00000001411A3E60, 0, }, // wglFreeMemoryNV
    { 0x00000001411A1A28, 0, }, // wglGetCurrentContext
    { 0x00000001411A3D80, 0, }, // wglGetCurrentReadDCARB
    { 0x00000001411A3D30, 0, }, // wglGetExtensionsStringARB
    { 0x00000001411A3E20, 0, }, // wglGetExtensionsStringEXT
    { 0x00000001411A3DA0, 0, }, // wglGetPbufferDCARB
    { 0x00000001411A3D50, 0, }, // wglGetPixelFormatAttribfvARB
    { 0x00000001411A3D40, 0, }, // wglGetPixelFormatAttribivARB
    { 0x00000001411A1A20, 0, }, // wglGetProcAddress
    { 0x00000001411A3E40, 0, }, // wglGetSwapIntervalEXT
    { 0x00000001411A3E70, 0, }, // wglGetVideoDeviceNV
    { 0x00000001411A3EC0, 0, }, // wglGetVideoInfoNV
    { 0x00000001411A3ED0, 0, }, // wglJoinSwapGroupNV
    { 0x00000001411A3D70, 0, }, // wglMakeContextCurrentARB
    { 0x00000001411A3F10, 0, }, // wglQueryFrameCountNV
    { 0x00000001411A3F00, 0, }, // wglQueryMaxSwapGroupsNV
    { 0x00000001411A3DD0, 0, }, // wglQueryPbufferARB
    { 0x00000001411A3EF0, 0, }, // wglQuerySwapGroupNV
    { 0x00000001411A3DB0, 0, }, // wglReleasePbufferDCARB
    { 0x00000001411A3DF0, 0, }, // wglReleaseTexImageARB
    { 0x00000001411A3E80, 0, }, // wglReleaseVideoDeviceNV
    { 0x00000001411A3EA0, 0, }, // wglReleaseVideoImageNV
    { 0x00000001411A3F20, 0, }, // wglResetFrameCountNV
    { 0x00000001411A3D20, 0, }, // wglRestoreBufferRegionARB
    { 0x00000001411A3D10, 0, }, // wglSaveBufferRegionARB
    { 0x00000001411A3EB0, 0, }, // wglSendPbufferToVideoNV
    { 0x00000001411A3E00, 0, }, // wglSetPbufferAttribARB
    { 0x00000001411A3E30, 0, }, // wglSwapIntervalEXT
};

gl_func_struct gl_addr_glut[7] = {
    { 0x000000000002A358, 0, }, // glFinish
    { 0x000000000002A438, 0, }, // glViewport
    { 0x000000000002A368, 0, }, // wglDeleteContext
    { 0x000000000002A420, 0, }, // wglGetCurrentContext
    { 0x000000000002A428, 0, }, // wglGetCurrentDC
    { 0x000000000002A448, 0, }, // wglGetProcAddress
    { 0x000000000002A430, 0, }, // wglMakeCurrent
};

bool GL_VERSION_4_3 = false;
bool GL_VERSION_4_4 = false;
bool GL_VERSION_4_5 = false;
bool GL_VERSION_4_6 = false;

GLuint current_program;

extern size_t opengl32_handle;

#ifdef DEBUG
static void print_debug_error_old(const char* c, const char* e) {
    if (e)
        printf("DivaGL Deprecated: Debug: %s Error: %s; Time %li\n", c, e, clock());
    else
        printf("DivaGL Deprecated: Debug: %s; Time %li\n", c, clock());
}
#endif

#ifdef DEBUG
static void print_debug_error(const char* c, const char* e) {
    if (e)
        printf("DivaGL: Debug: %s Error: %s; Time %li\n", c, e, clock());
#ifdef PRINT_DEBUG
    else
        printf("DivaGL: Debug: %s; Time %li\n", c, clock());
#endif
}
#endif

void wrap_addresses() {
#ifdef DEBUG
    print_debug_error("Patching tables", 0);
#endif

    for (int i = 0; i < 78; i++)
        if (wrap_addr_dll[i].func != *(size_t*)wrap_addr_dll[i].address) {
            wrap_addr_dll[i].orig_func = *(size_t*)wrap_addr_dll[i].address;
            inject_uint64_t((void*)wrap_addr_dll[i].address, wrap_addr_dll[i].func);
        }

    for (int i = 0; i < 9; i++)
        gl_addr_dll[i].func = *(size_t*)gl_addr_dll[i].address;

#ifdef DEBUG
    print_debug_error("DLL include table patched", 0);
#endif

    for (int i = 0; i < 27; i++)
        if (wrap_addr_glut[i].func != *(size_t*)(glut_handle + wrap_addr_glut[i].address)) {
            wrap_addr_glut[i].orig_func = *(size_t*)(glut_handle + wrap_addr_glut[i].address);
            inject_uint64_t((void*)(glut_handle + wrap_addr_glut[i].address), wrap_addr_glut[i].func);
        }

    for (int i = 0; i < 7; i++)
        gl_addr_glut[i].func = *(size_t*)(glut_handle + gl_addr_glut[i].address);

#ifdef DEBUG
    print_debug_error("GLUT DLL include table patched", 0);
#endif

    for (int i = 0; i < 50; i++)
        if (wrap_addr[i].func != *(size_t*)wrap_addr[i].address) {
            wrap_addr[i].orig_func = *(size_t*)wrap_addr[i].address;
            inject_uint64_t((void*)wrap_addr[i].address, wrap_addr[i].func);
        }

    for (int i = 0; i < 1057; i++)
        gl_addr[i].func = *(size_t*)gl_addr[i].address;

#ifdef DEBUG
    print_debug_error("wglGetProcAddresses table patched", 0);
#endif

    const char* version = (const char*)glGetStringGLUT(GL_VERSION);
    if (version) {
        int32_t major;
        int32_t minor;
        sscanf_s(version, "%d.%d", &major, &minor);

        GL_VERSION_4_3 = (major == 4 && minor >= 3) || major > 4;
        GL_VERSION_4_4 = (major == 4 && minor >= 4) || major > 4;
        GL_VERSION_4_5 = (major == 4 && minor >= 5) || major > 4;
        GL_VERSION_4_6 = (major == 4 && minor >= 6) || major > 4;
    }
}

HOOK(void, FASTCALL, wglGetProcAddresses, 0x0000000140461B50) {
    originalwglGetProcAddresses();
    wrap_addresses();
}

void wrap_patch() {
    // glShadeModel(GL_SMOOTH);
    WRITE_NOP_11(0x00000001401948CA);

    // glClampColorARB(GL_CLAMP_VERTEX_COLOR_ARB, GL_ZERO);
    // glClampColorARB(GL_CLAMP_FRAGMENT_COLOR_ARB, GL_FIXED_ONLY_ARB);
    // glClampColorARB(GL_CLAMP_READ_COLOR_ARB, GL_FIXED_ONLY_ARB);
    WRITE_JUMP(0x0000000140194932, 0x000000014019495B);

    INSTALL_HOOK(wglGetProcAddresses);
}

void GLAPIENTRY glAlphaFuncDLL(GLenum func, GLfloat ref) {
    if (func == GL_GREATER && ref >= 0.5f)
        uniform->arr[U_ALPHA_TEST] = 1;
    else
        uniform->arr[U_ALPHA_TEST] = 0;
    ((PFNGLALPHAFUNCDLLPROC)wrap_addr_dll[GLALPHAFUNCDLL].orig_func)(func, ref);
}

void GLAPIENTRY glBeginDLL(GLenum mode) {
    ((PFNGLBEGINDLLPROC)wrap_addr_dll[GLBEGINDLL].orig_func)(mode);
}

void GLAPIENTRY glBindTextureDLL(GLenum target, GLuint texture) {
    ((PFNGLBINDTEXTUREDLLPROC)wrap_addr_dll[GLBINDTEXTUREDLL].orig_func)(target, texture);
}

void GLAPIENTRY glClearDLL(GLbitfield mask) {
    ((PFNGLCLEARDLLPROC)wrap_addr_dll[GLCLEARDLL].orig_func)(mask);
}

void GLAPIENTRY glClearColorDLL(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
    ((PFNGLCLEARCOLORDLLPROC)wrap_addr_dll[GLCLEARCOLORDLL].orig_func)(red, green, blue, alpha);
}

void GLAPIENTRY glClearDepthDLL(GLdouble depth) {
    ((PFNGLCLEARDEPTHDLLPROC)wrap_addr_dll[GLCLEARDEPTHDLL].orig_func)(depth);
}

void GLAPIENTRY glClearStencilDLL(GLint s) {
    ((PFNGLCLEARSTENCILDLLPROC)wrap_addr_dll[GLCLEARSTENCILDLL].orig_func)(s);
}

void GLAPIENTRY glColor3fDLL(GLfloat red, GLfloat green, GLfloat blue) {
    ((PFNGLCOLOR3FDLLPROC)wrap_addr_dll[GLCOLOR3FDLL].orig_func)(red, green, blue);
}

void GLAPIENTRY glColor4fDLL(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
    ((PFNGLCOLOR4FDLLPROC)wrap_addr_dll[GLCOLOR4FDLL].orig_func)(red, green, blue, alpha);
}

void GLAPIENTRY glColor4ubDLL(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha) {
    ((PFNGLCOLOR4UBDLLPROC)wrap_addr_dll[GLCOLOR4UBDLL].orig_func)(red, green, blue, alpha);
}

void GLAPIENTRY glColor4ubvDLL(GLubyte const * v) {
    ((PFNGLCOLOR4UBVDLLPROC)wrap_addr_dll[GLCOLOR4UBVDLL].orig_func)(v);
}

void GLAPIENTRY glColorPointerDLL(GLint size, GLenum type, GLsizei stride, const void * pointer) {
    ((PFNGLCOLORPOINTERDLLPROC)wrap_addr_dll[GLCOLORPOINTERDLL].orig_func)(size, type, stride, pointer);
}

void GLAPIENTRY glCopyTexImage2DDLL(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border) {
    ((PFNGLCOPYTEXIMAGE2DDLLPROC)wrap_addr_dll[GLCOPYTEXIMAGE2DDLL].orig_func)(target, level, internalformat, x, y, width, height, border);
}

void GLAPIENTRY glCopyTexSubImage2DDLL(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
    ((PFNGLCOPYTEXSUBIMAGE2DDLLPROC)wrap_addr_dll[GLCOPYTEXSUBIMAGE2DDLL].orig_func)(target, level, xoffset, yoffset, x, y, width, height);
}

void GLAPIENTRY glCullFaceDLL(GLenum mode) {
    ((PFNGLCULLFACEDLLPROC)wrap_addr_dll[GLCULLFACEDLL].orig_func)(mode);
}

void GLAPIENTRY glDepthFuncDLL(GLenum func) {
    ((PFNGLDEPTHFUNCDLLPROC)wrap_addr_dll[GLDEPTHFUNCDLL].orig_func)(func);
}

void GLAPIENTRY glDisableDLL(GLenum cap) {
    ((PFNGLDISABLEDLLPROC)wrap_addr_dll[GLDISABLEDLL].orig_func)(cap);
}

void GLAPIENTRY glDisableClientStateDLL(GLenum array) {
    ((PFNGLDISABLECLIENTSTATEDLLPROC)wrap_addr_dll[GLDISABLECLIENTSTATEDLL].orig_func)(array);
}

void GLAPIENTRY glDrawArraysDLL(GLenum mode, GLint first, GLsizei count) {
    ((PFNGLDRAWARRAYSDLLPROC)wrap_addr_dll[GLDRAWARRAYSDLL].orig_func)(mode, first, count);
}

void GLAPIENTRY glDrawBufferDLL(GLenum buf) {
    ((PFNGLDRAWBUFFERDLLPROC)wrap_addr_dll[GLDRAWBUFFERDLL].orig_func)(buf);
}

void GLAPIENTRY glDrawElementsDLL(GLenum mode, GLsizei count, GLenum type, const void * indices) {
    ((PFNGLDRAWELEMENTSDLLPROC)wrap_addr_dll[GLDRAWELEMENTSDLL].orig_func)(mode, count, type, indices);
}

void GLAPIENTRY glEnableDLL(GLenum cap) {
    ((PFNGLENABLEDLLPROC)wrap_addr_dll[GLENABLEDLL].orig_func)(cap);
}

void GLAPIENTRY glEnableClientStateDLL(GLenum array) {
    ((PFNGLENABLECLIENTSTATEDLLPROC)wrap_addr_dll[GLENABLECLIENTSTATEDLL].orig_func)(array);
}

void GLAPIENTRY glEndDLL() {
    ((PFNGLENDDLLPROC)wrap_addr_dll[GLENDDLL].orig_func)();
}

void GLAPIENTRY glFogfDLL(GLenum pname, GLfloat param) {
    ((PFNGLFOGFDLLPROC)wrap_addr_dll[GLFOGFDLL].orig_func)(pname, param);
}

void GLAPIENTRY glFogfvDLL(GLenum pname, GLfloat const * params) {
    ((PFNGLFOGFVDLLPROC)wrap_addr_dll[GLFOGFVDLL].orig_func)(pname, params);
}

void GLAPIENTRY glFogiDLL(GLenum pname, GLint param) {
    ((PFNGLFOGIDLLPROC)wrap_addr_dll[GLFOGIDLL].orig_func)(pname, param);
}

void GLAPIENTRY glFrontFaceDLL(GLenum mode) {
    ((PFNGLFRONTFACEDLLPROC)wrap_addr_dll[GLFRONTFACEDLL].orig_func)(mode);
}

void GLAPIENTRY glFrustumDLL(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar) {
    mat4 mat;
    mat4_frustrum(left, right, bottom, top, zNear, zFar, &mat);
    mat4_transpose(&mat, &mat);
    glMultTransposeMatrixf((const GLfloat*)&mat);
    //((PFNGLFRUSTUMDLLPROC)wrap_addr_dll[GLFRUSTUMDLL].orig_func)(left, right, bottom, top, zNear, zFar);
}

void GLAPIENTRY glGetBooleanvDLL(GLenum pname, GLboolean * data) {
    switch (pname) {
    case GL_VERTEX_PROGRAM_ARB:
    case GL_FRAGMENT_PROGRAM_ARB:
        if (data)
            *data = GL_TRUE;
        return;
    }
    ((PFNGLGETBOOLEANVDLLPROC)wrap_addr_dll[GLGETBOOLEANVDLL].orig_func)(pname, data);
}

GLenum GLAPIENTRY glGetErrorDLL() {
    return ((PFNGLGETERRORDLLPROC)wrap_addr_dll[GLGETERRORDLL].orig_func)();
}

void GLAPIENTRY glGetFloatvDLL(GLenum pname, GLfloat * data) {
    ((PFNGLGETFLOATVDLLPROC)wrap_addr_dll[GLGETFLOATVDLL].orig_func)(pname, data);
}

void GLAPIENTRY glGetIntegervDLL(GLenum pname, GLint * data) {
    ((PFNGLGETINTEGERVDLLPROC)wrap_addr_dll[GLGETINTEGERVDLL].orig_func)(pname, data);
}

void GLAPIENTRY glGetMaterialfvDLL(GLenum face, GLenum pname, GLfloat * params) {
    ((PFNGLGETMATERIALFVDLLPROC)wrap_addr_dll[GLGETMATERIALFVDLL].orig_func)(face, pname, params);
}

void GLAPIENTRY glGetTexImageDLL(GLenum target, GLint level, GLenum format, GLenum type, void * pixels) {
    ((PFNGLGETTEXIMAGEDLLPROC)wrap_addr_dll[GLGETTEXIMAGEDLL].orig_func)(target, level, format, type, pixels);
}

void GLAPIENTRY glGetTexLevelParameterivDLL(GLenum target, GLint level, GLenum pname, GLint * params) {
    ((PFNGLGETTEXLEVELPARAMETERIVDLLPROC)wrap_addr_dll[GLGETTEXLEVELPARAMETERIVDLL].orig_func)(target, level, pname, params);
}

void GLAPIENTRY glGetTexParameterivDLL(GLenum target, GLenum pname, GLint * params) {
    ((PFNGLGETTEXPARAMETERIVDLLPROC)wrap_addr_dll[GLGETTEXPARAMETERIVDLL].orig_func)(target, pname, params);
}

GLboolean GLAPIENTRY glIsEnabledDLL(GLenum cap) {
    return ((PFNGLISENABLEDDLLPROC)wrap_addr_dll[GLISENABLEDDLL].orig_func)(cap);
}

void GLAPIENTRY glLightfvDLL(GLenum light, GLenum pname, GLfloat const * params) {
    ((PFNGLLIGHTFVDLLPROC)wrap_addr_dll[GLLIGHTFVDLL].orig_func)(light, pname, params);
}

void GLAPIENTRY glLightModelfvDLL(GLenum pname, GLfloat const * params) {
    ((PFNGLLIGHTMODELFVDLLPROC)wrap_addr_dll[GLLIGHTMODELFVDLL].orig_func)(pname, params);
}

void GLAPIENTRY glLineWidthDLL(GLfloat width) {
    ((PFNGLLINEWIDTHDLLPROC)wrap_addr_dll[GLLINEWIDTHDLL].orig_func)(width);
}

void GLAPIENTRY glLoadIdentityDLL() {
    ((PFNGLLOADIDENTITYDLLPROC)wrap_addr_dll[GLLOADIDENTITYDLL].orig_func)();
}

void GLAPIENTRY glLoadMatrixfDLL(GLfloat const * m) {
    ((PFNGLLOADMATRIXFDLLPROC)wrap_addr_dll[GLLOADMATRIXFDLL].orig_func)(m);
}

void GLAPIENTRY glMaterialfDLL(GLenum face, GLenum pname, GLfloat param) {
    ((PFNGLMATERIALFDLLPROC)wrap_addr_dll[GLMATERIALFDLL].orig_func)(face, pname, param);
}

void GLAPIENTRY glMaterialfvDLL(GLenum face, GLenum pname, GLfloat const * params) {
    ((PFNGLMATERIALFVDLLPROC)wrap_addr_dll[GLMATERIALFVDLL].orig_func)(face, pname, params);
}

void GLAPIENTRY glMatrixModeDLL(GLenum mode) {
    ((PFNGLMATRIXMODEDLLPROC)wrap_addr_dll[GLMATRIXMODEDLL].orig_func)(mode);
}

void GLAPIENTRY glMultMatrixdDLL(GLdouble const * m) {
    ((PFNGLMULTMATRIXDDLLPROC)wrap_addr_dll[GLMULTMATRIXDDLL].orig_func)(m);
}

void GLAPIENTRY glNormal3fDLL(GLfloat nx, GLfloat ny, GLfloat nz) {
    ((PFNGLNORMAL3FDLLPROC)wrap_addr_dll[GLNORMAL3FDLL].orig_func)(nx, ny, nz);
}

void GLAPIENTRY glNormalPointerDLL(GLenum type, GLsizei stride, const void * pointer) {
    ((PFNGLNORMALPOINTERDLLPROC)wrap_addr_dll[GLNORMALPOINTERDLL].orig_func)(type, stride, pointer);
}

void GLAPIENTRY glOrthoDLL(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar) {
    mat4 mat;
    mat4_ortho(left, right, bottom, top, zNear, zFar, &mat);
    mat4_transpose(&mat, &mat);
    glMultTransposeMatrixf((const GLfloat*)&mat);
    //((PFNGLORTHODLLPROC)wrap_addr_dll[GLORTHODLL].orig_func)(left, right, bottom, top, zNear, zFar);
}

void GLAPIENTRY glPixelStoreiDLL(GLenum pname, GLint param) {
    ((PFNGLPIXELSTOREIDLLPROC)wrap_addr_dll[GLPIXELSTOREIDLL].orig_func)(pname, param);
}

void GLAPIENTRY glPointSizeDLL(GLfloat size) {
    ((PFNGLPOINTSIZEDLLPROC)wrap_addr_dll[GLPOINTSIZEDLL].orig_func)(size);
}

void GLAPIENTRY glPolygonModeDLL(GLenum face, GLenum mode) {
    ((PFNGLPOLYGONMODEDLLPROC)wrap_addr_dll[GLPOLYGONMODEDLL].orig_func)(face, mode);
}

void GLAPIENTRY glPopAttribDLL() {
    ((PFNGLPOPATTRIBDLLPROC)wrap_addr_dll[GLPOPATTRIBDLL].orig_func)();
}

void GLAPIENTRY glPopClientAttribDLL() {
    ((PFNGLPOPCLIENTATTRIBDLLPROC)wrap_addr_dll[GLPOPCLIENTATTRIBDLL].orig_func)();
}

void GLAPIENTRY glPopMatrixDLL() {
    ((PFNGLPOPMATRIXDLLPROC)wrap_addr_dll[GLPOPMATRIXDLL].orig_func)();
}

void GLAPIENTRY glPushAttribDLL(GLbitfield mask) {
    ((PFNGLPUSHATTRIBDLLPROC)wrap_addr_dll[GLPUSHATTRIBDLL].orig_func)(mask);
}

void GLAPIENTRY glPushClientAttribDLL(GLbitfield mask) {
    ((PFNGLPUSHCLIENTATTRIBDLLPROC)wrap_addr_dll[GLPUSHCLIENTATTRIBDLL].orig_func)(mask);
}

void GLAPIENTRY glPushMatrixDLL() {
    ((PFNGLPUSHMATRIXDLLPROC)wrap_addr_dll[GLPUSHMATRIXDLL].orig_func)();
}

void GLAPIENTRY glReadBufferDLL(GLenum src) {
    ((PFNGLREADBUFFERDLLPROC)wrap_addr_dll[GLREADBUFFERDLL].orig_func)(src);
}

void GLAPIENTRY glReadPixelsDLL(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void * pixels) {
    ((PFNGLREADPIXELSDLLPROC)wrap_addr_dll[GLREADPIXELSDLL].orig_func)(x, y, width, height, format, type, pixels);
}

void GLAPIENTRY glRotatefDLL(GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {
    float_t c = cosf(angle * DEG_TO_RAD_FLOAT);
    float_t s = sinf(angle * DEG_TO_RAD_FLOAT);
    float_t t = 1.0f - c;

    mat4 mat;
    mat.row0 = { t * x * x + c, t * x * y + z * s, t * x * z - y * s, 0.0f };
    mat.row1 = { t * y * x - z * s, t * y * y + c, t * y * z + x * s, 0.0f };
    mat.row2 = { t * z * x + y * s, t * z * y - x * s, t * z * z + c, 0.0f };
    mat.row3 = { 0.0f, 0.0f, 0.0f, 1.0f };
    mat4_transpose(&mat, &mat);
    glMultTransposeMatrixf((const GLfloat*)&mat);
    //((PFNGLROTATEFDLLPROC)wrap_addr_dll[GLROTATEFDLL].orig_func)(angle, x, y, z);
}

void GLAPIENTRY glScalefDLL(GLfloat x, GLfloat y, GLfloat z) {
    mat4 mat;
    mat4_scale(x, y, z, &mat);
    mat4_transpose(&mat, &mat);
    glMultTransposeMatrixf((const GLfloat*)&mat);
    //((PFNGLSCALEFDLLPROC)wrap_addr_dll[GLSCALEFDLL].orig_func)(x, y, z);
}

void GLAPIENTRY glShadeModelDLL(GLenum mode) {
    ((PFNGLSHADEMODELDLLPROC)wrap_addr_dll[GLSHADEMODELDLL].orig_func)(mode);
}

void GLAPIENTRY glTexCoord2fDLL(GLfloat s, GLfloat t) {
    ((PFNGLTEXCOORD2FDLLPROC)wrap_addr_dll[GLTEXCOORD2FDLL].orig_func)(s, t);
}

void GLAPIENTRY glTexCoordPointerDLL(GLint size, GLenum type, GLsizei stride, const void * pointer) {
    ((PFNGLTEXCOORDPOINTERDLLPROC)wrap_addr_dll[GLTEXCOORDPOINTERDLL].orig_func)(size, type, stride, pointer);
}

void GLAPIENTRY glTexEnvfDLL(GLenum target, GLenum pname, GLfloat param) {
    ((PFNGLTEXENVFDLLPROC)wrap_addr_dll[GLTEXENVFDLL].orig_func)(target, pname, param);
}

void GLAPIENTRY glTexEnviDLL(GLenum target, GLenum pname, GLint param) {
    ((PFNGLTEXENVIDLLPROC)wrap_addr_dll[GLTEXENVIDLL].orig_func)(target, pname, param);
}

void GLAPIENTRY glTexImage1DDLL(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void * pixels) {
    GLint _internalformat;
    GLint _format;
    switch (internalformat) {
    case GL_LUMINANCE_ALPHA16F_ARB:
        _internalformat = GL_RG16F;
        _format = GL_RG;
        break;
    default:
        _internalformat = internalformat;
        _format = format;
        break;
    }

    ((PFNGLTEXIMAGE1DDLLPROC)wrap_addr_dll[GLTEXIMAGE1DDLL].orig_func)(target, level, _internalformat, width, border, _format, type, pixels);

    switch (internalformat) {
    case GL_LUMINANCE_ALPHA16F_ARB:
        glTexParameteriDLL(target, GL_TEXTURE_SWIZZLE_R, GL_RED);
        glTexParameteriDLL(target, GL_TEXTURE_SWIZZLE_G, GL_RED);
        glTexParameteriDLL(target, GL_TEXTURE_SWIZZLE_B, GL_RED);
        glTexParameteriDLL(target, GL_TEXTURE_SWIZZLE_A, GL_GREEN);
        break;
    }
}

void GLAPIENTRY glTexImage2DDLL(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void * pixels) {
    GLint _internalformat;
    switch (internalformat) {
    case GL_ALPHA8:
        _internalformat = GL_R8;
        break;
    case GL_LUMINANCE8:
        _internalformat = GL_R8;
        break;
    case GL_LUMINANCE8_ALPHA8:
        _internalformat = GL_RG8;
        break;
    default:
        _internalformat = internalformat;
        break;
    }

    ((PFNGLTEXIMAGE2DDLLPROC)wrap_addr_dll[GLTEXIMAGE2DDLL].orig_func)(target, level, _internalformat, width, height, border, format, type, pixels);

    switch (internalformat) {
    case GL_ALPHA8:
        glTexParameteriDLL(target, GL_TEXTURE_SWIZZLE_R, GL_ZERO);
        glTexParameteriDLL(target, GL_TEXTURE_SWIZZLE_G, GL_ZERO);
        glTexParameteriDLL(target, GL_TEXTURE_SWIZZLE_B, GL_ZERO);
        glTexParameteriDLL(target, GL_TEXTURE_SWIZZLE_A, GL_RED);
        break;
    case GL_LUMINANCE8:
        glTexParameteriDLL(target, GL_TEXTURE_SWIZZLE_R, GL_RED);
        glTexParameteriDLL(target, GL_TEXTURE_SWIZZLE_G, GL_RED);
        glTexParameteriDLL(target, GL_TEXTURE_SWIZZLE_B, GL_RED);
        glTexParameteriDLL(target, GL_TEXTURE_SWIZZLE_A, GL_ONE);
        break;
    case GL_LUMINANCE8_ALPHA8:
        glTexParameteriDLL(target, GL_TEXTURE_SWIZZLE_R, GL_RED);
        glTexParameteriDLL(target, GL_TEXTURE_SWIZZLE_G, GL_RED);
        glTexParameteriDLL(target, GL_TEXTURE_SWIZZLE_B, GL_RED);
        glTexParameteriDLL(target, GL_TEXTURE_SWIZZLE_A, GL_GREEN);
        break;
    }
}

void GLAPIENTRY glTexParameterfDLL(GLenum target, GLenum pname, GLfloat param) {
    ((PFNGLTEXPARAMETERFDLLPROC)wrap_addr_dll[GLTEXPARAMETERFDLL].orig_func)(target, pname, param);
}

void GLAPIENTRY glTexParameterfvDLL(GLenum target, GLenum pname, GLfloat const * params) {
    ((PFNGLTEXPARAMETERFVDLLPROC)wrap_addr_dll[GLTEXPARAMETERFVDLL].orig_func)(target, pname, params);
}

void GLAPIENTRY glTexParameteriDLL(GLenum target, GLenum pname, GLint param) {
    ((PFNGLTEXPARAMETERIDLLPROC)wrap_addr_dll[GLTEXPARAMETERIDLL].orig_func)(target, pname, param);
}

void GLAPIENTRY glTexSubImage2DDLL(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * pixels) {
    ((PFNGLTEXSUBIMAGE2DDLLPROC)wrap_addr_dll[GLTEXSUBIMAGE2DDLL].orig_func)(target, level, xoffset, yoffset, width, height, format, type, pixels);
}

void GLAPIENTRY glTranslatefDLL(GLfloat x, GLfloat y, GLfloat z) {
    mat4 mat;
    mat4_translate(x, y, z, &mat);
    mat4_transpose(&mat, &mat);
    glMultTransposeMatrixf((const GLfloat*)&mat);
    //((PFNGLTRANSLATEFDLLPROC)wrap_addr_dll[GLTRANSLATEFDLL].orig_func)(x, y, z);
}

void GLAPIENTRY glVertex3fDLL(GLfloat x, GLfloat y, GLfloat z) {
    ((PFNGLVERTEX3FDLLPROC)wrap_addr_dll[GLVERTEX3FDLL].orig_func)(x, y, z);
}

void GLAPIENTRY glVertex3fvDLL(GLfloat const * v) {
    ((PFNGLVERTEX3FVDLLPROC)wrap_addr_dll[GLVERTEX3FVDLL].orig_func)(v);
}

void GLAPIENTRY glVertexPointerDLL(GLint size, GLenum type, GLsizei stride, const void * pointer) {
    ((PFNGLVERTEXPOINTERDLLPROC)wrap_addr_dll[GLVERTEXPOINTERDLL].orig_func)(size, type, stride, pointer);
}

void GLAPIENTRY glActiveTexture(GLenum texture) {
    ((PFNGLACTIVETEXTUREPROC)wrap_addr[GLACTIVETEXTURE].orig_func)(texture);
}

void GLAPIENTRY glBindBufferARB(GLenum target, GLuint buffer) {
    ((PFNGLBINDBUFFERARBPROC)wrap_addr[GLBINDBUFFERARB].orig_func)(target, buffer);
}

void GLAPIENTRY glBindBufferBaseNV(GLenum target, GLuint index, GLuint buffer) {
    ((PFNGLBINDBUFFERBASENVPROC)wrap_addr[GLBINDBUFFERBASENV].orig_func)(target, index, buffer);
}

void GLAPIENTRY glBindFramebufferEXT(GLenum target, GLuint framebuffer) {
    ((PFNGLBINDFRAMEBUFFEREXTPROC)wrap_addr[GLBINDFRAMEBUFFEREXT].orig_func)(target, framebuffer);
}

void GLAPIENTRY glBindMultiTextureEXT(GLenum texunit, GLenum target, GLuint texture) {
    ((PFNGLBINDMULTITEXTUREEXTPROC)wrap_addr[GLBINDMULTITEXTUREEXT].orig_func)(texunit, target, texture);
}

void GLAPIENTRY glBindRenderbufferEXT(GLenum target, GLuint renderbuffer) {
    ((PFNGLBINDRENDERBUFFEREXTPROC)wrap_addr[GLBINDRENDERBUFFEREXT].orig_func)(target, renderbuffer);
}

void GLAPIENTRY glBufferDataARB(GLenum target, GLsizeiptrARB size, const void * data, GLenum usage) {
    ((PFNGLBUFFERDATAARBPROC)wrap_addr[GLBUFFERDATAARB].orig_func)(target, size, data, usage);
}

void GLAPIENTRY glClampColorARB(GLenum target, GLenum clamp) {
    ((PFNGLCLAMPCOLORARBPROC)wrap_addr[GLCLAMPCOLORARB].orig_func)(target, clamp);
}

void GLAPIENTRY glClientActiveTexture(GLenum texture) {
    ((PFNGLCLIENTACTIVETEXTUREPROC)wrap_addr[GLCLIENTACTIVETEXTURE].orig_func)(texture);
}

void GLAPIENTRY glCopyMultiTexSubImage2DEXT(GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
    ((PFNGLCOPYMULTITEXSUBIMAGE2DEXTPROC)wrap_addr[GLCOPYMULTITEXSUBIMAGE2DEXT].orig_func)(texunit, target, level, xoffset, yoffset, x, y, width, height);
}

void GLAPIENTRY glDeleteBuffersARB(GLsizei n, GLuint const * buffers) {
    ((PFNGLDELETEBUFFERSARBPROC)wrap_addr[GLDELETEBUFFERSARB].orig_func)(n, buffers);
}

void GLAPIENTRY glDeleteFramebuffersEXT(GLsizei n, GLuint const * framebuffers) {
    ((PFNGLDELETEFRAMEBUFFERSEXTPROC)wrap_addr[GLDELETEFRAMEBUFFERSEXT].orig_func)(n, framebuffers);
}

void GLAPIENTRY glDeleteRenderbuffersEXT(GLsizei n, GLuint const * renderbuffers) {
    ((PFNGLDELETERENDERBUFFERSEXTPROC)wrap_addr[GLDELETERENDERBUFFERSEXT].orig_func)(n, renderbuffers);
}

void GLAPIENTRY glDeleteTextures(GLsizei n, GLuint const * textures) {
    glDeleteTexturesDLL(n, textures);
    //((PFNGLDELETETEXTURESPROC)wrap_addr[GLDELETETEXTURES].orig_func)(n, textures);
}

void GLAPIENTRY glDisableVertexAttribArrayARB(GLuint index) {
    ((PFNGLDISABLEVERTEXATTRIBARRAYARBPROC)wrap_addr[GLDISABLEVERTEXATTRIBARRAYARB].orig_func)(index);
}

void GLAPIENTRY glDrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void * indices) {
    ((PFNGLDRAWRANGEELEMENTSPROC)wrap_addr[GLDRAWRANGEELEMENTS].orig_func)(mode, start, end, count, type, indices);
}

void GLAPIENTRY glEnableVertexAttribArray(GLuint index) {
    ((PFNGLENABLEVERTEXATTRIBARRAYPROC)wrap_addr[GLENABLEVERTEXATTRIBARRAY].orig_func)(index);
}

void GLAPIENTRY glEnableVertexAttribArrayARB(GLuint index) {
    ((PFNGLENABLEVERTEXATTRIBARRAYARBPROC)wrap_addr[GLENABLEVERTEXATTRIBARRAYARB].orig_func)(index);
}

void GLAPIENTRY glFramebufferRenderbufferEXT(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) {
    ((PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)wrap_addr[GLFRAMEBUFFERRENDERBUFFEREXT].orig_func)(target, attachment, renderbuffertarget, renderbuffer);
}

void GLAPIENTRY glFramebufferTexture2DEXT(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {
    ((PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wrap_addr[GLFRAMEBUFFERTEXTURE2DEXT].orig_func)(target, attachment, textarget, texture, level);
}

void GLAPIENTRY glGenFramebuffersEXT(GLsizei n, GLuint * framebuffers) {
    ((PFNGLGENFRAMEBUFFERSEXTPROC)wrap_addr[GLGENFRAMEBUFFERSEXT].orig_func)(n, framebuffers);
}

void GLAPIENTRY glGenRenderbuffersEXT(GLsizei n, GLuint * renderbuffers) {
    ((PFNGLGENRENDERBUFFERSEXTPROC)wrap_addr[GLGENRENDERBUFFERSEXT].orig_func)(n, renderbuffers);
}

void GLAPIENTRY glGenTextures(GLsizei n, GLuint * textures) {
    glGenTexturesDLL(n, textures);
    //((PFNGLGENTEXTURESPROC)wrap_addr[GLGENTEXTURES].orig_func)(n, textures);
}

void GLAPIENTRY glGetProgramEnvParameterfvARB(GLenum target, GLuint index, GLfloat * params) {
    ((PFNGLGETPROGRAMENVPARAMETERFVARBPROC)wrap_addr[GLGETPROGRAMENVPARAMETERFVARB].orig_func)(target, index, params);
}

void GLAPIENTRY glGetProgramivARB(GLenum target, GLenum pname, GLint * params) {
    switch (pname) {
    case GL_VERTEX_PROGRAM_ARB:
    case GL_FRAGMENT_PROGRAM_ARB:
        if (params)
            *params = 0;
        return;
    }
    ((PFNGLGETPROGRAMIVARBPROC)wrap_addr[GLGETPROGRAMIVARB].orig_func)(target, pname, params);
}

void GLAPIENTRY glGetTextureImageEXT(GLuint texture, GLenum target, GLint level, GLenum format, GLenum type, void * pixels) {
    ((PFNGLGETTEXTUREIMAGEEXTPROC)wrap_addr[GLGETTEXTUREIMAGEEXT].orig_func)(texture, target, level, format, type, pixels);
}

GLboolean GLAPIENTRY glIsBufferARB(GLuint buffer) {
    return ((PFNGLISBUFFERARBPROC)wrap_addr[GLISBUFFERARB].orig_func)(buffer);
}

void GLAPIENTRY glLoadTransposeMatrixf(GLfloat const * m) {
    ((PFNGLLOADTRANSPOSEMATRIXFPROC)wrap_addr[GLLOADTRANSPOSEMATRIXF].orig_func)(m);
}

void * GLAPIENTRY glMapNamedBufferEXT(GLuint buffer, GLenum access) {
    return ((PFNGLMAPNAMEDBUFFEREXTPROC)wrap_addr[GLMAPNAMEDBUFFEREXT].orig_func)(buffer, access);
}

void GLAPIENTRY glMultiTexCoord2f(GLenum target, GLfloat s, GLfloat t) {
    ((PFNGLMULTITEXCOORD2FPROC)wrap_addr[GLMULTITEXCOORD2F].orig_func)(target, s, t);
}

void GLAPIENTRY glMultiTexCoord2fv(GLenum target, GLfloat const * v) {
    ((PFNGLMULTITEXCOORD2FVPROC)wrap_addr[GLMULTITEXCOORD2FV].orig_func)(target, v);
}

void GLAPIENTRY glMultTransposeMatrixf(GLfloat const * m) {
    ((PFNGLMULTTRANSPOSEMATRIXFPROC)wrap_addr[GLMULTTRANSPOSEMATRIXF].orig_func)(m);
}

void GLAPIENTRY glNamedBufferDataEXT(GLuint buffer, GLsizeiptr size, const void * data, GLenum usage) {
    ((PFNGLNAMEDBUFFERDATAEXTPROC)wrap_addr[GLNAMEDBUFFERDATAEXT].orig_func)(buffer, size, data, usage);
}

void GLAPIENTRY glNamedBufferSubDataEXT(GLuint buffer, GLintptr offset, GLsizeiptr size, const void * data) {
    ((PFNGLNAMEDBUFFERSUBDATAEXTPROC)wrap_addr[GLNAMEDBUFFERSUBDATAEXT].orig_func)(buffer, offset, size, data);
}

void GLAPIENTRY glPointParameterf(GLenum pname, GLfloat param) {
    ((PFNGLPOINTPARAMETERFPROC)wrap_addr[GLPOINTPARAMETERF].orig_func)(pname, param);
}

void GLAPIENTRY glPointParameterfv(GLenum pname, GLfloat const * params) {
    ((PFNGLPOINTPARAMETERFVPROC)wrap_addr[GLPOINTPARAMETERFV].orig_func)(pname, params);
}

void GLAPIENTRY glPointParameteri(GLenum pname, GLint param) {
    ((PFNGLPOINTPARAMETERIPROC)wrap_addr[GLPOINTPARAMETERI].orig_func)(pname, param);
}

void GLAPIENTRY glPrimitiveRestartIndexNV(GLuint index) {
    glPrimitiveRestartIndex(index);
    //((PFNGLPRIMITIVERESTARTINDEXNVPROC)wrap_addr[GLPRIMITIVERESTARTINDEXNV].orig_func)(index);
}

void GLAPIENTRY glProgramBufferParametersfvNV(GLenum target, GLuint bindingIndex, GLuint wordIndex, GLsizei count, GLfloat const * params) {
    ((PFNGLPROGRAMBUFFERPARAMETERSFVNVPROC)wrap_addr[GLPROGRAMBUFFERPARAMETERSFVNV].orig_func)(target, bindingIndex, wordIndex, count, params);
}

void GLAPIENTRY glProgramEnvParameter4fARB(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
    ((PFNGLPROGRAMENVPARAMETER4FARBPROC)wrap_addr[GLPROGRAMENVPARAMETER4FARB].orig_func)(target, index, x, y, z, w);
}

void GLAPIENTRY glProgramEnvParameter4fvARB(GLenum target, GLuint index, GLfloat const * params) {
    ((PFNGLPROGRAMENVPARAMETER4FVARBPROC)wrap_addr[GLPROGRAMENVPARAMETER4FVARB].orig_func)(target, index, params);
}

void GLAPIENTRY glProgramEnvParameters4fvEXT(GLenum target, GLuint index, GLsizei count, GLfloat const * params) {
    ((PFNGLPROGRAMENVPARAMETERS4FVEXTPROC)wrap_addr[GLPROGRAMENVPARAMETERS4FVEXT].orig_func)(target, index, count, params);
}

void GLAPIENTRY glProgramLocalParameter4fARB(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
    ((PFNGLPROGRAMLOCALPARAMETER4FARBPROC)wrap_addr[GLPROGRAMLOCALPARAMETER4FARB].orig_func)(target, index, x, y, z, w);
}

void GLAPIENTRY glProgramLocalParameter4fvARB(GLenum target, GLuint index, GLfloat const * params) {
    ((PFNGLPROGRAMLOCALPARAMETER4FVARBPROC)wrap_addr[GLPROGRAMLOCALPARAMETER4FVARB].orig_func)(target, index, params);
}

void GLAPIENTRY glProgramLocalParameters4fvEXT(GLenum target, GLuint index, GLsizei count, GLfloat const * params) {
    ((PFNGLPROGRAMLOCALPARAMETERS4FVEXTPROC)wrap_addr[GLPROGRAMLOCALPARAMETERS4FVEXT].orig_func)(target, index, count, params);
}

void GLAPIENTRY glRenderbufferStorageEXT(GLenum target, GLenum internalformat, GLsizei width, GLsizei height) {
    ((PFNGLRENDERBUFFERSTORAGEEXTPROC)wrap_addr[GLRENDERBUFFERSTORAGEEXT].orig_func)(target, internalformat, width, height);
}

void GLAPIENTRY glRenderbufferStorageMultisampleCoverageNV(GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height) {
    ((PFNGLRENDERBUFFERSTORAGEMULTISAMPLECOVERAGENVPROC)wrap_addr[GLRENDERBUFFERSTORAGEMULTISAMPLECOVERAGENV].orig_func)(target, coverageSamples, colorSamples, internalformat, width, height);
}

GLboolean GLAPIENTRY glUnmapNamedBufferEXT(GLuint buffer) {
    return ((PFNGLUNMAPNAMEDBUFFEREXTPROC)wrap_addr[GLUNMAPNAMEDBUFFEREXT].orig_func)(buffer);
}

void GLAPIENTRY glUseProgram(GLuint program) {
    current_program = program;
    ((PFNGLUSEPROGRAMPROC)wrap_addr[GLUSEPROGRAM].orig_func)(program);
}

void GLAPIENTRY glVertexAttribPointerARB(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer) {
    ((PFNGLVERTEXATTRIBPOINTERARBPROC)wrap_addr[GLVERTEXATTRIBPOINTERARB].orig_func)(index, size, type, normalized, stride, pointer);
}

void GLAPIENTRY glBeginGLUT(GLenum mode) {
    ((PFNGLBEGINGLUTPROC)wrap_addr_glut[GLBEGINGLUT].orig_func)(mode);
}

void GLAPIENTRY glBitmapGLUT(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, GLubyte const * bitmap) {
    ((PFNGLBITMAPGLUTPROC)wrap_addr_glut[GLBITMAPGLUT].orig_func)(width, height, xorig, yorig, xmove, ymove, bitmap);
}

void GLAPIENTRY glDrawBufferGLUT(GLenum buf) {
    ((PFNGLDRAWBUFFERGLUTPROC)wrap_addr_glut[GLDRAWBUFFERGLUT].orig_func)(buf);
}

void GLAPIENTRY glEnableGLUT(GLenum cap) {
    ((PFNGLENABLEGLUTPROC)wrap_addr_glut[GLENABLEGLUT].orig_func)(cap);
}

void GLAPIENTRY glEndGLUT() {
    ((PFNGLENDGLUTPROC)wrap_addr_glut[GLENDGLUT].orig_func)();
}

void GLAPIENTRY glEvalMesh2GLUT(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2) {
    ((PFNGLEVALMESH2GLUTPROC)wrap_addr_glut[GLEVALMESH2GLUT].orig_func)(mode, i1, i2, j1, j2);
}

GLenum GLAPIENTRY glGetErrorGLUT() {
    return ((PFNGLGETERRORGLUTPROC)wrap_addr_glut[GLGETERRORGLUT].orig_func)();
}

void GLAPIENTRY glGetIntegervGLUT(GLenum pname, GLint * data) {
    ((PFNGLGETINTEGERVGLUTPROC)wrap_addr_glut[GLGETINTEGERVGLUT].orig_func)(pname, data);
}

GLubyte const *  GLAPIENTRY glGetStringGLUT(GLenum name) {
    return ((PFNGLGETSTRINGGLUTPROC)wrap_addr_glut[GLGETSTRINGGLUT].orig_func)(name);
}

void GLAPIENTRY glMap2fGLUT(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, GLfloat const * points) {
    ((PFNGLMAP2FGLUTPROC)wrap_addr_glut[GLMAP2FGLUT].orig_func)(target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);
}

void GLAPIENTRY glMapGrid2fGLUT(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2) {
    ((PFNGLMAPGRID2FGLUTPROC)wrap_addr_glut[GLMAPGRID2FGLUT].orig_func)(un, u1, u2, vn, v1, v2);
}

void GLAPIENTRY glNormal3fGLUT(GLfloat nx, GLfloat ny, GLfloat nz) {
    ((PFNGLNORMAL3FGLUTPROC)wrap_addr_glut[GLNORMAL3FGLUT].orig_func)(nx, ny, nz);
}

void GLAPIENTRY glNormal3fvGLUT(GLfloat const * v) {
    ((PFNGLNORMAL3FVGLUTPROC)wrap_addr_glut[GLNORMAL3FVGLUT].orig_func)(v);
}

void GLAPIENTRY glPixelStoreiGLUT(GLenum pname, GLint param) {
    ((PFNGLPIXELSTOREIGLUTPROC)wrap_addr_glut[GLPIXELSTOREIGLUT].orig_func)(pname, param);
}

void GLAPIENTRY glPolygonModeGLUT(GLenum face, GLenum mode) {
    ((PFNGLPOLYGONMODEGLUTPROC)wrap_addr_glut[GLPOLYGONMODEGLUT].orig_func)(face, mode);
}

void GLAPIENTRY glPopAttribGLUT() {
    ((PFNGLPOPATTRIBGLUTPROC)wrap_addr_glut[GLPOPATTRIBGLUT].orig_func)();
}

void GLAPIENTRY glPopMatrixGLUT() {
    ((PFNGLPOPMATRIXGLUTPROC)wrap_addr_glut[GLPOPMATRIXGLUT].orig_func)();
}

void GLAPIENTRY glPushAttribGLUT(GLbitfield mask) {
    ((PFNGLPUSHATTRIBGLUTPROC)wrap_addr_glut[GLPUSHATTRIBGLUT].orig_func)(mask);
}

void GLAPIENTRY glPushMatrixGLUT() {
    ((PFNGLPUSHMATRIXGLUTPROC)wrap_addr_glut[GLPUSHMATRIXGLUT].orig_func)();
}

void GLAPIENTRY glReadBufferGLUT(GLenum src) {
    ((PFNGLREADBUFFERGLUTPROC)wrap_addr_glut[GLREADBUFFERGLUT].orig_func)(src);
}

void GLAPIENTRY glRotatefGLUT(GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {
    float_t c = cosf(angle * DEG_TO_RAD_FLOAT);
    float_t s = sinf(angle * DEG_TO_RAD_FLOAT);
    float_t t = 1.0f - c;

    mat4 mat;
    mat.row0 = { t * x * x + c, t * x * y + z * s, t * x * z - y * s, 0.0f };
    mat.row1 = { t * y * x - z * s, t * y * y + c, t * y * z + x * s, 0.0f };
    mat.row2 = { t * z * x + y * s, t * z * y - x * s, t * z * z + c, 0.0f };
    mat.row3 = { 0.0f, 0.0f, 0.0f, 1.0f };
    mat4_transpose(&mat, &mat);
    glMultTransposeMatrixf((const GLfloat*)&mat);
    //((PFNGLROTATEFGLUTPROC)wrap_addr_glut[GLROTATEFGLUT].orig_func)(angle, x, y, z);
}

void GLAPIENTRY glScalefGLUT(GLfloat x, GLfloat y, GLfloat z) {
    mat4 mat;
    mat4_scale(x, y, z, &mat);
    mat4_transpose(&mat, &mat);
    glMultTransposeMatrixf((const GLfloat*)&mat);
    //((PFNGLSCALEFGLUTPROC)wrap_addr_glut[GLSCALEFGLUT].orig_func)(x, y, z);
}

void GLAPIENTRY glTranslatefGLUT(GLfloat x, GLfloat y, GLfloat z) {
    mat4 mat;
    mat4_translate(x, y, z, &mat);
    mat4_transpose(&mat, &mat);
    glMultTransposeMatrixf((const GLfloat*)&mat);
    //((PFNGLTRANSLATEFGLUTPROC)wrap_addr_glut[GLTRANSLATEFGLUT].orig_func)(x, y, z);
}

void GLAPIENTRY glVertex2fGLUT(GLfloat x, GLfloat y) {
    ((PFNGLVERTEX2FGLUTPROC)wrap_addr_glut[GLVERTEX2FGLUT].orig_func)(x, y);
}

void GLAPIENTRY glVertex3fGLUT(GLfloat x, GLfloat y, GLfloat z) {
    ((PFNGLVERTEX3FGLUTPROC)wrap_addr_glut[GLVERTEX3FGLUT].orig_func)(x, y, z);
}

void GLAPIENTRY glVertex3fvGLUT(GLfloat const * v) {
    ((PFNGLVERTEX3FVGLUTPROC)wrap_addr_glut[GLVERTEX3FVGLUT].orig_func)(v);
}

HGLRC GLAPIENTRY wglCreateContextGLUT(HDC hDc) {
    return ((PFNWGLCREATECONTEXTGLUTPROC)wrap_addr_glut[WGLCREATECONTEXTGLUT].orig_func)(hDc);
}

typedef void (GLAPIENTRY* PFNGLTEXPARAMETERIVPROC)(GLenum target, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY* PFNGLSTENCILMASKPROC)(GLuint mask);
typedef void (GLAPIENTRY* PFNGLPRIMITIVERESTARTINDEXPROC)(GLuint index);
typedef void (GLAPIENTRY* PFNGLCLEARDEPTHFPROC)(GLfloat d);
typedef void (GLAPIENTRY* PFNGLGETPROGRAMBINARYPROC)(GLuint program, GLsizei bufSize, GLsizei* length, GLenum* binaryFormat, void* binary);
typedef void (GLAPIENTRY* PFNGLPROGRAMBINARYPROC)(GLuint program, GLenum binaryFormat, const void* binary, GLsizei length);
typedef void (GLAPIENTRY* PFNGLBUFFERSTORAGEPROC)(GLenum target, GLsizeiptr size, const void* data, GLbitfield flags);
typedef void (GLAPIENTRY* PFNGLCREATEBUFFERSPROC)(GLsizei n, GLuint* buffers);
typedef void (GLAPIENTRY* PFNGLNAMEDBUFFERSTORAGEPROC)(GLuint buffer, GLsizeiptr size, const void* data, GLbitfield flags);
typedef void (GLAPIENTRY* PFNGLNAMEDBUFFERSUBDATAPROC)(GLuint buffer, GLintptr offset, GLsizeiptr size, const void* data);
typedef void* (GLAPIENTRY* PFNGLMAPNAMEDBUFFERPROC)(GLuint buffer, GLenum access);
typedef GLboolean (GLAPIENTRY* PFNGLUNMAPNAMEDBUFFERPROC)(GLuint buffer);
typedef void (GLAPIENTRY* PFNGLTEXTURESUBIMAGE2DPROC)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);

#if DEBUG
typedef void(GLAPIENTRY* PFNGLPUSHDEBUGGROUPPROC)(GLenum source, GLuint id, GLsizei length, const GLchar* message);
typedef void(GLAPIENTRY* PFNGLPOPDEBUGGROUPPROC)();
#endif

static PFNGLTEXPARAMETERIVPROC _glTexParameteriv = 0;
static PFNGLSTENCILMASKPROC _glStencilMask = 0;
static PFNGLPRIMITIVERESTARTINDEXPROC _glPrimitiveRestartIndex = 0;
static PFNGLCLEARDEPTHFPROC _glClearDepthf = 0;
static PFNGLGETPROGRAMBINARYPROC _glGetProgramBinary = 0;
static PFNGLPROGRAMBINARYPROC _glProgramBinary = 0;
static PFNGLBUFFERSTORAGEPROC _glBufferStorage = 0;
static PFNGLCREATEBUFFERSPROC _glCreateBuffers = 0;
static PFNGLNAMEDBUFFERSTORAGEPROC _glNamedBufferStorage = 0;
static PFNGLNAMEDBUFFERSUBDATAPROC _glNamedBufferSubData = 0;
static PFNGLMAPNAMEDBUFFERPROC _glMapNamedBuffer = 0;
static PFNGLUNMAPNAMEDBUFFERPROC _glUnmapNamedBuffer = 0;
static PFNGLTEXTURESUBIMAGE2DPROC _glTextureSubImage2D = 0;

#if DEBUG
static PFNGLPUSHDEBUGGROUPPROC _glPushDebugGroup = 0;
static PFNGLPOPDEBUGGROUPPROC _glPopDebugGroup = 0;
#endif

void GLAPIENTRY glTexParameteriv(GLenum target, GLenum pname, const GLint* params) {
    if (!_glTexParameteriv)
        _glTexParameteriv = (PFNGLTEXPARAMETERIVPROC)GetProcAddress((HMODULE)opengl32_handle, "glTexParameteriv");
    _glTexParameteriv(target, pname, params);
}

void GLAPIENTRY glStencilMask(GLuint mask) {
    if (!_glStencilMask)
        _glStencilMask = (PFNGLSTENCILMASKPROC)wglGetProcAddressDLL("glStencilMask");
    _glStencilMask(mask);
}

void GLAPIENTRY glPrimitiveRestartIndex(GLuint index) {
    if (!_glPrimitiveRestartIndex)
        _glPrimitiveRestartIndex = (PFNGLPRIMITIVERESTARTINDEXPROC)wglGetProcAddressDLL("glPrimitiveRestartIndex");
    _glPrimitiveRestartIndex(index);
}

void GLAPIENTRY glClearDepthf(GLfloat d) {
    if (!_glClearDepthf)
        _glClearDepthf = (PFNGLCLEARDEPTHFPROC)wglGetProcAddressDLL("glClearDepthf");
    _glClearDepthf(d);
}

void GLAPIENTRY glGetProgramBinary(GLuint program, GLsizei bufSize, GLsizei* length, GLenum* binaryFormat, void* binary) {
    if (!_glGetProgramBinary)
        _glGetProgramBinary = (PFNGLGETPROGRAMBINARYPROC)wglGetProcAddressDLL("glGetProgramBinary");
    _glGetProgramBinary(program, bufSize, length, binaryFormat, binary);
}

void GLAPIENTRY glProgramBinary(GLuint program, GLenum binaryFormat, const void* binary, GLsizei length) {
    if (!_glProgramBinary)
        _glProgramBinary = (PFNGLPROGRAMBINARYPROC)wglGetProcAddressDLL("glProgramBinary");
    _glProgramBinary(program, binaryFormat, binary, length);
}

void GLAPIENTRY glBufferStorage(GLenum target, GLsizeiptr size, const void* data, GLbitfield flags) {
    if (!_glBufferStorage)
        _glBufferStorage = (PFNGLBUFFERSTORAGEPROC)wglGetProcAddressDLL("glBufferStorage");
    _glBufferStorage(target, size, data, flags);
}

void GLAPIENTRY glCreateBuffers(GLsizei n, GLuint* buffers) {
    if (!_glCreateBuffers)
        _glCreateBuffers = (PFNGLCREATEBUFFERSPROC)wglGetProcAddressDLL("glCreateBuffers");
    _glCreateBuffers(n, buffers);
}

void GLAPIENTRY glNamedBufferStorage(GLuint buffer, GLsizeiptr size, const void* data, GLbitfield flags) {
    if (!_glNamedBufferStorage)
        _glNamedBufferStorage = (PFNGLNAMEDBUFFERSTORAGEPROC)wglGetProcAddressDLL("glNamedBufferStorage");
    _glNamedBufferStorage(buffer, size, data, flags);
}

void GLAPIENTRY glNamedBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr size, const void* data) {
    if (!_glNamedBufferSubData)
        _glNamedBufferSubData = (PFNGLNAMEDBUFFERSUBDATAPROC)wglGetProcAddressDLL("glNamedBufferSubData");
    _glNamedBufferSubData(buffer, offset, size, data);
}

void* GLAPIENTRY glMapNamedBuffer(GLuint buffer, GLenum access) {
    if (!_glMapNamedBuffer)
        _glMapNamedBuffer = (PFNGLMAPNAMEDBUFFERPROC)wglGetProcAddressDLL("glMapNamedBuffer");
    return _glMapNamedBuffer(buffer, access);
}

GLboolean GLAPIENTRY glUnmapNamedBuffer(GLuint buffer) {
    if (!_glUnmapNamedBuffer)
        _glUnmapNamedBuffer = (PFNGLUNMAPNAMEDBUFFERPROC)wglGetProcAddressDLL("glUnmapNamedBuffer");
    return _glUnmapNamedBuffer(buffer);
}

void GLAPIENTRY glTextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels) {
    if (!_glTextureSubImage2D)
        _glTextureSubImage2D = (PFNGLTEXTURESUBIMAGE2DPROC)wglGetProcAddressDLL("glTextureSubImage2D");
    _glTextureSubImage2D(texture, level, xoffset, yoffset, width, height, format, type, pixels);
}

#if DEBUG
void constexpr GLAPIENTRY glPushDebugGroup(GLenum source, GLuint id, GLsizei length, const GLchar* message) {
    if (!_glPushDebugGroup)
        _glPushDebugGroup = (PFNGLPUSHDEBUGGROUPPROC)wglGetProcAddressDLL("glPushDebugGroup");
    return _glPushDebugGroup(source, id, length, message);
}

void constexpr GLAPIENTRY glPopDebugGroup() {
    if (!_glPopDebugGroup)
        _glPopDebugGroup = (PFNGLPOPDEBUGGROUPPROC)wglGetProcAddressDLL("glPopDebugGroup");
    return _glPopDebugGroup();
}
#endif
