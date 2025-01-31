#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <thread>
#include <vector>
#include <mutex>

using namespace std;
namespace fs = filesystem;

const size_t CHUNK_SIZE = 64 * 1024 * 1024;
mutex writeMutex;

void processChunk(const string& chunk, const string& targetDate, ofstream& outFile) {
    string line;
    istringstream chunkStream(chunk);
    string outputBuffer;
    
    while (getline(chunkStream, line)) {
        if (line.find(targetDate) == 0) {
            outputBuffer += line + "\n";
        }
    }

    lock_guard<mutex> lock(writeMutex);
    outFile << outputBuffer;
}

void filterLogsByDate(const string& logFile, const string& targetDate) {
    string outputDir = "output";
    fs::create_directory(outputDir);
    string outputFile = outputDir + "/output_" + targetDate + ".txt";

    ifstream inFile(logFile, ios::in | ios::binary);
    ofstream outFile(outputFile, ios::out | ios::binary);

    if (!inFile) {
        cerr << "Error: Log file '" << logFile << "' not found.\n";
        return;
    }
    if (!outFile) {
        cerr << "Error: Could not create output file.\n";
        return;
    }

    vector<thread> threads;
    vector<string> chunks;
    string buffer(CHUNK_SIZE, '\0');

    while (inFile.read(&buffer[0], CHUNK_SIZE) || inFile.gcount() > 0) {
        chunks.emplace_back(buffer.substr(0, inFile.gcount()));
        threads.emplace_back(processChunk, ref(chunks.back()), ref(targetDate), ref(outFile));
    }

    for (auto& t : threads) {
        t.join();
    }

    cout << "Filtered logs saved to " << outputFile << "\n";
}

int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);

    if (argc != 3) {
        cerr << "Usage: ./script <log_file> <YYYY-MM-DD>\n";
        return 1;
    }

    string logFile = argv[1];
    string targetDate = argv[2];

    if (targetDate.length() != 10 || targetDate[4] != '-' || targetDate[7] != '-') {
        cerr << "Error: Invalid date format. Use YYYY-MM-DD.\n";
        return 1;
    }

    filterLogsByDate(logFile, targetDate);
    return 0;
}
