#pragma once

#include "vulki_model.h"

// std
#include <memory>

namespace VULKI {

    struct Transform2dComponent {
        glm::vec2 translation{};  // (position offset)
        glm::vec2 scale{ 1.f, 1.f };
        float rotation;

        glm::mat2 mat2() {
            const float s = glm::sin(rotation);
            const float c = glm::cos(rotation);
            glm::mat2 rotMatrix{ {c, s}, {-s, c} };

            glm::mat2 scaleMat{ {scale.x, .0f}, {.0f, scale.y} };
            return rotMatrix * scaleMat;
        }
    };

    class VulkiGameObject {
    public:
        using id_t = unsigned int;

        static VulkiGameObject createGameObject() {
            static id_t currentId = 0;
            return VulkiGameObject{ currentId++ };
        }

        VulkiGameObject(const VulkiGameObject&) = delete;
        VulkiGameObject& operator=(const VulkiGameObject&) = delete;
        VulkiGameObject(VulkiGameObject&&) = default;
        VulkiGameObject& operator=(VulkiGameObject&&) = default;

        id_t getId() { return id; }

        std::shared_ptr<VulkiModel> model{};
        glm::vec3 color{};
        Transform2dComponent transform2d{};

    private:
        VulkiGameObject(id_t objId) : id{ objId } {}

        id_t id;
    };
}  // namespace lve