#include "sspgm.h"


/* global variables */
unsigned int width,length,maxDataVal;
char magicNumber[3];
int dataArray[MAXY][MAXX];

/* funtion prototypes */
void ReadInit(void);
void ReadData(void);
void ProcessData(void);
void WritePgm(void);




/* main function, calls others */

void main(void) 
{
  (void) ReadInit();
  (void) ReadData();
  (void) ProcessData();
  (void) WritePgm();
}




/* read in initialization and parameters */

void ReadInit(void)
{
  fscanf(stdin,"%s\n",&magicNumber);
  fscanf(stdin,"%d %d\n",&width,&length);
}




/* read in the data from the stream */

void ReadData(void)
{
  int i,j;
  unsigned int dtmp;

  maxDataVal = 0;
  for(i=0;i<length;i++)
    for(j=0;j<width;j++) {
      fscanf(stdin,"%d\t",&dataArray[i][j]);
      if(dtmp > maxDataVal)
	maxDataVal = dtmp;
    }
}




/* process the data array, transforming it into a pgm matrix */

void ProcessData(void)
{
  int i,j,scale_factor;

  /* scale down the data into bins for pgm, zero is unique */
  scale_factor = maxDataVal / (MAXGREY - 1);
  for(i=0;i<length;i++)
    for(j=0;j<width;j++)
      if(dataArray[i][j] != 0)
	dataArray[i][j] = (dataArray[i][j] / scale_factor) + 1;
}




/* write out the processed data as a pgm file */

void WritePgm(void)
{
  int i,j;

  /* write out a standard pgm header */
  fprintf(stdout,"P2\n");
  fprintf(stdout,"%d %d\n",width,length);
  fprintf(stdout,"%d\n",MAXGREY);

  /* write out the pgm matrix */
  for(i=0;i<length;i++) {
    for(j=0;j<width;j++)
      fprintf(stdout,"%d ",dataArray[i][j]);
    fprintf(stdout,"\n");
  }
}
