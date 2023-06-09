#include <gtk/gtk.h>
#include <stdio.h>

gint totalPax = 1;
gdouble billAmount = 0.00;
gint tipDefault = 20;
gdouble tipPercentage = 20;

GtkWidget *youPayDisplay;
GtkWidget *totalPaxEntry;
GtkWidget *billAmountEntry;
GtkWidget *tipPercentageEntry;
GtkWidget *tipScale;

/* Exit button pressed. Displays exit popup dialog.
   Yes or No
   Yes closes the whole app, no closes dialog window */
void exitButtonClicked(__attribute__((unused)) GtkButton *button, 
                       gpointer   user_data)
{
    g_print("Exit button Clicked\n");

    GtkWidget  *dialog;
    GtkWidget  *dialogLabel;
    dialog = GTK_WIDGET(gtk_builder_get_object (user_data, "dialog"));
    dialogLabel = GTK_WIDGET(gtk_builder_get_object (user_data, "dialogLabel"));

    gtk_widget_show_all(dialog);

    // Thanks if tips more than or equal to 20%, reconsider if less than 20
    if (tipPercentage >= 20) {
        gtk_label_set_text(GTK_LABEL(dialogLabel), "Thanks for tipping :) \nAre you sure you want to quit?\n");
    }else {
        gtk_label_set_text(GTK_LABEL(dialogLabel), "Please consider tipping more :(\nAre you sure you want to quit?\n");
    }

    int response = gtk_dialog_run(GTK_DIALOG(dialog));

    switch (response)
    {
        case GTK_RESPONSE_YES:
            gtk_main_quit();
            break;

        case GTK_RESPONSE_NO:
            break;

        default:
            break;
    }

    gtk_widget_hide(dialog);

    g_print ("Response is %s\n", response == -8 ? "Yes" : "No");
} 


/* Calculates the final result and display in  */
void calculateClicked(__attribute__((unused)) GtkWidget *widget, 
                      __attribute__((unused)) gpointer   data)
{
    g_print("Calculate button Clicked\n");

    gdouble total = 0.00;

    // Calculate total bill amount after adding tips //
    total = billAmount * (1 + (tipPercentage/100));

    g_print("Bill Amount: $%.2f\n", billAmount);
    g_print("Total: $%.2f\n", total);

    // Convert total amount to string with 2 decimal points //
    gchar totalStr[100];
    sprintf(totalStr, "%.2f", total);
    
    // Calculate the amount user has to pay //
    gdouble youPay = total/totalPax;

    // Convert user pay amount to string with 2 decimal points //
    gchar youPayStr[100];
    sprintf(youPayStr, "%.2f", youPay);

    // Display user pay amount on youPayDisplay //
    gtk_label_set_text(GTK_LABEL(youPayDisplay), youPayStr);

    char buffer[100];
    g_snprintf(buffer, sizeof(buffer), "Total: $%.2f               You Pay: $%.2f\n", total, youPay);
    gtk_label_set_text(GTK_LABEL(youPayDisplay), buffer);
    g_print("%s\n", buffer);
}

/* Opens a window to let user set default tip percentage value
   Apply will change default value and current tip value
   Cancel will not do anything */
void setDefaultTipButtonClicked(__attribute__((unused)) GtkButton *button, 
                       gpointer   user_data)
{
    g_print("Exit button Clicked\n");

    GtkWidget  *setDefaultTipDialog;
    GtkWidget  *setDefaultTipEntry;

    setDefaultTipDialog = GTK_WIDGET(gtk_builder_get_object (user_data, "setDefaultTipDialog"));
    setDefaultTipEntry = GTK_WIDGET(gtk_builder_get_object (user_data, "setDefaultTipEntry"));

    gtk_widget_show_all(setDefaultTipDialog);

    // Apply or Cancel response //
    int response = gtk_dialog_run(GTK_DIALOG(setDefaultTipDialog));

    switch (response)
    {   
        // If apply
        case GTK_RESPONSE_APPLY:

            // Only update if default != 0 //
            if (atof(gtk_entry_get_text(GTK_ENTRY(setDefaultTipEntry))) > 0) {
                // Sets default and current tip percentage to newly set default //
                tipDefault = atof(gtk_entry_get_text(GTK_ENTRY(setDefaultTipEntry)));
                tipPercentage = tipDefault;

                // Convert new entry to string to be shown in tipPercentageEntry //
                gchar tipPercentageStr[3];
                g_ascii_dtostr(tipPercentageStr, 3, tipPercentage);
                gtk_entry_set_text(GTK_ENTRY(tipPercentageEntry), tipPercentageStr);
                g_print("Default tip has been set to %.0d%%\n", tipDefault);
            }

            break;

        case GTK_RESPONSE_CANCEL:
            // Just hide dialog
            break;

        default:
            // do_nothing_since_dialog_was_cancelled ();
            break;
    }

    gtk_widget_hide(setDefaultTipDialog);

    if (response == -10) {
        g_print("APPLY\n");
    }else if (response == -6) {
        g_print("CANCEL\n");
    }

    
} 


