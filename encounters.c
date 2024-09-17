#include <stdio.h>
#include <stdlib.h>

void initialize_animal(int *xPos, int *yPos, int numAnimals, int gridLength);
void reset_grid(int gridLength, int *grid);
void update_grid(int numAnimals, int gridLength, int *grid, int *xPos, int *yPos);
void animals_move(int gridLength, double stayProb, int *grid, int *xPos, int *yPos);
int encounter_track(int *grid, int gridLength);


int main(int argc, char *argv[])
{
    if (argc != 6){
        printf("Error: invalid number of argument.");
        return 1;
    }

    // Parse command-line arguments
    int gridLength = atoi(argv[1]);
    int numAnimals = atoi(argv[2]);
    int numIterations = atoi(argv[3]);
    double stayProb = atof(argv[4]);
    unsigned int seed = (unsigned int)atoi(argv[5]); //give a seed number

    // Input validation
    if (gridLength <= 0 || numAnimals <= 0 || numIterations <= 0 || stayProb < 0 || stayProb > 1)
    {
        printf("Error: Invalid input values\n");
        return 1;
    }

    srand(seed);  //generate predictable number sequence

    //allocate memory for grid and initialize the value to be 0.
    int *grid = (int *)calloc(gridLength * gridLength, sizeof(int));
    if (!grid){
        printf("memory allocation error.");
        return 1;
    }

    // array to hold animal position
    int *xPos = (int *)malloc(numAnimals * sizeof(int));
    int *yPos = (int *)malloc(numAnimals * sizeof(int));

    int encounters = 0;

    initialize_animal(xPos, yPos, numAnimals, gridLength);
    update_grid(numAnimals, gridLength, grid, xPos, yPos);
    encounters = encounter_track(grid, gridLength);
    printf("This is initialization, the total encounter number is %d\n", encounters);

    // move the animals
    for (int i = 0; i < numIterations; i++){
        // reset the grid for the iteration
        // reset_grid(gridLength, grid);
        animals_move(gridLength, stayProb, grid, xPos, yPos);
        update_grid(numAnimals, gridLength, grid, xPos, yPos);
        printf("This is iteration %d, the encounter number in this iteration is %d\n", i, encounter_track(grid, gridLength));
        encounters += encounter_track(grid, gridLength);
        printf("This is iteration %d, the total encounter number is %d\n", i, encounters);
    }

    // Output the result
    printf("%d\n", encounters);

    // Free dynamically allocated memory
    free(grid);
    free(xPos);
    free(yPos);

    return 0;
}




void initialize_animal(int *xPos, int *yPos, int numAnimals, int gridLength){
    // put aninals to initial position
    for (int i = 0; i < numAnimals; i++){
        xPos[i] = rand() % gridLength;
        yPos[i] = rand() % gridLength;
    }
}

void reset_grid(int gridLength, int *grid){
    for (int i = 0; i < gridLength * gridLength; i++){
        grid[i] = 0;
    }
}

void update_grid(int numAnimals, int gridLength, int *grid, int *xPos, int *yPos)
{
    for (int i = 0; i < numAnimals; i++){
        int index = xPos[i] * gridLength + yPos[i];
        grid[index] += 1;
    }
}

void animals_move1(int numAnimals, int gridLength, int stayProb, int *grid, int *xPos, int *yPos){   
    // move animals
    for (int i = 0; i < numAnimals; i++){     //positive order or negative order
        double r = (double)rand() / RAND_MAX;
        if (r >= stayProb){
            double move = (double)(1 - stayProb) / 4;

            // move left
            if (r < stayProb + move){
                yPos[i] = yPos[i] - 1;
                // check out of boundary case
                if (yPos[i] < 0){
                    yPos[i] = 1;
                }
            }

            // move right
            else if (stayProb + move <= r && r < stayProb + 2*move)
            {
                yPos[i] = yPos[i] + 1;
                if (yPos[i] >= gridLength){
                    yPos[i] = gridLength - 2;
                }
            }

            //move down
            else if (stayProb + 2*move <= r && r < stayProb + 3*move)
            {
                xPos[i] = xPos[i] + 1;
                // check out of boundary case
                if (xPos[i] >= gridLength)
                {
                    xPos[i] = gridLength - 2;
                }
                
            }

            //move up
            else if (stayProb + 3 * move <= r && r < 1)
            {
                xPos[i] = xPos[i] - 1;
                // check out of boundary case
                if (xPos[i] < 0)
                {
                    xPos[i] = 1;
                }
            }
        }
    }
}


// Funstion to make single animal move
void animal_move(int i, double stayProb, int *xPos, int *yPos, int gridLength)
{
    // Generate a random number to determine the movement of the animal
    double r = (double)rand() / RAND_MAX;
    if (r >= stayProb)
    {
        double move = (double)(1 - stayProb) / 4;

        // move left
        if (r < stayProb + move)
        {
            xPos[i] = xPos[i] - 1;
            // check out of boundary case
            if (xPos[i] < 0)
            {
                xPos[i] = 1;
            }
        }

        // move right
        else if (stayProb + move <= r && r < stayProb + 2 * move)
        {
            xPos[i] = xPos[i] + 1;
            if (xPos[i] >= gridLength)
            {
                xPos[i] = gridLength - 2;
            }
        }

        // move down
        else if (stayProb + 2 * move <= r && r < stayProb + 3 * move)
        {
            yPos[i] = yPos[i] - 1;
            // check out of boundary case
            if (yPos[i] < 0)
            {
                yPos[i] = 1;
            }            
        }

        // move up
        else if (stayProb + 3 * move <= r && r < 1)
        {
            yPos[i] = yPos[i] + 1;
            // check out of boundary case
            if (yPos[i] >= gridLength)
            {
                yPos[i] = gridLength - 2;
            }
        }
    }
}


// Function to update the position of animals
void animals_move(int gridLength, double stayProb, int *grid, int *xPos, int *yPos)
{
    // Loop through the entire grid (sweep over the grid)
    for (int x = 0; x < gridLength; x++)
    {
        for (int y = 0; y < gridLength; y++)
        {
            int index = x * gridLength + y;

            // Check if the grid position is occupied by any animals
            if (grid[index] > 0)
            { // Assume grid[index] contains number of animals at (x, y)

                // Loop over the animals in this grid position
                for (int i = 0; i < grid[index]; i++)
                {
                    if (xPos[i] == x && yPos[i] == y)
                    {
                        animal_move(i, stayProb, xPos, yPos, gridLength);
                    }
                    grid[index] -= 1;
                }        
            }
        }
    }
}




int encounter_track(int *grid, int gridLength){
    int encounters = 0;
    for (int i = 0; i < gridLength * gridLength; i++)
    {
        if (grid[i] > 1){
            encounters++;   
        }
    }
    return encounters;
}



