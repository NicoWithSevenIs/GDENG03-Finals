#pragma once

#include "Interfaces/Singleton.h"
#include "ECS/Systems/EntityManager.h"
#include <fstream>
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


		inline static void ImportJSON() {

		}
};