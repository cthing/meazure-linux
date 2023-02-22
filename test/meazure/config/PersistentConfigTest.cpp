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
#include <QTemporaryFile>
#include <QFileInfo>
#include <meazure/config/PersistentConfig.h>


Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)


class PersistentConfigTest : public QObject {

Q_OBJECT

private slots:
    [[maybe_unused]] void testReadWrite();
    [[maybe_unused]] void testIsPersistent();
    [[maybe_unused]] void testGetVersion();
};


[[maybe_unused]] void PersistentConfigTest::testReadWrite() {
    QTemporaryFile tempFile;
    tempFile.open();

    {
        PersistentConfig config(tempFile.fileName());
        config.writeBool("key1", true);
        config.writeInt("key2", -17);
        config.writeUInt("key3", 21);
        config.writeDbl("key4", 3.14159);
        config.writeStr("key5", "abcd");
    }

    QVERIFY(QFileInfo(tempFile).exists());

    {
        const PersistentConfig config(tempFile.fileName());
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

[[maybe_unused]] void PersistentConfigTest::testIsPersistent() {
    QTemporaryFile tempFile;
    tempFile.open();
    const PersistentConfig config(tempFile.fileName());
    QVERIFY(config.isPersistent());
}

[[maybe_unused]] void PersistentConfigTest::testGetVersion() {
    QTemporaryFile tempFile;
    tempFile.open();
    const PersistentConfig config(tempFile.fileName());
    QCOMPARE(config.getVersion(), 3);
}


QTEST_MAIN(PersistentConfigTest)

#include "PersistentConfigTest.moc"
