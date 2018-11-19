#ifndef SIMPLE_TEXT_ELEMENT_H
#define SIMPLE_TEXT_ELEMENT_H

#include <string>

#include <core/types/id_type.h>
#include <elements/element.h>


class SimpleTextElement : public Element
{
  public:
    static const std::string NAME;

  public:

    inline SimpleTextElement(void) {}

    SimpleTextElement(const core::UID& id, const std::string& text = "");
    virtual ~SimpleTextElement() override {}

    /**
     * @brief sets the text associated to the element
     * @param text the text to set
     */
    inline void
    setText(const std::string& text);

    /**
     * @brief returns the associated text
     * @return the associated text
     */
    inline const std::string&
    text(void) const;

    /**
     * @brief operator ==
     * @param other
     * @return
     */
    inline bool
    operator==(const SimpleTextElement& other) const;

    /**
     * @brief Returns the indexing text associated to this element if any
     * @return
     */
    virtual std::string
    getIndexingTest(void) const override;

    virtual std::string
    elementType(void) const override;

    virtual bool
    serialize(std::ostream& stream) const override;

    virtual bool
    deserialize(std::istream& stream) override;

    /**
     * @brief Will fill in this element with the information from the json value
     * @param json_value the json value
     * @return true on success | false otherwise
     */
    virtual bool
    loadFromJsonValue(rapidjson::Value& json_value) override;

    /**
     * @brief Creates a json value for this element information, associating it to the
     *        provided document
     * @param d the document
     * @return the json value generated
     */
    virtual rapidjson::Value
    toJsonValue(rapidjson::Document& d) const override;


  private:
    std::string text_;
};








//////////////////////////////////////////////////////////////////////////////////////////
// Inline text
//

inline void
SimpleTextElement::setText(const std::string& text)
{
  text_ = text;
}

inline const std::string&
SimpleTextElement::text(void) const
{
  return text_;
}

inline bool
SimpleTextElement::operator==(const SimpleTextElement& other) const
{
  return Element::operator==(other) && text_ == other.text_;
}


#endif // SIMPLE_TEXT_ELEMENT_H
