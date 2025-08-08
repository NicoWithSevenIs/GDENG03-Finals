#pragma once

#include "GameObject/GameObject.h"
#include <vector>

class Quad: public GameObject
{


public:
	std::vector<vertex> strip_list;

public:
	Quad();
	Quad(std::string name);

	bool load() override;
	bool release() override;
	void Update(float delta_time, Matrix4x4 view_matrix, Matrix4x4 projection_matrix) override;
	void Draw() override;
};

