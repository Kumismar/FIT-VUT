/*!
 * @file
 * @brief This file contains implementation of gpu
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <student/gpu.hpp>
#include <iostream>
#include <iomanip>
typedef struct triangle {
    OutVertex vertices[3];
} Triangle;

typedef struct barycentricCoordinates {
    float lambda0;
    float lambda1;
    float lambda2;
} BarycentricCoordinates;

void clear(GPUMemory& mem, ClearCommand& clearcmd)
{
    if (clearcmd.clearColor)
    {
        for (uint32_t i = 0; i < mem.framebuffer.width*mem.framebuffer.height*mem.framebuffer.channels; i+=4)
        {
            mem.framebuffer.color[i]   = (uint8_t)(clearcmd.color.r*255.0);
            mem.framebuffer.color[i+1] = (uint8_t)(clearcmd.color.g*255.0);
            mem.framebuffer.color[i+2] = (uint8_t)(clearcmd.color.b*255.0);
            mem.framebuffer.color[i+3] = (uint8_t)(clearcmd.color.a*255.0);
        }
    }
    if (clearcmd.clearDepth)
    {
        for (uint32_t i = 0; i < mem.framebuffer.width*mem.framebuffer.height; i++)
        {
            mem.framebuffer.depth[i] = clearcmd.depth;
        }
    }
}

void getVertexId(InVertex& inVertex, GPUMemory& mem, DrawCommand& drawcmd, uint32_t vertexNum)
{
    // Neindexovane kresleni
    if (drawcmd.vao.indexBufferID == -1) 
    {
        inVertex.gl_VertexID = vertexNum;
    }
    // Indexovane kresleni
    else
    {
        // Buffer s indexama vertices
        const void* indexBuffer = mem.buffers[drawcmd.vao.indexBufferID].data;

        // Posunuti pointeru v bufferu na vertices
        uint8_t* index = (uint8_t*)indexBuffer + drawcmd.vao.indexOffset;

        // Pole je v bytech -> pokud je vetsi typ, nasobime
        if (drawcmd.vao.indexType == IndexType::UINT8)
        {
            inVertex.gl_VertexID = index[vertexNum];
        }
        else if (drawcmd.vao.indexType == IndexType::UINT16)
        {
            inVertex.gl_VertexID = index[vertexNum*sizeof(uint16_t)];
        }
        else
        {
            inVertex.gl_VertexID = index[vertexNum*sizeof(uint32_t)];
        }
    }
}

void getTexturesAndUniforms(ShaderInterface& shaderInterface, GPUMemory& mem)
{
    shaderInterface.textures = mem.textures;
    shaderInterface.uniforms = mem.uniforms;    
}

void readAttributes(InVertex& inVertex, GPUMemory& mem, VertexArray& va)
{
    for (uint32_t i = 0; i < maxAttributes; i++)
    {
        // Buffer neni aktivovany -> next attribute
        if (va.vertexAttrib[i].bufferID == -1)
        {
            continue;
        }

        // Cteni z adresy buf_ptr + offset + stride*gl_VertexID
        uint32_t offset = va.vertexAttrib[i].offset + va.vertexAttrib[i].stride*inVertex.gl_VertexID;

        // Offset je v bytech -> pretypovani na 8bit datovy typ, posunuti o offset
        uint8_t* index = (uint8_t*)mem.buffers[va.vertexAttrib[i].bufferID].data + offset;

        // Podle typu atributu pretypujeme buffer; cteme 0. prvek, protoze uz jsme na spravnem offsetu
        if (va.vertexAttrib[i].type == AttributeType::EMPTY)
        {
            continue;
        }
        else if (va.vertexAttrib[i].type == AttributeType::FLOAT)
        {
            float* vertexAttributeBuffer = (float*)index;
            inVertex.attributes[i].v1 = vertexAttributeBuffer[0];
        }
        else if (va.vertexAttrib[i].type == AttributeType::UINT)
        {
            uint32_t* vertexAttributeBuffer = (uint32_t*)index;
            inVertex.attributes[i].u1 = vertexAttributeBuffer[0];
        }
        else if (va.vertexAttrib[i].type == AttributeType::UVEC2)
        {
            glm::uvec2* vertexAttributeBuffer = (glm::uvec2*)index;
            inVertex.attributes[i].u2 = vertexAttributeBuffer[0];
        }
        else if (va.vertexAttrib[i].type == AttributeType::UVEC3)
        {
            glm::uvec3* vertexAttributeBuffer = (glm::uvec3*)index;
            inVertex.attributes[i].u3 = vertexAttributeBuffer[0];
        }
        else if (va.vertexAttrib[i].type == AttributeType::UVEC4)
        {
            glm::uvec4* vertexAttributeBuffer = (glm::uvec4*)index;
            inVertex.attributes[i].u4 = vertexAttributeBuffer[0];
        }
        else if (va.vertexAttrib[i].type == AttributeType::VEC2)
        {
            glm::vec2* vertexAttributeBuffer = (glm::vec2*)index;
            inVertex.attributes[i].v2 = vertexAttributeBuffer[0];
        }
        else if (va.vertexAttrib[i].type == AttributeType::VEC3)
        {
            glm::vec3* vertexAttributeBuffer = (glm::vec3*)index;
            inVertex.attributes[i].v3 = vertexAttributeBuffer[0];
        }
        else if (va.vertexAttrib[i].type == AttributeType::VEC4)
        {
            glm::vec4* vertexAttributeBuffer = (glm::vec4*)index;
            inVertex.attributes[i].v4 = vertexAttributeBuffer[0];
        }
    }
}

void loadTriangle(Triangle& triangle, GPUMemory& mem, DrawCommand& drawcmd, Program& prg, uint32_t triangleNum, uint32_t drawNum)
{
    for (uint32_t vertNum = 0; vertNum < 3; vertNum++)
    {
        InVertex inVertex;
        ShaderInterface shaderInterface;

        inVertex.gl_DrawID = drawNum;
        getVertexId(inVertex, mem, drawcmd, 3*triangleNum + vertNum);
        getTexturesAndUniforms(shaderInterface, mem);
        readAttributes(inVertex, mem, drawcmd.vao);

        prg.vertexShader(triangle.vertices[vertNum], inVertex, shaderInterface);
    }
}

void runPerspectiveDivision(Triangle& triangle)
{
    for (uint8_t i = 0; i < 3; i++)
    {
        float w = triangle.vertices[i].gl_Position.w;
        triangle.vertices[i].gl_Position.x = triangle.vertices[i].gl_Position.x/w;
        triangle.vertices[i].gl_Position.y = triangle.vertices[i].gl_Position.y/w;
        triangle.vertices[i].gl_Position.z = triangle.vertices[i].gl_Position.z/w;
    }
    
}

void runViewportTransformation(Triangle& triangle, Frame& frame)
{
    for (uint8_t i = 0; i < 3; i++)
    {
        triangle.vertices[i].gl_Position.x = (triangle.vertices[i].gl_Position.x + 1)/2*frame.width;
        triangle.vertices[i].gl_Position.y = (triangle.vertices[i].gl_Position.y + 1)/2*frame.height; 
    }
    
}

bool isClockWise(Triangle& triangle)
{
    OutVertex A = triangle.vertices[0];
    OutVertex B = triangle.vertices[1];
    OutVertex C = triangle.vertices[2]; 

    int32_t u1 = B.gl_Position.x - A.gl_Position.x;
    int32_t u2 = B.gl_Position.y - A.gl_Position.y;
    int32_t v1 = C.gl_Position.x - B.gl_Position.x;
    int32_t v2 = C.gl_Position.y - B.gl_Position.y;

    int32_t crossProductZ = u1*v2 - u2*v1;

    return (crossProductZ < 0);
}

bool hasIdenticalVertices(Triangle& triangle)
{
    return ((triangle.vertices[0].gl_Position.x == triangle.vertices[1].gl_Position.x  &&
             triangle.vertices[0].gl_Position.y == triangle.vertices[1].gl_Position.y) ||

            (triangle.vertices[1].gl_Position.x == triangle.vertices[2].gl_Position.x  &&
             triangle.vertices[1].gl_Position.y == triangle.vertices[2].gl_Position.y) ||

            (triangle.vertices[2].gl_Position.x == triangle.vertices[0].gl_Position.x  &&
             triangle.vertices[2].gl_Position.y == triangle.vertices[0].gl_Position.y));
}

float findXMin(Triangle& triangle)
{
    float tmp = std::min<float>(triangle.vertices[0].gl_Position.x, triangle.vertices[1].gl_Position.x);
    return std::min<float>(tmp, triangle.vertices[2].gl_Position.x);
}

float findXMax(Triangle& triangle)
{
    float tmp = std::max<float>(triangle.vertices[0].gl_Position.x, triangle.vertices[1].gl_Position.x);
    return std::max<float>(tmp, triangle.vertices[2].gl_Position.x);
}

float findYMin(Triangle& triangle)
{
    float tmp = std::min<float>(triangle.vertices[0].gl_Position.y, triangle.vertices[1].gl_Position.y);
    return std::min<float>(tmp, triangle.vertices[2].gl_Position.y);
}

float findYMax(Triangle& triangle)
{
    float tmp = std::max<float>(triangle.vertices[0].gl_Position.y, triangle.vertices[1].gl_Position.y);
    return std::max<float>(tmp, triangle.vertices[2].gl_Position.y);
}

float findEdgeFunction(OutVertex& point1, OutVertex& point2, float x, float y)
{
    float u1 = (float)(point2.gl_Position.x - point1.gl_Position.x);
    float u2 = (float)(point2.gl_Position.y - point1.gl_Position.y);
    float v1 = (float)(x - point1.gl_Position.x);
    float v2 = (float)(y - point1.gl_Position.y);

    return (u1*v2 - u2*v1);
}

bool pointIsInTriangle(Triangle& triangle, float exy1, float exy2, float exy3, bool clockWise)
{
    return ((!clockWise && exy1 >= 0.0f && exy2 >= 0.0f && exy3 >= 0.0f) ||
             (clockWise && exy1 <= 0.0f && exy2 <= 0.0f && exy3 <= 0.0f));
}

glm::vec3 vectorFromPoints(OutVertex& a, OutVertex& b)
{
    return glm::vec3 {
        b.gl_Position.x - a.gl_Position.x,
        b.gl_Position.y - a.gl_Position.y,
        1.0f
    };
}

float triangleArea(OutVertex& a, OutVertex& b, OutVertex& c)
{
    glm::vec3 BA = vectorFromPoints(b, a);
    glm::vec3 AC = vectorFromPoints(a, c);

    float crossProductZ = BA.x*AC.y - BA.y*AC.x;

    return glm::abs(crossProductZ/2);
}

void calculateBarycentricCoordinates(BarycentricCoordinates& bc, OutVertex& A, OutVertex& B, OutVertex& C, OutVertex& P)
{
    float ABCarea = triangleArea(A, B, C);

    bc.lambda0 = triangleArea(P, B, C)/ABCarea;
    bc.lambda1 = triangleArea(P, A, C)/ABCarea;
    bc.lambda2 = triangleArea(P, A, B)/ABCarea;
}

void interpolateAttributes(InFragment& inFragment, Triangle& triangle, BarycentricCoordinates& bc, Program& prg)
{
    OutVertex A = triangle.vertices[0];
    OutVertex B = triangle.vertices[1];
    OutVertex C = triangle.vertices[2];
    
    float s = bc.lambda0/A.gl_Position.w + bc.lambda1/B.gl_Position.w + bc.lambda2/C.gl_Position.w;

    barycentricCoordinates interpolated;

    interpolated.lambda0 = bc.lambda0/(A.gl_Position.w*s);
    interpolated.lambda1 = bc.lambda1/(B.gl_Position.w*s);
    interpolated.lambda2 = bc.lambda2/(C.gl_Position.w*s);

    for (uint8_t currentAttribute = 0; currentAttribute < maxAttributes; currentAttribute++)
    {
        switch (prg.vs2fs[currentAttribute])
        {
            case AttributeType::EMPTY:
                break;
            case AttributeType::UINT:
            {
                inFragment.attributes[currentAttribute].u1 = A.attributes[currentAttribute].u1;
                break;
            }
            case AttributeType::UVEC2:
            {
                inFragment.attributes[currentAttribute].u2 = A.attributes[currentAttribute].u2;
                break;
            }
            case AttributeType::UVEC3:
            {
                inFragment.attributes[currentAttribute].u3 = A.attributes[currentAttribute].u3;
                break;
            }
            case AttributeType::UVEC4:
            {
                inFragment.attributes[currentAttribute].u4 = A.attributes[currentAttribute].u4;
                break;
            }
            case AttributeType::FLOAT:
            {
                inFragment.attributes[currentAttribute].v1 = A.attributes[currentAttribute].v1*interpolated.lambda0 +
                                                             B.attributes[currentAttribute].v1*interpolated.lambda1 +
                                                             C.attributes[currentAttribute].v1*interpolated.lambda2;
            }
            case AttributeType::VEC2:
            {
                inFragment.attributes[currentAttribute].v2 = A.attributes[currentAttribute].v2*interpolated.lambda0 +
                                                             B.attributes[currentAttribute].v2*interpolated.lambda1 +
                                                             C.attributes[currentAttribute].v2*interpolated.lambda2;
            }
            case AttributeType::VEC3:
            {
                inFragment.attributes[currentAttribute].v3 = A.attributes[currentAttribute].v3*interpolated.lambda0 +
                                                             B.attributes[currentAttribute].v3*interpolated.lambda1 +
                                                             C.attributes[currentAttribute].v3*interpolated.lambda2;
            }
            case AttributeType::VEC4:
            {
                inFragment.attributes[currentAttribute].v4 = A.attributes[currentAttribute].v4*interpolated.lambda0 +
                                                             B.attributes[currentAttribute].v4*interpolated.lambda1 +
                                                             C.attributes[currentAttribute].v4*interpolated.lambda2;
            }
            default:
                break;
        }
    }   
}

void createFragment(InFragment& inFragment, float x, float y, Triangle& triangle, Program& prg)
{
    OutVertex A = triangle.vertices[0];
    OutVertex B = triangle.vertices[1];
    OutVertex C = triangle.vertices[2];
    OutVertex P; P.gl_Position = glm::vec4(x, y, 0, 1);

    BarycentricCoordinates bc;

    calculateBarycentricCoordinates(bc, A, B, C, P);

    inFragment.gl_FragCoord.x = x;
    inFragment.gl_FragCoord.y = y;
    inFragment.gl_FragCoord.z = (float)(A.gl_Position.z*bc.lambda0 + B.gl_Position.z*bc.lambda1 + C.gl_Position.z*bc.lambda2);

    interpolateAttributes(inFragment, triangle, bc, prg);
}

void setColor(InFragment& inFragment, OutFragment& outFragment, Frame& framebuffer)
{
    uint32_t x = (uint32_t)(inFragment.gl_FragCoord.x);
    uint32_t y = (uint32_t)(inFragment.gl_FragCoord.y);
    float z = inFragment.gl_FragCoord.z;

    float rfloat = outFragment.gl_FragColor.x;
    float gfloat = outFragment.gl_FragColor.y;
    float bfloat = outFragment.gl_FragColor.z;
    float afloat = outFragment.gl_FragColor.w;

    uint32_t depthIndex = framebuffer.width*y + x;
    uint32_t colorIndex = depthIndex*framebuffer.channels;

    if (z < framebuffer.depth[framebuffer.width*y + x])
    {
        if (afloat != 1.0f)
        {
            rfloat = framebuffer.color[colorIndex + 0]/255.0f*(1 - afloat) + rfloat*afloat;
            gfloat = framebuffer.color[colorIndex + 1]/255.0f*(1 - afloat) + gfloat*afloat;
            bfloat = framebuffer.color[colorIndex + 2]/255.0f*(1 - afloat) + bfloat*afloat;
        }

        framebuffer.color[colorIndex + 0] = (uint8_t)(rfloat*255.0f);
        framebuffer.color[colorIndex + 1] = (uint8_t)(gfloat*255.0f);
        framebuffer.color[colorIndex + 2] = (uint8_t)(std::round(bfloat*255.0f));
        framebuffer.color[colorIndex + 3] = (uint8_t)(afloat*255.0f);
        
        if (afloat > 0.5f)
            framebuffer.depth[depthIndex] = z;
    }
}

void rasterizeTriangle(Triangle& triangle, DrawCommand& drawcmd, Program& prg, Frame& framebuffer)
{
    bool clockWise = isClockWise(triangle);
    if (drawcmd.backfaceCulling && clockWise)
        return;

    if (hasIdenticalVertices(triangle))
        return;

    float xmin = findXMin(triangle);
    float xmax = findXMax(triangle);
    float ymin = findYMin(triangle);
    float ymax = findYMax(triangle); 

    for (uint32_t y = 0; y < framebuffer.height; y++)
    {
        for (uint32_t x = 0; x < framebuffer.width; x++)
        {
            float xfloat = x + 0.5f;
            float yfloat = y + 0.5f;

            float exy1 = findEdgeFunction(triangle.vertices[0], triangle.vertices[1], xfloat, yfloat); 
            float exy2 = findEdgeFunction(triangle.vertices[1], triangle.vertices[2], xfloat, yfloat);
            float exy3 = findEdgeFunction(triangle.vertices[2], triangle.vertices[0], xfloat, yfloat);

            if (pointIsInTriangle(triangle, exy1, exy2, exy3, clockWise))
            {
                InFragment inFragment;
                OutFragment outFragment;
                ShaderInterface shaderInterface;

                createFragment(inFragment, xfloat, yfloat, triangle, prg);
                prg.fragmentShader(outFragment, inFragment, shaderInterface);
                setColor(inFragment, outFragment, framebuffer);
            }
        }
    }
}

void draw(GPUMemory& mem, DrawCommand& drawcmd, uint32_t drawNum)
{
    Program prg = mem.programs[drawcmd.programID];
    
    for (uint32_t i = 0; i < drawcmd.nofVertices/3; i++)
    {
        Triangle triangle;
        loadTriangle(triangle, mem, drawcmd, prg, i, drawNum);

        // Primitive assembly
        runPerspectiveDivision(triangle);
        runViewportTransformation(triangle, mem.framebuffer);
        rasterizeTriangle(triangle, drawcmd, prg, mem.framebuffer);
    }
}

//! [gpu_execute]
void gpu_execute(GPUMemory&mem,CommandBuffer &cb){
  /// \todo Tato funkce reprezentuje funkcionalitu grafické karty.<br>
  /// Měla by umět zpracovat command buffer, čistit framebuffer a kresli.<br>
  /// mem obsahuje paměť grafické karty.
  /// cb obsahuje command buffer pro zpracování.
  /// Bližší informace jsou uvedeny na hlavní stránce dokumentace.

    uint32_t drawNumber = 0;
    for (uint32_t i = 0; i < cb.nofCommands; i++)
    {
        if (cb.commands[i].type == CommandType::CLEAR)
        {
            clear(mem, cb.commands[i].data.clearCommand);
        }
        else if (cb.commands[i].type == CommandType::DRAW)
        {
            draw(mem, cb.commands[i].data.drawCommand, drawNumber);
            drawNumber++;
        }
    }

}
//! [gpu_execute]

/**
 * @brief This function reads color from texture.
 *
 * @param texture texture
 * @param uv uv coordinates
 *
 * @return color 4 floats
 */
glm::vec4 read_texture(Texture const&texture,glm::vec2 uv){
  if(!texture.data)return glm::vec4(0.f);
  auto uv1 = glm::fract(uv);
  auto uv2 = uv1*glm::vec2(texture.width-1,texture.height-1)+0.5f;
  auto pix = glm::uvec2(uv2);
  //auto t   = glm::fract(uv2);
  glm::vec4 color = glm::vec4(0.f,0.f,0.f,1.f);
  for(uint32_t c=0;c<texture.channels;++c)
    color[c] = texture.data[(pix.y*texture.width+pix.x)*texture.channels+c]/255.f;
  return color;
}

