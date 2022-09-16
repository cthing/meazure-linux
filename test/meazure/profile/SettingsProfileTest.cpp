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
#include <QTemporaryFile>
#include <QFileInfo>
#include <meazure/profile/SettingsProfile.h>


Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)


class SettingsProfileTest : public QObject {

Q_OBJECT

private slots:
    [[maybe_unused]] void testReadWrite();
    [[maybe_unused]] void testUserInitiated();
    [[maybe_unused]] void testGetVersion();
};


void SettingsProfileTest::testReadWrite() { // NOLINT(readability-convert-member-functions-to-static)
    QTemporaryFile tempFile;
    tempFile.open();

    {
        SettingsProfile profile(tempFile.fileName());
        profile.writeBool("key1", true);
        profile.writeInt("key2", -17);
        profile.writeUInt("key3", 21);
        profile.writeDbl("key4", 3.14159);
        profile.writeStr("key5", "abcd");
    }

    QVERIFY(QFileInfo(tempFile).exists());

    {
        SettingsProfile profile(tempFile.fileName());
        QVERIFY(profile.readBool("key1", false));
        QCOMPARE(profile.readInt("key2", 10), -17);
        QCOMPARE(profile.readInt("key3", 10), 21);
        QCOMPARE(profile.readDbl("key4", 1.5), 3.14159);
        QCOMPARE(profile.readStr("key5", "foo"), "abcd");

        QVERIFY(!profile.readBool("key15", false));
        QCOMPARE(profile.readInt("key16", 10), 10);
        QCOMPARE(profile.readInt("key17", 11), 11);
        QCOMPARE(profile.readDbl("key18", 1.5), 1.5);
        QCOMPARE(profile.readStr("key18", "foo"), "foo");
    }
}

void SettingsProfileTest::testUserInitiated() { // NOLINT(readability-convert-member-functions-to-static)
    QTemporaryFile tempFile;
    tempFile.open();
    SettingsProfile profile(tempFile.fileName());
    QVERIFY(!profile.userInitiated());
}

void SettingsProfileTest::testGetVersion() { // NOLINT(readability-convert-member-functions-to-static)
    QTemporaryFile tempFile;
    tempFile.open();
    SettingsProfile profile(tempFile.fileName());
    QCOMPARE(profile.getVersion(), 1);
}


QTEST_MAIN(SettingsProfileTest)

#include "SettingsProfileTest.moc"
