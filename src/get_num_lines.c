long int get_num_lines(FILE * stream){
	// Based on the tsvlib code I wrote
	long int initialPos = ftell(stream);

	long int linecount = 1;
	char char1, char2;
	fseek(stream, 0, SEEK_SET);
	char1 = '\n';
	while(char1 != EOF){
		char2 = char1;
		char1 = fgetc(stream);
		if(char1 == '\n' && char2 != '\n') linecount ++;
	}
	if(char2 == '\n') linecount --;

	fseek(stream, initialPos, SEEK_SET);	// Reset the stream to the position it was in when passed to this function
	return linecount;
}

