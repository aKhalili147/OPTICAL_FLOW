#include "lucas_kanade.h"

FHeader readFileHeader(FILE* fp) {
    FHeader image;
    fread(&image.signature,sizeof(short),1,fp);
    if(image.signature !=0x4D42){
        printf("Error, the file format is not in BMP.\n"); 
    exit(1);
    }
    fread(&image.fileSize,sizeof(int),1,fp);
    fread(&image.reserved1,sizeof(int),1,fp);
    fread(&image.offset,sizeof(int),1,fp);
    fread(&image.sizeofDibHeader,sizeof(int),1,fp);
    fread(&image.width,sizeof(int),1,fp);
    fread(&image.height,sizeof(int),1,fp);
    fread(&image.ncp,sizeof(short),1,fp);
    fread(&image.npp,sizeof(short),1,fp);
    image.restDibHeader = calloc(image.offset-30,sizeof(int)); // allocating 
    unsigned int k=0;
    unsigned int j=0;
    while(k<((image.offset-30)/4)){
        fread(&(image.restDibHeader[j]),sizeof(int),1,fp);
        k++;
        j++;
    }

    if(image.height<0)
        image.height = image.height*(-1);

    int padding =((4-(image.width*(image.npp/8))%4)%4);
    image.imageArray = calloc(image.height,sizeof(char*));
    for(int i=0;i<image.height;i++)
        image.imageArray[i] = calloc(image.width*image.npp/8,sizeof(char));

    for(int i=0;i<image.height;i++){
        for(int j=0;j<image.width*(image.npp/8);j++){
            fread(&image.imageArray[i][j],sizeof(unsigned char),1,fp);
            // printf("imageArray[%d][%d]:%d\n",i,j,image.imageArray[i][j] );
        }

        if(padding > 0)
            fseek(fp,padding,SEEK_CUR);
    }

    fclose(fp); 
    return image; // is a table of different characters that we have iside of file
}

avgPixel averageColor(unsigned char **mat,int width,int height,int ppx){
    PIXEL px;
    px.r = 0;
    px.g = 0;
    px.b = 0;
    avgPixel apx;

    for(int i=0;i<height;i++)
        for(int j=0;j<width*ppx;j+=ppx){
            px.r+=mat[i][j];
            px.g+=mat[i][j+1];
            px.b+=mat[i][j+2];
        }

    apx.avgR = px.r/(height*ppx);
    apx.avgG = px.g/(height*ppx);
    apx.avgB = px.b/(height*ppx);
    return apx;
}


unsigned char** decreasedResolutionArray(FHeader image){
    int factor = 3;
    int ppx = image.npp/8; // lazimdi!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    unsigned char **resArray; // output array after bluering
    avgPixel apx; // average colour struct 
    int flagAlpha = 0; // to know alpha channel is used for bitmap image or not
    int remainderW = image.width%factor;
    int remainderH = image.height%factor;
    int remH = factor - remainderH;
    int remW = factor - remainderW;

    if(ppx == 4)
        flagAlpha = 1;

    resArray = calloc(image.height,sizeof(unsigned char*));
    for(int i=0;i<image.height;i++)
        resArray[i] = calloc(image.width*ppx,sizeof(unsigned char));
    
    for(int i=0;i<image.height;i+=factor){
        for(int j=0;j<image.width*ppx;j+=factor*ppx){

            // factor FactorxFactor matrix creatiation       
            unsigned char **matFactor;
            matFactor = calloc(factor,sizeof(unsigned char*));
            for(int a=0;a<factor;a++)
                    matFactor[a] = calloc(factor*ppx,sizeof(unsigned char));

            //filling factor matrix from image.imageArray
    
            // for height if not divisible to 3 
            if( i == (image.height-remainderH)){                // problem is here
                for(int a=0,l=i-remH;a<factor;a++,l++)
                    for(int b=0,k=j;b<factor*ppx;b++,k++)
                        matFactor[a][b] = image.imageArray[l][k];                

                // to find a average value for 3x3 matrix for each colour(red,green,blue)
                apx = averageColor(matFactor,factor,factor,ppx);

                for(int a=i-remH;a<image.height;a++){
                    for(int b=j;b<factor*ppx+j;b+=ppx){
                        resArray[a][b] = apx.avgR;
                        resArray[a][b+1] = apx.avgG;
                        resArray[a][b+2] = apx.avgB;
                        if(flagAlpha == 1)
                            resArray[a][b+3] = image.imageArray[a][b+3];
                    }
                }
            }
            else if(j == (image.width-remainderW)*ppx){ // for width if not divisible to 3  // problem is here
                for(int a=0,l=i;a<factor;a++,l++)
                    for(int b=0,k=j-remW*ppx;b<factor*ppx;b++,k++)
                        matFactor[a][b] = image.imageArray[l][k];


                // to find a average value for 3x3 matrix for each colour(red,green,blue)
                apx = averageColor(matFactor,factor,factor,ppx);
                //red
                for(int a=i;a<factor+i;a++){
                    for(int b=j-remW*ppx;b<image.width;b+=ppx){
                        resArray[a][b] = apx.avgR;
                        resArray[a][b+1] = apx.avgG;
                        resArray[a][b+2] = apx.avgB;
                        if(flagAlpha == 1)
                            resArray[a][b+3] = image.imageArray[a][b+3];
                    }
                }
            }
            else{             // for normal case
                for(int a=0,l=i;a<factor;a++,l++)
                    for(int b=0,k=j;b<factor*ppx;b++,k++)
                        matFactor[a][b] = image.imageArray[l][k];

                // to find a average value for 3x3 matrix for each colour(red,green,blue)
                apx = averageColor(matFactor,factor,factor,ppx);
                //red
                for(int a=i;a<factor+i;a++){
                    for(int b=j;b<factor*ppx+j;b+=ppx){
                        resArray[a][b] = apx.avgR;
                        resArray[a][b+1] = apx.avgG;
                        resArray[a][b+2] = apx.avgB;
                        if(flagAlpha == 1)
                            resArray[a][b+3] = image.imageArray[a][b+3];
                    }
                }
            }

            for(int a=0;a<factor;a++)
                free(matFactor[a]);
            free(matFactor);           
        }
    }
    return resArray;
}

