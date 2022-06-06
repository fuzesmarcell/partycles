/*
====================================================================
Particle simulator written via the https://www.cs.cmu.edu/~baraff/sigcourse/notesc.pdf
notes.

$File: $
$Date: $
$Revision: $
$Createor: Fuzes Marcell $
$Notice: (C) Copyright 2022 by Fuzes Marcell, All Rights Reserved. $
====================================================================
*/

#include <raylib.h>

struct Particle {
  float m;
  float* x;  // x, y
  float* v;  // vx, vy
  float* f;
};

struct ParticleSystem {
  Particle* p;
  int n;
  float t;
};

static int particle_dims(ParticleSystem* ps) {
  return 4 * ps->n;
}

static void particle_get_state(ParticleSystem* ps, float* dst) {
  for (int i = 0; i < ps->n; i++) {
    *(dst++) = ps->p[i].x[0];
    *(dst++) = ps->p[i].x[1];
    *(dst++) = ps->p[i].v[0];
    *(dst++) = ps->p[i].v[1];
  }
}

static void particle_set_state(ParticleSystem* ps, float* src) {
  for (int i = 0; i < ps->n; i++) {
    ps->p[i].x[0] = *(src++);
    ps->p[i].x[1] = *(src++);
    ps->p[i].v[0] = *(src++);
    ps->p[i].v[1] = *(src++);
  }
}

static void clear_forces(ParticleSystem* ps) {
  for (int i = 0; i < ps->n; i++) {
    ps->p[i].f[0] = 0.f;
    ps->p[i].f[1] = 0.f;
  }
}

void compute_forces(ParticleSystem* ps);

static void particle_derivate(ParticleSystem* ps, float* dst) {
  clear_forces(ps);
  compute_forces(ps);
  for (int i = 0; i < ps->n; i++) {
    float m = ps->p[i].m;
    *(dst++) = ps->p[i].v[0];  // xdot = v
    *(dst++) = ps->p[i].v[1];
    *(dst++) = ps->p[i].f[0] / m;  // vdot = f/m
    *(dst++) = ps->p[i].f[1] / m;
  }
}

void scale_vector(float* src, float dt);
void add_vectors(float* a, float* b, float* result);

static void euler_step(ParticleSystem* ps, float dt) {
  float* temp1;
  float* temp2;

  particle_derivate(ps, temp1);
  scale_vector(temp1, dt);
  particle_get_state(ps, temp2);
  add_vectors(temp1, temp2, temp2);
  particle_set_state(ps, temp2);
  ps->t += dt;
}

int main(void) {
  // Initialization
  //--------------------------------------------------------------------------------------
  const int screen_width = 1980;
  const int screen_heigth = 1080;

  InitWindow(screen_width, screen_heigth, "Party simulator");

  Vector2 pos = {(float)screen_width / 2, (float)screen_heigth / 2};

  SetTargetFPS(144);

  // Main game loop
  while (!WindowShouldClose()) {
    // Update
    //----------------------------------------------------------------------------------
    if (IsKeyDown(KEY_RIGHT))
      pos.x += 2.0f;
    if (IsKeyDown(KEY_LEFT))
      pos.x -= 2.0f;
    if (IsKeyDown(KEY_UP))
      pos.y -= 2.0f;
    if (IsKeyDown(KEY_DOWN))
      pos.y += 2.0f;
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText("move the ball with arrow keys", 10, 10, 20, DARKGRAY);

    // DrawPixel(pos.x, pos.y, MAROON);
    DrawCircleV(pos, 10, MAROON);

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow();
  //--------------------------------------------------------------------------------------

  return 0;
}
