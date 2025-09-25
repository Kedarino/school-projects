//======== Copyright (c) 2022, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - test suite
//
// $NoKeywords: $ivs_project_1 $white_box_tests.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2023-03-07
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author JMENO PRIJMENI
 * 
 * @brief Implementace testu hasovaci tabulky.
 */

#include <vector>

#include "gtest/gtest.h"

#include "white_box_code.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy hasovaci tabulky, testujte nasledujici:
// 1. Verejne rozhrani hasovaci tabulky
//     - Vsechny funkce z white_box_code.h
//     - Chovani techto metod testuje pro prazdnou i neprazdnou tabulku.
// 2. Chovani tabulky v hranicnich pripadech
//     - Otestujte chovani pri kolizich ruznych klicu se stejnym hashem 
//     - Otestujte chovani pri kolizich hashu namapovane na stejne misto v 
//       indexu
//============================================================================//

struct EmptyMap : public ::testing::Test
{
   
    hash_map *TestMap;
    void SetUp()
    {
        TestMap = hash_map_ctor();

    }

    void TearDown()
    {
        hash_map_dtor(TestMap);
    }    
};

TEST_F(EmptyMap, ctor)
{
    
    EXPECT_EQ(TestMap->allocated, 8);
    EXPECT_EQ(TestMap->used, 0);
    EXPECT_TRUE(TestMap->first == NULL && TestMap->last == NULL);
    EXPECT_TRUE(TestMap != nullptr);
}

/*TEST_F(EmptyMap, dtor)
{   
   
    hash_map_dtor(TestMap);
    EXPECT_TRUE(TestMap == NULL);
    EXPECT_EQ(TestMap->index[0], nullptr);
    
}*/

TEST_F(EmptyMap, clear)
{
    hash_map_clear(TestMap);
    EXPECT_TRUE(TestMap->first == NULL && TestMap->last == NULL);
    EXPECT_EQ(TestMap->used, 0);
    EXPECT_EQ(TestMap->allocated, 8);

}

TEST_F(EmptyMap, reserve)
{
    EXPECT_TRUE(TestMap-> allocated == 8);
    hash_map_state_code_t check = hash_map_reserve(TestMap, 16);
    EXPECT_EQ(TestMap->allocated, 16);
    EXPECT_EQ(check, OK);
    check = hash_map_reserve(TestMap, 16);
    EXPECT_EQ(TestMap->allocated, 16);
    EXPECT_EQ(check, OK);
}

TEST_F(EmptyMap, capacity)
{
    size_t check = hash_map_capacity(TestMap);
    EXPECT_EQ(check, 8);

}

TEST_F(EmptyMap, size)
{
    size_t check = hash_map_size(TestMap);
    EXPECT_EQ(check, 0);
 
}

TEST_F(EmptyMap, contains)
{
    bool check = hash_map_contains(TestMap, "key");
    EXPECT_TRUE(check == false);
}

TEST_F(EmptyMap, put)
{
    const char *checkKey;
    checkKey = "check_key";
    int checkValue = 1;
    hash_map_state_code_t check = hash_map_put(TestMap, checkKey, checkValue);
    EXPECT_TRUE(check == OK);
    EXPECT_TRUE(strcmp(TestMap->first->key, checkKey) == 0);
    EXPECT_EQ(TestMap->first->value, checkValue);
}

TEST_F(EmptyMap, get)
{
    const char *checkKey;
    checkKey = "check_key";
    int checkValue;
    
    hash_map_state_code_t check = hash_map_get(TestMap, checkKey, &checkValue);
    EXPECT_EQ(check, KEY_ERROR);
}

TEST_F(EmptyMap, pop)
{
    const char *checkKey;
    checkKey = "check_key";
    int checkValue = 0;
    hash_map_state_code_t check = hash_map_pop(TestMap, checkKey, &checkValue);
    EXPECT_EQ(checkValue, NULL);
    EXPECT_EQ(check, KEY_ERROR);
}

TEST_F(EmptyMap, remove)
{
    const char *checkKey;
    checkKey = "check_key";
    hash_map_state_code_t check = hash_map_remove(TestMap, checkKey);
    EXPECT_EQ(check, KEY_ERROR);

}

struct NonEmptyMap : public ::testing::Test
{
   
    hash_map *TestMap;
    void SetUp()
    {
        TestMap = hash_map_ctor();
        hash_map_put(TestMap, "a", 1);
        hash_map_put(TestMap, "b", 2);
        hash_map_put(TestMap, "d", 5);
        hash_map_put(TestMap, "x", 10);
        

    }

