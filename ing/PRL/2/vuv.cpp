/**
 * @ Author: Ondřej Koumar
 * @ Project: PRL Project 2
 * @ Create Date: 2025-04-29
 * @ Description: Implementation of parallel tree node level computation using MPI.
 * @ Note: At least C++-17 has to be used for compilation! std::optional is used.
 */

#include <cmath>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <limits>
#include <mpi.h>
#include <optional>
#include <vector>

const unsigned INVALID_ID = std::numeric_limits<unsigned>::max();
const char FORWARD_EDGE = -1;
const char BACKWARD_EDGE = 1;
const int ROOT_PROCESS = 1;

/**
 * @brief Data structure representing oriented edge in the tree.
 */
struct OrientedEdge
{
    unsigned fromVertex;

    unsigned toVertex;

    /// Unique identifier of self.
    unsigned id;

    /// Id of reverse edge. Used for searching in constant time.
    unsigned reverseId;

    /// Id of successor in Euler tour.
    unsigned nextEdgeId;

    /// Direction of the edge considering root.
    char weight;

    int suffixSumValue;
};

/**
 * @brief Data structure representing all vertices' adjacency lists.
 */
struct AdjacencyMatrix
{
    /**
     * @brief Constructor that initializes adjacency list of each vertex to an empty list.
     *
     * @param size Number of vertices (thus, number of adjacency lists.)
     */
    AdjacencyMatrix(unsigned size)
        : matrix(size)
    {
    }

    /**
     * @brief Add `edge` to adjacency list of vertex defined by `vertexIdx`.
     *
     * @param edge Edge to be added.
     * @param vertexIdx Index of adjacency list of vertex.
     */
    void AddEdgeAt(const OrientedEdge& edge, unsigned vertexIdx)
    {
        matrix[vertexIdx].push_back(edge);
    }

    /// Adjacency matrix representation. Not the most effective out there, the inner vectors will be
    /// distributed on the heap. But it's not a big deal compared to all the MPI communication
    /// happening.
    std::vector<std::vector<OrientedEdge>> matrix;
};

/**
 * @brief Create an MPI Datatype for Oriented edge.
 *
 * Auxiliary function for the ability to send OrientedEdge directly over MPI.
 *
 * @return MPI_Datatype MPI_ORIENTED_EDGE.
 */
MPI_Datatype CreateOrientedEdgeType()
{
    const int numOfItems = 7;
    MPI_Datatype types[numOfItems] = { MPI_UNSIGNED, MPI_UNSIGNED, MPI_UNSIGNED, MPI_UNSIGNED,
                                       MPI_UNSIGNED, MPI_CHAR,     MPI_INT };

    MPI_Aint offsets[numOfItems];
    offsets[0] = offsetof(OrientedEdge, fromVertex);
    offsets[1] = offsetof(OrientedEdge, toVertex);
    offsets[2] = offsetof(OrientedEdge, id);
    offsets[3] = offsetof(OrientedEdge, reverseId);
    offsets[4] = offsetof(OrientedEdge, nextEdgeId);
    offsets[5] = offsetof(OrientedEdge, weight);
    offsets[6] = offsetof(OrientedEdge, suffixSumValue);

    int blockLengths[numOfItems] = { 1, 1, 1, 1, 1, 1, 1 };

    MPI_Datatype MPIEdgeType;
    MPI_Type_create_struct(numOfItems, blockLengths, offsets, types, &MPIEdgeType);
    MPI_Type_commit(&MPIEdgeType);

    return MPIEdgeType;
}

/**
 * @brief Try to search for a successor of `revEdge` in adjacency list.
 *
 * @param revEdge Reversed edge of edge that searches for it's successor in Euler tour.
 * @return std::optional<unsigned> Edge id if found; otherwise, std::nullopt.
 */
std::optional<unsigned> Next(const OrientedEdge& revEdge, const AdjacencyMatrix& localAdjMatrix)
{
    // The edges are both incoming and outgoing but it does not matter, because there are at
    // most six of them and just filtering the incoming/outgoing is more difficult than just
    // searching through all the edges.
    const std::vector<OrientedEdge>& edgesOfVertex = localAdjMatrix.matrix[revEdge.fromVertex];
    for (size_t i = 0; i < edgesOfVertex.size() - 1; i++) {
        if (revEdge.id == edgesOfVertex[i].id) {
            return edgesOfVertex[i + 1].id;
        }
    }

    return std::nullopt;
}

