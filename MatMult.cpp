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

const int xSize = 5;
const int ySize = 5;

int** create2DArray(){
  int** temp = new int*[xSize];
  
  for(int i = 0; i< xSize;i++){
    temp[i] = new int[ySize];
  }

  return temp;
}

void delete2DArray(int** arr){
  
  for(int i = 0; i< xSize;i++){
    delete(arr[i]);
  }

  delete arr;
}

void randPop(int** arr){
  for(int x=0;x<xSize;x++){
    for(int y=0;y<ySize;y++){
      arr[x][y] = (rand()%100+1);
    }
  }
}

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


//test matrixes
// int firstMat[ySize][xSize] = {{2,3,4,5},{1,3,4,2},{6,9,43,12},{23,2,1,6}};

// int secondMat[ySize][xSize] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

// int secondMat[ySize][xSize] = {{1,3,4,5},{1,2,4,2},{6,9,23,12},{23,2,2,6}};

// int resultMat[ySize][xSize];


int mult(int x, int y){
  int result = 0;
  for(int spot = 0;spot<xSize;spot++){
    result += firstMat[y][spot]*secondMat[spot][x];
  }
  resultMat[y][x] = result;
}

void spawnThreads(){
  int threadNum = xSize*ySize;
  std::thread threads[threadNum];

  int i=0;
  for(int y=0;y<ySize;y++){
    for(int x=0;x<xSize;x++){
      threads[i] = std::thread(mult,x,y);
      i++;
    }
  }

  for(int j=0;j<threadNum;j++){
        threads[j].join();
    }

}


int main(int argc, char **argv)
{
  randPop(firstMat);
  identityPop(secondMat);

  pre = get_timestamp();
  spawnThreads();
  post = get_timestamp();


  for (int y=0;y<ySize;y++){
    // printf("{");

    for (int x=0;x<ySize;x++){
      printf(" %d,",resultMat[y][x]);
    }
    printf("\n");
  }
  printf("Runtime: %llu microseconds\n", post - pre);

  delete2DArray(firstMat);
  delete2DArray(secondMat);
  delete2DArray(resultMat);


}
