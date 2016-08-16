#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include<time.h>
#include<string.h>

#define cross_prob 0.8
#define mute_prob 0.5
#define SIZE 1000
#define ITERATIONS 1000
#define noOfChromosomes 10

void check2(int chrom[SIZE],int n,int a[SIZE][SIZE],int used_col[SIZE]);
void crossover(int n,int a[SIZE][SIZE]);
void colourArray(int a[SIZE][SIZE],int n);
void adjacencyMatrix(int a[SIZE][SIZE], int n, int p1, int p2, FILE * inputF);
int calc(int chrom[SIZE],int n);
void mutation(int a[SIZE][SIZE],int n);
int colour[SIZE][SIZE],temp1[SIZE],temp2[SIZE],unique[11],used_colours[SIZE][SIZE];
void fitness(int a[SIZE][SIZE],int colour[SIZE][SIZE],int n);
void check(int n,int a[SIZE][SIZE]);


const char * files[]=
{"DSJC125.1.col","DSJC125.5.col","DSJC250.1.col","DSJC250.5.col","DSJC250.9.col","DSJC500.1.col","DSJC500.5.col","DSJC500.9.col","DSJR500.1.col","DSJR500.1c.col","DSJR500.5.col","flat300_20_0.col","flat300_26_0.col","flat300_28_0.col","fpsol2.i.1.col","fpsol2.i.2.col","fpsol2.i.3.col","inithx.i.3.col","le450_15a.col","le450_15b.col","le450_15c.col","le450_15d.col","le450_25a.col","le450_25b.col","le450_25c.col","le450_25d.col","le450_5a.col","le450_5b.col","le450_5c.col","le450_5d.col","mulsol.i.4.col","mulsol.i.5.col","school1.col","school1_nsh.col","zeroin.i.1.col","zeroin.i.2.col","zeroin.i.3.col","anna.col","david.col","huck.col","jean.col","miles1500.col","miles250.col","miles500.col","miles750.col","queen10_10.col","queen11_11.col","queen12_12.col","queen13_13.col","queen14_14.col","queen16_16.col","queen5_5.col","queen6_6.col","queen7_7.col","queen8_12.col","queen8_8.col","queen9_9.col","myciel3.col","myciel5.col","myciel7.col"};


int main()
{
	int a[SIZE][SIZE], y;
	FILE * inputFile;
	int entries, i;
	for (i = 0; i < 72; i++) {
		char file[] = "Data_set/";
		strcat(file, files[i]);
		inputFile = fopen(file, "r");
		printf("%s: ", files[i]);
		int p1 = 0, p2 = 0, j;  // p1, p2 correspondes to the pair of points(vertices) in each line of an input file.  
		if(inputFile == NULL){
			printf("File failed to open.");
			exit(EXIT_FAILURE);
		}
		fscanf(inputFile, "%d", &entries);
		adjacencyMatrix(a, entries, p1, p2, inputFile); //creating adjacency matrix from entires in input file.
	}
	//exit(0);
}


void adjacencyMatrix(int a[SIZE][SIZE], int n, int p1, int p2, FILE * inputF){
	int i,j;

	// initialising the matrix with all 0s. 
	for(i = 1;i <= n; i++){
		for(j = 1;j <= n; j++){
			a[i][j]=0;
		}
 	}

 	//value of matrix[x][y] is 1 if there exists an edge between x and y, 0 otherwise
	do{
    	for(i = 1;i <=n; i++)
    	{
        	for(j = 1;j <= n; j++)
        	{   if(i == p1 && j == p2){
            	     a[i][j] = 1;
                	 a[j][i] = 1;
            	}
        	}
        a[i][i] = 0;
    	}
	}while(fscanf(inputF, "%d %d", &p1, &p2) != EOF);
    colourArray(a, n); //passing the adjacency matrix to the method to begin the process

}

// method for cheking and ensuring validity of chromosome.
void check(int n, int a[SIZE][SIZE]) {

	int i, j, k, l, flag, temp, index, m, rep_flag, g;
	srand((unsigned)time(NULL));
	for( i = 1;i <= noOfChromosomes; i++ )
	{
		for( j = 1; j <= n; j++ )
		{
			for(k = 1;k <= n; k++)
			{
				if( j != k && a[j][k] == 1 && colour[i][j] == colour[i][k] ) {  // if there exists an edge between two vertices(j and k) and they have the same colour
					rep_flag = 0;
					g = 1;

					// find replacement colour
					while ( g <= n ) {
						if ( j != g && a[j][g] == 0 && colour[i][j] != colour[i][g] ) {
							temp = colour[i][g]; // store potential colour in temp
							flag = 0;
							for( l = 1; l <= n; l++ ) {
								if( a[i][l] == 1 && colour[i][l] == temp ) {
									flag = 1;
									break;
								}
							}
							if ( flag == 0 ) {
								colour[i][j] = temp;
								g = n + 1;
								rep_flag = 1;
							}
							else
								g++;
					  	}
					    g++;
					}
				}
				if ( rep_flag == 0 ) {
					// update used_colours matrix if new colour used.
					for( m = 0; m < n; m++) {
						if ( used_colours[i][m] == -1 ) {
							colour[i][j] = m;
							used_colours[i][m] = 10;
							g = n + 1;            
							break;	
						}
					}
				}
			}
		}		
	}
}
	
	

