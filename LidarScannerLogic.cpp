#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define PI 3.14159265
#define NUM_POINTS 360
#define MAX_DISTANCE 100.0
#define OBSTACLE_DISTANCE 50.0

void generate_environment(float env_data[], int points) {
    for (int i = 0; i < points; i++) {
        env_data[i] = (float)rand() / (float)(RAND_MAX) * (MAX_DISTANCE - 1.0) + 1.0;
        if (i % 45 == 0) {
            env_data[i] = OBSTACLE_DISTANCE;
        }
    }
}

void simulate_lidar(float env_data[], float lidar_data[], int points) {
    for (int i = 0; i < points; i++) {
        lidar_data[i] = env_data[i] + (float)rand() / (float)(RAND_MAX) * 2.0 - 1.0;
    }
}

void display_lidar_data(float lidar_data[], int points) {
    for (int i = 0; i < points; i++) {
        printf("%d: %.2f\n", i, lidar_data[i]);
    }
}

int main() {
    srand(time(NULL));

    float environment_data[NUM_POINTS];
    float lidar_data[NUM_POINTS];

    generate_environment(environment_data, NUM_POINTS);
    simulate_lidar(environment_data, lidar_data, NUM_POINTS);
    display_lidar_data(lidar_data, NUM_POINTS);

    return 0;
}
