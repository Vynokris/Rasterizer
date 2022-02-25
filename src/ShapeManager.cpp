#include <ShapeManager.hpp>
#include <filesystem>
#include <iostream>
#include <tinydir.h>


ShapeManager::ShapeManager()
{
    // Load an empty default texture.
    TextureData texture;
    textureNames.push_back("None");
    textures.push_back(texture);
    
    // Open the art directory.
    tinydir_dir dir;
    tinydir_open(&dir, "art");

    // Loop over all the files in the dirextory.
    while (dir.has_next)
    {
        tinydir_file file;
        tinydir_readfile(&dir, &file);

        // Get the file name.
        std::string fileName = file.name;
        if (fileName == ".." || fileName == ".") 
        {
            tinydir_next(&dir);
            continue;
        }

        // Load the texture.
        texture = loadBmpData(("art/" + fileName).c_str());
        textures.push_back(texture);

        // Remove the file extension and save the name.
        fileName = fileName.substr(0, fileName.length()-4);
        textureNames.push_back(fileName);

        tinydir_next(&dir);
    }
    tinydir_close(&dir);
}

ShapeManager::~ShapeManager()
{
    // Free the loaded textures.
    for (int i = 1; i < (int)textures.size(); i++)
        delete[] textures[i].pixels;
}

void ShapeManager::addShape(Shape _shape)
{
    if (shapes.size() < MAX_SHAPES)
    {
        // If the triangle data isn't set, give it default values.
        if (_shape.triangleData.a.pos == Vector3() &&
            _shape.triangleData.b.pos == Vector3() &&
            _shape.triangleData.c.pos == Vector3())
        {
            _shape.triangleData = Triangle3(Vertex{ {  0,  -0.5, 0 }, { 0, 0, 1 }, WHITE }, 
                                            Vertex{ { -0.5, 0.5, 0 }, { 0, 0, 1 }, WHITE }, 
                                            Vertex{ {  0.5, 0.5, 0 }, { 0, 0, 1 }, WHITE });
        }

        shapes.push_back(_shape);
    }
}

Shape& ShapeManager::getShape(const int& _index)
{
    return shapes[_index];
}

int ShapeManager::getShapeNum()
{
    return (int)shapes.size();
}

void ShapeManager::delShape(const int& _index)
{
    if (0 <= _index && _index < getShapeNum())
        shapes.erase(shapes.begin() + _index);
}

void ShapeManager::drawShapes(Renderer& _renderer)
{
    for (Shape shape : shapes)
    {
        // If the shpe's alpha is 0, don't draw it.
        if (shape.color.a <= 0.01)
            continue;

        // Tell the renderer to use the shape's material and texture.
        _renderer.setMaterial(shape.material);
        _renderer.setTexture(textures[shape.textureID]);
        _renderer.applyVertexColorToTextures(shape.applyVertexColor);

        // Disable backface culling for triangles and quads.
        if (shape.type == ShapeTypes::TRIANGLE || shape.type == ShapeTypes::QUAD)
            _renderer.doBackfaceCulling(false);
        else
            _renderer.doBackfaceCulling(true);

        // Translate the shape to its world position.
        _renderer.modelPushMat();
        _renderer.modelTranslate(shape.pos.x, shape.pos.y, shape.pos.z);
        _renderer.modelRotateX(shape.rot.x);
        _renderer.modelRotateY(shape.rot.y);
        _renderer.modelRotateZ(shape.rot.z);

        // Draw the shape.
        switch (shape.type)
        {
        case ShapeTypes::TRIANGLE:
            _renderer.drawTriangle(shape.triangleData);
            break;
        case ShapeTypes::QUAD:
            _renderer.drawDividedQuad(shape.color, shape.size);
            break;
        case ShapeTypes::CUBE:
            _renderer.drawCube(shape.color, shape.size);
            break;
        case ShapeTypes::DIVIDED_CUBE:
            _renderer.drawDividedCube(shape.color, shape.size, shape.subdivisions);
            break;
        case ShapeTypes::SPHERE:
            _renderer.drawSphere(shape.color, shape.size, shape.subdivisions, shape.subdivisions);
            break;
        default: 
            break;
        }

        _renderer.modelPopMat();
    }
}