void sort_chroms(int n){
	int i, j, index[noOfChromosomes], temp_chrom[SIZE][SIZE], temp, ith;
	
	for(i = 1; i <= noOfChromosomes; i++){
		index[i] = i;  //Calculating the unquie colours of each chromosome and storing in an array
	}
	for(i = 1; i < noOfChromosomes; i++){
		for(j = i + 1; j <= noOfChromosomes; j++){
			if( unique[i] > unique[j] ){
				temp = unique[i];
				unique[i] = unique[j];     //Sorting the unique array and book-keeping with the index array 
				unique[j] = temp;
				temp = index[i];
				index[i] = index[j];
				index[j] = temp;
			}
		}
	}
	/*printf("\n");
	for(i=1;i<=10;i++){
		printf("%d\n",unique[i]);
	}*/
	for(i = 1; i <= noOfChromosomes; i++){
		for(j = 1; j <= n; j++){
			ith = index[i];
			temp_chrom[i][j] = colour[ith][j]; //Creating the temp matrix according to the sorted unique array
		}
	}
	for(i = 1;i <= noOfChromosomes; i++){
		for(j = 1;j <= n; j++){
			colour[i][j] = temp_chrom[i][j];   //copying temp matrix to global matrix a
		}
	}
}

void fitness(int a[SIZE][SIZE], int colour[SIZE][SIZE], int n){

	int i, j, k, temp;
	for(i = 1; i <= noOfChromosomes; i++){
		temp = 0;
		// calculating number of unique colours in each chromosome.
		for(j = 0;j < n; j++){
			for(k = 1; k <= n; k++){   
				if( colour[i][k] == j) {
					temp++;
					break;
				}
			}  
		}
		unique[i]=temp; // storing fitness information in unique array.
		//printf("%d\n",unique[i]);
	}
}

void colourArray(int a[SIZE][SIZE],int n)
{
	int i, j, k, ctr=0, rand_prob, flag1=0;
	clock_t begin, end;
	double time_spent;

	begin = clock();
	
	//printing a for testing 
	/*for(i = 1;i <=n; i++)
	{
	for(j = 1;j<=n; j++)
	{   
	printf("%d ",a[i][j]);            
	} 	
	//printf("\n");
	}*/
	
	//Initiallising population
	srand( (unsigned) time(NULL) );
	for(i = 1;i <= noOfChromosomes; i++){
		for(j = 1;j <= n; j++){
			colour[i][j] = rand() % n; //colour matrix is used for book-keeping colouring information
		}
	}

	// initiallising used_colours array for storing information of colours utillised already for colouring vertices
	for(i = 1;i <= noOfChromosomes; i++){
		for(j = 1;j <= n; j++){
			used_colours[i][j-1] = -1;
		}
	}
	for(i = 1; i <= n; i++){
		for(j = 1;j <= n; j++){
			used_colours [i][ colour[i][j] ] = noOfChromosomes; // assigning arbitrary value(10) to colour value alreay used
		}
	}
	check(n, a); //checking validity of chromosome

	fitness(a, colour, n);  //dertimining fitness of chromosome 

	sort_chroms(n); // sorting order of appearence of chromosomes. 1st chromosome (1st row in matrix) has best fitness value (least colours used)

	srand( (unsigned) time(NULL) );
	while(ctr <= ITERATIONS){
		fitness(a, colour, n);
		crossover(n, a);
		fitness(a, colour, n);
		sort_chroms(n);
		rand_prob = (rand() % noOfChromosomes)/ noOfChromosomes;
		if(rand_prob <= mute_prob){
			mutation(a, n);
			fitness(a, colour, n);
			sort_chroms(n);
		}
		else
			continue;
		ctr++;
	}
	printf("%d", unique[1]);
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("(%f)\n",time_spent);
}

