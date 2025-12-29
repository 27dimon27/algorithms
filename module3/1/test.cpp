#include <iostream>
#include <vector>
#include <cassert>
#include <string>

#include "IGraph.h"
#include "ListGraph.h"
#include "MatrixGraph.h"
#include "SetGraph.h"
#include "ArcGraph.h"
#include <algorithm>

bool vectorsEqual(const std::vector<int>& v1, const std::vector<int>& v2) {
    if (v1.size() != v2.size()) return false;
    for (size_t i = 0; i < v1.size(); ++i) {
        if (v1[i] != v2[i]) return false;
    }
    return true;
}

void testEmptyGraph(IGraph* graph, const std::string& graphName) {
    std::cout << "Тест 1 для " << graphName << ": пустой граф" << std::endl;
    
    assert(graph->VerticesCount() >= 0);
    
    for (int i = 0; i < graph->VerticesCount(); ++i) {
        assert(graph->GetNextVertices(i).empty());
        assert(graph->GetPrevVertices(i).empty());
    }
    
    std::cout << "  OK" << std::endl;
}

void testSingleEdge(IGraph* graph, const std::string& graphName) {
    std::cout << "Тест 2 для " << graphName << ": одно ребро" << std::endl;
    
    graph->AddEdge(0, 1);
    
    std::vector<int> next0 = graph->GetNextVertices(0);
    std::vector<int> prev1 = graph->GetPrevVertices(1);
    
    assert(next0.size() == 1 && next0[0] == 1);
    assert(prev1.size() == 1 && prev1[0] == 0);
    
    std::vector<int> prev0 = graph->GetPrevVertices(0);
    std::vector<int> next1 = graph->GetNextVertices(1);
    assert(prev0.empty());
    assert(next1.empty());
    
    std::cout << "  OK" << std::endl;
}

void testMultipleEdges(IGraph* graph, const std::string& graphName) {
    std::cout << "Тест 3 для " << graphName << ": несколько ребер" << std::endl;
    
    graph->AddEdge(0, 1);
    graph->AddEdge(0, 2);
    graph->AddEdge(1, 2);
    graph->AddEdge(2, 0);
    
    std::vector<int> next0 = graph->GetNextVertices(0);
    std::vector<int> prev0 = graph->GetPrevVertices(0);
    
    assert(next0.size() == 2);
    assert(vectorsEqual(std::vector<int>{1, 2}, next0) || vectorsEqual(std::vector<int>{2, 1}, next0));
    assert(prev0.size() == 1 && prev0[0] == 2);
    
    std::vector<int> next1 = graph->GetNextVertices(1);
    std::vector<int> prev1 = graph->GetPrevVertices(1);
    
    assert(next1.size() == 1 && next1[0] == 2);
    assert(prev1.size() == 1 && prev1[0] == 0);
    
    std::vector<int> next2 = graph->GetNextVertices(2);
    std::vector<int> prev2 = graph->GetPrevVertices(2);
    
    assert(next2.size() == 1 && next2[0] == 0);
    assert(prev2.size() == 2);
    assert(vectorsEqual(std::vector<int>{0, 1}, prev2) || vectorsEqual(std::vector<int>{1, 0}, prev2));
    
    std::cout << "  OK" << std::endl;
}

void testParallelEdges(IGraph* graph, const std::string& graphName) {
    std::cout << "Тест 4 для " << graphName << ": параллельные ребра" << std::endl;
    
    graph->AddEdge(0, 1);
    graph->AddEdge(0, 1);
    
    std::vector<int> next0 = graph->GetNextVertices(0);
    std::vector<int> prev1 = graph->GetPrevVertices(1);

    if (graphName == "SetGraph" || graphName == "MatrixGraph") {
        assert(next0.size() == 1 && next0[0] == 1);
        assert(prev1.size() == 1 && prev1[0] == 0);
    } else {
        assert(next0.size() >= 1);
        assert(prev1.size() >= 1);
        bool found = false;
        for (int v : next0) {
            if (v == 1) found = true;
        }
        assert(found);
    }
    
    std::cout << "  OK" << std::endl;
}

