/* PSPPIRE - a graphical user interface for PSPP.
   Copyright (C) 2015  Free Software Foundation

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

#include "psppire-dialog-action-qq-plot.h"
#include "psppire-value-entry.h"

#include "dialog-common.h"
#include <ui/syntax-gen.h>
#include "psppire-var-view.h"

#include "psppire-dialog.h"
#include "builder-wrapper.h"

#include "psppire-dict.h"
#include "libpspp/str.h"

#include "language/stats/chart-category.h"

static void
psppire_dialog_action_qq_plot_class_init (PsppireDialogActionQQPlotClass *class);

G_DEFINE_TYPE (PsppireDialogActionQQPlot, psppire_dialog_action_qq_plot, PSPPIRE_TYPE_DIALOG_ACTION);

static gboolean
dialog_state_valid (gpointer rd_)
{ 
 PsppireDialogActionQQPlot *rd = PSPPIRE_DIALOG_ACTION_QQ_PLOT (rd_);

  if (0 == g_strcmp0 ("", gtk_entry_get_text(GTK_ENTRY (rd->parameter1_entry))) && 0 == g_strcmp0 ("", gtk_entry_get_text(GTK_ENTRY (rd->parameter2_entry))))
  {
	return FALSE;
  }

  return TRUE;
}

static void
populate_combo_model (GtkComboBox *cb)
{
  int i;
  GtkListStore *list =  gtk_list_store_new (4, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT);
  GtkTreeIter iter;
  GtkCellRenderer *renderer ;

	//nekad ce mozda biti potrebno da se izdvoji kao resurs combobox podaci, ali za sad neka bude ovako
  gtk_list_store_append (list, &iter);
  gtk_list_store_set (list, &iter, 0, "Normal", 1, "Normal", -1); //Normal distribution combobox
      
  renderer = gtk_cell_renderer_text_new ();
  gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (cb), renderer, FALSE);

  gtk_cell_layout_add_attribute (GTK_CELL_LAYOUT (cb), renderer, "text", 0);

  gtk_combo_box_set_model (GTK_COMBO_BOX (cb), GTK_TREE_MODEL (list));
  g_object_unref (list);
}

static void
refresh (PsppireDialogAction *rd_)
{
  // This function is called when the Reset Button is clicked.
  // It sets the dialog to its default state
  
  PsppireDialogActionQQPlot *rd = PSPPIRE_DIALOG_ACTION_QQ_PLOT (rd_);
  

  gtk_entry_set_text (GTK_ENTRY (rd->parameter1_entry), "");
  gtk_entry_set_text (GTK_ENTRY (rd->parameter2_entry), "");
  
  gtk_toggle_button_set_active (GTK_CHECK_BUTTON (rd->estimate_from_data_checkbox), TRUE);
  
  gtk_widget_set_sensitive  (rd->parameter1_entry, FALSE);
  gtk_widget_set_sensitive  (rd->parameter2_entry, FALSE);
  
  populate_combo_model (GTK_COMBO_BOX(rd->distribution_combobox));
   
  gtk_combo_box_set_active (GTK_COMBO_BOX (rd->distribution_combobox), 0);
  
  
}

static void
qq_plot_ok_onclick (GtkButton* ok, PsppireDialogActionQQPlot *act)
{
	g_print("KLIK NA OK \n");		
}

static void
qq_plot_reset_onclick (GtkButton* reset, PsppireDialogActionQQPlot *act)
{
	g_print("KLIK NA RESET \n");
}

static void
qq_plot_cancel_onclick (GtkButton* cancel, PsppireDialogActionQQPlot *act)
{
	g_print("KLIK NA CANCEL \n");
}

static void
qq_plot_estimate_data_toggled (GtkCheckButton *estimate_from_data_checkbox, PsppireDialogActionQQPlot *rd)
{
	if(gtk_toggle_button_get_active(GTK_CHECK_BUTTON(rd->estimate_from_data_checkbox))){
		
		gtk_widget_set_sensitive  (rd->parameter1_entry, FALSE);
		gtk_widget_set_sensitive  (rd->parameter2_entry, FALSE);
		
	}else {
		
		gtk_widget_set_sensitive  (rd->parameter1_entry, TRUE);
		gtk_widget_set_sensitive  (rd->parameter2_entry, TRUE);
		
	}
}



static void
psppire_dialog_action_qq_plot_activate (PsppireDialogAction *a)
{
  PsppireDialogActionQQPlot *act = PSPPIRE_DIALOG_ACTION_QQ_PLOT (a);
  PsppireDialogAction *pda = PSPPIRE_DIALOG_ACTION (a);

  GHashTable *thing = psppire_dialog_action_get_hash_table (pda);
  GtkBuilder *xml = g_hash_table_lookup (thing, a);
  
  
  GtkTreeViewColumn *column;
  GtkCellRenderer *renderer;
  
  
  if (!xml)
    {
      xml = gtk_builder_new_from_file("qq-plot.ui");
      g_hash_table_insert (thing, a, xml);
	}
	
      pda->dialog = get_widget_assert (xml, "qq-plot-dialog");
	  pda->source = get_widget_assert   (xml, "psppire-dictview1");
	

      act->selected_variables_treeview = get_widget_assert (xml, "treeview-selected-variables");
	  
	  act->estimate_from_data_checkbox = get_widget_assert (xml, "estimate-from-data-checkbox");
     
       
      act->parameter1_entry=get_widget_assert(xml,"parameter1-entry");
      act->parameter2_entry=get_widget_assert(xml,"parameter2-entry");
     
     
       
      act->qq_plot_ok=get_widget_assert(xml,"qq-plot-ok-button");
      act->qq_plot_cancel=get_widget_assert(xml,"qq-plot-cancel-button");
      act->qq_plot_reset=get_widget_assert(xml,"qq-plot-reset-button");
      
      act->distribution_combobox=get_widget_assert(xml,"distribution-combobox");
      
      
      gtk_entry_set_visibility (GTK_ENTRY(act->parameter1_entry), true);
      gtk_entry_set_input_purpose(GTK_ENTRY(act->parameter1_entry), GTK_INPUT_PURPOSE_NUMBER);
      
      gtk_entry_set_visibility (GTK_ENTRY(act->parameter2_entry), true);
      gtk_entry_set_input_purpose(GTK_ENTRY(act->parameter2_entry), GTK_INPUT_PURPOSE_NUMBER);
      
      
  
      g_signal_connect(act->qq_plot_ok, "clicked", G_CALLBACK (qq_plot_ok_onclick), pda);
	  g_signal_connect(act->qq_plot_reset, "clicked", G_CALLBACK (qq_plot_reset_onclick), pda);
	  g_signal_connect(act->qq_plot_cancel, "clicked", G_CALLBACK (qq_plot_cancel_onclick), pda);

	  g_signal_connect(act->estimate_from_data_checkbox, "toggled", G_CALLBACK (qq_plot_estimate_data_toggled), act);
      
    
            
      psppire_dialog_action_set_refresh (pda, refresh);

      psppire_dialog_action_set_valid_predicate (pda, dialog_state_valid);
						 
	  renderer = gtk_cell_renderer_text_new ();

      column = gtk_tree_view_column_new_with_attributes ("Values", renderer, "text", 0, NULL);
      
      

      //gtk_tree_view_append_column (GTK_TREE_VIEW (act->percentiles_treeview), column);
    
}

static char *
generate_syntax (const PsppireDialogAction *a)
{
 return NULL;
}

static void
psppire_dialog_action_qq_plot_class_init (PsppireDialogActionQQPlotClass *class)
{
  psppire_dialog_action_set_activation (class, psppire_dialog_action_qq_plot_activate);

  PSPPIRE_DIALOG_ACTION_CLASS (class)->generate_syntax = generate_syntax;
}


static void
psppire_dialog_action_qq_plot_init (PsppireDialogActionQQPlot *act)
{
}