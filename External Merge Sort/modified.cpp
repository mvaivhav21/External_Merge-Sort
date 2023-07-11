#include <bits/stdc++.h>
using namespace std;

struct HeapNode {
    string sentence;
    int index;
    HeapNode(const string& a, int b) : sentence(a), index(b) {}
    bool operator<(const HeapNode& rhs) const {
        return sentence.length() < rhs.sentence.length();
    }
};

int createRuns(const string& inputName, int totalMem) {
    ifstream input(inputName);
    if (!input) {
        exit(-1);
    }

    int runCount = 0;
    int totalMemSoFar = 0;

    ofstream output;
    vector<string> data;

    while (!input.eof()) {
        string sentence;
        getline(input, sentence);

        if (totalMemSoFar + sentence.size() < totalMem) {
            totalMemSoFar += sentence.size() + 1;
            data.push_back(sentence);
        } else {
            sort(data.begin(), data.end());

            runCount++;
            stringstream ss;
            ss << "run_" << runCount << ".txt";
            output.open(ss.str());

            for (size_t i = 0; i < data.size(); i++) {
                output << data[i] << endl;
            }

            output.close();
            data.clear();
            totalMemSoFar = sentence.size();
            data.push_back(sentence);
        }
    }

    if (!data.empty()) {
        sort(data.begin(), data.end());

        runCount++;
        stringstream ss;
        ss << "run_" << runCount << ".txt";
        output.open(ss.str());

        for (size_t i = 0; i < data.size(); i++) {
            output << data[i] << endl;
        }

        output.close();
    }

    input.close();
    return runCount;
}

void mergeRuns(int start, int end, int location) {
    int runsCount = end - start + 1;

    vector<ifstream> input(runsCount);
    for (int i = 0; i < runsCount; i++) {
        stringstream ss;
        ss << "run_" << start + i << ".txt";
        input[i].open(ss.str());
    }

    priority_queue<HeapNode, vector<HeapNode>> heap;

    ofstream output;
    stringstream ss;
    ss << "run_" << location << ".txt";
    output.open(ss.str());

    for (int i = 0; i < runsCount; i++) {
        string sentence;
        if (getline(input[i], sentence)) {
            heap.push(HeapNode(sentence, i));
        }
    }

    while (!heap.empty()) {
        string sentence = heap.top().sentence;
        int index = heap.top().index;
        heap.pop();

        output << sentence << endl;

        if (getline(input[index], sentence)) {
            heap.push(HeapNode(sentence, index));
        }
    }

    for (int i = 0; i < runsCount; i++) {
        input[i].close();
    }

    output.close();
}

void mergeAllRuns(int runsCount, const string& outputName) {
    int start = 1;
    int end = runsCount;

    while (start < end) {
        int location = end;
        int distance = 100;
        int time = (end - start + 1) / distance + 1;

        if ((end - start + 1) / time < distance) {
            distance = (end - start + 1) / time + 1;
        }

        while (start <= end) {
            int mid = min(start + distance, end);
            location++;
            mergeRuns(start, mid, location);
            start = mid + 1;
        }

        end = location;
    }

    stringstream ss;
    ss << "run_" << start << ".txt";
    rename(ss.str().c_str(), outputName.c_str());

    for (int i = 1; i < end; i++) {
        stringstream ss;
        ss << "run_" << i << ".txt";
        remove(ss.str().c_str());
    }
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cout << "Usage: " << argv[0] << " input_file output_file mem_size" << endl;
        return 0;
    }

    string inputName = argv[1];
    string outputName = argv[2];
    int totalMem = strtol(argv[3], nullptr, 0);

    createRuns(inputName, totalMem);
    int runsCount = createRuns(inputName, totalMem);
    mergeAllRuns(runsCount, outputName);

    return 0;
}
