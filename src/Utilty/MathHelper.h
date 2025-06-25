#pragma once

#include <glm/glm.hpp>

namespace MathHelper {
	const double PI = 3.14159265;

	/// <summary>
	/// ¨¤«×Âà©·«×
	/// </summary>
	float DegToRad(float deg) {
		return (PI / 180.0) * deg;
	}
}