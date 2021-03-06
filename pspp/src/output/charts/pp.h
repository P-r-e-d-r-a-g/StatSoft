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

#ifndef PP_H
#define PP_H

#include "libpspp/str.h"
#include "libpspp/hmap.h"
#include "data/value.h"
#include "output/chart-item.h"


struct pp_chart
  {
    struct chart_item chart_item;
    struct casereader *data;
    struct variable *byvar;
    char *xlabel, *xlabel_detrended;
    char *ylabel, *ylabel_detrended;

    int value_num;

    int distribution;
    double *distribution_params;
    double *distribution_percentages;
    double *distribution_percentiles;
    double *cfd_values;
    double *deviation;

    bool draw_detrended;

    double y_min, y_max;
    double x_min, x_max;
  };


struct variable;
struct freq;

struct pp_chart *pp_chart_create (struct casereader *reader,
	   	      		  const char *xlabel, const char *xlabel_detrended,
	   	      		  const char *ylabel, const char *ylabel_detrended,
	   	      		  const struct variable *byvar,
	   	      		  const char *label,
		      		  const int distribution,
		      		  const double *distribution_params,
		      		  const int value_num,
	   	      		  double ymin, double ymax);

/* This boilerplate for pp, a subclass of chart_item, was
   autogenerated by mk-class-boilerplate. */

#include <assert.h>
#include "libpspp/cast.h"

extern const struct chart_item_class pp_chart_class;

/* Returns true if SUPER is a pp, otherwise false. */
static inline bool
is_pp_chart (const struct chart_item *super)
{
  return super->class == &pp_chart_class;
}

/* Returns SUPER converted to pp.  SUPER must be a pp, as
   reported by is_pp. */
static inline struct pp_chart *
to_pp_chart (const struct chart_item *super)
{
  assert (is_pp_chart (super));
  return UP_CAST (super, struct pp_chart, chart_item);
}

/* Returns INSTANCE converted to chart_item. */
static inline struct chart_item *
pp_chart_super (const struct pp_chart *instance)
{
  return CONST_CAST (struct chart_item *, &instance->chart_item);
}

/* Increments INSTANCE's reference count and returns INSTANCE. */
static inline struct pp_chart *
pp_chart_ref (const struct pp_chart *instance)
{
  return to_pp (chart_item_ref (&instance->chart_item));
}

/* Decrements INSTANCE's reference count, then destroys INSTANCE if
   the reference count is now zero. */
static inline void
pp_chart_unref (struct pp_chart *instance)
{
  chart_item_unref (&instance->chart_item);
}

/* Returns true if INSTANCE's reference count is greater than 1,
   false otherwise. */
static inline bool
pp_chart_is_shared (const struct pp_chart *instance)
{
  return chart_item_is_shared (&instance->chart_item);
}

static inline void
pp_chart_submit (struct pp_chart *instance)
{
  chart_item_submit (&instance->chart_item);
}

#endif /* output/charts/pp.h */
