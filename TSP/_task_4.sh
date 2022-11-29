#!/bin/bash

# Nombre del job:
#SBATCH --job-name=tsp_4t
# Nombre del archivo de salida:
#SBATCH --output=tsp_4t.txt

#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1

# Numero de tasks:
#SBATCH --ntasks=1
# Numero de CPUs por task:
#SBATCH --cpus-per-task=4

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

module load gcc/9.2.0
./tsp
module unload gcc/9.2.0