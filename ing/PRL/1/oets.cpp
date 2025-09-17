/**
 * @ Author: Ondřej Koumar
 * @ Project: PRL Project 1
 * @ Create Date: 2025-04-02
 * @ Description: An implementation of parallel Odd-even transposition sort using MPI.
 */

#include <cstdint>
#include <fstream>
#include <iostream>
#include <mpi.h>
#include <vector>

const int ELEMENTS_TO_SEND = 1;
const int ELEMENTS_TO_RECIEVE = 1;
const int ROOT_PROCESS = 0;
const int DUMMY_TAG = 0;

/**
 * @brief Reads binary data from the file "numbers" into a vector and prints the initial sequence to
 * stdout.
 *
 * @param numbers A vector to be filled with the numbers read from the file.
 * @note Assumes the file "numbers" exists in the current working directory.
 * Uses MPI_Abort on file I/O errors.
 */
void ReadFile(std::vector<uint8_t>& numbers)
{
    std::ifstream inputFile("numbers", std::ios::in | std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    // Try to find the last byte of file, which's position is actually the number of elements to
    // sort - 1.
    inputFile.seekg(0, std::ios::end);
    std::streampos fileSizePos = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);
    if (fileSizePos == -1) {
        std::cerr << "Error getting file size." << std::endl;
        inputFile.close();
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    size_t numberOfElements = static_cast<size_t>(fileSizePos);

    numbers.resize(numberOfElements);

    // Because read() expects char* and our data is of type uint8_t* (unsigned char*), we
    // reinterpret cast it. In this case it should be safe, because read() only copies the data byte
    // by byte and does not care what type it is. We also could make a new vector of chars, and then
    // move the data to our first vector of uint8_ts.
    inputFile.read(reinterpret_cast<char*>(numbers.data()), numberOfElements);
    if (!inputFile || inputFile.gcount() != numberOfElements) {
        std::cerr << "Error reading file content. Expected " << numberOfElements << ", read "
                  << inputFile.gcount() << std::endl;
        inputFile.close();
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    inputFile.close();

    for (size_t i = 0; i < numbers.size(); i++) {
        // uint8_t is just typedef for unsigned char. If we want to write out a number and not a
        // character, we need to cast it to an int of any type.
        std::cout << static_cast<unsigned int>(numbers[i]);
        if (i != numbers.size() - 1) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
}

/**
 * @brief Decides if process should keep its element or change it with its partners'.
 *
 * Process with smaller rank always keeps the lower element.
 *
 * @param myRank Calling process' rank.
 * @param partnerRank Calling process' neighbors' rank.
 * @param myElement Calling process' element held from previous iteration.
 * @param partnerElement Neighbors' element held from previous iteration.
 * @return uint8_t An element that calling process should keep to the next algorithm iteration.
 */
uint8_t GetCorrectElement(int myRank, int partnerRank, uint8_t myElement, uint8_t partnerElement)
{
    if ((myRank < partnerRank && partnerElement < myElement) ||
        (myRank > partnerRank && partnerElement > myElement)) {
        return partnerElement;
    }

    return myElement;
}

/**
 * @brief Calculate neighbors' rank for given process rank and current phase.
 *
 * On even iterations, processes with even rank communicate with neighbors whose rank is 1 higher.
 * On odd iterations, processes with odd rank communicate with neighbors whose rank is 1 higher.
 * If the parity doesn't match, than their current neighbor has rank 1 lower.
 *
 * @param sortingPhase Phase/iteration of the algorithm.
 * @param myRank Calling process' rank.
 * @return int Rank of selected neighbor.
 */
int GetPartnerRank(int sortingPhase, int myRank)
{
    if (sortingPhase % 2 == myRank % 2) {
        return myRank + 1;
    }

    return myRank - 1;
}

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int myRank, numOfProcesses;

    MPI_Comm_size(MPI_COMM_WORLD, &numOfProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    std::vector<uint8_t> numbers;
    if (myRank == ROOT_PROCESS) {
        ReadFile(numbers);
    }

    uint8_t currentlyHeldElement;

    MPI_Scatter(static_cast<const void*>(numbers.data()),
                ELEMENTS_TO_SEND,
                MPI_UNSIGNED_CHAR,
                &currentlyHeldElement,
                ELEMENTS_TO_RECIEVE,
                MPI_UNSIGNED_CHAR,
                ROOT_PROCESS,
                MPI_COMM_WORLD);

    for (int phase = 0; phase < numOfProcesses; phase++) {
        int partnerRank = GetPartnerRank(phase, myRank);

        if (partnerRank >= 0 && partnerRank < numOfProcesses) {
            uint8_t recievedElement;

            MPI_Sendrecv(static_cast<const void*>(&currentlyHeldElement),
                         ELEMENTS_TO_SEND,
                         MPI_UNSIGNED_CHAR,
                         partnerRank,
                         DUMMY_TAG,
                         static_cast<void*>(&recievedElement),
                         ELEMENTS_TO_RECIEVE,
                         MPI_UNSIGNED_CHAR,
                         partnerRank,
                         DUMMY_TAG,
                         MPI_COMM_WORLD,
                         MPI_STATUS_IGNORE);

            currentlyHeldElement = GetCorrectElement(myRank,
                                                     partnerRank,
                                                     currentlyHeldElement,
                                                     recievedElement);
        }
    }

    MPI_Gather(&currentlyHeldElement,
               ELEMENTS_TO_SEND,
               MPI_UNSIGNED_CHAR,
               static_cast<void*>(numbers.data()),
               ELEMENTS_TO_RECIEVE,
               MPI_UNSIGNED_CHAR,
               ROOT_PROCESS,
               MPI_COMM_WORLD);

    if (myRank == ROOT_PROCESS) {
        for (uint8_t num : numbers) {
            std::cout << static_cast<unsigned int>(num) << "\n";
        }
    }

    MPI_Finalize();
    return 0;
}
