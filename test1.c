#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	int **imageArray;
	imageArray = calloc(10,sizeof(int*));
	for(int i=0;i<10;i++)
		imageArray[i] = calloc(10,sizeof(int));

	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++)
			printf("%d",imageArray[i][j] );
		printf("\n");
	}
	return 0;
}