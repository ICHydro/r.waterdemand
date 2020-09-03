
/****************************************************************************
 *
 * MODULE:       r.waterdemand
 *
 * AUTHOR(S):    Wouter Buytaert, Imperial College London
 *               reusing routines from the r.topidx module written by
 *               Huidae Cho, Hydro Laboratory, Kyungpook National University
 *
 * PURPOSE:      Allocates population to nearest upstream river cell.
 *
 * COPYRIGHT:    (C) Wouter Buytaert & GRASS Development Team
 *
 *               This program is free software under the GNU General Public
 *               License (>=v2). Read the file COPYING that comes with GRASS
 *               for details.
 *
 *****************************************************************************/

#define _MAIN_C_
#include <math.h>
#include <string.h>
#include <stdlib.h>
//#include <unistd.h>
#include <grass/gis.h>
#include <grass/glocale.h>
#include "global.h"

int main(int argc, char **argv)
{
    struct GModule *module;

    struct
    {
        struct Option *elevmap;
        struct Option *rivermap;
	struct Option *popmap;
        struct Option *output;
    } params;

    int    loc[2];
    int    i, j, ii, jj, counter;
    double dist, distlimit, dx, dy, mindist;
    int    radius;

    G_gisinit(argv[0]);


    module = G_define_module();
    G_add_keyword(_("raster"));
    G_add_keyword(_("hydrology"));
    module->description =
	_("Allocates population density to nearest upstream river cell.");

    params.elevmap = G_define_standard_option(G_OPT_R_ELEV);
    params.elevmap->description = _("Input elevation map");
    
    params.rivermap = G_define_standard_option(G_OPT_R_MAP);
    params.rivermap->key = "river";
    params.rivermap->description = _("Input river map");

    params.popmap = G_define_standard_option(G_OPT_R_MAP);
    params.popmap->key = "pop";
    params.popmap->description = _("Input population map");

    params.output = G_define_standard_option(G_OPT_R_OUTPUT);
    params.output->key = "output";
    params.output->description = _("Output water extraction map");

    if (G_parser(argc, argv))
	exit(EXIT_FAILURE);

    elevname = params.elevmap->answer;
    rivername = params.rivermap->answer;
    popname = params.popmap->answer;
    outname = params.output->answer;

    G_get_window(&window);

    read_cells();

    /* allocate memory for output map */
    out = (DCELL **) G_malloc(sizeof(DCELL *) * window.rows);
    for (i = 0; i < window.rows; i++) {
	out[i] = (DCELL *) G_malloc(sizeof(DCELL) * window.cols);
    }

    /* do calculations */

    /* copy population map to output. Will then be reshuffled */
    for(i = 0; i < window.rows; i++){
      for(j = 0; j < window.cols; j++){
	out[i][j] = pop[i][j];
      }
    }

    /* continue sweeping until all cells are done: */
    counter = 1;
    distlimit = 20000; /* utm projection */
    if ((G_projection() == PROJECTION_LL)) distlimit = 0.2; /* latlong projection */

    dx = window.ew_res;
    dy = window.ns_res;
//    radius = ceil(distlimit / dx);   /* ceiling, suppose dx == dy */
    radius = (int) (1 + distlimit / dx);
    printf("distlimit = %f, dx = %f, dy = %f\n", distlimit, dx, dy);
    printf("radius = %d\n", radius);

    for(i = 0; i < window.rows; i++){
      for(j = 0; j < window.cols; j++){
	/* for cell with population and not being a river cell: */
        if((pop[i][j] > ZERO) && (river[i][j] == 0)) {
  	  /* look for closest river cell with higher elevation */
	  mindist = distlimit;
          /* new routine: limit search to distlimit radius boundingbox */
          for(ii = -radius; ii <= radius; ii++){
    	    for(jj = -radius; jj <= radius; jj++){
              if(((i + ii) >= 0) && ((j + jj) >= 0) &&
		 ((i + ii) < window.rows) && ((j + jj) < window.cols)) {
		if((river[i + ii][j + jj] > 0) && elev[i + ii][j + jj] >= elev[i][j]) {
	          dist = sqrt(pow(dx * jj, 2) + pow(dy * ii, 2));
		  if(dist < mindist) {
		    mindist = dist;
		    loc[0] = i + ii;
		    loc[1] = j + jj;
		  }
		}
	      }
	    }
	  }
	  /* if a cell has been found within the distance limit */
      	  if(mindist < distlimit) {
	    /* move population to rivercell */
	    out[loc[0]][loc[1]] += out[i][j];
	    out[i][j] = 0;
	  }
	}
      }
    }

    /* write output map */

    write_cells();

    /* close */

    for (i = 0; i < window.rows; i++) {
	G_free(elev[i]);
	G_free(river[i]);
	G_free(pop[i]);
        G_free(out[i]);
    }
    G_free(elev);
    G_free(river);
    G_free(pop);
    G_free(out);

    exit(EXIT_SUCCESS);
}
