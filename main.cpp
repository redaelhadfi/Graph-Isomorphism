#define _USE_MATH_DEFINES
#include <cmath>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Spinner.H>
#include <FL/Fl_Input.H>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include "graph_utils.h"

#include "graph_utils.h"  // Include the Graph structure and function declaration

// Function declaration
bool areGraphsIsomorphic(const Graph& g1, const Graph& g2);

// Graph structure


// Global variables
Graph graph1;
Graph graph2;
Fl_Text_Buffer* textBuffer;

class GraphWidget : public Fl_Box {
    const Graph* graph;

public:
    GraphWidget(int x, int y, int w, int h, const char* label = nullptr)
        : Fl_Box(x, y, w, h, label), graph(nullptr) {}

    void setGraph(const Graph* g) {
        graph = g;
        redraw();
    }
void draw() override {
    Fl_Box::draw();

    if (!graph || graph->numVertices == 0) return;

    int radius = 20;  // Radius of vertices
    int centerX = x() + w() / 2, centerY = y() + h() / 2;
    int vertices = graph->numVertices;
    double angleStep = 2 * M_PI / vertices;

    // Draw edges with weights
    for (int i = 0; i < vertices; ++i) {
        int vx = centerX + std::cos(i * angleStep) * (w() / 3);
        int vy = centerY + std::sin(i * angleStep) * (h() / 3);

        for (int j = i + 1; j < vertices; ++j) {  // Avoid duplicating edges
            int weight = graph->adjacencyMatrix[i][j];
            if (weight > 0) {  // Only draw edges with positive weights
                int wx = centerX + std::cos(j * angleStep) * (w() / 3);
                int wy = centerY + std::sin(j * angleStep) * (h() / 3);

                // Set edge color based on weight
                fl_color(FL_BLUE);
                if (weight > 5) fl_color(FL_RED);  // Highlight heavier edges

                // Draw edge line
                fl_line(vx, vy, wx, wy);

                // Draw weight at midpoint with a background circle
                int mx = (vx + wx) / 2;
                int my = (vy + wy) / 2;
                fl_color(FL_BLACK);
                fl_pie(mx - 10, my - 10, 20, 20, 0, 360);  // Background circle for weight
                fl_color(FL_WHITE);
                fl_draw(std::to_string(weight).c_str(), mx - 5, my + 5);
            }
        }
    }

    // Draw vertices with labels
    for (int i = 0; i < vertices; ++i) {
        int vx = centerX + std::cos(i * angleStep) * (w() / 3);
        int vy = centerY + std::sin(i * angleStep) * (h() / 3);

        // Draw vertex background
        fl_color(FL_DARK_GREEN);
        fl_pie(vx - radius - 2, vy - radius - 2, (radius + 2) * 2, (radius + 2) * 2, 0, 360);

        // Draw vertex circle
        fl_color(FL_GREEN);
        fl_pie(vx - radius, vy - radius, radius * 2, radius * 2, 0, 360);

        // Draw vertex label
        fl_color(FL_WHITE);
        fl_draw(std::to_string(i + 1).c_str(), vx - 5, vy + 5);
    }
}

};

GraphWidget* graph1Widget;
GraphWidget* graph2Widget;

class InputDialog : public Fl_Window {
    Graph* targetGraph;
    GraphWidget* targetWidget;
    std::vector<std::vector<Fl_Input*>> matrixInputs;
    Fl_Spinner* vertexSpinner;

public:
    InputDialog(Graph* graph, GraphWidget* widget, const char* title)
        : Fl_Window(700, 600, title), targetGraph(graph), targetWidget(widget) {
        vertexSpinner = new Fl_Spinner(150, 20, 60, 25, "Vertices:");
        vertexSpinner->minimum(1);
        vertexSpinner->maximum(10);
        vertexSpinner->value(graph->numVertices > 0 ? graph->numVertices : 1);
        vertexSpinner->callback(spinner_callback, this);

        createMatrix(static_cast<int>(vertexSpinner->value()));

        Fl_Button* createButton = new Fl_Button(250, 500, 100, 30, "Create Graph");
        createButton->callback(create_callback, this);

        this->end();
    }