void ShapeManager::showImGuiControls()
{
    // Create a vector of c-string texture names to be used by ImGui.
    static std::vector<const char*> textureNamesChar;
    if (textureNamesChar.size() <= 0)
        for (int i = 0; i < (int)textureNames.size(); i++)
            textureNamesChar.push_back(textureNames[i].c_str());
    
    // List of shape type names.
    static const char* shapeTypeNames[] = { "Triangle", "Quad", "Cube", "Subdivided cube", "Sphere" };

    // Create the object manager section of the engine.
    if (ImGui::CollapsingHeader("Objects"))
    {
        // Compute "add shape" button padding.
        ImVec2 p0 = ImGui::GetCursorScreenPos();
        ImGui::SetCursorScreenPos({ p0.x+7, p0.y });
        ImGui::BeginGroup();

        // Button to add shapes.
        if (ImGui::Button("Create a new object", ImGui::GetItemRectSize()))
            addShape();
        ImGui::EndGroup();
            
        // Compute shape info padding.
        ImGui::SetCursorScreenPos({ p0.x+10, p0.y+23 });
        ImGui::BeginGroup();

        // Show info on the scene's shapes.
        for (int i = 0; i < getShapeNum(); i++)
        {
            ShapeTypes  shapeType = shapes[i].type;
            std::string shapeName = "Shape " + to_string(i+1);

            if (ImGui::CollapsingHeader(shapeName.c_str()))
            {
                // Compute item padding.
                ImVec2 p1 = ImGui::GetCursorScreenPos();
                ImGui::SetCursorScreenPos({ p1.x+7, p1.y });
                ImGui::BeginGroup();
                ImGui::PushID(i);

                // Shape type.
                int curShapeType = (int)shapes[i].type;
                ImGui::Combo("Type", &curShapeType, shapeTypeNames, IM_ARRAYSIZE(shapeTypeNames));
                shapes[i].type = (ShapeTypes)curShapeType;

                // Remove button.
                if (ImGui::Button("Remove")) delShape(i);

                ImGui::NewLine();
                ImGui::Text("World transform:");

                // Position, rotation, size.
                ImGui::DragFloat3("Position", &shapes[i].pos.x, 0.25);
                ImGui::DragFloat3("Rotation", &shapes[i].rot.x, PI/12, 0,  2*PI);
                if (shapeType != ShapeTypes::TRIANGLE)
                    ImGui::DragFloat ("Size", &shapes[i].size, 0.25, 0);

                // Subdivisions.
                if (shapeType == ShapeTypes::DIVIDED_CUBE || shapeType == ShapeTypes::SPHERE)
                    ImGui::DragInt("Subdivisions", &shapes[i].subdivisions, 1, 3, 100);
                
                // Triangle vertices.
                if (shapeType == ShapeTypes::TRIANGLE)
                {
                    ImGui::NewLine();
                    ImGui::Text("Vertices:");

                    Vertex* curVertex = &shapes[i].triangleData.a;
                    for (int i = 0; i < 3; i++, curVertex++)
                    {
                        std::string posStr = std::to_string(i+1) + (i==0 ? "st" : (i==1 ? "nd" : "rd")) + " position";
                        std::string colStr = std::to_string(i+1) + (i==0 ? "st" : (i==1 ? "nd" : "rd")) + " color";

                        if (ImGui::DragFloat3(posStr.c_str(), &curVertex->pos.x, 0.5))
                        {
                            Vector3 newNormal = Vector3(shapes[i].triangleData.a.pos, shapes[i].triangleData.b.pos)
                                              ^ Vector3(shapes[i].triangleData.a.pos, shapes[i].triangleData.c.pos);
                            shapes[i].triangleData.a.normal = newNormal;
                            shapes[i].triangleData.b.normal = newNormal;
                            shapes[i].triangleData.c.normal = newNormal;
                        }
                        ImGui::ColorEdit4(colStr.c_str(), &curVertex->color.r);
                    }
                }

                ImGui::NewLine();
                ImGui::Text("Appearance:");

                // Color and texture.
                if (shapeType != ShapeTypes::TRIANGLE)
                {
                    ImGui::ColorEdit4("Color",   &shapes[i].color.r);
                    ImGui::Combo     ("Texture", &shapes[i].textureID, textureNamesChar.data(), textureNamesChar.size());
                }

                // Material.
                ImGui::DragFloat("Diffuse",   &shapes[i].material.diffuse,   0.25, 0, 10);
                ImGui::DragFloat("Specular",  &shapes[i].material.specular,  0.25, 0, 10);
                ImGui::DragFloat("Shininess", &shapes[i].material.shininess, 0.25, 0, 10);

                // Vertex hue on texture.
                if (shapeType != ShapeTypes::TRIANGLE && shapes[i].textureID != 0)
                    ImGui::Checkbox("Apply color to texture", &shapes[i].applyVertexColor);

                ImGui::PopID();
                ImGui::EndGroup();
            }

        }
        ImGui::EndGroup();
    }
}
