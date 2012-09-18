timeseries_to_spectrum: src/timeseries_to_spectrum.c src/read_args.c src/clean_up.c src/get_num_lines.c
	gcc -Wall -lfftw3 src/timeseries_to_spectrum.c -o bin/timeseries_to_spectrum