/**
 * @brief Find edge successor in Euler tour.
 *
 * If `revEdge` has a successor in adjacency list, return the successors id.
 * Otherwise, search for the first edge in adjacency list of vertex which the edge leads to.
 *
 * @param revEdge Reverse edge of edge that needs to know its successor.
 * @return Id of edge that is next in Euler tour.
 */
unsigned CalculateEtourSuccessor(const OrientedEdge& revEdge, const AdjacencyMatrix& localAdjMatrix)
{
    std::optional<unsigned> searchedEdge = Next(revEdge, localAdjMatrix);
    if (searchedEdge) {
        return searchedEdge.value();
    }

    // The algorithm defined in lectures says return AdjList(e_i.v).
    // Because we work with reverse edge, we return AdjList(e_r.u).
    return localAdjMatrix.matrix[revEdge.fromVertex][0].id;
}

/**
 * @brief Sequentially build adjacency matrix and array of all oriented edges.
 *
 * @param matrix Adjacency matrix to build.
 * @param edges Edge array to build.
 * @param inputChars Array of characters which the tree is built from.
 */
void BuildAdjMatrixAndEdgeArray(AdjacencyMatrix& matrix,
                                std::vector<OrientedEdge>& edges,
                                const std::vector<char>& inputChars)
{
    unsigned edgeId = 0;
    unsigned numOfVertices = inputChars.size();

    // The tree in array is represented in a standard way, where current node
    // is on index i, its left and right son are on indeces 2i+1 and 2i+2 respectively.
    for (unsigned vertexIdx = 0; vertexIdx < numOfVertices; vertexIdx++) {
        unsigned leftSonIdx = 2 * vertexIdx + 1;
        unsigned rightSonIdx = leftSonIdx + 1;

        // Left son exists -> construct edges leading to and from it.
        // Add edges to fathers and sons adjacent lists.
        if (leftSonIdx < numOfVertices) {
            unsigned forwardEdgeId = edgeId++;
            unsigned backwardEdgeId = edgeId++;
            OrientedEdge forwardEdge = { vertexIdx,      leftSonIdx, forwardEdgeId,
                                         backwardEdgeId, INVALID_ID, FORWARD_EDGE };
            OrientedEdge backwardEdge = { leftSonIdx,    vertexIdx,  backwardEdgeId,
                                          forwardEdgeId, INVALID_ID, BACKWARD_EDGE };

            matrix.AddEdgeAt(forwardEdge, vertexIdx);
            matrix.AddEdgeAt(backwardEdge, leftSonIdx);
            edges.push_back(forwardEdge);
            edges.push_back(backwardEdge);
        }

        // Right son exists -> construct edges leading to and from it.
        // Add edges to fathers and sons adjacent lists.
        if (rightSonIdx < numOfVertices) {
            unsigned forwardEdgeId = edgeId++;
            unsigned backwardEdgeId = edgeId++;
            OrientedEdge forwardEdge = { vertexIdx,      rightSonIdx, forwardEdgeId,
                                         backwardEdgeId, INVALID_ID,  FORWARD_EDGE };
            OrientedEdge backwardEdge = { rightSonIdx,   vertexIdx,  backwardEdgeId,
                                          forwardEdgeId, INVALID_ID, BACKWARD_EDGE };

            matrix.AddEdgeAt(forwardEdge, vertexIdx);
            matrix.AddEdgeAt(backwardEdge, rightSonIdx);
            edges.push_back(forwardEdge);
            edges.push_back(backwardEdge);
        }
    }
}

/**
 * @brief Find Euler tour for input tree.
 *
 * @param myRank Rank of the calling process.
 * @param MPI_ORIENTED_EDGE MPI datatype representation of OrientedEdge.
 * @param localAdjMatrix Local copy of adjacency matrix.
 * @param localAllEdgesArray Local copy of edge array.
 */
