#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct
{
    int x;
    int y;
} Animal;

void initialize_animals(Animal *animals, int num_animals, int grid_length);
void move_animals(Animal *animals, int num_animals, int grid_length, double stay_prob);
int count_encounters(Animal *animals, int num_animals);

int main(int argc, char *argv[])
{
    if (argc != 6)
    {
        printf("Error: Incorrect number of arguments.\n");
        return 1;
    }

    // Parse inputs
    int grid_length = atoi(argv[1]);
    int num_animals = atoi(argv[2]);
    int iterations = atoi(argv[3]);
    double stay_prob = atof(argv[4]);
    unsigned int seed = (unsigned int)(argv[5]);

    // Validate inputs
    if (grid_length <= 0 || num_animals <= 0 || iterations <= 0 || stay_prob < 0 || stay_prob > 1)
    {
        printf("Error: Invalid input values.\n");
        return 1;
    }

    srand(seed); // Seed the random number generator

    // Allocate memory for animals
    Animal *animals = (Animal *)malloc(num_animals * sizeof(Animal));
    if (animals == NULL)
    {
        printf("Error: Memory allocation failed.\n");
        return 1;
    }

    // Initialize animals on the grid
    initialize_animals(animals, num_animals, grid_length);

    // Simulate movements
    int total_encounters = 0;
    for (int i = 0; i < iterations; i++)
    {
        move_animals(animals, num_animals, grid_length, stay_prob);
        total_encounters += count_encounters(animals, num_animals);
    }

    // Output the number of encounters
    printf("%d\n", total_encounters);

    // Free allocated memory
    free(animals);

    return 0;
}

void initialize_animals(Animal *animals, int num_animals, int grid_length)
{
    for (int i = 0; i < num_animals; i++)
    {
        animals[i].x = rand() % grid_length;
        animals[i].y = rand() % grid_length;
    }
}

void move_animals(Animal *animals, int num_animals, int grid_length, double stay_prob)
{
    for (int i = 0; i < num_animals; i++)
    {
        double r = (double)rand() / RAND_MAX;

        if (r < stay_prob)
        {
            // Stay in place
            continue;
        }

        // Randomly pick a direction (north, south, east, west)
        r = (double)rand() / RAND_MAX;
        if (r < 0.25)
        {
            // Move north (decrease y)
            animals[i].y = (animals[i].y == 0) ? 1 : animals[i].y - 1;
        }
        else if (r < 0.5)
        {
            // Move south (increase y)
            animals[i].y = (animals[i].y == grid_length - 1) ? grid_length - 2 : animals[i].y + 1;
        }
        else if (r < 0.75)
        {
            // Move west (decrease x)
            animals[i].x = (animals[i].x == 0) ? 1 : animals[i].x - 1;
        }
        else
        {
            // Move east (increase x)
            animals[i].x = (animals[i].x == grid_length - 1) ? grid_length - 2 : animals[i].x + 1;
        }
    }
}

int count_encounters(Animal *animals, int num_animals)
{
    int encounters = 0;
    for (int i = 0; i < num_animals; i++)
    {
        for (int j = i + 1; j < num_animals; j++)
        {
            if (animals[i].x == animals[j].x && animals[i].y == animals[j].y)
            {
                encounters++;
            }
        }
    }
    return encounters;
}