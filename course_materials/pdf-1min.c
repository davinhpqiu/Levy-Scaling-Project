#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#define MAXCHAR 1000
#define NBINS 2001
#define NUMBEROFTAUS 20
// #define NUMBEROFTAUS 37

#define FILENAME "1min-Day-ES-skip.csv"
#define MAXSIZE 2895667
#define XMAX 50000
#define XMIN -50000
#define TICK 0.25
#define OPENHOUR 8 
#define OPENMINUTE 30 
#define MULTIPLIER 100.0
// #define FILENAME "1min-ES-skip.csv"
// #define MAXSIZE 8447530
// #define XMAX 5000
// #define XMIN -5000
// #define OPENHOUR 8 
// #define OPENMINUTE 30 
// #define MULTIPLIER 100.0
// #define FILENAME "1min-Day-HI-skip.csv"
// #define MAXSIZE 1986820
// #define XMAX 16000
// #define XMIN -16000
// #define TICK 1
// #define OPENHOUR 9
// #define OPENMINUTE 15
// #define MULTIPLIER 1.0
// #define FILENAME "1min-Day-YM-skip.csv"
// #define MAXSIZE 2390440
// #define XMAX 6000
// #define XMIN -6000
// #define TICK 1
// #define OPENHOUR 8
// #define OPENMINUTE 30
// #define MULTIPLIER 1.0
// #define FILENAME "1min-YM-skip.csv"
// #define MAXSIZE 6968579
// #define XMAX 200
// #define XMIN -200
// #define OPENHOUR 8
// #define OPENMINUTE 30
// #define MULTIPLIER 1.0
// #define FILENAME "1min-Night-YM-skip.csv"
// #define MAXSIZE 4383417
// #define XMAX 200
// #define XMIN -200
// #define OPENHOUR 8
// #define OPENMINUTE 30
// #define MULTIPLIER 1.0
// #define FILENAME "1min-Day-NQ-skip.csv"
// #define MAXSIZE 2677959
// #define XMAX 250000
// #define XMIN -250000
// #define TICK 0.25
// #define OPENHOUR 8
// #define OPENMINUTE 30
// #define MULTIPLIER 100.0
// #define FILENAME "1min-NQ-skip.csv"
// #define MAXSIZE 7504072
// #define XMAX 5000
// #define XMIN -5000
// #define OPENHOUR 8
// #define OPENMINUTE 30
// #define MULTIPLIER 100.0
// #define FILENAME "1min-Day-ER-skip.csv"
// #define MAXSIZE 2346712
// #define XMAX 6000
// #define XMIN -6000
// #define TICK 0.1
// #define OPENHOUR 8
// #define OPENMINUTE 30
// #define MULTIPLIER 10.0
// #define FILENAME "1min-ER-skip.csv"
// #define MAXSIZE 5845593
// #define XMAX 200
// #define XMIN -200
// #define OPENHOUR 8
// #define OPENMINUTE 30
// #define MULTIPLIER 10.0
// #define FILENAME "1min-Day-FT-skip.csv"
// #define MAXSIZE 3449124
// #define XMAX 30000
// #define XMIN -30000
// #define TICK 0.5
// #define OPENHOUR 8
// #define OPENMINUTE 0
// #define MULTIPLIER 10.0
// #define FILENAME "1min-FT-skip.csv"
// #define MAXSIZE 5524177
// #define XMAX 1000
// #define XMIN -1000
// #define OPENHOUR 8
// #define OPENMINUTE 0
// #define MULTIPLIER 10.0
// #define FILENAME "1min-Day-DA-skip.csv"
// #define MAXSIZE 3682660
// #define XMAX 30000
// #define XMIN -30000
// #define TICK 0.5
// #define OPENHOUR 9
// #define OPENMINUTE 0
// #define MULTIPLIER 10.0
// #define FILENAME "1min-Day-CF-skip.csv"
// #define MAXSIZE 3287759
// #define XMAX 30000
// #define XMIN -30000
// #define TICK 0.5
// #define OPENHOUR 9
// #define OPENMINUTE 0
// #define MULTIPLIER 10.0
// #define FILENAME "1min-Day-PT-skip.csv"
// #define MAXSIZE 2297866
// #define XMAX 4000
// #define XMIN -4000
// #define TICK 0.1
// #define OPENHOUR 9
// #define OPENMINUTE 30
// #define MULTIPLIER 10.0
// #define FILENAME "1min-PT-skip.csv"
// #define MAXSIZE 3169922
// #define XMAX 200
// #define XMIN -200
// #define TICK 0.1
// #define OPENHOUR 9
// #define OPENMINUTE 30
// #define MULTIPLIER 10.0
// #define FILENAME "1min-Day-EN-skip.csv"
// #define MAXSIZE 2565870
// #define XMAX 10000
// #define XMIN -10000
// #define TICK 5
// #define OPENHOUR 7
// #define OPENMINUTE 45
// #define MULTIPLIER 1.0

