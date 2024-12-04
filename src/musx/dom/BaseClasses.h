/*
 * Copyright (C) 2024, Robert Patterson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#pragma once

#include <cstdint>

namespace musx {
namespace dom {

using Cmper = uint16_t;     ///< Enigma "comperator" key type
using Inci = int16_t;       ///< Enigma "incidend" key type
using Evpu = int32_t;       ///< EVPU value (288 per inch)
using Efix = int32_t;       ///< EFIX value (64 per EVPU, 64*288=18432 per inch)
using Edu = int32_t;        ///< EDU value (1024 per quarter note)

class Document;
/** @brief Shared `Document` pointer */
using DocumentPtr = std::shared_ptr<Document>;
/** @brief Shared weak `Document` pointer */
using DocumentWeakPtr = std::weak_ptr<Document>;

/**
 * @brief Base class to enforce polymorphism across all DOM classes.
 * 
 * This class uses CRTP (Curiously Recurring Template Pattern) to ensure that all
 * derived classes define a static constexpr `XmlNodeName` of type `constexpr char[]`.
 * 
 * @tparam Derived The derived class inheriting from this base.
 */
class Base
{
public:
    /**
     * @brief Virtual destructor for polymorphic behavior.
     */
    virtual ~Base() = default;

    /**
     * @brief Gets the weak reference to the Document.
     * 
     * @return A weak pointer to the Document instance.
     */
    DocumentWeakPtr getDocument() const { return m_document; }

protected:
    /**
     * @brief Constructs the base class and enforces the static constexpr XmlNodeName.
     * 
     * @param document A weak pointer to the parent document
     */
    Base(const DocumentWeakPtr& document) : m_document(document) {}

private:
    const DocumentWeakPtr m_document;
};

/**
 * @brief Base class for all "options" types.
 * 
 * Options types derive from this base class so they can reside in the options pool.
 */
class OptionsBase : public Base
{
protected:
    /**
     * @brief Constructs the OptionsBase and validates XmlNodeName in the derived class.
     *
     * @param document A weak pointer to the parent document
     */
    OptionsBase(const DocumentWeakPtr& document) : Base(document) {}
};

/**
 * @brief Base class for all "others" types.
 * 
 * This class provides common attributes and methods for handling
 * "others" types in the DOM, including `cmper` and `inci`.
 */
class OthersBase : public Base
{
private:
    Cmper m_cmper; ///< Common attribute: cmper (key value).
    Inci m_inci;  ///< Array index: inci (starting from 0).

protected:
    /**
     * @brief Constructs an OthersBase object.
     * 
     * @param cmper The `cmper` key value.
     * @param inci The array index (`inci`).
     * @param document A weak pointer to the parent document
     */
    OthersBase(const DocumentWeakPtr& document, int cmper, int inci = 0)
        : Base(document), m_cmper(cmper), m_inci(inci) {}

public:
    /**
     * @brief Gets the `cmper` key value.
     * 
     * @return The `cmper` value.
     */
    Cmper getCmper() const { return m_cmper; }

    /**
     * @brief Sets the `cmper` key value.
     * 
     * @param cmper The new `cmper` value.
     */
    void setCmper(Cmper cmper) { m_cmper = cmper; }

    /**
     * @brief Gets the array index (`inci`).
     * 
     * @return The `inci` value.
     */
    Inci getInci() const { return m_inci; }

    /**
     * @brief Sets the array index (`inci`).
     * 
     * @param inci The new `inci` value.
     */
    void setInci(Inci inci) { m_inci = inci; }
};

} // namespace dom
} // namespace musx
