#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "okFrontPanelDLL.h"

void print_usage(void)
{
	printf("grab_ok is used for reading and writing data from/to an Opal Kelly FPGA\n\n");
	printf("Usage: grab_ok -h   (display this text)\n");
	printf("       grab_ok -b firmware [-I identifier]\n");
	printf("               [-i input_file] [-o output_file -c count]\n");
	printf("\n");
	printf(" -b : path to the FPGA bitfile to use.\n");
	printf(" -I : identifier of the FPGA to connect to. Useful if more than one\n");
	printf("      FPGA board is connected.\n");
	printf(" -i : path to input file to transmit to the FPGA.\n");
	printf(" -o : path to the output file that will be written with data from the FPGA.\n");
	printf(" -c : count of bytes (multiple of 1024) that will be read from the FPGA.\n");
	printf("\n");
}


int main(int argc, char *argv[])
{
	int i;
	int c;
	int rc;
	char *token;
	okUSBFRONTPANEL_HANDLE xem;
	FILE *fptr;
	char buf[1024];
	
	/* ==============================
	 * Variables for program arguments
	 */
	char *identifier = NULL;
	char *firmware = NULL;
	char *infilename = NULL;
	char *outfilename = NULL;
	int count = 0;
	/* ============================== */

	for(i=1; i<argc; i++){
		if(!strcmp("-h", argv[i])){
			print_usage();
			return 0;
		}else if(!strcmp("-b", argv[i])){
			if(i == argc-1){
				fprintf(stderr, "Error: -b argument given, but firmware filename not given.\n");
				print_usage();
				return 1;
			}
			firmware = strdup(argv[i+1]);
			i++;
		}else if(!strcmp("-c", argv[i])){
			if(i == argc-1){
				fprintf(stderr, "Error: -c argument given, but count not given.\n");
				print_usage();
				return 1;
			}
			count = atoi(argv[i+1]);
			if(count <= 0){
				fprintf(stderr, "Error: Invalid count (%d)\n", count);
				print_usage();
				return 1;
			}
			i++;
		}else if(!strcmp("-I", argv[i])){
			if(i == argc-1){
				fprintf(stderr, "Error: -I argument given, but identifier not given.\n");
				print_usage();
				return 1;
			}
			identifier = argv[i+1];
			i++;
		}else if(!strcmp("-i", argv[i])){
			if(i == argc-1){
				fprintf(stderr, "Error: -i argument given, but input filename not given.\n");
				print_usage();
				return 1;
			}
			infilename = strdup(argv[i+1]);
			i++;
		}else if(!strcmp("-o", argv[i])){
			if(i == argc-1){
				fprintf(stderr, "Error: -o argument given, but output filename not given.\n");
				print_usage();
				return 1;
			}
			outfilename = strdup(argv[i+1]);
			i++;
		}else{
			fprintf(stderr, "Error: Argument '%s' not recognised.\n", argv[i]);
			return 1;
		}
	}

	if(outfilename){
		if(!count){
			print_usage();
			return 1;
		}
	}

	if(!infilename && !outfilename){
		print_usage();
		return 1;
	}

	if(!okFrontPanelDLL_LoadLib(NULL)){
		return 1;
	}

	xem = okUsbFrontPanel_Construct();

	if(okUsbFrontPanel_OpenBySerial(xem, identifier) != ok_NoError) return 1;
	if(okUsbFrontPanel_LoadDefaultPLLConfiguration(xem) != ok_NoError) return 1;
	if(okUsbFrontPanel_SetBTPipePollingInterval(xem, 1) != ok_NoError) return 1;
	if(okUsbFrontPanel_ConfigureFPGA(xem, firmware) != ok_NoError) return 1;

	if(outfilename){
		fptr = fopen(outfilename, "rb");
		while(!feof(fptr)){
			c = fread(buf, 1, 1024, fptr);
			okUsbFrontPanel_WriteToBlockPipeIn(xem, 0x80, 1024, c, buf);
		}
		fclose(fptr);
	}

	if(infilename){
		fptr = fopen(infilename, "wb");
		while(count > 0){
			c = okUsbFrontPanel_ReadFromBlockPipeOut(xem, 0xA0, 1024, 1024, buf);
			count -= c;
			fwrite(buf, 1, c, fptr);
		}
		fclose(fptr);
	}

	return 0;
}
