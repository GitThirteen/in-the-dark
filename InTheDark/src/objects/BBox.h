#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>

/**
* @brief A rectangular 3D bounding box is defined by two vertices - the bottom
* left one (lower) and the upper right one (upper), thus allowing to interpolate all other vertices
* of the bounding box. The lower and upper vertices are relative to the object position,
* which is the center of the top plane of the bounding box.
*/
struct BBox {

	/* Bounding Box
		  X -----/---- (U)
		 / |    /      /|
		/  |   P      / |
	   /   |         /  |
	  X ----------- X   |
	  |    X -------|-- X
	  |   /         |  /
	  |  /          | /
	  | /           |/
	 (L) ---------- X
	*/

	glm::vec3 lower;
	glm::vec3 upper;
};