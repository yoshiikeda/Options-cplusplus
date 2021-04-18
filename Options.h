#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include <memory>
#include <string>
#include <vector>

class Option
{
private:
    static constexpr char DELIMITER{'-'};

private:
    Option(std::unique_ptr<const std::string>&&,
           std::unique_ptr<const std::string>&&) noexcept;

public:
    static std::unique_ptr<const Option> Create(const char[]) noexcept;
    static std::unique_ptr<const Option> Create(const std::string&&) noexcept;

public:
    const std::string& Key() const noexcept;
    const std::string& Value() const noexcept;

    bool Valid() const noexcept;

private:
    const std::unique_ptr<const std::string> _key{nullptr};
    const std::unique_ptr<const std::string> _value{nullptr};
};

class OptionCollection
{
public:
    void Parse(const int, const char**);
    void Parse(const std::vector<std::string>&&);

    std::size_t Count() const noexcept;
    const Option& operator [](const std::size_t) const;

private:
    std::vector<std::unique_ptr<const Option>> _options{};
};

#endif // __OPTIONS_H__
