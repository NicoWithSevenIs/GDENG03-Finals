#pragma once

#include "Math/Vector2D.h"
#include "Math/Vector3D.h"

class MeshVertex
{
	public:
		Vector3D vert;
		Vector2D tex;
		Vector3D normals;

		inline MeshVertex(Vector3D v3, Vector2D v2, Vector3D n): vert(v3), tex(v2), normals(n){}


};

