#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 6)
    {
        printf("Error: Invalid number of arguments\n");
        return 1;
    }

    // Parse command-line arguments
    int gridLength = atoi(argv[1]);
    int numAnimals = atoi(argv[2]);
    int numIterations = atoi(argv[3]);
    double stayProb = atof(argv[4]);
    unsigned int seed = (unsigned int)atoi(argv[5]);

    // Input validation
    if (gridLength <= 0 || numAnimals <= 0 || numIterations <= 0 || stayProb < 0 || stayProb > 1)
    {
        printf("Error: Invalid input values\n");
        return 1;
    }

    srand(seed);

    // Dynamic allocation of the grid (1D array)
    int *grid = (int *)calloc(gridLength * gridLength, sizeof(int));
    if (!grid)
    {
        printf("Memory allocation error\n");
        return 1;
    }

    // Array to hold animal positions
    int *xPos = (int *)malloc(numAnimals * sizeof(int));
    int *yPos = (int *)malloc(numAnimals * sizeof(int));

    // Initialize positions
    for (int i = 0; i < numAnimals; i++)
    {
        xPos[i] = rand() % gridLength;
        yPos[i] = rand() % gridLength;
    }

    int encounters = 0;

    // Simulation loop
    for (int iter = 0; iter < numIterations; iter++)
    {
        // Reset grid for this iteration
        for (int i = 0; i < gridLength * gridLength; i++)
        {
            grid[i] = 0;
        }

        // Update positions and track encounters
        for (int i = 0; i < numAnimals; i++)
        {
            double r = (double)rand() / RAND_MAX;
            if (r >= stayProb)
            {
                double move = (double)rand() / RAND_MAX;
                if (move < 0.25)
                {
                    xPos[i] = (xPos[i] - 1 + gridLength) % gridLength; // North
                }
                else if (move < 0.5)
                {
                    xPos[i] = (xPos[i] + 1) % gridLength; // South
                }
                else if (move < 0.75)
                {
                    yPos[i] = (yPos[i] - 1 + gridLength) % gridLength; // West
                }
                else
                {
                    yPos[i] = (yPos[i] + 1) % gridLength; // East
                }
            }

            int index = xPos[i] * gridLength + yPos[i];
            grid[index]++;
        }

        // Count encounters
        for (int i = 0; i < gridLength * gridLength; i++)
        {
            if (grid[i] > 1)
            {
                encounters++;
            }
        }
    }

    // Output the result
    printf("%d\n", encounters);

    // Free dynamically allocated memory
    free(grid);
    free(xPos);
    free(yPos);

    return 0;
}
