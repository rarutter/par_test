
#stuff

libs = Split("""netcdf_c++
                hdf5
                hdf5_hl
                mpi_cxx
                mpi
                netcdf""")

#if doesn't work, try adding 'z' 'm' and 'sz' to libs.

platform_include_path = ['/usr/include',
                         '/usr/include/openmpi-x86_64',
                         '/home/ruth/development/netcdf-c/include',
                         '~/usr/local/include']

library_path = ['/usr/lib64','~/usr/local/lib']

src_files = "parNC.cpp"
#src_files = "tst_compounds.c"

compiler = '/usr/lib64/openmpi/bin/mpic++'
#compiler = 'g++'

compiler_flags = '-Werror -ansi -g -fPIC';
#compiler_flags = compiler_flags + ' -m64 -DWITHMPI';

object_list = Object(src_files, CXX=compiler, CPPPATH=platform_include_path,
	                 CPPFLAGS=compiler_flags);

Program('parNC', object_list, CXX=compiler, CPPPATH=platform_include_path,
	     LIBS=libs);

