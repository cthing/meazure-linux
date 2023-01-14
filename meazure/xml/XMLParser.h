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

#include <utility>
#include <xercesc/sax/AttributeList.hpp>
#include <xercesc/sax/InputSource.hpp>
#include <xercesc/sax/DocumentHandler.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/EntityResolver.hpp>
#include <xercesc/parsers/SAXParser.hpp>
#include <QString>
#include <QStringLiteral>
#include <map>
#include <list>
#include <stack>
#include <iostream>
#include <exception>
#include <functional>


class XMLParser;


/// Exception thrown when a syntax or validation error occurs during XML parsing.
///
class XMLParsingException : std::exception {

public:
    XMLParsingException(QString message, QString  pathname, int line, int column) :
            m_message(std::move(message)),
            m_pathname(std::move(pathname)),
            m_line(line),
            m_column(column) {
    }

    [[nodiscard]] const QString& getMessage() const {
        return m_message;
    }

    [[nodiscard]] const QString& getPathname() const {
        return m_pathname;
    }

    [[nodiscard]] int getLine() const {
        return m_line;
    }

    [[nodiscard]] int getColumn() const {
        return m_column;
    }

private:
    QString m_message;
    QString m_pathname;
    int m_line;
    int m_column;
};


/// The class contains the attributes associated with an XML start element. In addition to iterating through
/// the attributes, the class provides searching and other attribute manipulation capabilities.
///
class XMLAttributes {

    friend XMLParser;

public:
    /// Constructs an empty instance of the XML attributes class.
    ///
    XMLAttributes() = default;

    XMLAttributes(const XMLAttributes& attrs) = default;
    XMLAttributes(XMLAttributes&& attrs) = default;

    ~XMLAttributes() = default;

    XMLAttributes& operator=(const XMLAttributes& attrs) = default;
    XMLAttributes& operator=(XMLAttributes&&) = default;

    /// Indicates whether there are any attributes present.
    ///
    /// @return true if there are no attributes.
    ///
    [[nodiscard]] bool isEmpty() const { return m_attributeMap.empty(); }

    /// Returns the value of the specified attribute as a string.
    ///
    /// @param[in] name Attribute name.
    /// @param[out] value Attribute value as a string.
    /// @return true if the attribute is found.
    ///
    bool getValueStr(const QString& name, QString& value) const;

    /// Returns the value of the specified attribute as a string.
    ///
    /// @param[in] name Attribute name.
    /// @param[in] valueFunc Function that will be called with the value of the attribute.
    /// @return true if the attribute is found.
    ///
    bool getValueStr(const QString& name, const std::function<void (const QString&)>& valueFunc) const;

    /// Returns the value of the specified attribute converted to an integer.
    ///
    /// @param[in] name Attribute name.
    /// @param[out] value Attribute value as an integer.
    /// @return true if the attribute is found.
    ///
    bool getValueInt(const QString& name, int& value) const;

    /// Returns the value of the specified attribute converted to an integer.
    ///
    /// @param[in] name Attribute name.
    /// @param[in] valueFunc Function that will be called with the value of the attribute.
    /// @return true if the attribute is found.
    ///
    bool getValueInt(const QString& name, const std::function<void (int)>& valueFunc) const;

    /// Returns the value of the specified attribute converted to an unsigned integer.
    ///
    /// @param[in] name Attribute name.
    /// @param[out] value Attribute value as an unsigned integer.
    /// @return true if the attribute is found.
    ///
    bool getValueUInt(const QString& name, unsigned int& value) const;

    /// Returns the value of the specified attribute converted to an unsigned integer.
    ///
    /// @param[in] name Attribute name.
    /// @param[in] valueFunc Function that will be called with the value of the attribute.
    /// @return true if the attribute is found.
    ///
    bool getValueUInt(const QString& name, const std::function<void (unsigned int)>& valueFunc) const;

    /// Returns the value of the specified attribute converted to a double.
    ///
    /// @param[in] name Attribute name.
    /// @param[out] value Attribute value as a double.
    /// @return true if the attribute is found.
    ///
    bool getValueDbl(const QString& name, double& value) const;

