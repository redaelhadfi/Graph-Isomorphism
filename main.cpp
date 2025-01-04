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

    int radius = 25;          // Radius of vertices
    int selfLoopOffset = 35;  // Offset for self-loop arcs
    int edgeWeightRadius = 15; // Radius for edge weight background
    int centerX = x() + w() / 2, centerY = y() + h() / 2;
    int vertices = graph->numVertices;
    double angleStep = 2 * M_PI / vertices;

    // Draw edges and self-loops
    for (int i = 0; i < vertices; ++i) {
        int vx = centerX + std::cos(i * angleStep) * (w() / 3);
        int vy = centerY + std::sin(i * angleStep) * (h() / 3);

        for (int j = 0; j < vertices; ++j) {
            int weight = graph->adjacencyMatrix[i][j];
            if (weight > 0) {
                if (i == j) {
                    // Self-loop: Draw as an arc
                    fl_color(fl_rgb_color(255, 100, 100)); // Modern red
                    fl_arc(vx - selfLoopOffset, vy - selfLoopOffset, selfLoopOffset * 2, selfLoopOffset * 2, 0, 300);

                    // Self-loop weight
                    fl_color(fl_rgb_color(255, 255, 255)); // White weight text
                    fl_draw(std::to_string(weight).c_str(), vx + selfLoopOffset, vy - selfLoopOffset - 5);
                } else if (i < j) {  // Avoid duplicate edges
                    int wx = centerX + std::cos(j * angleStep) * (w() / 3);
                    int wy = centerY + std::sin(j * angleStep) * (h() / 3);

                    // Draw edge with variable thickness
                    fl_color(fl_rgb_color(100, 200, 255)); // Light blue
                    fl_line_style(FL_SOLID, std::min(3 + weight, 7)); // Thickness based on weight
                    fl_line(vx, vy, wx, wy);

                    // Edge weight at midpoint
                    int mx = (vx + wx) / 2;
                    int my = (vy + wy) / 2;
                    fl_color(fl_rgb_color(50, 50, 50)); // Dark background
                    fl_pie(mx - edgeWeightRadius, my - edgeWeightRadius, edgeWeightRadius * 2, edgeWeightRadius * 2, 0, 360);
                    fl_color(fl_rgb_color(255, 255, 255)); // White text
                    fl_draw(std::to_string(weight).c_str(), mx - 8, my + 5);
                }
            }
        }
    }

    // Draw vertices
    for (int i = 0; i < vertices; ++i) {
        int vx = centerX + std::cos(i * angleStep) * (w() / 3);
        int vy = centerY + std::sin(i * angleStep) * (h() / 3);

        // Shadow
        fl_color(fl_rgb_color(50, 50, 50)); // Dark gray
        fl_pie(vx - radius - 5, vy - radius - 5, (radius + 5) * 2, (radius + 5) * 2, 0, 360);

        // Border
        fl_color(fl_rgb_color(80, 200, 120)); // Modern green
        fl_pie(vx - radius, vy - radius, radius * 2, radius * 2, 0, 360);

        // Label background
        fl_color(fl_rgb_color(255, 255, 255)); // White
        fl_pie(vx - radius + 5, vy - radius + 5, (radius - 5) * 2, (radius - 5) * 2, 0, 360);

        // Vertex label
        fl_color(fl_rgb_color(0, 0, 0)); // Black text
        fl_draw(std::to_string(i + 1).c_str(), vx - 8, vy + 5);
    }

    fl_line_style(0); // Reset line style
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

    // Clear existing inputs
    for (auto& row : matrixInputs) {
        for (auto& input : row) {
            remove(input);
            delete input;
        }
    }
    matrixInputs.clear();

    // Resize and populate new inputs
    matrixInputs.resize(size);
    for (int i = 0; i < size; ++i) {
        matrixInputs[i].resize(size);
        for (int j = 0; j < size; ++j) {
            Fl_Input* input = new Fl_Input(
                xOffset + j * spacing, yOffset + i * spacing, inputWidth, inputWidth
            );
            if (i < targetGraph->numVertices && j < targetGraph->numVertices) {
                input->value(std::to_string(targetGraph->adjacencyMatrix[i][j]).c_str());
            } else {
                input->value("0");
            }
            matrixInputs[i][j] = input;
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
    // Reset the graph structures
    graph1.numVertices = 0;
    graph1.adjacencyMatrix.clear();
    graph2.numVertices = 0;
    graph2.adjacencyMatrix.clear();

    // Clear the graph widgets
    graph1Widget->setGraph(nullptr);
    graph1Widget->redraw();
    graph2Widget->setGraph(nullptr);
    graph2Widget->redraw();

    // Clear the text buffer
    textBuffer->text("");

    // Optional: Log the reset action
    textBuffer->append("Graphs and UI cleared successfully.\n");
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

    Fl_Window* window = new Fl_Window(1200, 800, "Graph Isomorphism Checker");
    window->color(fl_rgb_color(240, 240, 240)); // Light gray background

    // Title Label
    Fl_Box* titleLabel = new Fl_Box(0, 10, 1200, 40, "Graph Isomorphism Checker");
    titleLabel->labelsize(24);
    titleLabel->labelfont(FL_BOLD);
    titleLabel->align(FL_ALIGN_CENTER);

    // Buttons Section
    Fl_Group* buttonGroup = new Fl_Group(50, 60, 250, 200, "Controls");
    buttonGroup->box(FL_ENGRAVED_BOX);
    buttonGroup->color(fl_rgb_color(220, 250, 220)); // Light gray
    buttonGroup->align(FL_ALIGN_TOP_LEFT);

    Fl_Button* inputGraph1Button = new Fl_Button(60, 80, 200, 30, "Input Graph 1");
    inputGraph1Button->color(fl_rgb_color(100, 200, 255)); // Light blue
    inputGraph1Button->labelcolor(FL_WHITE);
    inputGraph1Button->labelfont(FL_BOLD);
    inputGraph1Button->callback(inputGraph1);

    Fl_Button* inputGraph2Button = new Fl_Button(60, 130, 200, 30, "Input Graph 2");
    inputGraph2Button->color(fl_rgb_color(100, 200, 255));
    inputGraph2Button->labelcolor(FL_WHITE);
    inputGraph2Button->labelfont(FL_BOLD);
    inputGraph2Button->callback(inputGraph2);

    Fl_Button* checkButton = new Fl_Button(60, 180, 200, 30, "Check Isomorphism");
    checkButton->color(fl_rgb_color(50, 150, 50)); // Green
    checkButton->labelcolor(FL_WHITE);
    checkButton->labelfont(FL_BOLD);
    checkButton->callback(checkIsomorphism);

    Fl_Button* clearButton = new Fl_Button(60, 220, 200, 30, "Clear");
    clearButton->color(fl_rgb_color(200, 50, 50)); // Red
    clearButton->labelcolor(FL_WHITE);
    clearButton->labelfont(FL_BOLD);
    clearButton->callback(clearResults);

    buttonGroup->end();

    // Text Display Section
    Fl_Group* textGroup = new Fl_Group(320, 60, 850, 200, "Console Output");
    textGroup->box(FL_ENGRAVED_BOX);
    textGroup->color(fl_rgb_color(220, 220, 220)); // Light gray
    textGroup->align(FL_ALIGN_TOP_LEFT);

    Fl_Text_Display* textDisplay = new Fl_Text_Display(330, 90, 830, 160);
    textDisplay->buffer(textBuffer);
    textDisplay->box(FL_DOWN_BOX);
    textDisplay->color(FL_BLACK);
    textDisplay->textcolor(FL_WHITE);
    textDisplay->textfont(FL_COURIER);
    textDisplay->textsize(14);

    textGroup->end();

    // Graph Widgets Section
    Fl_Group* graphGroup = new Fl_Group(50, 280, 1100, 450, "Graph Visualization");
    graphGroup->box(FL_ENGRAVED_BOX);
    graphGroup->color(fl_rgb_color(220, 220, 220));
    graphGroup->align(FL_ALIGN_TOP_LEFT);

    graph1Widget = new GraphWidget(60, 300, 500, 400, "Graph 1");
    graph1Widget->box(FL_ENGRAVED_BOX);
    graph1Widget->color(fl_rgb_color(240, 240, 240));

    graph2Widget = new GraphWidget(640, 300, 500, 400, "Graph 2");
    graph2Widget->box(FL_ENGRAVED_BOX);
    graph2Widget->color(fl_rgb_color(240, 240, 240));

    graphGroup->end();

    // Show the window
    window->end();
    window->show();
    return Fl::run();
}
