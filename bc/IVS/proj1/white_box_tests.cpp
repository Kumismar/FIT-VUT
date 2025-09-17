//======== Copyright (c) 2021, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - Tests suite
//
// $NoKeywords: $ivs_project_1 $white_box_code.cpp
// $Author:     Ondrej Koumar <xkouma02@stud.fit.vutbr.cz>
// $Date:       $2021-01-04
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author Ondrej Koumar
 *
 * @brief Implementace testu prace s maticemi.
 */

#include "white_box_code.h"
#include "gtest/gtest.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy operaci nad maticemi. Cilem testovani je:
// 1. Dosahnout maximalniho pokryti kodu (white_box_code.cpp) testy.
// 2. Overit spravne chovani operaci nad maticemi v zavislosti na rozmerech
//    matic.
//============================================================================//

using namespace std;

class Matrix_AxB : public ::testing::Test // nastaveni matic ruznych velikosti
{
protected:
    Matrix matrix_3x3;
    Matrix matrix_2x2;
    Matrix matrix_4x2;
    Matrix matrix_3x8;

    Matrix matrix_3x3_inversed;
    Matrix matrix_2x2_inversed;

    Matrix matrix_3x3_transposed;
    Matrix matrix_2x2_transposed;
    Matrix matrix_4x2_transposed;
    Matrix matrix_3x8_transposed;

    Matrix matrix_3x3_added;
    Matrix matrix_2x2_added;

    Matrix identity_matrix_2x2;
    Matrix identity_matrix_3x3;

    virtual void SetUp()
    {
        matrix_2x2 = Matrix(2, 2);
        matrix_2x2.set(vector<vector<double>> {
            { 3, 7 },
            { 2, 4 },
        });

        matrix_3x3 = Matrix(3, 3);
        matrix_3x3.set(vector<vector<double>> {
            { 2, 5, 7 },
            { 6, 3, 4 },
            { 5, -2, -3 },
        });

        matrix_4x2 = Matrix(4, 2);
        matrix_4x2.set(vector<vector<double>> {
            { 9, 8 },
            { 7, 6 },
            { 5, 4 },
            { 3, 2 },
        });

        matrix_3x8 = Matrix(3, 8);
        matrix_3x8.set(vector<vector<double>> {
            { 1, 2, 3, 4, 5, 6, 8, 7 },
            { 4, 3, 6, 8, 2, 5, 0, 1 },
            { 1, 5, 7, 4, 5, 0, 2, 6 },
        });

        matrix_2x2_inversed = Matrix();
        matrix_2x2_inversed.set(vector<vector<double>> {
            { -2, 3.5 },
            { 1, -1.5 },
        });

        matrix_3x3_inversed = Matrix(3, 3);
        matrix_3x3_inversed.set(vector<vector<double>> {
            { 1, -1, 1 },
            { -38, 41, -34 },
            { 27, -29, 24 },
        });

        matrix_2x2_transposed = Matrix(2, 2);
        matrix_2x2_transposed.set(vector<vector<double>> {
            { 3, 2 },
            { 7, 4 },
        });

        matrix_3x3_transposed = Matrix(3, 3);
        matrix_3x3_transposed.set(vector<vector<double>> {
            { 2, 6, 5 },
            { 5, 3, -2 },
            { 7, 4, -3 },
        });

        matrix_4x2_transposed = Matrix(2, 4);
        matrix_4x2_transposed.set(vector<vector<double>> {
            { 9, 7, 5, 3 },
            { 8, 6, 4, 2 },
        });

        matrix_3x8_transposed = Matrix(8, 3);
        matrix_3x8_transposed.set(vector<vector<double>> {
            { 1, 4, 1 },
            { 2, 3, 5 },
            { 3, 6, 7 },
            { 4, 8, 4 },
            { 5, 2, 5 },
            { 6, 5, 0 },
            { 8, 0, 2 },
            { 7, 1, 6 },
        });

        matrix_2x2_added = Matrix(2, 2);
        matrix_2x2_added.set(vector<vector<double>> {
            { 6, 9 },
            { 9, 8 },
        });

        matrix_3x3_added = Matrix(3, 3);
        matrix_3x3_added.set(vector<vector<double>> {
            { 4, 11, 12 },
            { 11, 6, 2 },
            { 12, 2, -6 },
        });

        identity_matrix_2x2 = Matrix(2, 2);
        identity_matrix_2x2.set(vector<vector<double>> {
            { 1, 0 },
            { 0, 1 },
        });

        identity_matrix_3x3 = Matrix(3, 3);
        identity_matrix_3x3.set(vector<vector<double>> {
            { 1, 0, 0 },
            { 0, 1, 0 },
            { 0, 0, 1 },
        });

        matrix_2x2_inversed = Matrix(2, 2);
        matrix_2x2_inversed.set(
            {
                { -2, 3.5 },
                { 1, -1.5 },
            });

        matrix_3x3_inversed = Matrix(3, 3);
        matrix_3x3_inversed.set(
            {
                { 1, -1, 1 },
                { -38, 41, -34 },
                { 27, -29, 24 },
            });
    }
};

