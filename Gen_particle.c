#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define cross_prob 0.8
#define mute_prob 0.5
#define size 500
#define SIZE 500
#define acceptance_prob 0.4
#define GEN_ITERATIONS 5
#define PARTICLE_ITERATION 10
#define PARTICLES 30
#define no_of_chromosomes 30


int n, a[size][size];
struct data
{
    int colour[size];
    int fitness;
    int used_colours[size];
  } *chromosomes[no_of_chromosomes], *particles[PARTICLES];

void adjacencyMatrix(int a[500][500], int n, int p1, int p2, FILE * inputF);
void check(struct data **chrom);
void fitness(struct data **chrom);
void swap(struct data **chrom1, struct data **chrom2);
void sort_chroms(int x);
void crossover(struct data **chrom1,struct data **chrom2,struct data **chrom11,struct data **chrom12);
void mutation();
void special_mutation();
int particles_swarming();
void init_particle_swarming();

int main()
{
  int y, i, j, temp_colour, ctr=1, min_no, probability;
  FILE * inputFile;
  int entries, x;
  clock_t begin, end;
  double time_spent;
  begin = clock();  
  inputFile = fopen("miles1500.col","r");
  int p1 = 0, p2 = 0;
  if (inputFile == NULL)
  {
      printf("File failed to open.\n");
      exit(EXIT_FAILURE);
  }
  fscanf(inputFile, "%d", &entries);
  adjacencyMatrix(a, entries, p1, p2, inputFile);
  n = entries;

    for (i = 1; i <= no_of_chromosomes; i++)
    {
       chromosomes[i] = (struct data *)malloc(sizeof(struct data));
    }

    // Initiallizing used colour array with -1

    for (i = 1; i <= no_of_chromosomes; i++)
    {
	   for (j = 1; j <= n; j++)
	   {
		    chromosomes[i]->used_colours[j - 1] = -1;
	   }
	}

    // Initialising population

    srand((unsigned)time(NULL));
    for (i = 1; i <= no_of_chromosomes; i++){
	for (j = 1; j <= n; j++)
	{
	    temp_colour = rand() % n;
	    chromosomes[i]->colour[j] = temp_colour;
	    chromosomes[i]->used_colours[temp_colour] = 10;    
	}
    }
       
    // Validating and calculating fitness	

	for (i = 1; i <= no_of_chromosomes; i++)
	{
	   check(&chromosomes[i]);
	   fitness(&chromosomes[i]);
	}
   min_no = chromosomes[1]->fitness;

   // Initating particle swarm

   // Allocating memory for particles        
	for (i = 1; i <= PARTICLES; i++)
	{
     	   particles[i] = (struct data *)malloc(sizeof(struct data));
        }

   // Generating initial particles

   init_particle_swarming();	

   while (ctr <= GEN_ITERATIONS)
   {

       // Sorting chromosomes
           sort_chroms(10);

       // Performing crossover    
    	crossover(&chromosomes[1], &chromosomes[2], &chromosomes[11], &chromosomes[12]);
       
	// Performing mutation
       probability = (rand() % 10) / 10;
       if (probability < acceptance_prob)
           special_mutation();
       else   
    	    mutation();
       if(chromosomes[1]->fitness == min_no)
          ctr++;
       else 
          ctr = 1;
       min_no = chromosomes[1]->fitness;
	printf("Chromatic no: %d\n", min_no);
    }	
   end = clock();
   time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
   printf("%d(%f)\n", chromosomes[1]->fitness, time_spent);

return 0;
}


void adjacencyMatrix(int a[500][500], int n, int p1, int p2, FILE * inputF){
int i, j;

	for (i = 1; i <= n; i++)
	  for (j = 1; j <= n; j++)
		a[i][j] = 0;
 
do
{
    for (i = 1;i <= n; i++)
    {
        for (j = 1;j <= n; j++)
        {   if (i == p1 && j == p2){
                 a[i][j] = 1;
                 a[j][i] = 1;
            }
        }
        a[i][i] = 0;
    }
}while (fscanf(inputF, "%d %d", &p1, &p2) != EOF);

}

void check (struct data **chrom)
{
		
    int i, j, k, l, flag, temp, index, m, rep_flag;

	for (i = 0; i < n; i++)
	    (*chrom)->used_colours[i] = -1;

	for (i = 1; i <= n; i++)
	    (*chrom)->used_colours[ (*chrom)->colour[i] ] = 10;

	for (i = 1; i <= n; i++)
	{
		for (j = 1; j <= n; j++)
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
					    k = n + 1;
					    rep_flag = 1;
				        }
				       else
				          k++;
  				}
                              k++;
			      }
                            
			    if (rep_flag == 0)
			    {
				for (m = 0;m < n; m++)
				{
					if ( (*chrom)->used_colours[m] == -1)
					{
						(*chrom)->colour[i] = m;
						(*chrom)->used_colours[m] = 10;
						k = n + 1;                                   							break;	
						}
					   }
	 	
			    }
			}
		}
	} 
}

void fitness (struct data **chrom)
{

int i, j, k, temp = 0;
   for (j = 0; j < n; j++)
   {
       for (k = 1; k <= n; k++)
       {   
	   if ( (*chrom)->colour[k] == j)
	   {
	      temp++;
              break;
             }
	   }  
       }
   (*chrom)->fitness = temp;
  
}

void sort_chroms (int x)
{	
  int i, j;

	for (i = 1;i < x; i++)
	{
		for (j = i+1; j <= x; j++)
		{
			if (chromosomes[i]->fitness > chromosomes[j]->fitness)
				swap (&chromosomes[i], &chromosomes[j]);
		}
	}
}

