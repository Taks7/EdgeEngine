
#include "Globals.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <vector>
#include "Primitive.h"


// ------------------------------------------------------------
Primitive::Primitive() : transform(IdentityMatrix), color(White), wire(false), axis(false), type(PrimitiveTypes::Primitive_Point)
{}

// ------------------------------------------------------------
PrimitiveTypes Primitive::GetType() const
{
	return type;
}

void Primitive::Update()
{
	//TODO 5: Set the primitive position and rotation to the PhysBody position and rotation
	
}

// ------------------------------------------------------------
void Primitive::Render() const
{
	glPushMatrix();
	glMultMatrixf(transform.M);

	if(axis == true)
	{
		// Draw Axis Grid
		
		glLineWidth(2.0f);

		glBegin(GL_LINES);

		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
		glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
		glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
		glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

		glEnd();

		glLineWidth(1.0f);

		
	}

	

	glColor3f(color.r, color.g, color.b);


	InnerRender();

	glPopMatrix();
}

// ------------------------------------------------------------
void Primitive::InnerRender() const
{
	glPointSize(5.0f);

	glBegin(GL_POINTS);

	glVertex3f(0.0f, 0.0f, 0.0f);

	glEnd();

	glPointSize(1.0f);
}

// ------------------------------------------------------------
void Primitive::SetPos(float x, float y, float z)
{
	transform.translate(x, y, z);
	//TODO 6: Set the body position to the new position too!
}

// ------------------------------------------------------------
void Primitive::SetRotation(float angle, const vec3 &u)
{
	transform.rotate(angle, u);
	//TODO 6: Set the body rotation to the new rotation too!
}

// ------------------------------------------------------------
void Primitive::Scale(float x, float y, float z)
{
	transform.scale(x, y, z);
	//TODO 6: Set the body scale to the new scale too!
}

// CUBE ============================================
Cube::Cube() : Primitive(), size(1.0f, 1.0f, 1.0f)
{
	type = PrimitiveTypes::Primitive_Cube;
}

Cube::Cube(float sizeX, float sizeY, float sizeZ) : Primitive(), size(sizeX, sizeY, sizeZ)
{
	type = PrimitiveTypes::Primitive_Cube;
	GLfloat vertices[] = { sizeX, sizeY, sizeZ,  -sizeX, sizeY, sizeZ,  -sizeX,-sizeY, sizeZ,   sizeX,-sizeY, sizeZ,   // v0,v1,v2,v3 (front)
							sizeX, sizeY, sizeZ,   sizeX,-sizeY, sizeZ,   sizeX,-sizeY,-sizeZ,   sizeX, sizeY,-sizeZ,   // v0,v3,v4,v5 (right)
							sizeX, sizeY, sizeZ,   sizeX, sizeY,-sizeZ,  -sizeX, sizeY,-sizeZ,  -sizeX, sizeY, sizeZ,   // v0,v5,v6,v1 (top)
						  -sizeX, sizeY, sizeZ,  -sizeX, sizeY,-sizeZ,  -sizeX,-sizeY,-sizeZ,  -sizeX,-sizeY, sizeZ,   // v1,v6,v7,v2 (left)
						  -sizeX,-sizeY,-sizeZ,   sizeX,-sizeY,-sizeZ,   sizeX,-sizeY, sizeZ,  -sizeX,-sizeY, sizeZ,   // v7,v4,v3,v2 (bottom)
							sizeX,-sizeY,-sizeZ,  -sizeX,-sizeY,-sizeZ,  -sizeX, sizeY,-sizeZ,   sizeX, sizeY,-sizeZ }; // v4,v7,v6,v5 (back)

	GLubyte indices[] = { 0, 1, 2,   2, 3, 0,      // front
					   4, 5, 6,   6, 7, 4,      // right
					   8, 9,10,  10,11, 8,      // top
					  12,13,14,  14,15,12,      // left
					  16,17,18,  18,19,16,      // bottom
					  20,21,22,  22,23,20 };    // back
	
		// activate and specify pointer to vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	// draw a cube
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

	// deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);
}

