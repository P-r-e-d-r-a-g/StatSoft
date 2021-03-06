/* PSPP - a program for statistical analysis.
   Copyright (C) 2004, 2008, 2009, 2011 Free Software Foundation, Inc.

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

#ifndef OUTPUT_CHARTS_NP_PLOT_H
#define OUTPUT_CHARTS_NP_PLOT_H 1

#include "output/chart-item.h"

struct np;

/* An NP or DNP plot. */
struct np_plot_chart
  {
    struct chart_item chart_item;
    struct casereader *data;
    bool detrended;

    /* Copied directly from struct np. */
    double y_min, y_max;
    double dns_min, dns_max;

    /* Calculated. */
    double slope, intercept;
    double y_first, y_last;
    double x_lower, x_upper;
    double slack;
  };

struct chart_item *np_plot_create (const struct np *,
                                   const struct casereader *,
                                   const char *label);
struct chart_item *dnp_plot_create (const struct np *,
                                    const struct casereader *,
                                    const char *label);

/* This boilerplate for np_plot_chart, a subclass of chart_item, was
   autogenerated by mk-class-boilerplate. */

#include <assert.h>
#include "libpspp/cast.h"

extern const struct chart_item_class np_plot_chart_class;

/* Returns true if SUPER is a np_plot_chart, otherwise false. */
static inline bool
is_np_plot_chart (const struct chart_item *super)
{
  return super->class == &np_plot_chart_class;
}

/* Returns SUPER converted to np_plot_chart.  SUPER must be a np_plot_chart, as
   reported by is_np_plot_chart. */
static inline struct np_plot_chart *
to_np_plot_chart (const struct chart_item *super)
{
  assert (is_np_plot_chart (super));
  return UP_CAST (super, struct np_plot_chart, chart_item);
}

/* Returns INSTANCE converted to chart_item. */
static inline struct chart_item *
np_plot_chart_super (const struct np_plot_chart *instance)
{
  return CONST_CAST (struct chart_item *, &instance->chart_item);
}

/* Increments INSTANCE's reference count and returns INSTANCE. */
static inline struct np_plot_chart *
np_plot_chart_ref (const struct np_plot_chart *instance)
{
  return to_np_plot_chart (chart_item_ref (&instance->chart_item));
}

/* Decrements INSTANCE's reference count, then destroys INSTANCE if
   the reference count is now zero. */
static inline void
np_plot_chart_unref (struct np_plot_chart *instance)
{
  chart_item_unref (&instance->chart_item);
}

/* Returns true if INSTANCE's reference count is greater than 1,
   false otherwise. */
static inline bool
np_plot_chart_is_shared (const struct np_plot_chart *instance)
{
  return chart_item_is_shared (&instance->chart_item);
}

static inline void
np_plot_chart_submit (struct np_plot_chart *instance)
{
  chart_item_submit (&instance->chart_item);
}

#endif /* output/charts/np-plot.h */
