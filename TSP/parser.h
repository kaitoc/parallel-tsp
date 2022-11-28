#ifndef TSP_PARSER_H
#define TSP_PARSER_H

#ifndef INF
#define INF INT_MAX
#endif

#include <bits/stdc++.h>

using namespace std;

void load_data(string filename, int& n, int**& graph)
{
    ifstream file {"test_data/" + filename};
    
    file >> n;

    graph = new int*[n];
    for (int i = 0; i < n; i++)
        graph[i] = new int[n];

    vector<pair<int, int>> pos;

    int c, x, y;

    for (int i = 0; i < n; i++)
    {
        file >> c >> x >> y;
        pos.push_back(make_pair(x, y));
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
            if (i == j)
            {
                graph[i][j] = INF;
                continue;
            }
            
            int dist = round(sqrt(pow(pos[i].first - pos[j].first, 2) + pow(pos[i].second - pos[j].second, 2) * 1.0));
            graph[i][j] = dist;
            graph[j][i] = dist;
        }

    file.close();
}

#endif //TSP_PARSER_H
