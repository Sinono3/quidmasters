#pragma once
#include "Model.hpp"

template <const unsigned long R, const unsigned long C>
using Map = std::array<std::array<char, C>, R>;

template <const unsigned long R, const unsigned long C>
Model buildMapModel(const Map<R, C> &map);

#include "math/Vector3.hpp"
#include <vector>

template <const unsigned long R, const unsigned long C>
Model buildMapModel(const Map<R, C> &map) {
	std::array<Vector3f, 2 * (R + 1) * (C + 1)> positions;

	// Create point grid (R + 1) x (C + 1) x 2
	for (int y = 0; y < 2; y++) {
		for (int i = 0; i < (R + 1); i++) {
			for (int j = 0; j < (C + 1); j++) {
				positions[(y * (R + 1) * (C + 1)) + i * (C + 1) + j] =
					Vector3f(j, y, i);
			}
		}
	}

	std::vector<unsigned int> indices;

	// Create walls according to the map
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++) {
			if (map[i][j] == 'W') {
				// Bottom vertices
				unsigned int bln = (0 * (R + 1) * (C + 1)) + i * (C + 1) + j;
				unsigned int brn =
					(0 * (R + 1) * (C + 1)) + i * (C + 1) + j + 1;
				unsigned int bls =
					(0 * (R + 1) * (C + 1)) + (i + 1) * (C + 1) + j;
				unsigned int brs =
					(0 * (R + 1) * (C + 1)) + (i + 1) * (C + 1) + j + 1;

				// Top vertices
				unsigned int tln = (1 * (R + 1) * (C + 1)) + i * (C + 1) + j;
				unsigned int trn =
					(1 * (R + 1) * (C + 1)) + i * (C + 1) + j + 1;
				unsigned int tls =
					(1 * (R + 1) * (C + 1)) + (i + 1) * (C + 1) + j;
				unsigned int trs =
					(1 * (R + 1) * (C + 1)) + (i + 1) * (C + 1) + j + 1;

				// Left face
				indices.push_back(bln);
				indices.push_back(tln);
				indices.push_back(bls);
				indices.push_back(bls);
				indices.push_back(tls);
				indices.push_back(tln);

				// Right face
				indices.push_back(brn);
				indices.push_back(trn);
				indices.push_back(brs);
				indices.push_back(brs);
				indices.push_back(trs);
				indices.push_back(trn);

				// the North face
				indices.push_back(bln);
				indices.push_back(brn);
				indices.push_back(trn);
				indices.push_back(trn);
				indices.push_back(tln);
				indices.push_back(bln);

				// the South face
				indices.push_back(bls);
				indices.push_back(brs);
				indices.push_back(trs);
				indices.push_back(trs);
				indices.push_back(tls);
				indices.push_back(bls);
			}
		}
	}

	std::array<float, 3 * 2 * (R + 1) * (C + 1)> flattenedPositions;
	for (int i = 0; i < positions.size(); i++) {
		flattenedPositions[3 * i] = positions[i].x;
		flattenedPositions[3 * i + 1] = positions[i].y;
		flattenedPositions[3 * i + 2] = positions[i].z;
	}

	return Model(flattenedPositions, indices);
}
