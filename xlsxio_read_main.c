#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "xlsxio_read.h"

struct xlsx_list_sheets_data {
  int index;
  char* firstsheet;
};

int xlsx_list_sheets_callback (const char* name, void* callbackdata)
{
  struct xlsx_list_sheets_data* data = (struct xlsx_list_sheets_data*)callbackdata;
  if (!data->firstsheet)
    data->firstsheet = strdup(name);
  printf("%3i: %s\n", data->index, name);
  data->index++;
  return 0;
}

int sheet_row_callback (size_t row, size_t maxcol, void* callbackdata)
{
  printf("\n");
//printf("[%i]", (int)row);/////
  return 0;
}

int sheet_cell_callback (size_t row, size_t col, const char* value, void* callbackdata)
{
  if (col > 1)
    printf("\t");
//printf("[%i,%i]", (int)row, (int)col);/////
  if (value)
    printf("%s", value);
  return 0;
}

int main (int argc, char* argv[])
{
  int i;
  xlsxioreadhandle xlsxioread;
  for (i = 1; i < argc; i++) {
    if ((xlsxioread = xlsxioread_open(argv[i])) != NULL) {
      //list available sheets
      struct xlsx_list_sheets_data sheetdata;
      printf("Available sheets:\n");
      sheetdata.index = 0;
      sheetdata.firstsheet = NULL;
      xlsxioread_list_sheets(xlsxioread, xlsx_list_sheets_callback, &sheetdata);
      printf("Sheets found: %i\n", sheetdata.index);

      //perform tests
      xlsxioread_process_sheet(xlsxioread, NULL, XLSXIOREAD_SKIP_EMPTY_ROWS, sheet_cell_callback, sheet_row_callback, NULL);

      //clean up
      free(sheetdata.firstsheet);
      xlsxioread_close(xlsxioread);
    }
  }
  return 0;
}
