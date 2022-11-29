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

int upper = INF;

int tsp_recursive(int **g, int p_prev, int depth, int cost)
{
	if (depth == n)
	{
		upper = min(upper, cost);
		return upper;
	}

	int **g_p = new_graph(g);
	
	priority_queue<pair<int, int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;

	for (int p_next = next_unvisted(-1); p_next < n; p_next = next_unvisted(p_next))
	{
		copy_graph(g_p, g);
		int c_temp = cost + reduce_node(g_p, p_prev, p_next);
		if (c_temp < upper)
			pq.push(make_pair(c_temp, p_next));
	}

	int c_ans = INF;

	while (!pq.empty())
	{
		pair<int, int> p = pq.top();
		pq.pop();

		int p_next = p.second;
		int c_temp = p.first;

		if (c_temp > upper)
			break;
		
		cout << "depth: " << depth << " p_prev: " << p_prev << " p_next: " << p_next << " cost: " << c_temp << " upper: " << upper << endl;
		
		visited[p_next] = true;

		copy_graph(g_p, g);
		reduce_node(g_p, p_prev, p_next);

		int c_rec = tsp_recursive(g_p, p_next, depth + 1, c_temp);

		if (c_rec != INF)
		{
			path[depth] = p_next;
			c_ans = min(c_ans, c_rec);
		}

		visited[p_next] = false;
	}

	delete_graph(g_p);

	return c_ans;
}

int tsp()
{
	int **g = new_graph(graph);

	int p_start = 0;
	int cost_initial = 0;

	path = new int[n];
	visited = new int[n];

	fill_n(path, n, -1);
	fill_n(visited, n, 0);

	for (int i = 0; i < n; i++)
		cost_initial += reduce_row(g, i);
	for (int j = 0; j < n; j++)
		cost_initial += reduce_col(g, j);
	
	path[0] = p_start;
	visited[p_start] = true;

	int cost = tsp_recursive(g, p_start, 1, cost_initial);

	delete_graph(g);

	return cost;
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