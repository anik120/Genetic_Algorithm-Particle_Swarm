#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include<time.h>
#include<string.h>

#define PARTICLES 30
#define SIZE 1000
#define ITERATIONS 1000


void adjacencyMatrix(int n, int p1, int p2, FILE * inputF);
void particles_swarming(int n);
void check(int n);
void fitness(int n);
int a[SIZE][SIZE], particles[SIZE][SIZE], used_colours[SIZE][SIZE], fit[PARTICLES];

const char * files[]=
{"DSJC125.1.col","DSJC125.5.col","DSJC250.1.col","DSJC250.5.col","DSJC250.9.col","DSJC500.1.col","DSJC500.5.col","DSJC500.9.col","DSJR500.1.col","DSJR500.1c.col","DSJR500.5.col","flat300_20_0.col","flat300_26_0.col","flat300_28_0.col","fpsol2.i.1.col","fpsol2.i.2.col","fpsol2.i.3.col","inithx.i.3.col","le450_15a.col","le450_15b.col","le450_15c.col","le450_15d.col","le450_25a.col","le450_25b.col","le450_25c.col","le450_25d.col","le450_5a.col","le450_5b.col","le450_5c.col","le450_5d.col","mulsol.i.4.col","mulsol.i.5.col","school1.col","school1_nsh.col","zeroin.i.1.col","zeroin.i.2.col","zeroin.i.3.col","anna.col","david.col","huck.col","jean.col","miles1500.col","miles250.col","miles500.col","miles750.col","queen10_10.col","queen11_11.col","queen12_12.col","queen13_13.col","queen14_14.col","queen16_16.col","queen5_5.col","queen6_6.col","queen7_7.col","queen8_12.col","queen8_8.col","queen9_9.col","myciel3.col","myciel5.col","myciel7.col"};


void adjacencyMatrix(int n, int p1, int p2, FILE * inputF) {
	int i, j;

	// initialising the matrix with all 0s. 
	for(i = 1;i <= n; i++){
		for(j = 1;j <= n; j++){
			a[i][j] = 0;
		}
	}
	//value of matrix[x][y] is 1 if there exists an edge between x and y, 0 otherwise
	do{
		for(i = 1;i <= n; i++){
			for(j = 1;j <= n; j++){   
				if(i == p1 && j == p2){
					a[i][j] = 1;
					a[j][i] = 1;
				}
			}
			a[i][i] = 0;
		}
	}while(fscanf(inputF, "%d %d", &p1, &p2) != EOF);
	particles_swarming(n); //passing the adjacency matrix to the method to begin the process
	//exit(0);
}



void fitness(int n){
	int i, j, k, temp;

	// calculating number of unique colours.
	for(i = 1;i <= PARTICLES; i++){
		temp = 0;
		for(j = 1;j <= n; j++){
			for(k = 1;k <= n; k++){
				if(particles[i][k] == j){
					temp++;
					break;
				}
			}
		}

	fit[i] = temp;
	}
}

void check(int n){
	int i, j, k, l, x, flag, temp, index, m, rep_flag;
	//printf("Entered check\n");
	for(x = 1;x <= PARTICLES; x++){		
		for(i = 1;i <= n; i++){
			for(j = 1;j <= n; j++){
				if(i != j && a[i][j] == 1 && particles[x][i] == particles[x][j]){ // if there exists an edge between two vertices(j and k) and they have the same colour
					rep_flag = 0;
					k = 1;
					// find replacement colour
					while(k <= n){
						if(i != k && a[i][k] == 0 && particles[x][i] != particles[x][k]){
							temp = particles[x][k]; // store potential colour in temp
							flag = 0;
							for(l = 1;l <= n; l++){
								if(a[i][l] == 1 && particles[x][l] == temp){
									flag = 1;
									break;
								}
							}
							if(flag == 0){
								particles[x][i] = temp;
								k = n + 1;
								rep_flag = 1;
							}
							else
								k++;
						}
						k++;
					}
					// update used_colours matrix if new colour used.
					if(rep_flag == 0){
						for(m = 1;m <= n; m++){
							if(used_colours[x][m] == -1){
								particles[x][i] = m;
								used_colours[x][m] = 10;
								break;
							}
						}
					}
				}
			}
		}
	}
}


