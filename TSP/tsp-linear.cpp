#include <bits/stdc++.h>
#include "parser.h"

using namespace std;

#ifndef INF
#define INF INT_MAX
#endif

int n;
int **graph;
int *visited;
int *path;

int **new_graph(int **g)
{
	int **g_p = new int *[n];
	for (int i = 0; i < n; i++)
	{
		g_p[i] = new int[n];
		for (int j = 0; j < n; j++)
			g_p[i][j] = g[i][j];
	}
	return g_p;
}

void delete_graph(int **g)
{
	for (int i = 0; i < n; i++)
		delete[] g[i];
	delete[] g;
}

void copy_graph(int **g_a, int **g_b)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			g_a[i][j] = g_b[i][j];
}

int reduce_row(int **g, int i)
{
	int reduce_factor = INF;
	for (int j = 0; j < n; j++)
	{
		if (g[i][j] == 0)
			return 0;
		if (g[i][j] < reduce_factor)
			reduce_factor = g[i][j];
	}
	if (reduce_factor == INF)
		return 0;
	for (int j = 0; j < n; j++)
		if (g[i][j] != INF)
			g[i][j] -= reduce_factor;
	return reduce_factor;
}

int reduce_col(int **g, int j)
{
	int reduce_factor = INF;
	for (int i = 0; i < n; i++)
	{
		if (g[i][j] == 0)
			return 0;
		if (g[i][j] < reduce_factor)
			reduce_factor = g[i][j];
	}
	if (reduce_factor == INF)
		return 0;
	for (int i = 0; i < n; i++)
		if (g[i][j] != INF)
			g[i][j] -= reduce_factor;
	return reduce_factor;
}

void set_row_inf(int **g, int i)
{
	for (int j = 0; j < n; j++)
		g[i][j] = INF;
}

void set_col_inf(int **g, int j)
{
	for (int i = 0; i < n; i++)
		g[i][j] = INF;
}

int next_unvisted(int s)
{
	for (int j = s + 1; j < n; j++)
	{
		if (visited[j] == 0)
			return j;
	}
	return n + 1;
}

bool is_all_visited()
{
	for (int i = 0; i < n; i++)
		if (visited[i] == 0)
			return false;
	return true;
}

int reduce_node(int **g, int p_prev, int p_next)
{
	int weight = g[p_prev][p_next];

	set_row_inf(g, p_prev);
	set_col_inf(g, p_next);
	g[p_next][p_prev] = INF;

	int cost = 0;
	for (int i = 0; i < n; i++)
		cost += reduce_row(g, i);
	for (int j = 0; j < n; j++)
		cost += reduce_col(g, j);

	return cost + weight;
}

int tsp_recursive(int **g, int cost_upper, int p_start, int p_prev, int depth, int cost)
{
	if (is_all_visited())
		return cost;

	int **g_p = new_graph(g);

	int cost_temp;
	vector<pair<int, int>> v;

	for (int p_next = next_unvisted(-1); p_next < n; p_next = next_unvisted(p_next))
	{
		copy_graph(g_p, g);
		cost_temp = cost + reduce_node(g_p, p_prev, p_next);
		if (cost_temp < cost_upper)
			v.push_back(make_pair(cost_temp, p_next));
	}

	sort(v.begin(), v.end());

	for (int i = 0; i < v.size(); i++)
	{
		visited[v[i].second] = true;
		copy_graph(g_p, g);
		reduce_node(g_p, p_prev, v[i].second);
		cost_temp = tsp_recursive(g_p, cost_upper, p_start, v[i].second, depth + 1, v[i].first);
		if (cost_temp > 0 && cost_temp < cost_upper)
		{
			path[depth] = v[i].second;
			delete_graph(g_p);
			return cost_temp;
		}
		visited[v[i].second] = false;
	}

	delete_graph(g_p);
	return -1;
}

int tsp()
{
	int **g = new_graph(graph);
	int **g_temp = new_graph(g);
	int **g_min = new_graph(g);

	path = new int[n];
	visited = new int[n];

	int cost, cost_initial = 0, cost_upper = INF, cost_upper_prev;
	int p_start = 0;

	for (int i = 0; i < n; i++)
		cost_initial += reduce_row(g, i);
	for (int j = 0; j < n; j++)
		cost_initial += reduce_col(g, j);
	
	int **g_bkp = new_graph(g);

	cost = cost_initial;
	fill_n(path, n, -1);
	fill_n(visited, n, 0);
	path[0] = p_start;
	visited[p_start] = true;

	for (int pos = 1; pos < n; pos++)
	{
		int p_min, p_curr = path[pos - 1];
		int cost_min = INF;
		for (int p_next = next_unvisted(-1); p_next < n; p_next = next_unvisted(p_next))
		{
			copy_graph(g_temp, g);
			int cost_temp = cost + reduce_node(g_temp, p_curr, p_next);
			if (cost_temp < cost_min)
			{
				cost_min = cost_temp;
				p_min = p_next;
				copy_graph(g_min, g_temp);
			}
		}
		cost = cost_min;
		path[pos] = p_min;
		visited[p_min] = true;
		copy_graph(g, g_min);
	}

	do
	{
		cost_upper = cost;
		cost_upper_prev = cost_upper;

		cost = cost_initial;
		fill_n(visited, n, 0);
		path[0] = p_start;
		visited[p_start] = true;

		cost = tsp_recursive(g_bkp, cost_upper, p_start, p_start, 1, cost);

		if (cost > 0)
			cost_upper = cost;

	} while (cost_upper < cost_upper_prev);

	delete_graph(g_bkp);
	delete_graph(g_min);
	delete_graph(g_temp);
	delete_graph(g);

	return cost_upper;
}

int main()
{
	load_data("microtest.tsp", n, graph);
	//load_data("xqf131.tsp", n, graph);
	int cost = tsp();
	cout << "cost: " << cost << endl;
	cout << "path: ";
	for (int i = 0; i < n; i++)
		cout << path[i] + 1 << " ";
	cout << endl;
	return 0;
}