void BuildEulerTour(const int myRank,
                    const MPI_Datatype& MPI_ORIENTED_EDGE,
                    const AdjacencyMatrix& localAdjMatrix,
                    std::vector<OrientedEdge>& localAllEdgesArray)
{
    OrientedEdge& myEdge = localAllEdgesArray[myRank];
    const OrientedEdge& myEdgeRev = localAllEdgesArray[myEdge.reverseId];

    myEdge.nextEdgeId = CalculateEtourSuccessor(myEdgeRev, localAdjMatrix);

    // After calculating successor for each edge, share the information across all processes.
    MPI_Allgather(MPI_IN_PLACE,
                  0,
                  MPI_DATATYPE_NULL,
                  localAllEdgesArray.data(),
                  1,
                  MPI_ORIENTED_EDGE,
                  MPI_COMM_WORLD);

    // The last edge in Euler tour has no successor -> we set the successor to itself.
    // Deterministically, I choose the last edge as the last edge added to adjacency list of root.
    unsigned lastEdgeId = localAdjMatrix.matrix[0].back().reverseId;
    localAllEdgesArray[lastEdgeId].nextEdgeId = lastEdgeId;
}

/**
 * @brief Calculate suffix sum from `myRank` position to the end of the Euler tour.
 *
 * @param myRank Rank of calling process.
 * @param localAllEdgesArray Local copy of process' array of all edges.
 * @param localAdjMatrix Local copy of process' adjacent matrix.
 */
void CalculateSuffixSum(const int myRank,
                        std::vector<OrientedEdge>& localAllEdgesArray,
                        AdjacencyMatrix& localAdjMatrix)
{
    unsigned numOfEdges = localAllEdgesArray.size();
    std::vector<int> sums(numOfEdges);
    std::vector<unsigned> successors(numOfEdges);

    sums[myRank] = localAllEdgesArray[myRank].weight;
    successors[myRank] = localAllEdgesArray[myRank].nextEdgeId;

    MPI_Allgather(MPI_IN_PLACE, 0, MPI_DATATYPE_NULL, sums.data(), 1, MPI_INT, MPI_COMM_WORLD);
    MPI_Allgather(
        MPI_IN_PLACE, 0, MPI_DATATYPE_NULL, successors.data(), 1, MPI_UNSIGNED, MPI_COMM_WORLD);

    // For simplified further code, I set the value of the last edge to 0. This way, I don't have to
    // care about the last edge being added multiple times during the calculation. Later, when
    // finding the tree node levels, I add 2 instead of 1, because the last edge is always a
    // backward edge.
    unsigned lastEdgeId = localAllEdgesArray[localAdjMatrix.matrix[0].back().reverseId].id;
    sums[lastEdgeId] = 0;

    // Until all the processes reach end of euler tour, add successors suffix sum to mine and move
    // in the list. Using a fixed number of iterations which is computed by every process which
    // makes the code much easier and more readable.
    size_t numOfIterations = static_cast<size_t>(ceil(log2(numOfEdges)));
    for (size_t i = 0; i < numOfIterations; i++) {
        unsigned currentSuccId = successors[myRank];

        sums[myRank] = sums[myRank] + sums[currentSuccId];
        successors[myRank] = successors[currentSuccId];

        MPI_Allgather(MPI_IN_PLACE, 0, MPI_DATATYPE_NULL, sums.data(), 1, MPI_INT, MPI_COMM_WORLD);
        MPI_Allgather(
            MPI_IN_PLACE, 0, MPI_DATATYPE_NULL, successors.data(), 1, MPI_INT, MPI_COMM_WORLD);
    }

    localAllEdgesArray[myRank].suffixSumValue = sums[myRank];
}

/**
 * @brief Calculate calling process' vertex level. After that, reduce all calculated vertex levels
 * to one final array.
 *
 * @param myRank Rank of calling process.
 * @param numOfVertices Number of vertices in the binary tree.
 * @param localAllEdgesArray Local copy of process' array of all edges.
 * @param finalLevels A vector of all vertex levels after the calculation.
 */