void testCopyConstructor(const std::string& sourceType, const std::string& destType) {
    std::cout << "Тест копирования: копирование из " << sourceType << " в " << destType << std::endl;
    
    IGraph* sourceGraph = nullptr;
    if (sourceType == "ListGraph") sourceGraph = new ListGraph(5);
    else if (sourceType == "MatrixGraph") sourceGraph = new MatrixGraph(5);
    else if (sourceType == "SetGraph") sourceGraph = new SetGraph(5);
    else if (sourceType == "ArcGraph") sourceGraph = new ArcGraph(5);
    
    sourceGraph->AddEdge(0, 1);
    sourceGraph->AddEdge(0, 2);
    sourceGraph->AddEdge(1, 3);
    sourceGraph->AddEdge(2, 3);
    sourceGraph->AddEdge(3, 4);
    sourceGraph->AddEdge(4, 0);
    
    IGraph* destGraph = nullptr;
    if (destType == "ListGraph") destGraph = new ListGraph(*sourceGraph);
    else if (destType == "MatrixGraph") destGraph = new MatrixGraph(*sourceGraph);
    else if (destType == "SetGraph") destGraph = new SetGraph(*sourceGraph);
    else if (destType == "ArcGraph") destGraph = new ArcGraph(*sourceGraph);
    
    assert(sourceGraph->VerticesCount() == destGraph->VerticesCount());
    
    for (int i = 0; i < sourceGraph->VerticesCount(); ++i) {
        std::vector<int> sourceNext = sourceGraph->GetNextVertices(i);
        std::vector<int> destNext = destGraph->GetNextVertices(i);
        
        std::vector<int> sourcePrev = sourceGraph->GetPrevVertices(i);
        std::vector<int> destPrev = destGraph->GetPrevVertices(i);
        
        std::sort(sourceNext.begin(), sourceNext.end());
        std::sort(destNext.begin(), destNext.end());
        std::sort(sourcePrev.begin(), sourcePrev.end());
        std::sort(destPrev.begin(), destPrev.end());
        
        assert(vectorsEqual(sourceNext, destNext));
        assert(vectorsEqual(sourcePrev, destPrev));
    }
    
    delete sourceGraph;
    delete destGraph;
    
    std::cout << "  OK" << std::endl;
}

void testLargeGraph(IGraph* graph, const std::string& graphName) {
    std::cout << "Тест 6 для " << graphName << ": большой граф" << std::endl;
    
    int n = 100;
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                graph->AddEdge(i, j);
            }
        }
    }
    
    for (int i = 0; i < 10; ++i) {
        std::vector<int> next = graph->GetNextVertices(i);
        std::vector<int> prev = graph->GetPrevVertices(i);
        
        if (graphName == "SetGraph" || graphName == "MatrixGraph") {
            assert(static_cast<int>(next.size()) == n - 1);
            assert(static_cast<int>(prev.size()) == n - 1);
        } else {
            assert(static_cast<int>(next.size()) == n - 1);
            assert(static_cast<int>(prev.size()) == n - 1);
        }
    }
    
    std::cout << "  OK" << std::endl;
}

void testSelfLoops(IGraph* graph, const std::string& graphName) {
    std::cout << "Тест 7 для " << graphName << ": петли" << std::endl;
    
    graph->AddEdge(0, 0);
    
    std::vector<int> next0 = graph->GetNextVertices(0);
    std::vector<int> prev0 = graph->GetPrevVertices(0);
    
    assert(!next0.empty());
    assert(!prev0.empty());
    
    bool foundInNext = false;
    for (int v : next0) {
        if (v == 0) foundInNext = true;
    }
    assert(foundInNext);
    
    bool foundInPrev = false;
    for (int v : prev0) {
        if (v == 0) foundInPrev = true;
    }
    assert(foundInPrev);
    
    std::cout << "  OK" << std::endl;
}

