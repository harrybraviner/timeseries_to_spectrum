#include <stdio.h>

int clean_up(FILE *input_timeseries, FILE *output_spectrum){
	
	fclose(input_timeseries);
	fclose(output_spectrum);

	return 0;	// Success
}
