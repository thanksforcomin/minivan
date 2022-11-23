#define GLEW_STATIC
#include "renderer.h"

#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <functional>
#include <future>

#include "indexBuffer.h"
#include "vertexBuffer.h"
#include "vertexArray.h"
#include "vertexBufferLayout.h"
#include "shader.h"
#include "renderer.h"
#include "texture.h"
#include "camera.h"
#include "controls.h"
#include "TEST_cube.h"
//#include "cursor.h"
#include "image.h"
#include "defines.h"


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std::placeholders;

int main(void)
{
    typedef glm::vec3 Vector3;
    typedef glm::mat4 Matrix4;

    constexpr unsigned int width = 540;
    constexpr unsigned int height = 720;
    float deltaTime = 0;
    float lastTime = 0;
    float currTime = 0;
    char err_buffer;

    GLFWwindow* window = 0;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(height, width, "Minivan engine", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
    	std::cout << "Failed to initialize GLAD" << std::endl;
    	return -1;
		}

    std::cout << glGetString(GL_VERSION) << std::endl;

    std::cout << "Minivan engine prototype compiled" << std::endl;

    glfwSwapInterval(0);

    float vertices[] = {
    -0.5f, -0.5f, -0.5f, /*tex coords*/ 0.0f, 0.0f, /*normal*/ 0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f, /*tex coords*/ 1.0f, 0.0f, /*normal*/ 0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f, /*tex coords*/ 1.0f, 1.0f, /*normal*/ 0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f, /*tex coords*/ 1.0f, 1.0f, /*normal*/ 0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f, /*tex coords*/ 0.0f, 1.0f, /*normal*/ 0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f, /*tex coords*/ 0.0f, 0.0f, /*normal*/ 0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f, /*tex coords*/ 0.0f, 0.0f, /*normal*/ 0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, /*tex coords*/ 1.0f, 0.0f, /*normal*/ 0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f, /*tex coords*/ 1.0f, 1.0f, /*normal*/ 0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f, /*tex coords*/ 1.0f, 1.0f, /*normal*/ 0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, /*tex coords*/ 0.0f, 1.0f, /*normal*/ 0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, /*tex coords*/ 0.0f, 0.0f, /*normal*/ 0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, /*tex coords*/ 1.0f, 0.0f, /*normal*/ -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, /*tex coords*/ 1.0f, 1.0f, /*normal*/ -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, /*tex coords*/ 0.0f, 1.0f, /*normal*/ -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, /*tex coords*/ 0.0f, 1.0f, /*normal*/ -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, /*tex coords*/ 0.0f, 0.0f, /*normal*/ -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, /*tex coords*/ 1.0f, 0.0f, /*normal*/ -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f, /*tex coords*/ 1.0f, 0.0f, /*normal*/ 1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f, /*tex coords*/ 1.0f, 1.0f, /*normal*/ 1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f, /*tex coords*/ 0.0f, 1.0f, /*normal*/ 1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f, /*tex coords*/ 0.0f, 1.0f, /*normal*/ 1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f, /*tex coords*/ 0.0f, 0.0f, /*normal*/ 1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f, /*tex coords*/ 1.0f, 0.0f, /*normal*/ 1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f, /*tex coords*/ 0.0f, 1.0f, /*normal*/ 0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f, /*tex coords*/ 1.0f, 1.0f, /*normal*/ 0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f, /*tex coords*/ 1.0f, 0.0f, /*normal*/ 0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f, /*tex coords*/ 1.0f, 0.0f, /*normal*/ 0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, /*tex coords*/ 0.0f, 0.0f, /*normal*/ 0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, /*tex coords*/ 0.0f, 1.0f, /*normal*/ 0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f, /*tex coords*/ 0.0f, 1.0f, /*normal*/ 0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f, /*tex coords*/ 1.0f, 1.0f, /*normal*/ 0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f, /*tex coords*/ 1.0f, 0.0f, /*normal*/ 0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f, /*tex coords*/ 1.0f, 0.0f, /*normal*/ 0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, /*tex coords*/ 0.0f, 0.0f, /*normal*/ 0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, /*tex coords*/ 0.0f, 1.0f, /*normal*/ 0.0f,  1.0f,  0.0f
    };

    glm::vec3 cube_positions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    //model
    Matrix4 model = Matrix4(1.0f);
    model = glm::translate(model, Vector3(0.0, 0.0, 0.0));

    //model cube test

    TEST_cube cube;
    TEST_cube lamp(Vector3(1.2, 1.0, 2.0));
    lamp.resize(Vector3(0.2));

    //camera new

    Camera cam(std::ref(deltaTime));

    //projection
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)height / (float)width, 0.1f, 100.0f);

    vertexArray va;
    vertexArray lamp_va;
    vertexBuffer vb(vertices, sizeof(vertices));

    vertexBufferLayout geometryLayout;
    geometryLayout.push<float>(3);
    geometryLayout.push<float>(2);
    geometryLayout.push<float>(3);
    va.addBuffer(vb, geometryLayout);

    vertexBufferLayout lamp_geometryLayout;
    lamp_geometryLayout.push<float>(3);
    lamp_geometryLayout.push<float>(2);
    lamp_geometryLayout.push<float>(3);
    lamp_va.addBuffer(vb, lamp_geometryLayout);

    shader sh("../res/Shaders/Phong.shader");
    shader lamp_shader("../res/Shaders/LampShader.shader");

    //customCursor cursor("Res/Textures/salo.png");

    Controls controller;
    controller.bindCursor(std::bind(&Camera::cameraMouseInput, &cam, _1, _2, _3));

    //controller.bindCursor(std::bind(&TEST_cube::mouse_callback, &cube, _1, _2, _3));

    void* example_data = new char;

    //lazyLoader<image> loader;
    //loader.push("Res/Textures/jinx.jpg", std::bind((image* obj)[] { /*some code*/ std::cout << "help me\n"}));

    glm::vec3 lightColor(1.0, 1.0, 1.0);
    float ambient_strenght = 1.0;

    texture tx("../res/Textures/container.png");
    texture tx_specular("../res/Textures/container_specular.png");
    texture tx_emission("../res/Textures/matrix.jpg");
    lamp_shader.setUniform3fv("lamp_color", lightColor);
    sh.setUniform3fv("camera_pos", cam.position());
    tx.bind(0);
    sh.setUniform1i("material.diffuse", 0);
    tx_specular.bind(1);
    sh.setUniform1i("material.specular", 1);
    sh.setUniform1f("material.shiny", 50.0f);

    sh.setUniform3fv("lighting.position", lamp.position());
    sh.setUniform3f("lighting.ambient", 0.2f, 0.2f, 0.2f);
    sh.setUniform3f("lighting.diffuse", 0.5f, 0.5f, 0.5f);
    sh.setUniform3f("lighting.specular", 1.0f, 1.0f, 1.0f);

    sh.setUniform1f("lighting.constant", 1.0f);
    sh.setUniform1f("lighting.lin", 0.09f);
    sh.setUniform1f("lighting.quad", 0.032f);

    sh.setUniform1f("lighting.cutOff", glm::cos(glm::radians(12.5f)));
    sh.setUniform1f("lighting.outerCutOff", glm::cos(glm::radians(20.0f)));

    glEnable(GL_DEPTH_TEST);
    
    std::cout << "Error " << glGetError() << "\n\n";

    //cam.setCameraPos(Vector3(0.0, 0.0, 4.0));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        currTime = glfwGetTime();
        deltaTime = currTime - lastTime;
        
        //std::cout << "Framerate:  " << 1 / deltaTime << " FPS" << std::endl;

        /* Render here */
        glClearColor(0.1, 0.1, 0.1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //controller.processFastInput();

        if (controller.getKeyPressed(GLFW_KEY_W)) cam.moveForward();
        if (controller.getKeyPressed(GLFW_KEY_A)) cam.moveLeft();
        if (controller.getKeyPressed(GLFW_KEY_S)) cam.moveBackward();
        if (controller.getKeyPressed(GLFW_KEY_D)) cam.moveRight();

        sh.setUniform3fv("lighting.position", cam.position());
        sh.setUniform3fv("lighting.direction", cam.front());

        //std::cout << "X offset " << controller.getXoffset() << std::endl;
        //std::cout << "Y offset " << controller.getYoffset() << std::endl;

        //std::cout << "1! Error " << glGetError() << "\n";

        //sh.setUniform3fv("lighting.ambient", ambientColor);
        //sh.setUniform3fv("lighting.diffuse", diffuseColor);

        //sh.setUniform1f("time", currTime);

        sh.setUniformMatrix4f("projection", projection);
        sh.setUniformMatrix4f("camera", cam.getView());
        sh.setUniform3fv("camera_pos", cam.position());
        sh.bind();
        va.bindArray();

        for (glm::vec3 pos : cube_positions) {
            cube.set_position(pos);
            cube.rotate(pos, 20.0f);
            sh.setUniformMatrix4f("model", cube.getModel());
            sh.setUniformMatrix4f("normal_model", glm::transpose(glm::inverse(cube.getModel())));
            glDrawArrays(GL_TRIANGLES, 0, va.getCount());
        }

        //std::cout << "2! Error " << glGetError() << "\n";
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        lastTime = currTime;

        /* Poll for and process events */
        glfwPollEvents();
    }

    //tx.~texture();
    sh.~shader();

    std::cout << "Application closed\n";
    
    glfwTerminate();
    return 0;
}