    /// Returns the value of the specified attribute converted to a double.
    ///
    /// @param[in] name Attribute name.
    /// @param[in] valueFunc Function that will be called with the value of the attribute.
    /// @return true if the attribute is found.
    ///
    bool getValueDbl(const QString& name, const std::function<void (double)>& valueFunc) const;

    /// Returns the value of the specified attribute converted to a boolean.
    ///
    /// @param[in] name Attribute name.
    /// @param[out] value Attribute value as a boolean. This parameter is set to true if the attribute contains the
    ///     string "true" or "1". false is returned otherwise.
    /// @return true if the attribute is found.
    ///
    bool getValueBool(const QString& name, bool& value) const;

    /// Returns the value of the specified attribute converted to a boolean.
    ///
    /// @param[in] name Attribute name.
    /// @param[in] valueFunc Function that will be called with the boolean value of the attribute. This value is true
    ///     if the attribute contains the string "true" or "1". false is returned otherwise.
    /// @return true if the attribute is found.
    ///
    bool getValueBool(const QString& name, const std::function<void (bool)>& valueFunc) const;

private:
    using AttributeMap = std::map<QString, QString>;

    /// Constructs an instance of the class based on the specified attributes provided by the Xerces parser.
    ///
    /// @param[in] atts Attributes from Xerces.
    ///
    explicit XMLAttributes(const xercesc::AttributeList& atts);

    AttributeMap m_attributeMap;
};


/// A node in the XML DOM. The XMLParser class can build a DOM from the parsed file. This is a very minimal DOM
/// and does not conform to the W3C DOM spec.
///
class XMLNode {

public:
    using NodeList = std::list<XMLNode*>;                ///< Represents a list of DOM nodes.
    using NodeList_c = std::list<const XMLNode*>;        ///< Represents a list of DOM nodes.
    using NodeIter_c = NodeList::const_iterator;         ///< Constant iterator over the DOM nodes.

    /// Indicates the type of the DOM node.
    ///
    enum class Type {
        Unknown,        ///< Initial type for a DOM node.
        Element,        ///< The node represents an XML element.
        Data            ///< The node represents data contained between XML elements.
    };


    /// Constructs an empty DOM node of unknown type.
    ///
    XMLNode();

    /// Constructs a DOM node representing the specified element.
    ///
    /// @param[in] elementName The node represents this element.
    /// @param[in] attrs Attributes associated with the element.
    ///
    XMLNode(QString elementName, XMLAttributes  attrs);

    /// Constructs a DOM node representing the specified data.
    ///
    /// @param[in] data Data for the node.
    ///
    explicit XMLNode(QString data);

    XMLNode(const XMLNode& node) = default;
    XMLNode(XMLNode&&) = default;

    virtual ~XMLNode();

    XMLNode& operator=(const XMLNode& node) = default;
    XMLNode& operator=(XMLNode&&) = default;

    /// Returns the type of the node.
    ///
    /// @return Node type.
    ///
    [[nodiscard]] Type getType() const { return m_type; }

    /// Returns the data for the node. The data returned depends on the node type:
    ///
    /// <table cellspacing="0" cellpadding="3">
    /// <tr><th>Node Type</th><th>GetData() Return</th></tr>
    /// <tr><td>Unknown</td><td>""</td></tr>
    /// <tr><td>Element</td><td>Element name</td></tr>
    /// <tr><td>Data</td><td>Data</td></tr>
    /// </table>
    /// @return Data appropriate for the node.
    ///
    [[nodiscard]] const QString& getData() const { return m_data; }

    /// Concatenates all child data nodes into a single string.
    ///
    /// @return String concatenation of all child data nodes.
    ///
    [[nodiscard]] QString getChildData() const;

    /// Returns the attributes associated with the node if it is of type Element.
    ///
    /// @return Attributes associated with the node.
    ///
    [[nodiscard]] const XMLAttributes& getAttributes() const { return m_attributes; }

    /// Indicates whether there are any attributes associated with the node if it is of type Element.
    ///
    /// @return true if the node has attributes.
    ///
    [[nodiscard]] bool hasAttributes() const { return !m_attributes.isEmpty(); }

    /// Adds the specified DOM node as a child of this node.
    ///
    /// @param[in] child Node to add as a child.
    ///
    void addChild(XMLNode* child) {
        assert(child != nullptr);
        child->setParent(this);
        m_children.push_back(child);
    }

