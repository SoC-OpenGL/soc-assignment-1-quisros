//
//  ring.cpp
//  glinat0
//
//  Created by Shabnam Sahay on 28/03/20.
//  Copyright © 2020 PAR. All rights reserved.
//

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include"utility/stb_image.h"

#include<iostream>
#include<cmath>
using namespace std;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

float degtorad(int degrees) {
    const float pi = 3.14159;
    return (degrees*pi)/180;
}



// settings
const unsigned int SCR_WIDTH = 700;
const unsigned int SCR_HEIGHT = 700;

//vertex shader code

const char *vertexShaderSource = "#version 330 core\n"

    "layout(location = 0) in vec3 aPos;\n"
    "layout(location = 1) in vec3 aColor;\n"
    "layout(location = 2) in vec2 aTexCoord;\n"

    "out vec3 ourColor;\n"
    "out vec2 TexCoord;\n"

    "void main ()\n"
    "{\n"
    "    gl_Position = vec4(aPos, 1.0);\n"
    "    ourColor = aColor;\n"
    "    TexCoord = aTexCoord;\n"
    "}\0";


//fragment shader code

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D ourTexture;\n"
    "void main ()\n"
    "{\n"
    "    FragColor = texture(ourTexture, TexCoord);\n"
    "}\0";



int main () {
    
    
    
    //glfw: initialize and configure
    
    glfwInit();
    if (!glfwInit ()) { // glfwInit() attempts to initialize GLFW lib
        cerr << "ERROR: could not start GLFW3" << endl;
        return 1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Textured Ring", NULL, NULL);
    
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glViewport(0,0,SCR_WIDTH,SCR_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    
    
    
    //start GLEW extension handler
    glewExperimental = GL_TRUE; //enables experimental extensions
    glewInit();
    
    
    
    
    //creating vertex shader object
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //attach shader source code to object and compile shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);


    //checking if vertex shader compilation was successful

    int success;

    //storage container for error msgs (if any)
    char infoLog[512];

    //check if compilation successful
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS, &success);

    if(!success) {
        //retrieve error message with given fn and print the error msg
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    }
    
    //creating and compiling fragment shader object
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);


    //checking if fragment shader compilation was successful
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS, &success);

    if(!success) {
        //retrieve error message with given fn and print the error msg
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
    }

    //creating shader program object
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
        
    //attaching previously compiled shaders to it, then linking them
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
        
    //checking if linking failed
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }
    

    //deleting initial shaders which are now redundant
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    
    
    
    // set up vertex data (and buffer(s)) and configure vertex attributes
    
    float r = 0.75;
    
    float vertices[360*8];
    
    /*
    
    for(int i = 0; i<8; i++) {
        if (i<6) {
            vertices[i] = 0.0;
        }
        else {
            vertices[i] = 0.5;
        }
    }
     
    */
    
    for(int i = 0; i<360; i++) {
        float theta = degtorad(i);
        int crindex = i*8;
        vertices[crindex] = r*cos(theta);
        vertices[crindex+1] = r*sin(theta);
        vertices[crindex+2] = 0.0;
        vertices[crindex+3] = 0.0;
        vertices[crindex+4] = 0.0;
        vertices[crindex+5] = 0.0;
        vertices[crindex+6] = (0.5*cos(theta)) + 0.5;
        vertices[crindex+7] = (0.5*sin(theta)) + 0.5;
    }
    
    
    
    //generating vao and vbo
    unsigned int VAO, VBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    
    //load and create a texture
    
    unsigned int texture;  //id which references texture
    glGenTextures(1, &texture);
    
    //bind it so that subsequent texture commands configure currently bound texture
    glBindTexture(GL_TEXTURE_2D, texture);
    
    //set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    //set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    //load image, create texture and generate mipmaps
    int width, height, nrChannels;
    unsigned char *data = stbi_load("textures/tripixels.jpg", &width, &height, &nrChannels, 0);
    
    //now that texture is bound, generating the texture using image data
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        //generate all required mipmaps for currently bound texture
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture" << endl;
    }
    
    //free the image memory now
    stbi_image_free(data);
    

    
    // render loop
    
    while(!glfwWindowShouldClose(window)) {
        
        //input
        processInput(window);
        
        //render
        
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //activating shader program
        glUseProgram(shaderProgram);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        
        //drawing the ring
        for (int i = 0; i<359; i++) {
            glDrawArrays(GL_LINES, i, 2);
        }
        
        glBindVertexArray(0);
        
        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    //clear all previously allocated GLFW resources
    glfwTerminate();
    return 0;
    
}