/* Author: Alexei Chekhlov
   Date: 1/15/2026
   Description: Calculates PDF of 1min data with possible skips with data given in .csv format */

int main() { 
    FILE *fp, *fp1; 
    char row[MAXCHAR]; 
    char *token; 
    char str[1];
    int OpenPrice, HighPrice, LowPrice, *ClosePrice, Volume, *NumTime; 
    int *NumTimeFilled, *ClosePriceFilled, *DoesPriceExist; 
    char **TextDate, **TextTime, *TextTimeCopy; 
    char *endPtr; 
    int i, j, k, n, SizeFilled;
    int Tau[NUMBEROFTAUS];
    int DeltaPrice, DeltaTime; 
    int NumHours, NumMinutes, DaysPassed, PrevNumTime; 
    int *Bin, Dx, **BinCount;
    double **PDF; 

/* 20-size grid of time-steps */
    Tau[0] = 1; 
    Tau[1] = 2; 
    Tau[2] = 3; 
    Tau[3] = 6; 
    Tau[4] = 10; 
    Tau[5] = 18; 
    Tau[6] = 32; 
    Tau[7] = 56; 
    Tau[8] = 100; 
    Tau[9] = 178; 
    Tau[10] = 316; 
    Tau[11] = 562; 
    Tau[12] = 1000; 
    Tau[13] = 1778; 
    Tau[14] = 3162; 
    Tau[15] = 5623; 
    Tau[16] = 10000; 
    Tau[17] = 17783; 
    Tau[18] = 31623; 
    Tau[19] = 56234; 

/* 37-size grid of time-steps 
    Tau[0] = 1; 
    Tau[1] = 2; 
    Tau[2] = 3; 
    Tau[3] = 4; 
    Tau[4] = 6; 
    Tau[5] = 7; 
    Tau[6] = 10; 
    Tau[7] = 13; 
    Tau[8] = 18; 
    Tau[9] = 24; 
    Tau[10] = 32; 
    Tau[11] = 42; 
    Tau[12] = 56; 
    Tau[13] = 75; 
    Tau[14] = 100; 
    Tau[15] = 133; 
    Tau[16] = 178; 
    Tau[17] = 237; 
    Tau[18] = 316; 
    Tau[19] = 422; 
    Tau[20] = 562; 
    Tau[21] = 750; 
    Tau[22] = 1000; 
    Tau[23] = 1334; 
    Tau[24] = 1778; 
    Tau[25] = 2371; 
    Tau[26] = 3162; 
    Tau[27] = 4217; 
    Tau[28] = 5623; 
    Tau[29] = 7499; 
    Tau[30] = 10000; 
    Tau[31] = 13335; 
    Tau[32] = 17783; 
    Tau[33] = 23714; 
    Tau[34] = 31623; 
    Tau[35] = 42170; 
    Tau[36] = 56234; */

/* memory allocation */
    TextDate = malloc(sizeof(char*)*MAXSIZE); 
    TextTime = malloc(sizeof(char*)*MAXSIZE); 
    TextTimeCopy = malloc(sizeof(char*));
    NumTime = malloc(sizeof(int)*MAXSIZE);
    for (i=0; i<MAXSIZE; i++) {
        TextDate[i] = malloc(sizeof(char*)); 
        TextTime[i] = malloc(sizeof(char*));
    }
    ClosePrice = malloc(sizeof(int)*MAXSIZE);
    Bin = malloc(sizeof(int)*NBINS);
    BinCount = malloc(NUMBEROFTAUS*sizeof(int*));
    for (n=0; n<NUMBEROFTAUS; n++)
        BinCount[n] = malloc(NBINS*sizeof(int));
    PDF = malloc(NUMBEROFTAUS*sizeof(double*));
    for (n=0; n<NUMBEROFTAUS; n++)
        PDF[n] = malloc(NBINS*sizeof(double));
    printf("memory allocated...\n"); 

/* read the file */
    i = 0; 
    fp = fopen(FILENAME,"r");
    fgets(row, MAXCHAR, fp); 
    while ((feof(fp) != true) && (i < MAXSIZE))
    {
        fgets(row, MAXCHAR, fp); 
        token = strtok(row, ","); 
        strcpy(TextDate[i],token); 
        token = strtok(NULL, ","); 
        strcpy(TextTime[i],token); 
        token = strtok(NULL, ","); 
        OpenPrice = (int)(MULTIPLIER*strtof(token, &endPtr)); 
        token = strtok(NULL, ","); 
        HighPrice = (int)(MULTIPLIER*strtof(token, &endPtr)); 
        token = strtok(NULL, ","); 
        LowPrice = (int)(MULTIPLIER*strtof(token, &endPtr));
        token = strtok(NULL, ","); 
        ClosePrice[i] = (int)(MULTIPLIER*strtof(token, &endPtr)); 
        token = strtok(NULL, ","); 
        Volume = (int)strtof(token, &endPtr); 
        i = i+1; 
    }
    fclose(fp);
    printf("file read...\n");

/* transform text time into numerical time which is equal to 1 at the 1st minute */
    fp = fopen("TimePrice.csv","w"); 
    fprintf(fp,"Time, Price\n"); 
    PrevNumTime = 0; 
    for (i=0; i<MAXSIZE; i++) { 
        strcpy(TextTimeCopy,TextTime[i]); 
        token = strtok(TextTimeCopy,":"); 
        NumHours = (int)strtof(token, &endPtr); 
        token = strtok(NULL, ":"); 
        NumMinutes = (int)strtof(token, &endPtr); 
        NumTime[i] = PrevNumTime+NumHours*60+NumMinutes-OPENHOUR*60-OPENMINUTE;
        if ((i<MAXSIZE-1) && (strcmp(TextDate[i+1],TextDate[i])!=0)) PrevNumTime=NumTime[i]; 
        fprintf(fp,"%d, %d\n", NumTime[i], ClosePrice[i]); 
    } 
    fclose(fp); 
    printf("transform text time into time in minutes...\n");

/* fill-in the possible gaps */
    k = -1;
    for (i=0; i<MAXSIZE-1; i++) { // first, determine and allocate the necessary memory
        for (j=NumTime[i]; j<NumTime[i+1]; j++) { 
            k++;
        }
    }
    SizeFilled = k+1;
    NumTimeFilled = malloc(sizeof(int)*SizeFilled);
    ClosePriceFilled = malloc(sizeof(int)*SizeFilled);
    DoesPriceExist = malloc(sizeof(int)*SizeFilled);
    k = -1;
    for (i=0; i<MAXSIZE-1; i++) { // second, fill-in the missing minutes and prices
        for (j=NumTime[i]; j<NumTime[i+1]; j++) {
            k++;
            NumTimeFilled[k] = j;
            ClosePriceFilled[k] = ClosePrice[i];
            if (NumTime[i+1]-NumTime[i]==1) DoesPriceExist[k] = 1;
            else DoesPriceExist[k] = 0;
        }
    }
    fp = fopen("FilledTimePrice.csv","w");
    fprintf(fp,"TimeFilled, PriceFilled\n");
    for (k=0; k<SizeFilled; k++) {
        fprintf(fp,"%d, %d\n",NumTimeFilled[k], ClosePriceFilled[k]);
    }
    fclose(fp);
    printf("possible gaps are filled...\n");

/* create bins */
    Dx = (int)((XMAX-XMIN)/(NBINS-1.0)); 
    for (n=0; n<NBINS; n++) 
        Bin[n] = XMIN+Dx*n; 
    printf("bins are created...\n");

/* PDF calculations */
    for (n=0; n<NUMBEROFTAUS; n++) { // steps in time-steps
	printf("TimeStep=%d\n",n); 
        for (i=Tau[n]; i<SizeFilled; i++) {
            DeltaPrice = ClosePriceFilled[i] - ClosePriceFilled[i-Tau[n]]; 
//            if (DoesPriceExist[i]==1 && DoesPriceExist[i-Tau[n]]==1) { // use only the real non-filled prices
                if (DeltaPrice < Bin[0]+Dx/2) BinCount[n][0] += 1;
                for (j=1; j<NBINS-1; j++) {
                    if ((DeltaPrice < Bin[j]+Dx/2) && (DeltaPrice >= Bin[j]-Dx/2)) {
                        BinCount[n][j] += 1; 
//                        printf("i=%d P1=%d P2=%d dP=%d j=%d Bin=%d BinCount=%d\n",i,ClosePriceFilled[i],ClosePriceFilled[i-Tau[n]],DeltaPrice,j,Bin[j],BinCount[n][j]);
                    }
                }
                if (DeltaPrice >= Bin[NBINS-1]-Dx/2) BinCount[n][NBINS-1] += 1;
//                scanf("%s",str);
//            }
        }
        for (j=0; j<NBINS; j++) { 
            PDF[n][j] = (double)BinCount[n][j]/((double)(SizeFilled-Tau[n]+1))/Dx*((double)MULTIPLIER);
        }
    }
    fp = fopen("ProbabilityDensity.csv","w");
    fp1 = fopen("Frequencies.csv","w");
    fprintf(fp,"%s,"," "); // PDF output
    fprintf(fp1,"%s,"," ");
    for (n=0; n<NUMBEROFTAUS; n++) {
        fprintf(fp,"%d,",Tau[n]);
        fprintf(fp1,"%d,",Tau[n]);
    }
    fprintf(fp,"\n");
    fprintf(fp1,"\n");
    for (j=0; j<NBINS; j++) {
        fprintf(fp,"%d,",Bin[j]);
        fprintf(fp1,"%d,",Bin[j]);
        for (n=0; n<NUMBEROFTAUS; n++) {
            fprintf(fp,"%22.19f,",PDF[n][j]);
            fprintf(fp1,"%d,",BinCount[n][j]);
        }
        fprintf(fp,"\n");
        fprintf(fp1,"\n");
    }
    fclose(fp);
    fclose(fp1);

    return 0;
}
