//
// Created by Collin on 1/4/2025.
//

#ifndef MESH_HPP
#define MESH_HPP
#include <fstream>
#include <sstream>
#include <vector>

#include "Core/Log.hpp"
#include "Core/Math.hpp"
#include "glad/glad.h"

class Mesh {
public:
    // Constructor
    Mesh(const std::vector<Vec3>& vertices,
         const std::vector<Vec3>& normals,
         const std::vector<Vec2>& uvs,
         const std::vector<unsigned int>& indices) {
        InitMesh(vertices, normals, uvs, indices);
    }

    // Constructor from OBJ
    explicit Mesh(const std::string& filename) {
        LoadMesh(filename);
    }

    // Default constructor (no data)
    Mesh() = default;

    // Copy/Move operations
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&& other) noexcept { MoveFrom(std::move(other)); }
    Mesh& operator=(Mesh&& other) noexcept {
        if(this != &other) {
            Destroy();
            MoveFrom(std::move(other));
        }
        return *this;
    }

    // Deconstructor
    ~Mesh() {
        Destroy();
    }

    // Update mesh buffers with new data
    void Update(const std::vector<Vec3>& vertices,
                const std::vector<Vec3>& normals,
                const std::vector<Vec2>& uvs,
                const std::vector<unsigned int>& indices) {
        glBindVertexArray(VAO);

        // Update Vertices
        glBindBuffer(GL_ARRAY_BUFFER, VBO_V);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3)*vertices.size(), vertices.data(), GL_STATIC_DRAW);

        // Update Normals
        glBindBuffer(GL_ARRAY_BUFFER, VBO_N);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3)*normals.size(), normals.data(), GL_STATIC_DRAW);

        // Update UV
        glBindBuffer(GL_ARRAY_BUFFER, VBO_UV);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2)*uvs.size(), uvs.data(), GL_STATIC_DRAW);

        // Update EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);

        // Store indice count
        indicesCount = static_cast<GLsizei>(indices.size());

        Log::Output(Log::Severity::MESSAGE,"Mesh has been updated!");
    }

    // Binds the mesh for use
    void Bind() const {
        glBindVertexArray(VAO);
    }

    // Unbinds the mesh
    void Unbind() const {
        glBindVertexArray(0);
    }

    // Draws the mesh
    void Draw() const {
        glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
    }

