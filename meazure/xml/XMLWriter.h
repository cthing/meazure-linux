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

#pragma once

#include <QString>
#include <QChar>
#include <iostream>
#include <list>
#include <stack>
#include <memory>
#include <utility>


/// This class writes pretty printed XML in UTF-8 encoding. Because this class serves only the needs of this
/// application, it does not support XML namespaces. Typical usage:</p>
/// <ol>
///      <li>Create an XMLWriter specifying the output destination.</li>
///      <li>Begin the XML output by calling the startDocument method.
///      <li>Call XML output methods as needed (e.g. startElement).</li>
///      <li>Complete the XML output by calling the endDocument method to properly terminate the XML document.
///          No XML output methods can be called following the call to endDocument.</li>
///      <li>The XmlWriter instance cannot be reused.</li>
/// </ol>
/// <p>The following is an example of using XMLWriter to write a simple XML document to the standard output:</p>
/// <pre>
/// XMLWriter writer(std::cout);
///
/// writer.startDocument();
/// writer.startElement("elem1");
/// writer.addAttribute("id", "1234");
/// writer.characters("Hello World");
/// writer.endElement();
/// writer.endDocument();
/// </pre>
/// <p>The following XML is displayed on the standard output:</p>
/// <pre>
/// &lt;?xml version="1.0" encoding="UTF-8"?&gt;
/// &lt;elem1 id="1234"&gt;Hello World&lt;/elem1&gt;
/// </pre>
///
class XMLWriter {

public:
    /// Creates an XML writer that writes to the specified writer.
    ///
    /// @param[in] out Output destination.
    ///
    explicit XMLWriter(std::ostream& out) : m_out(out) {
    }

    virtual ~XMLWriter() = default;

    XMLWriter(const XMLWriter&) = delete;
    XMLWriter(XMLWriter&&) = delete;
    XMLWriter& operator=(const XMLWriter&) = delete;

    /// Starts an XML document by writing the XML header. This method writes a header with UTF-8 encoding.
    /// This method must be called before any other XMLWriter output method is called.
    ///
    /// @return This writer instance
    ///
    XMLWriter& startDocument();

    /// Ends the XML output. This method must be called to properly terminate the XML document. The XMLWriter cannot
    /// be reused. This method does not close the underlying output stream.
    ///
    /// @return This writer instance
    ///
    XMLWriter& endDocument();

    /// Writes a Document Type Declaration.
    ///
    /// @param[in] name Root element name
    /// @param[in] systemId System identified (must be specified).
    /// @return This writer instance
    ///
    XMLWriter& doctype(const QString& name, const QString& systemId);

    /// Starts a new element. Attributes for the element can be specified using the addAttributes. Each call to
    /// startElement requires a corresponding call to endElement.
    ///
    /// @param[in] name Name of the element
    /// @return This writer instance
    ///
    XMLWriter& startElement(const QString& name);

    /// Writes an end tag for the element that is open at the current level. Each call to startElement()
    /// requires a corresponding call to endElement.
    ///
    /// @return This writer instance
    ///
    XMLWriter& endElement();

    /// Adds an attribute to the current start tag.
    ///
    /// @param[in] name Attribute name
    /// @param[in] value Attribute value
    /// @return This writer instance
    ///
    XMLWriter& addAttribute(const QString& name, const QString& value);

    /// Adds an attribute to the current start tag.
    ///
    /// @param[in] name Attribute name
    /// @param[in] value Attribute value
    /// @return This writer instance
    ///
    XMLWriter& addAttribute(const QString& name, int value);

    /// Adds an attribute to the current start tag.
    ///
    /// @param[in] name Attribute name
    /// @param[in] value Attribute value
    /// @return This writer instance
    ///
    XMLWriter& addAttribute(const QString& name, unsigned int value);

    /// Adds an attribute to the current start tag.
    ///
    /// @param[in] name Attribute name
    /// @param[in] value Attribute value
    /// @return This writer instance
    ///
    XMLWriter& addAttribute(const QString& name, double value);

    /// Writes the specified string as escaped XML data. Line endings are normalized to LF so that the output
    /// stream can translate them to the platform specific ending.
    ///
    /// @param[in] str String to write
    /// @return This writer instance
    ///
    XMLWriter& characters(const QString& str);

