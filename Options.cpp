#include <memory>
#include <string>
#include <vector>

#include "Options.h"

Option::Option(std::unique_ptr<const std::string>&& key_,
               std::unique_ptr<const std::string>&& value_) noexcept : _key(std::move(key_)),
                                                                       _value(std::move(value_))
{
    // Empty
}

std::unique_ptr<const Option> Option::Create(const char arg_[]) noexcept
{
    return Create(std::string(arg_));
}

std::unique_ptr<const Option> Option::Create(const std::string&& arg_) noexcept
{
    auto result{std::unique_ptr<const Option>{nullptr}};

    if (!arg_.empty())
    {
        auto key{new std::string{}};
        auto value{new std::string{}};

        auto valid{true};

        std::size_t k{0};

        valid = (arg_[k++] == DELIMITER);

        if (k < arg_.size() && valid)
        {
            if (arg_[k] == DELIMITER)
            {
                ++k;
            }
            else
            {
                valid = (arg_.size() == 2);
            }

            while (k < arg_.size() && valid)
            {
                valid = (arg_[k] != DELIMITER);

                if (valid)
                {
                    *key += arg_[k++];
                }
            }

            if (valid)
            {
                result = std::unique_ptr<const Option>(new Option(std::unique_ptr<const std::string>(key),
                                                                  std::unique_ptr<const std::string>(value)));
            }
        }
    }

    return result;
}

const std::string& Option::Key() const noexcept
{
    return *_key;
}

const std::string& Option::Value() const noexcept
{
    return *_value;
}

bool Option::Valid() const noexcept
{
    return !(_key->empty());
}

void OptionCollection::Parse(const int argc_, const char** argv_)
{
    std::vector<std::string> args{};

    for (std::size_t k{1}; k < static_cast<std::size_t>(argc_); ++k)
    {
        args.push_back(std::string(argv_[k]));
    }

    Parse(std::move(args));
}

void OptionCollection::Parse(const std::vector<std::string>&& args_)
{
    bool valid = true;

    for (auto&& k{args_.cbegin()}; k != args_.cend() && valid; ++k)
    {
        auto o{Option::Create(std::move(const_cast<std::string&>(*k)))};

        if (o != nullptr && o->Valid())
        {
            _options.push_back(std::move(o));
        }
        else
        {
            valid = false;
        }
    }

    if (!valid)
    {
        _options.clear();
    }
}

std::size_t OptionCollection::Count() const noexcept
{
    return _options.size();
}

const Option& OptionCollection::operator [](const std::size_t i_) const
{
    return *(_options[i_]);
}
