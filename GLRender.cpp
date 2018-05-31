#include "GLRender.hpp"

void GLRender::BeginDraw(int value)
{
	glBegin(value);
}

void GLRender::EndDraw()
{
	glEnd();
}

void GLRender::Enable(int mode)
{
	glEnable( mode );
}

void GLRender::Disable(int mode)
{
	glDisable( mode );
}

void GLRender::PushMatrix()
{
	glPushMatrix();
}

void GLRender::PopMatrix()
{
	glPopMatrix();
}

void GLRender::LoadIdentityMatrix()
{
	glLoadIdentity();
}

void GLRender::Translatef(float x, float y, float z)
{
	glTranslatef(x,y,z);
}

void GLRender::Translated(double x, double y, double z)
{
	glTranslated(x,y,z);
}

void GLRender::Rotatef(float angle,float x, float y, float z)
{
	glRotatef(angle,x,y,z);
}

void GLRender::Rotated(double angle,double x, double y, double z)
{
	glRotatef(angle,x,y,z);
}

void GLRender::Scalef(float x, float y, float z)
{
	glScalef(x,y,z);
}

void GLRender::Scaled(double x, double y, double z)
{
	glScaled(x,y,z);
}

void GLRender::BindTexture(int mode, int id)
{
	glBindTexture(mode, id);
}

void GLRender::Vertex3f(float x, float y, float z)
{
	glVertex3f(x,y,z);
}

void GLRender::Vertex3d(double x, double y, double z)
{
	glVertex3d(x,y,z);
}

void GLRender::Vertex3i(int x, int y, int z)
{
	glVertex3i(x,y,z);
}

void GLRender::TexCoord2d(double x, double y)
{
	glTexCoord2d(x,y);
}

void GLRender::TexCoord2f(float x, float y)
{
	glTexCoord2f(x,y);
}

void GLRender::Color4f(float r,float g, float b, float alpha)
{
	glColor4f(r,g,b,alpha);
}

void GLRender::Color3f(float r,float g, float b)
{
	glColor3f(r,g,b);
}

void GLRender::ClearColor(float r, float g, float b, float alpha)
{
	glClearColor(r,g,b,alpha);
}

void GLRender::ClearDepth(double value)
{
	glClearDepth(value);
}

void GLRender::EnableDepthMask()
{
	glDepthMask(true);
}

void GLRender::DisableDepthMask()
{
	glDepthMask(false);
}

void GLRender::DeleteBuffers(int size,unsigned int* id)
{
	glDeleteBuffers(size, id);
}

void GLRender::EnableClientState(int mode)
{
	glEnableClientState( mode );
}

void GLRender::DisableClientState(int mode)
{
	glDisableClientState( mode );
}

void GLRender::BindBuffer(int mode,unsigned int id)
{
	glBindBuffer(mode, id);
}

void GLRender::DrawArray(int mode, int first, int size)
{
	glDrawArrays(mode, first, size);
}

void GLRender::GenerateBuffer(int size, unsigned int* id)
{
	glGenBuffers(size,id);
}

void GLRender::PolygonMode(int mode, int mode1)
{
	glPolygonMode(mode, mode1);
}

void GLRender::BlendFunc(int mode , int mode1)
{
	glBlendFunc(mode , mode1);
}

void GLRender::Clear(int mode)
{
	glClear( mode );
}

void GLRender::PointSize(float value)
{
	glPointSize(value);
}

void GLRender::VertexPointer(int size, float mode, int stripe,const GLvoid* ptr)
{
	glVertexPointer(size, mode, stripe, ptr);
}

void GLRender::ColorPointer(int size, float mode, int stripe,const GLvoid* ptr)
{
	glColorPointer(size, mode, stripe, ptr);
}

void GLRender::TexCoordPointer(int size, float mode, int stripe,const GLvoid* ptr)
{
	glTexCoordPointer(size, mode, stripe, ptr);
}

void GLRender::FrontFace(int mode)
{
	glFrontFace(mode);
}

void GLRender::BufferData(int mode, GLsizeiptr size, const GLvoid* ptr, int mode1)
{
	glBufferData( mode, size, ptr, mode1 );
}