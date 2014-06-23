
//include nc stuff and header files
//cpp will function, but does not include NetCDF 4 functionality (eg parallel)
//#include <netcdfcpp.h>
#include <netcdf.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>

//#include <mpi.h>

//cohort id, temperature, time
#define NDIMS 2 
#define NVALS 10
#define NCHTS 12

#define FILE_NAME "test.nc"
#define CELL "Grid_Cell"

#define ERRCODE 2
#define ERR(e) {printf("Error: %s\n", nc_strerror(e)); exit(ERRCODE);}

//For test program.
//#define NDIMS 2
#define NX 2
#define NY 12

int main(int argc, char **argv){

  //*****MPI stuff*****
  int size, rank;
  //MPI_Init(&argc, &argv);
  //MPI_Comm_size();
  //MPI_Comm_rank();
  //stuff.

  //*****NetCDF stuff*****

  //Compound type struct
  struct gridcell{
    int year;
    int Nitrogen;

  };
  int compoundtypeid;

  //Basic definitions
  int ncid;//for file
  int cid_dimid, time_dimid; //dimensions
  int varid; //for something? this might be Nid...
//  int Nid;
  // int retval;//for error handling - need to do


     /* When we create netCDF variables and dimensions, we get back an
 *     * ID for each one. */
   int dimids[NDIMS];


   /* This is the data array we will write. It will be filled with a
 *     * progression of numbers for this example. */
   int data_out[NX][NCHTS];
   /* Loop indexes, and error handling. */
   int x, y, retval;


  // int Ns[NCHTS];

  // for(int ii=0; ii<NCHTS; ii++){
  //   Ns[ii] = ii+ii;
  // }

      /* Create some pretend data. If this wasn't an example program, we
 *     * would have some real data to write, for example, model
 *         * output. */
   for (x = 0; x < NX; x++)
      for (y = 0; y < NCHTS; y++)
         data_out[x][y] = x * NCHTS + y;


   /* Always check the return code of every netCDF function call. In
 *     * this example program, any retval which is not equal to NC_NOERR
 *         * (0) will cause the program to print an error message and exit
 *             * with a non-zero return code. */
   /* Create the file. The NC_CLOBBER parameter tells netCDF to
 *     * overwrite this file, if it already exists.*/
   if ((retval = nc_create(FILE_NAME, NC_CLOBBER|NC_NETCDF4, &ncid)))
      ERR(retval);


  //Compound type definition
//  if ((retval = nc_def_compound(ncid, sizeof(struct gridcell), CELL, &compoundtypeid)));
//    ERR(retval);


  // //Define dimensions - will return ID for each
  // if((retval = nc_def_dim(ncid, "cID", NCHTS, &cid_dimid)))
  //   ERR(retval);
  // if((retval = nc_def_dim(ncid, "time", NC_UNLIMITED, &time_dimid)))
  //   ERR(retval);

   /* Define the dimensions. NetCDF will hand back an ID for each. */
   if ((retval = nc_def_dim(ncid, "time", NX, &time_dimid)))
      ERR(retval);
   if ((retval = nc_def_dim(ncid, "cID", NY, &cid_dimid)))
      ERR(retval);



  // dimids[0] = time_dimid;
  // dimids[1] = cid_dimid;
   /* The dimids array is used to pass the IDs of the dimensions of
 *     * the variable. */
   dimids[0] = time_dimid;
   dimids[1] = cid_dimid;



  // if((retval = nc_def_var(ncid, "Npool", NC_INT, NDIMS, dimids, &Nid)))
  //   ERR(retval);
   /* Define the variable. The type of the variable in this case is
 *     * NC_INT (4-byte integer). */
   if ((retval = nc_def_var(ncid, "data", NC_INT, NDIMS, 
                            dimids, &varid)))
      ERR(retval);

    
  //End metadata definition
    if((retval = nc_enddef(ncid)))
      ERR(retval);


  // if((retval = nc_put_var_float(ncid, Nid, &Ns[0][0])))
  //   ERR(retval);
   /* Write the pretend data to the file. Although netCDF supports
 *     * reading and writing subsets of data, in this case we write all
 *         * the data in one operation. */
   if ((retval = nc_put_var_int(ncid, varid, &data_out[0][0])))
      ERR(retval);



   /* Close the file. This frees up any internal netCDF resources
 *     * associated with the file, and flushes any buffers. */
   if ((retval = nc_close(ncid)))
      ERR(retval);


   printf("*** SUCCESS writing test.nc!\n");
   return 0;

}



