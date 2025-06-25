#pragma once

#include <glm/glm.hpp>

namespace MathHelper {
	inline const double PI = 3.14159265;

	inline const glm::vec3 VEC3_ZERO = glm::vec3(0, 0, 0);
	inline const glm::vec3 VEC3_ONE = glm::vec3(1, 1, 1);
	inline const glm::vec3 VEC3_UP = glm::vec3(0, 1, 0);
	inline const glm::vec3 VEC3_DOWN = glm::vec3(0, -1, 0);

	/// <summary>
	/// ¨¤«×Âà©·«×
	/// </summary>
	inline float DegToRad(float deg) {
		return (PI / 180.0) * deg;
	}
}