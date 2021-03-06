/* PSPP - a program for statistical analysis.
   Copyright (C) 2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include <config.h>

#include "output/charts/pp.h"

#include <stdlib.h>

#include "libpspp/cast.h"
#include "libpspp/str.h"
#include "libpspp/array.h"
#include "output/chart-item-provider.h"

#include "data/variable.h"
#include "data/case.h"
#include "data/casereader.h"

#include "gl/xalloc.h"
#include <gsl/gsl_cdf.h>
#include "data/variable.h"
#include "language/stats/freq.h"


#define X_LABEL "Observed value"
#define X_LABEL_DETRENDED "Observed value"
#define Y_LABEL "Expected value"
#define Y_LABEL_DETRENDED "Deviation from normal"

/* Constants for distribution params */
#define NORMAL_PARAMS_NUM 2
#define NORMAL_MEAN 0
#define NORMAL_VAR 1

enum
{
	NORMAL
};

struct pp_chart *
pp_chart_create (struct casereader *reader,
	   	 const char *xlabel, const char *xlabel_detrended,
	   	 const char *ylabel, const char *ylabel_detrended,
	   	 const struct variable *byvar,
	   	 const char *label,
		 const int distribution,
		 const double *distribution_params,
		 const int value_num,
	   	 double xmin, double xmax)
{
  struct pp_chart *ppc;

  ppc = xzalloc (sizeof *ppc);
  chart_item_init (&ppc->chart_item, &pp_chart_class, label);
  ppc->data = reader;

  ppc->x_min = xmin;
  ppc->x_max = xmax;
  
  ppc->value_num = value_num;

  ppc->distribution = distribution;
  ppc->distribution_params = distribution_params;
  ppc->distribution_percentages = xzalloc (sizeof(double) * value_num);
  ppc->distribution_percentiles = xzalloc (sizeof(double) * value_num);

  ppc->cfd_values = xzalloc (sizeof(double) * value_num);
  g_print("Create\n");
  calculate_distribution_percentiles_pp (ppc);
  calculate_distribution_percentages (ppc);
  ppc->deviation = xzalloc (sizeof(double) * value_num);
  calculate_deviation_cfd_values_pp(ppc);

  ppc->y_min = 0;
  ppc->y_max = ppc->cfd_values[ppc->value_num - 1];

  ppc->draw_detrended = false;

  ppc->xlabel = xstrdup (xlabel);
  ppc->xlabel_detrended = xstrdup (xlabel_detrended);
  ppc->ylabel = xstrdup (ylabel);
  ppc->ylabel_detrended = xstrdup (ylabel_detrended);
  ppc->byvar = byvar != NULL ? var_clone (byvar) : NULL;

  return ppc;
}

static void
pp_chart_destroy (struct chart_item *chart_item)
{
  struct pp_chart *pp = to_pp_chart (chart_item);
  casereader_destroy (pp->data);
  free (pp->xlabel);
  free (pp->xlabel_detrended);
  free (pp->ylabel);
  free (pp->ylabel_detrended);
  if (pp->byvar)
    var_destroy (pp->byvar);
  free (pp);
}

const struct chart_item_class pp_chart_class =
  {
    pp_chart_destroy
  };

void
calculate_distribution_percentages(struct pp_chart *ppc)
{
  for (int i = 1; i <= ppc->value_num; i++)
	  ppc->distribution_percentages[i-1] = (i-0.5) / ppc->value_num;
}

void 
calculate_deviation_cfd_values_pp(struct pp_chart *ppc)
{
	
  struct casereader *data = casereader_clone (ppc->data);
  g_print("data: %d\n",ppc->data);
  struct ccase *c;
  int i = 0;
  float mean = ppc->distribution_params[NORMAL_MEAN];
  float variance = ppc->distribution_params[NORMAL_VAR];

  for (; (c = casereader_read (data)) != NULL; case_unref (c))
    {
      float x = case_data_idx (c, 0)->f;
      ppc->cfd_values[i] = mean + gsl_cdf_gaussian_P(x, variance);
      ppc->deviation[i] =  ppc->cfd_values[i] - ppc->distribution_percentiles[i++];

    }
  casereader_destroy (data);  
}
void calculate_distribution_percentiles_pp (struct pp_chart *qqc)
{
	switch (qqc->distribution)
	{
		case NORMAL:
			calculate_normal_percentiles(qqc->distribution_percentiles, qqc->value_num, qqc->distribution_params[NORMAL_MEAN], qqc->distribution_params[NORMAL_VAR]);
			break;
		default:
			break;
	}
}