void CalculateVertexLevel(const int myRank,
                          const unsigned numOfVertices,
                          const std::vector<OrientedEdge>& localAllEdgesArray,
                          std::vector<int>& finalLevels)
{
    // Create a local array of -1s, set the vertex to desired value (its level in the tree).
    std::vector<int> vertexLevels(numOfVertices, -1);
    const OrientedEdge& myEdge = localAllEdgesArray[myRank];
    if (myEdge.weight == FORWARD_EDGE) {
        vertexLevels[myEdge.toVertex] = myEdge.suffixSumValue + 2;
    }

    // Reduce all local vertexLevels array to one array of vertex levels.
    MPI_Allreduce(
        vertexLevels.data(), finalLevels.data(), numOfVertices, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
}

/**
 * @brief Print levels of tree vertices.
 *
 * @param myRank Rank of the calling process.
 * @param inputChars The input tree (represented as array of chars/string).
 * @param vertexLevels Array of final calculated vertex levels.
 */
void PrintVertexLevels(int& myRank, std::vector<char>& inputChars, std::vector<int>& vertexLevels)
{
    unsigned inputCharsLength = inputChars.size();
    if (myRank == ROOT_PROCESS) {
        vertexLevels[0] = 0;
        for (size_t i = 0; i < inputCharsLength; i++) {
            std::cout << inputChars[i] << ":" << vertexLevels[i];
            if (i < inputCharsLength - 1) {
                std::cout << ",";
            }
        }
        std::cout << std::endl;
    }
}

/**
 * @brief
 *
 * @param tree Input array of chars (tree) in raw form.
 * @param myRank Rank of the calling process.
 * @param inputChars Array of characters to be initialized (as vector, non-raw form).
 * @param inputCharsLength Length of the input array (to be initialized in this function).
 */
void ShareInput(const char* tree,
                const int myRank,
                std::vector<char>& inputChars,
                unsigned int& inputCharsLength)
{
    if (myRank == ROOT_PROCESS) {
        inputCharsLength = std::strlen(tree);
        MPI_Bcast(&inputCharsLength, 1, MPI_UNSIGNED, ROOT_PROCESS, MPI_COMM_WORLD);
        inputChars.resize(inputCharsLength);
        std::copy(tree, tree + inputCharsLength, inputChars.begin());
        MPI_Bcast(inputChars.data(), inputChars.size(), MPI_CHAR, ROOT_PROCESS, MPI_COMM_WORLD);
    }
    else {
        MPI_Bcast(&inputCharsLength, 1, MPI_UNSIGNED, ROOT_PROCESS, MPI_COMM_WORLD);
        inputChars.resize(inputCharsLength);
        MPI_Bcast(inputChars.data(), inputChars.size(), MPI_CHAR, ROOT_PROCESS, MPI_COMM_WORLD);
    }
}

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);
    int myRank;
    int numOfProcessors;
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &numOfProcessors);

    MPI_Datatype MPI_ORIENTED_EDGE = CreateOrientedEdgeType();

    // Could use std::string but I need to access (read/write) the .data().
    std::vector<char> inputChars;
    unsigned inputCharsLength = 0;

    ShareInput(argv[1], myRank, inputChars, inputCharsLength);

    AdjacencyMatrix localAdjMatrix(inputCharsLength);
    std::vector<OrientedEdge> localAllEdgesArray;

    // I chose to let every process build its adjacency matrix and edge array by itself. Another way
    // is to build it once and then distribute it to other processes. Broadcast operations are quite
    // expensive, thus, I chose this approach.
    BuildAdjMatrixAndEdgeArray(localAdjMatrix, localAllEdgesArray, inputChars);

    BuildEulerTour(myRank, MPI_ORIENTED_EDGE, localAdjMatrix, localAllEdgesArray);
    CalculateSuffixSum(myRank, localAllEdgesArray, localAdjMatrix);

    std::vector<int> vertexLevels(inputCharsLength);
    CalculateVertexLevel(myRank, inputCharsLength, localAllEdgesArray, vertexLevels);
    PrintVertexLevels(myRank, inputChars, vertexLevels);

    MPI_Type_free(&MPI_ORIENTED_EDGE);
    MPI_Finalize();
    return 0;
}
