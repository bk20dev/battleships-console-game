#pragma once

#include <ostream>

#include "../core/dimension.hpp"

namespace console
{
    class console_view;

    class console
    {
        std::ostream& output_stream;

    public:
        virtual ~console() = default;

        explicit console(std::ostream& output_stream);

        void clear() const;

        virtual void move_to(int x, int y) const;

        [[nodiscard]] virtual std::shared_ptr<console_view> create_view(const core::position& offset) const;
    };

    class console_view final : public console
    {
        core::position position;

    public:
        console_view(const console& base_console, const core::position& position);

        void move_to(int x, int y) const override;

        [[nodiscard]] std::shared_ptr<console_view> create_view(const core::position& offset) const override;
    };
}
