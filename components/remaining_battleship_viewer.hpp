#pragma once

#include "../core/component.hpp"
#include "../models/battleship.hpp"

namespace components
{
    class remaining_battleship_viewer final : public core::component
    {
        const std::vector<models::battleship> available_battleships;
        const std::vector<models::battleship>& destroyed_battleships;

        [[nodiscard]] static bool is_battleship_present(const models::battleship&,
                                                        const std::vector<models::battleship>&);

        void paint_battleship(int y, models::battleship, bool is_destroyed) const;
        void paint_battleships() const;

    public:
        remaining_battleship_viewer(int x, int y, const std::shared_ptr<console::console>& console,
                                    const std::vector<models::battleship>& available_battleships,
                                    const std::vector<models::battleship>& destroyed_battleships);

        void paint() override;
    };
}
