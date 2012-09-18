#include <stdio.h>
#include <stdlib.h>
#include <fftw3.h>

#include "./read_args.c"
#include "./clean_up.c"
#include "./get_num_lines.c"

// FIXME - this is currently specific to the timevar output file of SNOOPY, try to generalise

int main(int argc, char *argv[]){
	FILE *input_timeseries, *output_spectrum;
	unsigned long interpolation_resolution;
	int i, j;
	if(read_args(argc, argv, &input_timeseries, &output_spectrum, &interpolation_resolution)) return -1;

	// Count the number of lines in the input
	unsigned long input_resolution = get_num_lines(input_timeseries) - 1;
	fprintf(stderr, "Found %lu lines in the input file.\n", input_resolution);
	// Throw away the first line of the input (it's a comment)
	int char1 = '\0';
	while(char1 != '\n' && char1 != EOF) {char1 = fgetc(input_timeseries);}

	// Assign memory to hold the input
	double *input_times = malloc(sizeof(double)*input_resolution);
	double *input_values = malloc(sizeof(double)*input_resolution);
	for(i=0; i<input_resolution; i++){
		fscanf(input_timeseries, "%lf\t%lf", &input_times[i], &input_values[i]);
		// Throw away the rest of the line
		char1 = '\0';
		while(char1 != '\n' && char1 != EOF) {char1 = fgetc(input_timeseries);}
	}

	// Assign memory to hold the interpolation
	if(interpolation_resolution==0){
		fprintf(stdout, "Using input resolution of %lu as interpolation resolution.\n", input_resolution);
		interpolation_resolution = input_resolution;
	}
	double *interpolation_times = malloc(sizeof(double)*interpolation_resolution);
	double *interpolation_values = malloc(sizeof(double)*interpolation_resolution);

	// Interpolate, so we have equally spaced data
	j=0;
	for(i=0; i<interpolation_resolution; i++){
		interpolation_times[i] = i*input_times[input_resolution-1]/interpolation_resolution;
		while(j<input_resolution-2 && (input_times[j+1] < interpolation_times[i])) j++;
		interpolation_values[i] = (input_values[j+1] - input_values[j])*(interpolation_times[i] - input_times[j])/(input_times[j+1] - input_times[j]) + input_values[j];
	}

	fftw_complex *spectrum = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*(interpolation_resolution/2 + 1));
	fftw_plan spectrum_plan = fftw_plan_dft_r2c_1d(interpolation_resolution, interpolation_values, spectrum, FFTW_ESTIMATE);
	fftw_execute(spectrum_plan);

	// Print the spectrum to the output file
	fprintf(output_spectrum, "#Freq\tRe(spectrum)\tIm(spectrum)\n");
	for(i=0; i<interpolation_resolution/2 + 1; i++){
		fprintf(output_spectrum, "%lf\t%lf\t%lf\n", i/(interpolation_times[interpolation_resolution-1] - interpolation_times[0]), ((double*)spectrum[i])[0], ((double*)spectrum[i])[1]);
	}

	clean_up(input_timeseries, output_spectrum);

	return 0;
}
