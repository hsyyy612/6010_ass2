#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int x, y;  // Coordinates for the animal
} Animal;

// Function to move an animal according to the random number and stay probability
void move_animal(Animal *a, int grid_length, double stay_prob) {
    // Generate a random number and adjust it to the range [0, 1]
    double r = (double)rand() / RAND_MAX;

    // If r < stay_prob, the animal stays in place
    if (r < stay_prob) return;

    // Determine the move direction
    if (r < stay_prob + (1 - stay_prob)/4) {
        a->x = (a->x == 0) ? 1 : a->x - 1;
    } else if (r < stay_prob + (1 - stay_prob)/2) {
        a->x = (a->x == grid_length - 1) ? grid_length - 2 : a->x + 1;
    } else if (r < stay_prob + (1 - stay_prob)/4 * 3) {
        a->y = (a->y == 0) ? 1 : a->y - 1;
    } else {
        a->y = (a->y == grid_length - 1) ? grid_length - 2 : a->y + 1;
    }
}

// Function to count encounters (more than one animal in the same cell)
int count_encounters(Animal *animals, int num_animals, int grid_length) {
    // Create a grid to track animal positions
    int *grid = (int *)calloc(grid_length * grid_length, sizeof(int));
    int encounters = 0;

    // Place animals in the grid
    for (int i = 0; i < num_animals; i++) {
        int pos = animals[i].x * grid_length + animals[i].y;
        grid[pos]++;
    }

    int re_order = 0;    // initialize animals index for Re_order

    // Count cells with more than one animal (encounters)
    for (int i = 0; i < grid_length * grid_length; i++) {
        if (grid[i] > 1) {
            encounters++;  // Only count one encounter per cell, even if more than 2 animals
        }

        // Re-order animals to ensure the sequence following grid position
        for (int j = 0; j < grid[i]; j++) {
          animals[re_order].x = i / grid_length;
          animals[re_order].y = i % grid_length;  
          re_order++;
        }
    }

    if ( re_order != num_animals ) printf("Error on Re-order. Index = %d\n", re_order);
    
    // Free dynamically allocated memory for the grid
    free(grid);
    return encounters;
}

int main(int argc, char *argv[]) {
    // Check if the correct number of arguments are provided
    if (argc != 6) {
        fprintf(stderr, "Usage: %s <grid_length> <num_animals> <iterations> <stay_prob> <seed>\n", argv[0]);
        return 1;
    }

    // Parse the input arguments
    int grid_length = atoi(argv[1]);
    int num_animals = atoi(argv[2]);
    int iterations = atoi(argv[3]);
    double stay_prob = atof(argv[4]);
    unsigned int seed = (unsigned int)atoi(argv[5]);

    if (grid_length <= 0 || num_animals <= 0 || iterations <= 0 || stay_prob < 0 || stay_prob > 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }

    // Initialize random seed
    srand(seed);

    // Dynamically allocate memory for animals
    Animal *animals = (Animal *)malloc(num_animals * sizeof(Animal));
    if (!animals) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    // Initial random placement of animals on the grid
    for (int i = 0; i < num_animals; i++) {
        animals[i].x = rand() % grid_length;
        animals[i].y = rand() % grid_length;
    }

    int total_encounters = 0;


    // Include encounters on initialzing
    total_encounters = count_encounters(animals, num_animals, grid_length);

    // Run the simulation for the specified number of iterations
    for (int t = 0; t < iterations; t++) {
        // Move each animal
        for (int i = 0; i < num_animals; i++) {
            move_animal(&animals[i], grid_length, stay_prob);
        }

        // Count encounters in this iteration
        total_encounters += count_encounters(animals, num_animals, grid_length);
    }

    // Print the total number of encounters as required
    printf("%d\n", total_encounters);

    // Free dynamically allocated memory
    free(animals);

    return 0;
}