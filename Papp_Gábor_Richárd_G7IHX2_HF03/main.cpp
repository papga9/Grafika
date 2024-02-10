#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "shader.h"
#include "camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
static glm::vec3 calculateBoxPosition();
void scroll_callback(GLFWwindow* window,  double xoffset, double yoffset);



unsigned int loadTexture(char const* path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));

bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0;
float lastY = SCR_HEIGHT / 2.0;

glm::vec3 lightPos = glm::vec3(20.0f, 20.0f, 30.0f);

bool movement[4] = {
    false,
    false,
    false,
    false
};

const float sun_max = 500.0f;
int sun = sun_max;


struct Texture {
    unsigned int diffuse;
    unsigned int specular;
};
struct BuildingBlock {
public:
    glm::vec3 position;
    Texture texture;
    float scale;
    float angle[3];

    BuildingBlock(int x, int y, int z,Texture tx, float scale):position(glm::vec3(x,y,z)), texture(tx), scale(scale) {
        angle[0] = 0.0f;
        angle[1] = 0.0f;
        angle[1] = 0.0f;
    }

    BuildingBlock():position(glm::vec3(0,0,0)), texture(Texture()), scale(1.0f) {
    }
};



std::vector<BuildingBlock> cubePositions;


unsigned currentTextureIndex;

BuildingBlock indicatorBlock;

float angle_x;
float angle_y;
std::vector<Texture> textures;

