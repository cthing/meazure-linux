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
#include <meazure/position-log/model/PosLogInfo.h>
#include <QDateTime>

Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)


class PosLogInfoTest : public QObject {

Q_OBJECT

private slots:
    [[maybe_unused]] void testConstruction();
    [[maybe_unused]] void testSetGet();
    [[maybe_unused]] void testAssignmentEquality();
};


[[maybe_unused]] void PosLogInfoTest::testConstruction() {
    const PosLogInfo info;
    QVERIFY(info.getTitle().isEmpty());
    QVERIFY(!info.getCreated().isValid());
    QVERIFY(info.getAppName().isEmpty());
    QVERIFY(info.getAppVersion().isEmpty());
    QVERIFY(info.getAppBuild().isEmpty());
    QVERIFY(info.getMachineName().isEmpty());
    QVERIFY(info.getDescription().isEmpty());
}

[[maybe_unused]] void PosLogInfoTest::testSetGet() {
    PosLogInfo info;

    info.setTitle("abc");
    QCOMPARE(info.getTitle(), "abc");

    const QDateTime dateTime = QDateTime::fromString("2023-01-09T18:10:37+0000", Qt::ISODate);
    info.setCreated(dateTime);
    QCOMPARE(info.getCreated(), dateTime);

    info.setAppName("defg");
    QCOMPARE(info.getAppName(), "defg");

    info.setAppVersion("1.2.3");
    QCOMPARE(info.getAppVersion(), "1.2.3");

    info.setAppBuild("23");
    QCOMPARE(info.getAppBuild(), "23");

    info.setMachineName("host17");
    QCOMPARE(info.getMachineName(), "host17");

    info.setDescription("lmnopq");
    QCOMPARE(info.getDescription(), "lmnopq");
}

[[maybe_unused]] void PosLogInfoTest::testAssignmentEquality() {
    PosLogInfo info1;
    PosLogInfo info2;

    info1.setTitle("abc");
    info1.setCreated(QDateTime::currentDateTime());
    info1.setAppName("def");
    info1.setAppVersion("1.2.3");
    info1.setAppBuild("34");
    info1.setMachineName("hjki");
    info1.setDescription("asdf");

    info2.setTitle("uydr");
    info2.setCreated(QDateTime::currentDateTime());
    info2.setAppName("gfd");
    info2.setAppVersion("3.4.3");
    info2.setAppBuild("2");
    info2.setMachineName("pkljh");
    info2.setDescription("yut");

    QVERIFY(info1 == info1);
    QVERIFY(info1 != info2);

    info2 = info1;
    QVERIFY(info1 == info2);
}


QTEST_MAIN(PosLogInfoTest)

#include "PosLogInfoTest.moc"