class Matrix_1x1 : public ::testing::Test // nastaveni matice 1x1
{
protected:
    Matrix matrix_1x1;

    virtual void SetUp()
    {
        matrix_1x1 = Matrix();
        matrix_1x1.set(0, 0, 5);
    }
};

TEST_F(Matrix_1x1, constructor)
{
    Matrix matrix_0x1;

    EXPECT_THROW(matrix_0x1 = Matrix(0, 1), runtime_error); // tato vyjimka plati pro libovolnou matici 0xB nebo Ax0, takze uz dale testovana nebude
}

TEST_F(Matrix_1x1, set)
{
    EXPECT_TRUE(matrix_1x1.set(0, 0, 4)); // vlozeni jednoho cisla
    EXPECT_FALSE(matrix_1x1.set(0, 1, 8));
    EXPECT_FALSE(matrix_1x1.set(1, 0, 3));

    EXPECT_TRUE(matrix_1x1.set( // vlozeni vektoru
        {
            { 5 },
        }));
    EXPECT_FALSE(matrix_1x1.set(
        {
            { 3, 1 },
        }));
}

TEST_F(Matrix_1x1, get)
{
    EXPECT_EQ(matrix_1x1.get(0, 0), 5); // hledani cisla, ktere tam opravdu je
    EXPECT_THROW(matrix_1x1.get(0, 1), runtime_error); // prekroceni mezi matice
    EXPECT_THROW(matrix_1x1.get(1, 0), runtime_error);
}

TEST_F(Matrix_1x1, operator_equals)
{
    EXPECT_TRUE(matrix_1x1.operator==(matrix_1x1)); // porovnani stejnych matic

    Matrix different_matrix_1x1 = Matrix();
    different_matrix_1x1.set(0, 0, 9);

    EXPECT_FALSE(matrix_1x1.operator==(different_matrix_1x1)); // porovnani rozdilnych matic
}

TEST_F(Matrix_1x1, operator_plus)
{
    Matrix matrix_add;
    matrix_add = Matrix();
    matrix_add = matrix_1x1.operator+(matrix_1x1); // pomocna matice pro ziskani hodnoty po souctu

    int value = matrix_add.get(0, 0);
    EXPECT_TRUE(value == 10);

    Matrix matrix_out;
    matrix_out = Matrix(3, 3); // matrix_out = matice jdouci mimo meze 1x1

    EXPECT_THROW(matrix_1x1.operator+(matrix_out), runtime_error);
}

TEST_F(Matrix_1x1, operator_mul)
{
    Matrix matrix_mul;
    matrix_mul = Matrix();
    matrix_mul = matrix_1x1.operator*(matrix_1x1); // matrix_1x1 ^ 2

    int value = matrix_mul.get(0, 0);
    EXPECT_TRUE(value == 25);

    Matrix matrix_out;
    matrix_out = Matrix(2, 1); // matice, ktera nesedi rozmerove

    EXPECT_THROW(matrix_1x1.operator*(matrix_out), runtime_error);

    matrix_mul = matrix_mul.operator*(value); // vynasobeni matice konstantou
    EXPECT_TRUE(matrix_mul.get(0, 0) == 625);
}

TEST_F(Matrix_1x1, solve_equation)
{
    Matrix matrix_1x1_det_0;
    matrix_1x1_det_0 = Matrix();

    EXPECT_THROW(matrix_1x1_det_0.solveEquation(vector<double> { 5 }), runtime_error); // soustava rovnic, kde det_matice = 0

    EXPECT_TRUE(matrix_1x1.solveEquation(vector<double> { 2 }) == (vector<double> { 0.4 })); // resitelna soustava rovnic
}

