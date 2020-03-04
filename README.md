# SortAlgorithms
Sort parallel algorithms created with C++ and MPI for HPC exam

## You need MPICH and C++ tools to build project.

# Build
### make

# Run on a single computer
### mpirun -np {number_of_process} ./main

# Run on a cluster
### Set masters and slaves IPs, Hostnames, Usernames on "machinefile" and in "initCluster.sh"
### mpirun -np {number_of_nodes} --hostfile machinefile ./main 
