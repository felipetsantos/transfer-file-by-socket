char * readFile(char * filePath, int * sizeContent){

	FILE *f = fopen(filePath, "rb");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	(*sizeContent) = fsize;
	
	char *string = malloc(fsize + 1);
	fread(string, fsize, 1, f);
	fclose(f);

	string[fsize] = 0;
	return string;
}

void saveFile(char * filePath,char * buffer, int sizeBuffer){
	FILE* pFile;
	pFile = fopen(filePath,"wb");
	if (pFile){
	    fwrite(buffer, sizeBuffer, 1, pFile);
	}
	fclose(pFile);
}