void Cube::InnerRender() const
{	
	float sx = size.x * 0.5f;
	float sy = size.y * 0.5f;
	float sz = size.z * 0.5f;

	glBegin(GL_QUADS);

	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-sx, -sy, sz);
	glVertex3f( sx, -sy, sz);
	glVertex3f( sx,  sy, sz);
	glVertex3f(-sx,  sy, sz);

	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f( sx, -sy, -sz);
	glVertex3f(-sx, -sy, -sz);
	glVertex3f(-sx,  sy, -sz);
	glVertex3f( sx,  sy, -sz);

	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(sx, -sy,  sz);
	glVertex3f(sx, -sy, -sz);
	glVertex3f(sx,  sy, -sz);
	glVertex3f(sx,  sy,  sz);

	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-sx, -sy, -sz);
	glVertex3f(-sx, -sy,  sz);
	glVertex3f(-sx,  sy,  sz);
	glVertex3f(-sx,  sy, -sz);

	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-sx, sy,  sz);
	glVertex3f( sx, sy,  sz);
	glVertex3f( sx, sy, -sz);
	glVertex3f(-sx, sy, -sz);

	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(-sx, -sy, -sz);
	glVertex3f( sx, -sy, -sz);
	glVertex3f( sx, -sy,  sz);
	glVertex3f(-sx, -sy,  sz);

	glEnd();
}

// SPHERE ============================================

SpherePrimitive::SpherePrimitive(float radius, float rings, float sectors)
{
	type = PrimitiveTypes::Primitive_Sphere;
	//TODO 4: Initialize the PhysBody to be a Sphere
	//std::vector<GLfloat> vertices;
	//std::vector<GLfloat> normals;
	//std::vector<GLfloat> texcoords;
	//std::vector<GLushort> indices;

	//float const R = 1. / (float)(rings - 1);
	//float const S = 1. / (float)(sectors - 1);
	//int r, s;

	//vertices.resize(rings * sectors * 3);
	//normals.resize(rings * sectors * 3);
	//texcoords.resize(rings * sectors * 2);
	//std::vector<GLfloat>::iterator v = vertices.begin();
	//std::vector<GLfloat>::iterator n = normals.begin();
	//std::vector<GLfloat>::iterator t = texcoords.begin();
	//for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) {
	//	float const y = sin(-M_PI_2 + M_PI * r * R);
	//	float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
	//	float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

	//	*t++ = s * S;
	//	*t++ = r * R;

	//	*v++ = x * radius;
	//	*v++ = y * radius;
	//	*v++ = z * radius;

	//	*n++ = x;
	//	*n++ = y;
	//	*n++ = z;
	//}

	//indices.resize(rings * sectors * 4);
	//std::vector<GLushort>::iterator i = indices.begin();
	//for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) {
	//	*i++ = (r + 1) * sectors + s;
	//	*i++ = (r + 1) * sectors + (s + 1);
	//	*i++ = r * sectors + (s + 1);
	//	*i++ = r * sectors + s;
	//}


	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	////glTranslatef(x, y, z);

	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	//glNormalPointer(GL_FLOAT, 0, &normals[0]);
	//glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
	//glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
	//glPopMatrix();
}

void SpherePrimitive::InnerRender() const
{

}


// CYLINDER ============================================
Cylinder::Cylinder() : Primitive(), radius(1.0f), height(1.0f)
{
	type = PrimitiveTypes::Primitive_Cylinder;
}

Cylinder::Cylinder(float radius, float height) : Primitive(), radius(radius), height(height)
{
	type = PrimitiveTypes::Primitive_Cylinder;
}

void Cylinder::InnerRender() const
{
	int n = 30;

	// Cylinder Bottom
	glBegin(GL_POLYGON);
	
	for(int i = 360; i >= 0; i -= (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians
		glVertex3f(-height*0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder Top
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);
	for(int i = 0; i <= 360; i += (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians
		glVertex3f(height * 0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder "Cover"
	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i < 480; i += (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians

		glVertex3f(height*0.5f,  radius * cos(a), radius * sin(a) );
		glVertex3f(-height*0.5f, radius * cos(a), radius * sin(a) );
	}
	glEnd();
}

// LINE ==================================================
Line::Line() : Primitive(), origin(0, 0, 0), destination(1, 1, 1)
{
	type = PrimitiveTypes::Primitive_Line;
}

Line::Line(float x, float y, float z) : Primitive(), origin(0, 0, 0), destination(x, y, z)
{
	type = PrimitiveTypes::Primitive_Line;
}

void Line::InnerRender() const
{
	glLineWidth(2.0f);

	glBegin(GL_LINES);

	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(destination.x, destination.y, destination.z);

	glEnd();

	glLineWidth(1.0f);
}

// PLANE ==================================================
NormalPlane::NormalPlane() : Primitive(), normal(0, 1, 0), constant(1)
{
	type = PrimitiveTypes::Primitive_Plane;
}

NormalPlane::NormalPlane(float x, float y, float z, float d) : Primitive(), normal(x, y, z), constant(d)
{
	type = PrimitiveTypes::Primitive_Plane;
}

void NormalPlane::InnerRender() const
{
	glLineWidth(1.0f);

	glBegin(GL_LINES);

	float d = 200.0f;

	for(float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();
}