void particles_swarming(int n){
	int i, j, gbest, velocity, ctr, c_temp1, index1, index2, c_temp2, flag, iterations = 1;
	clock_t begin, end;
	double time_spent;

	begin = clock();


	srand( (unsigned) time(NULL) );
	/*for(i=1;i<=n;i++){
		for(j=1;j<=n;j++){
			printf("%d ",a[i][j]);}
	printf("\n");
	}*/

	//assigning random colours to generate initial population
	for(i = 1;i <= PARTICLES; i++){
		for(j = 1;j <= n; j++){
			particles[i][j] = (rand() % n) + 1;
		}
	}
	while(iterations <= ITERATIONS){		
		for(i = 1;i <= PARTICLES; i++){
			for(j = 1;j <= n; j++){
				used_colours[i][j] = -1;
			}
		}
		// identifying colours used	
		for(i = 1;i <= PARTICLES; i++){
			for(j = 1;j <= n; j++){
				used_colours[i][ particles[i][j] ] = 10;
			}
		}
		check(n);
		fitness(n);
		/*printf("Particles:-   Fitness\n");
		for(i=1;i<=PARTICLES;i++){
			for(j=1;j<=n;j++){
				printf("%d  ",particles[i][j]);}
		printf("     %d\n",fit[i]); 	}*/

		// determining global best fitness
		gbest = SIZE;
		for(i = 1;i <= PARTICLES; i++){
			if(fit[i] < gbest){
				gbest = fit[i];
			}
		}
		//printf("Global best: %d\n",gbest);
		//Particles approaching towards the gbest
		for(i = 1;i <= PARTICLES; i++){
			ctr = 1;
			velocity = fit[i] - gbest;
			while(ctr <= velocity){
				index1 = (rand() % n) + 1;
				flag = 0;
				while(flag == 0){
					index2 = (rand() % n) + 1;
					if(index2 != index1){
						flag = 1;
					}
				}
				c_temp1 = particles[i][index1];
				c_temp2 = particles[i][index2];
				for(j = 1;j <= n; j++){
					if(particles[i][j] == c_temp1){
						particles[i][j] = c_temp2;
					}
				}
				ctr++;
			}
		}
		for(i = 1;i <= PARTICLES; i++){
			for(j = 1;j <= n; j++){
				used_colours[i][j] = -1;
			}
		}

	    for(i = 1;i <= PARTICLES; i++){
			for(j = 1;j <= n; j++){
				used_colours[i][particles[i][j]] = 10;
			}
	    }
		check(n);
		fitness(n);
		/*printf("Particles:-                             Fitness\n");
		for(i=1;i<=PARTICLES;i++){
			for(j=1;j<=n;j++){
				printf("%d  ",particles[i][j]);}
		printf("        %d\n",fit[i]); 	}*/
		gbest = SIZE;
		for(i = 1;i <= PARTICLES; i++){
			if(fit[i] < gbest){
				gbest = fit[i];
			}
		}
		//Trying to improve gbest
		for(i = 1;i <= PARTICLES; i++){
			if(fit[i] == gbest){
				index1 = (rand() % n) + 1;
				c_temp1 = particles[i][index1];
				flag = 0;
				while(flag == 0){
					index2 = (rand() % n) + 1;
					if(index2 != index1 && particles[i][index2] != c_temp1){
						flag = 1;
						c_temp2 = particles[i][index2];
					}
				}
				for(j = 1;j <= n; j++){
					if(particles[i][j] == c_temp1){
						particles[i][index1] = c_temp2;
					}
				}
			}
		}
		for(i = 1;i <= PARTICLES; i++){
			for(j = 1;j <= n; j++){
				used_colours[i][j] = -1;
			}
		}
		for(i = 1;i <= PARTICLES; i++){
			for(j = 1;j <= n; j++){
				used_colours[i][particles[i][j]] = 10;
			}
	    }
		check(n);
		fitness(n);
		//printf("Particles:-                                Fitness\n");
		/*for(i=1;i<=PARTICLES;i++){
			for(j=1;j<=n;j++){
				printf("%d  ",particles[i][j]);}
		printf("            %d\n",fit[i]); 	}*/
		gbest = SIZE;
		for(i = 1;i <= PARTICLES; i++){
			if(fit[i] < gbest){
				gbest = fit[i];
			}
		}
		iterations++;
	}	
	printf("%d", gbest);
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("(%f)\n", time_spent);
}	
	
int main()
{
	int a[SIZE][SIZE], y;
	FILE * inputFile;
	int entries, i;
	for (i = 0; i < sizeof(files)/sizeof(files[1]) ; i++) {
		char file[] = "Data_set/";
		strcat(file, files[i]);
		inputFile = fopen(file, "r");
		printf("%s: ", files[i]);
		int p1 = 0, p2 = 0,j; // p1, p2 correspondes to the pair of points(vertices) in each line of an input file.

		if(inputFile == NULL){
			printf("File failed to open.");
			exit(EXIT_FAILURE);
		}
		fscanf(inputFile, "%d", &entries);
		adjacencyMatrix(entries, p1, p2, inputFile); //creating adjacency matrix from entires in input file.
	}
	exit(0);
}
