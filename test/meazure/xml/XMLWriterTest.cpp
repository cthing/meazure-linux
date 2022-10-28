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
#include <sstream>
#include <memory>
#include <meazure/xml/XMLWriter.h>

Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)


struct TestXMLWriter : public XMLWriter {

    explicit TestXMLWriter(std::ostream& out) : XMLWriter(out) {}

    void writeQuoted(const QString& str) override { XMLWriter::writeQuoted(str); }

    void writeEscaped(const QString& str) override { XMLWriter::writeEscaped(str); };

    void writeEscaped(const QChar& ch) override { XMLWriter::writeEscaped(ch); };

    void writeNewline() override { XMLWriter::writeNewline(); }

    void writeLiteral(const QString& str) override { XMLWriter::writeLiteral(str); }

    void writeLiteral(const char* str) override { XMLWriter::writeLiteral(str); }

    void writeLiteral(char ch) override { XMLWriter::writeLiteral(ch); }
};


struct TestFixture {
    TestFixture() : writer(stream) {
        stream.exceptions(std::ios::failbit | std::ios::badbit);
    }

    void clear() { stream.str(""); }

    std::ostringstream stream;
    TestXMLWriter writer;
};


class XMLWriterTest : public QObject {

Q_OBJECT

private slots:
    [[maybe_unused]] void init();
    [[maybe_unused]] void testWriteLiteralChar();
    [[maybe_unused]] void testWriteLiteralString();
    [[maybe_unused]] void testWriteLiteralQString();
    [[maybe_unused]] void testWriteNewline();
    [[maybe_unused]] void testWriteEscapedChar();
    [[maybe_unused]] void testWriteEscapedString();
    [[maybe_unused]] void testWriteQuoted();
    [[maybe_unused]] void testStartEndDocument();
    [[maybe_unused]] void testElements();
    [[maybe_unused]] void testAttributes();
    [[maybe_unused]] void testDoctype();
    [[maybe_unused]] void testCharacters();

private:
    std::unique_ptr<TestFixture> m_fixture;
};


[[maybe_unused]] void XMLWriterTest::init()
{
    m_fixture.reset(new TestFixture());
}


[[maybe_unused]] void XMLWriterTest::testWriteLiteralChar() {
    m_fixture->clear();
    m_fixture->writer.writeLiteral('a');
    QCOMPARE(m_fixture->stream.str().data(), "a");

    m_fixture->clear();
    m_fixture->writer.writeLiteral(' ');
    QCOMPARE(m_fixture->stream.str().data(), " ");

    m_fixture->clear();
    m_fixture->writer.writeLiteral('<');
    QCOMPARE(m_fixture->stream.str().data(), "<");

    m_fixture->clear();
    m_fixture->writer.writeLiteral('>');
    QCOMPARE(m_fixture->stream.str().data(), ">");

    m_fixture->clear();
    m_fixture->writer.writeLiteral('\x99');
    QCOMPARE(m_fixture->stream.str().data(), "\x99");
}

[[maybe_unused]] void XMLWriterTest::testWriteLiteralString() {
    m_fixture->clear();
    m_fixture->writer.writeLiteral("a");
    QCOMPARE(m_fixture->stream.str().data(), "a");

    m_fixture->clear();
    m_fixture->writer.writeLiteral(" ");
    QCOMPARE(m_fixture->stream.str().data(), " ");

    m_fixture->clear();
    m_fixture->writer.writeLiteral("<");
    QCOMPARE(m_fixture->stream.str().data(), "<");

    m_fixture->clear();
    m_fixture->writer.writeLiteral(">");
    QCOMPARE(m_fixture->stream.str().data(), ">");

    m_fixture->clear();
    m_fixture->writer.writeLiteral("A bc<>&\"'");
    QCOMPARE(m_fixture->stream.str().data(), "A bc<>&\"'");

    m_fixture->clear();
    m_fixture->writer.writeLiteral("\u2122\u2026");
    QCOMPARE(m_fixture->stream.str().data(), "\u2122\u2026");
}

[[maybe_unused]] void XMLWriterTest::testWriteLiteralQString() {
    m_fixture->clear();
    m_fixture->writer.writeLiteral(QString("a"));
    QCOMPARE(m_fixture->stream.str().data(), "a");

    m_fixture->clear();
    m_fixture->writer.writeLiteral(QString(" "));
    QCOMPARE(m_fixture->stream.str().data(), " ");

    m_fixture->clear();
    m_fixture->writer.writeLiteral(QString("<"));
    QCOMPARE(m_fixture->stream.str().data(), "<");

    m_fixture->clear();
    m_fixture->writer.writeLiteral(QString(">"));
    QCOMPARE(m_fixture->stream.str().data(), ">");

    m_fixture->clear();
    m_fixture->writer.writeLiteral(QString("A bc<>&\"'"));
    QCOMPARE(m_fixture->stream.str().data(), "A bc<>&\"'");

    m_fixture->clear();
    m_fixture->writer.writeLiteral(QString("\u2122\u2026"));
    QCOMPARE(m_fixture->stream.str().data(), "\u2122\u2026");
}

