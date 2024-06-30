// System Headers
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <btBulletDynamicsCommon.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/matrix_transform.hpp>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>

#include "Game.h"

//  void OffsetVertices(float offset) {
//         GLuint offsetIndex = glGetUniformLocation(shaderProgram, "m_yVertexOffset");
//         float offsetRef;
//         glGetUniformfv(shaderProgram, offsetIndex, &offsetRef);
//         glUniform1f(offsetIndex, offsetRef + offset);
//     }


int main(int argc, char * argv[]) {

    Game game;
    
    game.Initialize();

    while (game.IsRunning()) {
        game.Loop();
    }

    game.Shutdown();
    
    return EXIT_SUCCESS;
}
