#pragma once

#include <ostream>

#include "../core/dimension.hpp"

namespace console
{
    class console_view;

    class console
    {
    protected:
        std::ostream& output_stream;

        [[nodiscard]] virtual std::string move_to(int x, int y) const;

    public:
        virtual ~console() = default;

        explicit console(std::ostream& output_stream);

        void clear() const;

        void set_cursor_display(bool display = true) const;

        void write_at(int x, int y, const std::string& text) const;

        void fill_rectangle(const core::rectangle& rectangle, const std::string& character,
                            const std::string& line_prefix = "") const;

        [[nodiscard]] virtual std::shared_ptr<console_view> create_view(const core::offset& offset) const;
    };

    class console_view final : public console
    {
        core::position position;

        [[nodiscard]] std::string move_to(int x, int y) const override;

    public:
        console_view(const console& base_console, const core::position& position);

        [[nodiscard]] std::shared_ptr<console_view> create_view(const core::offset& offset) const override;
    };
}