[[maybe_unused]] void XMLWriterTest::testWriteNewline() {
    m_fixture->writer.writeNewline();
    QCOMPARE(m_fixture->stream.str().data(), "\n");
}

[[maybe_unused]] void XMLWriterTest::testWriteEscapedChar() {
    m_fixture->writer.writeEscaped('a');
    QCOMPARE(m_fixture->stream.str().data(), "a");

    m_fixture->clear();
    m_fixture->writer.writeEscaped('\n');
    QCOMPARE(m_fixture->stream.str().data(), "\n");

    m_fixture->clear();
    m_fixture->writer.writeEscaped('\t');
    QCOMPARE(m_fixture->stream.str().data(), "\t");

    m_fixture->clear();
    m_fixture->writer.writeEscaped('\r');
    QCOMPARE(m_fixture->stream.str().data(), "\r");

    m_fixture->clear();
    m_fixture->writer.writeEscaped('&');
    QCOMPARE(m_fixture->stream.str().data(), "&amp;");

    m_fixture->clear();
    m_fixture->writer.writeEscaped('<');
    QCOMPARE(m_fixture->stream.str().data(), "&lt;");

    m_fixture->clear();
    m_fixture->writer.writeEscaped('>');
    QCOMPARE(m_fixture->stream.str().data(), "&gt;");

    m_fixture->clear();
    m_fixture->writer.writeEscaped('"');
    QCOMPARE(m_fixture->stream.str().data(), "&quot;");

    m_fixture->clear();
    m_fixture->writer.writeEscaped('\'');
    QCOMPARE(m_fixture->stream.str().data(), "&apos;");

    m_fixture->clear();
    m_fixture->writer.writeEscaped('\x99');
    QCOMPARE(m_fixture->stream.str().data(), "&#153;");

    m_fixture->clear();
    m_fixture->writer.writeEscaped('\x1A');
    QCOMPARE(m_fixture->stream.str().data(), "ctrl-26");

    m_fixture->clear();
    m_fixture->writer.writeEscaped(u'\uE010');
    QCOMPARE(m_fixture->stream.str().data(), "&#57360;");
}

[[maybe_unused]] void XMLWriterTest::testWriteEscapedString() {
    m_fixture->writer.writeEscaped("a\n\t\r&<>\"'");
    QCOMPARE(m_fixture->stream.str().data(), "a\n\t\r&amp;&lt;&gt;&quot;&apos;");

    m_fixture->clear();
    m_fixture->writer.writeEscaped("\u2122\u2026");
    QCOMPARE(m_fixture->stream.str().data(), "&#8482;&#8230;");
}

[[maybe_unused]] void XMLWriterTest::testWriteQuoted() {
    m_fixture->writer.writeQuoted("abcd &efg");
    QCOMPARE(m_fixture->stream.str().data(), "\"abcd &amp;efg\"");
}

[[maybe_unused]] void XMLWriterTest::testStartEndDocument() {
    m_fixture->writer.startDocument().endDocument();
    QCOMPARE(m_fixture->stream.str().data(), "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n");
}

[[maybe_unused]] void XMLWriterTest::testElements() {
    m_fixture->writer.startDocument()
                   .startElement("elem1")
                   .startElement("elem2")
                   .endElement()
                   .endElement()
                   .endDocument();
    QCOMPARE(m_fixture->stream.str().data(), R"|(<?xml version="1.0" encoding="UTF-8"?>
<elem1>
    <elem2/>
</elem1>
)|");
}

[[maybe_unused]] void XMLWriterTest::testAttributes() {
    m_fixture->writer.startDocument()
                   .startElement("elem")
                   .addAttribute("attr1", "abc")
                   .addAttribute("attr2", 2)
                   .addAttribute("attr3", 3.5)
                   .endElement()
                   .endDocument();
    QCOMPARE(m_fixture->stream.str().data(), R"|(<?xml version="1.0" encoding="UTF-8"?>
<elem attr1="abc" attr2="2" attr3="3.5"/>
)|");
}

[[maybe_unused]] void XMLWriterTest::testDoctype() {
    m_fixture->writer.startDocument()
                   .doctype("elem", "http://www.cthing.com/dtd/PositionLog1.dtd")
                   .startElement("elem")
                   .endElement()
                   .endDocument();
    QCOMPARE(m_fixture->stream.str().data(), R"|(<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE elem SYSTEM "http://www.cthing.com/dtd/PositionLog1.dtd">
<elem/>
)|");
}

[[maybe_unused]] void XMLWriterTest::testCharacters() {
    m_fixture->writer.startDocument()
                   .startElement("elem")
                   .characters("Hello\n\nWorld")
                   .endElement()
                   .endDocument();
    QCOMPARE(m_fixture->stream.str().data(), "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<elem>Hello\n\nWorld</elem>\n");
}


QTEST_MAIN(XMLWriterTest)

#include "XMLWriterTest.moc"