    void TearDown()
    {
        hash_map_dtor(TestMap);
    }    
};

TEST_F(NonEmptyMap, ctor)
{
    TestMap = hash_map_ctor();
    EXPECT_EQ(TestMap->allocated, 8);
    EXPECT_EQ(TestMap->used, 0);
    EXPECT_TRUE(TestMap->first == NULL && TestMap->last == NULL);
    EXPECT_TRUE(TestMap != nullptr);
}

TEST_F(NonEmptyMap, dtor)
{

}

TEST_F(NonEmptyMap, clear)
{
    hash_map_clear(TestMap);
    EXPECT_TRUE(TestMap->first == NULL && TestMap->last == NULL);
    EXPECT_EQ(TestMap->used, 0);
    EXPECT_EQ(TestMap->allocated, 8);
}

TEST_F(NonEmptyMap, reserve)
{
    //alokace stejného počtu
    hash_map_state_code_t check = hash_map_reserve(TestMap, 5);
    EXPECT_EQ(check, OK);
    EXPECT_EQ(TestMap->allocated, 5);

    //alokace menšího počtu (nelze)
    check = hash_map_reserve(TestMap, 2);
    EXPECT_EQ(check, VALUE_ERROR);
    EXPECT_EQ(TestMap->allocated, 5);

    //alokace vetšího počtu
    check = hash_map_reserve(TestMap, 10);
    EXPECT_EQ(check, OK);
    EXPECT_EQ(TestMap->allocated, 10);
    EXPECT_EQ(TestMap->used, 4);
}

TEST_F(NonEmptyMap, size)
{
    size_t check = hash_map_size(TestMap);
    EXPECT_EQ(check, 4);
}

TEST_F(NonEmptyMap, capacity)
{
    size_t check = hash_map_capacity(TestMap);
    EXPECT_EQ(check, 8);
}

TEST_F(NonEmptyMap, contains)
{
    bool check = hash_map_contains(TestMap, "a");
    EXPECT_EQ(check, true);
    check = hash_map_contains(TestMap, "aloha");
    EXPECT_EQ(check, false);

}

TEST_F(NonEmptyMap, put)
{
    hash_map_state_code_t check = hash_map_put(TestMap, "a", 1);
    EXPECT_EQ(check, KEY_ALREADY_EXISTS);

    check = hash_map_put(TestMap, "aloha", 11);
    EXPECT_EQ(check, OK);
    EXPECT_EQ(TestMap->allocated, 8);

    check = hash_map_put(TestMap, "aloha2", 12);
    EXPECT_EQ(check,OK);
    EXPECT_EQ(TestMap->allocated, 16);

}

TEST_F(NonEmptyMap, get)
{
    int checkValue = 0;
    hash_map_state_code_t check = hash_map_get(TestMap, "aloha", &checkValue);
    EXPECT_EQ(check, KEY_ERROR);
    EXPECT_EQ(checkValue, 0);
    check = hash_map_get(TestMap, "a", &checkValue);
    EXPECT_EQ(check, OK);
    EXPECT_EQ(checkValue, 1);
    
}

TEST_F(NonEmptyMap, pop)
{
    int checkValue = 0;
    hash_map_state_code_t check = hash_map_pop(TestMap, "aloha", &checkValue);
    EXPECT_EQ(checkValue, NULL);
    EXPECT_EQ(check, KEY_ERROR);

    checkValue = 0;
    check = hash_map_pop(TestMap, "a", &checkValue);
    EXPECT_EQ(checkValue, 1);
    EXPECT_FALSE(hash_map_contains(TestMap, "a"));
    EXPECT_EQ(check, OK);
}

TEST_F(NonEmptyMap, remove)
{
    hash_map_state_code_t check = hash_map_remove(TestMap, "aloha");
    EXPECT_EQ(check, KEY_ERROR);
    
    check = hash_map_remove(TestMap, "a");
    EXPECT_EQ(check, OK);
    EXPECT_FALSE(hash_map_contains(TestMap,"a"));

    check = hash_map_remove(TestMap, "b");
    EXPECT_EQ(check, OK);
    EXPECT_FALSE(hash_map_contains(TestMap,"b"));

    check = hash_map_remove(TestMap, "d");
    EXPECT_EQ(check, OK);
    EXPECT_FALSE(hash_map_contains(TestMap,"d"));

    check = hash_map_remove(TestMap, "x");
    EXPECT_EQ(check, OK);
    EXPECT_FALSE(hash_map_contains(TestMap,"x"));


}




/*** Konec souboru white_box_tests.cpp ***/
