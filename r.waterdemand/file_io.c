#include <string.h>
#include <grass/gis.h>
#include <grass/raster.h>
#include <grass/glocale.h>
#include "global.h"

void read_cells(void)
{
    int fd_elev, fd_pop, fd_river, i, j;
    RASTER_MAP_TYPE data_type;
    CELL *ccell = NULL;
    FCELL *fcell = NULL;
    struct Cell_head inhead;
    char buf_wrns[32], buf_wrew[32], buf_mrns[32], buf_mrew[32];

    fd_elev  = Rast_open_old(elevname, "");
    fd_pop   = Rast_open_old(popname, "");
    fd_river = Rast_open_old(rivername, "");

    elev  = (DCELL **) G_malloc(sizeof(DCELL *) * window.rows);
    pop   = (DCELL **) G_malloc(sizeof(DCELL *) * window.rows);
    river = (DCELL **) G_malloc(sizeof(DCELL *) * window.rows);

    // not sure how this works. I assume that the resolution will be the same for the other input maps too?

    data_type = Rast_get_map_type(fd_elev);   
    Rast_get_cellhd(elevname, "", &inhead);

    if (window.ew_res < inhead.ew_res || window.ns_res < inhead.ns_res) {
	G_format_resolution(window.ew_res, buf_wrew, G_projection());
	G_format_resolution(window.ns_res, buf_wrns, G_projection());
	G_format_resolution(inhead.ew_res, buf_mrew, G_projection());
	G_format_resolution(inhead.ns_res, buf_mrns, G_projection());
	G_fatal_error(_("The current region resolution [%s x %s] is finer "
			"than the input map's resolution [%s x %s]. "
			"The current region resolution must be identical "
			"to, or coarser than, the input map's resolution."),
		      buf_wrew, buf_wrns, buf_mrew, buf_mrns);
    }


    // Read each map...

    // 1. elev

    if (data_type == CELL_TYPE)
	ccell = (CELL *) G_malloc(sizeof(CELL) * window.cols);
    else if (data_type == FCELL_TYPE)
	fcell = (FCELL *) G_malloc(sizeof(FCELL) * window.cols);

    G_message(_("Reading elevation map..."));

    for (i = 0; i < window.rows; i++) {
	G_percent(i, window.rows, 2);

	elev[i] = (DCELL *) G_malloc(sizeof(DCELL) * window.cols);

	if (data_type == CELL_TYPE) {
	    Rast_get_c_row(fd_elev, ccell, i);
	    for (j = 0; j < window.cols; j++) {
		if (Rast_is_c_null_value(&ccell[j]))
		    Rast_set_d_null_value(&elev[i][j], 1);
		else
		    elev[i][j] = (DCELL) ccell[j];
	    }
	}
	else if (data_type == FCELL_TYPE) {
	    Rast_get_f_row(fd_elev, fcell, i);
	    for (j = 0; j < window.cols; j++) {
		if (Rast_is_f_null_value(&fcell[j]))
		    Rast_set_d_null_value(&elev[i][j], 1);
		else
		    elev[i][j] = (DCELL) fcell[j];
	    }
	}
	else
	    Rast_get_d_row(fd_elev, elev[i], i);
    }
    if (data_type == CELL_TYPE)
	G_free(ccell);
    else if (data_type == FCELL_TYPE)
	G_free(fcell);
    G_percent(i, window.rows, 2);
    Rast_close(fd_elev);

    // 2. pop

    data_type = Rast_get_map_type(fd_pop);   
    Rast_get_cellhd(popname, "", &inhead);

    if (data_type == CELL_TYPE)
	ccell = (CELL *) G_malloc(sizeof(CELL) * window.cols);
    else if (data_type == FCELL_TYPE)
	fcell = (FCELL *) G_malloc(sizeof(FCELL) * window.cols);

    G_message(_("Reading popation map..."));

    for (i = 0; i < window.rows; i++) {
	G_percent(i, window.rows, 2);

	pop[i] = (DCELL *) G_malloc(sizeof(DCELL) * window.cols);

	if (data_type == CELL_TYPE) {
	    Rast_get_c_row(fd_pop, ccell, i);
	    for (j = 0; j < window.cols; j++) {
		if (Rast_is_c_null_value(&ccell[j]))
		    Rast_set_d_null_value(&pop[i][j], 1);
		else
		    pop[i][j] = (DCELL) ccell[j];
	    }
	}
	else if (data_type == FCELL_TYPE) {
	    Rast_get_f_row(fd_pop, fcell, i);
	    for (j = 0; j < window.cols; j++) {
		if (Rast_is_f_null_value(&fcell[j]))
		    Rast_set_d_null_value(&pop[i][j], 1);
		else
		    pop[i][j] = (DCELL) fcell[j];
	    }
	}
	else
	    Rast_get_d_row(fd_pop, pop[i], i);
    }
    if (data_type == CELL_TYPE)
	G_free(ccell);
    else if (data_type == FCELL_TYPE)
	G_free(fcell);
    G_percent(i, window.rows, 2);
    Rast_close(fd_pop);

    // 3. river

    data_type = Rast_get_map_type(fd_river);   
    Rast_get_cellhd(rivername, "", &inhead);

    if (data_type == CELL_TYPE)
	ccell = (CELL *) G_malloc(sizeof(CELL) * window.cols);
    else if (data_type == FCELL_TYPE)
	fcell = (FCELL *) G_malloc(sizeof(FCELL) * window.cols);

    G_message(_("Reading riveration map..."));

    for (i = 0; i < window.rows; i++) {
	G_percent(i, window.rows, 2);

	river[i] = (DCELL *) G_malloc(sizeof(DCELL) * window.cols);

	if (data_type == CELL_TYPE) {
	    Rast_get_c_row(fd_river, ccell, i);
	    for (j = 0; j < window.cols; j++) {
		if (Rast_is_c_null_value(&ccell[j]))
		    Rast_set_d_null_value(&river[i][j], 1);
		else
		    river[i][j] = (DCELL) ccell[j];
	    }
	}
	else if (data_type == FCELL_TYPE) {
	    Rast_get_f_row(fd_river, fcell, i);
	    for (j = 0; j < window.cols; j++) {
		if (Rast_is_f_null_value(&fcell[j]))
		    Rast_set_d_null_value(&river[i][j], 1);
		else
		    river[i][j] = (DCELL) fcell[j];
	    }
	}
	else
	    Rast_get_d_row(fd_river, river[i], i);
    }
    if (data_type == CELL_TYPE)
	G_free(ccell);
    else if (data_type == FCELL_TYPE)
	G_free(fcell);
    G_percent(i, window.rows, 2);
    Rast_close(fd_river);

}

void write_cells(void)
{
    int fd, i;
    struct History history;

    fd = Rast_open_new(outname, DCELL_TYPE);

    G_message(_("Writing output map..."));

    for (i = 0; i < window.rows; i++) {
	G_percent(i, window.rows, 2);
	Rast_put_d_row(fd, out[i]);
    }
    G_percent(i, window.rows, 2);
    Rast_close(fd);

    Rast_short_history(outname, "raster", &history);
    Rast_set_history(&history, HIST_DATSRC_1, elevname);
    Rast_write_history(outname, &history);
}

