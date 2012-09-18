#include <stdio.h>
#include <string.h>

int read_args(int argc, char *argv[], FILE **input_timeseries, FILE **output_spectrum, unsigned long *interpolation_resolution){
	*input_timeseries = NULL;
	*output_spectrum = NULL;
	*interpolation_resolution = 0;
	if(argc < 3){
		fprintf(stderr, "Error reading command line arguments. Need at least two arguments.\nCorrect usage: timeseries_to_spectrum input_file output_file\n");
		return -1;	// Failure
	}

	int i;	// Argument counter
	/* Begin reading arguments */
	for(i=1; i<argc; i++){
		if(!strcmp(argv[i],"--res") && i<(argc-1)){
			*interpolation_resolution = strtod(argv[i+1],NULL);
			i++; // Since we've actually consumed two arguments
		} else if(*input_timeseries==NULL){
			// Open the filename as input
			*input_timeseries = fopen(argv[i],"r");
			if(*input_timeseries==NULL){
				fprintf(stderr, "Error, unable to open %s for reading.\n", argv[i]);
				return -1;
			}
		} else {
			// Open the filename as output
			*output_spectrum = fopen(argv[i],"w");
			if(*output_spectrum==NULL){
				fprintf(stderr, "Error, unable to open %s for writing.\n", argv[i]);
				return -1;
			}

		}
	}
	/* Done reading arguments */

	return 0;	// Success
}
