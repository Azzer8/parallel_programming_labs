#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;


class GpuSpec {
    string model;
    int memorySize; // MB
    friend class ClusterNode;

public:
    GpuSpec(const string& model, int memorySize) 
        : model(model), memorySize(memorySize) {}

protected:
    void Print() const {
        cout << "GPU Model: " << model << ", Memory Size: " << memorySize << " MB" << endl;
    }

    void Import(ifstream& in) {
        getline(in, model, ',');
        model.erase(0, model.find_first_not_of(" \n\r\t"));
        in >> memorySize;
    }

    void Export(ofstream& out) const {
        out << model << "," << memorySize << endl;
    }
};


class CpuSpec {
    string model;
    int cores;
    float frequency;
    friend class ClusterNode;

public:
    CpuSpec(const string& model, int cores, float frequency) 
        : model(model), cores(cores), frequency(frequency) {}

protected:
    void Print() const {
        cout << "CPU Model: " << model << ", Cores: " << cores << ", Frequency: " << frequency << " GHz" << endl;
    }

    void Import(ifstream& in) {
        getline(in, model, ',');
        model.erase(0, model.find_first_not_of(" \n\r\t"));
        in >> cores;
        in.ignore();
        in >> frequency;
    }

    void Export(ofstream& out) const {
        out << model << "," << cores << "," << frequency << endl;
    }
};


class RamSpec {
    int size;
    int speed;
    friend class ClusterNode;

public:
    RamSpec(int size, int speed) : size(size), speed(speed) {}

protected:
    void Print() const {
        cout << "RAM Size: " << size << " GB, Speed: " << speed << " MHz" << endl;
    }

    void Import(ifstream& in) {
        in >> size;
        in.ignore();
        in >> speed;
    }

    void Export(ofstream& out) const {
        out << size << "," << speed << endl;
    }
};


class LanSpec {
    string type;
    int speed;
    friend class ClusterNode;

public:
    LanSpec(const string& type, int speed) : type(type), speed(speed) {}

protected:
    void Print() const {
        cout << "LAN Type: " << type << ", Speed: " << speed << " Mbps" << endl;
    }

    void Import(ifstream& in) {
        getline(in, type, ',');
        type.erase(0, type.find_first_not_of(" \n\r\t"));
        in >> speed;
    }

    void Export(ofstream& out) const {
        out << type << "," << speed << endl;
    }
};


class ClusterNode {
    GpuSpec gpu;
    CpuSpec cpu;
    RamSpec ram;
    LanSpec lan;
    friend class Cluster;

public:
    ClusterNode()
    : gpu("", 0), cpu("", 0, 0.0), ram(0, 0), lan("", 0) {}

    ClusterNode(const string& gpuModel, int gpuMemorySize, 
                    const string& cpuModel, int cpuCores, float cpuFrequency, 
                    int ramSize, int ramSpeed, 
                    const string& lanType, int lanSpeed)
        : gpu(gpuModel, gpuMemorySize), cpu(cpuModel, cpuCores, cpuFrequency), 
        ram(ramSize, ramSpeed), lan(lanType, lanSpeed) {}

protected:
    void Print() const {
        cout << "--- Cluster Node ---" << endl;
        gpu.Print();
        cpu.Print();
        ram.Print();
        lan.Print();
    }

    void Import(ifstream& in) {
        gpu.Import(in);
        cpu.Import(in);
        ram.Import(in);
        lan.Import(in);
    }

    void Export(ofstream& out) const {
        gpu.Export(out);
        cpu.Export(out);
        ram.Export(out);
        lan.Export(out);
    }
};


struct Cluster {
    vector<ClusterNode> nodes;

public:
    void AddNode(const ClusterNode& node) {
        nodes.push_back(node);
    }

    void Print() const {
        cout << "=== Cluster ===" << endl;
        for (size_t i = 0; i < nodes.size(); ++i) {
            cout << "\n\tNode " << i + 1 << ":" << endl;
            nodes[i].Print();
        }
    }

    void Import(const string& filename) {
        ifstream inputFile(filename);
        if (!inputFile.is_open()) {
            cerr << "Error opening " << filename << " for reading" << endl;
            return;
        }

        size_t nodeCount;
        inputFile >> nodeCount;

        nodes.clear();
        nodes.reserve(nodeCount);
        for (size_t i = 0; i < nodeCount; ++i) {
            ClusterNode node;
            node.Import(inputFile);
            nodes.push_back(node);
        }

        inputFile.close();
        cout << "\nCluster data imported from " << filename << endl;
    }

    void Export(const string& filename) const {
        ofstream outputFile(filename);
        if (!outputFile.is_open()) {
            cerr << "Error opening " << filename << " for writing" << endl;
            return;
        }

        outputFile << nodes.size() << endl;
        for (const auto& node : nodes) {
            node.Export(outputFile);
        }

        outputFile.close();
        cout << "\nCluster data exported to " << filename << endl;
    }
};


int main() {
    ClusterNode node1 (
        "NVIDIA RTX 4090", 24'576, "Intel i9-14900K", 8, 3.5, 
        64, 3'200, "Ethernet", 1'000
    );
    ClusterNode node2 (
        "NVIDIA  H100", 496'000, "AMD Ryzen 9 8900X", 16, 5.5, 
        128, 5'600, "Ethernet", 10'000
    );

    Cluster cluster1;
    cluster1.AddNode(node1);
    cluster1.AddNode(node2);
    cluster1.Print();
    cluster1.Export("cluster_data.txt");
    
    Cluster importedCluster;
    importedCluster.Import("cluster_data.txt");
    importedCluster.Print();
    return 0;
}
