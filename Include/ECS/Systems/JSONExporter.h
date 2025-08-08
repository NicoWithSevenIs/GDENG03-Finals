#pragma once

#include "Interfaces/Singleton.h"
#include "ECS/Systems/EntityManager.h"
#include "GameObject/ParentingManager.h"
#include <fstream>
#include <json/json.h>

class JSONExporter : public Singleton<JSONExporter> {

	public:
		inline static void ExportToJSON(std::string filename) {
			std::ofstream file(filename);
			file << "{\n\"GameObjects\": [\n";

			auto all = EntityManager::get_all();

			for (int i =0; i < all.size(); i++) {
				std::string out;
				all[i]->ExportToJSON(out);
				file << out;
				file << (i < all.size() - 1 ? ",\n" : "\n");
			}

			file << "]\n}\n";
			file.close();
		}


		inline static void ImportJSON(std::string filename) {
			std::vector<Entity*> imported;

			std::ifstream file(filename);
			if (!file) {
				std::cout << "File " << filename << " can't be opened";
				return;
			}

			Json::Value root;
			Json::CharReaderBuilder builder;
			std::string errors;




			if (!Json::parseFromStream(builder, file, &root, &errors)) {
				std::cout << "Error parsing JSON file" <<  errors << std::endl;
				file.close();
				return;
			}

			for (int i = 0; i < root["GameObjects"].size(); i++) {

				auto entity = new Entity(root["GameObjects"][i]["Name"].asString());
				entity->enabled = root["GameObjects"][i]["Enabled"].asInt();

				auto translate = root["GameObjects"][i]["Transform"]["Translate"];
				auto scale = root["GameObjects"][i]["Transform"]["Scale"];
				auto rotate = root["GameObjects"][i]["Transform"]["Rotate"];

				entity->m_transform.m_translation = Vector3D(translate[0].asFloat(), translate[1].asFloat(), translate[2].asFloat());
				entity->m_transform.m_scale = Vector3D(scale[0].asFloat(), scale[1].asFloat(), scale[2].asFloat());

				float x = rotate[0].asFloat();
				float y = rotate[1].asFloat();
				float z = rotate[2].asFloat();
				float w = rotate[3].asFloat();

				// roll (x-axis rotation)
				double sinr_cosp = 2 * (w * x + y * z);
				double cosr_cosp = 1 - 2 * (x * x + y * y);
				double r_x = std::atan2(sinr_cosp, cosr_cosp);

				// pitch (y-axis rotation)
				double sinp = std::sqrt(1 + 2 * (w * y - x * z));
				double cosp = std::sqrt(1 - 2 * (w * y - x * z));
				double r_y = 2 * std::atan2(sinp, cosp) - 3.14159265358979323846 / 2;

				// yaw (z-axis rotation)
				double siny_cosp = 2 * (w * z + x * y);
				double cosy_cosp = 1 - 2 * (y * y + z * z);
				double r_z = std::atan2(siny_cosp, cosy_cosp);

				entity->m_transform.m_rotation = Vector3D(r_x,r_y,r_z);
				
				Json::Value component_data;

				if (root["GameObjects"][i]["Components"].isMember("Renderer")) {
					std::istringstream is(root["GameObjects"][i]["Components"]["Renderer"].asString());
					Json::parseFromStream(builder, is, &component_data, &errors);
					if (component_data["Type"] == "Cube") {
						entity->AddComponent<CubeRenderer>();
					}
				}

				if (root["GameObjects"][i]["Components"].isMember("PhysicsComponent")) {
					std::istringstream is(root["GameObjects"][i]["Components"]["PhysicsComponent"].asString());
					Json::parseFromStream(builder, is, &component_data, &errors);
					auto p6Component = entity->AddComponent<PhysicsComponent>(reactphysics3d::BodyType::DYNAMIC);
					PhysicsSystem::AddPhysicsComponent(p6Component);
				}

				imported.push_back(entity);

			}

			file.close();

			EntityManager::Release();
			ParentingManager::get().Release();
			for (auto e : imported) {
				EntityManager::AddObject(e);
			}
		}
};