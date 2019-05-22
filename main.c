#include "lucas_kanade.h"

int main(int argc,char* argv[]){
    FILE* fp;        // to read the 4th argument as a file name
    FILE* fp2;

    if((fp = fopen(argv[1],"rb")) == NULL)
        printf("Error, the file doesn't exist.\n");

    if((fp2 = fopen(argv[2],"rb")) == NULL)
        printf("Error, the file doesn't exist.\n");
    
    FHeader image = readFileHeader(fp); //Attention!!! You should check that the pot folder and codes are in the same folder 
    FHeader image2 = readFileHeader(fp2); //Attention!!! You should check that the pot folder and codes are in the same folder 

    // printContent(image,argv[1]);  // printing the content of file
    // printContent(image2,argv[2]);  // printing the content of file
    
    unsigned char **temp;
    temp = calloc(image.height,sizeof(unsigned char*));  // ALLOCATING STRIPPED ARRAY
    for(int i=0;i<image.height;i++)
        temp[i] = calloc(image.width*(image.npp/8),sizeof(unsigned char));

    temp = decreasedResolutionArray(image);
    writeToFile("XXX_R.bmp",image,temp); // WRITING IT TO NEW FILE

    unsigned char **temp2;
    temp2 = calloc(image.height,sizeof(unsigned char*));  // ALLOCATING STRIPPED ARRAY
    for(int i=0;i<image.height;i++)
        temp2[i] = calloc(image.width*(image.npp/8),sizeof(unsigned char));

    temp2 = decreasedResolutionArray(image2);
    writeToFile("XXX_R2.bmp",image2,temp2); // WRITING IT TO NEW FILE

    unsigned char **temp3;
    temp3 = calloc(image.height,sizeof(unsigned char*));  // ALLOCATING STRIPPED ARRAY
    for(int i=0;i<image.height;i++)
        temp3[i] = calloc(image.width*(image.npp/8),sizeof(unsigned char));
 

    int flagA = image.npp/8%4;
    temp3 = compareImages(temp,temp2,image.width,image.height,image.npp/8,3,flagA);
    writeToFile("XXX_COMPARE.bmp",image2,temp3); // WRITING IT TO NEW FILE

    for(int i=0;i<image.height;i++)
        free(temp2[i]);
    free(temp2);


    for(int i=0;i<image.height;i++)
        free(temp[i]);
    free(temp);
}


