#pragma once

#include "utils_stdafx.h"

namespace Utils
{
	namespace Algo
	{
		template<typename T>
		__declspec(dllexport) void depthFirstSearch(T p_vertex,
			std::vector<int>& VERTEX_STATE,
			const std::vector<std::vector<bool>>& EDGES,
			std::vector<T>& output)
		{
			VERTEX_STATE[p_vertex] = 1; // visited

			for (int i = 0; i < VERTEX_STATE.size(); ++i)
			{
				if (EDGES[i][p_vertex] == true && VERTEX_STATE[i] == 0)
					Algo::depthFirstSearch<T>(i, VERTEX_STATE, EDGES, output);
			}

			VERTEX_STATE[p_vertex] = 2; // done
			output.push_back(p_vertex);
		}
//		__declspec(dllexport) unsigned int upper_power_of_two(unsigned int v);
	}
}