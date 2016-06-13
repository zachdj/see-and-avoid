#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// the Waypoint class represents a particular node in a navigational path
class Waypoint
{
public:
	Waypoint();
	Waypoint(glm::vec3 position);
	glm::vec3 GetPosition();
	void Complete();
	void Activate();
	bool IsActive();
	~Waypoint();

private:
	glm::vec3 position;
	bool active;
};