    /// Returns a constant iterator over the children of this node.
    ///
    /// @return Constant iterator over the children nodes.
    ///
    [[nodiscard]] NodeIter_c getChildIter() const { return m_children.begin(); }

    /// Indicates whether the specified iterator has reached the end of the list of children nodes.
    ///
    /// @param[in] iter Constant iter to test.
    /// @return true if the iterator has reached the end of the list of children nodes.
    ///
    [[nodiscard]] bool atEnd(const NodeIter_c& iter) const { return iter == m_children.end(); }

    /// Attempts to find the specified child element.
    ///
    /// @param[in] elementName Name of the element to find
    /// @return The first child element with the specified name or nullptr if not found.
    ///
    [[nodiscard]] const XMLNode* findChildElement(const QString& elementName) const {
        for (const XMLNode* node : m_children) {
            if (node->getType() == Type::Element && node->getData() == elementName) {
                return node;
            }
        }
        return nullptr;
    }

    /// Attempts to find the specified child elements.
    ///
    /// @param[in] elementName Name of the elements to find
    /// @return All child elements with the specified name or an empty list if not found.
    ///
    [[nodiscard]] NodeList_c findChildElements(const QString& elementName) const {
        std::list<const XMLNode*> nodes;
        for (const XMLNode* node : m_children) {
            if (node->getType() == Type::Element && node->getData() == elementName) {
                nodes.push_back(node);
            }
        }
        return nodes;
    }

    friend std::ostream& operator<<(std::ostream& os, const XMLNode& node);

private:
    /// Sets the specified node as the parent for this node. This allows moving up the node hierarchy to the root.
    ///
    /// @param[in] parent Node to set as the parent of this.
    ///
    void setParent(XMLNode* parent) { m_parent = parent; }


    Type m_type;                    ///< Type for the node.
    QString m_data;                 ///< Either empty, element name, or character data depending on the node type.
    XMLAttributes m_attributes;     ///< Attributes associated with an element node.
    NodeList m_children;            ///< Children of this node.
    XMLNode* m_parent;              ///< Parent of this node.
};

std::ostream& operator<<(std::ostream& os, const XMLNode::Type& type);


/// Mix-in class for XML parser callback methods. A class that wants XML parsing services inherits from this class,
/// overrides the methods of this class for the events of interest, creates an instance of the XMLParser class
/// and points it at the XML file to parse. The parser calls back using the methods of this class to indicate various
/// XML parsing events. This is classic SAX parsing behavior. The class has noop implementations for all methods.
///
struct XMLParserHandler {

    XMLParserHandler() = default;
    virtual ~XMLParserHandler() = default;

    XMLParserHandler(const XMLParserHandler&) = default;
    XMLParserHandler(XMLParserHandler&&) = default;

    XMLParserHandler& operator=(const XMLParserHandler&) = default;
    XMLParserHandler& operator=(XMLParserHandler&&) = default;

    /// Called when a new element is opened (e.g. &lt;foo&gt; or &lt;foo/&gt;).
    ///
    /// @param[in] container Parent element.
    /// @param[in] elementName Name of the element being opened.
    /// @param[in] attrs Attributes associated with the element.
    ///
    virtual void startElement(const QString& container, const QString& elementName, const XMLAttributes& attrs);

    /// Called when an element is closed (e.g. &lt;/foo&gt; or &lt;foo/&gt;).
    ///
    /// @param[in] container Parent element.
    /// @param[in] elementName Name of the element being closed.
    ///
    virtual void endElement(const QString& container, const QString& elementName);

    /// Called for all character data encountered between elements. Note that this method will be called for all
    /// character data including whitespace between consecutive elements. The DTD must be used to determine what
    /// whitespace is relevant.
    ///
    /// @param[in] container Name of the closest open element containing the character data.
    /// @param[in] data Character data.
    ///
    virtual void characterData(const QString& container, const QString& data);

    /// Called to resolve an external entity (e.g. DTD).
    ///
    /// @param[in] systemId System identifier of the external entity.
    /// @return Input source for the external entity. The default implementation returns nullptr.
    ///
    virtual xercesc::InputSource* resolveEntity(const QString& systemId);

