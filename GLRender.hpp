#ifndef GL_MYRENDER
#define GL_MYRENDER
#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "GameCommon.hpp"

using namespace std;

class GLRender
{
	public:
		GLRender();
		~GLRender();
		static void BeginDraw(int value);
		static void Clear(int mode);
		static void EndDraw();
		static void Enable(int mode);
		static void Disable(int mode);
		static void BlendFunc(int mode, int mode1);
		static void PushMatrix();
		static void PopMatrix();
		static void LoadIdentityMatrix();
		static void Translatef(float x, float y, float z);
		static void Translated(double x, double y, double z);
		static void Rotatef(float angle, float x, float y, float z);
		static void Rotated(double angle, double x, double y, double z);
		static void Scalef(float x, float y, float z);
		static void Scaled(double x, double y, double z);
		static void BindTexture(int mode, int id);
		static void Vertex3f(float x,float y, float z);
		static void Vertex3d(double x, double y, double z);
		static void Vertex3i(int x,int y,int z);
		static void TexCoord2d(double x, double y);
		static void TexCoord2f(float x, float y);
		static void Color4f(float r, float g, float b, float alpha);
		static void Color3f(float r, float g, float b);
		static void ClearColor(float r, float g, float b, float alpha);
		static void ClearDepth(double value);
		static void EnableDepthMask();
		static void DisableDepthMask();
		static void DeleteBuffers(int size, unsigned int* id);
		static void EnableClientState(int mode);
		static void DisableClientState(int mode);
		static void BindBuffer(int mode, unsigned int id);
		static void DrawArray(int mode,int firstIndex, int size);
		static void GenerateBuffer(int size, unsigned int* id);
		static void PolygonMode(int mode, int mode1);
		static void PointSize(float size);
		static void VertexPointer(int, float, int,const GLvoid*);
		static void ColorPointer(int, float, int,const GLvoid*);
		static void TexCoordPointer(int, float, int, const GLvoid*);
		static void BufferData(int mode, GLsizeiptr size, const GLvoid*, int mode1 );
		static void FrontFace(int mode);
};

#endif