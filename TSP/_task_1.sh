#!/bin/bash

# Nombre del job:
#SBATCH --job-name=tsp_1t
# Nombre del archivo de salida:
#SBATCH --output=tsp_1t.txt

#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1

# Numero de tasks:
#SBATCH --ntasks=1
# Numero de CPUs por task:
#SBATCH --cpus-per-task=1

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

module load gcc/9.2.0
./tsp
module unload gcc/9.2.0
