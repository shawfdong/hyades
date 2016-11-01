/*
  The entire set of C++ language bindings have been deprecated since MPI-2.2
*/
#include "mpi.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
  int rank, size;

  MPI::Init(argc, argv);
  rank = MPI::COMM_WORLD.Get_rank();
  size = MPI::COMM_WORLD.Get_size();  
  cout << "Hello, world! I am process " << rank << " of " << size << endl;
  MPI::Finalize();

  return 0;
}