    /// Flushes the output. This method is especially useful for ensuring that the entire document has been output
    /// without having to close the writer. This method is invoked automatically by the endDocument() method.
    ///
    void flush();

protected:
    /// Represents an XML element attribute.
    ///
    struct Attribute {
        Attribute(QString  name, QString  value) : m_name(std::move(name)), m_value(std::move(value)) {}

        QString m_name;
        QString m_value;
    };


    using AttributePtr = std::shared_ptr<Attribute>;
    using Attributes = std::list<AttributePtr>;


    /// Writes a start tag and handles the case where the element is empty.
    ///
    /// @param[in] isEmpty true to write start element as if it were empty
    ///
    virtual void writeStartElement(bool isEmpty);

    /// Writes an end tag.
    ///
    virtual void writeEndElement();

    /// Writes out an attribute list, quoting and escaping values. The attributes will be written all on one line.
    ///
    /// @param[in] attributes The attribute list to write.
    ///
    virtual void writeAttributes(const Attributes& attributes);

    /// Indents the output based on the element nesting level.
    ///
    virtual void writeIndent();

    /// Writes the specified string to the output as an escaped string surrounded by double quotes.
    ///
    /// @param[in] str String to quote, escape and write
    ///
    virtual void writeQuoted(const QString& str);

    /// Writes the specified string to the output escaping the XML special characters using the standard XML escape
    /// sequences. Control characters and characters outside the ASCII range are escaped using a numeric character
    /// reference. Invalid XML control characters are written as "ctrl-nnnn".
    ///
    /// @param[in] str String to escape and write
    ///
    virtual void writeEscaped(const QString& str);

    /// Writes the specified character to the output escaping the special characters using the standard XML escape
    /// sequences. Control characters and characters outside the ASCII range are escaped using a numeric character
    /// reference. Invalid XML control characters are written as "ctrl-nnnn".
    ///
    /// @param[in] ch Character to escape and write
    ///
    virtual void writeEscaped(const QChar& ch);

    /// Writes a newline character. The runtime is relied upon to translate the line feed character into the
    /// platform specific line ending appropriate to the output stream (e.g. CR+LF for ofstream on Windows).
    ///
    virtual void writeNewline();

    /// Writes the specified string to the output without escaping.
    ///
    /// @param[in] str String to write
    ///
    virtual void writeLiteral(const QString& str);

    /// Writes the specified literal string without any escaping or conversion.
    ///
    /// @param[in] str String to write
    ///
    virtual void writeLiteral(const char* str);

    /// Writes the specified literal character without any escaping or conversion.
    ///
    /// @param[in] ch Character to write
    ///
    virtual void writeLiteral(char ch);

private:
    /// Represents the state of the writer.
    ///
    enum class State {
        BeforeDoc,
        BeforeRoot,
        InStartTag,
        AfterTag,
        AfterData,
        AfterRoot,
        AfterDoc
    };


    /// Represents a writing state transition event.
    ///
    enum class Event {
        Attribute,
        Characters,
        EndDocument,
        EndElement,
        StartDocument,
        StartElement
    };


    /// Represents an XML element.
    ///
    struct Element {
        Element(QString  name, State state): m_name(std::move(name)), m_state(state) {}

        QString m_name;             ///< Name of the element
        Attributes m_attributes;    ///< Attributes for the element
        State m_state;              ///< Writer state in which this element is being written
    };


    using ElementPtr = std::shared_ptr<Element>;
    using ElementStack = std::stack<ElementPtr>;


    /// Heart of the XMLWriter state machine. Based on the current state and the specified event, an action is fired,
    /// if any, and the next state is set. If the event is illegal for the current state, an std::ios_base::failure is
    /// thrown.
    ///
    /// @param[in] event The event to handle
    /// @return Previous state
    /// @throws std::ios_base::failure If the event is illegal given the current state.
    ///
    State handleEvent(Event event);

    /// Obtains a string representation of the specified writing state for debugging purposes.
    ///
    /// @param[in] state Writing state whose string representation is to be returned
    /// @return String representation of the specified state.
    ///
    static QString getStateName(State state);

    /// Obtains a string representation of the specified writing event for debugging purposes.
    ///
    /// @param[in] event Writing event whose string representation is to be returned
    /// @return String representation of the specified event.
    ///
    static QString getEventName(Event event);


    static const char* indent;    ///< String for each level of indentation


    std::ostream& m_out;                        ///< Output stream to write the XML
    ElementStack m_elementStack;                ///< Stack of open elements
    State m_currentState { State::BeforeDoc };  ///< Current state of the writer state machine.
};
