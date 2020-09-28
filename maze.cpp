/* 
maze.cpp

Author: Maya Ramde

Short description of this file:
*/

#include <iostream>
#include "mazeio.h"
#include "queue.h"

using namespace std;

// Prototype for maze_search, which you will fill in below.
int maze_search(char**, int, int);

// main function to read, solve maze, and print result
int main(int argc, char* argv[]) {
   int rows, cols, result;
   char** mymaze = NULL;
   
   if(argc < 2)
   {
       cout << "Please provide a maze input file" << endl;
       return 1;
   }
   mymaze = read_maze(argv[1], &rows, &cols); // FILL THIS IN
   
   if (mymaze == NULL) {
      cout << "Error, input format incorrect" << endl;
      return 1;
   }

   // when working on Checkpoint 3, you will call maze_search here.
   // here. but for Checkpoint 1, just assume we found the path.
   result = maze_search(mymaze, rows, cols); // TO BE CHANGED

   // examine value returned by maze_search and print appropriate output
   if (result == 1) { // path found!
      print_maze(mymaze, rows, cols);
   }
   else if (result == 0) { // no path :(
      cout << "No path could be found!" << endl;
   }
   else { // result == -1
      cout << "Invalid maze." << endl;
   }

   // ADD CODE HERE to delete all memory 
   // that read_maze allocated
    for (int i = 0; i < rows; i++) {
        delete [] mymaze[i];
    }
    delete [] mymaze;

   return 0;
}

/**************************************************
 * Attempt to find shortest path and return:
 *  1 if successful
 *  0 if no path exists
 * -1 if invalid maze (not exactly one S and one F)
 *
 * If path is found fill it in with '*' characters
 *  but don't overwrite the 'S' and 'F' cells
 *************************************************/