    /// Returns the pathname of the currently parsed file.
    ///
    /// @return Pathname of the file currently being parsed. This class's implementation of this method returns
    ///         the empty string.
    ///
    virtual QString getFilePathname();
};


/// Provides SAX style XML parsing. This class is a wrapper around the Xerces parser including XML DTD validation.
/// A class that wants XML parsing services inherits from the XMLParserHandler class, overrides the methods of
/// that class for the events of interest, creates an instance of this class and points it at the XML file to parse.
/// The parser calls back using the methods of the XMLParserHandler class to indicate various XML parsing events.
/// This is classic SAX parsing behavior. The class can also build a primitive DOM.
///
class XMLParser : public xercesc::DocumentHandler, public xercesc::EntityResolver, public xercesc::ErrorHandler {

public:
    /// Constructs an XML parser that only builds a DOM.
    ///
    XMLParser();

    /// Constructs an XML parser.
    ///
    /// @param[in] handler Callback object for parsing events.
    /// @param[in] buildDOM Indicates whether a DOM should be built.
    ///
    explicit XMLParser(XMLParserHandler* handler, bool buildDOM = false);

    XMLParser(const XMLParser& other) = delete;
    XMLParser(XMLParser&&) = delete;

    ~XMLParser() override;

    XMLParser& operator=(const XMLParser&) = delete;


    /// Parses the specified XML file.
    ///
    /// @param[in] pathname XML file to parse
    ///
    void parseFile(const QString& pathname);

    /// Parses the specified XML content.
    ///
    /// @param[in] content String of XML content to parse
    ///
    void parseString(const QString& content);

    /// If a DOM was constructed, this method returns its root node.
    ///
    /// @return Root node of the DOM or nullptr if none was constructed.
    ///
    [[nodiscard]] const XMLNode* getDOM() const { return m_dom; }

private:
    using ElementStack = std::stack<QString>;       ///< A stack type for elements.
    using NodeStack = std::stack<XMLNode*>;         ///< A stack type for DOM nodes.
    using PathnameStack = std::stack<QString>;      ///< A stack type for entity pathnames.

    // DocumentHandler

    /// Receives notification of the beginning of a document. The parser will invoke this method only once, before
    /// any other methods in this interface.
    ///
    void startDocument() override;

    /// Receives an object for locating the origin of SAX document events. The locator allows the application to
    /// determine the end position of any document-related event, even if the parser is not reporting an error.
    /// Typically, the application will use this information for reporting its own errors (such as character content
    /// that does not match an application's business rules). Note that the locator will return correct information
    /// only during the invocation of the events in this interface. The application should not attempt to use it at
    /// any other time.
    ///
    /// @param[in] locator An object that can return the location of any SAX document event. The object is only
    ///     'on loan' to the client code and no attempt should me made to delete or modify it.
    ///
    void setDocumentLocator(const xercesc::Locator* locator) override;

    /// Receives notification of a processing instruction. The parser will invoke this method once for each processing
    /// instruction found. Note that the processing instructions may occur before or after the main document element.
    /// The parser will never report an XML declaration (XML 1.0, section 2.8) or a text declaration (XML 1.0,
    /// section 4.3.1) using this method.
    ///
    /// @param[in] target Processing instruction target
    /// @param[in] data processing instruction data, or null if none was supplied.
    ///
    void processingInstruction(const XMLCh* target, const XMLCh* data) override;

    /// Receives notification of the beginning of an element. The parser will invoke this method at the beginning of
    /// every element in the XML document. There will be a corresponding endElement() event for every startElement()
    /// event (even when the element is empty). All of the element's content will be reported, in order, before the
    /// corresponding endElement() event.
    ///
    /// @param[in] name Name of the element
    /// @param[in] attrs Element attributes. Note that the attribute list will contain only attributes with explicit
    ///     values (specified or defaulted). IMPLIED attributes will be omitted.
    ///
    void startElement(const XMLCh* name, xercesc::AttributeList& attrs) override;

    /// Receives notification of the end of an element. The parser will invoke this method at the end of every element
    /// in the XML document. There will be a corresponding startElement() event for every endElement() event (even
    /// when the element is empty).
    ///
    /// @param[in] name Name of the element
    ///
    void endElement(const XMLCh* name) override;