    static void spinner_callback(Fl_Widget*, void* v) {
        InputDialog* dialog = (InputDialog*)v;
        dialog->recreateMatrix();
    }

    static void create_callback(Fl_Widget*, void* v) {
        InputDialog* dialog = (InputDialog*)v;
        dialog->createGraph();
    }

    void recreateMatrix() {
        for (auto& row : matrixInputs) {
            for (auto& input : row) {
                remove(input);
                delete input;
            }
        }
        matrixInputs.clear();
        createMatrix(static_cast<int>(vertexSpinner->value()));
        this->redraw();
    }

void createMatrix(int size) {
    int xOffset = 50, yOffset = 70, inputWidth = 40, spacing = 50;

    matrixInputs.resize(size);
    for (int i = 0; i < size; ++i) {
        matrixInputs[i].resize(size);
        for (int j = 0; j < size; ++j) {
            Fl_Input* input = new Fl_Input(
                xOffset + j * spacing, yOffset + i * spacing, inputWidth, inputWidth
            );
            input->value("0");  // Default weight is 0 (no edge)
            matrixInputs[i][j] = input;

            if (i == j) {
                input->deactivate();  // Disable self-loops
            }
        }
    }
}

void createGraph() {
    int size = static_cast<int>(vertexSpinner->value());
    targetGraph->numVertices = size;
    targetGraph->adjacencyMatrix.resize(size, std::vector<int>(size, 0));

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            try {
                int weight = std::stoi(matrixInputs[i][j]->value());
                if (weight < 0) throw std::runtime_error("Invalid weight.");
                targetGraph->adjacencyMatrix[i][j] = weight;
            } catch (...) {
                textBuffer->append("Error: Invalid input in adjacency matrix.\n");
                return;
            }
        }
    }

    targetWidget->setGraph(targetGraph);
    hide();
}

};

void clearResults(Fl_Widget*, void*) {
    graph1 = {};
    graph2 = {};
    graph1Widget->setGraph(nullptr);
    graph2Widget->setGraph(nullptr);
    textBuffer->text("");
}

void inputGraph1(Fl_Widget*, void*) {
    InputDialog* dialog = new InputDialog(&graph1, graph1Widget, "Input Graph 1");
    dialog->show();
}

void inputGraph2(Fl_Widget*, void*) {
    InputDialog* dialog = new InputDialog(&graph2, graph2Widget, "Input Graph 2");
    dialog->show();
}



void checkIsomorphism(Fl_Widget*, void*) {
    if (graph1.numVertices == 0 || graph2.numVertices == 0) {
        textBuffer->append("Error: Create both graphs first.\n");
        return;
    }

bool result = areGraphsIsomorphic(static_cast<const Graph&>(graph1), static_cast<const Graph&>(graph2));
    textBuffer->append(result ? "Graphs are isomorphic.\n" : "Graphs are NOT isomorphic.\n");
}

int main() {
    textBuffer = new Fl_Text_Buffer();

    Fl_Window* window = new Fl_Window(1200, 750, "Graph Isomorphism Checker");
    window->color(FL_DARK3);

    Fl_Button* inputGraph1Button = new Fl_Button(50, 30, 200, 40, "Input Graph 1");
    inputGraph1Button->callback(inputGraph1);

    Fl_Button* inputGraph2Button = new Fl_Button(50, 90, 200, 40, "Input Graph 2");
    inputGraph2Button->callback(inputGraph2);

    Fl_Button* checkButton = new Fl_Button(50, 150, 200, 40, "Check Isomorphism");
    checkButton->callback(checkIsomorphism);

    Fl_Button* clearButton = new Fl_Button(50, 210, 200, 40, "Clear");
    clearButton->callback(clearResults);

    Fl_Text_Display* textDisplay = new Fl_Text_Display(300, 30, 850, 200);
    textDisplay->buffer(textBuffer);

    graph1Widget = new GraphWidget(50, 300, 500, 400, "Graph 1");
    graph2Widget = new GraphWidget(650, 300, 500, 400, "Graph 2");

    window->end();
    window->show();
    return Fl::run();
}