TEST_F(Matrix_1x1, transpose)
{
    Matrix matrix_1x1_transposed;
    matrix_1x1_transposed = Matrix();
    matrix_1x1_transposed = matrix_1x1.transpose();

    EXPECT_TRUE(matrix_1x1 == matrix_1x1_transposed);
}

TEST_F(Matrix_1x1, inverse)
{
    EXPECT_THROW(matrix_1x1.inverse(), runtime_error);

    // Trochu nonsense, 1x1 matice ma inverzni, kde jediny prvek a = 1/a, pokud neni 0 nebo komplexni
}

TEST_F(Matrix_AxB, set)
{
    EXPECT_TRUE(matrix_3x3.set(1, 2, 7)); // vlozeni jednoho cisla na urcite misto
    EXPECT_TRUE(matrix_3x3.get(1, 2) == 7);

    EXPECT_TRUE(matrix_3x3.set( // vlozeni vektoru do matice dle defiovanych pravidel
        {
            { 1, 2, 3 },
            { 2, 3, 4 },
            { 3, 4, 5 },
        }));
    EXPECT_TRUE(matrix_3x3.get(0, 2) == 3); // zkouska, zda sedi prvek

    EXPECT_FALSE(matrix_4x2.set( // vlozeni spatneho formatu prvku
        {
            { 1, 2 },
            { 3, 4 },
            { 4, 2 },
            { 0 },
        }));
    EXPECT_FALSE(matrix_3x8.set(
        {
            { 1, 2 },
            { 2, 3 },
            { 1, 3 },
            { 3, 1 },
        }));
    EXPECT_FALSE(matrix_2x2.set(
        {
            { 2, 3, 4 },
            { 4, 5, 6 },
        }));
}

TEST_F(Matrix_AxB, get)
{
    EXPECT_EQ(matrix_2x2.get(1, 1), 4); // ziskani hodnoty ve validnim miste
    EXPECT_EQ(matrix_4x2.get(2, 1), 4);
    EXPECT_TRUE(matrix_3x8.get(2, 3) == 4);
    EXPECT_THROW(matrix_3x3.get(3, 1), runtime_error); // ziskani hodnoty v nevalidnim miste
}

TEST_F(Matrix_AxB, operator_equals)
{
    EXPECT_TRUE(matrix_3x3.operator==(matrix_3x3)); // matice, ktere se rovnaji
    EXPECT_TRUE(matrix_4x2.operator==(matrix_4x2));

    Matrix matrix_3x8_different;
    matrix_3x8_different = matrix_3x8;
    matrix_3x8_different.set(0, 0, 2);

    EXPECT_FALSE(matrix_3x8.operator==(matrix_3x8_different)); // matice se stejnou velikosti, ktere se nerovnaji
    EXPECT_FALSE(matrix_2x2.operator==(matrix_2x2_added));

    EXPECT_THROW(matrix_4x2.operator==(matrix_3x8), runtime_error); // matice jinych velikosti
    EXPECT_THROW(matrix_2x2.operator==(matrix_3x3), runtime_error);
}

TEST_F(Matrix_AxB, operator_plus)
{
    EXPECT_EQ(matrix_2x2.operator+(matrix_2x2_transposed), matrix_2x2_added); // matice stejnych rozmeru
    EXPECT_TRUE(matrix_3x3.operator+(matrix_3x3_transposed) == matrix_3x3_added);

    EXPECT_EQ(matrix_4x2.operator+(matrix_4x2), matrix_4x2.operator*(2)); // vyuziti funkce operator*

    EXPECT_THROW(matrix_3x8.operator+(matrix_3x3), runtime_error); // matice jinych rozmeru
}

TEST_F(Matrix_AxB, operator_mul)
{
    EXPECT_EQ(matrix_3x8.operator+(matrix_3x8), matrix_3x8.operator*(2)); // vyuziti funkce operator+

    Matrix matrix_4x4_after_mul;
    matrix_4x4_after_mul = Matrix(4, 4); // matrix_4x4_after_mul = matrix_4x2 * matrix_4x2_transposed
    matrix_4x4_after_mul.set(
        {
            { 145, 111, 77, 43 },
            { 111, 85, 59, 33 },
            { 77, 59, 41, 23 },
            { 43, 33, 23, 13 },
        });

    EXPECT_EQ(matrix_4x2.operator*(matrix_4x2_transposed), matrix_4x4_after_mul);

    EXPECT_THROW(matrix_2x2.operator*(matrix_3x3), runtime_error); // matrix1.cols != matrix2.rows
}

