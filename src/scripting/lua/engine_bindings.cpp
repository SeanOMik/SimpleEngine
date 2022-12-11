#include "scripting/lua/engine_bindings.h"
#include "math/math.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <glm/gtx/string_cast.hpp>
#include <sol/raii.hpp>
#include <sol/types.hpp>

#include <sstream>

namespace simpleengine::scripting::lua {
    sol::table EngineBindings::bind_full_engine(sol::this_state s) {
        bind_math_objects(s);

        return sol::table{};
    }

    void EngineBindings::bind_math_objects(sol::this_state s) {
        sol::state_view lua{s};

        lua.new_usertype<simpleengine::Vec2f>("Vec2",
            "x", &simpleengine::Vec2f::x,
            "y", &simpleengine::Vec2f::y,
            
            sol::call_constructor, sol::factories(
                [](const float& val) {
                    return simpleengine::Vec2f(val);
                },
                [](const float& x, const float& y) {
                    return simpleengine::Vec2f(x, y);
                }
            ),

            sol::meta_function::addition, sol::overload(
                [](const simpleengine::Vec2f& a, const simpleengine::Vec2f& b) {
                    return a + b;
                },
                [](const simpleengine::Vec2f& self, const float& val) {
                    return self + val;
                }
            ),
            sol::meta_function::subtraction, sol::overload(
                [](const simpleengine::Vec2f& a, const simpleengine::Vec2f& b) {
                    return a - b;
                },
                [](const simpleengine::Vec2f& self, const float& val) {
                    return self - val;
                }
            ),
            sol::meta_function::multiplication, sol::overload(
                [](const simpleengine::Vec2f& a, const simpleengine::Vec2f& b) {
                    return a * b;
                },
                [](const simpleengine::Vec2f& self, const float& val) {
                    return self * val;
                }
            ),
            sol::meta_function::division, sol::overload(
                [](const simpleengine::Vec2f& a, const simpleengine::Vec2f& b) {
                    return a / b;
                },
                [](const simpleengine::Vec2f& self, const float& val) {
                    return self / val;
                }
            ),
            sol::meta_function::to_string, [](const simpleengine::Vec2f& self) {
                std::stringstream ss;
                ss << "(" << self.x << ", " << self.y << ")";

                return ss.str();
            }
        );

        lua.new_usertype<simpleengine::Vec3f>("Vec3",
            "x", &simpleengine::Vec3f::x,
            "y", &simpleengine::Vec3f::y,
            "z", &simpleengine::Vec3f::z,

            sol::call_constructor, sol::factories(
                [](const float& val) {
                    return simpleengine::Vec3f(val);
                },
                [](const float& x, const float& y, const float& z) {
                    return simpleengine::Vec3f(x, y, z);
                }
            ),

            sol::meta_function::addition, sol::overload(
                [](const simpleengine::Vec3f& a, const simpleengine::Vec3f& b) {
                    return a + b;
                },
                [](const simpleengine::Vec3f& self, const float& val) {
                    return self + val;
                }
            ),
            sol::meta_function::subtraction, sol::overload(
                [](const simpleengine::Vec3f& a, const simpleengine::Vec3f& b) {
                    return a - b;
                },
                [](const simpleengine::Vec3f& self, const float& val) {
                    return self - val;
                }
            ),
            sol::meta_function::multiplication, sol::overload(
                [](const simpleengine::Vec3f& a, const simpleengine::Vec3f& b) {
                    return a * b;
                },
                [](const simpleengine::Vec3f& self, const float& val) {
                    return self * val;
                }
            ),
            sol::meta_function::division, sol::overload(
                [](const simpleengine::Vec3f& a, const simpleengine::Vec3f& b) {
                    return a / b;
                },
                [](const simpleengine::Vec3f& self, const float& val) {
                    return self / val;
                }
            ),
            sol::meta_function::index, [](const glm::mat3& self, const int& i) {
                return self[i];
            },
            sol::meta_function::to_string, [](const simpleengine::Vec3f& self) {
                std::stringstream ss;
                ss << "(" << self.x << ", " << self.y << ", " << self.z << ")";

                return ss.str();
            }
        );

        lua.new_usertype<simpleengine::Vec4f>("Vec4",
            "w", &simpleengine::Vec4f::w,
            "x", &simpleengine::Vec4f::x,
            "y", &simpleengine::Vec4f::y,
            "z", &simpleengine::Vec4f::z,

            sol::call_constructor, sol::factories(
                [](const float& val) {
                    return simpleengine::Vec4f(val);
                },
                [](const float& x, const float& y, const float& z, const float& w) {
                    return simpleengine::Vec4f(x, y, z, w);
                }
            ),

            sol::meta_function::addition, sol::overload(
                [](const simpleengine::Vec4f& a, const simpleengine::Vec4f& b) {
                    return a + b;
                },
                [](const simpleengine::Vec4f& self, const float& val) {
                    return self + val;
                }
            ),
            sol::meta_function::subtraction, sol::overload(
                [](const simpleengine::Vec4f& a, const simpleengine::Vec4f& b) {
                    return a - b;
                },
                [](const simpleengine::Vec4f& self, const float& val) {
                    return self - val;
                }
            ),
            sol::meta_function::multiplication, sol::overload(
                [](const simpleengine::Vec4f& a, const simpleengine::Vec4f& b) {
                    return a * b;
                },
                [](const simpleengine::Vec4f& self, const float& val) {
                    return self * val;
                }
            ),
            sol::meta_function::division, sol::overload(
                [](const simpleengine::Vec4f& a, const simpleengine::Vec4f& b) {
                    return a / b;
                },
                [](const simpleengine::Vec4f& self, const float& val) {
                    return self / val;
                }
            ),
            sol::meta_function::index, [](const simpleengine::Vec4f& self, const int& i) {
                return self[i];
            },
            sol::meta_function::to_string, [](const simpleengine::Vec4f& self) {
                std::stringstream ss;
                ss << "(" << self.x << ", " << self.y << ", " << self.z << ", " << self.w << ")";

                return ss.str();
            }
        );

        lua.new_usertype<simpleengine::Quat>("Quat",
            "w", &simpleengine::Quat::w,
            "x", &simpleengine::Quat::x,
            "y", &simpleengine::Quat::y,
            "z", &simpleengine::Quat::z,

            sol::call_constructor, sol::factories(
                [](const float& w, const float& x, const float& y, const float& z) {
                    return simpleengine::Quat(w, x, y, z);
                }
            ),

            sol::meta_function::addition, [](const simpleengine::Quat& a, const simpleengine::Quat& b) {
                return a + b;
            },
            sol::meta_function::subtraction, [](const simpleengine::Quat& a, const simpleengine::Quat& b) {
                return a - b;
            },
            sol::meta_function::multiplication, sol::overload(
                [](const simpleengine::Quat& a, const simpleengine::Quat& b) {
                    return a * b;
                },
                [](const simpleengine::Quat& self, const float& val) {
                    return self * val;
                }
            ),
            sol::meta_function::division, [](const simpleengine::Quat& self, const float& val) {
                return self / val;
            },
            sol::meta_function::to_string, [](const simpleengine::Quat& self) {
                std::stringstream ss;
                ss << "(" << self.w << ", " << self.x << ", " << self.y << ", " << self.z << ")";

                return ss.str();
            }
        );

        lua.new_usertype<simpleengine::Mat3f>("Mat3",
            sol::call_constructor, sol::factories(
                [](const float& val) {
                    return simpleengine::Mat3f(val);
                },
                []() {
                    return simpleengine::Mat3f(1.f);
                }
            ),

            sol::meta_function::addition, sol::overload(
                [](const glm::mat3& a, const glm::mat3& b) {
                    return a + b;
                },
                [](const glm::mat3& self, const float& val) {
                    return self + val;
                }
            ),
            sol::meta_function::subtraction, sol::overload(
                [](const glm::mat3& a, const glm::mat3& b) {
                    return a - b;
                },
                [](const glm::mat3& self, const float& val) {
                    return self - val;
                }
            ),
            sol::meta_function::multiplication, sol::overload(
                [](const glm::mat3& a, const glm::mat3& b) {
                    return a * b;
                },
                [](const glm::mat3& self, const float& val) {
                    return self * val;
                }
            ),
            sol::meta_function::division, sol::overload(
                [](const glm::mat3& a, const glm::mat3& b) {
                    return a / b;
                },
                [](const glm::mat3& self, const float& val) {
                    return self / val;
                }
            ),
            sol::meta_function::index, [](const glm::mat3& self, const int& i) {
                return self[i];
            },
            sol::meta_function::to_string, [](const glm::mat3& self) {
                return glm::to_string(self);
            }
        );

        lua.new_usertype<simpleengine::Mat4f>("Mat4",
            sol::call_constructor, sol::factories(
                [](const float& val) {
                    return simpleengine::Mat4f(val);
                },
                []() {
                    return simpleengine::Mat4f(1.f);
                }
            ),

            sol::meta_function::addition, sol::overload(
                [](const simpleengine::Mat4f& a, const simpleengine::Mat4f& b) {
                    return a + b;
                },
                [](const simpleengine::Mat4f& self, const float& val) {
                    return self + val;
                }
            ),
            sol::meta_function::subtraction, sol::overload(
                [](const simpleengine::Mat4f& a, const simpleengine::Mat4f& b) {
                    return a - b;
                },
                [](const simpleengine::Mat4f& self, const float& val) {
                    return self - val;
                }
            ),
            sol::meta_function::multiplication, sol::overload(
                [](const simpleengine::Mat4f& a, const simpleengine::Mat4f& b) {
                    return a * b;
                },
                [](const simpleengine::Mat4f& self, const float& val) {
                    return self * val;
                }
            ),
            sol::meta_function::division, sol::overload(
                [](const simpleengine::Mat4f& a, const simpleengine::Mat4f& b) {
                    return a / b;
                },
                [](const simpleengine::Mat4f& self, const float& val) {
                    return self / val;
                }
            ),
            sol::meta_function::index, [](const simpleengine::Mat4f& self, const int& i) {
                return self[i];
            },
            sol::meta_function::to_string, [](const simpleengine::Mat4f& self) {
                return glm::to_string(self);
            }
        );
    }
}