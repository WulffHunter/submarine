// Jared Rand
// 500683609

#ifndef QUADMESH_H
#define QUADMESH_H
#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
	#include <glut/glut.h>
#else
	#include <gl/glut.h>
#endif
#include <stdbool.h>
#include "Vector3D.h"
#include "Util.h"

// Data structure for a vertex
typedef struct MeshVertex
{
	Vector3D position;
	Vector3D normal;
} MeshVertex;

// Data structure for a quad (4-sided polygon)
typedef struct MeshQuad
{
	// pointers to vertices of each quad in the array of vertices
	MeshVertex *vertices[4];	
} MeshQuad;

typedef struct
{
	int maxMeshSize;
	float meshDim;

	int numVertices;
	MeshVertex *vertices;    // Dynamic array of all vertices

	int numQuads;
	MeshQuad *quads;         // Dynamic array of all quads

	int numFacesDrawn;
	
	GLfloat mat_ambient[4];
    GLfloat mat_specular[4];
    GLfloat mat_diffuse[4];
	GLfloat mat_shininess[1];

	Texture* texture;
} QuadMesh;

QuadMesh NewQuadMesh(int maxMeshSize, Texture* texture);
void SetMaterialQM(QuadMesh* qm, Vector3D ambient, Vector3D diffuse, Vector3D specular, double shininess);
bool CreateMemoryQM(QuadMesh* qm);
bool InitMeshQM(QuadMesh* qm, int meshSize, Vector3D origin, double meshLength, double meshWidth, Vector3D dir1, Vector3D dir2);
void DrawMeshQM(QuadMesh* qm, int meshSize);
void FreeMemoryQM(QuadMesh* qm);
void ComputeNormalsQM(QuadMesh* qm);

#endif