void swap(struct data **chrom1, struct data **chrom2){
	struct data *temp = *chrom1;
	*chrom1 = *chrom2;
	*chrom2 = temp;
}
	


void crossover(struct data **chrom1, struct data **chrom2, struct data **chrom11, struct data **chrom12)
{
float rand_prob;
int i, index;

srand((unsigned)time(NULL));
rand_prob = (rand() % 10) / 10;

//creating new chromosomes via crossover

 if (rand_prob < cross_prob)
 {                    
	index = (rand() % n) + 1;
	
	for(i = 1; i <= index; i++)
	{
		(*chrom11)->colour[i] = (*chrom1)->colour[i];
		(*chrom11)->used_colours[i-1] = (*chrom1)->used_colours[i-1];
		(*chrom12)->colour[i] = (*chrom2)->colour[i];
		(*chrom12)->used_colours[i-1] = (*chrom2)->used_colours[i-1];
        }
	
	i = index + 1;
	while(i <= n)
	{
        	(*chrom11)->colour[i] = (*chrom2)->colour[i];
		(*chrom12)->used_colours[i-1] = (*chrom2)->used_colours[i - 1];
		(*chrom12)->colour[i] = (*chrom1)->colour[i];
		(*chrom12)->used_colours[i-1] = (*chrom2)->used_colours[i-1];
		i++;
	}

       // Checking the validity of the new chromosmes

	check(&chromosomes[11]);
	check(&chromosomes[12]);

	// Calculating fitness of new chromosomes.

	fitness(&chromosomes[11]);
	fitness(&chromosomes[12]);

	// Comparing fitness of the new chromosomes and the worst two chromosomes

        if((*chrom11)->fitness <= (*chrom12)->fitness)
	{
		
		if(chromosomes[11]->fitness < chromosomes[10]->fitness)
			swap(&chromosomes[10], &chromosomes[11]);
			
		if(chromosomes[12]->fitness < chromosomes[9]->fitness)
			swap(&chromosomes[9], &chromosomes[12]);
	
	}
	else
	{
		if(chromosomes[12]->fitness < chromosomes[10]->fitness)
			swap(&chromosomes[10], &chromosomes[12]);

		else if(chromosomes[11]->fitness < chromosomes[9]->fitness)
			swap(&chromosomes[9], &chromosomes[11]);
	}	
 }

}

void mutation()
{	
    int index, vertex, rep_vertex, flag=0, i, c_temp;	
    srand((unsigned)time(NULL));
    index = (rand() % 10) + 1;
    vertex = (rand() % n) + 1;
    c_temp = chromosomes[index]->colour[vertex];
    while (flag == 0)
    {
    rep_vertex = (rand() % n) + 1;
    if (vertex != rep_vertex && a[vertex][rep_vertex] == 0)
        flag = 1;
    }  
    chromosomes[index]->colour[vertex] = chromosomes[index]->colour[rep_vertex];
    for (i = 1; i <= n; i++)
    {
        if (chromosomes[index]->colour[i] == c_temp)
               break;
    }
    if (i > n)
        chromosomes[index]->used_colours[c_temp] == -1;
}


void special_mutation(){
	
	int ga_index, particle_index, flag = 0, particle_fitness, i;
	float acpt_prob;	
	srand((unsigned)time(NULL));
	
	// Choosing a random chromosome to repalce with a particle
 
        ga_index = (rand() % no_of_chromosomes)+1;

	// Running particle swarm and generating particles

	particle_fitness = particles_swarming();    
    	srand((unsigned)time(NULL));
	
	// Switching a best particle generated with a chromosome
        acpt_prob = (rand()%10)/10;
    	if(particle_fitness < chromosomes[ga_index]->fitness || acpt_prob < acceptance_prob)
    	{
	    while (flag == 0)
	    {
	   	particle_index = (rand() % PARTICLES) + 1;
		if ( particles[particle_index]->fitness == particle_fitness)
			flag = 1; 
	    }
       	   chromosomes[ga_index] = particles[particle_index];
    	}
    	
}



void init_particle_swarming()
{
    int i, j, temp_colour;

	// initiallizing used colour array with -1
    for (i = 1; i <= PARTICLES; i++)
    {
	   for (j = 1; j <= n; j++)
           {
		    particles[i]->used_colours[j-1] = -1;
	   }
	}

    // initialising population
    srand((unsigned)time(NULL));

    for(i = 1; i <= PARTICLES; i++)
    {
	for(j = 1; j <= n; j++)
        {
	    temp_colour = rand() % n;
	    particles[i]->colour[j] = temp_colour;
	    particles[i]->used_colours[temp_colour] = 10; 
	    
	}
    }
       
    //validating and calculating fitness	
	for(i = 1; i <= PARTICLES; i++)
	{
	   check (&particles[i]);
	   fitness (&particles[i]);
	}

}
int particles_swarming()
{
  int y, i, j, gbest, GBEST, velocity, ctr, c_temp1, index1, index2, c_temp2, flag, iterations = 1, temp_colour;
  
    
	// Calculating global best
	
	gbest = SIZE;
	for (i = 1; i <= PARTICLES; i++)
        {
		if(particles[i]->fitness < gbest)
			gbest = particles[i]->fitness;
	}
	
	GBEST = gbest;

  while (iterations <= PARTICLE_ITERATION)
   {
		
	// Particles approaching towards the gbest
	
	for (i = 1; i <= PARTICLES; i++)
        {
		ctr = 1;
		velocity = (particles[i]->fitness)-gbest;
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
  
   return gbest;
}