void writeToFile(char *fileName,FHeader image,unsigned char **arrayStrip){
    int padding =((4-(image.width*(image.npp/8))%4)%4);
    FILE *fpR;
    fpR = fopen(fileName,"wb");
    fwrite(&image.signature,sizeof(short),1,fpR);
    fwrite(&image.fileSize,sizeof(int),1,fpR);
    fwrite(&image.reserved1,sizeof(int),1,fpR);
    fwrite(&image.offset,sizeof(int),1,fpR);
    fwrite(&image.sizeofDibHeader,sizeof(int),1,fpR);
    fwrite(&image.width,sizeof(int),1,fpR);
    fwrite(&image.height,sizeof(int),1,fpR);
    fwrite(&image.ncp,sizeof(short),1,fpR);
    fwrite(&image.npp,sizeof(short),1,fpR);
    int j=0; // counter1 
    int i=0; // counter2 
    while(i<((image.offset-30)/4)){
        fwrite(&(image.restDibHeader[j]),sizeof(int),1,fpR);
        i++;
        j++;
    }
    int ch=0;
    for(int i=0;i<image.height;i++){
        for(int j=0;j<image.width*(image.npp/8);j++){
            fwrite(&arrayStrip[i][j],sizeof(unsigned char),1,fpR);
            // printf("imageArray[%d][%d]:%d\n",i,j,image.imageArray[i][j] );
        }
        for(int c=0;c<padding;c++)
            fputc(ch,fpR); //fputc() is used to write a single character at a time to a given file.
  
    }

    fclose(fpR);
}


void printContent(FHeader image,char *fileName){
    printf("original file name:%s\n",fileName );
    printf("original fileSize:%u\n",image.fileSize );
    printf("reserved1:%u\n",image.reserved1 );
    printf("offset:%u\n",image.offset );
    printf("sizeofDibHeader:%u\n",image.sizeofDibHeader );
    printf("width:%u\n",image.width );
    printf("height:%u\n",image.height );
    printf("ncp:%u\n",image.ncp );
    printf("npp:%u\n",image.npp );
}

void printImageArray(FHeader image){
    for(int i=0;i<image.height;i++)
        for(int j=0;j<image.width*(image.npp/8);j++)
            printf("%u-", image.imageArray[i][j]);
}


unsigned char** compareImages(unsigned char **imageArray1,unsigned char **imageArray2,int width,int height,int ppx,int factor,int flag){

    unsigned char **resArray; // resArray is an array which stores the different pixels between two images
    //allocation of resArray
    resArray = calloc(height,sizeof(unsigned char*));
    for(int i=0;i<height;i++)
        resArray[i] = calloc(width*ppx,sizeof(unsigned char));

    // printf("flag:%d\n",flag );

    for(int i=0;i<height;i+=factor){
        for(int j=0;j<width*ppx;j+=ppx*factor){

            // printf("i:%d\tj:%d\n",i,j);      
            if(  range(imageArray1[i][j],imageArray1[i][j+1],imageArray1[i][j+2],imageArray2[i][j],imageArray2[i][j+1],imageArray2[i][j+2])){
                for(int a=i;a<i+factor && a<height;a++)
                    for(int b=j;b<j+ppx*factor && b<width*ppx;b++){
                        resArray[a][b] = imageArray2[a][b];
                        // printf("i:%d\tj:%d\n",a,b);      
                    }
            }
            else{
                for(int a=i;a<i+factor && a<height;a++)
                    for(int b=j;b<j+ppx*factor && b<width*ppx;b+=factor){
                        resArray[a][b] = 0;
                        resArray[a][b+1] = 0;
                        resArray[a][b+2] = 255;
                        if(flag==0)
                            resArray[a][b+3] = imageArray2[a][b+3];
                        // printf("i:%d\tj:%d\n",a,b);      
                    }
            }
        }
    }
    return resArray;
}



int range(int px11,int px12,int px13,int px21,int px22,int px23){
    int x=0,y=0,z=0;

    if(px11 > (px21 -20) && px11 < (px21 +20))
        x = 1;
    if(px12 > (px22 -20) && px12 < (px22 +20))
        y = 1;
    if(px13 > (px23 -20) && px13 < (px23 +20))
        z = 1;

    if(x == 1 && y == 1 && z == 1)
        return 1;
    else
        return 0;
}


