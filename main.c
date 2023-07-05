#include <MiniFB.h>

#include <inttypes.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 1600
#define HEIGHT 900
#define HALF_WIDTH (WIDTH / 2)
#define HALF_HEIGHT (HEIGHT / 2)
#define FOCAL_LENGTH 250
#define SCALE 50

#define CAMERA_SPEED 1

#define TEXTURE_SIZE 8
const uint32_t floor_texture[TEXTURE_SIZE * TEXTURE_SIZE] = {
    0x000000,
    0x000000,
    0x000000,
    0x000000,
    0x000000,
    0x000000,
    0x000000,
    0x000000,

    0x000000,
    0xffffff,
    0xffffff,
    0xffffff,
    0xffffff,
    0xffffff,
    0xffffff,
    0x000000,

    0x000000,
    0xffffff,
    0xffffff,
    0xffffff,
    0xffffff,
    0xffffff,
    0xffffff,
    0x000000,

    0x000000,
    0xffffff,
    0xffffff,
    0xffffff,
    0xffffff,
    0xffffff,
    0xffffff,
    0x000000,

    0x000000,
    0xffffff,
    0xffffff,
    0xffffff,
    0xffffff,
    0xffffff,
    0xffffff,
    0x000000,

    0x000000,
    0xffffff,
    0xffffff,
    0xffffff,
    0xffffff,
    0xffffff,
    0xffffff,
    0x000000,

    0x000000,
    0xffffff,
    0xffffff,
    0xffffff,
    0xffffff,
    0xffffff,
    0xffffff,
    0x000000,

    0x000000,
    0x000000,
    0x000000,
    0x000000,
    0x000000,
    0x000000,
    0x000000,
    0x000000,
};

void render_frame(uint32_t *buffer, float camera_z) {
    float angle = sinf(camera_z);
    float angle_sin = sinf(angle);
    float angle_cos = cosf(angle);

    for (size_t j = HALF_HEIGHT; j < HEIGHT; j++) {
        for (size_t i = 0; i < WIDTH; i++) {
            // X Y Z
            float x = HALF_WIDTH - (float)i;
            float y = (float)j + FOCAL_LENGTH;
            float z = (float)j - HALF_HEIGHT + 0.01f;
            float fog = z / HALF_HEIGHT;

            float rotated_x = x * angle_cos - y * angle_sin;
            float rotated_y = x * angle_sin + y * angle_cos;

            // Projection
            float px = rotated_x / z * SCALE;
            float py = (rotated_y / z + camera_z) * SCALE;

            // Floor pixel position and color
            int floor_pos_x = abs((int)px) % TEXTURE_SIZE;
            int floor_pos_y = abs((int)py) % TEXTURE_SIZE;

            int floor_color = floor_texture[floor_pos_x + floor_pos_y * TEXTURE_SIZE];

            int floor_color_r = (int)(((floor_color & 0xff0000) >> 16) * fog);
            int floor_color_g = (int)(((floor_color & 0x00ff00) >> 8) * fog);
            int floor_color_b = (int)((floor_color & 0x0000ff) * fog);
            floor_color = (floor_color_r << 16) + (floor_color_g << 8) + floor_color_b;

            buffer[i + j * WIDTH] = floor_color;
            buffer[i + (HEIGHT - j) * WIDTH] = floor_color;
        }
    }
}

int main(void) {
    struct mfb_window *window = mfb_open_ex("Mode7", WIDTH, HEIGHT, WF_RESIZABLE);
    if (!window)
        return 0;

    uint32_t *buffer = calloc(WIDTH * HEIGHT, sizeof(uint32_t));
    struct mfb_timer *timer = mfb_timer_create();
    mfb_timer_reset(timer);
    float camera_z = 0.0f;

    do {
        float delta_time = (float)mfb_timer_delta(timer);
        printf("frame time: %fs\n", delta_time);
        camera_z += CAMERA_SPEED * delta_time;

        render_frame(buffer, camera_z);

        int state = mfb_update_ex(window, buffer, WIDTH, HEIGHT);

        if (state < 0) {
            window = NULL;
            break;
        }
    } while (mfb_wait_sync(window));

    mfb_timer_destroy(timer);
}
