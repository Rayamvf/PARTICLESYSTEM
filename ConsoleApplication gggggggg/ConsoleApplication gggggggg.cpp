#include "raylib.h"
#include <vector>
#include <algorithm>
#include <cmath>


struct Particle {
    Vector2 position;
    Vector2 velocity;
    float radius;
    Color color;
    float lifeTime;
    bool active;

};


Particle CreateParticle(Vector2 position) {
    Particle p;
    p.position = position;
    p.velocity = { (float)GetRandomValue(-100, 100) / 100.0f, (float)GetRandomValue(-100, 100) / 100.0f };
    p.radius = 3.0f;
    p.color = BLACK;
    p.lifeTime = 2.0f; // Partikel bleibt für 2 Sekunden aktiv
    p.active = true;
    return p;
   
}



int main() {
    
    const int screenWidth = 1600;
    const int screenHeight = 900;
    InitWindow(screenWidth, screenHeight, "Partikelsystem mit Physik");

   
    std::vector<Particle> particles;
    float particleSpawnTimer = 0.001f; // Zeitintervall zwischen dem Erzeugen neuer Partikel
    float timeSinceLastSpawn = 0.0f;

    Image icon = LoadImage("2898362(1).png");

    SetWindowIcon(icon);
    UnloadImage(icon);

   
    const Vector2 gravity = { 0.0f, 9.81f }; // Schwerkraft
    const float friction = 0.99f; // Reibung
    const float attractionStrength = 100.0f; // Stärke der Anziehungskraft zur Maus

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        
        float deltaTime = GetFrameTime();
        Vector2 mousePosition = GetMousePosition();

        timeSinceLastSpawn += deltaTime;
        if (timeSinceLastSpawn >= particleSpawnTimer) {
            // Neuer Partikel wird erzeugt
            particles.push_back(CreateParticle({ screenWidth / 2.0f, screenHeight / 2.0f }));
            timeSinceLastSpawn = 0.0f;
        }

        for (auto& p : particles) {
            if (p.active) {
                // Schwerkraft anwenden
                p.velocity.x += gravity.x * deltaTime;
                p.velocity.y += gravity.y * deltaTime;

                // Reibung anwenden
                p.velocity.x *= friction;
                p.velocity.y *= friction;

                // Anziehungskraft zur Maus anwenden
                Vector2 direction = { mousePosition.x - p.position.x, mousePosition.y - p.position.y };
                float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
                if (distance > 0) {
                    direction.x /= distance;
                    direction.y /= distance;
                }
                p.velocity.x += direction.x * attractionStrength * deltaTime;
                p.velocity.y += direction.y * attractionStrength * deltaTime;
                if (IsKeyDown(KEY_G)) {
                    p.velocity.x -= direction.x * attractionStrength * deltaTime;
                    p.velocity.y -= direction.y * attractionStrength * deltaTime;
                }
                
                p.position.x += p.velocity.x;
                p.position.y += p.velocity.y;

                
                if (p.position.x <= p.radius || p.position.x >= screenWidth - p.radius) {
                    p.velocity.x *= -1;
                }
                if (p.position.y <= p.radius || p.position.y >= screenHeight - p.radius) {
                    p.velocity.y *= -1;
                }

               
                p.lifeTime -= deltaTime;
                p.color.a = (unsigned char)(255 * (p.lifeTime / 2.0f)); 

                if (p.lifeTime <= 0) {
                    p.active = false;
                }
            }
        }

        // Zeichnen
        BeginDrawing();
        ClearBackground(WHITE);

        for (const auto& p : particles) {
            if (p.active) {
                DrawCircleV(p.position, p.radius, p.color);
            }
        }
        
        DrawText("Partikelsystem mit Physik", 10, 10, 20, BLACK);
        DrawText("G for stabilisierung ", 10, 80, 20, BLACK);

        EndDrawing();

        // Entferne inaktive Partikel aus der Liste
        particles.erase(std::remove_if(particles.begin(), particles.end(), [](const Particle& p) { return !p.active; }), particles.end());
    }

    CloseWindow();

    return 0;
}