private:
    // OpenGL objects
    GLuint VAO{}, VBO_V{}, VBO_N{}, VBO_UV{}, EBO{};
    GLsizei indicesCount;

    void InitMesh(const std::vector<Vec3>& vertices,
              const std::vector<Vec3>& normals,
              const std::vector<Vec2>& uvs,
              const std::vector<unsigned int>& indices) {

        indicesCount = static_cast<GLsizei>(indices.size());

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // vertices
        glGenBuffers(1, &VBO_V);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_V);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), nullptr);
        glEnableVertexAttribArray(0);

        // normals
        glGenBuffers(1, &VBO_N);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_N);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), nullptr);
        glEnableVertexAttribArray(1);

        // uvs
        glGenBuffers(1, &VBO_UV);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_UV);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2) * uvs.size(), uvs.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), nullptr);
        glEnableVertexAttribArray(2);

        // indices
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);

        //Log::Output(Log::Severity::HAPPY, "A mesh has been created");
    }

    // Destroy a mesh
    void Destroy() {
        if(VBO_V) glDeleteBuffers(1, &VBO_V);
        if(VBO_N) glDeleteBuffers(1, &VBO_N);
        if(VBO_UV) glDeleteBuffers(1, &VBO_UV);
        if(EBO) glDeleteBuffers(1, &EBO);
        if(VAO) glDeleteVertexArrays(1, &VAO);
    }

    // Move function
    void MoveFrom(Mesh&& other) {
        VAO = other.VAO;    other.VAO = 0;
        VBO_V = other.VBO_V; other.VBO_V = 0;
        VBO_N = other.VBO_N; other.VBO_N = 0;
        VBO_UV = other.VBO_UV; other.VBO_UV = 0;
        EBO = other.EBO;    other.EBO = 0;
        indicesCount = other.indicesCount;
        other.indicesCount = 0;
    }

    // Computes normals if OBJ has none
    void ComputeNormals(std::vector<Vec3>& outPositions,
                        std::vector<Vec3>& outNormals,
                        const std::vector<unsigned int>& outIndices)
    {
        outNormals.resize(outPositions.size(), Vec3(0.0f, 0.0f, 0.0f));

        // For each triangle, accumulate face normal into each of the 3 vertices
        for (size_t i = 0; i < outIndices.size(); i += 3)
        {
            unsigned int i0 = outIndices[i + 0];
            unsigned int i1 = outIndices[i + 1];
            unsigned int i2 = outIndices[i + 2];

            const Vec3& v0 = outPositions[i0];
            const Vec3& v1 = outPositions[i1];
            const Vec3& v2 = outPositions[i2];

            // Normal = cross( v1 - v0, v2 - v0 )
            Vec3 edge1 = v1 - v0;
            Vec3 edge2 = v2 - v0;
            Vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));

            outNormals[i0] += faceNormal;
            outNormals[i1] += faceNormal;
            outNormals[i2] += faceNormal;
        }

        // Normalize
        for (auto & n : outNormals)
        {
            if (glm::length(n) > 1e-6f)
                n = glm::normalize(n);
            else
                n = Vec3(0.0f, 1.0f, 0.0f); // fallback
        }
        Log::Output(Log::Severity::MESSAGE, "Normals were computed because the OBJ had none.");
    }


    // Loads the mesh from an OBJ file
    // Handles different syntaxes of OBJ files, and unifies indices data
    void LoadMesh(const std::string& filename)
    {
        std::vector<Vec3> tempPositions;
        std::vector<Vec2> tempUVs;
        std::vector<Vec3> tempNormals;

        struct FaceVertex {
            int v  = -1;  // position index
            int vt = -1;  // uv index
            int vn = -1;  // normal index
        };

        std::vector<std::vector<FaceVertex>> faces; // each 'f' line -> one vector of FaceVertex

        // read file
        std::ifstream file(filename);
        if(!file.is_open()) {
            Log::Output(Log::Severity::ERROR, "Mesh file could not be read: " + filename);
            return;
        }

        std::string line;
        while(std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;

            if(prefix == "v")
            {
                // position
                Vec3 pos;
                iss >> pos.x >> pos.y >> pos.z;
                tempPositions.push_back(pos);
            }
            else if(prefix == "vt")
            {
                // uv
                Vec2 uv;
                iss >> uv.x >> uv.y;
                tempUVs.push_back(uv);
            }
            else if(prefix == "vn")
            {
                // normal
                Vec3 n;
                iss >> n.x >> n.y >> n.z;
                tempNormals.push_back(n);
            }
            else if(prefix == "f")
            {
                // face, can have multiple vertices
                // e.g. "f 1 2 3" or "f 1/2 2/3 3/4" or "f 1/2/3 2/3/4" or "f 1//3" etc.
                std::vector<FaceVertex> faceVerts;
                std::string token;
                while (iss >> token)
                {
                    FaceVertex fv;

                    int slashCount = 0;
                    for (char c : token) {
                        if (c == '/') slashCount++;
                    }

                    std::stringstream ss(token);
                    ss >> fv.v;    // always read position

                    // if there is at least one slash, parse vt
                    if (slashCount >= 1)
                    {
                        size_t firstSlash = token.find('/');
                        // substring after first slash
                        std::string afterFirst = token.substr(firstSlash + 1);
                        if (!afterFirst.empty() && afterFirst[0] != '/')
                        {
                            std::stringstream ss2(afterFirst);
                            ss2 >> fv.vt; // uv index
                        }
                        // if there's a second slash, parse vn
                        size_t secondSlash = afterFirst.find('/');
                        if (secondSlash != std::string::npos)
                        {
                            std::string afterSecond = afterFirst.substr(secondSlash + 1);
                            if (!afterSecond.empty())
                            {
                                std::stringstream ss3(afterSecond);
                                ss3 >> fv.vn; // normal index
                            }
                        }
                    }

                    // convert all to 0-based if needed
                    if (fv.v  > 0) fv.v  -= 1;
                    if (fv.vt > 0) fv.vt -= 1;
                    if (fv.vn > 0) fv.vn -= 1;

                    faceVerts.push_back(fv);
                }

                faces.push_back(faceVerts);
            }
        }
        file.close();

        // step 2: Unify the data into final arrays
        // strat: keep a map from (v, vt, vn) -> new index
        // then build finalPositions, finalUVs, finalNormals, finalIndices
        struct VertexKey {
            int v, vt, vn;
            bool operator==(const VertexKey &o) const {
                return (v == o.v && vt == o.vt && vn == o.vn);
            }
        };
        struct KeyHash {
            std::size_t operator()(const VertexKey &key) const {
                // very simple hash combine
                auto h1 = std::hash<int>()(key.v);
                auto h2 = std::hash<int>()(key.vt);
                auto h3 = std::hash<int>()(key.vn);
                // combine them
                return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1);
            }
        };

        std::unordered_map<VertexKey, unsigned int, KeyHash> remap;
        std::vector<Vec3> finalPositions;
        std::vector<Vec3> finalNormals;
        std::vector<Vec2> finalUVs;
        std::vector<unsigned int> finalIndices;

        // for each face, triangulate
        // so for each face with N vertices form N-2 triangles
        for (auto& fvList : faces)
        {
            if (fvList.size() < 3) continue; // skip degenerate

            // triangulate by fan
            for (size_t i = 1; i + 1 < fvList.size(); ++i)
            {
                FaceVertex f0 = fvList[0];
                FaceVertex f1 = fvList[i];
                FaceVertex f2 = fvList[i + 1];

                FaceVertex tri[3] = { f0, f1, f2 };

                for (auto & faceV : tri)
                {
                    VertexKey key { faceV.v, faceV.vt, faceV.vn };

                    auto it = remap.find(key);
                    if (it != remap.end()) {
                        // already has a merged vertex
                        finalIndices.push_back(it->second);
                    }
                    else {
                        // create a new merged vertex
                        unsigned int newIndex = static_cast<unsigned int>(finalPositions.size());
                        remap[key] = newIndex;
                        finalIndices.push_back(newIndex);

                        // Position
                        if (faceV.v >= 0 && faceV.v < (int)tempPositions.size()) {
                            finalPositions.push_back(tempPositions[faceV.v]);
                        } else {
                            finalPositions.push_back(Vec3(0.0f, 0.0f, 0.0f));
                        }

                        // UV
                        if (faceV.vt >= 0 && faceV.vt < (int)tempUVs.size()) {
                            finalUVs.push_back(tempUVs[faceV.vt]);
                        } else {
                            finalUVs.push_back(Vec2(0.0f, 0.0f));
                        }

                        // Normal
                        if (faceV.vn >= 0 && faceV.vn < (int)tempNormals.size()) {
                            finalNormals.push_back(tempNormals[faceV.vn]);
                        } else {
                            // store dummy normal for now
                            // might stay in scenario where there were *some* normals
                            finalNormals.push_back(Vec3(0.0f, 0.0f, 0.0f));
                        }
                    }
                }
            }
        }

        bool hasNormalData = !tempNormals.empty();
        if (!hasNormalData) {
            // File had no normals, so compute them
            ComputeNormals(finalPositions, finalNormals, finalIndices);
        } else {
            // The OBJ had normal lines, but some might have been missing from faces.
            // I am assuming that if there were vn, they were valid
        }

        // Pass the data to VAO creator
        InitMesh(finalPositions, finalNormals, finalUVs, finalIndices);
    }
};

#endif //MESH_HPP
