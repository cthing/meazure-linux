/*
 * Copyright 2023 C Thing Software
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
#include <QTemporaryFile>
#include <QFileInfo>
#include <meazure/config/ExportedConfig.h>


Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)


class ExportedConfigTest : public QObject {

Q_OBJECT

private slots:
    [[maybe_unused]] void testReadWrite();
    [[maybe_unused]] void testIsPersistent();
    [[maybe_unused]] void testGetVersion();
};


[[maybe_unused]] void ExportedConfigTest::testReadWrite() {
    QTemporaryFile tempFile;
    tempFile.open();

    {
        ExportedConfig config(tempFile.fileName(), ExportedConfig::Write);
        config.writeBool("key1", true);
        config.writeInt("key2", -17);
        config.writeUInt("key3", 21);
        config.writeDbl("key4", 3.14159);
        config.writeStr("key5", "abcd");
    }

    QVERIFY(QFileInfo(tempFile).exists());

    {
        const ExportedConfig config(tempFile.fileName(), ExportedConfig::Read);
        QVERIFY(config.readBool("key1", false));
        QCOMPARE(config.readInt("key2", 10), -17);
        QCOMPARE(config.readInt("key3", 10), 21);
        QCOMPARE(config.readDbl("key4", 1.5), 3.14159);
        QCOMPARE(config.readStr("key5", "foo"), "abcd");

        QVERIFY(!config.readBool("key15", false));
        QCOMPARE(config.readInt("key16", 10), 10);
        QCOMPARE(config.readInt("key17", 11), 11);
        QCOMPARE(config.readDbl("key18", 1.5), 1.5);
        QCOMPARE(config.readStr("key18", "foo"), "foo");
    }
}

[[maybe_unused]] void ExportedConfigTest::testIsPersistent() {
    QTemporaryFile tempFile;
    tempFile.open();
    const ExportedConfig config(tempFile.fileName(), ExportedConfig::Write);
    QVERIFY(!config.isPersistent());
}

[[maybe_unused]] void ExportedConfigTest::testGetVersion() {
    QTemporaryFile tempFile;
    tempFile.open();
    const ExportedConfig config(tempFile.fileName(), ExportedConfig::Write);
    QCOMPARE(config.getVersion(), 2);
}


QTEST_GUILESS_MAIN(ExportedConfigTest)

#include "ExportedConfigTest.moc"
