/*
 * Copyright 2022 C Thing Software
 *
 * This file is part of Meazure.
 *
 * Meazure is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Meazure is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with Meazure.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QTest>
#include <QtPlugin>
#include <meazure/utils/EnumIterator.h>
#include <vector>
#include <algorithm>

Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)

class EnumIteratorTest : public QObject {

    Q_OBJECT

private slots:
    [[maybe_unused]] void testIterationUnassinged();
    [[maybe_unused]] void testIterationAssinged();
    [[maybe_unused]] void testDereference();
    [[maybe_unused]] void testIncrement();
    [[maybe_unused]] void testInitialValue();
    [[maybe_unused]] void testEquality();
    [[maybe_unused]] void testCount();
    [[maybe_unused]] void testAlgorithms();
};


[[maybe_unused]] void EnumIteratorTest::testIterationUnassinged() {
    enum TestEnum {
        Value1,
        Value2,
        Value3,
        Value17,
        Value18
    };

    using TestEnumIter = EnumIterator<TestEnum, Value1, Value18>;

    std::vector<int> results;
    for (const TestEnum v : TestEnumIter()) {
        results.push_back(v);
    }

    const std::vector<int> expected { 0, 1, 2, 3, 4 };
    QCOMPARE(results, expected);
}

[[maybe_unused]] void EnumIteratorTest::testIterationAssinged() {
    enum TestEnum {
        Value1 = 18,
        Value2 = 19,
        Value3 = 20,
        Value17 = 21,
        Value18 = 22
    };

    using TestEnumIter = EnumIterator<TestEnum, Value1, Value18>;

    std::vector<int> results;
    for (const TestEnum v : TestEnumIter()) {
        results.push_back(v);
    }

    const std::vector<int> expected { 18, 19, 20, 21, 22 };
    QCOMPARE(results, expected);
}

[[maybe_unused]] void EnumIteratorTest::testDereference() {
    enum TestEnum {
        Value1 = 18,
        Value2 = 19,
        Value3 = 20,
        Value17 = 21,
        Value18 = 22
    };

    using TestEnumIter = EnumIterator<TestEnum, Value1, Value18>;

    TestEnumIter iter;
    QCOMPARE(*iter, 18);
}

[[maybe_unused]] void EnumIteratorTest::testIncrement() {
    enum TestEnum {
        Value1 = 18,
        Value2 = 19,
        Value3 = 20,
        Value17 = 21,
        Value18 = 22
    };

    using TestEnumIter = EnumIterator<TestEnum, Value1, Value18>;

    TestEnumIter iter;
    ++iter;
    QCOMPARE(*iter, 19);
    iter++;
    QCOMPARE(*iter, 20);
}

[[maybe_unused]] void EnumIteratorTest::testInitialValue() {
    enum TestEnum {
        Value1 = 18,
        Value2 = 19,
        Value3 = 20,
        Value17 = 21,
        Value18 = 22
    };

    using TestEnumIter = EnumIterator<TestEnum, Value1, Value18>;

    TestEnumIter iter(Value3);
    QCOMPARE(*iter, 20);
    iter++;
    QCOMPARE(*iter, 21);
}

[[maybe_unused]] void EnumIteratorTest::testEquality() {
    enum TestEnum {
        Value1 = 18,
        Value2 = 19,
        Value3 = 20,
        Value17 = 21,
        Value18 = 22
    };

    using TestEnumIter = EnumIterator<TestEnum, Value1, Value18>;

    TestEnumIter iter1;
    TestEnumIter iter2;
    const TestEnumIter iter3;

    ++iter1;
    ++iter2;
    QVERIFY(iter1 == iter2);
    QVERIFY(iter1 <= iter2);
    QVERIFY(iter1 >= iter2);
    QVERIFY(iter1 != iter3);
    QVERIFY(iter3 < iter1);
    QVERIFY(iter3 <= iter1);
    QVERIFY(iter1 > iter3);
    QVERIFY(iter1 >= iter3);
}

[[maybe_unused]] void EnumIteratorTest::testCount() {
    enum TestEnum {
        Value1,
        Value2,
        Value3,
        Value17,
        Value18
    };

    using TestEnumIter = EnumIterator<TestEnum, Value1, Value18>;

    TestEnumIter iter;
    QCOMPARE(iter.count(), 5);
}

[[maybe_unused]] void EnumIteratorTest::testAlgorithms() {
    enum TestEnum {
        Value1 = 18,
        Value2 = 19,
        Value3 = 20,
        Value17 = 21,
        Value18 = 22
    };

    using TestEnumIter = EnumIterator<TestEnum, Value1, Value18>;

    TestEnumIter iter1;
    const bool found = std::any_of(iter1.begin(), iter1.end(), [](TestEnum v) {
        return v == Value3;
    });
    QVERIFY(found);

    TestEnumIter iter2;
    EnumIterator iter = std::find_if(iter2.begin(), iter2.end(), [](TestEnum v) {
        return v == Value17;
    });
    QVERIFY(iter != iter.end());
    QCOMPARE(*iter, Value17);
}


QTEST_MAIN(EnumIteratorTest)

#include "EnumIteratorTest.moc"
