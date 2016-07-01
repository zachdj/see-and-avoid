
/*
Class that can assign waypoints to a given aircraft. These waypoints are the flight patterns each aircraft flies between.
*/

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
// the Waypoint class represents a particular node in a navigational path
class Waypoint
{
public:
	Waypoint();
	Waypoint(glm::vec3 position);
	glm::vec3 GetPosition();
	bool Equals(Waypoint * other);
	void Complete();
	void Activate();
	bool IsActive();
	~Waypoint();

private:
	glm::vec3 position;
	bool active;
};

