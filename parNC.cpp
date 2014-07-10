
//include nc stuff and header files
//cpp will function, but does not include NetCDF 4 functionality (eg parallel)
//#include <netcdfcpp.h>
#include <netcdf.h>
#include <netcdf_par.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

//cohort id, temperature, time
#define NDIMS 3 
#define DIMSIZE 4 
#define QTR_DATA (DIMSIZE * DIMSIZE / 4)
#define NUM_PROC 4//this is unused?
#define NUM_SLABS 10

#define FILE_NAME "test.nc"

#define ERRCODE 2
#define ERR(e) {printf("Error: %s\n", nc_strerror(e)); exit(ERRCODE);}


int main(int argc, char **argv){

  //*****MPI declarations*****
  int mpi_namelen;
  char mpi_name[MPI_MAX_PROCESSOR_NAME];
  int mpi_size, mpi_rank;
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Info info = MPI_INFO_NULL;

  //*****NetCDF stuff*****

  //Basic definitions
  int ncid;//for file
  int cid_dimid, time_dimid; //dimensions
  int varid; //for something? this might be Nid...
  int retval;

  size_t start[NDIMS], count[NDIMS];

  /**subdividing data for parallel write**/
  int data[DIMSIZE * DIMSIZE], res;
  int slab_data[QTR_DATA];

  /* When we create netCDF variables and dimensions, we get back an
  * ID for each one. */
  int dimids[NDIMS];

  //*****MPI initialization*****
  MPI_Init(&argc, &argv);
  MPI_Comm_size(comm, &mpi_size);
  MPI_Comm_rank(comm, &mpi_rank);
  MPI_Get_processor_name(mpi_name, &mpi_namelen);
  std::cout<<"mpi name: "<<mpi_name<<" size: "<<mpi_size<<" rank: "
           <<mpi_rank<<std::endl;
 

  for(int ii=mpi_rank*QTR_DATA; ii<(mpi_rank+1)*QTR_DATA; ii++){
    data[ii] = mpi_rank;
  }
  for(int ii=0; ii<QTR_DATA; ii++){
    slab_data[ii] = mpi_rank;
  }

  //**create file**
  if ((retval = nc_create(FILE_NAME, NC_CLOBBER|NC_MPIIO|NC_NETCDF4, &ncid)))
     ERR(retval);
  //if((retval = nc_create_par(FILE_NAME, NC_CLOBBER|NC_MPIIO|NC_NETCDF4,
  //                          comm, info, &ncid)));
     ERR(retval);

  //Define dimensions - will return ID for each
  if ((retval = nc_def_dim(ncid, "d1", DIMSIZE, dimids)))
     ERR(retval);
  if ((retval = nc_def_dim(ncid, "d2", DIMSIZE, &dimids[1])))
     ERR(retval);
  if ((retval = nc_def_dim(ncid, "d3", NUM_SLABS, &dimids[2])))
     ERR(retval);


  if ((retval = nc_def_var(ncid, "data", NC_INT, NDIMS, 
                           dimids, &varid)))
     ERR(retval);

    
  //End metadata definition
  if((retval = nc_enddef(ncid)))
     ERR(retval);

  //** Set up slab **
  start[0] = mpi_rank * DIMSIZE/mpi_size;
  start[1] = 0;
  count[0] = DIMSIZE/mpi_size;
  count[1] = DIMSIZE;
  count[2] = 1;

  for(start[2] = 0; start[2]<NUM_SLABS; start[2]++){

    if((retval = nc_put_vara_int(ncid, varid, start, count, slab_data)))
       ERR(retval);
  }

  /* Close the file. This frees up any internal netCDF resources
    * associated with the file, and flushes any buffers. */
  if ((retval = nc_close(ncid)))
      ERR(retval);

  printf("*** SUCCESS writing test.nc!\n");

  MPI_Finalize();

  return 0;
}



