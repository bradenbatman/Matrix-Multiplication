//Braden Batman                                                                                                                                                                                             
//CIS 363 Concurrent Programming                                                                                                                                                                            
//HPC Project - Matrix Multiplication                                                                                                                                                                              

#include <thread>
#include <sys/time.h>

typedef unsigned long long timestamp_t;
const int MICROSECONDS_PER_SECOND = 1000000.0L;

// get_timestamp returns time of day in microseconds.
//static - local scope shouldn't matter for only one file
timestamp_t get_timestamp ()
{
   struct timeval now;
   ::gettimeofday (&now, NULL);
   return now.tv_usec +
      static_cast<timestamp_t>(now.tv_sec) * MICROSECONDS_PER_SECOND;
}

timestamp_t pre, post;

//matrixSize determines the size of the x and y dimensions of the square matrixes.
int matrixSize = 200;
int xSize = matrixSize;
int ySize = matrixSize;

//Allocates the memory spots for a 2D array.
int** create2DArray(){
  int** temp = new int*[xSize];
  
  for(int i = 0; i< xSize;i++){
    temp[i] = new int[ySize];
  }

  return temp;
}

//Frees the memory spots for the passed in 2D array.
void delete2DArray(int** arr){
  for(int i = 0; i< xSize;i++){
    delete(arr[i]);
  }

  delete arr;
}

//Populates each spot in the passed in 2D array with a random value (1-100).
void randPop(int** arr){
  for(int x=0;x<xSize;x++){
    for(int y=0;y<ySize;y++){
      arr[x][y] = (rand()%100+1);
    }
  }
}

//Populates a passed in array with the identity matrix. 
void identityPop(int** arr){
  for(int x=0;x<xSize;x++){
    for(int y=0;y<ySize;y++){
        if(x==y){
            arr[x][y] = 1;
        }
        else{
            arr[x][y] = 0;
        }
    }
  }
}

int** firstMat = create2DArray();
int** secondMat = create2DArray();
int** resultMat = create2DArray();

//The multiplication step for any given spot in the array.
int mult(int x, int y){
  int result = 0;

  //The value in the result array is equal to each value in the first matrix's row times the corrosponding value in the
  //second matrix's column.
  for(int spot = 0;spot<xSize;spot++){
    result += firstMat[y][spot]*secondMat[spot][x];
  }
  resultMat[y][x] = result;
}

//Performs mult for each spot in a row
void rowMult(int row){
  for(int x=0;x<xSize;x++){
    mult(x,row);
  }
}

//Spawn threads to perform the mult function for each row in the result matrix.
void spawnThreads(){
  int threadNum = ySize;
  std::thread threads[threadNum];

  int i=0;
  for(int y=0;y<ySize;y++){
      threads[i] = std::thread(rowMult, y);
    i++;
  }

  for(int j=0;j<threadNum;j++){
        threads[j].join();
    }
}

//Prints a matrix
void printMat(int** mat){
    for (int y=0;y<ySize;y++){
        for (int x=0;x<ySize;x++){
            printf(" %d,",mat[y][x]);
        }
        printf("\n");
    }
  printf("\n");
}

int main(int argc, char **argv)
{
  randPop(firstMat);
  randPop(secondMat);

  //Having the second matrix set as the identity matrix is useful for testing, as it makes firstMat and resultMat the same.
  // identityPop(secondMat);

  pre = get_timestamp();
  spawnThreads();
  post = get_timestamp();

  // printMat(firstMat);
  // printMat(resultMat);

  printf("Runtime: %llu microseconds\n", post - pre);
  printf("Size of Matrixes: %d x %d \n", xSize,ySize);

  delete2DArray(firstMat);
  delete2DArray(secondMat);
  delete2DArray(resultMat);
}
