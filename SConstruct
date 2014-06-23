


#stuff


libs = Split("""netcdf_c++
                hdf5
                hdf5_hl
                netcdf""")

#if doesn't work, try adding 'z' 'm' and 'sz' to libs.

src_files = "parNC.cpp"

compiler = 'g++'

object_list = Object(src_files, CXX=compiler);

Program('parNC', object_list, CXX=compiler, LIBS=libs);