void crossover(int n, int a[SIZE][SIZE]){

	float index, rand_prob;
	int i, j, temp1[SIZE], temp2[SIZE], k, flag=0, temp, fit_new1, fit_new2, flag1=0, used_col[SIZE];
	for(i = 0; i < n; i++)
		used_col[i] = -1;
	srand( (unsigned) time(NULL) );
	rand_prob = ( rand() % noOfChromosomes ) / noOfChromosomes;
	if( rand_prob < cross_prob ){                    //creating new chromosomes(temp1 and temp2) via crossover 
		index = (rand() % n) +1;
		//printf("\n %f \n", index);
		for(i = 1; i <= index; i++){
			temp1[i] = colour[1][i];
			temp2[i] = colour[2][i];
	        }
		i = index + 1;
		while(i <= n){
	        temp1[i] = colour[2][i];
			temp2[i] = colour[1][i];
			i++;
		}

	    //checking the validity of the new chromosmes
		check2( temp1, n, a, used_col);
		check2(temp2, n, a, used_col);

		

	    //comparing fitness of the new chromosomes and the worst two chromosomes
	    fit_new1 = calc(temp1, n);
		fit_new2 = calc(temp2, n);
		
		// storing new chromosome if fitness is better than worse chromosomes(replacing worse chromosomes).
		if(fit_new1 <= fit_new2){
			if(fit_new1 < unique[10]){
				for(i = 1;i <= n;i++)
					colour[noOfChromosomes][i] = temp1[i];
				}
			if(fit_new2 < unique[9]){
					for(i = 1;i < 10; i++)
						colour[9][i] = temp2[i];
					
				}
		}
		else{
			if(fit_new2 < unique[10]){			
					for(i = 1; i <= n; i++)
						colour[10][i] = temp2[i];

				}
			else if( fit_new1 < unique[9]){
					for(i = 1; i <= n; i++)
					
						colour[9][i] = temp1[i];
				}
		}
		/*printf("\n");
		for(i = 1; i <= 10; i++){
				for(j = 1; j <= n; j++){
						printf("%d", colour[i][j]);
					}
			printf("\n");
			}*/
			
		fitness(a, colour, n);
		sort_chroms(n);

	}
//else
//continue;
}

void check2(int chrom[SIZE],int n,int a[SIZE][SIZE],int used_col[SIZE]){
	int i, j, k, l, flag, temp, index, m, rep_flag;
	for(i = 1;i <= n; i++){
		used_col[i-1] = -1;
	}
	for(i = 1;i <= n; i++){
		used_col[chrom[i]] = 10;
	}
	//printf("Entered check\n");
	for(i = 1; i <= n; i++){
		for(j = 1;j <= n; j++){
			if(i != j && a[i][j] == 1 && chrom[i] == chrom[j]){
				rep_flag = 0;
				k = 1;
				while(k <= n){
					if(i != k && a[i][k] == 0 && chrom[i] != chrom[k]){
						temp = chrom[k];
						flag = 0;
						for(l = 1;l <= n; l++){
							if(a[i][l] == 1 && chrom[l] == temp){
								flag = 1;
								break;
							}
						}
						if(flag == 0){
							chrom[i] = temp;
							k = n + 1;
							rep_flag = 1;
						}
						else
							k++;
					}
					k++;
				}
				if(rep_flag == 0){
					for(m = 0;m < n; m++){
						if(used_col[m] == -1){
							chrom[i] = m;
							used_col[m] = 10;
							k = n + 1;
							break;	
						}
					}
				}
			}
		}
	} 
}
		
int calc(int chrom[SIZE],int n){
	int i,j,temp=0; 
	for(i=0;i<n;i++){
		for(j=1;j<=n;j++){   
			if(chrom[j]==i){
				temp++;
				break;
			}
		}  
	}    
	return temp;
} 

void mutation(int a[SIZE][SIZE], int n){
	int index, vertex, zero, flag=0, c_temp, i, j, k, temp_colour[SIZE], fit1, fit2, flag1 = 0, used_cols[SIZE];
	srand( (unsigned) time(NULL) );
	index=( rand() % noOfChromosomes) + 1;
	//Copying into new array to generate neighbour
	for(j = 1;j <= n; j++){
		temp_colour[j] = colour[index][j];
	}

	fit1 = calc(temp_colour, n);
	//Emptying one colour class
	vertex = (rand() % n) + 1;  
	c_temp = temp_colour[vertex];
	for(i = 1;i <= n; i++){
		if(temp_colour[i] == c_temp){
			while(flag != 1){
				zero = (rand() % n) + 1;
				if(a[i][zero] == 0 && temp_colour[zero] != c_temp)
					flag = 1;
			}
			temp_colour[i] = temp_colour[zero];            
		}
	}
	/*printf("Colour array:-");
	for(i=1;i<=n;i++){
	printf("%d ",temp_colour[i]); 
	}
	printf("\n");*/
	check2(temp_colour, n, a, used_cols);



	//printf("\nFitness:-");
	fit2 = calc(temp_colour, n);
	if(fit2 < fit1){
		for(i = 1;i <= n; i++)
			colour[index][i] = temp_colour[i];
	}
		
	fitness(a, colour, n);
	sort_chroms(n);
}



        