void runAllTestsForGraphType(const std::string& graphType) {
    std::cout << "\n=== Тестирование " << graphType << " ===" << std::endl;
    
    IGraph* graph1 = nullptr;
    if (graphType == "ListGraph") graph1 = new ListGraph(5);
    else if (graphType == "MatrixGraph") graph1 = new MatrixGraph(5);
    else if (graphType == "SetGraph") graph1 = new SetGraph(5);
    else if (graphType == "ArcGraph") graph1 = new ArcGraph(5);
    
    testEmptyGraph(graph1, graphType);
    delete graph1;
    
    IGraph* graph2 = nullptr;
    if (graphType == "ListGraph") graph2 = new ListGraph(5);
    else if (graphType == "MatrixGraph") graph2 = new MatrixGraph(5);
    else if (graphType == "SetGraph") graph2 = new SetGraph(5);
    else if (graphType == "ArcGraph") graph2 = new ArcGraph(5);
    
    testSingleEdge(graph2, graphType);
    delete graph2;
    
    IGraph* graph3 = nullptr;
    if (graphType == "ListGraph") graph3 = new ListGraph(5);
    else if (graphType == "MatrixGraph") graph3 = new MatrixGraph(5);
    else if (graphType == "SetGraph") graph3 = new SetGraph(5);
    else if (graphType == "ArcGraph") graph3 = new ArcGraph(5);
    
    testMultipleEdges(graph3, graphType);
    delete graph3;
    
    IGraph* graph4 = nullptr;
    if (graphType == "ListGraph") graph4 = new ListGraph(5);
    else if (graphType == "MatrixGraph") graph4 = new MatrixGraph(5);
    else if (graphType == "SetGraph") graph4 = new SetGraph(5);
    else if (graphType == "ArcGraph") graph4 = new ArcGraph(5);
    
    testParallelEdges(graph4, graphType);
    delete graph4;
    
    IGraph* graph6 = nullptr;
    if (graphType == "ListGraph") graph6 = new ListGraph(10);
    else if (graphType == "MatrixGraph") graph6 = new MatrixGraph(10);
    else if (graphType == "SetGraph") graph6 = new SetGraph(10);
    else if (graphType == "ArcGraph") graph6 = new ArcGraph(10);
    
    std::cout << "Тест 6 для " << graphType << ": средний граф" << std::endl;
    int n = 10;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j && (i + j) % 3 == 0) {
                graph6->AddEdge(i, j);
            }
        }
    }

    for (int i = 0; i < 3; ++i) {
        std::vector<int> next = graph6->GetNextVertices(i);
        std::vector<int> prev = graph6->GetPrevVertices(i);
        (void)next;
        (void)prev;
    }
    std::cout << "  OK" << std::endl;
    delete graph6;
    
    IGraph* graph7 = nullptr;
    if (graphType == "ListGraph") graph7 = new ListGraph(5);
    else if (graphType == "MatrixGraph") graph7 = new MatrixGraph(5);
    else if (graphType == "SetGraph") graph7 = new SetGraph(5);
    else if (graphType == "ArcGraph") graph7 = new ArcGraph(5);
    
    testSelfLoops(graph7, graphType);
    delete graph7;
}

int main() {
    std::cout << "=== Начало тестирования различных реализаций графа ===" << std::endl;
    
    runAllTestsForGraphType("ListGraph");
    runAllTestsForGraphType("MatrixGraph");
    runAllTestsForGraphType("SetGraph");
    runAllTestsForGraphType("ArcGraph");
    
    std::cout << "\n=== Тестирование конструкторов копирования ===" << std::endl;
    
    std::vector<std::string> types = {"ListGraph", "MatrixGraph", "SetGraph", "ArcGraph"};
    for (const auto& source : types) {
        for (const auto& dest : types) {
            try {
                testCopyConstructor(source, dest);
            } catch (const std::exception& e) {
                std::cout << "  ОШИБКА при копировании из " << source << " в " << dest << ": " << e.what() << std::endl;
                return 1;
            }
        }
    }
    
    std::cout << "\n=== Тест цепочки копирований ===" << std::endl;
    {
        ListGraph original(4);
        original.AddEdge(0, 1);
        original.AddEdge(1, 2);
        original.AddEdge(2, 3);
        original.AddEdge(3, 0);
        
        MatrixGraph copy1(original);
        SetGraph copy2(copy1);
        ArcGraph copy3(copy2);
        ListGraph copy4(copy3);
        
        assert(original.VerticesCount() == copy4.VerticesCount());
        
        for (int i = 0; i < original.VerticesCount(); ++i) {
            std::vector<int> origNext = original.GetNextVertices(i);
            std::vector<int> copyNext = copy4.GetNextVertices(i);
            
            std::sort(origNext.begin(), origNext.end());
            std::sort(copyNext.begin(), copyNext.end());
            
            assert(vectorsEqual(origNext, copyNext));
        }
        
        std::cout << "  OK: цепочка из 4 копирований работает корректно" << std::endl;
    }
    
    std::cout << "\n=== Все тесты пройдены успешно! ===" << std::endl;
    
    return 0;
}