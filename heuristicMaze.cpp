//Heuristic maze generation
//Simple yet satisfying

#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

const int N = 500;

int a[N][N], aux[N][N];
// a[][] = 1 if there is an obstacle in the maze, 0 if it's free
// aux[x][y] stores the minimum path from (0,0) to (x, y)[BFS]

struct Queue
{
    int row;
    int col;
};
Queue myQueue[N*N]; //self-customed queue

void blocksIntroduction(int nr, int height, int n)
{
        /*
            Introduces in the empty array nr vertical blocks of the shape height x 1
	    where n = array dimension. 
	    This will make our square binary array look more like a maze,
            where the obstacles are not entirely random, but rather grouped in long blocks.
        */

    int x, y;
    for(int i = 1; i <= nr; i++)
    {
        x = rand()%n + 1; //random row
        y = rand()%n + 1; //random column
        if( (x + y) != 2 || y != n)
	   {
           // We don't want to put the block on (1, 1) or (n, n)
        	for(int j = x; j <= x + height - 1 && j<=n ; j++)
        	{
            	a[j][y] = 1;
            	aux[j][y] = -1;
        	}
	   }
    }

}

void initArray(int n)
{
    //We initialize array with 1s on the margins
    for(int i = 0; i <= n + 1; i++)
        a[0][i] = a[n+1][i] = a[i][0] = a[i][n+1] = 1;
}

void print (int n)
{
    //Print the square maze
    for(int i = 0; i <= n+1; i++)
    {
        for(int j = 0; j<= n+1; j++)
            {
                if(a[i][j])
                    cout<< '#';
                else cout<<' ';
            }
        cout<<"\n";
    }
}

int dx[]= {0, 0, 1, -1};
int dy[]= {1, -1, 0, 0};
//arrays to help us control direction

void minPath(int n) // BFS algorithm
{
        /*  We need to ensure that we have a path from (0, 0) to (n, n)-this is mandatory for our binary array to be a maze-
            We find the shortest path from (0, 0) to (n, n)
                - this will exist because we put sufficiently few blocks
                - it will be less straighforward (more maze-like) because of our random blocks
            We will keep this path intact and randomize only the rest of the matrix
        */

    int p = 1, u = 1;
    aux[n/2][1] = aux[n][n/2] = -1;
    // Heuristic ! We blocked first column and last row - they are very tempting for a player!
    myQueue[1].row = 1;
    myQueue[1].col = 1;
    aux[1][1] = 1;
    int x, y;
    while(p <= u)
    {
        x = myQueue[p].row;
        y = myQueue[p].col;
        //Inspect first element of queue
        p++;
        for( int i = 0; i <= 3; i++)
        {
            if(a[x + dx[i]][y + dy[i]] == 0 && aux[x + dx[i]][y + dy[i]] == 0)
            {
                u++;
                myQueue[u].row = x + dx[i];
                myQueue[u].col = y + dy[i];
                aux[x + dx[i]][y + dy[i]] = aux[x][y]+ 1; //distance is bigger with 1
            }
        }
    }
    x = n;
    y = n;
    a[x][y] = -5;

    //Reconstruct the path that we produced and mark it in a[][] with -5
    while(x != 1 || y!= 1)
    {
        for(int i = 0; i <= 3; i++)
        {
            if(aux[x + dx[i]][y + dy[i]] == aux[x][y] - 1)
            {
                x += dx[i];
                y += dy[i];
                a[x][y]= -5;
                break;
            }
        }
    }



}

void randomize(int n)
{
    for(int i = 1; i <= n; i++)
        for(int j = 1; j <= n; j++)
        {
            if(a[i][j] == 0)
            {
                a[i][j] = rand()%2; // Randomize it
            }
            if(a[i][j] == -5)
            {
                a[i][j] = 0;
                // We let this be 0, because it is part of our pre-computed path
            }
        }
}

int main()
{
    srand(time(NULL));
    int n;
    cin >> n; //More interesting for a bigger n
    initArray(n);
    blocksIntroduction(n/4, 6, n); // this is decided after some experiments - it can be adapted!
    minPath(n);
    randomize(n);
    print(n);

    return 0;
}
