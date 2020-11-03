#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv) {
        int totallines = 0;
	int totalwords = 0;
	int totalnums = 0; 
	int totalchars = 0;
	int totalspec = 0;
        FILE *in;
	
	//out initialized to stdout, a file if the argument is specified. 
        FILE *out = stdout;

        int opt;
        
	//test conditionals. Intialized to false
        int ldefault = 0;
        int wdefault = 0;
        int ndefault = 0;
        int cdefault = 0;
	int sdefault = 0;
 
	//default spacing for the output lines
        int fdefault = 20;
        
	int i = 1;

        while(((opt = getopt(argc, argv, "slcnwo:f:")) != -1)) {
                switch (opt) {
                    case 'f' :
			i+=2;
			fdefault = atoi(optarg);
			break;
                    case 'o' :
			i+=2; 
			out = fopen(optarg, "w"); 
			break;
                    case 'l' :
			i++; 
			ldefault = 1; 
			break;
                    case 'w' :
			i++; 
			wdefault = 1; 
			break;
                    case 'n' :
			i++; 
			ndefault = 1; 
			break;
                    case 'c' :
			i++; 
			cdefault = 1; 
			break;
		    case 's' :
			i++;
			sdefault = 1;
			break;
		    default:
			printf("Error: invalid arg\n");
			return -1;
			//abort();
                }
        }

	// Sets default to count lines, characters, and words if there are no arguments

	if(!sdefault && !ldefault && !ndefault && !cdefault && !wdefault){
		ldefault = 1;
		cdefault = 1;
		wdefault = 1;
	}

	for(i = i; i < argc; i++){

		char *line = NULL;
		size_t line_buff = 0;
		int lines = 0;
		ssize_t line_size;

		int words = 0;

		int chars = 0;
		
		int nums = 0;

		int specCount = 0;
		in = fopen(argv[i], "r");
		if(in == NULL){
			fprintf(stderr, "%s: No such file or directory\n", argv[i]);
			fclose(in);
			continue;
		} 
		
		line_size = getline(&line, &line_buff, in);	
		while(line_size >= 0){
			int stringlen = strlen(line);

			//line size + 1 to account for the null string pointer added

			//temp string for counting numbers
                        char *temp = (char *)malloc(line_size + 1);
			
			//temp string for counting special characters
			char *special = (char *)malloc(line_size + 1);
			
                        char *token;
                        int numcount = 0;
                        int linenum = 0;
                        int cursor = 0;

			int specialChar = 0;
			int specCursor = 0;

                        lines++;
                        chars += stringlen;

                        strcpy(temp, line);
                        token = strtok(temp, " \n\t");
                        while(token != NULL){
                                words++;
                                token = strtok(NULL, " \n\t");
                        }
			while(cursor < stringlen){
                                numcount = strcspn(line + cursor, "0123456789");
                                cursor += numcount;
                                if(cursor < stringlen){
                                        nums++;
                                }
                                linenum = strspn(line + cursor, "0123456789");
                                cursor += linenum;

                                //printf("lines: %d, numcount: %d, linenum: %d, strlen: %d\n",lines, numcount, linenum, stringlen);

                        }
			free(temp);

						
                        strcpy(special, line);
                        token = strtok(special, " \n\t");
                        while(token != NULL){
                                words++;
                                token = strtok(NULL, " \n\t");
                        }
			while(specCursor < stringlen){
                                numcount = strcspn(line + specCursor, "!@#$%^&*()_+?/|");
                                specCursor += numcount;
                                specialChar  = strspn(line + specCursor, "!@#$%^&*()_+?/|");
				specCount += specialChar;
                                specCursor += specialChar;

                                //printf("lines: %d, numcount: %d, linenum: %d, strlen: %d\n",lines, numcount, linenum, stringlen);

                        }
			free(special);
		
			line_size = getline(&line, &line_buff, in);
					
		}
		
		fprintf(out, "\n%*s:",fdefault * -1,  argv[i]);
		if(cdefault == 1){
			fprintf(out, "%5d", chars);
		}
		if(wdefault == 1){
			fprintf(out, "%5d", words);
		}
		if(ndefault == 1){
			fprintf(out, "%5d", nums);
		}
		if(ldefault == 1){
			fprintf(out, "%5d", lines);
		}
		if(sdefault == 1){
			fprintf(out, "%5d", specCount);
		}

		totallines += lines;
		totalwords += words;
		totalnums += nums;
		totalchars += chars;
		totalspec += specCount;
		if(line){
			fclose(in);
		}
		
	}
	fprintf(out, "\n%*s:", fdefault * -1, "total");
		if(cdefault == 1){
			fprintf(out, "%5d", totalchars);
		}
		if(wdefault == 1){
			fprintf(out, "%5d", totalwords);
		}
		if(ndefault == 1){
			fprintf(out, "%5d", totalnums);
		}
		if(ldefault == 1){
			fprintf(out, "%5d", totallines);
		}
		if(sdefault == 1){
			fprintf(out, "%5d", totalspec);
		}
	fclose(out);

}