    /// Receives notification of character data. The parser will call this method to report each chunk of character
    /// data. SAX parsers may return all contiguous character data in a single chunk, or they may split it into
    /// several chunks. However, all of the characters in any single event must come from the same external entity.
    /// Note that the parser map report whitespace using the ignorableWhitespace() method rather than this
    /// one.
    ///
    /// @param[in] chars Characters from the XML document
    /// @param[in] length Number of characters read
    ///
    void characters(const XMLCh* chars, XMLSize_t length) override;

    /// Receives notification of ignorable whitespace in element content. The parser uses this method to report each
    /// chunk of ignorable whitespace (see the W3C XML 1.0 recommendation, section 2.10). The parser may return all
    /// contiguous whitespace in a single chunk, or may split it into several chunks. However, all of the characters
    /// in any single event will come from the same external entity.
    ///
    /// @param[in] chars Characters from the XML document
    /// @param[in] length Number of characters read
    ///
    void ignorableWhitespace(const XMLCh* chars, XMLSize_t length) override;

    /// Receives notification of the end of a document. The parser will invoke this method only once, and it will
    /// be the last method invoked during the parse. The parser will not invoke this method until it has either
    /// abandoned parsing (because of an unrecoverable error) or reached the end of input.
    ///
    void endDocument() override;

    /// Resets the document object for reuse. This method helps in reseting the document implementation defaults
    /// each time the document is begun.
    ///
    void resetDocument() override;

    // EntityResolver

    /// Allows the application to resolve external entities. The parser will call this method before opening any
    /// external entity except the top-level document entity (including the external DTD subset, external entities
    /// referenced within the DTD, and external entities referenced within the document element). The application may
    /// request that the parser resolve the entity itself, that it use an alternative URI, or that it use an entirely
    /// different input source. If the system identifier is a URL, the SAX parser will resolve it fully before
    /// reporting it to the application.
    ///
    /// @param[in] publicId Public identifier of the external entity being referenced, or null if none was supplied
    /// @param[in] systemId System identifier of the external entity being referenced.
    /// @return An InputSource object describing the new input source, or null to request that the parser open a
    ///     regular URI connection to the system identifier. The returned InputSource is owned by the parser which
    ///     is responsible to clean up the memory.
    ///
    xercesc::InputSource* resolveEntity(const XMLCh* publicId, const XMLCh* systemId) override;

    // ErrorHandler

    /// Receives notification of a warning. The parser will use this method to report conditions that are not errors
    /// or fatal errors as defined by the XML 1.0 recommendation.
    ///
    /// @param[in] exc Warning information encapsulated in a SAX parse exception.
    ///
    void warning(const xercesc::SAXParseException& exc) override;

    /// Receives notification of a recoverable error. This corresponds to the definition of "error" in section 1.2
    /// of the W3C XML 1.0 Recommendation. For example, the parser uses this callback to report the violation of a
    /// validity constraint.
    ///
    /// @param[in] exc Error information encapsulated in a SAX parse exception.
    ///
    void error(const xercesc::SAXParseException& exc) override;

    /// Receives notification of a non-recoverable error. This corresponds to the definition of "fatal error" in
    /// section 1.2 of the W3C XML 1.0 Recommendation. For example, the parser uses this callback to report the
    /// violation of a well-formedness constraint.
    ///
    /// @param[in] exc Error information encapsulated in a SAX parse exception.
    ///
    void fatalError(const xercesc::SAXParseException& exc) override;

    /// Reset the error handler object for reuse. This method helps in resetting the error handler object
    /// implementation defaults each time the parsing is begun.
    ///
    void resetErrors() override;


    static XMLParserHandler m_noopHandler;  ///< Do nothing handler when only building a DOM

    XMLParserHandler* m_handler;            ///< XML event callback object.
    bool m_buildDOM;                        ///< Indicates whether a DOM should be built.
    xercesc::SAXParser* m_parser;           ///< Xerces XML parser.
    ElementStack m_elementStack;            ///< Stack of open elements.
    NodeStack m_nodeStack;                  ///< Stack of XML DOM nodes.
    PathnameStack m_pathnameStack;          ///< Stack of pathnames for the entities being parsed.
    XMLNode* m_dom;                         ///< Root node of the DOM being built, or nullptr.
};
