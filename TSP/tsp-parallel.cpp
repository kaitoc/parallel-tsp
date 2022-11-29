#include <bits/stdc++.h>
#include <omp.h>
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
	#pragma omp parallel for schedule(dynamic)
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
	#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < n; i++)
		delete[] g[i];
	delete[] g;
}

void copy_graph(int **g_a, int **g_b)
{
	#pragma omp parallel for schedule(dynamic) collapse(2)
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			g_a[i][j] = g_b[i][j];
}

int reduce_row(int **g, int i)
{
	int reduce_factor = INF;
	#pragma omp parallel for schedule(dynamic) reduction(min:reduce_factor) 
	for (int j = 0; j < n; j++)
	{
		if (g[i][j] < reduce_factor)
			reduce_factor = g[i][j];
	}
	if (reduce_factor == INF || reduce_factor == 0)
		return 0;
	#pragma omp parallel for schedule(dynamic)
	for (int j = 0; j < n; j++)
		if (g[i][j] != INF)
			g[i][j] -= reduce_factor;
	return reduce_factor;
}

int reduce_col(int **g, int j)
{
	int reduce_factor = INF;
	#pragma omp parallel for schedule(dynamic) reduction(min:reduce_factor) 
	for (int i = 0; i < n; i++)
	{
		if (g[i][j] < reduce_factor)
			reduce_factor = g[i][j];
	}
	if (reduce_factor == INF || reduce_factor == 0)
		return 0;
	#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < n; i++)
		if (g[i][j] != INF)
			g[i][j] -= reduce_factor;
	return reduce_factor;
}

void set_row_inf(int **g, int i)
{
	#pragma omp parallel for schedule(dynamic)
	for (int j = 0; j < n; j++)
		g[i][j] = INF;
}

void set_col_inf(int **g, int j)
{
	#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < n; i++)
		g[i][j] = INF;
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

double start_time, end_time;

void print_path()
{
	cout << "{" << endl;
	cout << "\t\"cost\": " << upper << ", " << endl;
	cout << "\t\"path\": [ ";
	for (int i = 0; i < n - 1; i++)
		cout << path[i] + 1 << ", ";
	cout << path[n-1] << " ]," << endl;
	cout << "\t\"time\": " << omp_get_wtime() - start_time << endl;
	cout << "}," << endl;
}

void tsp_recursive(int **g, int p_prev, int depth, int cost)
{
	if (depth == n)
	{
		upper = min(upper, cost);
		print_path();
		return;
	}

	int **g_p = new_graph(g);
	
	priority_queue<pair<int, int>, vector<pair<int,int> >, greater<pair<int,int> > > pq;

	#pragma omp parallel for schedule(dynamic)
	for (int p_next = 0; p_next < n; p_next++)
	{
		if (visited[p_next] != 0)
			continue;
		
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
		
		visited[p_next] = true;

		copy_graph(g_p, g);
		reduce_node(g_p, p_prev, p_next);

		path[depth] = p_next;

		tsp_recursive(g_p, p_next, depth + 1, c_temp);

		visited[p_next] = false;
	}

	delete_graph(g_p);
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

	tsp_recursive(g, p_start, 1, cost_initial);

	delete_graph(g);

	return upper;
}

int main()
{
	//load_data("microtest.tsp", n, graph);
	load_data("xqf131.tsp", n, graph);
	
	int cost;
	
	start_time = omp_get_wtime(); 
	
	#pragma omp parallel
	{
		#pragma omp single
		{
			cost = tsp();
		}
	}

	end_time = omp_get_wtime(); 
	/*
	cout << "cost: " << cost << endl;
	cout << "path: ";
	for (int i = 0; i < n; i++)
		cout << path[i] + 1 << " ";
	cout << endl;
	cout << "time: " << end_time - start_time << endl;
	*/
	return 0;
}