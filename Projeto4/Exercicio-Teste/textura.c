#define STB_IMAGE_IMPLEMENTATION 
#include "../TrabalhoFinal/stb_image/stb_image.h"

#include <GL/glut.h> 
#include <stdio.h>

// Variáveis globais
GLfloat angle, fAspect, rotX, rotY, obsZ;
GLuint idTextura;
int width, height, channels;
unsigned char* img_data;

// Função para carregar uma imagem .BMP
GLuint LoadTexture(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (!file) return 0;
	img_data = stbi_load(filename,&width,&height,&channels,3);
    // Verifica o cabeçalho do arquivo BMP
    fseek(file, 18, SEEK_SET);
    fread(&width, 4, 1, file);
    fread(&height, 4, 1, file);
    fseek(file, 28, SEEK_SET);
    int bpp;
    fread(&bpp, 2, 1, file);
    if (bpp != 24)
    {
        fclose(file);
        return 0;
    }
    fseek(file, 54, SEEK_SET);

    // Aloca memória para a imagem
    int size = width * height * 3;
    unsigned char *data = (unsigned char *)malloc(size);
    fread(data, size, 1, file);
    fclose(file);

    // Gera e configura a textura
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    free(data);
    return texture;
}


// Função responsável pela especificação dos parâmetros de iluminação
void DefineIluminacao(void)
{
    GLfloat luzAmbiente0[4] = {0.2, 0.2, 0.2, 1.0};
    GLfloat luzDifusa0[4] = {0.7, 0.7, 0.7, 1.0}; // "cor"
    GLfloat luzEspecular0[4] = {1.0, 1.0, 1.0, 1.0}; // "brilho"
    GLfloat posicaoLuz0[4] = {0.0, -40.0, 0.0, 1.0};

	GLfloat luzAmbiente1[4] = {1, 1, 1, 1.0};
    GLfloat luzDifusa1[4] = {20.0, 20.0, 20.0, 1.0}; // "cor"
    GLfloat luzEspecular1[4] = {20.0, 20.0, 20.0, 1.0}; // "brilho"
    GLfloat posicaoLuz1[4] = {0.0, 40.0, 0.0, 1.0};

    // Capacidade de brilho do material
    GLfloat especularidade[4] = {1.0, 1.0, 1.0, 1.0};
    GLint especMaterial = 60;

    // Define a refletância do material
    glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
    // Define a concentração do brilho
    glMateriali(GL_FRONT, GL_SHININESS, especMaterial);

    // Ativa o uso da luz ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente0);

    // Define os parâmetros da luz de número 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular0);
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz0);

	// Ativa o uso da luz ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente1);
	// Define os parâmetros da luz de número 1
    glLightfv(GL_LIGHT1, GL_AMBIENT, luzAmbiente1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, luzDifusa1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, luzEspecular1);
    glLightfv(GL_LIGHT1, GL_POSITION, posicaoLuz1);

    // Habilita o modelo de colorização de Gouraud
    glShadeModel(GL_SMOOTH);
}

// Função callback chamada para fazer o desenho
void Desenha(void)
{
    // Limpa a janela e o depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Desenha um cubo no qual a textura é aplicada
    glBindTexture(GL_TEXTURE_2D, idTextura);
	glColor3f(0.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        // Face frontal
		glTexCoord2f(-1.0f, -1.0f); glVertex3f(-1.0f, -1.0f, 1.0f); 
		glTexCoord2f(1.0f, -1.0f); glVertex3f( 1.0f, -1.0f, 1.0f); 
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 1.0f); 
		glTexCoord2f(-1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); 
        // glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
        // glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
        // glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        // glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
        // Face posterior
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
        // Face superior
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
        // Face inferior
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
        // Face lateral direita
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
        // Face lateral esquerda
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glEnd();

    glutSwapBuffers();
}

// Função usada para especificar a posição do observador virtual
void PosicionaObservador(void)
{
    // Especifica sistema de coordenadas do modelo
    glMatrixMode(GL_MODELVIEW);
    // Inicializa sistema de coordenadas do modelo
    glLoadIdentity();
    DefineIluminacao();
    // Especifica posição do observador e do alvo
    glTranslatef(0, 0, -obsZ);
    glRotatef(rotX, 1, 0, 0);
    glRotatef(rotY, 0, 1, 0);
}

// Inicializa parâmetros de rendering
void Inicializa(void)
{
    // Define a cor de fundo da janela de visualização como preta
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Habilita a definição da cor do material a partir da cor corrente
    glEnable(GL_COLOR_MATERIAL);
    // Habilita o uso de iluminação
    glEnable(GL_LIGHTING);
    // Habilita a luz de número 0
    glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
    // Habilita o depth-buffering
    glEnable(GL_DEPTH_TEST);

    // Inicializa a variável que especifica o ângulo da projeção perspectiva
    angle = 50;

    // Inicializa as variáveis usadas para alterar a posição do observador virtual
    rotX = 30;
    rotY = 0;
    obsZ = 10;

    // Comandos de inicialização para textura
    idTextura = LoadTexture("imagem.bmp");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT); 
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_TEXTURE_2D);
}

// Função usada para especificar o volume de visualização
void EspecificaParametrosVisualizacao(void)
{
    // Especifica sistema de coordenadas de projeção
    glMatrixMode(GL_PROJECTION);
    // Inicializa sistema de coordenadas de projeção
    glLoadIdentity();

    // Especifica a projeção perspectiva (angulo, aspecto, zMin, zMax)
    gluPerspective(angle, fAspect, 0.5, 500);

    PosicionaObservador();
}

// Função callback chamada quando o tamanho da janela é alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
    // Para prevenir uma divisão por zero
    if (h == 0) h = 1;

    // Especifica as dimensões da viewport
    glViewport(0, 0, w, h);

    // Calcula a correção de aspecto
    fAspect = (GLfloat)w / (GLfloat)h;

    EspecificaParametrosVisualizacao();
}

// Função callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) // Zoom-in
    {
        if (angle >= 10) angle -= 5;
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) // Zoom-out
    {
        if (angle <= 130) angle += 5;
    }
    EspecificaParametrosVisualizacao();
    glutPostRedisplay();
}

// Callback para gerenciar eventos do teclado para teclas especiais
void TeclasEspeciais(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_LEFT: rotY--; break;
        case GLUT_KEY_RIGHT: rotY++; break;
        case GLUT_KEY_UP: rotX++; break;
        case GLUT_KEY_DOWN: rotX--; break;
        case GLUT_KEY_HOME: obsZ++; break;
        case GLUT_KEY_END: obsZ--; break;
        case GLUT_KEY_F10: // "camera reset"
            rotX = 30;
            rotY = 0;
            obsZ = 10;
            angle = 50;
            EspecificaParametrosVisualizacao();
            break;
		case GLUT_KEY_F1: // Mudar para imagem.bmp
            idTextura = LoadTexture("imagem.bmp");
            break;
        case GLUT_KEY_F2: // Mudar para leao.bmp
            idTextura = LoadTexture("leao.bmp");
            break;
    }
    PosicionaObservador();
    glutPostRedisplay();
}

// Programa Principal
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(400, 350);
    glutCreateWindow("Visualizacao 3D com Mapeamento de Textura");
    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    glutMouseFunc(GerenciaMouse);
    glutSpecialFunc(TeclasEspeciais);
    Inicializa();
    glutMainLoop();
    return 0;
}
