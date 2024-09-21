#pragma once

#include "vulki_model.h"

#include <glm/gtc/matrix_transform.hpp>
// std
#include <memory>
#include <unordered_map>

namespace VULKI {

    struct TransformComponent {
        glm::vec3 translation{};  // (position offset)
        glm::vec3 scale{ 1.f, 1.f, 1.f };
        glm::vec3 rotation{};

        // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
        // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
        // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
        glm::mat4 mat4();
        glm::mat3 normalMatrix();

    };

    class VulkiGameObject {
    public:
        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, VulkiGameObject>;

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
        TransformComponent transform{};

    private:
        VulkiGameObject(id_t objId) : id{ objId } {}

        id_t id;
    };
}  