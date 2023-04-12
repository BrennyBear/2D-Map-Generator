////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// Author: Brennen Romo                                                                                       ////////////////  
//////////////// Date: April 7th, 2023                                                                                      ////////////////
//////////////// Project: Windows Renderer                                                                                  ////////////////
//////////////// Description: This Program creates a window, which we can resize, set the title of,Maximize/minimize with   ////////////////
////////////////               the Space Bar                                                                                ////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <Windows.h>
#include <iostream>
#include <ctime>
#include <stdlib.h> 
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glm.hpp"

#include "Shader.h"

void windowSizeCallback(GLFWwindow* window, int width, int height);
void windowCloseCallback(GLFWwindow* window);
void keyboardInput(GLFWwindow* window, float& randy);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

/////////////////////// Global Settings ////////////////////////////////////////////////////////////////////////////////////////////
const int screenHeight = 1200;
const int screenWidth = 1600;

/////////////////////// Global Data //////////////////////////////////////////////////////////////////////////////////////////////
// Vertex Data for our Rectangle
float vertices[] = {
         1.0,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
         1.0, -1.0, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom right
        -1.0, -1.0, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // bottom left
        -1.0,  1.0, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left 
};
// Index Data for our Rectangle
unsigned int indices[] = { 
    0, 1, 3,  // 1st Primitive
    1, 2, 3   // 2nd Primitive
};


static float randy = 0;
/////////////////////////// Main Program /////////////////////////////////////////////////////////////////////////

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    //initialize GLFW
    if (!glfwInit())
    {
        //close program if we've failed to run GLFW
        std::cout << "failed to initialize GLFW";
        return -1;
    }
    //Definitions for when we are using Apple device
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    //Set GLFW Window Parameters here (Using version 4.6)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    //using Core profile of OpenGL, (Compatibility profile is for legacy)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //////////////////////////create Fullscreen Window here////////////////////////
    // 
     GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Render Window", glfwGetPrimaryMonitor(), NULL);

    //////////////////////////create Windowed fullscreen here////////////////////////

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    //glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    //glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    //glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    //glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    //GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Render Window", NULL, NULL);


    ////////////////////////create window here////////////////////////////////
    
   // GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Render Window", NULL, NULL);

    //Check if Window was created properly
    if (!window)
    {
        std::cout << "Error Creating our GLFW Window!" << std::endl;
        //close down GLFW
        glfwTerminate();
        return -1;
    }
    // Make our Window the current Context
    glfwMakeContextCurrent(window);

    /////////////////////////////////////////////////////////////////////////////////
    // /////////////////////////// Set Callback Functions Here ///////////////////////////
    // /////////////////////////////////////////////////////////////////////////////////
    

    //Sets the Callback function for when the frameBuffer is resized to our custom Callback function.
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    //Set callback Function for when Window is resized
    glfwSetWindowSizeCallback(window, windowSizeCallback);

    //set callback function for when we close glfw Window 
    glfwSetWindowCloseCallback(window, windowCloseCallback);

    // Start glad and load all openGL function pointers (for whichever specific system and archritecture our program is running on)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////// Shaders //////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////


    Shader ourShader("shader.vs", "shader.fs");
    Shader noiseShader("shader.vs", "sNoise.fs");


    //declare our Vertex Buffer Object, Vertex Attricute Object, and Element Buffer Object
    unsigned int VBO, VAO, EBO;
    //Generate VAO, VBO,EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // First, Bind our Vertex Array Object
    glBindVertexArray(VAO);
    // Bind our VBO now that we have a VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //Static Draw for data reused many times without changing

    // Bind our EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //Specify how our Vertex Data is laid out. (x,y,z position coordinates) 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Specify how our Vertex Data is laid out. (r,g,b colour attributes) 
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //Unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Unbind EBO (Only after VAO is done being used
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //Unbind VAO
    glBindVertexArray(0);


    // WireFrame Mode!
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////// Textures   ////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////



    noiseShader.use();
    float randy = 0;
    std::srand(time(NULL));
    randy = rand();
    noiseShader.setFloat("u_time",randy);
    noiseShader.setVec2("u_resolution", mode->width, mode->height);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    // Start Render Loop here
    do
    {
        //input
        keyboardInput(window,randy);

        //clear the backbuffer to set colour
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

 


        // draw our first triangle
        noiseShader.use();
        std::cout << randy << std::endl;
        noiseShader.setFloat("u_time", randy);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time 



        //Swap buffers
        glfwSwapBuffers(window);
        // poll windows events, call callback functions for events
        glfwPollEvents();
    } while (!glfwWindowShouldClose(window));

    //Delete our Buffers
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);



    // close GLFW
    glfwTerminate();
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// FUNCTION DEFINITIONS           ////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// Function for Processing Inputs
void keyboardInput(GLFWwindow* window, float& randy)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) //if Escape key is pressed, close Window
        glfwSetWindowShouldClose(window, true);
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) //if Escape key is pressed, close Window
    {
        int maximized = glfwGetWindowAttrib(window, GLFW_MAXIMIZED);
        if (!maximized)
            glfwMaximizeWindow(window);
        else
            glfwRestoreWindow(window);
    }
    else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        std::srand(time(NULL)*128);
        randy = rand();
        
    }
}


//Function callback for changing our Window's size
void windowSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    std::cout << "Width: " << width << "\n" << "Height: " << height << "\n";
}

//Function callback for closing our Window
void windowCloseCallback(GLFWwindow* window)
{
    std::cout << "Goodbye!";
}



//Function for when screen is resized
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    //Set viewport size to new viewport size. 
    glViewport(0, 0, width, height);
}

