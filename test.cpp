#include <iostream>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Poly.h"
#include "Matrix.h"


TEST(BaseTests, ConstantTest) {
    Polynomial<int> p(2);
    EXPECT_EQ(p.GetPolyRepresentation(), "2");
    p -= 4;
    EXPECT_EQ(p.GetPolyRepresentation(), "-2");
    p = p + 8;
    EXPECT_EQ(p.GetPolyRepresentation(), "6");
    p = p - 100;
    EXPECT_EQ(p.GetPolyRepresentation(), "-94");
}

TEST(BaseTests, ArithmeticTest) {
    Polynomial<int> q = 1 + x();
    EXPECT_EQ(q.GetPolyRepresentation(), "1 + x");
    q = 1 + 3 * x();
    q = 1 + x() * 3;
    EXPECT_EQ(q.GetPolyRepresentation(), "1 + 3x");
    q = Pow(q, 2);
    EXPECT_EQ(q.GetPolyRepresentation(), "1 + 6x + 9x^2");
    q = Pow(q, 4);
    EXPECT_EQ(q.GetPolyRepresentation(), "1 + 24x + 252x^2 + 1512x^3 + 5670x^4 + 13608x^5 + 20412x^6 + 17496x^7 + 6561x^8");

    q = 1 + 3 * Pow(x(), 2) - x() + 8;
    EXPECT_EQ(q.GetPolyRepresentation(), "9 - x + 3x^2");

    q = 1 + x(2) + x(3) + x(4);
    EXPECT_EQ(q.GetPolyRepresentation(), "1 + w + z + y");
    auto w = Pow(q, 2);
    EXPECT_EQ(w.GetPolyRepresentation(), "1 + 2w + w^2 + 2z + 2zw + z^2 + 2y + 2yw + 2yz + y^2");
}

TEST(BaseTests, Substitute) {
    Polynomial<int> p = x();
    for (int i = 10; i <= 100; i += 10) {
        EXPECT_EQ(p(i).GetConst(), i);
    }
    Polynomial<int> q = 1 + 2 * x() + Pow(x(), 2);
    for (int i = -10; i <= 10; i++) {
        EXPECT_EQ(q(i).GetConst(), 1 + 2 * i + i*i);
    }

    q = q(q);
    EXPECT_EQ(q.GetPolyRepresentation(), "4 + 8x + 8x^2 + 4x^3 + x^4");

    Polynomial<int> a = 1 + x() * x(2) + x(2) * x(3) + x(3);
    EXPECT_EQ(a.GetPolyRepresentation(), "1 + z + yz + xy");

    EXPECT_EQ(a(1)(1, 1)(1, 2).GetConst(), 4);
    EXPECT_EQ(a(1)(13, 1)(53, 2).GetConst(), 756);

    EXPECT_EQ(a(1 + x(), 0).GetPolyRepresentation(), "1 + z + y + yz + xy");
    EXPECT_EQ(a(1 + x(), 1).GetPolyRepresentation(), "1 + 2z + x + xz + x^2");
    EXPECT_EQ(a(1 + x(), 2).GetPolyRepresentation(), "2 + y + x + 2xy");
    EXPECT_THROW((a(1 + x(), 3)), std::out_of_range);
}

TEST(SomeTests, TrashClean) {
    Polynomial<int> p = 0 * x() + 1 + 0 * x(2);
    EXPECT_EQ(p.GetPolyRepresentation(), "1 + 0y + 0x");
    p.TrashClean();
    EXPECT_EQ(p.GetPolyRepresentation(), "1");
    Polynomial<int> q = 1 + 0 * Pow(x(), 3) + Pow(x(3), 4) + Pow(x(4), 5);
    EXPECT_EQ(q.GetPolyRepresentation(), "1 + w^5 + z^4 + 0x^3");
    q.TrashClean();
    EXPECT_EQ(q.GetPolyRepresentation(), "1 + w^5 + z^4");
}

TEST(MatrixTests, First) {
    Polynomial<int> p = 1 + x() + Pow(x(), 2);
    Matrix<int> a({{1, 1}, {1, 1}});
    Matrix<int> res_a({{4, 3}, {3, 4}});
    EXPECT_EQ(p(a).GetMatrix(), res_a.GetMatrix());

    Polynomial<int> q = 5 + 10 * x() + Pow(x(), 3);
    Matrix<int> b({{1, 1, 1}, {1, 1, 1}, {2, 2, 2}});
    Matrix<int> res_b({{31, 26, 26}, {26, 31, 26}, {52, 52, 57}});
    EXPECT_EQ(q(b).GetMatrix(), res_b.GetMatrix());
}