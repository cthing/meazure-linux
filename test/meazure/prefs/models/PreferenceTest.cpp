/*
 * Copyright 2023 C Thing Software
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <QTest>
#include <QtPlugin>
#include <QSignalSpy>
#include <meazure/prefs/models/Preference.h>
#include <vector>

Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)


class PreferenceTest : public QObject {

Q_OBJECT

private slots:
    [[maybe_unused]] void testConstruction();
    [[maybe_unused]] void testInitialize();
    [[maybe_unused]] void testSetValue();
    [[maybe_unused]] void testRestore();
    [[maybe_unused]] void testCollection();
    [[maybe_unused]] void testSignalOnInitialize();
    [[maybe_unused]] void testSignalOnSetValue();
    [[maybe_unused]] void testSignalOnRestore();
    [[maybe_unused]] void testNoSignalOnSetValue();
    [[maybe_unused]] void testNoSignalOnRestore();
    [[maybe_unused]] void testSignalOnDirty();
};


[[maybe_unused]] void PreferenceTest::testConstruction() {
    const Preference<int> pref(10);
    QCOMPARE(pref.getValue(), 10);
    QCOMPARE(pref, 10);
    QCOMPARE(pref.getDefaultValue(), 10);
    QVERIFY(!pref.isDirty());
}

[[maybe_unused]] void PreferenceTest::testInitialize() {
    Preference<int> pref(10);
    pref.initialize(12);
    QVERIFY(!pref.isDirty());
    QCOMPARE(pref.getValue(), 12);
    QCOMPARE(pref, 12);

    pref.setValue(17);
    QVERIFY(pref.isDirty());
    pref.initialize(15);
    QVERIFY(!pref.isDirty());
}

[[maybe_unused]] void PreferenceTest::testSetValue() {
    Preference<int> pref1(10);
    pref1.setValue(17);
    QCOMPARE(pref1.getValue(), 17);
    QCOMPARE(pref1, 17);
    QVERIFY(pref1.isDirty());

    Preference<int> pref2(10);
    pref2.initialize(12);
    QVERIFY(!pref2.isDirty());
    pref2.setValue(12);
    QCOMPARE(pref2.getValue(), 12);
    QCOMPARE(pref2, 12);
    QVERIFY(!pref2.isDirty());
}

[[maybe_unused]] void PreferenceTest::testRestore() {
    Preference<int> pref1(10);
    pref1.initialize(12);
    QVERIFY(!pref1.isDirty());
    pref1.restore();
    QCOMPARE(pref1.getValue(), 10);
    QCOMPARE(pref1, 10);
    QVERIFY(pref1.isDirty());

    Preference<int> pref2(10);
    pref2.initialize(10);
    QVERIFY(!pref2.isDirty());
    pref2.restore();
    QCOMPARE(pref2.getValue(), 10);
    QCOMPARE(pref2, 10);
    QVERIFY(!pref2.isDirty());
}

[[maybe_unused]] void PreferenceTest::testCollection() {
    const std::vector<int> defaultNumbers { 1, 2, 3, 4 ,5 };
    Preference<std::vector<int>> pref(defaultNumbers);
    QCOMPARE(pref.getValue(), defaultNumbers);
    QCOMPARE(pref.getDefaultValue(), defaultNumbers);
    QVERIFY(!pref.isDirty());

    const std::vector<int> initNumbers { 17, 16, 15 };
    pref.initialize(initNumbers);
    QVERIFY(!pref.isDirty());
    QCOMPARE(pref.getValue(), initNumbers);

    const std::vector<int> numbers { 21, 6, 5 };
    pref.setValue(numbers);
    QVERIFY(pref.isDirty());
    QCOMPARE(pref.getValue(), numbers);
}

[[maybe_unused]] void PreferenceTest::testSignalOnInitialize() {
    Preference<int> pref(10);

    const QSignalSpy preferenceSpy(&pref, SIGNAL(valueChanged(QVariant)));
    QVERIFY(preferenceSpy.isValid());

    pref.initialize(15);

    QCOMPARE(preferenceSpy.size(), 1);
    QList<QVariant> arguments = preferenceSpy[0];
    QCOMPARE(arguments.size(), 1);
    QCOMPARE(arguments[0].toInt(), 15);
}

[[maybe_unused]] void PreferenceTest::testSignalOnSetValue() {
    Preference<int> pref(10);

    const QSignalSpy preferenceSpy(&pref, SIGNAL(valueChanged(QVariant)));
    QVERIFY(preferenceSpy.isValid());

    pref.setValue(15);

    QCOMPARE(preferenceSpy.size(), 1);
    QList<QVariant> arguments = preferenceSpy[0];
    QCOMPARE(arguments.size(), 1);
    QCOMPARE(arguments[0].toInt(), 15);
}

[[maybe_unused]] void PreferenceTest::testSignalOnRestore() {
    Preference<int> pref(10);
    pref.initialize(12);

    const QSignalSpy preferenceSpy(&pref, SIGNAL(valueChanged(QVariant)));
    QVERIFY(preferenceSpy.isValid());

    pref.restore();

    QCOMPARE(preferenceSpy.size(), 1);
    QList<QVariant> arguments = preferenceSpy[0];
    QCOMPARE(arguments.size(), 1);
    QCOMPARE(arguments[0].toInt(), 10);
}

[[maybe_unused]] void PreferenceTest::testNoSignalOnSetValue() {
    Preference<int> pref(10);
    pref.initialize(12);

    const QSignalSpy preferenceSpy(&pref, SIGNAL(valueChanged(QVariant)));
    QVERIFY(preferenceSpy.isValid());

    pref.setValue(12);

    QVERIFY(preferenceSpy.isEmpty());
}

[[maybe_unused]] void PreferenceTest::testNoSignalOnRestore() {
    Preference<int> pref(10);

    const QSignalSpy preferenceSpy(&pref, SIGNAL(valueChanged(QVariant)));
    QVERIFY(preferenceSpy.isValid());

    pref.restore();

    QVERIFY(preferenceSpy.isEmpty());
}

[[maybe_unused]] void PreferenceTest::testSignalOnDirty() {
    Preference<int> pref(10);

    QSignalSpy preferenceSpy(&pref, SIGNAL(dirtyChanged(bool)));
    QVERIFY(preferenceSpy.isValid());

    pref.initialize(12);

    QCOMPARE(preferenceSpy.size(), 1);
    QList<QVariant> arguments = preferenceSpy[0];
    QCOMPARE(arguments.size(), 1);
    QCOMPARE(arguments[0].toBool(), false);

    preferenceSpy.clear();

    pref.setValue(29);

    QCOMPARE(preferenceSpy.size(), 1);
    arguments = preferenceSpy[0];
    QCOMPARE(arguments.size(), 1);
    QCOMPARE(arguments[0].toBool(), true);

    preferenceSpy.clear();

    pref.clearDirty();

    QCOMPARE(preferenceSpy.size(), 1);
    arguments = preferenceSpy[0];
    QCOMPARE(arguments.size(), 1);
    QCOMPARE(arguments[0].toBool(), false);
}


QTEST_MAIN(PreferenceTest)

#include "PreferenceTest.moc"