int maze_search(char** maze, int rows, int cols) 
{
    Location start;
    bool path_found = false;
    int startCheck = 0;
    int finishCheck = 0;
    
    // First, check if there is only 1 start and 1 finish in the maze
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (maze[i][j] == 'S') {
                start.row = i;
                start.col = j;
                startCheck++;
            } else if (maze[i][j] == 'F') {
                finishCheck++;
            }
        }
    }
    
    // Return -1 if it's an invalid maze
    if (startCheck != 1 || finishCheck != 1) {
        return -1;
    }
    
    Queue q(rows*cols); // initialize queue
    q.add_to_back(start); // add starting location as first item in queue
    
    // initialize visit array
    int** visit = new int*[rows];
    for (int i = 0; i < rows; i++) {
        visit[i] = new int[cols];
    }
    
    // set all values in visit array to zero
    for (int i = 0; i < rows; i++) {
         for (int j = 0; j < cols; j++) {
            visit[i][j] = 0;
         }
    }
    visit[start.row][start.col] = 1; // starting position has already been visited
    
    // predecessor position to the start position is not on the maze grid
    Location start_predecessor;
    start_predecessor.row = -1;
    start_predecessor.col = -1;
    
    // Initialize and fill a 'predecessor' 2D arr.
    Location ** predecessor = new Location*[rows];
    for (int i = 0; i < rows; i++) {
        predecessor[i] = new Location[cols];
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            predecessor[i][j] = start_predecessor;
        }
    }
    
    // Search for possible paths as long as queue isn't empty
    while(!q.is_empty()) { 
        Location curr = q.remove_from_front(); // current location is found at front of queue
        
        // There are 4 possible steps to take...so we must check all of them.
        // Possible steps are: up (one_up), down (one_down), left (one_left), right (one_right)
        Location one_down;
        // Where one_down is located with respect to curr
        one_down.row = curr.row + 1;
        one_down.col = curr.col;
        // Proceed only if the position of one_down exists in maze
        if (one_down.row >=0 && one_down.row < rows && one_down.col >=0 && one_down.col < cols) {
            // Check if one_down is the finish position
            if (maze[one_down.row][one_down.col] == 'F') {
                // Back track and replace each previous predecessor value with a '*' to show path
                while (maze[curr.row][curr.col] != 'S') {
                    maze[curr.row][curr.col] = '*';
                    curr = predecessor[curr.row][curr.col];       
                }
                path_found = true;
                break;
            } // If one_right is not the finish position, make it as visited, set next predecessor position, and it to queue
            else if (maze[one_down.row][one_down.col] == '.' && visit[one_down.row][one_down.col] == 0) {
                visit[one_down.row][one_down.col] = 1;
				predecessor[one_down.row][one_down.col] = curr;
				q.add_to_back(one_down);
            }
        }
        
        Location one_up;
        // Where one_up is located with respect to curr
        one_up.row = curr.row - 1;
        one_up.col = curr.col;
        // Proceed only if the position of one_up exists in maze
        if (one_up.row >=0 && one_up.row < rows && one_up.col >=0 && one_up.col < cols) {
            // Check if one_up is the finish position
            if (maze[one_up.row][one_up.col] == 'F') {
                // Back track and replace each previous predecessor value with a '*' to show path
                while (maze[curr.row][curr.col] != 'S') {
                    maze[curr.row][curr.col] = '*';
                    curr = predecessor[curr.row][curr.col];       
                }
                path_found = true;
                break;
            } // If one_up is not the finish position, make it as visited, set next predecessor position, and it to queue
            else if (maze[one_up.row][one_up.col] == '.' && visit[one_up.row][one_up.col] == 0) {
                visit[one_up.row][one_up.col] = 1;
				predecessor[one_up.row][one_up.col] = curr;
				q.add_to_back(one_up);
            }
        }
        
        Location one_left;
        // Where one_left is located with respect to curr
        one_left.row = curr.row;
        one_left.col = curr.col - 1;
        // Proceed only if the position of one_left exists in maze
        if (one_left.row >=0 && one_left.row < rows && one_left.col >=0 && one_left.col < cols) {
            // Check if one_leftt is the finish position
            if (maze[one_left.row][one_left.col] == 'F') {
                // Back track and replace each previous predecessor value with a '*' to show path
                while (maze[curr.row][curr.col] != 'S') {
                    maze[curr.row][curr.col] = '*';
                    curr = predecessor[curr.row][curr.col];       
                }
                path_found = true;
                break;
            } // If one_left is not the finish position, make it as visited, set next predecessor position, and it to queue
            else if (maze[one_left.row][one_left.col] == '.' && visit[one_left.row][one_left.col] == 0) {
                visit[one_left.row][one_left.col] = 1;
				predecessor[one_left.row][one_left.col] = curr;
				q.add_to_back(one_left);
            }
        }
        
        Location one_right;
        // Where one_right is located with respect to curr
        one_right.row = curr.row;
        one_right.col = curr.col + 1;
        // Proceed only if the position of one_right exists in maze
        if (one_right.row >=0 && one_right.row < rows && one_right.col >=0 && one_right.col < cols) {
            // Check if one_right is the finish position
            if (maze[one_right.row][one_right.col] == 'F') {
                // Back track and replace each previous predecessor value with a '*' to show path
                while (maze[curr.row][curr.col] != 'S') {
                    maze[curr.row][curr.col] = '*';
                    curr = predecessor[curr.row][curr.col];       
                }
                path_found = true;
                break;
            } // If one_right is not the finish position, make it as visited, set next predecessor position, and it to queue
            else if (maze[one_right.row][one_right.col] == '.' && visit[one_right.row][one_right.col] == 0) {
                visit[one_right.row][one_right.col] = 1;
				predecessor[one_right.row][one_right.col] = curr;
				q.add_to_back(one_right);
            }
        }
        
        
    }

    // Delete allocated dyn. memory
    for (int i = 0; i < rows; i++)
	{
		delete [] predecessor[i];
		delete [] visit[i];
	}
	delete [] predecessor;
	delete [] visit;
    
    // return 1 or 0, whether a path was found or not 
    if (path_found) {
        return 1;
    } else {
        return 0;
    }
}
