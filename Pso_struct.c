#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define PARTICLES 30
#define SIZE 1000
#define ITERATIONS 100
#define c1 2
#define c2 2

int n, a[SIZE][SIZE];

struct data
{
    int colour[SIZE];
    int fitness;
    int used_colours[SIZE];
  } *particles[30];


void adjacencyMatrix (int a[SIZE][SIZE], int n, int p1, int p2, FILE * inputF);
void check (struct data **chrom);
void fitness (struct data **chrom);



int main()
{
  int y, i, j, gbest, GBEST, velocity = 0, ctr, c_temp1, index1, index2, c_temp2, flag, iterations=1, temp_colour;
  FILE * inputFile;
  int entries, x;
  clock_t begin, end;
  double time_spent;
  begin = clock();  
  inputFile = fopen("myciel7.col","r");
  int p1=0, p2 =0;
  if (inputFile == NULL)
  {
           printf("File failed to open.\n");
           exit(EXIT_FAILURE);
  }
  fscanf(inputFile,"%d", &entries);
  adjacencyMatrix (a,entries,p1,p2,inputFile);
   n = entries;
    for (i = 1;i <= 30; i++){
       particles[i] = (struct data *)malloc(sizeof(struct data));
    }
    //initiallizing used colour array with -1
    for (i = 1; i <= 30; i++)
    {
	   for(j = 1; j <= n; j++)
           {
		    particles[i]->used_colours[j-1] = -1;
	   }
	}

    //initialising population
    srand((unsigned)time(NULL));

    for (i = 1; i <= 30; i++)
    {
	for (j = 1;j <= n; j++)
        {
	    temp_colour = rand() % n;
	    particles[i]->colour[j] = temp_colour;
	    particles[i]->used_colours[temp_colour] = 10; 	    
	}
    }
       
    //validating and calculating fitness	
	for (i = 1; i <= PARTICLES; i++){
	   check (&particles[i]);
	   fitness (&particles[i]);
	}

	// Calculating global best
	
	gbest = SIZE;
	for (i = 1; i <= PARTICLES; i++)
        {
		if(particles[i]->fitness < gbest)
			gbest = particles[i]->fitness;
	}
	
	GBEST = gbest;

  while (iterations <= ITERATIONS)
   {
		
	// Particles approaching towards the gbest
	
	for (i = 1; i <= PARTICLES; i++)
        {
		ctr = 1;
	velocity = (particles[i]->fitness - gbest);
	printf("%d \n",velocity);	    
		while (ctr <= velocity)
                {
			index1 = (rand()%n)+1;
			flag = 0;
			while (flag == 0)
                        {
				index2 = (rand()%n)+1;
				if (index2 != index1)
					flag = 1;
			}
			c_temp1 = particles[i]->colour[index1];
			c_temp2 = particles[i]->colour[index2];
			for (j = 1; j <= n; j++)
                        {
				if (particles[i]->colour[j] == c_temp1)
					particles[i]->colour[j] = c_temp2;
			}
		ctr++;
		}
	}
	
	// Resetting used colour matrix, ensuring validity and checking fitness
	
	for (i = 1; i <= PARTICLES; i++)
		for (j = 1; j <= n; j++)
			particles[i]->used_colours[j - 1] = -1;

    	for (i=1; i <= PARTICLES; i++)
		for (j = 1; j <= n; j++)
			particles[i]->used_colours[ particles[i]->colour[j] ] = 10;
	for (i = 1; i <=PARTICLES; i++)
	{
		check (&particles[i]);
		fitness (&particles[i]);
	}

	// Trying to improve gbest
	
	for (i = 1; i <= PARTICLES; i++)
        {
		if (particles[i]->fitness == gbest)
                {
			index1 = (rand()%n)+1;
			c_temp1 = particles[i]->colour[index1];
			flag = 0;
			while (flag == 0)
                        {
				index2 = (rand()%n)+1;
				if (index2 != index1 && particles[i]->colour[index2] != c_temp1){
					flag = 1;
					c_temp2 = particles[i]->colour[index2];
				}
			}
			for(j = 1; j <= n; j++)
                        {
				if (particles[i]->colour[j] == c_temp1)
					particles[i]->colour[index1] = c_temp2;
			}
		}
	}
	
	// Resetting used colour matrix, ensuring validity and checking fitness

	for(i = 1;i <= PARTICLES; i++)
		for(j = 1;j <= n;j++)
			particles[i]->used_colours[j - 1] = -1;

    	for(i = 1;i <= PARTICLES; i++)
		for(j = 1; j <= n; j++)
			particles[i]->used_colours[ particles[i]->colour[j] ] = 10;
	
	for(i = 1; i <= PARTICLES; i++)
	{
		check (&particles[i]);
		fitness (&particles[i]);
	}
    	
	// Calculating global best
	
	gbest = SIZE;
	for (i = 1; i <= PARTICLES; i++)
        {
		if(particles[i]->fitness < gbest)
			gbest = particles[i]->fitness;
	}
    if (gbest == GBEST)
    	iterations++;
    else 
	iterations = 1;
    
    GBEST = gbest;   
    }	
   
   end = clock();
   time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
   printf("%d(%f)\n", gbest,time_spent);
return 0;
}


void adjacencyMatrix(int a[SIZE][SIZE], int n, int p1, int p2, FILE * inputF){
int i,j;

	for(i = 1; i <= n; i++)
	  for(j = 1; j <= n; j++)
	  	a[i][j] = 0;
 
do
{
    for(i = 1; i <= n; i++)
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

}

void check (struct data **chrom)
{
		
    int i, j, k, l, flag, temp, index, m, rep_flag;
	for (i = 0; i < n; i++)
	    (*chrom)->used_colours[i] = -1;
	for (i = 1; i <= n; i++)
	    (*chrom)->used_colours[(*chrom)->colour[i]] = 10;
	for (i = 1; i <= n; i++)
        {
		for (j = 1; j <=n; j++)
		{
			rep_flag = 1;
			if (i != j && a[i][j] == 1 && (*chrom)->colour[i] == (*chrom)->colour[j])
 			{
			    rep_flag = 0;
			    k = 1;
			    while (k <= n)
			    {
				if (i != k && a[i][k] == 0 && (*chrom)->colour[i] != (*chrom)->colour[k])
				{
					temp = (*chrom)->colour[k];
				        flag = 0;
			                for (l = 1; l <= n; l++)
					{
					     if (a[i][l] == 1 && (*chrom)->colour[l] == temp)
					     {
						flag = 1;
						break;
					      }
				        }
				       if (flag == 0)
				       {	
					    (*chrom)->colour[i] = temp;
					    k = n+1;
					    rep_flag = 1;
				       }
				       else
				          k++;
  				}
                              k++;
			      }
                            
			    if (rep_flag == 0)
			    {
				for(m = 0; m < n; m++)
				{
					if ((*chrom)->used_colours[m] == -1)
					{
						(*chrom)->colour[i] = m;
						(*chrom)->used_colours[m] = 10;
						k = n+1;                                   							break;	
					}
				}
	 	
			    }
			}
		}
	} 
}

void fitness (struct data **chrom){

int i, j, k, temp=0;
   for (j = 0; j < n; j++){
       for (k = 1; k <= n; k++){   
	   if ((*chrom)->colour[k] == j)
	   {
	      temp++;
              break;
           }
       }  
   }
   (*chrom)->fitness = temp;
}