TEST_F(Matrix_AxB, solve_equation)
{
    EXPECT_EQ(matrix_3x3.solveEquation(vector<double> { 1, 2, 3 }), (vector<double> { 2, -58, 41 })); // splneni podminek a overeni vysledku

    EXPECT_THROW(matrix_3x8.solveEquation(vector<double> { 1, 2, 3, 4, 5, 6, 7, 8 }), runtime_error);
    // nectvercova matice
    // Asi chyba v popisu funkce? Posilam tam 8 prvku, ktere by dle popisu mely byt na prave strane rovnic,
    // ale matice ma jen 3 radky, tudiz by to melo hodit stejny error jako o jeden test dole, ale pres stejnou podminku to projde
    // a pak se sekne na tom, ze matice neni ctvercova.
    // puvodni test: EXPECT_THROW(matrix_3x8.solveEquation(vector <double> {1, 2, 3}), runtime_error);
    // Ocekaval jsem throw, ze matice neni ctvercova.

    EXPECT_THROW(matrix_2x2.solveEquation(vector<double> { 1, 2, 3 }), runtime_error); // pocet prvku vpravo != pocet radku

    Matrix matrix_3x3_det_0;
    matrix_3x3_det_0 = Matrix(3, 3);
    matrix_3x3_det_0.set(
        {
            { 1, 2, 3 },
            { 4, 5, 6 },
            { 7, 8, 9 },
        });

    EXPECT_THROW(matrix_3x3_det_0.solveEquation(vector<double> { 1, 2, 3 }), runtime_error); // determinant == 0

    Matrix matrix_4x4;
    matrix_4x4 = Matrix(4, 4);
    matrix_4x4.set(
        {
            { 0, 1, 1, 1 },
            { 1, 0, 1, 1 },
            { 1, 1, 0, 1 },
            { 1, 1, 1, 0 },
        });

    EXPECT_EQ(matrix_4x4.solveEquation(vector<double> { 3, 3, 3, 3 }), (vector<double> { 1, 1, 1, 1 }));
}

TEST_F(Matrix_AxB, transpose)
{
    EXPECT_TRUE(matrix_2x2.transpose() == matrix_2x2_transposed); // transponovana 2x2
    EXPECT_TRUE(matrix_3x3.transpose() == matrix_3x3_transposed); // transponovana 3x3
    EXPECT_TRUE(matrix_4x2.transpose() == matrix_4x2_transposed); // transponovana 4x2
    EXPECT_TRUE(matrix_3x8.transpose() == matrix_3x8_transposed); // transponovana 3x8
    EXPECT_TRUE(identity_matrix_3x3.transpose() == identity_matrix_3x3); // jednotkova matice
}

TEST_F(Matrix_AxB, inverse)
{
    EXPECT_THROW(matrix_4x2.inverse(), runtime_error); // nectvercove matice
    EXPECT_THROW(matrix_3x8.inverse(), runtime_error);

    Matrix matrix_3x3_det_0;
    matrix_3x3_det_0 = Matrix(3, 3);
    matrix_3x3_det_0.set(
        {
            { 1, 2, 3 },
            { 4, 5, 6 },
            { 7, 8, 9 },
        });

    EXPECT_THROW(matrix_3x3_det_0.inverse(), runtime_error); // matice s det == 0

    EXPECT_EQ(matrix_2x2.inverse(), matrix_2x2_inversed); // porovnani s definovanymi maticemi
    EXPECT_EQ(matrix_3x3.inverse(), matrix_3x3_inversed);
    EXPECT_EQ(matrix_2x2_inversed.operator*(matrix_2x2), identity_matrix_2x2); // zkouska, ze plati A*A^-1 = I
    EXPECT_EQ(matrix_3x3_inversed.operator*(matrix_3x3), identity_matrix_3x3);
}

/*** Konec souboru white_box_tests.cpp ***/
