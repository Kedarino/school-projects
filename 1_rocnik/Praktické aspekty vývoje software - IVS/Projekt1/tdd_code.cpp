//======== Copyright (c) 2023, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - graph
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2023-03-07
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author Martin Dočekal
 * @author Karel Ondřej
 *
 * @brief Implementace metod tridy reprezentujici graf.
 */

#include "tdd_code.h"


Graph::Graph(){}

Graph::~Graph(){}

std::vector<Node*> Graph::nodes() {
    std::vector<Node*> nodes;
    int size = AllNodes.size();
    for(int i = 0; i < size; i++)
    {
        Node *p = &AllNodes[i];
        nodes.push_back(p);
    }
    return nodes;
}

std::vector<Edge> Graph::edges() const{
    std::vector<Edge> edges;
    int size = ALLEgdes.size();
    for(int i = 0; i < size; i++)
    {
        Edge p = ALLEgdes[i];
        edges.push_back(p);

    }
    return edges;
}

Node* Graph::addNode(size_t nodeId) {
    Node NewNode;
    NewNode.id = nodeId;
    NewNode.color = 0;
   int size =  AllNodes.size();
    for(int i = 0; i<size ;i++)
    {
        if(AllNodes[i].id == NewNode.id)
        {
            return nullptr;
        }
    }
    AllNodes.push_back(NewNode);
    return  &(AllNodes.back());
       
     
    
}

bool Graph::addEdge(const Edge& edge){
    if(edge.a == edge.b)
    {
        return false;
    }

    if(containsEdge(edge))
    {
        return false;
    }

    bool check_a = false;
    bool check_b = false;

    for(int i = 0; i < AllNodes.size(); i++)
    {
        if(AllNodes[i].id == edge.a)
        {
            check_a = true;
        }
        if(AllNodes[i].id == edge.b)
        {
            check_b = true;
        }

    }
    if(check_a == false)
        {
            addNode(edge.a);
        }
    if(check_b == false)
        {
            addNode(edge.b);
        }
        
    ALLEgdes.push_back(edge);
    return true;

}

void Graph::addMultipleEdges(const std::vector<Edge>& edges) {
    
    for(int i = 0; i < edges.size(); i++)
    {
        addEdge(edges[i]);

    }
}

Node* Graph::getNode(size_t nodeId){
    for(int i = 0; i< AllNodes.size();i++)
    {
        if(AllNodes[i].id == nodeId)
        {
            return(&AllNodes[i]);
        }
    }
    return nullptr;
}

bool Graph::containsEdge(const Edge& edge) const{
    for(int i = 0; i < ALLEgdes.size(); i++)
    {
        if(ALLEgdes[i].operator==(edge))
        {
            return true;
        }
    }
    return false;
}

void Graph::removeNode(size_t nodeId){
    
    for(int i = 0; i < AllNodes.size(); i++)
    {
        if(AllNodes[i].id == nodeId)
        {
            for(int l = 0; l < ALLEgdes.size(); l++)
            {
                if(ALLEgdes[l].a == nodeId || ALLEgdes[l].b == nodeId)
                {
                    removeEdge(ALLEgdes[l]);
                    l -= 1;
                    
                }
                
            }
            
            AllNodes.erase(AllNodes.begin() + i);
            return;
        }
    }
    
    throw std::out_of_range ("Out of range");
}

void Graph::removeEdge(const Edge& edge){
    
    for( int i = 0; i < ALLEgdes.size();i++)
    {
        if(ALLEgdes[i].a == edge.a && ALLEgdes[i].b == edge.b)
        {
            ALLEgdes.erase(ALLEgdes.begin()+i);
            return;
        }
    }
    throw std::out_of_range ("Out of range");
    
}

size_t Graph::nodeCount() const{
    
    return AllNodes.size();
}

size_t Graph::edgeCount() const{
    return ALLEgdes.size();
}

size_t Graph::nodeDegree(size_t nodeId) const{
    bool check_node = false;
    for(int i = 0; i < AllNodes.size(); i++)
    {
        if(nodeId == AllNodes[i].id)
        {
            check_node = true;
            break;
        }
    } 
    if(check_node == false)
    {
       throw std::out_of_range ("Out of range"); 
    }
    
    int count = 0;
    for(int i = 0; i < ALLEgdes.size(); i++)
    {
        if(ALLEgdes[i].a == nodeId || ALLEgdes[i].b == nodeId)
        {
            count++;
        }
    }
    return count;
    
}

size_t Graph::graphDegree() const{
    int max = 0;
    for(int i = 1; i < AllNodes.size(); i++)
    {
        if(nodeDegree(AllNodes[i].id) > max)
        {
            max = nodeDegree(AllNodes[i].id);
        }
    }
    return max;
}


void Graph::coloring(){
    
    std::vector<int> available_colors;
    std::vector<int> neighbour;
    for(int i = 0; i < graphDegree() + 1; i++)
    {
        available_colors.push_back(i+1); 
    }
    for(int i = 0; i < AllNodes.size(); i++)
    {
        for(int l = 0; l < ALLEgdes.size(); l++)
        {
            if(ALLEgdes[l].a == AllNodes[i].id)
            {
                for(int k = 0; k < AllNodes.size(); k++)
                {
                    if(AllNodes[k].id == ALLEgdes[l].b)
                    {
                        neighbour.push_back(AllNodes[k].color);
                    }

                }

            }
            if(ALLEgdes[l].b == AllNodes[i].id)
            {
                for(int k = 0; k < AllNodes.size(); k++)
                {
                    if(AllNodes[k].id == ALLEgdes[l].a)
                    {
                        neighbour.push_back(AllNodes[k].color);
                    }

                }
            }

        }
        for(int h = 0; h < available_colors.size(); h++)
        {
            bool check = false;
            for(int j = 0; j < neighbour.size(); j++)
            {
                if(available_colors[h] == neighbour[j])
                {
                    check = true;
                }
            }
            if (check == false)
            {
                AllNodes[i].color = available_colors[h];
                break;
            }
            
        }
        

    }
}

void Graph::clear() {
    AllNodes.clear();
    ALLEgdes.clear();
}

/*** Konec souboru tdd_code.cpp ***/
