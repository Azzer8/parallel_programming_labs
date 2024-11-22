#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;


class GpuSpec {
    string model;
    int memorySize;
    friend class ClusterNode;

public:
    GpuSpec(const string& model, int memorySize) 
        : model(model), memorySize(memorySize) {}

protected:
    void Print() const {
        cout << "GPU Model: " << model << ", Memory Size: " << memorySize << " MB" << endl;
    }

    void Import(ifstream& in) {
        
    }

    void Export(ofstream& out) const {
        
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
    
    }

    void Export(ofstream& out) const {
        
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
        
    }

    void Export(ofstream& out) const {
        
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
        
    }

    void Export(ofstream& out) const {
        
    }
};


class ClusterNode {
    GpuSpec gpu;
    CpuSpec cpu;
    RamSpec ram;
    LanSpec lan;
    friend class Cluster;

public:
    ClusterNode(const string& gpuModel, int gpuMemorySize, const string& cpuModel, int cpuCores, float cpuFrequency, int ramSize, int ramSpeed, const string& lanType, int lanSpeed)
        : gpu(gpuModel, gpuMemorySize), cpu(cpuModel, cpuCores, cpuFrequency), ram(ramSize, ramSpeed), lan(lanType, lanSpeed) {}


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
            cout << "Node " << i + 1 << ":" << endl;
            nodes[i].Print();
        }
    }

    void Import(const string& filename) {
        return;
    }

    void Export(const string& filename) const {
        return;
    }
};


int main() {
    ClusterNode node1 (
        "NVIDIA RTX 4090", 24576, "Intel i9-14900K", 8, 3.5, 
        64, 3200, "Ethernet", 1000
    );

    Cluster cluster1;
    cluster1.AddNode(node1);
    cluster1.Print();
    return 0;
}
