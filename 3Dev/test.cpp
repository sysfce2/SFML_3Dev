#include <Engine.h>

int main()
{
    Engine engine;
    
    engine.GetSettings().depthBits = 24;
    engine.CreateWindow(1280, 720, "test");
    engine.Init(45, 1280, 720, 0.1, 5000);
    
    engine.GetWindow().setMouseCursorVisible(false);
    engine.GetWindow().setVerticalSyncEnabled(true);
    engine.GetWindow().setFramerateLimit(120);

    glewInit();

    engine.EventLoop([&](sf::Event& event) { if(event.type == sf::Event::Closed) engine.Close(); });
    
    Light l(GL_LIGHT0, 0, 5, 0);
    Camera cam(0, 0, -10, 1);
    Model teapot("resources/models/teapot.obj", "resources/textures/gold.jpg", 0, 0, 0);

    GLuint skybox[6] = {
        LoadTexture("resources/skybox/skybox_front.bmp"),
        LoadTexture("resources/skybox/skybox_back.bmp"),
        LoadTexture("resources/skybox/skybox_left.bmp"),
        LoadTexture("resources/skybox/skybox_right.bmp"),
        LoadTexture("resources/skybox/skybox_bottom.bmp"),
        LoadTexture("resources/skybox/skybox_top.bmp")
    };

    engine.Loop([&]() 
    { 
        cam.Move(1);
        cam.Mouse(engine.GetWindow());
        cam.Look();

        l.Update();

        teapot.Draw();

        glDisable(GL_LIGHTING);
        Shape::Draw(skybox, cam.x, cam.y, cam.z, 1000, 1000, 1000);
        glEnable(GL_LIGHTING);
    });

    engine.Launch();
}