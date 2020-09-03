
#include <grass/gis.h>
#include <grass/raster.h>

#define is_cv_null(i,j)         Rast_is_d_null_value(&cv(i,j))
#define is_atbv_null(i,j)       Rast_is_d_null_value(&atbv(i,j))

#define ZERO                    0.0000001

#ifdef _MAIN_C_
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL char *elevname, *popname, *rivername, *outname;
GLOBAL struct Cell_head window;
GLOBAL DCELL **elev, **pop, **out;
GLOBAL DCELL **river; /* need to adapt read_map if we make this memory saving CELL */

void read_cells(void);
void write_cells(void);
