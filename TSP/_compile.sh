module load gcc/9.2.0
g++ tsp-parallel.cpp -fopenmp -lpthread -o tsp
module unload gcc/9.2.0