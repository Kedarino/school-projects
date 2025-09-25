//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author JMENO PRIJMENI
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//
struct EmptyTree : public ::testing::Test
{   
    Node_t *TestNode;
    int TestKey = 3;
protected:
    BinaryTree TestTree;    
};
TEST_F(EmptyTree, InsertNode)
{
       
    auto result = TestTree.InsertNode(TestKey);
    EXPECT_TRUE(result.first);
    EXPECT_TRUE(result.second!= nullptr);
}

TEST_F(EmptyTree, DeleteNode)
{
    bool result = TestTree.DeleteNode(TestKey);
    EXPECT_FALSE(result);
    
}

TEST_F(EmptyTree, FindNode)
{
    TestNode = TestTree.FindNode(TestKey);
    EXPECT_EQ(TestNode, nullptr);
}


struct NonEmptyTree : public ::testing::Test
{   
    Node_t *RootNode;
    Node_t *TestNode;
    int TestKey = 3;
    int NewKey = 6;
    
    void SetUp() override{
    TestTree.InsertNode(TestKey);
    TestTree.InsertNode(2);
    TestTree.InsertNode(4);
    TestTree.InsertNode(5);
    TestTree.InsertNode(1);
    }
    
    protected:
    BinaryTree TestTree;
    
};

TEST_F(NonEmptyTree, InsertNode_KeyExists)
{
    auto result = TestTree.InsertNode(TestKey);
    EXPECT_FALSE(result.first);
    EXPECT_EQ(result.second->key, TestKey);
} 

TEST_F(NonEmptyTree, InsertNode_KeyDNExists)
{
    
    auto result =  TestTree.InsertNode(NewKey);
    EXPECT_TRUE(result.first);
    EXPECT_EQ(result.second->key, NewKey);
}

TEST_F(NonEmptyTree, DeleteNode_KeyExists)
{
    bool result = TestTree.DeleteNode(TestKey);
    EXPECT_TRUE(result);
    Node_t *resultTest = TestTree.FindNode(TestKey);
    EXPECT_TRUE(resultTest == nullptr);
    
}

TEST_F(NonEmptyTree, DeleteNode_KeyDNExists)
{
    bool result = TestTree.DeleteNode(NewKey);
    EXPECT_FALSE(result);
    
}

TEST_F(NonEmptyTree, FindNode_KeyExists)
{
    Node_t *result = TestTree.FindNode(TestKey);
    EXPECT_TRUE(result != nullptr); 
    EXPECT_EQ(result->key, TestKey);
}

TEST_F(NonEmptyTree, FindNode_KeyDNExists)
{
    Node_t *result = TestTree.FindNode(NewKey);
    EXPECT_TRUE(result == nullptr);
}
TEST_F(NonEmptyTree, RootTest)
{
    Node_t *result = TestTree.GetRoot();
    EXPECT_EQ(result->key, 3);
}

struct TreeAxioms : public ::testing::Test
{
    Node_t *Root;
    void SetUp() override{
        TestTree.InsertNode(3);
        TestTree.InsertNode(2);
        TestTree.InsertNode(4);
        TestTree.InsertNode(5);
        TestTree.InsertNode(1);
        Root = TestTree.GetRoot();
    }
    

    protected:
    BinaryTree TestTree;
};

TEST_F(TreeAxioms, Axiom1)
{
    std::vector<Node_t *> LeafNodes;
    TestTree.GetLeafNodes(LeafNodes);
    bool check = true;
    
    for(int i = 0; i < LeafNodes.size(); i++)
    {
        if(LeafNodes[i]->color == RED)
        {
            check = false;
            break;
        }
    }
    EXPECT_TRUE(check);
}

TEST_F(TreeAxioms, Axiom2)
{
    std::vector<Node_t *> NonLeafNodes;
    TestTree.GetNonLeafNodes(NonLeafNodes);
    
    
    for(int i = 0; i < NonLeafNodes.size(); i++)
    {
        if(NonLeafNodes[i]->color == RED)
        {
            EXPECT_EQ(NonLeafNodes[i]->pLeft->color, BLACK);
            EXPECT_EQ(NonLeafNodes[i]->pRight->color, BLACK);
        }
    }
    
}

TEST_F(TreeAxioms, Axiom3)
{
   
   Node_t *RootTest;
   int check, NumberOfBlack;
   std::vector<Node_t *> LeafNodes;
   TestTree.GetLeafNodes(LeafNodes);
   for( int i = 0; i < LeafNodes.size(); i++)
   { 
        NumberOfBlack = 0;
        RootTest = LeafNodes[i]->pParent;
        do
        {   
            if(RootTest->color == BLACK)
            {
                NumberOfBlack++;
            }
            RootTest = RootTest->pParent;
        }
        while(RootTest != Root);
    
        if(i == 0)
        {
            check = NumberOfBlack;    
        }
        EXPECT_EQ(NumberOfBlack, check);
          
    }
    

}

/*** Konec souboru black_box_tests.cpp ***/
