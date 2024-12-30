#define _USE_MATH_DEFINES // For M_PI
#include <cmath>          // For cos, sin, and M_PI
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Box.H>
#include "graph_utils.h"
#include <iostream>
#include <sstream>
#include <chrono>

// Global variables
Graph graph1, graph2;
Fl_Text_Buffer* textBuffer = new Fl_Text_Buffer();

// Custom Widget for Graph Visualization
class GraphWidget : public Fl_Box {
    const Graph* graph;

public:
    GraphWidget(int x, int y, int w, int h, const char* label = nullptr)
        : Fl_Box(x, y, w, h, label), graph(nullptr) {}

    void setGraph(const Graph* g) {
        graph = g;
        redraw(); // Trigger a redraw when a new graph is set
    }

    void draw() override {
        Fl_Box::draw(); // Draw the default box

        if (!graph || graph->numVertices == 0) return;

        int radius = 20; // Vertex radius
        int centerX = x() + w() / 2, centerY = y() + h() / 2;
        int vertices = graph->numVertices;
        double angleStep = 2 * M_PI / vertices;

        // Draw edges and vertices
        for (int i = 0; i < vertices; ++i) {
            int vx = centerX + std::cos(i * angleStep) * (w() / 3);
            int vy = centerY + std::sin(i * angleStep) * (h() / 3);

            // Draw edges
            for (int j = 0; j < vertices; ++j) {
                if (graph->adjacencyMatrix[i][j] == 1) {
                    int wx = centerX + std::cos(j * angleStep) * (w() / 3);
                    int wy = centerY + std::sin(j * angleStep) * (h() / 3);
                    fl_color(FL_BLUE);
                    fl_line(vx, vy, wx, wy);
                }
            }

            // Draw vertices
            fl_color(FL_GREEN);
            fl_pie(vx - radius, vy - radius, radius * 2, radius * 2, 0, 360);
            fl_color(FL_WHITE);
            fl_draw(std::to_string(i + 1).c_str(), vx - 5, vy + 5);
        }
    }
};

// Graph Widgets
GraphWidget* graph1Widget;
GraphWidget* graph2Widget;

// Clear Results
void clearResults(Fl_Widget* w, void* p) {
    graph1 = {};
    graph2 = {};
    graph1Widget->setGraph(nullptr);
    graph2Widget->setGraph(nullptr);
    textBuffer->text(""); // Clear the results display
}

// Load Graph 1
void loadGraph1(Fl_Widget* w, void* p) {
    const char* file = fl_file_chooser("Load Graph 1", "*.txt", "");
    if (file) {
        try {
            graph1 = loadGraphFromFile(file);
            graph1Widget->setGraph(&graph1); // Update visualization
            textBuffer->append("Graph 1 loaded successfully.\n");
        } catch (const std::exception& e) {
            textBuffer->append(("Error: " + std::string(e.what()) + "\n").c_str());
        }
    }
}

// Load Graph 2
void loadGraph2(Fl_Widget* w, void* p) {
    const char* file = fl_file_chooser("Load Graph 2", "*.txt", "");
    if (file) {
        try {
            graph2 = loadGraphFromFile(file);
            graph2Widget->setGraph(&graph2); // Update visualization
            textBuffer->append("Graph 2 loaded successfully.\n");
        } catch (const std::exception& e) {
            textBuffer->append(("Error: " + std::string(e.what()) + "\n").c_str());
        }
    }
}

// Check Isomorphism
void checkIsomorphism(Fl_Widget* w, void* p) {
    if (graph1.numVertices == 0 || graph2.numVertices == 0) {
        textBuffer->append("Error: Please load both graphs before checking isomorphism.\n");
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();
    bool result = areGraphsIsomorphic(graph1, graph2);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::ostringstream oss;
    oss << (result ? "The graphs are isomorphic.\n" : "The graphs are NOT isomorphic.\n");
    oss << "Execution time: " << elapsed.count() << " seconds.\n";
    textBuffer->append(oss.str().c_str());
}

// Main Application
int main() {
    Fl_Window* window = new Fl_Window(1200, 750, "Graph Isomorphism Checker");
    window->color(FL_DARK3); // Background color

    // Buttons
    Fl_Button* loadGraph1Button = new Fl_Button(50, 30, 200, 40, "Load Graph 1");
    loadGraph1Button->color(FL_GREEN);
    loadGraph1Button->labelcolor(FL_WHITE);
    loadGraph1Button->callback(loadGraph1);

    Fl_Button* loadGraph2Button = new Fl_Button(50, 90, 200, 40, "Load Graph 2");
    loadGraph2Button->color(FL_GREEN);
    loadGraph2Button->labelcolor(FL_WHITE);
    loadGraph2Button->callback(loadGraph2);

    Fl_Button* checkButton = new Fl_Button(50, 150, 200, 40, "Check Isomorphism");
    checkButton->color(FL_BLUE);
    checkButton->labelcolor(FL_WHITE);
    checkButton->callback(checkIsomorphism);

    Fl_Button* clearButton = new Fl_Button(50, 210, 200, 40, "Clear");
    clearButton->color(FL_RED);
    clearButton->labelcolor(FL_WHITE);
    clearButton->callback(clearResults);

    // Results Display
    Fl_Text_Display* textDisplay = new Fl_Text_Display(300, 30, 850, 200);
    textDisplay->buffer(textBuffer);
    textDisplay->color(FL_BLACK);
    textDisplay->textcolor(FL_WHITE);

    // Graph Visualization Widgets
    graph1Widget = new GraphWidget(50, 300, 500, 400, "Graph 1");
    graph1Widget->box(FL_BORDER_BOX);
    graph1Widget->color(FL_GRAY);

    graph2Widget = new GraphWidget(650, 300, 500, 400, "Graph 2");
    graph2Widget->box(FL_BORDER_BOX);
    graph2Widget->color(FL_GRAY);

    window->end();
    window->show();
    return Fl::run();
}
