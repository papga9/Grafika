#include "DrawElement.h"



DrawElement::DrawElement() {
    pos_x = pos_y = 0.0f;
    initialize();
}

DrawElement::DrawElement(float _x, float _y):pos_x(_x), pos_y(_y) {
    initialize();

}

DrawElement::DrawElement(float _x, float _y,float _r, float _g, float _b): pos_x(_x),pos_y(_y),r(_r),g(_g),b(_b) {
    initialize();
}



void DrawElement::initialize() {
    float vertices[] = {
        1.0f,  1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f
    };

    for (short i = 0; i < 12; ++i) {
        vertices[i] *= SCALE;
    }


    unsigned int indices[] =
    {
        0, 1, 3,
        1, 2, 3
    };


   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);
   glGenBuffers(1, &EBO);

   glBindVertexArray(VAO);


   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);

   ourShader.init("4.0.shader.vs", "4.0.shader.fs");
}

void DrawElement::drawBodyParts() const {
    for (short i = 0; i < bodyParts.size(); ++i)
    {
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(bodyParts[i].first, bodyParts[i].second, 0.0));
        ourShader.setMat4("trans", trans);

        int vertexColorLocation = glGetUniformLocation(ourShader.ID, "ourColor");
        glUniform4f(vertexColorLocation, r,g,b, 1.0f);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

std::pair<float, float> DrawElement::getPosition() const {
    return bodyParts.front();
}

float DrawElement::getScale() const {
    return SCALE;
}