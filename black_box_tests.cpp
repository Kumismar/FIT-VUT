//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     Ondrej Koumar <xkouma02@stud.fit.vutbr.cz>
// $Date:       $2022-03-10
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author Ondrej Koumar
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

class EmptyTree : public ::testing::Test
{
    protected:
        BinaryTree tree;
};

class NonEmptyTree : public ::testing::Test
{
    protected:
        BinaryTree tree;

        virtual void SetUp()
        {
            int values[8] = {10, 5, 85, 15, 90, 40, 30, 20};        // hodnoty v neprazdnem stromu

            for (int i = 0; i < 8; i++)
            {
                tree.InsertNode(values[i]);
            }
        }
};

TEST_F(EmptyTree, InsertNode)
{
    EXPECT_TRUE(tree.GetRoot() == NULL);

    tree.InsertNode(5);
    EXPECT_TRUE(tree.GetRoot() != NULL);
    EXPECT_EQ(tree.GetRoot()->key, 5);
}

TEST_F(EmptyTree, FindNode)
{
    ASSERT_TRUE(tree.GetRoot() == NULL);        // asi by stacilo expect, ale v momente, kdy emptytree ma root, tak vsechny ostatni testy budou neuspech
    EXPECT_TRUE(tree.FindNode(10) == NULL);
}

TEST_F(EmptyTree, DeleteNode)
{
    ASSERT_TRUE(tree.GetRoot() == NULL);
    EXPECT_FALSE(tree.DeleteNode(8));
}

TEST_F(NonEmptyTree, InsertNode)
{
    EXPECT_TRUE(tree.GetRoot() != NULL);
    
    std::pair <bool, BinaryTree::Node_t*> existing_pair = tree.InsertNode(85);      // vlozeni jiz existujici node
    bool existing_node = existing_pair.first;
    BinaryTree::Node_t *pointer_existing_node = existing_pair.second;

    EXPECT_FALSE(existing_node);
    EXPECT_EQ(pointer_existing_node->key, 85);

    std::pair <bool, BinaryTree::Node_t*> non_existing_pair = tree.InsertNode(35);      // vlozeni zatim neexistujici node
    bool new_existing_node = non_existing_pair.first;
    BinaryTree::Node_t *pointer_new_existing_node = non_existing_pair.second;

    EXPECT_TRUE(new_existing_node);
    EXPECT_EQ(pointer_new_existing_node->key, 35);
}

TEST_F(NonEmptyTree, FindNode)
{
    BinaryTree::Node_t *existing_node = tree.FindNode(5);       // nalezeni existujici node
    EXPECT_TRUE(existing_node != NULL); 
    EXPECT_TRUE(existing_node->key == 5);

    BinaryTree::Node_t *non_existing_node = tree.FindNode(57);      // hledani neexistujici node
    EXPECT_TRUE(non_existing_node == NULL);
}

TEST_F(NonEmptyTree, DeleteNode)
{
    EXPECT_TRUE(tree.DeleteNode(90));       // smazani existujici node
    EXPECT_FALSE(tree.DeleteNode(42));      // smazani neexistujici node

    EXPECT_TRUE(tree.DeleteNode(10));       // smazani rootu a nalezeni noveho
    EXPECT_EQ(tree.GetRoot()->key, 30);
}

TEST_F(NonEmptyTree, Axiom_1)
{
    std::vector <BinaryTree::Node_t*> leaves;       
    tree.GetLeafNodes(leaves);

    for (BinaryTree::Node_t *leaf : leaves)     // projizdi pole pointeru na leaves a testuje kazdy z nich
    {
        EXPECT_TRUE(leaf->color == BLACK);
        EXPECT_TRUE(leaf->pLeft == NULL);
        EXPECT_TRUE(leaf->pRight == NULL);
        EXPECT_TRUE(leaf->pParent != NULL);
    }
}

TEST_F(NonEmptyTree, Axiom_2)
{   
    std::vector <BinaryTree::Node_t*> nodes;
    tree.GetNonLeafNodes(nodes);

    for (BinaryTree::Node_t *node : nodes)      // vezme vsechny nodes krome listu a pokud je cervene barvy, potomci musi byt cerni
    {
        if (node->color == RED)
        {
            EXPECT_TRUE(node->pRight->color == BLACK);
            EXPECT_TRUE(node->pLeft->color == BLACK);
        }
    }
}
 

TEST_F(NonEmptyTree, Axiom_3)
{
    std::vector <BinaryTree::Node_t*> leaves;
    tree.GetLeafNodes(leaves);

    unsigned previous_black_nodes = 0;

    for (BinaryTree::Node_t *current_leaf : leaves)     // vezme listy jeden po jednom a postupne projede az ke korenu
    {
        unsigned black_nodes = 0;       

        while (current_leaf->pParent != NULL)
        {
            if (current_leaf->pParent->color == BLACK)
            {
                black_nodes++;      // po ceste ke korenu pocita nodes, ktere jsou cerne
            }

            current_leaf = current_leaf->pParent;
        }

        if (previous_black_nodes != 0)      // pokud je to prvni list (predtim se nic neporovnavalo, takze previous_back_nodes == 0), tak se nic neporovnava
        {                                   // 0, protoze vzdy v prechozi ceste byl aspon jeden cerny (pocita se i root)
            EXPECT_EQ(black_nodes, previous_black_nodes);
        }

        previous_black_nodes = black_nodes;
    }
}

/*** Konec souboru black_box_tests.cpp ***/