int main() {

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetScrollCallback(window, scroll_callback);





    float etc[10 * 8] = {
    400.0f, -4.0f, 400.0f, 100.0f, 100.0f, 0.0f, 1.0f, 0.0f,
    400.0f, -4.0f,-400.0f, 100.0f, -100.0f, 0.0f, 1.0f, 0.0f,
    -400.0f,-4.0f, -400.0f, -100.0f, -100.0f, 0.0f, 1.0f, 0.0f,
    -400.0f,-4.0f,-400.0f, -100.0f, -100.0f, 0.0f, 1.0f, 0.0f,
    -400.0f,-4.0f,400.0f, -100.0f, 100.0f, 0.0f, 1.0f, 0.0f,
    400.0f, -4.0f,400.0f, 100.0f, 100.0f, 0.0f, 1.0f, 0.0f,

    -0.002f, 0.0f, -0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0019f, 0.0f, -0.2f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.002f, -0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, -0.0019f, -0.2f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f
    };


    float cube[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

   

    std::vector<float> vertices;

    for (int i = 0; i < sizeof(etc) / sizeof(float); i++)
        vertices.push_back(etc[i]);


    int resolution = 4;
    for (unsigned int i = 0; i <= 180 / resolution; i++) {
        for (unsigned int j = 0; j <= 360 / resolution; j++) {
            vertices.push_back(sin(glm::radians((float)resolution * i)) * cos(glm::radians((float)resolution * j)));
            vertices.push_back(cos(glm::radians((float)resolution * i)));
            vertices.push_back(sin(glm::radians((float)resolution * i)) * sin(glm::radians((float)resolution * j)));
            vertices.push_back(1.0f - glm::radians((float)resolution * j) / glm::radians(180.0f));
            vertices.push_back(1.1f - glm::radians((float)resolution * i) / glm::radians(180.0f));
            vertices.push_back(sin(glm::radians((float)resolution * i)) * cos(glm::radians((float)resolution * j)));
            vertices.push_back(cos(glm::radians((float)resolution * i)));
            vertices.push_back(sin(glm::radians((float)resolution * i)) * sin(glm::radians((float)resolution * j)));

            vertices.push_back(sin(glm::radians((float)resolution * (i + 1))) * cos(glm::radians((float)resolution * j)));
            vertices.push_back(cos(glm::radians((float)resolution * (i + 1))));
            vertices.push_back(sin(glm::radians((float)resolution * (i + 1))) * sin(glm::radians((float)resolution * j)));
            vertices.push_back(1.0f - glm::radians((float)resolution * j) / glm::radians(180.0f));
            vertices.push_back(1.1f - glm::radians((float)resolution * (i + 1)) / glm::radians(180.0f));
            vertices.push_back(sin(glm::radians((float)resolution * (i + 1))) * cos(glm::radians((float)resolution * j)));
            vertices.push_back(cos(glm::radians((float)resolution * (i + 1))));
            vertices.push_back(sin(glm::radians((float)resolution * (i + 1))) * sin(glm::radians((float)resolution * j)));

            vertices.push_back(sin(glm::radians((float)resolution * (i + 1))) * cos(glm::radians((float)resolution * (j + 1))));
            vertices.push_back(cos(glm::radians((float)resolution * (i + 1))));
            vertices.push_back(sin(glm::radians((float)resolution * (i + 1))) * sin(glm::radians((float)resolution * (j + 1))));
            vertices.push_back(1.0f - glm::radians((float)resolution * (j + 1)) / glm::radians(180.0f));
            vertices.push_back(1.1f - glm::radians((float)resolution * (i + 1)) / glm::radians(180.0f));
            vertices.push_back(sin(glm::radians((float)resolution * (i + 1))) * cos(glm::radians((float)resolution * (j + 1))));
            vertices.push_back(cos(glm::radians((float)resolution * (i + 1))));
            vertices.push_back(sin(glm::radians((float)resolution * (i + 1))) * sin(glm::radians((float)resolution * (j + 1))));

            vertices.push_back(sin(glm::radians((float)resolution * i)) * cos(glm::radians((float)resolution * j)));
            vertices.push_back(cos(glm::radians((float)resolution * i)));
            vertices.push_back(sin(glm::radians((float)resolution * i)) * sin(glm::radians((float)resolution * j)));
            vertices.push_back(1.0f - glm::radians((float)resolution * j) / glm::radians(180.0f));
            vertices.push_back(1.1f - glm::radians((float)resolution * i) / glm::radians(180.0f));
            vertices.push_back(sin(glm::radians((float)resolution * i)) * cos(glm::radians((float)resolution * j)));
            vertices.push_back(cos(glm::radians((float)resolution * i)));
            vertices.push_back(sin(glm::radians((float)resolution * i)) * sin(glm::radians((float)resolution * j)));

            vertices.push_back(sin(glm::radians((float)resolution * (i + 1))) * cos(glm::radians((float)resolution * (j + 1))));
            vertices.push_back(cos(glm::radians((float)resolution * (i + 1))));
            vertices.push_back(sin(glm::radians((float)resolution * (i + 1))) * sin(glm::radians((float)resolution * (j + 1))));
            vertices.push_back(1.0f - glm::radians((float)resolution * (j + 1)) / glm::radians(180.0f));
            vertices.push_back(1.1f - glm::radians((float)resolution * (i + 1)) / glm::radians(180.0f));
            vertices.push_back(sin(glm::radians((float)resolution * (i + 1))) * cos(glm::radians((float)resolution * (j + 1))));
            vertices.push_back(cos(glm::radians((float)resolution * (i + 1))));
            vertices.push_back(sin(glm::radians((float)resolution * (i + 1))) * sin(glm::radians((float)resolution * (j + 1))));

            vertices.push_back(sin(glm::radians((float)resolution * i)) * cos(glm::radians((float)resolution * (j + 1))));
            vertices.push_back(cos(glm::radians((float)resolution * i)));
            vertices.push_back(sin(glm::radians((float)resolution * i)) * sin(glm::radians((float)resolution * (j + 1))));
            vertices.push_back(1.0f - glm::radians((float)resolution * (j + 1)) / glm::radians(180.0f));
            vertices.push_back(1.1f - glm::radians((float)resolution * i) / glm::radians(180.0f));
            vertices.push_back(sin(glm::radians((float)resolution * i)) * cos(glm::radians((float)resolution * (j + 1))));
            vertices.push_back(cos(glm::radians((float)resolution * i)));
            vertices.push_back(sin(glm::radians((float)resolution * i)) * sin(glm::radians((float)resolution * (j + 1))));
        }
    }
    

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);



    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int cubeVBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glEnableVertexAttribArray(0);





    
    std::string path = "grass.jpg";
    unsigned int ground_diffuse = loadTexture(path.c_str());
    path = "grass.jpg";
    unsigned int ground_specular = loadTexture(path.c_str());

    path = "sun.jpg";
    unsigned int sun_diffuse = loadTexture(path.c_str());

    path = "wood.jpg";
    unsigned int wood = loadTexture(path.c_str());
    path = "wood.jpg";
    unsigned int wood2 = loadTexture(path.c_str());

    path = "stone.jpg";
    unsigned int stone_diffuse = loadTexture(path.c_str());
    path = "stone.jpg";
    unsigned int stone_specular = loadTexture(path.c_str());

    path = "metal.jpg";
    unsigned int metal_diffuse = loadTexture(path.c_str());
    path = "metal_specular.jpg";
    unsigned int metal_specular = loadTexture(path.c_str());




    Texture groundTexture;
    groundTexture.diffuse = ground_diffuse;
    groundTexture.specular = ground_specular;

    Texture woodTexture;
    woodTexture.diffuse = wood;
    woodTexture.specular = wood2;

    Texture metalTexture;
    metalTexture.diffuse = metal_diffuse;
    metalTexture.specular = metal_specular;

    Texture stoneTexture;
    stoneTexture.diffuse = stone_diffuse;
    stoneTexture.specular = stone_specular;

    

    textures.push_back(woodTexture);
    textures.push_back(groundTexture);
    textures.push_back(metalTexture);
    textures.push_back(stoneTexture);



    currentTextureIndex = 0;




    glEnable(GL_DEPTH_TEST);

    Shader groundShader("ground.vs", "ground.fs");
    groundShader.use();
    groundShader.setInt("ground.diffuse", 0);
    groundShader.setInt("ground.specular", 1);


    Shader sunShader("sun.vs", "sun.fs");
    sunShader.use();
    sunShader.setInt("texture1", 0);

    Shader modelShader("shader.vs", "shader.fs");
    modelShader.use();
    modelShader.setInt("material.diffuse", 0);
    modelShader.setInt("material.specular", 1);



    Shader colorShader("color.vs", "color.fs");

    double start_time = glfwGetTime();

    double time_limit = 120.0; 
    float deltaTime = 0.0f;

    float lastFrame = 0.0f;


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {

        if (glfwGetTime() - start_time >= time_limit)
            break;
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        // render
        // ------
        glClearColor(0,0,0,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 200.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);



        angle_y = glm::quarter_pi<float>() - acos(length(camera.Up * 1.5f) / length(glm::vec3(0.0f, 0.0f, 100.0f))) / 2.0f;
        angle_x = (glm::half_pi<float>() - acos(length(camera.Right * 1.5f) / length(glm::vec3(0.0f, 0.0f, 100.0f)))) * 0.75f;


        groundShader.use();



        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ground_diffuse);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ground_specular);
        groundShader.setFloat("ground.shininess", 32.0f);

        groundShader.setVec3("light.ambient", glm::vec3(0.8f, 0.8f * sun / sun_max, 0.8f * sun / sun_max));
        groundShader.setVec3("light.diffuse", glm::vec3(0.8f, 0.8f * sun / sun_max, 0.8f * sun / sun_max));
        groundShader.setVec3("light.specular", glm::vec3(0.2f, 0.2f * sun / sun_max, 0.2f * sun / sun_max));
        groundShader.setVec3("light.position", lightPos);
        groundShader.setVec3("viewPos", camera.Position);
        groundShader.setBool("torch.on", false);
        groundShader.setVec3("torch.direction", glm::vec3(glm::rotate(glm::rotate(glm::mat4(1.0f), angle_y, camera.Right), angle_x, camera.Up) * glm::vec4(camera.Front, 1.0f)));
        groundShader.setVec3("torch.ambient", glm::vec3(0.8f));
        groundShader.setVec3("torch.diffuse", glm::vec3(0.8f));
        groundShader.setVec3("torch.specular", glm::vec3(1.0f));
        groundShader.setVec3("torch.position", camera.Position + camera.Front * 4.0f + camera.Right * 1.0f - camera.Up * 1.5f);
        groundShader.setFloat("torch.cutOff", cos(glm::radians(15.0f)));
        groundShader.setFloat("attenuation_const", 1.0f);
        groundShader.setFloat("attenuation_lin", 0.0045f);
        groundShader.setFloat("attenuation_quad", 0.0016f);
        groundShader.setVec3("fogColor", glm::vec3(0.05f));

        groundShader.setMat4("projection", projection);
        groundShader.setMat4("view", view);
        groundShader.setMat4("model", model);



        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);


        sunShader.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sun_diffuse);
        sunShader.setMat4("projection", projection);
        sunShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::translate(model, camera.Position);
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
        model = glm::scale(model, glm::vec3(2.0f));
        sunShader.setMat4("model", model);
        sunShader.setVec3("color", glm::vec3(1.0f, sun / sun_max, sun / sun_max));

        glDrawArrays(GL_TRIANGLES, (sizeof(etc) / sizeof(float)) / 8, (vertices.size() - sizeof(etc) / sizeof(float)) / 8);



        colorShader.use();
        colorShader.setMat4("projection", projection);
        colorShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, camera.Position);
        model = glm::rotate(model, -glm::half_pi<float>() - glm::radians(camera.Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(camera.Pitch), glm::vec3(1.0f, 0.0f, 0.0f));
        colorShader.setMat4("model", model);
        colorShader.setVec3("color", glm::vec3(1.0f));

        glDrawArrays(GL_LINE_STRIP, 6, 2);
        glDrawArrays(GL_LINE_STRIP, 6 + 2, 2);

      //  colorShader.setVec3("color", glm::vec3(1.0f, 1.0f, 0.0f));

        modelShader.use();
       

        // light properties
        modelShader.setVec3("light.position", lightPos);

      
        
        modelShader.setVec3("viewPos", camera.Position);

        modelShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        modelShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        modelShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        // material properties
        modelShader.setFloat("material.shininess", 32.0f);

        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", view);

     
                

        for (BuildingBlock& block : cubePositions) {
            glBindVertexArray(cubeVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, block.texture.diffuse);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, block.texture.specular);
            model = glm::mat4(1.0f);
            model = glm::translate(model, block.position);
            modelShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        model = glm::mat4(1.0f);
        model = glm::translate(model, camera.Position);
        model = glm::translate(model, camera.Front * 2.0f);
        model = glm::translate(model, camera.Right * 0.5f);
        model = glm::translate(model, -camera.Up * 0.75f);
        model = glm::rotate(model, angle_y, camera.Right);
        model = glm::rotate(model, angle_x, camera.Up);

        model = glm::rotate(model, indicatorBlock.angle[0], camera.Right);
        model = glm::rotate(model, indicatorBlock.angle[1], camera.Up);
        model = glm::rotate(model, indicatorBlock.angle[2], camera.Front);

        model = glm::rotate(model, -glm::half_pi<float>() - glm::radians(camera.Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(camera.Pitch), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f));
        modelShader.setMat4("model", model);
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures.at(currentTextureIndex).diffuse);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures.at(currentTextureIndex).specular);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        if (movement[0])
            camera.ProcessKeyboard(FORWARD,deltaTime);
        if (movement[1])
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (movement[2])
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (movement[3])
            camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        movement[0] = true;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
        movement[0] = false;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        movement[1] = true;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
        movement[1] = false;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        movement[2] = true;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE)
        movement[2] = false;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        movement[3] = true;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
        movement[3] = false;

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        if (indicatorBlock.angle[0] != glm::half_pi<float>() *3)
        {
            indicatorBlock.angle[0] += glm::half_pi<float>();
        }
        else {
            indicatorBlock.angle[0] = 0;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        if (indicatorBlock.angle[1] != glm::half_pi<float>() * 3)
        {
            indicatorBlock.angle[1] += glm::half_pi<float>();
        }
        else {
            indicatorBlock.angle[1] = 0;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        if (indicatorBlock.angle[2] != glm::half_pi<float>() * 3)
        {
            indicatorBlock.angle[2] += glm::half_pi<float>();
        }
        else {
            indicatorBlock.angle[2] = 0;
        }
    }
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        //getting cursor position
        glfwGetCursorPos(window, &xpos, &ypos);

        glm::vec3 newBox = calculateBoxPosition();


        
            BuildingBlock buildingBlock;
            buildingBlock.texture = textures.at(currentTextureIndex);
            buildingBlock.position = newBox;
            cubePositions.push_back(buildingBlock);
     
       
        
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

unsigned int loadTexture(char const* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
        std::cout << "Texture failed to load at path: " << path << "\n";

    stbi_image_free(data);

    return textureID;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

static glm::vec3 calculateBoxPosition() {
    float x = round(camera.Position.x+2.0f*camera.Front.x*cos(angle_x));
    float y = round(camera.Position.y+camera.Front.y);
    float z = round(camera.Position.z+2.0f*camera.Front.z+sin(angle_x));


    bool yChanged = false;
    std::cout << "other cubes: \n";
    for (BuildingBlock& b : cubePositions) {
         if (glm::distance(b.position, glm::vec3(x,y,z)) == 1.0f && b.position.y == y){
            yChanged = true;
            break;
        }
         else if (b.position.x == x && b.position.z == z) {
             y = b.position.y + 1.0f;
             yChanged = true;
         }
    }
    if (!yChanged) {
        y = -3.0f;
    }

    return glm::vec3(x, y, z);

}

void scroll_callback(GLFWwindow* window,double xoffset, double yoffset) {
    if (currentTextureIndex + 1 == textures.size()) {
        currentTextureIndex = 0;
    }
    else {
        currentTextureIndex++;
    }
}