/* Resets all entries and displays to starting value */
void resetClicked(__attribute__((unused)) GtkWidget *widget, 
                       __attribute__((unused)) gpointer   data)
{
    g_print("Reset Clicked\n");

    // Set all variables to starting value //
    totalPax = 1;
    billAmount = 0;
    tipPercentage = tipDefault;

    // Set widget displays to 0 or default value //
    gtk_label_set_text(GTK_LABEL(youPayDisplay), "You Pay: $0.00\n");
    gtk_entry_set_text(GTK_ENTRY(totalPaxEntry), "1");
    gtk_entry_set_text(GTK_ENTRY(billAmountEntry), "0.00");

    // Convert default tip percentage to string //
    gchar tipPercentageStr[3];
    g_ascii_dtostr(tipPercentageStr, 3, tipPercentage);

    // Set tip percentage entry and slider to default tip value //
    gtk_entry_set_text(GTK_ENTRY(tipPercentageEntry), tipPercentageStr);
    gtk_range_set_value(GTK_RANGE(tipScale), tipDefault);
    
}


/* Converts input from totalPaxEntry to int and update totalPax value */
void totalPaxEntryChanged(__attribute__((unused)) GtkWidget *widget, 
                       __attribute__((unused)) gpointer   data)
{
    g_print("Total Pax Entry Changed\n");

    totalPax = atoi(gtk_entry_get_text(GTK_ENTRY(totalPaxEntry)));
    g_print("New total pax: %d\n", totalPax);

}


/* Converts input from billAmountEntry to double and update billAmount value */
void billAmountEntryChanged(__attribute__((unused)) GtkWidget *widget, 
                       __attribute__((unused)) gpointer   data)
{
    if (atof(gtk_entry_get_text(GTK_ENTRY(billAmountEntry))) >= 0) {
        g_print("Bill amount entry Changed\n");

        billAmount = atof(gtk_entry_get_text(GTK_ENTRY(billAmountEntry)));
        g_print("New bill amount: $%.2f\n", billAmount);
    }
    
}


/* Converts input from tipPercentageEntry to int and update tipPercentage value 
   Also moves slider to appropriate position */
void tipsPercentageEntryChanged(__attribute__((unused)) GtkWidget *widget, 
                       __attribute__((unused)) gpointer   data)
{
    if (atoi(gtk_entry_get_text(GTK_ENTRY(tipPercentageEntry))) >= 0) {
        g_print("Tips Percentage Entry Changed\n");

        // Convert input from tipPercentageEntry to integer and assign to tipPercentage //
        tipPercentage = atoi(gtk_entry_get_text(GTK_ENTRY(tipPercentageEntry)));

        // If tipPercentage is 0 >= x <= 100, move the slider to correct position //
        if (tipPercentage >= 0 && tipPercentage <= 100) {
            gtk_range_set_value(GTK_RANGE(tipScale), tipPercentage);
        }

        g_print("New tips percentage: %.0f\n", tipPercentage);
    }
    
}


/* Converts input from tipPercentageSlider to int and update tipPercentage value 
   Also changes display on tipPercentage Entry*/
void tipsPercentageSliderChanged(__attribute__((unused)) GtkRange *range, 
                       __attribute__((unused)) gpointer   data)
{   
    if (gtk_range_get_value(GTK_RANGE(tipScale)) >= 0) {
        g_print("Tips Percentage Slider Changed\n");

        // Get value from scale and assign it to tipPercentage //
        tipPercentage = gtk_range_get_value(GTK_RANGE(tipScale));

        // Convert tipPercentage to string and display on tipPercentageEntry //
        gchar tipPercentageStr[4];
        g_ascii_dtostr(tipPercentageStr, 4, tipPercentage);
        gtk_entry_set_text(GTK_ENTRY(tipPercentageEntry), tipPercentageStr);

        g_print("%.0f%% tips given\n", tipPercentage);
    }
    
}


int main(int argc, char *argv[]) {

    GtkWidget *window;
    GtkBuilder *builder = gtk_builder_new();
    
    gtk_init(&argc, &argv);

    gtk_builder_add_from_file(builder, "tips_calculator.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    youPayDisplay = GTK_WIDGET(gtk_builder_get_object(builder, "youPayDisplay"));
    totalPaxEntry = GTK_WIDGET(gtk_builder_get_object(builder, "totalPaxEntry"));
    billAmountEntry = GTK_WIDGET(gtk_builder_get_object(builder, "billAmountEntry"));
    tipPercentageEntry = GTK_WIDGET(gtk_builder_get_object(builder, "tipPercentageEntry"));
    tipScale = GTK_WIDGET(gtk_builder_get_object(builder, "tipScale"));

    gtk_scale_set_has_origin(GTK_SCALE(tipScale), TRUE);
    
    gtk_builder_connect_signals(builder, builder);
 
    gtk_widget_show_all (window);

    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
               
    gtk_main ();
    
    return 0;

}
