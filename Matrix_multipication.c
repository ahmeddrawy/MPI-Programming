// author  : Ahmed Mohamed Hanafy - ID : 20170357 - 20/03/2020

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc , char * argv[]){
	int my_rank;		/* rank of process	*/
	int p;			/* number of process	*/
	int source;		/* rank of sender	*/
	int dest;		/* rank of reciever	*/
	int tag = 0;		/* tag for messages	*/
	MPI_Status status;	/* return status for 	*/

	/* Start up MPI */
	MPI_Init( &argc , &argv );

	/* Find out process rank */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	/* Find out number of process */
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	// printf("%d\n",my_rank);

	if( my_rank == 0){ /// master thread
		int n = 3, m = 3 , x = 3 ,mt = 3;
	//	printf("please provide the n*m for matrix 1 ");
		scanf("%d%d", &n , &m);
	//	printf("\n");
	//	printf("please provide the n*m for matrix 2 ");
		scanf("%d%d", &mt , &x);
	//	printf("\n");
		if(mt!=m){
			printf("m1 must be equal to n2\n");
			MPI_Finalize();
			return -1 ;		
		}
		for(int i = 1; i < p ; ++i){
			MPI_Send( &n, 1, MPI_INT, i, tag, MPI_COMM_WORLD); 
			MPI_Send( &m, 1, MPI_INT, i, tag, MPI_COMM_WORLD); 
			MPI_Send( &x, 1, MPI_INT, i, tag, MPI_COMM_WORLD); 
		}
		int **mat = malloc(n*sizeof(int *));
		int **mat2 = malloc(m*sizeof(int *));
		// printf("%d %d \n",n ,m );
		for (int i = 0; i < n; ++i){
			mat[i] = malloc(m*sizeof(int));
		}
		for (int i = 0; i < m; ++i)
			mat2[i] = malloc(x*sizeof(int));
		for (int i = 0; i < n; ++i){
			for (int j = 0; j < m; ++j){
				scanf("%d" , &mat[i][j]);
				/* code */
			}
		}
		for (int i = 0; i < m; ++i){
			for (int j = 0; j < x; ++j){
				scanf("%d" , &mat2[i][j]);
				/* code */
			}
		}

		for(int i = 0 ; i < n ; ++i){
			MPI_Send( mat[i], m, MPI_INT, (i)%(p-1) +1, tag, MPI_COMM_WORLD); ///sending row of one
			for(int j = 0 ; j < m ; ++j)
				MPI_Send( mat2[j], x, MPI_INT, (i)%(p-1) +1, tag, MPI_COMM_WORLD); ///sending matrix 2
			// printf("%s\n" , message);
		}

		int **res = malloc(n*sizeof(int *));
		for (int i = 0; i < n; ++i)
			res[i] = malloc(x*sizeof(m));
		for(int i = 0 ; i < n ; ++i){
			int *row2 = malloc(x*sizeof(int)); 
			MPI_Recv(row2, x, MPI_INT, (i)%(p-1) +1, tag, MPI_COMM_WORLD, &status);			
			res[i] = row2;
		}
		for(int i = 0 ; i < n ; ++i){
			for(int j= 0 ; j <  x ; ++j){
				printf("%d\t",res[i][j]);
			}
			printf("\n");
		}
	}
	else{ /// slave
		// int *nn =  malloc (sizeof(int) );
		// int *mm =  malloc (sizeof(int) );
		// int *xx =  malloc (sizeof(int) );

		int n = 0;
		int m = 0;
		int x = 0;
		// printf("hey from process %d\n",my_rank);
		MPI_Recv(&n, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
		MPI_Recv(&m, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
		MPI_Recv(&x, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
		int **mat2 = malloc(m*sizeof(int *));
		// printf("%d %d \n",n ,m );
		for (int i = 0; i < m; ++i){
			mat2[i] = malloc(x*sizeof(int));
		}
		for(int lo = 0 ; lo < n/(p-1) +(my_rank<= n%(p-1)) ; ++lo){
			
			int *res = malloc(x*sizeof(int));
			int *row = malloc(m*sizeof(int));
			MPI_Recv(row, m, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
			
			for(int i = 0 ; i < m ; ++i)
				MPI_Recv(mat2[i], x, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
			for(int i = 0 ; i < x; ++i)res[i] = 0 ;
			for(int i = 0 ; i < m ; ++i){
				for(int j = 0 ; j < x ; ++j){
					res[j]+=row[i]*mat2[i][j];
				}

			}
			MPI_Send( res, x, MPI_INT, 0, tag, MPI_COMM_WORLD); ///sending row of one

		}
	}

	/* shutdown MPI */
	MPI_Finalize();
	return 0;
}